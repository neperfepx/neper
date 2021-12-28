---
layout: default
title: Downloads
nav_order: 7
---

# Downloads

Neper is distributed as a free / open-source software, under the terms of the [GNU General Public License (GPL)](http://www.gnu.org/licenses/gpl.html). In short, this means that everyone is free to use Neper and to redistribute it on a free basis. Neper is not in the public domain; it is copyrighted and there are restrictions on its distribution (see the [license](http://www.gnu.org/licenses/gpl.html) and the related [FAQ](http://www.gnu.org/copyleft/gpl-faq.html)).

It you use Neper for your own work, please mention it explicitly and cite one or several of the papers given in the Reference papers section below.  The papers are available for download.

## Current official release

- The current official release (and previous ones) is available on the [GitHub repository](https://github.com/rquey/neper/releases).   Older versions are archived [here](old-versions.md).

- The PDF documentation is available [here](docs/neper.pdf).

## Continuous-integration versions

New versions of the code are published in-between official releases and are available on the [GitHub repository](https://github.com/rquey/neper).  This is also the place for interactions, see the [community](community.md) page.

## Supplementary software

Neper's dependencies include
  [GSL](http://www.gnu.org/software/gsl/),
  [muParser](http://beltoforion.de/article.php?a=muparser) (included in Neper),
  [nanoflann](https://github.com/jlblancoc/nanoflann) (included in Neper),
  [openGJK](https://github.com/MattiaMontanari/openGJK) (included in Neper),
  [NLopt](https://nlopt.readthedocs.io) (included in Neper),
  [tinycolormap](https://github.com/yuki-koyama/tinycolormap) (included in Neper),
  [Gmsh](http://gmsh.info/),
  [libScotch](http://www.labri.fr/perso/pelegrin/scotch) (included in Neper) and
  [POV-Ray](http://www.povray.org).

## Reference papers

- Romain Quey, Paul Dawson and Fabrice Barbe, Large-scale 3D random polycrystals for the finite element method: Generation, meshing and remeshing, Comput. Methods Appl. Mech. Engrg., vol. 200, pp. 1729-1745, 2011: [paper](docs/neper-reference-paper.pdf)

- Romain Quey and Loïc Renversade, Optimal polyhedral description of 3D polycrystals: Method and application to statistical and synchrotron X-ray diffraction data, Comput. Methods Appl. Mech. Engrg., vol. 330, pp. 308-333, 2018: [postprint](https://hal.archives-ouvertes.fr/hal-01626440) [link](https://www.sciencedirect.com/science/article/pii/S0045782517307028)

- Romain Quey, Aurelien Villani and Claire Maurice, Nearly uniform sampling of crystal orientations. J. Appl. Crystallogr., vol. 51, pp. 1162-1173, 2018. [reprint](quey-villani-maurice-JAC-2018.pdf) [link](http://scripts.iucr.org/cgi-bin/paper?buy=yes&cnor=ks5599&showscheme=yes)

## Other resources (unofficial)

- Videos are available [here](https://www.youtube.com/watch?v=ImOU7ZysEH4&list=PLct8iNZXls-BMU7aleWoSoxgD8OFFe48W) and constitute a good first introduction to Neper.
- [Phon](https://phon.readthedocs.io): insert interface elements and export to Abaqus or OOFEM. *Note: interface elements can be inserted directly in Neper using `-interface cohesive`.*
- [Neper2CAE](https://github.com/ngrilli/Neper2CAE) and [PyCiGen](https://github.com/ngrilli/PyCiGen): generate a columnar grain-growth polycrystal, insert interface elements and export to Abaqus (hex elements)
