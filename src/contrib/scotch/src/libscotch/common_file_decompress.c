/* Copyright 2008,2010,2015,2018,2020 IPB, Universite de Bordeaux, INRIA & CNRS
**
** This file is part of the Scotch software package for static mapping,
** graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-C license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
**
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : common_file_decompress.c                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles compressed streams  **/
/**                for decompression.                      **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 11 mar 2008     **/
/**                                 to   : 15 may 2008     **/
/**                # Version 5.1  : from : 27 jun 2010     **/
/**                                 to   : 27 jun 2010     **/
/**                # Version 6.0  : from : 27 apr 2015     **/
/**                                 to   : 30 aug 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define COMMON_FILE
#define COMMON_FILE_DECOMPRESS

#ifndef COMMON_NOMODULE
#include "module.h"
#endif /* COMMON_NOMODULE */
#include "common.h"
#include "common_file.h"
#include "common_file_compress.h"
#ifdef COMMON_FILE_COMPRESS_BZ2
#include "bzlib.h"
#endif /* COMMON_FILE_COMPRESS_BZ2 */
#ifdef COMMON_FILE_COMPRESS_GZ
#include "zlib.h"
#endif /* COMMON_FILE_COMPRESS_GZ */
#ifdef COMMON_FILE_COMPRESS_LZMA
#include "lzma.h"
#endif /* COMMON_FILE_COMPRESS_LZMA */

/*
**  The static definitions.
*/

static FileCompressTab      filetab[] = {
#ifdef COMMON_FILE_COMPRESS_BZ2
                                          { ".bz2",  FILECOMPRESSTYPEBZ2,    },
#else /* COMMON_FILE_COMPRESS_BZ2 */
                                          { ".bz2",  FILECOMPRESSTYPENOTIMPL },
#endif /* COMMON_FILE_COMPRESS_BZ */
#ifdef COMMON_FILE_COMPRESS_GZ
                                          { ".gz",   FILECOMPRESSTYPEGZ,     },
#else /* COMMON_FILE_COMPRESS_GZ */
                                          { ".gz",   FILECOMPRESSTYPENOTIMPL },
#endif /* COMMON_FILE_COMPRESS_GZ */
#ifdef COMMON_FILE_COMPRESS_LZMA
                                          { ".lzma", FILECOMPRESSTYPELZMA    },
                                          { ".xz",   FILECOMPRESSTYPELZMA    },
#else /* COMMON_FILE_COMPRESS_LZMA */
                                          { ".lzma", FILECOMPRESSTYPENOTIMPL },
                                          { ".xz",   FILECOMPRESSTYPENOTIMPL },
#endif /* COMMON_FILE_COMPRESS_LZMA */
                                          { NULL,    FILECOMPRESSTYPENOTIMPL } };

/*********************************/
/*                               */
/* Basic routines for filenames. */
/*                               */
/*********************************/

/* This routine searches the given file name
** for relevant extensions and returns the
** corresponding code if it is the case.
** It returns:
** - FILECOMPRESSTYPENONE     : no recognized file extension.
** - FILECOMPRESSTYPENOTIMPL  : compression algorithm not implemented.
** - FILECOMPRESSTYPExxxx     : implemented compression algorithm.
*/

int
fileDecompressType (
const char * const          nameptr)              /*+ Name string +*/
{
  int                 namelen;
  int                 i;

  namelen = strlen (nameptr);
  for (i = 0; filetab[i].name != NULL; i ++) {
    int                 extnlen;                  /* Name of extension string */

    extnlen = strlen (filetab[i].name);
    if ((namelen >= extnlen) && (strncmp (filetab[i].name, nameptr + (namelen - extnlen), extnlen) == 0))
      return (filetab[i].type);
  }

  return (FILECOMPRESSTYPENONE);
}

/* This routine creates a thread to decompress the
** given stream according to the given (un)compression
** algorithm.
** If threads are available, decompression will be
** performed by an auxiliary thread. Else, a child process
** will be fork()'ed, and after completion this process
** will remain a zombie until the main process terminates.
** It returns:
** - !NULL  : stream holding decompressed data.
** - NULL   : on error.
*/

static
void *                                            /* (void *) to comply to the Posix pthread API */
fileDecompress2 (
FileCompress * const        compptr)
{
  switch (compptr->typeval) {
#ifdef COMMON_FILE_COMPRESS_BZ2
    case FILECOMPRESSTYPEBZ2 :
      fileDecompressBz2 (compptr);
      break;
#endif /* COMMON_FILE_COMPRESS_BZ2 */
#ifdef COMMON_FILE_COMPRESS_GZ
    case FILECOMPRESSTYPEGZ :
      fileDecompressGz (compptr);
      break;
#endif /* COMMON_FILE_COMPRESS_GZ */
#ifdef COMMON_FILE_COMPRESS_LZMA
    case FILECOMPRESSTYPELZMA :
      fileDecompressLzma (compptr);
      break;
#endif /* COMMON_FILE_COMPRESS_LZMA */
    default :
      errorPrint ("fileDecompress2: method not implemented");
  }

  close   (compptr->infdnum);                     /* Close writer's end */
  memFree (compptr->bufftab);                     /* Free data buffer   */
#ifdef COMMON_DEBUG
  compptr->bufftab = NULL;
#endif /* COMMON_DEBUG */

  return (NULL);                                  /* Don't care anyway */
}

