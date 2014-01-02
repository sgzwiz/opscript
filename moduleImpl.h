/* 
 * moduleImpl.h - implementation of the module interface. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __moduleimpl_h__
#define __moduleimpl_h__

#include  "utils.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeUnitImpl
template <class TUnitClass, class TManagerClass>
class CmoduleImpl : public Cmodule
{
public:
  std::string _m_name;
  std::string _m_fullpath;
  std::string _m_asmpath;
  std::vector<TUnitClass*> _m_used_units;
  bool _m_compiled;
  bool _m_executed;
  thash<sym_procedure*> proc_dict;
  sym_namespace undeclared_items;
  TManagerClass *_m_manager;
  unsigned _m_index;

  CsyntaxTree _m_syntaxTree;   
public:
  CmoduleImpl();
  ~CmoduleImpl();

  size_t get_index();
  void set_compiled(bool b);
  void set_executed(bool b);
  bool is_compiled();
  bool is_executed();

  CModuleManager *get_manager();
  sym_procedure *get_procedure(char *pname);
  sym_base *get_symbol(const char *name, symbol_context *ns, bool recursive=false, sym_location *sloc=0);  
  sym_type *add_undeclared(syntaxNode *p, sym_type **);
  void update_undeclared(sym_type *);
  void output_constants(FILE *p);
  size_t get_procedure_count();
  void add_procedure(const char *pname, sym_procedure *pproc);

  const char *get_name();
  const char *get_filename();
  const char *get_asmname();
  CsyntaxTree *get_syntax_tree();

  bool is_const(symbol_context *ns, syntaxNode *p);
  void add_unit(Cmodule *);
};

template <class TUnitClass, class TManagerClass>
CmoduleImpl<TUnitClass,TManagerClass>::CmoduleImpl()
{
  _m_compiled = false;
  _m_executed = false;
  is_script = false;
  error_count = 0;
  ds = 0;
  cs = 0;
  _m_name = "";
  _m_fullpath = "";
  _m_asmpath = "";
  main_offset = -1;

  global.name = "@global";
  global.parent = 0;
  global.size = 0;
  global.space_type = 0;
  global.first_symbol = 0;
  global.last_symbol = 0;
  global.proc_count = 0;

  undeclared_items.name = "@global";
  undeclared_items.parent = 0;
  undeclared_items.size = 0;
  undeclared_items.space_type = 0;
  undeclared_items.first_symbol = 0;
  undeclared_items.last_symbol = 0;
  undeclared_items.proc_count = 0;
  
  name_stack.ns_var = 0;
  name_stack.last = 0;
  name_stack.ps=&global;
}

template <class TUnitClass, class TManagerClass>
CmoduleImpl<TUnitClass,TManagerClass>::~CmoduleImpl()
{  
}

template <class TUnitClass, class TManagerClass>
size_t CmoduleImpl<TUnitClass,TManagerClass>::get_index()
{
  return _m_index;
}

template <class TUnitClass, class TManagerClass>
CModuleManager *CmoduleImpl<TUnitClass,TManagerClass>::get_manager()
{
  return _m_manager;
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::set_compiled(bool b)
{
  _m_compiled=b;
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::set_executed(bool b)
{
  _m_executed=b;
}

template <class TUnitClass, class TManagerClass>
bool CmoduleImpl<TUnitClass,TManagerClass>::is_compiled()
{
  return _m_compiled;
}

template <class TUnitClass, class TManagerClass>
bool CmoduleImpl<TUnitClass,TManagerClass>::is_executed()
{
  return _m_executed;
}

template <class TUnitClass, class TManagerClass>
sym_procedure *CmoduleImpl<TUnitClass,TManagerClass>::get_procedure(char *pname)
{
  tlist<sym_procedure*> pl;
  CmoduleImpl *pUnit;

  if(proc_dict.find(pname,pl))
    return *pl.begin();

  std::string n = pname;
  size_t offset = n.find('.');

  if(offset!=-1)
  { 
    n.erase(offset,n.size() - (offset));
    n.erase(0,1);
  }
  else
    n = "";

  if(n.empty())
    pUnit = (CmoduleImpl*)_m_manager->get_unit((size_t)0);
  else
    pUnit = (CmoduleImpl*)_m_manager->get_unit(n.c_str());

  if(pUnit)
    if(pUnit->proc_dict.find(pname,pl))
      return *pl.begin();

  return 0;
}

template <class TUnitClass, class TManagerClass>
size_t CmoduleImpl<TUnitClass,TManagerClass>::get_procedure_count()
{
  return proc_dict.size();
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::add_procedure(const char *pname, sym_procedure *pproc)
{
  proc_dict.insert(pname,pproc);
}

template <class TUnitClass, class TManagerClass>
sym_base *CmoduleImpl<TUnitClass,TManagerClass>::get_symbol(const char *name, symbol_context *ns, bool recursive, sym_location *sloc)
{
  sym_location ss;
  sym_base *pbase;
  Cmodule *pUnit;

  if(!sloc)
    sloc=&ss;

  // always look in system.pas first
  pUnit = (_m_used_units.size()>0)?_m_used_units[0]:this;

  sloc->ns = &pUnit->name_stack;
  sloc->ps = &pUnit->global;
      
  if(pbase = pUnit->global.get_member(name))
    return pbase;

  // now lookup up the information in the passed in array...
  sloc->ns = ns;

  do
  {
    sloc->ps=sloc->ns->ps;
    
    do
    {
      if(pbase = sloc->ps->get_member(name))
        return pbase;

      sloc->ps=sloc->ps->parent;
    }
    while(sloc->ps&&recursive);
    
    sloc->ns=sloc->ns->last;
  }
  while(sloc->ns && recursive);

  if(recursive)
  {
    size_t i = 0;

    for(i = 1;i<_m_used_units.size();i++)
    {
      pUnit = _m_used_units[i];

      sloc->ns = &pUnit->name_stack;
      sloc->ps = &pUnit->global;
      
      /*
      if(pUnit->global.variables.find(name,v))
        return (sym_base*)*v.begin();
      if(pUnit->global.types.find(name,t))
        return (sym_base*)*t.begin();
      if(pUnit->global.procedures.find(name,p))
        return (sym_base*)*p.begin();
      */
      if(pbase = pUnit->global.get_member(name))
        return pbase;
    }

    sloc->ns = 0;
    sloc->ps = 0;
  }
  
  return 0;
}

