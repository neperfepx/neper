/*
                 __________                                      
    _____   __ __\______   \_____  _______  ______  ____ _______ 
   /     \ |  |  \|     ___/\__  \ \_  __ \/  ___/_/ __ \\_  __ \
  |  Y Y  \|  |  /|    |     / __ \_|  | \/\___ \ \  ___/ |  | \/
  |__|_|  /|____/ |____|    (____  /|__|  /____  > \___  >|__|   
        \/                       \/            \/      \/        
  Copyright (C) 2004-2011 Ingo Berg

  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software
  without restriction, including without limitation the rights to use, copy, modify, 
  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
  permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or 
  substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
*/ 
  
  
#include "muParserCallback.h"
  
/** \file
    \brief Implementation of the parser callback class.
*/ 
  
{
  
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type0 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
  ParserCallback::ParserCallback (fun_type1 a_pFun, bool a_bAllowOpti,
                                    int a_iPrec,
                                    ECmdCode a_iCode) 
                                                                a_pFun) 
    m_iArgc (1) 
    m_iCode (a_iCode) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Constructor for constructing function callbacks taking two arguments. 
      \throw nothrow
  */ 
ParserCallback::ParserCallback (fun_type2 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Constructor for constructing binary operator callbacks. 
      \param a_pFun Pointer to a static function taking two arguments
      \param a_bAllowOpti A flag indicating this function can be optimized
      \param a_iPrec The operator precedence
      \param a_eOprtAsct The operators associativity
      \throw nothrow
  */ 
  ParserCallback::ParserCallback (fun_type2 a_pFun, 
                                    
                                    
    
    m_eOprtAsct (a_eOprtAsct) 
    m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type3 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type4 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type5 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type6 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type7 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type8 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type9 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (fun_type10 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type0 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type1 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Constructor for constructing function callbacks taking two arguments. 
      \throw nothrow
  */ 
ParserCallback::ParserCallback (bulkfun_type2 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type3 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type4 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type5 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type6 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type7 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type8 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type9 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (bulkfun_type10 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (multfun_type a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (strfun_type1 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (strfun_type2 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
ParserCallback::ParserCallback (strfun_type3 a_pFun, bool a_bAllowOpti) 
  m_pFun ((void *) a_pFun) 
  m_eOprtAsct (oaNONE) 
  m_bAllowOpti (a_bAllowOpti) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Default constructor. 
      \throw nothrow
  */ 
ParserCallback::ParserCallback () 
  m_pFun (0) 
  m_iCode (cmUNKNOWN) 
  {
  } 
    //---------------------------------------------------------------------------
  /** \brief Copy constructor. 
      \throw nothrow
  */ 
    ParserCallback::ParserCallback (const ParserCallback & ref) 
  {
    
    
    
    
    
    
    
  
    //---------------------------------------------------------------------------
  /** \brief Clone this instance and return a pointer to the new instance. */ 
  ParserCallback * ParserCallback::Clone ()const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return tru if the function is conservative.

      Conservative functions return always the same result for the same argument.
      \throw nothrow
  */ 
    bool ParserCallback::IsOptimizable () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Get the callback address for the parser function. 
  
      The type of the address is void. It needs to be recasted according to the
      argument number to the right type.

      \throw nothrow
      \return #pFun
  */ 
  void *ParserCallback::GetAddr () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return the callback code. */ 
    ECmdCode ParserCallback::GetCode () const 
  {
    
  
   
    //---------------------------------------------------------------------------
    ETypeCode ParserCallback::GetType () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return the operator precedence. 
      \throw nothrown

     Only valid if the callback token is an operator token (binary or infix).
  */ 
  int ParserCallback::GetPri () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Return the operators associativity. 
      \throw nothrown

     Only valid if the callback token is a binary operator token.
  */ 
    EOprtAssociativity ParserCallback::GetAssociativity () const 
  {
    
  
   
    //---------------------------------------------------------------------------
  /** \brief Returns the number of function Arguments. */ 
  int ParserCallback::GetArgc () const 
  {
    
  
 