int
fileDecompress (
File * const                fileptr,              /*+ Compressed input stream   +*/
const int                   typeval)              /*+ (Un)compression algorithm +*/
{
  int                 filetab[2];
  FILE *              readptr;
  FileCompress *      compptr;

  if (typeval <= FILECOMPRESSTYPENONE)            /* If nothing to do */
    return (0);

  if (pipe (filetab) != 0) {
    errorPrint ("fileDecompress: cannot create pipe");
    return (1);
  }

  if ((readptr = fdopen (filetab[0], "r")) == NULL) { /* New stream master will read from */
    errorPrint ("fileDecompress: cannot create stream");
    close  (filetab[0]);
    close  (filetab[1]);
    return (1);
  }

  if (((compptr = memAlloc (sizeof (FileCompress))) == NULL) || /* Compression structure to be freed by master */
      ((compptr->bufftab = memAlloc (FILECOMPRESSDATASIZE)) == NULL)) {
    errorPrint ("fileDecompress: out of memory");
    if (compptr != NULL)
      memFree (compptr);
    fclose (readptr);
    close  (filetab[1]);
    return (1);
  }

  compptr->typeval = typeval;                     /* Fill structure to be passed to decompression thread/process */
  compptr->infdnum = filetab[1];
  compptr->oustptr = fileptr->fileptr;            /* Compressed stream to read from */

#ifdef COMMON_PTHREAD_FILE
  if (pthread_create (&compptr->thrdval, NULL, (void * (*) (void *)) fileDecompress2, (void *) compptr) != 0) { /* If could not create thread */
    errorPrint ("fileDecompress: cannot create thread");
    memFree (compptr->bufftab);
    memFree (compptr);
    fclose  (readptr);
    close   (filetab[1]);
    return  (1);
  }
#else /* COMMON_PTHREAD_FILE */
  switch (compptr->procval = fork ()) {
    case -1 :                                     /* Error */
      errorPrint ("fileDecompress: cannot create child process");
      memFree (compptr->bufftab);
      memFree (compptr);
      fclose  (readptr);
      close   (filetab[1]);
      return  (1);
    case 0 :                                      /* We are the son process    */
      fclose (readptr);                           /* Close reader pipe stream  */
      fileDecompress2 (compptr);                  /* Perform decompression     */
      exit (EXIT_SUCCESS);                        /* Exit gracefully           */
    default :                                     /* We are the father process */
      close (filetab[1]);                         /* Close the writer pipe end */
  }
#endif /* COMMON_PTHREAD_FILE */

  fileptr->fileptr = readptr;                     /* Master can read from pipe */
  fileptr->compptr = compptr;

  return (0);
}

/* This routine decompresses a stream compressed
** in the bzip2 format.
** It returns:
** - void  : in all cases. Decompression stops
**           immediately in case of error.
*/

#ifdef COMMON_FILE_COMPRESS_BZ2
static
void
fileDecompressBz2 (
FileCompress * const        compptr)
{
  BZFILE *            decoptr;
  int                 bytenbr;
  int                 flagval;

  if (FILECOMPRESSDATASIZE < (BZ_MAX_UNUSED)) {
    errorPrint ("fileDecompressBz2: cannot start decompression (1)");
    return;
  }
  if ((decoptr = BZ2_bzReadOpen (&flagval, compptr->oustptr, 0, 0, NULL, 0)) == NULL) {
    errorPrint ("fileDecompressBz2: cannot start decompression (2)");
    BZ2_bzReadClose (&flagval, decoptr);
    return;
  }

  while ((bytenbr = BZ2_bzRead (&flagval, decoptr, compptr->bufftab, FILECOMPRESSDATASIZE), flagval) >= BZ_OK) { /* If BZ_OK or BZ_STREAM_END */
    if (write (compptr->infdnum, compptr->bufftab, bytenbr) != bytenbr) {
      errorPrint ("fileDecompressBz2: cannot write");
      flagval = BZ_STREAM_END;                    /* Avoid other error message */
      break;
    }
    if (flagval == BZ_STREAM_END) {               /* If end of compressed stream */
      void *              byunptr;
      int                 byunnbr;

      BZ2_bzReadGetUnused (&flagval, decoptr, &byunptr, &byunnbr); /* Get remaining chars in stream */
      if ((byunnbr == 0) && (feof (compptr->oustptr) != 0)) { /* If end of decompressed stream too  */
        flagval = BZ_STREAM_END;
        break;
      }
      memMov (compptr->bufftab, byunptr, byunnbr);
      BZ2_bzReadClose (&flagval, decoptr);
      if ((decoptr = BZ2_bzReadOpen (&flagval, compptr->oustptr, 0, 0, compptr->bufftab, byunnbr)) == NULL) {
        errorPrint ("fileDecompressBz2: cannot start decompression (3)");
        flagval = BZ_STREAM_END;
        break;
      }
    }
  }
  if (flagval != BZ_STREAM_END)
    errorPrint ("fileDecompressBz2: cannot read");

  BZ2_bzReadClose (&flagval, decoptr);
  fclose (compptr->oustptr);                      /* Do as zlib does */
}
#endif /* COMMON_FILE_COMPRESS_BZ2 */

