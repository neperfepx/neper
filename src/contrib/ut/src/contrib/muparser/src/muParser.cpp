/* 
                 __________                                      
    _____   __ __\______   \_____  _______  ______  ____ _______ 
   /     \ |  |  \|     ___/\__  \ \_  __ \/  ___/_/ __ \\_  __ \
  |  Y Y  \|  |  /|    |     / __ \_|  | \/\___ \ \  ___/ |  | \/
  |__|_|  /|____/ |____|    (____  /|__|  /____  > \___  >|__|   
        \/                       \/            \/      \/        

  Copyright (C) 2013 Ingo Berg

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
  
#include "muParser.h"
#include "muParserTemplateMagic.h"
  
//--- Standard includes ------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <numeric>
  
/** \brief Pi (what else?). */ 
#define PARSER_CONST_PI  3.141592653589793238462643
  
/** \brief The Eulerian number. */ 
#define PARSER_CONST_E   2.718281828459045235360287
  

/** \file
    \brief Implementation of the standard floating point parser.
*/ 
  
/** \brief Namespace for mathematical applications. */ 
  namespace mu 
{
  
    //---------------------------------------------------------------------------
    // Trigonometric function
    value_type Parser::Sin (value_type v)
  {
    return MathImpl < value_type >::Sin (v);
  }
  
  {
    return MathImpl < value_type >::Cos (v);
  }
  
  {
    return MathImpl < value_type >::Tan (v);
  }
  
  {
    return MathImpl < value_type >::ASin (v);
  }
  
  {
    return MathImpl < value_type >::ACos (v);
  }
  
  {
    return MathImpl < value_type >::ATan (v);
  }
  
  {
    return MathImpl < value_type >::ATan2 (v1, v2);
  }
  
  {
    return MathImpl < value_type >::Sinh (v);
  }
  
  {
    return MathImpl < value_type >::Cosh (v);
  }
  
  {
    return MathImpl < value_type >::Tanh (v);
  }
  
  {
    return MathImpl < value_type >::ASinh (v);
  }
  
  {
    return MathImpl < value_type >::ACosh (v);
  }
  
  {
    return MathImpl < value_type >::ATanh (v);
  }
  
    //---------------------------------------------------------------------------
    // Logarithm functions
    
