/* 
 * syntaxTree.h - declaration of objects used by the syntax tree. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __syntaxTree_h__
#define __syntaxTree_h__

struct token_item
{
  int node_type;
  char *name;
  int line;
};

struct syntaxNode; // needed for the definition of YYVALUE

#ifndef YYBISON
  #include "opascal.hpp"
#endif

#ifdef NODE_ITEM
#undef NODE_ITEM
#endif

#define NODE_ITEM(x) e_##x,

enum SyntaxNodes { 
  dummy=1000,
  #include "astNodeNames.inc"
};

#undef NODE_ITEM

#include "containers.h"
#include "symbolObjects.h"

struct syntaxNode
{
  int   node_type;  
  char *node_name;
  int   node_line;

  syntaxNode(SyntaxNodes i);
  syntaxNode();
  virtual ~syntaxNode(){}

  static char *syntaxNodeNames[];

  size_t count;
   std::vector<syntaxNode*> children;

  syntaxNode *get_node(size_t type);
  
  void clear();
  void appendChild(syntaxNode *);
  void prependChild(syntaxNode *);
  void copyNode(syntaxNode *);
  void insertChild(syntaxNode *, size_t inx);
  int getNodeLine();

  // information used by the syntax validator...
  union
  {
    sym_namespace *name_space;
    sym_variable *variable;
    sym_type *data_type;
    sym_procedure *procedure;
  };

  sym_variable *implied_var;

  // for expression expansion...
  union
  {
    size_t deref_count;
    int flags;
  };
  
  // information used by the code generator.
  size_t cindex;
  reg_location reg_loc;
  unsigned char negate_ : 1;
  unsigned char static_ : 1;
};

template <class T> 
class CsyntaxTreeT
{
private:
  thash<char*> _m_duplicateStrings;
  std::vector<T*> _m_childNodes;
  T *_m_pRoot;
	T *_m_scriptNode;
public:
  CsyntaxTreeT(){_m_pRoot=0;_m_scriptNode=0;};

  virtual ~CsyntaxTreeT()
  { 
    typedef std::vector<T*>::iterator _itertype;

    // clear child nodes.
    _itertype it=_m_childNodes.begin();
    for(;it!=_m_childNodes.end();it++)delete *it;

    // clear node names.
    thash<char*>::iterator strit = strit=_m_duplicateStrings.begin();
    for(;strit!=_m_duplicateStrings.end();strit++)
      free(*strit);
    _m_duplicateStrings.clear();
  };

  char *duplicateString(const char *p)
  {
    char *pdup;
    tlist<char *> l;
    if(_m_duplicateStrings.find(p,l))
      return *l.begin();

    pdup = strdup(p);
    _m_duplicateStrings.insert(pdup,(char*)pdup);
    return pdup;
  }

  virtual syntaxNode *createNode(int nodeId, token_item &item)
  {
    syntaxNode *node = createNode();

    node->node_name = (char*)item.name;
    node->node_type = nodeId;
    node->node_line = item.line;  

    return node;
  }

  virtual syntaxNode *createNode(SyntaxNodes nodeId){
    _m_childNodes.push_back(new T(nodeId));
    return _m_childNodes.back();
  }
  
  virtual syntaxNode *createNode(){
    _m_childNodes.push_back(new T());
    return _m_childNodes.back();
  }

	T *addScriptItem(T *p)
	{
		T *pInsertNode;
		if(_m_scriptNode==0){
			_m_scriptNode=createNode(e_script);
		  _m_scriptNode->appendChild(createNode(e_statement_list));
		}

		switch(p->node_type)
		{
		  case e_statement:
		  case e_statement_list:
			case e_statement_raise:
      case e_statement_with:
      case e_statement_assign:
      case e_statement_repeat:
      case e_statement_while:
      case e_statement_if:
      case e_statement_for:
      case e_statement_case:
      case e_statement_try:
      case e_statement_goto:
      case e_statement_label:
      	pInsertNode=_m_scriptNode->children[0];
		  break;
			default:
				pInsertNode=_m_scriptNode;
		} 
			    
		pInsertNode->appendChild(p);

		return _m_scriptNode;
	}

	T *getRoot(){ return _m_pRoot;}
  void setRoot(T *r){ _m_pRoot = r;}
};

typedef CsyntaxTreeT<syntaxNode> CsyntaxTree;

std::string tostring(int i);
std::string tostring(double d);
std::string tostring(float f);
std::string tostring(unsigned long);
std::string tostring(unsigned int);

#endif//__syntax_Tree_h_