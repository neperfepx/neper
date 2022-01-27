/*
                 __________                                      
    _____   __ __\______   \_____  _______  ______  ____ _______ 
   /     \ |  |  \|     ___/\__  \ \_  __ \/  ___/_/ __ \\_  __ \
  |  Y Y  \|  |  /|    |     / __ \_|  | \/\___ \ \  ___/ |  | \/
  |__|_|  /|____/ |____|    (____  /|__|  /____  > \___  >|__|   
        \/                       \/            \/      \/        
  Copyright (C) 2011 Ingo Berg

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
  
  
#include "muParserInt.h"
  
#include <cmath>
#include <algorithm>
#include <numeric>
  

/** \file
    \brief Implementation of a parser using integer value.
*/ 
  
/** \brief Namespace for mathematical applications. */ 
  namespace mu 
{
  
  {
    return (value_type) Round (fabs ((double) v));
  } 
  {
    return (Round (v) < 0) ? -1 : (Round (v) > 0) ? 1 : 0;
  }
  
  {
    return (Round (v1) == 1) ? Round (v2) : Round (v3);
  }
  
  {
    return Round (v1) + Round (v2);
  }
  
  {
    return Round (v1) - Round (v2);
  }
  
  {
    return Round (v1) * Round (v2);
  }
  
  {
    return Round (v1) / Round (v2);
  }
  
  {
    return Round (v1) % Round (v2);
  }
  
  {
    return Round (v1) >> Round (v2);
  }
  
  {
    return Round (v1) << Round (v2);
  }
  
  {
    return Round (v1) & Round (v2);
  }
  
  {
    return Round (v1) | Round (v2);
  }
  
  {
    return Round (v1) && Round (v2);
  }
  
  {
    return Round (v1) || Round (v2);
  }
  
  {
    return Round (v1) < Round (v2);
  }
  
  {
    return Round (v1) > Round (v2);
  }
  
  {
    return Round (v1) <= Round (v2);
  }
  
  {
    return Round (v1) >= Round (v2);
  }
  
  {
    return Round (v1) == Round (v2);
  }
  
  {
    return Round (v1) != Round (v2);
  }
  
  {
    return !Round (v);
  }
  
  {
    
  
//---------------------------------------------------------------------------
// Unary operator Callbacks: Infix operators
    value_type ParserInt::UnaryMinus (value_type v) 
  {
    
  
  
//---------------------------------------------------------------------------
    value_type ParserInt::Sum (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      
    
  
  
//---------------------------------------------------------------------------
    value_type ParserInt::Min (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      
    
  
  
//---------------------------------------------------------------------------
    value_type ParserInt::Max (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      
    
  
  
//---------------------------------------------------------------------------
// Default value recognition callback
  int ParserInt::IsVal (const char_type * a_szExpr, int *a_iPos,
                        value_type * a_fVal) 
  {
    
    
    
      
    
    
    
    
      
    
    if (stream.fail ())
      
    
      
    
    
    
  
  
//---------------------------------------------------------------------------
/** \brief Check a given position in the expression for the presence of 
           a hex value. 
    \param a_szExpr Pointer to the expression string
    \param [in/out] a_iPos Pointer to an integer value holding the current parsing 
           position in the expression.
    \param [out] a_fVal Pointer to the position where the detected value shall be stored.

  Hey values must be prefixed with "0x" in order to be detected properly.
*/ 
  int ParserInt::IsHexVal (const char_type * a_szExpr, int *a_iPos,
                           value_type * a_fVal) 
  {
    
      
    
    
      // New code based on streams for UNICODE compliance:
      stringstream_type::pos_type nPos (0);
    
    
    
    
      
    
    
    
  
  
//---------------------------------------------------------------------------
  int ParserInt::IsBinVal (const char_type * a_szExpr, int *a_iPos,
                           value_type * a_fVal) 
  {
    
      
    
    
            (a_szExpr[i + 1] == '0' || a_szExpr[i + 1] == '1') && i < iBits;
            ++i)
      
    
      
    
      
        exception_type (_T
                        ("Binary to integer conversion error (overflow)."));
    
    
    
  
  
//---------------------------------------------------------------------------
/** \brief Constructor. 

  Call ParserBase class constructor and trigger Function, Operator and Constant initialization.
*/ 
ParserInt::ParserInt () 
  {
    
    AddValIdent (IsBinVal);
    
    
    
    
  
  
//---------------------------------------------------------------------------
  void ParserInt::InitConst () 
  {
  
//---------------------------------------------------------------------------
  void ParserInt::InitCharSets () 
  {
    
                      ("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    
    
  
//---------------------------------------------------------------------------
/** \brief Initialize the default functions. */ 
  void ParserInt::InitFun () 
  {
    
    
    
    
    
    
  
//---------------------------------------------------------------------------
/** \brief Initialize operators. */ 
  void ParserInt::InitOprt () 
  {
    
      // disable all built in operators, not all of them useful for integer numbers
      // (they don't do rounding of values)
      EnableBuiltInOprt (false);
    
      // Disable all built in operators, they wont work with integer numbers
      // since they are designed for floating point numbers
      DefineInfixOprt (_T ("-"), UnaryMinus);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