    // Logarithm base 2
    value_type Parser::Log2 (value_type v) 
  {
    
#ifdef MUP_MATH_EXCEPTIONS
      if (v <= 0)
      
    
#endif  /* 
      
  
  
    // Logarithm base 10
    value_type Parser::Log10 (value_type v) 
  {
    
#ifdef MUP_MATH_EXCEPTIONS
      if (v <= 0)
      
    
#endif  /* 
      
  
  
// Logarithm base e (natural logarithm)
    value_type Parser::Ln (value_type v) 
  {
    
#ifdef MUP_MATH_EXCEPTIONS
      if (v <= 0)
      
    
#endif  /* 
      
  
  
    //---------------------------------------------------------------------------
    //  misc
    value_type Parser::Exp (value_type v)
  {
    return MathImpl < value_type >::Exp (v);
  }
  
  {
    return MathImpl < value_type >::Abs (v);
  }
  
  {
    
#ifdef MUP_MATH_EXCEPTIONS
      if (v < 0)
      
    
#endif  /* 
      
  
  
  {
    return MathImpl < value_type >::Rint (v);
  }
  
  {
    return MathImpl < value_type >::Sign (v);
  }
  
    //---------------------------------------------------------------------------
  /** \brief Callback for the unary minus operator.
      \param v The value to negate
      \return -v
  */ 
    value_type Parser::UnaryMinus (value_type v) 
  {
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Callback for the unary minus operator.
      \param v The value to negate
      \return -v
  */ 
    value_type Parser::UnaryPlus (value_type v) 
  {
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Callback for adding multiple values. 
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */ 
    value_type Parser::Sum (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      fRes += a_afArg[i];
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Callback for averaging multiple values. 
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */ 
    value_type Parser::Avg (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      fRes += a_afArg[i];
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Callback for determining the minimum value out of a vector. 
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */ 
    value_type Parser::Min (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Callback for determining the maximum value out of a vector. 
      \param [in] a_afArg Vector with the function arguments
      \param [in] a_iArgc The size of a_afArg
  */ 
    value_type Parser::Max (const value_type * a_afArg, int a_iArgc) 
  {
    
      
    
    
      fRes = std::max (fRes, a_afArg[i]);
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Default value recognition callback. 
      \param [in] a_szExpr Pointer to the expression
      \param [in, out] a_iPos Pointer to an index storing the current position within the expression
      \param [out] a_fVal Pointer where the value should be stored in case one is found.
      \return 1 if a value was found 0 otherwise.
  */ 
  int Parser::IsVal (const char_type * a_szExpr, int *a_iPos,
                     value_type * a_fVal) 
  {
    
    
    
    stream.imbue (Parser::s_locale);
    
    
    
      
    
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Constructor. 

    Call ParserBase class constructor and trigger Function, Operator and Constant initialization.
  */ 
Parser::Parser () 
  {
    
    
    
    
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Define the character sets. 
      \sa DefineNameChars, DefineOprtChars, DefineInfixOprtChars
    
    This function is used for initializing the default character sets that define
    the characters to be useable in function and variable names and operators.
  */ 
  void Parser::InitCharSets () 
  {
    
                      ("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    
                      ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*^/?<>=#!$%&|~'_{}"));
    
  
    //---------------------------------------------------------------------------
  /** \brief Initialize the default functions. */ 
  void Parser::InitFun () 
  {
    
      
    {
      
        // When setting MUP_BASETYPE to an integer type
        // Place functions for dealing with integer values here
        // ...
        // ...
        // ...
    }
    
    else
      
    {
      
        // trigonometric functions
        DefineFun (_T ("sin"), Sin);
      
      
      
        // arcus functions
        DefineFun (_T ("asin"), ASin);
      
      
      
      
        // hyperbolic functions
        DefineFun (_T ("sinh"), Sinh);
      
      
      
        // arcus hyperbolic functions
        DefineFun (_T ("asinh"), ASinh);
      
      
      
        // Logarithm functions
        DefineFun (_T ("log2"), Log2);
      
      
      
      
        // misc
        DefineFun (_T ("exp"), Exp);
      
      
      
      
      
        // Functions with variable number of arguments
        DefineFun (_T ("sum"), Sum);
      
      
      
    
  
  
    //---------------------------------------------------------------------------
  /** \brief Initialize constants.
  
    By default the parser recognizes two constants. Pi ("pi") and the Eulerian
    number ("_e").
  */ 
  void Parser::InitConst () 
  {
    
    
  
    //---------------------------------------------------------------------------
  /** \brief Initialize operators. 
  
    By default only the unary minus operator is added.
  */ 
  void Parser::InitOprt () 
  {
    
    
  
    //---------------------------------------------------------------------------
  void Parser::OnDetectVar (string_type * /*pExpr */ , int & /*nStart */ ,
                            int & /*nEnd */ ) 
  {
    
      // this is just sample code to illustrate modifying variable names on the fly.
      // I'm not sure anyone really needs such a feature...
      /*
         
         
         string sVar(pExpr->begin()+nStart, pExpr->begin()+nEnd);
         string sRepl = std::string("_") + sVar + "_";
         
         int nOrigVarEnd = nEnd;
         cout << "variable detected!\n";
         cout << "  Expr: " << *pExpr << "\n";
         cout << "  Start: " << nStart << "\n";
         cout << "  End: " << nEnd << "\n";
         cout << "  Var: \"" << sVar << "\"\n";
         cout << "  Repl: \"" << sRepl << "\"\n";
         nEnd = nStart + sRepl.length();
         cout << "  End: " << nEnd << "\n";
         pExpr->replace(pExpr->begin()+nStart, pExpr->begin()+nOrigVarEnd, sRepl);
         cout << "  New expr: " << *pExpr << "\n";
       */ 
  } 
    //---------------------------------------------------------------------------
  /** \brief Numerically differentiate with regard to a variable. 
      \param [in] a_Var Pointer to the differentiation variable.
      \param [in] a_fPos Position at which the differentiation should take place.
      \param [in] a_fEpsilon Epsilon used for the numerical differentiation.

    Numerical differentiation uses a 5 point operator yielding a 4th order 
    formula. The default value for epsilon is 0.00074 which is
    numeric_limits<double>::epsilon() ^ (1/5) as suggested in the muparser
    forum:

    http://sourceforge.net/forum/forum.php?thread_id=1994611&forum_id=462843
  */ 
  value_type Parser::Diff (value_type * a_Var, 
                             
  {
    
    {
    0, 0, 0, 0}, 
    
      // Backwards compatible calculation of epsilon inc case the user doesn't provide
      // his own epsilon
      if (fEpsilon == 0)
      
        (a_fPos == 0) ? (value_type) 1e-10 : (value_type) 1e-7 *a_fPos;
    
    f[0] = Eval ();
    
    f[1] = Eval ();
    
    f[2] = Eval ();
    
    f[3] = Eval ();
    
    
    
  
