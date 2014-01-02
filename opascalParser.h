/* 
 * opascalParser.h - Declaration of lexer and parser classes.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __opascalParser_h__
#define __opascalParser_h__

#ifndef yyFlexLexer
  #define yyFlexLexer pascalFlexLexer
  #define yywrap      pascalwrap

  #include <FlexLexer.h>
#endif

#include "syntaxTree.h"
#include "scriptErrors.h"

class CopascalLexxer : public yyFlexLexer
{
public:
  CopascalLexxer(int starting_line = 1, istream* arg_yyin = 0,ostream* arg_yyout = 0)
  : yyFlexLexer( arg_yyin, arg_yyout )
  { 
    _m_syntaxTree = 0;
    _m_text = 0;
    yylineno = starting_line;
		_m_semicolonErr = false;
  }

  ~CopascalLexxer();

  CopascalLexxer(const char *p, int starting_line = 1);

  virtual int LexerInput(char* buf, int max_size);
  virtual int yylex(YYSTYPE *ptype);
  virtual int yywrap(){ return 1;}
  virtual int parserError( const char *s, int count, short currState, short *stateStack ,
                                                     YYSTYPE &currType, YYSTYPE *typeStack,
																										 int *errstatus);

  CsyntaxTree *_m_syntaxTree;
  scriptErrorHandler *_m_pErrHandler;
  void seq_to_str(char *p, YYSTYPE *t);
	void clearSemicolonErr(){_m_semicolonErr=false;}
	void destructError();

private:
	// used for syncronization after missing semicolon 
	bool _m_semicolonErr;
	int _m_errLine;
	std::string _m_errToken;

	int *_m_errStatus;
	char *_m_text;
  size_t _m_curr_pos;
  size_t _m_len;
};

class CpascalParser
{
private:
  CopascalLexxer *_m_currLexer;

	int pascalparse(void *);
	void yydestruct (int yytype, YYSTYPE *yyvaluep);

public:
  int parseFile(const char *filepath, CsyntaxTree *tree, scriptErrorHandler *pErrHandler);
  int parseText(const char *text,  CsyntaxTree *tree, scriptErrorHandler *pErrHandler, int starting_line = 1);
};

#include "syntaxValidator.h"

#endif
