/* 
 * symbolObjects.cpp - Implementation of symbol table objects.
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
#include "containers.h"
#include "symbolObjects.h"
#include "syntaxTree.h"

#pragma warning(disable : 4313)

sym_base::sym_base()
{
	scode=0;
  unit_index=0;
  next=0;
}

sym_base::~sym_base()
{
}

sym_procedure::sym_procedure()
{
  name = 0;
  code_name = 0;
  sym_type *ret_type = 0;
  parameters = 0;
  parent=0;
  next_overload=0; // needed for overloaded members
  dispid=0; // used for automation objects
  param_count=0;
  default_count=0;
  param_size=0;
  proc_number=0;
  proc_offset=0;
  proc_gnumber=0;
  static_proc  = 0;
  safecall = 0;
  overloaded = 0;
  built_in = 0;
  function = 0;
  inline_fn = 0;
  implemented = 0;
  visibility = 0;
  constructor = 0;
}

sym_procedure::~sym_procedure()
{
  if(parameters)
    free(parameters);
}

sym_variable::sym_variable()
{
  name=0;
  prop_info=0;
  type=0;
	parent=0;
  offset=0;
  default_ = 0;
  const_  = 0;
  param   = 0;
  static_var  = 0;
  visibility  = 0;
  byref  = 0;
}

sym_variable::~sym_variable()
{
}

sym_type::sym_type(const char *n, NativeTypeCodes code, unsigned s,unsigned char f)
{
  scode = sy_type;
  name=n;
  tycode=code;
  size=s;
  base_type=0;
  ptr_level=0;
  flags=f;
}

sym_type::sym_type()
{
  name=0;
  tycode=ty_dummy;
  size=0;
  tnumber=0;
  flags=0;
  base_type=0;
  ptr_level=0;
}
  
array_type::array_type()
{
	count=0;
  element_count=0;
  dimentions=0; 
}

array_type::~array_type()
{
	for(size_t i=0;i<count;i++){
	  delete(dimentions[i]);
	}

	if(dimentions)free(dimentions);
}
  
sym_base *sym_namespace::get_member(const char *name)
{
  tlist<sym_procedure*> p;
  tlist<sym_variable*> v;
  tlist<sym_type*> t;
  sym_type *ptype;
  sym_namespace *ps = this;

  do
  {
    if(ps->variables.find(name,v))
      return (sym_base*)*v.begin();
    if(ps->types.find(name,t))
      return (sym_base*)*t.begin();
    if(ps->procedures.find(name,p))
      return (sym_base*)*p.begin();

    ptype = ps->space_type;

    if(ptype && ptype->base_type)
      ps=ps->parent;
    else 
      ps=0;
  }
  while(ps);

  return 0;
}

void sym_namespace::add_symbol(sym_base *s)
{
  s->next = 0;

  if(this->last_symbol == 0)
  {
    this->first_symbol = s;
    this->last_symbol = s;
  }
  else
  {
    this->last_symbol->next = s;
    this->last_symbol = s;
  }
}

Cconstants::Cconstants()
{
  _m_size = 0;
}

Cconstants::~Cconstants()
{
  clear();
}

 void Cconstants::print_consts(FILE *pf)
{
  USES_CONVERSION;
  size_t i;
  size_t len;
  string_data *sdata;
  wstring_data *bdata;

  for(i=0;i<_m_array.size();i++)
  {
    switch(_m_array[i].type->tycode)
    {
      case ty_real:
        fprintf(pf,"c c%i %c%s\n", i+1,(_m_array[i].type->size==4)?'f':'d',_m_array[i].value);
      break;
      case ty_integer:
        fprintf(pf,"c c%i i%s\n", i+1,_m_array[i].value);
      break;
      case ty_boolean:
        fprintf(pf,"c c%i c%i\n", i+1,(strcmp(_m_array[i].value,"false")==0)?0:1);
      break;
      case ty_pointer:
        fprintf(pf,"c c%i i0\n", i+1);
      break;
      case ty_char:
        fprintf(pf,"c c%i c%i\n", i+1,_m_array[i].value[0]);
      break;
      case ty_string:
        len=strlen(_m_array[i].value);
        sdata = (string_data*)malloc(sizeof(string_data)+len);
        sdata->types = 0;
        sdata->len   = len;
        sdata->delphi_ref = -1;
        strcpy(sdata->strdata,_m_array[i].value);
        fprintf(pf,"c c%i i%i\n", i+1,sdata->strdata);
        _m_strings.push_back(sdata);
      break;
      #ifdef _WIN32
      case ty_widestring:
        bdata = (wstring_data*)malloc(sizeof(wstring_data));
        bdata->type = 0;
        {bdata->str = SysAllocString(A2W(_m_array[i].value));}
        fprintf(pf,"c c%i i%i\n", i+1,bdata->str);
				_m_wstrings.push_back(bdata);
      break;
      #endif
      case ty_record:
      case ty_variant:
        fprintf(pf,"c c%i h%s\n", i+1,_m_array[i].value);
      break;
    }
  }
   
  fprintf(pf,"\n");
}

size_t Cconstants::add(syntaxNode *p)
{
  sym_constant c;
  size_t i; 
  for(i=0;i<_m_array.size();i++)
  {
    if(_m_array[i].type->tycode==p->data_type->tycode)
      if(strcmp(_m_array[i].value,p->node_name)==0)
      {
        p->cindex = i;
        return _m_array[i].offset;
      }
  }
 
  c.type = p->data_type;
  c.value = p->node_name;
  c.offset = _m_size;

  _m_size += c.type->size;
  p->cindex = i;

  _m_array.push_back(c);

  return c.offset;
}

void Cconstants::clear()
{
  size_t i = 0;

  for(i=0;i<_m_strings.size();i++)
    free(_m_strings[i]);

	for(i=0;i<_m_wstrings.size();i++)
		free(_m_wstrings[i]);

	_m_strings.clear();
	_m_wstrings.clear();
  _m_array.clear();
}