template <class TUnitClass, class TManagerClass>
sym_type *CmoduleImpl<TUnitClass,TManagerClass>::add_undeclared(syntaxNode *p, sym_type **ptype)
{
  cundeclared_type *pundtype;
  
  pundtype = (cundeclared_type*)undeclared_items.get_member(p->node_name);
  
  if(!pundtype)
  {  
    pundtype = new cundeclared_type();

    pundtype->flags = TTYPE_IMPLEMENTED;     
    pundtype->name = p->node_name;
    pundtype->ptr_level = 0;
    pundtype->scode = sy_type;
    pundtype->tycode = ty_undeclared;
    pundtype->size = 0;
    pundtype->unit_index = _m_index;
    pundtype->base_type = 0;

    undeclared_items.types.insert(pundtype->name,pundtype);
  }

  cundeclared_type::ref_location ref;

  ref.pref = ptype;
  ref.line = p->getNodeLine();

  pundtype->_m_locations.push_back(ref);

  return pundtype;
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::update_undeclared(sym_type *p)
{
  if(undeclared_items.types.size()==0)
    return;

  tlist<sym_type*> l;
  cundeclared_type *ptype;
  
  if(undeclared_items.types.find(p->name,l))
  {
    ptype = ((cundeclared_type*)*l.begin());

    for(size_t i=0;i<ptype->_m_locations.size();i++)
      *ptype->_m_locations[i].pref = p;
  }
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::output_constants(FILE *p)
{
  fprintf(p,"g %i\n", global.size);
  _m_constants->print_consts(p);
}

template <class TUnitClass, class TManagerClass>
const char *CmoduleImpl<TUnitClass,TManagerClass>::get_name()
{
  return _m_name.c_str();
}

template <class TUnitClass, class TManagerClass>
const char *CmoduleImpl<TUnitClass,TManagerClass>::get_filename()
{
  return _m_fullpath.c_str();
}

template <class TUnitClass, class TManagerClass>
const char *CmoduleImpl<TUnitClass,TManagerClass>::get_asmname()
{
  return _m_asmpath.c_str();
}

template <class TUnitClass, class TManagerClass>
CsyntaxTree *CmoduleImpl<TUnitClass,TManagerClass>::get_syntax_tree()
{
  return &_m_syntaxTree;
}

template <class TUnitClass, class TManagerClass>
bool CmoduleImpl<TUnitClass,TManagerClass>::is_const(symbol_context *ns, syntaxNode *p)
{
  sym_variable *pvar;

  switch(p->node_type)
  {
    case TOK_INTEGER:
    case TOK_REAL:
    case TOK_STRING:
    case TOK_HEX:
    case TOK_TRUE:
    case TOK_FALSE:
    case TOK_NIL:
      return true;
    break;
    case TOK_IDENTIFIER:
      pvar=(sym_variable*)get_symbol(p->node_name,ns,true);
      if((pvar->scode==sy_variable)&&(pvar->const_))
        return true;
    break;
  }
  return false;
}

template <class TUnitClass, class TManagerClass>
void CmoduleImpl<TUnitClass,TManagerClass>::add_unit(Cmodule *p)
{
  // make sure this unit is not here already...
  for(size_t i=0;i<_m_used_units.size();i++)
  {
    if(_m_used_units[i]==p)
      return;
  }

  _m_used_units.push_back((TUnitClass*)p);
}

#endif 