/* This routine decompresses a stream compressed
** in the gzip format.
** It returns:
** - void  : in all cases. Decompression stops
**           immediately in case of error.
*/

#ifdef COMMON_FILE_COMPRESS_GZ
static
void
fileDecompressGz (
FileCompress * const        compptr)
{
  gzFile              decoptr;
  int                 bytenbr;

  if ((decoptr = gzdopen (fileno (compptr->oustptr), "rb")) == NULL) {
    errorPrint ("fileDecompressGz: cannot start decompression");
    return;
  }

  while ((bytenbr = gzread (decoptr, compptr->bufftab, FILECOMPRESSDATASIZE)) > 0) {
    if (write (compptr->infdnum, compptr->bufftab, bytenbr) != bytenbr) {
      errorPrint ("fileDecompressGz: cannot write");
      break;
    }
  }
  if (bytenbr < 0)
    errorPrint ("fileDecompressGz: cannot read");

  gzclose (decoptr);
}
#endif /* COMMON_FILE_COMPRESS_GZ */

/* This routine decompresses a stream compressed
** in the lzma format.
** It returns:
** - void  : in all cases. Decompression stops
**           immediately in case of error.
*/

#ifdef COMMON_FILE_COMPRESS_LZMA
static
void
fileDecompressLzma (
FileCompress * const        compptr)
{
  lzma_stream         decodat = LZMA_STREAM_INIT; /* Decoder data          */
  lzma_action         deacval;                    /* Decoder action value  */
  lzma_ret            dereval;                    /* Decoder return value  */
  byte *              obuftab;                    /* Decoder output buffer */

  if ((obuftab = memAlloc (FILECOMPRESSDATASIZE)) == NULL) {
    errorPrint ("fileDecompressLzma: out of memory");
    return;
  }

  if (lzma_stream_decoder (&decodat, UINT64_MAX, LZMA_CONCATENATED) != LZMA_OK) {
    errorPrint ("fileDecompressLzma: cannot start decompression");
    memFree    (obuftab);
    return;
  }

  deacval           = LZMA_RUN;
  dereval           = LZMA_OK;
  decodat.avail_in  = 0;
  decodat.next_out  = obuftab;
  decodat.avail_out = FILECOMPRESSDATASIZE;
  do {
    if ((decodat.avail_in == 0) && (deacval == LZMA_RUN)) {
      ssize_t             bytenbr;

      bytenbr = fread (compptr->bufftab, 1, FILECOMPRESSDATASIZE, compptr->oustptr); /* Read from pipe */
      if (ferror (compptr->oustptr)) {
        errorPrint ("fileDecompressLzma: cannot read");
        break;
      }
      if (bytenbr == 0)
        deacval = LZMA_FINISH;                    /* If end of stream, request completion of decoding */
      decodat.next_in  = compptr->bufftab;
      decodat.avail_in = bytenbr;
    }

    dereval = lzma_code (&decodat, deacval);

    if ((decodat.avail_out == 0) || (dereval == LZMA_STREAM_END)) { /* Write when output buffer full or end of encoding */
      size_t              obufnbr;

      obufnbr = FILECOMPRESSDATASIZE - decodat.avail_out; /* Compute number of bytes to write */
      if (write (compptr->infdnum, obuftab, obufnbr) != obufnbr) {
        errorPrint ("fileDecompressLzma: cannot write");
        break;
      }
      decodat.next_out  = obuftab;
      decodat.avail_out = FILECOMPRESSDATASIZE;
    }
  } while (dereval == LZMA_OK);

  lzma_end (&decodat);
  memFree  (obuftab);

  fclose (compptr->oustptr);                      /* Do as zlib does */
}
#endif /* COMMON_FILE_COMPRESS_LZMA */
