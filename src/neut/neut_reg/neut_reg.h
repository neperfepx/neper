/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_reg.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_REG_H
#define NEUT_REG_H

  extern void net_reg_set_zero (struct REG *pReg);
  extern void net_reg_free (struct REG Reg);

#endif				/* NEUT_REG_H */

#ifdef __cplusplus
}
#endif
