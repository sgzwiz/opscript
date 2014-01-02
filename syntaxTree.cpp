/* 
 * syntaxTree.cpp - Implementation of syntax tree objects.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#include "stdafx.h"
#include "common.h"
#include "containers.h"
#include "syntaxTree.h"

#define NODE_ITEM(x) #x,

char *syntaxNode::syntaxNodeNames[] =  { 
  #include "astNodeNames.inc"
};

syntaxNode::syntaxNode(SyntaxNodes nodeId)
{
  node_type = nodeId;
  node_line = -1;
  node_name = syntaxNodeNames[(unsigned)nodeId - (unsigned)e_goal];
  count=0;
  procedure=0;
  implied_var=0;
  deref_count=0;
}

syntaxNode::syntaxNode()
{
  node_type=0;
  node_name=0;
  node_line=-1;
  count=0;
  procedure=0;
  implied_var=0;
  deref_count=0;
}

void syntaxNode::clear()
{
  children.clear();
  count=0;
}

syntaxNode *syntaxNode::get_node(size_t type)
{
  for(size_t i=0;i<count;i++)
    if(children[i]->node_type == type)
      return children[i];
  return 0;
}

void syntaxNode::appendChild(syntaxNode *node)
{
	if(!node)
		return; 

  this->children.push_back(node);
  count=children.size();
}

void syntaxNode::prependChild(syntaxNode *node)
{
	if(!node)
		return; 

  children.insert(children.begin(),node);
  count=children.size();
}

void syntaxNode::copyNode(syntaxNode *node)
{
	if(!node)
		return; 

  children.clear();

  children = node->children;
  node->children.clear();

  count=children.size();
}

void syntaxNode::insertChild(syntaxNode *node, size_t inx)
{
	if(!node)
		return; 

  size_t i;
  std::vector<syntaxNode*>::iterator it;

  for(i=0;i<inx;it++,i++);

  children.insert(it,node);
  count=children.size();
}

