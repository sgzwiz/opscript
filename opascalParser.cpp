/* 
 * opascalParser.cpp - parser class functions. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#include <atlbase.h>
#include <fstream>
#include <malloc.h>
#include <memory.h>
#include "common.h"
#include "opascalParser.h"
#include "moduleCompiler.h"
#include "utils.h"

// global variables -- used by the parser
extern int pascaldebug;

std::string tostring(int i)
{
  std::stringstream b;
  std::string s;

  b << i;
  b >> s;

  return s;
}

std::string tostring(unsigned long i)
{
  std::stringstream b;
  std::string s;

  b << i;
  b >> s;

  return s;
}

std::string tostring(unsigned int i)
{
  std::stringstream b;
  std::string s;

  b << i;
  b >> s;

  return s;
}

std::string tostring(double d)
{
  std::stringstream b;
  std::string s;

  b << d;
  b >> s;

  return s;
}

std::string tostring(float f)
{
  std::stringstream b;
  std::string s;

  b << f;
  b >> s;

  return s;
}

std::string tostring( YYSTYPE &type , int tok=-1)
{
  if(tok==0)
    return "<EOF>";

  std::string t;

  if (type.name.node_type <TOK_FINAL_TOKEN)
    t = type.name.name;
  else
    t = type.node->node_name;
 
  return t;
}

std::string BuildError( int count, short currState, short *stateStack ,
                                                     YYSTYPE &currType, YYSTYPE *typeStack )
{
  std::string s;

  s = "\n";
  
  s += "   current state : " +  tostring(stateStack[count])  + "\n";
  s += "   error token   : " +  tostring(currType,currState) + "\n";
  s += "   parse stack   : \n";
  
  for(int i=1;i<=count;i++)
    s += "        (" + (tostring(stateStack[i])) + ") " + tostring(typeStack[i]) + "\n";

  return s;
}

// yyss  -- state stack
// yyssp -- current stack possition
// yyvs  -- value stack 
// yyvsp -- current value 
int CopascalLexxer::parserError( const char *s , int count, short currState, short *stateStack ,
                                                     YYSTYPE &currType, YYSTYPE *typeStack,
																										 int *errstatus ) 
{
	// right now, state 43 is the state that contains the following rule:
	// script_item_list . error script_item. If we hit this, then we were expecting a 
	// semicolon, but didn't find it.
	if(stateStack[count] == 46){
		_m_semicolonErr = true;
		_m_errLine = yylineno;
		_m_errToken = (currState < dummy )?currType.name.name:currType.node->node_name;
	  return 0;
	}

	std::string errStr = BuildError(count,currState,stateStack,currType, typeStack );

  CParserErr err(lineno(),"%s",errStr.c_str());
  
  _m_pErrHandler->handle_error(&err);

  return 0;
};

CopascalLexxer::~CopascalLexxer()
{
  if(_m_text)
    free(_m_text);
}

CopascalLexxer::CopascalLexxer( const char *p, int starting_line )
{
  _m_text = strdup(p);
  _m_curr_pos=0;
  _m_len = strlen(p);
  yylineno = starting_line;
  _m_semicolonErr=false;
}

int CopascalLexxer::LexerInput(char* buf, int size)
{
  if(_m_text)
  {
    int len;

    if(_m_curr_pos == _m_len)
      return 0;

    len = min(size,(int)(_m_len-_m_curr_pos));

    memcpy(buf, &_m_text[_m_curr_pos], len);
    _m_curr_pos += len;
    return len;
  }
  else
    return yyFlexLexer::LexerInput(buf,size);
}

void CopascalLexxer::seq_to_str(char *p, YYSTYPE *t)
{
}

// the sole point of this function is to watch for a token 
// being discarded after we hit the missing semicolon condition.
// if that happens, then we were not able to resync properly. will need to 
// watch this...
void CopascalLexxer::destructError()
{
	if(_m_semicolonErr){
		clearSemicolonErr(); // clear it. Other errors may be thrown after this, 
		                       // 
		CParserErr *pErr = new CParserErr(_m_errLine,"Expected semicolon, \"%s\" found instead", _m_errToken.c_str());
		_m_pErrHandler->handle_error(pErr);
		delete pErr;
	}
}

void xml_char(FILE *pf, const char *s)
{
  while(*s)
  {
    fprintf(pf,"&#x%x;",*s);
    s++;
  }
}

void output_xml(FILE *pf, int indent, syntaxNode *p, int use_indents)
{
  //output the indent...
  int i=0;
  char ibuff[4000];

  if(use_indents)
  {
    memset(ibuff,' ',indent);
    ibuff[indent]=0;
  }
  else
    ibuff[0] = 0;
    
  if(p->node_type<1000)
  {
    switch(p->node_type)
    {
      case TOK_IDENTIFIER:
        fprintf(pf,"%s<identifier name = \"%s\"/>\n", ibuff,p->node_name);
      break;
      case TOK_HEX:
      case TOK_INTEGER:
      case TOK_REAL:
        fprintf(pf,"%s<number value = \"%s\"/>\n", ibuff,p->node_name);
      break;
      case TOK_STRING:
        fprintf(pf,"%s<string value = \"", ibuff);
        //remove the first and last items
        xml_char(pf,p->node_name);
        fprintf(pf,"\"/>\n");
      break;
      case TOK_EQ:
      case TOK_NE:
      case TOK_LT:
      case TOK_LE:
      case TOK_GT:
      case TOK_GE:
        fprintf(pf,"%s<rel_op>",ibuff);
        xml_char(pf,p->node_name);
        fprintf(pf,"</rel_op>\n");
      break;
      case TOK_PLUS:
      case TOK_MINUS:
      case TOK_STAR:
      case TOK_SLASH:
        fprintf(pf,"%s<arith_op>",ibuff);
        xml_char(pf,p->node_name);
        fprintf(pf,"</arith_op>\n");
      break;
      case TOK_LAND:
      case TOK_LOR:
        fprintf(pf,"%s<bit_op>",ibuff);
        xml_char(pf,p->node_name);
        fprintf(pf,"</bit_op>\n");
      break;
      case TOK_ADDR:
        fprintf(pf,"%s<addr_op>",ibuff);
        xml_char(pf,p->node_name);
        fprintf(pf,"</addr_op>\n");
      break;
      case TOK_DEREF:
        fprintf(pf,"%s<deref_op>",ibuff);
        xml_char(pf,p->node_name);
        fprintf(pf,"</deref_op>\n");
      break;
      
      default:
       if(((p->node_name[0]>='a') && (p->node_name[0]<='z')) ||
         ((p->node_name[0]>='A') && (p->node_name[0]<='Z')))
       {
         fprintf(pf,"%s<keyword>%s</keyword>\n", ibuff,p->node_name);
       }
       else
       {
         fprintf(pf,"%s<separator>",ibuff);
         xml_char(pf,p->node_name);
         fprintf(pf,"</separator>\n");
       }
    }
  }
  else
  {
    fprintf(pf,"%s<%s>\n",ibuff,p->node_name);
    for(size_t k=0;k<p->count;k++)
    {
      output_xml(pf,indent+2,p->children[k],use_indents);
    }
    //enumerate all the children...
    fprintf(pf,"%s</%s>\n",ibuff,p->node_name);
  }
}

int CpascalParser::parseText(const char *text, CsyntaxTree *tree,  scriptErrorHandler *pErrHandler, int starting_line)
{
  int ret;

  {
    CopascalLexxer pL(text,starting_line);

		_m_currLexer = &pL;

    pL._m_syntaxTree=tree;
    pL._m_pErrHandler=pErrHandler;
    ret = pascalparse(&pL);
  }

  return ret;
}

int CpascalParser::parseFile(const char *fname, CsyntaxTree *tree,  scriptErrorHandler *pErrHandler)
{ 
  int ret;
 
  fstream fs;
  fs.open(fname,ios_base::in);

  if(!fs.is_open())
    return -1;

  {
    CopascalLexxer pL(1,&fs);

		_m_currLexer = &pL;

    pL._m_syntaxTree=tree;
    pL._m_pErrHandler=pErrHandler;
    ret = pascalparse(&pL);
  }

  fs.close();
  return ret;
}

void CpascalParser::yydestruct (int yytype, YYSTYPE *yyvaluep)
{
	_m_currLexer->destructError();
}

void emit_xml_code(syntaxNode *psyntax, int use_xsl,int use_indents)
{
  FILE *xml = fopen("xmlfile.xml", "wt");

  if(use_xsl)
    fprintf(xml,"%s", "<?xml version=\"1.0\"?>\n"
                      "<?xml:stylesheet type=\"text/xsl\" href=\"ipascal.xsl\"?>\n");
  output_xml(xml,0,psyntax,use_indents);
  fclose(xml);
}

