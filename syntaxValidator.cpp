/* 
 * syntaxValidator.cpp - Implementation of grammar syntax validator.
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
#include "common.h"
#include "moduleObjects.h"
#include "opascalParser.h"
#include "moduleimpl.h"
#include <stdarg.h>
#include <malloc.h>

Cmodule::Cmodule()
{
	cs = 0;
	ds = 0;
  _m_constants = new Cconstants;
}

Cmodule::~Cmodule()
{
  if(_m_constants)
  {
    _m_constants->clear();
    delete _m_constants;
  }

	for(size_t i=0;i<_m_symbols.size();i++){
	  delete _m_symbols[i];
	}

	if(cs)free(cs);
	if(ds)free(ds);
}

void Cmodule::loadNativeTypes()
{
  // initialize the native types...
  // integer types...
//	sym_type **native_types = (sym_type**)calloc(sizeof(void*),23);
	std::vector<sym_type*> native_types;

  native_types.push_back( new sym_type("integer"  , ty_integer, sizeof(int),TTYPE_IMPLEMENTED) );
  native_types.push_back( new sym_type("cardinal" , ty_integer, sizeof(unsigned),TTYPE_UNSIGNED | TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("shortint" , ty_integer, sizeof(char),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("smallint" , ty_integer, sizeof(short),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("longint"  , ty_integer, sizeof(int),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("int64"    , ty_integer, sizeof(unsigned),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("byte"     , ty_integer, sizeof(char),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("word"     , ty_integer, sizeof(short),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("longword" , ty_integer, sizeof(int),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("currency" , ty_integer, sizeof(unsigned),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  // character types
  native_types.push_back( new sym_type("ansichar" , ty_char, sizeof(char),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("widechar" , ty_char, sizeof(short),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("char"     , ty_char, sizeof(char),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
 // boolean types
  native_types.push_back( new sym_type("boolean"  , ty_boolean, sizeof(char),TTYPE_UNSIGNED| TTYPE_IMPLEMENTED));
  //real types
  native_types.push_back( new sym_type("single"   , ty_real, sizeof(float),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("double"   , ty_real, sizeof(double),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("extended" , ty_real, sizeof(double),TTYPE_IMPLEMENTED));
  // string types.
  native_types.push_back( new sym_type("string"   , ty_string, sizeof(void*),TTYPE_IMPLEMENTED));
  native_types.push_back( new sym_type("widestring" , ty_widestring, sizeof(void*),TTYPE_IMPLEMENTED));
  // void pointer type
  native_types.push_back( new sym_type("pointer"  , ty_pointer, sizeof(void*),TTYPE_IMPLEMENTED));
  // interface type
  native_types.push_back( new sym_type("interface", ty_interface,sizeof(void*),TTYPE_IMPLEMENTED));
  // variant type
  native_types.push_back( new sym_type("olevariant", ty_variant,sizeof(VARIANT),TTYPE_IMPLEMENTED));
  
	for(size_t i=0;i<native_types.size();i++){
    global.types.insert(native_types[i]->name,native_types[i]);
	  registerSymbol(native_types[i]);
	}
  
  int rc = parseSystem();

	if(rc){
    return;
	}

  CsyntaxValidator analyze;

  try
  {
    // first, make sure it's actually a unit... 
    if(get_syntax_tree()->getRoot()->children[0]->node_type != e_unit)
      throw CSyntaxErr(1,"%s is not a valid unit.\n",get_name());

    // do a syntax check on the unit...
    analyze.validateTree(this);

    // add variant constants...
    {
      syntaxNode pnode;
      sym_variable *pvar = new sym_variable;

      pnode.data_type = (sym_type*)global.get_member("olevariant");
      pnode.node_name = get_syntax_tree()->duplicateString("0A000000000000000400028000000000");

      pvar->name = get_syntax_tree()->duplicateString("vtempty");
      pvar->const_ = 1;
      pvar->default_ = 0;
      pvar->scode = sy_variable;
      pvar->param = 0;
      pvar->byref = 0;
      pvar->default_ = 0;
      pvar->static_var = 0;
      pvar->prop_info = 0;
      pvar->parent = &global;
      pvar->unit_index = 0;
      pvar->type = pnode.data_type;
      pvar->offset = (int)_m_constants->add(&pnode);

      global.variables.insert(pvar->name,pvar);
      global.add_symbol(pvar);

			registerSymbol(pvar);
    }
  }
  catch(CSyntaxErr err)
  {
    handle_error(&err);
  }
}

char *CsyntaxValidator::get_unknown_name(sym_type *p)
{ 
  std::string s;
  char buffer[20];

  sprintf(buffer,"%i",p->tnumber);
  s="@unnammed";
  s+=buffer;

  return duplicateString(s.c_str());
}

void Cmodule::registerSymbol(sym_base *pSym)
{
	_m_symbols.push_back(pSym);
}

char *CsyntaxValidator::duplicateString(const char *dstr)
{
  return _m_punit->get_syntax_tree()->duplicateString(dstr);
}

std::string CsyntaxValidator::proc_name(Cmodule *pUnit, sym_procedure *pproc)
{
  std::string s;

  s = "$";

  if(pUnit->get_index() != 0)
  {
    s += pUnit->get_name();
    s += ".";
  }

  s += "_";

  if(pproc->parent)
  {
    s += pproc->parent->name;
    s += "_";
  }

  s += pproc->name;

  if(pproc->overloaded)
    s += get_signature(pproc);

  return s;
}
CScriptErr::CScriptErr()
{
  _m_line = 0;
}

CScriptErr::~CScriptErr()
{
}

CRuntimeErr::CRuntimeErr(int line, const char *src, const char *msg)
{
  _m_source = src;
  _m_message = msg;
  _m_line = line;  
}

CSyntaxErr::CSyntaxErr(int line, const char *msg,...)
{
  char err_buffer[1024];
  va_list marker;
  va_start( marker, msg);     

  vsprintf(err_buffer,msg,marker);
  _m_message = err_buffer;
	_m_source = "Syntax Parser";
  _m_line = line;  
};

CParserErr::CParserErr(int line, const char *msg,...)
{
  char err_buffer[1024];
  va_list marker;
  va_start( marker, msg);     

  vsprintf(err_buffer,msg,marker);
  _m_message = err_buffer;
	_m_source = "Grammar Parser";
  _m_line = line;  
};


CCompilerErr::CCompilerErr(int line, const char *msg,...)
{
  char err_buffer[1024];
  va_list marker;
  va_start( marker, msg);     

  vsprintf(err_buffer,msg,marker);
  _m_message = err_buffer;
	_m_source = "Code Compiler";
  _m_line = line;  
};


 //0123456789012345678901234567890123456
 // EDCD6A60 B053 11D0 A62F 00A0C922E752 
 //{EDCD6A60-B053-11D0-A62F-00A0C922E752}
 //EDCD6A60B05311D0A62F00A0C922E752 

std::string CsyntaxValidator::get_guid_memory(char *p)
{
  std::string s;
  char buff[33];
  int  j;
  int  pos=0;
  buff[32] = 0;

  for(j=7;j>=1;j-=2)
  {
    memcpy(&buff[pos],&p[j],2);
    pos+=2;
  }

  for(j=12;j>=10;j-=2)
  {
    memcpy(&buff[pos],&p[j],2);
    pos+=2;
  }

  for(j=17;j>=15;j-=2)
  {
    memcpy(&buff[pos],&p[j],2);
    pos+=2;
  }
  
  memcpy(&buff[16],&p[20],4);
  memcpy(&buff[20],&p[25],12);
  

  s = buff;
  return s;
}

bool CsyntaxValidator::check_guid_syntax(char *p)
{
  if(strlen(p)!= 38)
    return false;

  for(int i=0;i<38;i++)
  {
    switch(i)
    {
      case 0:
        if(p[i]!='{')
          return false;
      break;
      case 37:
        if(p[i]!='}')
          return false;
      break;
      case 9:
      case 14:
      case 19:
      case 24:
        if(p[i]!='-')
          return false;
      break;
      default:
        if(!       
          (((p[i]>='A') && (p[i]<='F')) ||
          ((p[i]>='a') && (p[i]<='f')) ||
          ((p[i]>='0') && (p[i]<='9'))))
          return false;
    }
  }

  return true;
}

int syntaxNode::getNodeLine()
{
  if(node_line!=-1)
    return node_line;

  // see if we have any children.
  if(children.size()== 0)
    return 1;

	// return the line from the first child
  return children[0]->getNodeLine();
}

void add_type_sig(std::string &s, sym_type *ptype, int lineno)
{
  char buffer[20];

  switch(ptype->tycode)
  {
    case ty_variant:
      s += 'v';
    break;
    case ty_char:
      s += 'c';
    break;
    case ty_integer:
      switch(ptype->size){
      case 1: s += 'y';break;
      case 2: s += 'm';break;
      case 4: s += 'i';break;
      }
    break;
    case ty_boolean:
      s += 'b';
    break;
    case ty_real:
      switch(ptype->size){
        case 4: s += 'f';break;
        case 8: s += 'r';break;
      }
    break;
    case ty_widestring:
      s += 'w';
    break;
    case ty_string:
      s += 's';
    break;
    case ty_set:
			throw CSyntaxErr(lineno,"Not implemented");
    break;
    case ty_function:
			throw CSyntaxErr(lineno,"Not implemented");
    break;
    case ty_procedure:
			throw CSyntaxErr(lineno,"Not implemented");
    break;
    case ty_enumeration:
			throw CSyntaxErr(lineno,"Not implemented");
    break;
    case ty_pointer:
      s += 'p';
      if(ptype->base_type)
        add_type_sig(s,ptype->base_type,lineno);
      else
        s += 'v';
    break;
    case ty_array:
      sprintf(buffer,"%i",ptype->tnumber);
      s += 'a';
      s += ptype->name;
      s += buffer;            
    break;
    case ty_interface:
      sprintf(buffer,"%i",ptype->tnumber);
      s += 'n';
      s += ptype->name;
      s += buffer;            
    break;
    case ty_record:
      sprintf(buffer,"%i",ptype->tnumber);
      s += 'e';
      s += ptype->name;
      s += buffer;            
    break;
    case ty_class:
      sprintf(buffer,"%i",ptype->tnumber);
      s += 'l';
      s += ptype->name;
      s += buffer;            
    break;
		default: 
			throw CSyntaxErr(lineno,"Invalid type");
  }
}

std::string CsyntaxValidator::get_signature(sym_procedure *pproc)
{
  std::string s;

	int lineno;

  for(size_t i=0;i<pproc->param_count;i++)
	{
		lineno = (pproc->psyntax)?pproc->psyntax->getNodeLine():0;
		// get the line number...
    add_type_sig(s,pproc->parameters[i]->type,lineno);
	}
  return s;
}

sym_procedure *CsyntaxValidator::get_matching_overload( int line, sym_procedure *pproc, syntaxNode *pparams )
{
  sym_procedure *pcurr;
  sym_type *ptype;
  std::vector<sym_procedure*> items;
  size_t i;
  size_t j;

  for(pcurr=pproc;pcurr;pcurr=pcurr->next_overload)
  {
    if(pparams)
    {
      if((size_t)pparams->count>pcurr->param_count)
        continue;

      for(i=0;i<pparams->count;i++)
      {
        ptype = get_expr_type(pparams->children[i]);
         
        if(!is_equal_type(pcurr->parameters[i]->type,ptype))
          goto next_procedure1;        
      }

      if(i<pcurr->param_count)
        if(!pcurr->parameters[i]->default_)
          goto next_procedure1;

      items.push_back(pcurr);
    }
    else
    {
      if(!pcurr->param_count || pcurr->parameters[0]->default_ )
        items.push_back(pcurr);
    }   
    
 next_procedure1:
   continue; 
  }

  if(items.size()>1)
  {
    pcurr = 0;

    // try to restrict the search...
    if(pparams)
    {
      for(j=0;j<items.size();j++)
      {
        for(i=0;i<pparams->count;i++)
        {
          ptype = get_expr_type(pparams->children[i]);
         
          if(!is_equal_type(items[j]->parameters[i]->type,ptype,true))
            goto next_procedure2;
        }

        if(i<items[j]->param_count)
          if(!items[j]->parameters[i]->default_)
            goto next_procedure1;

        if(pcurr)
          throw CSyntaxErr(line,"ambiguous overloaded call to '%s'.", pproc->name); 
        else
          pcurr=items[j];

next_procedure2:
        continue;
      }

      if(!pcurr)
        throw CSyntaxErr(line,"ambiguous overloaded call to '%s'.", pproc->name); 
    }
    else
      throw CSyntaxErr(line,"ambiguous overloaded call to '%s'.", pproc->name); 
  }
  else 
  if(items.size() == 1)
    pcurr = items[0];

  return pcurr;
}

sym_procedure *CsyntaxValidator::get_overloaded_declaration( sym_procedure *pproc, syntaxNode *pparams )
{
  sym_procedure *pcurr;
  std::vector<sym_procedure*> items;
  size_t i,j,k;
  syntaxNode *plist,*param;
  sym_type *ptype;

  for(pcurr=pproc;pcurr;pcurr=pcurr->next_overload)
  {
    if(pparams)
    {
      for(i=0,k=0;i<pparams->count;i++)
      {
        switch(pparams->children[i]->node_type)
        {
          case e_parameter:
            param = pparams->children[i];
          break;
          case e_formal_parameter:
            param = pparams->children[i]->children[1];
          break;
        }

        plist = param->children[0];

        for(j=0;j<plist->count;j++)
        {
          if(k>=pcurr->param_count)
            goto next_procedure;
          
          ptype = get_expr_type(plist->children[j]);

          if(!is_equal_type(pcurr->parameters[k]->type,ptype,true))
            goto next_procedure;

          k++;
        }
      }      

      if(k==pcurr->param_count)
        items.push_back(pcurr);
    }
    else
    {
      if(pcurr->param_count==0)
        items.push_back(pcurr);
    }

next_procedure:
    continue;
  }

  if(items.size() > 0)
    return items[0];

  return 0;
}

inline void add_boundary(array_type *parr, array_bound *pbound )
{
  unsigned elems = (pbound->high - pbound->low + 1);
  
  parr->element_count = (parr->element_count)?parr->element_count * elems:elems;
   
  parr->dimentions = (array_bound**)realloc(parr->dimentions,sizeof(void*)*(parr->count+1));
  parr->dimentions[parr->count]=pbound;
  parr->count++;
}

void CsyntaxValidator::build_array(symbol_context *pns, sym_namespace *ps, array_type *arr, syntaxNode *bound_def, syntaxNode *type)
{
  size_t i,j;
  syntaxNode *psimple;
  syntaxNode *prange;
  sym_type *ptype;
  sym_type *ptype2;
  sym_variable *pvar;

  array_bound *pbound;
  // add the boundaries
  // it must be an ordinal type or a ranged type...
add_bounds_section:

  for(i=0;i<bound_def->count;i++)
  {
    psimple = bound_def->children[i];

    switch(psimple->children[0]->node_type)
    {
      case TOK_IDENTIFIER:
        ptype = get_type(pns,psimple,0);
     
        if(!ptype)
          throw CSyntaxErr(psimple->getNodeLine(),"Unknown identifier: '%s'.",psimple->children[0]->node_name);  
        if((ptype->tycode != ty_integer) && (ptype->tycode != ty_boolean) && (ptype->tycode != ty_char))
          throw CSyntaxErr(psimple->getNodeLine(),"Ordinal type required.");  

        // build the bound info...
        pbound = new array_bound;
        pbound->bound_type = (ptype->tycode==ty_integer)?INTEGER_TYPE:ptype;
        
        switch(ptype->size)
        {
          case 1:
            if(ptype->flags & TTYPE_UNSIGNED)
            {
              pbound->low  = 0;
              pbound->high = (ptype->tycode == ty_boolean)?1:255;
            }
            else
            {
              pbound->high = 127;
              pbound->low = -128;
            }
          break;
          case 2:
            if(ptype->flags & TTYPE_UNSIGNED)
            {
              pbound->high = 65535;
              pbound->low = 0;
            }
            else
            {
              pbound->high = 32767;
              pbound->low = -32768;
            }          
          break;
          default:
            delete pbound;
            throw CSyntaxErr(psimple->getNodeLine(),"Data type too large: Exceeds 2GB.");
        }
      break;
      case e_ranged_item:
        prange=psimple->children[0];
        reduce_expression(pns,prange->children[0],false);
        reduce_expression(pns,prange->children[1],false);

        if(!_m_punit->is_const(pns,prange->children[0]))
          throw CSyntaxErr(prange->children[0]->getNodeLine(),"Constant or type identifier expected.");
        if(!_m_punit->is_const(pns,prange->children[1]))
          throw CSyntaxErr(prange->children[1]->getNodeLine(),"Constant or type identifier expected.");
         
         ptype=get_expr_type(prange->children[0]);
         ptype2=get_expr_type(prange->children[1]);

         if(ptype!=ptype2)
           throw CSyntaxErr(prange->children[0]->getNodeLine(),"Incompatible types : %s and %s.", ptype->name, ptype2->name);

         pbound = new array_bound;
         pbound->bound_type = ptype;

         switch(ptype->tycode)
         {
           case ty_integer: 
             pbound->low = expr_val_int(0,prange->children[0]);
             pbound->high = expr_val_int(0,prange->children[1]);
           break;
           case ty_string:
             if((strlen(prange->children[0]->node_name)!=1)||(strlen(prange->children[1]->node_name)!=1))
               throw CSyntaxErr( prange->children[1]->getNodeLine(),"Incompatible type : string.");
            pbound->low=prange->children[0]->node_name[0];
            pbound->high=prange->children[1]->node_name[0];
            pbound->bound_type = CHAR_TYPE;
           break;
           case ty_boolean:
             pbound->low = (prange->children[0]->node_name[0]=='t')?1:0;
             pbound->high = (prange->children[1]->node_name[0]=='t')?1:0;
           break;
           default:
             delete pbound;
             throw CSyntaxErr(psimple->getNodeLine(),"Ordinal type required.");  
         }
      break;
      default:
        throw CSyntaxErr(psimple->getNodeLine(),"Ordinal type required.");
    }

    if(pbound->low > pbound->high)
    {
      delete pbound;
      throw CSyntaxErr( prange->children[1]->getNodeLine(),"Constant expression violates subrange bounds.");
    }
    
    // add the boundary
    add_boundary(arr,pbound);    
  }

  // add the type.
  switch(type->node_type)
  {
    case e_simple_type:
      arr->base_type = get_type(pns,type,ps);
    break;
    case e_struct_type:
      j=0;
      if(type->children[j]->node_type==TOK_PACKED)
        j++;
      switch(type->children[j]->node_type)
      {
        case TOK_ARRAY:
          bound_def=type->children[j+1];
          type=type->children[j+2];
          goto add_bounds_section;
        break;
        default:
          arr->base_type = get_type(pns,type,ps);
      }
    break;
  } 

  // make sure the array does not exeed 2GB
  //unsigned __int64 tmpval = arr->base_type->size *  arr->element_count;
  double tmpval = arr->base_type->size *  arr->element_count;

  if(tmpval > 2147483647)
    throw CSyntaxErr(bound_def->getNodeLine(),"Data type too large: Exceeds 2GB.");

  pvar = new sym_variable;
  pvar->name = 0;
  pvar->parent = ps;
  pvar->const_ = 0;
  pvar->static_var=0;
  pvar->param = 0;
  pvar->byref = 0;
  pvar->default_ = 0;
  pvar->scode = sy_variable;
  pvar->visibility = VIS_PUBLIC;
  pvar->prop_info = 0;
  pvar->offset = 0;
  pvar->type = arr->base_type;
  pvar->unit_index = _m_punit->get_index();

	_m_punit->registerSymbol(pvar);

  arr->pvar = pvar;
  arr->size = (unsigned int)tmpval;
}

void CsyntaxValidator::build_record(symbol_context *pns, record_type *rec, syntaxNode *p)
{
  size_t i;

  syntaxNode *pfields = p->get_node(e_field_list);
  syntaxNode *var_section = p->get_node(e_variant_section);
  syntaxNode *pvariables;
  syntaxNode *ptype;
  sym_namespace *sspace=&rec->members;
  symbol_context stack_node;

  sspace->size = 0;
  sspace->space_type = rec;

  stack_node.ns_var = 0;
  stack_node.last = pns;
  stack_node.ps = sspace;

  // add each of the fields...
  if(pfields)
    for(i=0;i<pfields->count;i++)
    {
      pvariables = pfields->children[i]->children[0];
      ptype = pfields->children[i]->children[1];

      try
      {
        add_variable(pns,sspace,pvariables,ptype,VIS_PUBLIC);
      }
      catch(CSyntaxErr err)
      {
        _m_punit->handle_error(&err);
      }
    }

  rec->size = sspace->size;
}

void CsyntaxValidator::build_class(symbol_context *pns, sym_namespace *ps, class_type *cls, syntaxNode *p)
{
  size_t i,j;
  char visibility;
  unsigned offset=0;

  syntaxNode  *pinheritance = p->get_node(e_class_heritage);
  syntaxNode  *pbody = p->get_node(e_class_body);
  syntaxNode  *psection;
  syntaxNode  *class_item;
  class_type *ptype;
  symbol_context stack_node;

  cls->members.space_type = cls;
  
  stack_node.ns_var = 0;
  stack_node.last = pns;
  stack_node.ps = &cls->members;
  

  // figure out the size...
  if(pinheritance)
  {
    // try to get the first item in the list...
    syntaxNode *pbase = pinheritance->children[0];

    ptype=(class_type*)_m_punit->get_symbol(pbase->children[0]->node_name,&stack_node,true);

    if(!ptype)
     throw CSyntaxErr(pbase->children[0]->getNodeLine(),"Undeclared identifier : %s.", pbase->children[0]->node_name);

    if(ptype->scode!=sy_type||(ptype->tycode!=ty_class))
      throw CSyntaxErr(pbase->children[0]->getNodeLine(),"Identifier %s is not a class name.", pbase->children[0]->node_name);

    cls->members.parent = &ptype->members;
    cls->members.size = ptype->members.size;
    cls->members.proc_count = ptype->members.proc_count;
    cls->base_type = ptype;
    cls->members.first_symbol = 0;
    cls->members.last_symbol = 0;
  }
  else
  {
    cls->members.parent = 0;
    cls->members.size = 0;
    cls->base_type=0;
    cls->members.proc_count=0;
    cls->members.first_symbol = 0;
    cls->members.last_symbol = 0;
  }

  // fill the body.
  for(i=0;i<pbody->count;i++)
  {
    psection=pbody->children[i];

    switch(psection->node_type)
    {
      case e_unqualified_section:
      case e_private_section:
        visibility=VIS_PRIVATE;
      break;
      case e_protected_section:
        visibility=VIS_PROTECTED;
      break;
      case e_public_section:
        visibility=VIS_PUBLIC;
      break;
      case e_published_section:
        visibility=VIS_PUBLISHED;
      break;
    }

    for(j=0;j<psection->count;j++)
    {
      class_item=psection->children[j];

      switch(class_item->node_type)
      {
        case e_variable_member:
          add_variable(&stack_node, &cls->members,class_item->children[0],class_item->children[1],visibility);
        break;
        case e_method_member:
          switch(class_item->children[0]->node_type)
          {
            case e_function_header:
              add_function_declaration(&stack_node,&cls->members,class_item,visibility);
            break;
            case e_procedure_header:
              add_procedure_declaration(&stack_node,&cls->members,class_item,visibility);
            break;
            case e_constructor_header:
              add_constructor_declaration(&stack_node,&cls->members,class_item,visibility);
            break; 
          }
        break;
        case e_property_definition:
          add_property(&stack_node,&cls->members,class_item,visibility);
        break;
      }
    }
  }

  cls->size = 4;
}

void CsyntaxValidator::build_interface(symbol_context *pns, sym_namespace *ps, interface_type *cls, syntaxNode *p)
{
  size_t i,j;
  char visibility;
  unsigned offset=0;

  syntaxNode  *pinheritance = p->get_node(e_class_heritage);
  syntaxNode  *pibody = p->get_node(e_interface_body);
  syntaxNode  *guid = pibody->get_node(e_opt_guid);
  syntaxNode  *pbody = (pibody->count>1)?pibody->children[1]:0;
  syntaxNode  *psection;
  syntaxNode  *class_item;
  class_type *ptype;
  symbol_context stack_node;

  if(guid)
  {
    if(false==check_guid_syntax(guid->children[0]->node_name))
     throw CSyntaxErr(guid->children[0]->getNodeLine(),"Improper GUID syntax.");
    
    guid->children[0]->node_name = duplicateString(get_guid_memory(guid->children[0]->node_name).c_str());
    guid->children[0]->data_type = (sym_type*)_m_punit->get_symbol("tguid",pns);
    cls->guid = guid->children[0];
    cls->guid_index = _m_punit->_m_constants->add(guid->children[0]);
  }
  else
    cls->guid = 0;

  cls->members.space_type = cls;
  cls->members.first_symbol = 0;
  cls->members.last_symbol = 0;

  stack_node.ns_var = 0;
  stack_node.last = pns;
  stack_node.ps = &cls->members;

  // figure out the size...
  if(pinheritance)
  {
    // try to get the first item in the list...
    syntaxNode *pbase = pinheritance->children[0];

    ptype=(class_type*)_m_punit->get_symbol(pbase->children[0]->node_name,&stack_node,true);

    if(!ptype)
     throw CSyntaxErr(pbase->children[0]->getNodeLine(),"Undeclared identifier : %s.", pbase->children[0]->node_name);

    if(ptype->scode!=sy_type||(ptype->tycode!=ty_interface))
      throw CSyntaxErr(pbase->children[0]->getNodeLine(),"Identifier %s is not an interface name.", pbase->children[0]->node_name);

    cls->members.parent = &ptype->members;
    cls->members.size = ptype->members.size;
    cls->base_type = ptype;
    cls->members.proc_count=ptype->members.proc_count;
  }
  else
  {
    cls->members.parent = 0;
    cls->members.size = 0;
    cls->base_type=0;
    cls->members.proc_count=0;
  }

  // fill the body.
  if(pbody)
    for(i=0;i<pbody->count;i++)
    {
      psection=pbody->children[i];

      switch(psection->node_type)
      {
        case e_unqualified_section:
          visibility=VIS_PUBLIC;
        break;
        case e_private_section:
        case e_protected_section:
        case e_public_section:
        case e_published_section:
          throw CSyntaxErr(psection->getNodeLine(),"not valid for interface types.");
        break;
      }

      for(j=0;j<psection->count;j++)
      {
        class_item=psection->children[j];

        switch(class_item->node_type)
        {
          case e_variable_member:
            throw CSyntaxErr(class_item->getNodeLine(),"interfaces do not have member variables");
          break;
          case e_method_member:
            switch(class_item->children[0]->node_type)
            {
              case e_function_header:
                add_function_declaration(pns,&cls->members,class_item,visibility);
              break;
              case e_procedure_header:
                add_procedure_declaration(pns,&cls->members,class_item,visibility);
              break; 
            }
          break;
          case e_property_definition:
            if(!cls->disponly)
              add_property(&stack_node,&cls->members,class_item,visibility);
          break;
        }
      }
    }

  cls->size = 4;
}

sym_type *CsyntaxValidator::get_type(symbol_context *ns, syntaxNode *ptype_info, sym_namespace *ps, syntaxNode *ptype_name)
{
  class_type *ctype; 
  record_type *rtype;
  interface_type *itype;
  array_type *atype;
  sym_type *ptr_type;
  size_t j;

  tlist<sym_type*> t;

  switch(ptype_info->node_type)
  {
    case e_simple_type:
      switch(ptype_info->children[0]->node_type)
      {
        case TOK_IDENTIFIER:
          // this allows using the name within the object itself
          // (in the case of record and class types)
          if(ps && ps->name && (strcmp(ps->name,ptype_info->children[0]->node_name)==0))
            return ps->space_type;
            
          ptr_type = (sym_type*)_m_punit->get_symbol(ptype_info->children[0]->node_name,ns,true);
           
          if(ptr_type && ptr_type->scode!=sy_type)
            throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"invalid type name.");

          if(ptr_type)
            _m_punit->update_undeclared(ptr_type);

          return ptr_type;
        break;
        case TOK_TYPE:
          throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"Not implemented");
        break;
        case e_sized_string:
          throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"Not implemented");
        break;
        case e_ranged_item:
          throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"Not implemented");
        break;
        case e_defptr_list:
          
          ptr_type = new sym_type;
          ptr_type->name = (ptype_name)?ptype_name->node_name:get_unknown_name(ptr_type);
          ptr_type->ptr_level = ptype_info->children[0]->count;
          ptr_type->scode = sy_type;
          ptr_type->tycode = ty_pointer;
          ptr_type->size = sizeof(void*);
          ptr_type->base_type = (sym_type*)_m_punit->get_symbol(ptype_info->children[1]->node_name,ns,true);
          ptr_type->unit_index = _m_punit->get_index();
           
          if(!ptr_type->base_type)
          {
            ptr_type->base_type = _m_punit->add_undeclared(ptype_info->children[1],&ptr_type->base_type);
          }

          if(ptr_type->base_type->scode!=sy_type)
            throw CSyntaxErr(ptype_info->children[1]->getNodeLine(),"invalid type name.");

          _m_punit->update_undeclared(ptr_type);
					_m_punit->registerSymbol(ptr_type);

          return ptr_type;
          //throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"pointer types not valid");
        break;
        case TOK_LPAREN:
          // it's an enumerated type...
          throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"Not implemented");
        break;
      }
    break;
    case e_struct_type:
      j=0;
      if(ptype_info->children[j]->node_type== TOK_PACKED)
        j++;

      switch(ptype_info->children[j]->node_type)
      {
        case TOK_ARRAY:
          if(ptype_name)
          {
            atype = (array_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

            if(atype)
              throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
          }

          atype = new array_type;
          atype->flags = TTYPE_IMPLEMENTED;     
          atype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(atype);
          atype->ptr_level = 0;
          atype->scode = sy_type;
          atype->tycode = ty_array;
          atype->size = 0;
          atype->unit_index = _m_punit->get_index();
          atype->count = 0;
          atype->base_type = 0;
          atype->dimentions = 0;
          atype->element_count = 0;

          build_array(ns,ps,atype,ptype_info->children[j+1],ptype_info->children[j+2]);
   
          _m_punit->update_undeclared(atype);
					_m_punit->registerSymbol(atype);
          
          return atype;
        break;
        case TOK_RECORD:
          // see if the record has been previously declared...
          if(ptype_name)
          {
            rtype = (record_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

            if(rtype)
              throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
          }

          if(!rtype)
            rtype = new record_type;

          rtype->flags = TTYPE_IMPLEMENTED;
          rtype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(rtype);
          rtype->members.parent = 0;
          rtype->members.name = rtype->name;
          rtype->members.first_symbol = 0;
          rtype->members.last_symbol = 0;
          rtype->members.unit_index = _m_punit->get_index();
          rtype->base_type = 0;
          rtype->tycode = ty_record;
          rtype->scode = sy_type;
          rtype->ptr_level = 0;
          rtype->unit_index = _m_punit->get_index();

          build_record(ns,rtype,ptype_info->children[j+1]);

          _m_punit->update_undeclared(rtype);
          _m_punit->registerSymbol(rtype);

          return rtype;
        break;
        case TOK_SET:
					throw CSyntaxErr(ptype_name->getNodeLine(),"Not Implemented\n");
        break;
      }

    break;
    case e_class_type:
      if(ptype_name)
      {
        ctype = (class_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

        if(ctype)
          throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
      }
       
      if(!ctype)
        ctype = new class_type;
      
      ctype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(ctype);
      ctype->flags = TTYPE_IMPLEMENTED;
      ctype->members.parent = 0;
      ctype->members.name = ctype->name;
      ctype->members.unit_index = _m_punit->get_index();
      ctype->base_type = 0;
      ctype->tycode = ty_class;
      ctype->scode = sy_type;
      ctype->ptr_level = 1;
      ctype->unit_index = _m_punit->get_index();

      build_class(ns,ps,ctype,ptype_info);

      _m_punit->update_undeclared(ctype);
			_m_punit->registerSymbol(ctype);

      return ctype;
    break;
    case e_object_type:
      throw CSyntaxErr(ptype_info->children[0]->getNodeLine(),"Not implemented");
    break;
    case e_interface_forward_decl:
      if(ptype_name)
      {
        itype = (interface_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

        if(itype)
        {
          if(itype->tycode != ty_interface)
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);

          if(itype->flags & TTYPE_IMPLEMENTED)
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
        }
      }

      if(!itype)
      {
        itype = new interface_type;
        itype->disponly = 0;
        itype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(itype);
        itype->members.parent = 0;
        itype->members.name = itype->name;
        itype->members.unit_index = _m_punit->get_index();
        itype->base_type = 0;
        itype->tycode = ty_interface;
        itype->scode = sy_type;
        itype->ptr_level = 1;
        itype->unit_index = _m_punit->get_index();
        itype->size = 4;
				
				_m_punit->registerSymbol(itype);
      }

      itype->flags = 0;

      return itype;
    break;
    case e_disp_forward_decl:
      if(ptype_name)
      {
        itype = (interface_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

        if(itype)
        {
          if(itype->tycode != ty_interface)
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);

          if(itype->flags & TTYPE_IMPLEMENTED)
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
        }
      }

      if(!itype)
      {
        itype = new interface_type;
        itype->disponly = 1;
        itype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(itype);
        itype->members.parent = 0;
        itype->members.name = itype->name;
        itype->members.unit_index = _m_punit->get_index();
        itype->base_type = 0;
        itype->tycode = ty_interface;
        itype->scode = sy_type;
        itype->ptr_level = 1;
        itype->unit_index = _m_punit->get_index();
        itype->size = 4;
        _m_punit->registerSymbol(itype);
			}

      itype->flags = 0;

      return itype;
    break;
    case e_interface_type:
    case e_dispinterface_type:
      if(ptype_name)
      {
        itype = (interface_type*)_m_punit->get_symbol(ptype_name->node_name,ns);

        if(itype)
        {
          if((itype->tycode != ty_interface) && (itype->tycode != ty_undeclared))
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);

          if(itype->flags & TTYPE_IMPLEMENTED)
            throw CSyntaxErr(ptype_name->getNodeLine(),"identifier redeclared : %s.",ptype_name->node_name);
        }
      }

      if(!itype)
      {
        itype = new interface_type;
        itype->disponly = (ptype_info->node_type==e_dispinterface_type)?1:0; 
        itype->name = (ptype_name)?ptype_name->node_name:get_unknown_name(itype);
        itype->members.parent = 0;
        itype->members.name = itype->name;
        itype->members.unit_index = _m_punit->get_index();
        itype->base_type = 0;
        itype->tycode = ty_interface;
        itype->scode = sy_type;
        itype->ptr_level = 1;
        itype->unit_index = _m_punit->get_index();
				_m_punit->registerSymbol(itype);
      }
      
      itype->flags = TTYPE_IMPLEMENTED;
      
      build_interface(ns,ps,itype,ptype_info);

      return itype;
    break;
    case e_function_type:
      throw CSyntaxErr(ptype_info->getNodeLine(),"Not Implemented");
    break;
    case e_procedure_type:
      throw CSyntaxErr(ptype_info->getNodeLine(),"Not Implemented");
    break;
  }

  return 0;
}

bool is_numeric(syntaxNode *p)
{
  sym_type *ptype=get_expr_type(p);

  if(!ptype)
    return false;

  switch(ptype->tycode)
  {
    case ty_integer:
    case ty_real:
      return true;
    break;
  }

  return false;
}

bool CsyntaxValidator::is_equal_type(sym_type *t1, sym_type *t2,bool strict)
{
  if(t1->tycode != t2->tycode)
  {
    if(!strict)
    {
      // allow numeric types to be equal...
      if(t1->tycode==ty_integer && t2->tycode == ty_real)
        return true;
      if(t2->tycode==ty_integer && t1->tycode == ty_real)
        return true;
      //allow string types to be equal
      if(t2->tycode==ty_string && t1->tycode ==ty_widestring)
        return true;
      if(t1->tycode==ty_string && t2->tycode==ty_widestring)
        return true;
      
      // allow variant types to match integer,real,string and interface
      // types.
      if(t1->tycode==ty_char&& t2->tycode == ty_integer)
        return true;
      if(t2->tycode==ty_char&& t1->tycode == ty_integer)
        return true;
      if(t1->tycode==ty_char&& t2->tycode == ty_real)
        return true;
      if(t2->tycode==ty_char&& t1->tycode == ty_real)
        return true;
      if(t1->tycode==ty_boolean&& t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_boolean&& t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_char&& t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_char&& t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_integer && t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_integer && t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_real && t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_real && t1->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_string && t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_string && t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_widestring && t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_widestring && t2->tycode == ty_variant)
        return true;
      if(t2->tycode==ty_interface && t1->tycode == ty_variant)
        return true;
      if(t1->tycode==ty_interface && t2->tycode == ty_variant)
        return true;
    }

    if((t1->tycode==ty_interface)&&(t2->tycode==ty_pointer))
      return true;
    if((t2->tycode==ty_interface)&&(t1->tycode==ty_pointer))
      return true;

    return false;
  }

  // don't allow types of different size to be equal.
  if((strict)&&(t1->size != t2->size))
    return false;

  if(t1->tycode == ty_pointer)
  {
    // check the level..
    if(t1->ptr_level!=t2->ptr_level)
      return false;

    if(t1->base_type && t2->base_type)
      return is_equal_type(t1->base_type,t2->base_type);
  }

  if((t1->tycode == ty_record)||(t1->tycode == ty_class)||(t1->tycode==ty_interface))
  {
    if(t1==t2)
      return true;

    sym_type *pctype;

    if(!strict && ((t1->tycode == ty_class) || (t1->tycode == ty_interface)))
    {
      // allow class types to be equal if the first object is a 
      // base type of the second one...
      pctype = t2->base_type;

      while(pctype)
      {
        if(t1==pctype)
          return true;
        pctype=pctype->base_type;
      }
    }

    return false;
  }

  return true;
}

sym_type *get_expr_type(syntaxNode *p)
{
  if(!p->data_type)
    return 0;

  switch(p->data_type->scode)
  {
    case sy_variable:
      return p->variable->type;
    break;
    case sy_type:
      return p->data_type;
    break;
    case sy_procedure:
      return p->procedure->ret_type;
    break;
  }

  return 0;
}

int expr_val_int(symbol_context *ns,syntaxNode *p)
{
  int val = atoi(p->node_name);
  if(p->negate_)
    val *= -1;
  return val;
}

double expr_val_dbl(symbol_context *ns,syntaxNode *p)
{
  double val=atof(p->node_name);
  if(p->negate_)
    val *= -1;
  return val;
}

void CsyntaxValidator::exp_mult(symbol_context *ns, syntaxNode *expr1, syntaxNode *expr2, int op)
{
  sym_type *ptyp1=get_expr_type(expr1);
  sym_type *ptyp2=get_expr_type(expr2);
  std::string number_str;

  int ival;
  int itmp;
  double dblval;
  double dbltmp;

  if((ptyp1->tycode==ty_boolean)||(ptyp2->tycode==ty_boolean)||
     (ptyp1->tycode==ty_char)||(ptyp2->tycode==ty_char)||
     (ptyp1->tycode==ty_string)||(ptyp2->tycode==ty_string)||
     (ptyp1->tycode==ty_widestring)||(ptyp2->tycode==ty_widestring)||
     (ptyp1->tycode==ty_record)||(ptyp2->tycode==ty_record)||
     (ptyp1->tycode==ty_class)||(ptyp2->tycode==ty_class)||
     (ptyp1->tycode==ty_interface)||(ptyp2->tycode==ty_interface)||     
     (ptyp1->tycode==ty_array)||(ptyp2->tycode==ty_array)||     
     (ptyp1->tycode==ty_set)||(ptyp2->tycode==ty_set)||     
     (ptyp1->tycode==ty_function)||(ptyp2->tycode==ty_function)||     
     (ptyp1->tycode==ty_procedure)||(ptyp2->tycode==ty_procedure)||     
     (ptyp1->tycode==ty_pointer)||(ptyp2->tycode==ty_pointer))     
     throw CSyntaxErr(expr1->getNodeLine(),"invalid type found.");

  switch(ptyp1->tycode)
  {
    case ty_integer:
      switch(ptyp2->tycode)
      {
        case ty_integer:
          switch(op)
          {
            case TOK_STAR:
              ival = expr_val_int(ns,expr1) * expr_val_int(ns,expr2);
              number_str = tostring(ival);//sprintf(number_buffer,"%i",ival);
            break;
            case TOK_DIV:
              if(!(itmp = expr_val_int(ns,expr2)))
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              ival = expr_val_int(ns,expr1) / itmp;
              number_str = tostring(ival);//sprintf(number_buffer,"%i",ival);
            break;
            case TOK_SLASH:
              expr1->data_type = REAL_TYPE;
              if((dbltmp = (double)expr_val_int(ns,expr2)) == 0)
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              dblval = ((double)expr_val_int(ns,expr1))/dbltmp;
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);         
            break;
            case TOK_MOD:
              if(!(itmp = expr_val_int(ns,expr2)))
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              ival = expr_val_int(ns,expr1) % itmp;
              number_str = tostring(ival);//sprintf(number_buffer,"%i",ival);
            break;
          }
        break;
        case ty_real:
          switch(op)
          {
            case TOK_STAR:
              expr1->data_type = REAL_TYPE;
              dblval = ((double)expr_val_int(ns,expr1))*expr_val_dbl(ns,expr2);
              number_str = tostring(dblval);///sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_DIV:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
            case TOK_SLASH:
              expr1->data_type = REAL_TYPE;
              if((dbltmp = expr_val_dbl(ns,expr2)) == 0)
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              dblval = ((double)expr_val_int(ns,expr1))/dbltmp;
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_MOD:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
          }
        break;
      }
    break;
    case ty_real:
      switch(ptyp2->tycode)
      {
        case ty_integer:
          switch(op)
          {
            case TOK_STAR:
              dblval = expr_val_dbl(ns,expr1)*(double)expr_val_int(ns,expr2);
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_DIV:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
            case TOK_SLASH:
              if((dbltmp = (double)expr_val_int(ns,expr2)) == 0)
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              dblval = expr_val_dbl(ns,expr1)/dbltmp;
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_MOD:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
          }
        break;
        case ty_real:
          switch(op)
          {
            case TOK_STAR:
              dblval = expr_val_dbl(ns,expr1)*expr_val_dbl(ns,expr2);
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_DIV:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
            case TOK_SLASH:
              if((dbltmp = expr_val_dbl(ns,expr2) == 0))
                throw CSyntaxErr(expr2->getNodeLine(),"division by zero.");
              dblval = expr_val_dbl(ns,expr1)/dbltmp;
              number_str = tostring(dblval);//sprintf(number_buffer,"%e",dblval);
            break;
            case TOK_MOD:
              throw CSyntaxErr(expr2->getNodeLine(),"invalid mod expression.");
            break;
          }
        break;
      }
    break;
  }

  expr1->node_name = duplicateString(number_str.c_str());
}

void CsyntaxValidator::exp_add(symbol_context *ns, syntaxNode *expr1, syntaxNode *expr2, int op)
{
  sym_type *ptyp1=get_expr_type(expr1);
  sym_type *ptyp2=get_expr_type(expr2);
  std::string number_str;
  
  int ires;
  double dblres;
  
  // cannot add any of these types...
  if((ptyp1->tycode==ty_boolean)||(ptyp2->tycode==ty_boolean)||
     (ptyp1->tycode==ty_char)||(ptyp2->tycode==ty_char)||
     (ptyp1->tycode==ty_record)||(ptyp2->tycode==ty_record)||
     (ptyp1->tycode==ty_class)||(ptyp2->tycode==ty_class)||
     (ptyp1->tycode==ty_interface)||(ptyp2->tycode==ty_interface)||     
     (ptyp1->tycode==ty_array)||(ptyp2->tycode==ty_array)||     
     (ptyp1->tycode==ty_set)||(ptyp2->tycode==ty_set)||     
     (ptyp1->tycode==ty_function)||(ptyp2->tycode==ty_function)||     
     (ptyp1->tycode==ty_procedure)||(ptyp2->tycode==ty_procedure)||     
     (ptyp1->tycode==ty_pointer)||(ptyp2->tycode==ty_pointer))     
    throw CSyntaxErr(expr1->getNodeLine(),"invalid type found.");
  
  switch(ptyp1->tycode)
  {
    case ty_integer:
      switch(ptyp2->tycode)
      {
        case ty_integer:
          ires = (op==TOK_PLUS)?
                 (expr_val_int(ns,expr1)+expr_val_int(ns,expr2)):
                 (expr_val_int(ns,expr1)-expr_val_int(ns,expr2));
          number_str = tostring(ires);//sprintf(number_buffer,"%i",ires);
        break;
        case ty_real:
          dblres = (op==TOK_PLUS)?
                   (expr_val_int(ns,expr1)+expr_val_dbl(ns,expr2)):
                   (expr_val_int(ns,expr1)-expr_val_dbl(ns,expr2));
          expr1->data_type=ptyp2;
          number_str = tostring(dblres);//sprintf(number_buffer,"%e",dblres);         
        break;
      }
      expr1->node_name=duplicateString(number_str.c_str());
      expr1->negate_ = 0;
    break;
    case ty_real:
      switch(ptyp2->tycode)
      {
        case ty_integer:
          dblres = (op==TOK_PLUS)?
                   (expr_val_dbl(ns,expr1)+expr_val_int(ns,expr2)):
                   (expr_val_dbl(ns,expr1)-expr_val_int(ns,expr2));
          number_str = tostring(dblres);//sprintf(number_buffer,"%e",dblres);         
        break;
        case ty_real:
          dblres = (op==TOK_PLUS)?
                   (expr_val_dbl(ns,expr1)+expr_val_dbl(ns,expr2)):
                   (expr_val_dbl(ns,expr1)-expr_val_dbl(ns,expr2));
          number_str = tostring(dblres);//fssprintf(number_buffer,"%e",dblres);         
        break;
      }
      expr1->node_name=duplicateString(number_str.c_str());
      expr1->negate_ = 0;
    break;
    case ty_string:
    case ty_widestring:
      char *ptmp = (char*)malloc(strlen(expr1->node_name)+strlen(expr2->node_name)+1);
      sprintf(ptmp,"%s%s",expr1->node_name,expr2->node_name);
      expr1->node_name=duplicateString(ptmp);
      free(ptmp);
    break;
  }
}

//*************************************************************************************
// Expression Evaluation
//*************************************************************************************
void CsyntaxValidator::reduce_method_expression(symbol_context *nstack, syntaxNode *p, bool is_record)
{
  syntaxNode *pparameters = p->get_node(e_expression_list);
  sym_procedure *pproc;
  sym_variable *pvar;
  size_t i;
  symbol_context *last_stack;
  sym_location location;

  switch(p->children[0]->node_type)
  {
    case TOK_INHERITED:
			throw CSyntaxErr(p->children[0]->getNodeLine(),"Not implemented");
    break;
    case TOK_IDENTIFIER:
      if(!pparameters)
      {
        reduce_simple(nstack,p,is_record);

        if(p->variable->scode != sy_procedure)
          throw CSyntaxErr(p->children[0]->getNodeLine(),"%s is not a valid procedure or type.", p->node_name);
       
        if(p->procedure->overloaded && p->procedure->next_overload)
        {
           pproc = get_matching_overload(p->children[0]->getNodeLine(),p->procedure,pparameters);
              
           if(!pproc)
             throw CSyntaxErr(p->children[0]->getNodeLine(),
             "There is no overloaded version of %s that can be called with these arguments.", p->children[0]->node_name);
        
           p->procedure = pproc;
        }
        
        if(p->procedure->param_count>0)
        {
          if(!p->procedure->parameters[0]->default_)
            throw CSyntaxErr(p->children[0]->getNodeLine(),"not enough parameters.", p->node_name);
        }
      }
      else
      {
        // find the symbol for this procedure
        if(!is_record)
        {
          p->data_type = (sym_type*)_m_punit->get_symbol(p->children[0]->node_name,nstack,true,&location);
          p->implied_var = (location.ns)?location.ns->ns_var:0;
        }
        else
        {
          p->data_type=(sym_type*)nstack->ps->get_member(p->children[0]->node_name);
          p->implied_var = 0;
        }

        if(!p->data_type)
        {
          if(is_record)
            throw CSyntaxErr(p->children[0]->getNodeLine(),"%s is not a member of %s.",p->children[0]->node_name, nstack->ps->name);
          else
            throw CSyntaxErr(p->children[0]->getNodeLine(),"Undeclared identifier : %s.", p->children[0]->node_name);
        }

        last_stack = (nstack->last && is_record)?nstack->last:nstack;

        // it should be a procedure or a type, in which case we are type casting
        switch(p->procedure->scode)
        {
          case sy_procedure:
            for(i=0;i<pparameters->count;i++)
            {
              reduce_expression(last_stack,pparameters->children[i],false);

              if(_m_punit->is_const(last_stack,pparameters->children[i])&&pparameters->children[i]->variable->scode!=sy_variable)
                _m_punit->_m_constants->add(pparameters->children[i]);
              
              reduce_properties_read(pparameters->children[i]);
            }

            if(p->procedure->overloaded && p->procedure->next_overload)
            {
              pproc = get_matching_overload(p->children[0]->getNodeLine(),p->procedure,pparameters);
              
              if(!pproc)
                throw CSyntaxErr(p->children[0]->getNodeLine(),
                "There is no overloaded version of %s that can be called with these arguments.", p->children[0]->node_name);
              p->procedure = pproc;   
            }
            else
              pproc = p->procedure;            
                  
            // make sure the parameter count is the same 
            if(pproc->param_count!=pparameters->count)
            {
              if(pparameters->count<pproc->param_count)
              {
                if(!pproc->parameters[pparameters->count]->default_)
                  throw CSyntaxErr(p->children[0]->getNodeLine(),"not enough parameters.");
              }
              else
                throw CSyntaxErr(p->children[0]->getNodeLine(),"%s does not take %i parameters.", pproc->name, pparameters->count );
            }

            for(i=0;i<pparameters->count;i++)
            {
              if(pproc->parameters[i]->byref)
              {
                pvar=pparameters->children[i]->variable;
                switch(pvar->scode)
                {
                  case sy_procedure:
                  case sy_type:
                    throw CSyntaxErr(pparameters->children[i]->getNodeLine(),"types of formal and actual parameter must be equal.");
                  break;
                  case sy_variable:
                    if((pvar->const_)&&strcmp(pvar->type->name,"tguid"))
                      throw CSyntaxErr(pparameters->children[i]->getNodeLine(),"types of formal and actual parameter must be equal.");
                  break;
                }
              }
            
              if(!is_equal_type(pproc->parameters[i]->type,get_expr_type(pparameters->children[i]),(pproc->parameters[i]->byref)?true:false))
                throw CSyntaxErr(pparameters->children[i]->getNodeLine()," parameter %i differs from the declaration type.",i+1);
            }
          break;
          default:
            throw CSyntaxErr(p->children[0]->getNodeLine(),"%s is not a procedure.", p->children[0]->node_name);
        }
      }
    break;
  }
}

void CsyntaxValidator::reduce_array_elt_expr(symbol_context *ns, syntaxNode *p, bool is_record)
{
  std::vector<syntaxNode*> expressions;
  size_t i,j;
  int val;

  syntaxNode *pexpr,*plist, *pcurr;
  array_type *parr_type=0;
  sym_variable *pprop_variable=0;
  sym_procedure *pprop_procedure=0;
  sym_type *pexpr_type;
  sym_type *pdim_type;
  array_bound *pbound;
  
  pcurr=p;

  do
  {
    pexpr=pcurr->children[0];
    plist=pcurr->children[1];

    expressions.push_back(plist);

    if(pexpr->children[0]->node_type == e_array_elt_expression)
      pcurr=pexpr->children[0];
  }
  while(pexpr->children[0]->node_type == e_array_elt_expression);

  if(expressions.size()>1)
  {
    plist=_m_punit->get_syntax_tree()->createNode(e_expression_list);

    for(i=0;i<expressions.size();i++)
    {
      pcurr=expressions[i];
      for(j=pcurr->count;j>0;j--)
        plist->prependChild(pcurr->children[j-1]);
      
      pcurr->clear();
    }

    p->children[1] = plist;
  }

  // get the array object
  reduce_expression(ns,pexpr,is_record);

  switch(pexpr->variable->scode)
  {
    case sy_procedure:
      parr_type=(array_type*)pexpr->procedure->ret_type;
    break;
    case sy_type:
      parr_type=(array_type*)pexpr->data_type;
    break;
    case sy_variable:
      if(pexpr->variable->prop_info)
      {
        pprop_variable=pexpr->variable;

        if(pexpr->variable->prop_info->array_proc)
          pprop_procedure=pexpr->variable->prop_info->array_proc;
        else
          parr_type=(array_type*)pprop_variable->type;
      }
      else
        parr_type=(array_type*)pexpr->variable->type;
    break;
  }

  if(parr_type)
  {
    if(parr_type->tycode != ty_array)
      throw CSyntaxErr(pexpr->getNodeLine(),"Array type required.");
  
    if(plist->count != parr_type->count)
      throw CSyntaxErr(plist->getNodeLine(),"Unexpected number of expressions.");
  }
  else
  {
    throw CSyntaxErr(plist->getNodeLine(),"Not implemented");
  }

  p->cindex = 0;

  for(j=0;j<plist->count;j++)
  {
    reduce_expression(ns,plist->children[j],false);

    pexpr_type=get_expr_type(plist->children[j]);
    
    if(parr_type)
      pdim_type=parr_type->dimentions[j]->bound_type;
    else
      pdim_type=pprop_procedure->parameters[j]->type;
 
    if(_m_punit->is_const(ns,plist->children[j]))
    {
      p->cindex++;

      // get the value of the expression...
      switch(pexpr_type->tycode)
      {
        case ty_integer:
          val = expr_val_int(0,plist->children[j]);
        break;
        case ty_string:
          if(strlen(plist->children[j]->node_name)!=1)
            throw CSyntaxErr(plist->children[j]->getNodeLine(),"Incompatible types : %s and %s.",
                             pdim_type->name,pexpr_type->name);

          pexpr_type = CHAR_TYPE;
          plist->children[j]->data_type = CHAR_TYPE;
          val = plist->children[j]->node_name[0];
        break;
        case ty_char:
          val = plist->children[j]->node_name[0];
        break;
        case ty_boolean:
          val = (plist->children[j]->node_name[0]=='t')?1:0;
        break;
        default:
          throw CSyntaxErr(plist->children[j]->getNodeLine(),"Incompatible types : %s and %s.",
                           pdim_type->name,pexpr_type->name);
      }

      // make sure the expression fits the bounds...
      if(parr_type)
      {
        pbound = parr_type->dimentions[j];

        if((pbound->high < val)||(pbound->low > val))
          throw CSyntaxErr(plist->children[j]->getNodeLine(),"Constant expression violates subrange bounds.");
      }

      // add the expression
      if(plist->children[j]->variable->scode!=sy_variable)
        _m_punit->_m_constants->add(plist->children[j]);
    }

 
    if(!is_equal_type(pexpr_type,pdim_type,true))
      throw CSyntaxErr(plist->children[j]->getNodeLine(),"Incompatible types : %s and %s.",
                       pdim_type->name,pexpr_type->name);
  
    reduce_properties_read(plist->children[j]);
  }

  p->children[0] = pexpr;
  
  if(parr_type)
    p->variable = parr_type->pvar;
  else
    p->variable = pprop_variable;
}

void CsyntaxValidator::reduce_simple(symbol_context *ns, syntaxNode *p, bool is_record)
{
  size_t i=0;
  sym_value val;
  sym_type *ptype;
  syntaxNode *pderef;
  std::vector<syntaxNode*> &pchildren=p->children;
  sym_location location;
  std::string num_buffer;
  std::string number_str;
  
  p->negate_ = 0;

  if(p->children[0]->node_type == TOK_MINUS)
  {
    i++;
    p->negate_ = 1;
  }
  else
  if(p->children[0]->node_type == TOK_PLUS)
    i++;

  switch(p->children[i]->node_type)
  {
    case TOK_INTEGER:
      if(p->negate_)
      {
        //sprintf(num_buffer,"%i",atoi(p->children[i]->node_name) * -1);
        num_buffer = tostring(atoi(p->children[i]->node_name) * -1);
        p->node_name = duplicateString(num_buffer.c_str());
        p->negate_ = 0;
      }
      else
        p->node_name = p->children[i]->node_name;
      p->node_type = p->children[i]->node_type;
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->data_type = INTEGER_TYPE;
    break;
    case TOK_HEX:
      //sprintf(num_buffer,"%i",strtoul(&p->children[i]->node_name[1],0,16));
      num_buffer=tostring(strtoul(&p->children[i]->node_name[1],0,16));
      p->node_type = p->children[i]->node_type;
      p->node_name = duplicateString(num_buffer.c_str());
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->negate_=0;
      
     p->data_type = INTEGER_TYPE;
    break;
    case TOK_REAL:
      if(p->negate_)
      {
        //sprintf(num_buffer,"%f",atof(p->children[i]->node_name) * -1);
        num_buffer=tostring(atof(p->children[i]->node_name) * -1);
        p->node_name = duplicateString(num_buffer.c_str());
        p->negate_ = 0;
      }
      else
        p->node_name = p->children[i]->node_name;
      p->node_type = p->children[i]->node_type;
      p->node_name = p->children[i]->node_name;
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->data_type = REAL_TYPE;
    break;
    case TOK_STRING:
      p->node_type = p->children[i]->node_type;
      p->node_name = p->children[i]->node_name;
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->data_type = STRING_TYPE;
    break;
    case TOK_TRUE:
    case TOK_FALSE:
      if(i)
        throw CSyntaxErr(p->children[i]->getNodeLine(),"invalid expression.");

      p->node_type = p->children[0]->node_type;
      p->node_name = p->children[0]->node_name;
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->data_type = BOOLEAN_TYPE;
    break;
    case TOK_NIL:
      if(i)
        throw CSyntaxErr(p->children[i]->getNodeLine(),"invalid expression.");

      p->node_type = p->children[0]->node_type;
      p->node_name = p->children[0]->node_name;
      p->node_line = p->children[i]->node_line;
      p->clear();
      p->data_type = VOID_TYPE;
    break;
    case TOK_IDENTIFIER:
      if(strcmp(p->children[0]->node_name,"exit")==0)
        break;

      if(strcmp(p->children[0]->node_name,"break")==0)
      {
        // special case functions...
        if(this->_m_loopContext.size()==0)
          throw CSyntaxErr(p->children[0]->getNodeLine(),"illegal break");

        break;
      }

      if(!is_record)
      {
        p->data_type = (sym_type*)_m_punit->get_symbol(p->children[0]->node_name,ns,true,&location);
        p->implied_var = (location.ns)?location.ns->ns_var:0;
      }
      else
      {
        p->data_type=(sym_type*)ns->ps->get_member(p->children[0]->node_name);
        p->implied_var = 0;
      }
      if(!p->data_type)
      {
        if(is_record)
          throw CSyntaxErr(p->children[i]->getNodeLine(),"%s is not a member of %s.",p->children[i]->node_name, ns->ps->name);
        else
        {
          throw CSyntaxErr(p->children[i]->getNodeLine(),"Undeclared identifier : %s.", p->children[i]->node_name);
        }
      }
      // we might be trying to dereference it...
      pderef = p->get_node(e_defptr_list);
      p->deref_count=(pderef)?pderef->count:0;

      switch(p->data_type->scode)
      {
        case sy_type:
          p->node_type = TOK_IDENTIFIER;
          p->node_name = p->variable->name;
          p->node_line = p->children[i]->node_line;
          p->clear();
      
        break;

        case sy_variable:
          // it might be a named constant, in which case, we just reduce it to be a number...
          if(p->variable->const_)
          {
            ptype=p->variable->type;
            val=p->variable->value;

            // can't dereference a const...
            if(p->deref_count>0)
              throw CSyntaxErr(p->children[i]->getNodeLine(),"cannot dereference a const.");

            switch(ptype->tycode)
            {
              case ty_integer:
                number_str = tostring(val.int_val);//sprintf(number_buffer,"%i", val.int_val);
                p->node_name = duplicateString(number_str.c_str());
                p->node_type = TOK_INTEGER;
                p->data_type = INTEGER_TYPE;
                p->node_line = p->children[i]->node_line;
                p->clear();
              break;
              case ty_real:
                number_str = tostring(val.real_val);//sprintf(number_buffer,"%e", val.real_val);
                p->node_name = duplicateString(number_str.c_str());
                p->node_type = TOK_REAL;
                p->data_type = REAL_TYPE;
                p->node_line = p->children[i]->node_line;
                p->clear();
              break;
              case ty_string:
              case ty_widestring:
                p->node_name = val.string_val;
                p->data_type = (p->data_type->scode==ty_string)?STRING_TYPE:WIDESTRING_TYPE;
                p->node_line = p->children[i]->node_line;
                p->clear();
      
              break;
              case ty_record:
              case ty_variant:
                // it's a record constant...
                p->node_type = p->children[0]->node_type;
                p->node_name = p->children[0]->node_name;
                p->node_line = p->children[0]->node_line;
                p->clear();
      
              break;
            }
          }
          else
          {
            p->node_type = TOK_IDENTIFIER;
            p->node_name = p->variable->name;
            p->node_line = p->children[i]->node_line;
            p->clear();
          }
        break;
        case sy_procedure:
          // it's actually a procedure / function call
          p->node_type = e_proc_func_expression;
          p->node_name = duplicateString(syntaxNode::syntaxNodeNames[e_proc_func_expression-e_goal]);
          p->node_line = p->children[i]->node_line;

          if(p->procedure->overloaded && p->procedure->next_overload)
          {
            p->procedure = get_matching_overload(p->children[0]->getNodeLine(),p->procedure,0);
              
            if(!p->procedure)
              throw CSyntaxErr(p->children[0]->getNodeLine(),
              "There is no overloaded version of %s that can be called with these arguments.", p->children[0]->node_name);
          }
          
          // check to make sure there are enough parameters...
          if(p->procedure->param_count>0)
          {
            // the first parameter must have a default value, otherwise, there aren't enough params.
            if(!p->procedure->parameters[0]->default_)
              throw CSyntaxErr(p->getNodeLine(),"not enough parameters.", p->node_name);
          }
        break;
      }
    break;
    case e_proc_func_expression:
      *p = *p->children[0];
      reduce_method_expression(ns,p,is_record);
    break;
    case e_array_elt_expression:
      *p = *p->children[0];
      reduce_array_elt_expr(ns,p,is_record);
    break;
    case e_record_expression:
      *p = *p->children[0];
      reduce_record_expr(ns,p,is_record);
    break;
  }
}

#ifdef _WIN32
void CsyntaxValidator::reduce_dispatch(size_t i, symbol_context *ns, syntaxNode *p)
{
  syntaxNode *pnode;
  syntaxNode *pparameters;
  std::vector<syntaxNode*> expressions;
  size_t j,k;
  syntaxNode *pexpr,*plist, *pcurr;

  for(;i<p->count;i++)
  {
    pnode = p->children[i];
    std::vector<syntaxNode *> &pchildren = pnode->children;
    
    switch(pnode->children[0]->node_type)
    {
      case TOK_IDENTIFIER:
        pnode->node_type = TOK_IDENTIFIER;
        pnode->node_name = pnode->children[0]->node_name;
        pnode->node_line = pnode->children[0]->node_line;
        pnode->clear();
        pnode->data_type = WIDESTRING_TYPE;
        _m_punit->_m_constants->add(pnode);
        pnode->data_type=0;
        pnode->flags = DISPATCH_METHOD|DISPATCH_PROPERTYGET;
      break;
      case e_proc_func_expression:
        *pnode = *pnode->children[0];
        pnode->flags = DISPATCH_METHOD|DISPATCH_PROPERTYGET;
        
        pnode->children[0]->data_type = WIDESTRING_TYPE;
        _m_punit->_m_constants->add(pnode->children[0]);
        pnode->children[0]->data_type = 0;
        
        pparameters = pnode->get_node(e_expression_list);

        if(pparameters)
        {
          for(j=0;j<pparameters->count;j++)
          {
            reduce_expression(ns,pparameters->children[j],false);

            if(_m_punit->is_const(ns,pparameters->children[j])&&pparameters->children[j]->variable->scode!=sy_variable)
              _m_punit->_m_constants->add(pparameters->children[j]);
            
            reduce_properties_read(pparameters->children[j]);
          }
        }
      break;
      case e_array_elt_expression:
        *pnode = *pnode->children[0];
        pnode->flags = DISPATCH_METHOD|DISPATCH_PROPERTYGET;
      
        pcurr=pnode;

        expressions.clear();

        do
        {
          pexpr=pcurr->children[0];
          plist=pcurr->children[1];

          expressions.push_back(plist);

          if(pexpr->children[0]->node_type == e_array_elt_expression)
            pcurr=pexpr->children[0];
        }
        while(pexpr->children[0]->node_type == e_array_elt_expression);

        if(expressions.size()>1)
        {
          plist=_m_punit->get_syntax_tree()->createNode(e_expression_list);

          for(k=0;k<expressions.size();k++)
          {
            pcurr=expressions[k];
            for(j=pcurr->count;j>0;j--)
              plist->prependChild(pcurr->children[j-1]);
            pcurr->clear();
          }

          pnode->children[1] = plist;
        }

        if(pexpr->children[0]->node_type!=TOK_IDENTIFIER)
          throw CSyntaxErr(pexpr->getNodeLine(),"Invalid expression.");
        else
        {
          pexpr->node_type = TOK_IDENTIFIER;
          pexpr->node_name = pexpr->children[0]->node_name;
          pexpr->node_line = pexpr->children[0]->node_line;
          pexpr->clear();
          pexpr->data_type = WIDESTRING_TYPE;
          _m_punit->_m_constants->add(pexpr);
        }

        for(j=0;j<plist->count;j++)
        {
          reduce_expression(ns,plist->children[j],false);

          if(_m_punit->is_const(ns,plist->children[j]))
          {
            // add the expression
            if(plist->children[j]->variable->scode!=sy_variable)
              _m_punit->_m_constants->add(plist->children[j]);
          }
          else
            reduce_properties_read(plist->children[j]);
        }
      break;
    }
  }
}
#endif

void CsyntaxValidator::reduce_record_expr(symbol_context *ns, syntaxNode *p, bool is_record)
{
  sym_type *ptype;
  symbol_context stack_node;
  bool type_seen=false;
  size_t i;

  reduce_expression(ns,p->children[0],is_record);

  stack_node.ns_var = 0;
  stack_node.last = ns;
  ns = &stack_node;

  if(p->children[0]->variable->scode == sy_type)
    type_seen=true;

  for(i=0;i<p->count-1;i++)
  {
    ptype=get_expr_type(p->children[i]);

    if(!ptype)
      throw CSyntaxErr(p->children[i]->getNodeLine(),"Record, object, or class type required.");

    switch(ptype->tycode)
    {
      case ty_record:
        if(type_seen)
          throw CSyntaxErr(p->children[i]->getNodeLine(),"records do not have constructors.");
        ns->ps = &((record_type*)ptype)->members;
        reduce_expression(ns,p->children[i+1],true);
      break;
      case ty_class:
        ns->ps = &((class_type*)ptype)->members;
        reduce_expression(ns,p->children[i+1],true);

        if(type_seen)
        {
          // we must have either a static variable / procedure or a constructor...
          switch(p->children[i+1]->procedure->scode)
          {
            case sy_procedure:
              if(!p->children[i+1]->procedure->static_proc)
              {
                if(p->children[i+1]->procedure->constructor!=1)
                  throw CSyntaxErr(p->children[i]->getNodeLine(),"not a constructor function.");
              }
            break;
            case sy_variable:
              if(!p->children[i+1]->variable->static_var)
                throw CSyntaxErr(p->children[i]->getNodeLine(),"not a constructor function.");
            break;
            default:
              throw CSyntaxErr(p->children[i]->getNodeLine(),"not a constructor function.");
          }
        }
      break;
      case ty_interface:
        if(type_seen)
          throw CSyntaxErr(p->children[i]->getNodeLine(),"innterfaces do not have constructors.");
        
        #ifdef _WIN32
        if(ptype == DISPATCH_TYPE)
        {
          reduce_dispatch(i+1,ns->last,p);
          p->data_type=VARIANT_TYPE;
          return;
        }
        else
        {
        #endif
          ns->ps = &((interface_type*)ptype)->members;
          reduce_expression(ns,p->children[i+1],true);
        #ifdef _WIN32
        }
        #endif
      break;
      case ty_variant:
        #ifdef _WIN32
        reduce_dispatch(i+1,ns->last,p);
        p->data_type=VARIANT_TYPE;
        return;
        #endif
      break;
      default:
        throw CSyntaxErr(p->children[i]->getNodeLine(),"Record, object, or class type required.");
    }
  }  
  
  p->data_type=p->children[i]->data_type;
}

void CsyntaxValidator::reduce_logical_or(symbol_context *ns, syntaxNode *p)
{
  size_t i = 0;
  sym_type *ptype;

  // reduce each expression and make sure they are each 
  // boolean...
  p->data_type = BOOLEAN_TYPE;
  
  for(i=0;i<p->count;i++)
  {
    reduce_expression(ns,p->children[i],false);

    if(_m_punit->is_const(ns,p->children[i]) && (p->children[i]->variable->scode != sy_variable))
      _m_punit->_m_constants->add(p->children[i]);
    
    ptype = get_expr_type(p->children[i]);

    if(!ptype || ((ptype->tycode != ty_boolean ) && (ptype->tycode != ty_variant)))
      throw CSyntaxErr(p->children[i]->getNodeLine(),"not a boolean type.");     
  }
}

void CsyntaxValidator::reduce_logical_and(symbol_context *ns, syntaxNode *p)
{
  size_t i = 0;
  sym_type *ptype;

  // reduce each expression and make sure they are each 
  // boolean...
  p->data_type = BOOLEAN_TYPE;
  
  for(i=0;i<p->count;i++)
  {
    if(p->children[i]->node_type!= TOK_NOT)
    {
      reduce_expression(ns,p->children[i],false);

      if(_m_punit->is_const(ns,p->children[i])&&(p->children[i]->variable->scode != sy_variable))
        _m_punit->_m_constants->add(p->children[i]);
    
      ptype = get_expr_type(p->children[i]);

      if(!ptype || ((ptype->tycode != ty_boolean) &&(ptype->tycode != ty_variant)))
        throw CSyntaxErr(p->children[i]->getNodeLine(),"not a boolean type.");     
    }
  }
}

void CsyntaxValidator::reduce_equality(symbol_context *ns, syntaxNode *p)
{
  size_t i;
  sym_type *ptype1,*ptype2;

  p->data_type=BOOLEAN_TYPE;
  // the first two types must match...
  reduce_expression(ns,p->children[0],false);
  reduce_expression(ns,p->children[2],false);

  ptype1=get_expr_type(p->children[0]);
  ptype2=get_expr_type(p->children[2]);

  if(_m_punit->is_const(ns,p->children[0])&&(p->children[0]->variable->scode != sy_variable))
    _m_punit->_m_constants->add(p->children[0]);

  if(_m_punit->is_const(ns,p->children[2])&&(p->children[2]->variable->scode != sy_variable))
    _m_punit->_m_constants->add(p->children[2]);

  switch(p->children[1]->node_type)
  {
    case TOK_EQ:
    case TOK_NE:
      if(!is_equal_type(ptype1,ptype2))
        throw CSyntaxErr(p->children[1]->getNodeLine(),"types are not equal.");

      for(i=4;i<p->count;i+=2)
      {
        reduce_expression(ns,p->children[i],false);
        ptype1=get_expr_type(p->children[i]);

        if(_m_punit->is_const(ns,p->children[i])&&(p->children[i]->variable->scode != sy_variable))
          _m_punit->_m_constants->add(p->children[i]);
        if(!ptype1 || (ptype1->tycode != ty_boolean))
          throw CSyntaxErr(p->children[2]->getNodeLine(),"type is not boolean.");
      }
    break;
    case TOK_IS:
      if(p->count>3)
       throw CSyntaxErr(p->children[3]->getNodeLine(),"not a valid expression.");

      if((ptype1->tycode!=ty_class)||(ptype2->scode != sy_type))
        throw CSyntaxErr(p->children[2]->getNodeLine(),"invalid is expression.");
    break;
    case TOK_IN:
      if(p->count>3)
       throw CSyntaxErr(p->children[3]->getNodeLine(),"not a valid expression.");
    break;
  }
}

void CsyntaxValidator::reduce_relational(symbol_context *ns, syntaxNode *p)
{
  if(p->count!=3)
    throw CSyntaxErr(p->children[2]->getNodeLine(),"invalid expression.");

  syntaxNode *p1=p->children[0];
  syntaxNode *p2=p->children[2];

  reduce_expression(ns,p1,false);
  reduce_expression(ns,p2,false);

  if(_m_punit->is_const(ns,p1)&&(p1->variable->scode != sy_variable))
    _m_punit->_m_constants->add(p1);

  if(_m_punit->is_const(ns,p2)&&(p2->variable->scode != sy_variable))
    _m_punit->_m_constants->add(p2);
  
  //both expressions must be numeric...
  p->data_type = BOOLEAN_TYPE;

  if((!is_numeric(p1)&&(get_expr_type(p1)!=VARIANT_TYPE))||(!is_numeric(p2)&&(get_expr_type(p2)!=VARIANT_TYPE)))
    throw CSyntaxErr(p->children[1]->getNodeLine(),"invalid types for relational expression.");
}

void CsyntaxValidator::reduce_multiplicative(symbol_context *ns, syntaxNode *p)
{
  size_t i;
  syntaxNode *pconst=0;
  sym_type  *pexpr_type=0;
  int op;
  size_t mov_count;
  std::vector<syntaxNode *> &pchildren=p->children;

  for(i=0;i<p->count;i++)
  {
    switch(p->children[i]->node_type)
    {
      case TOK_DIV:
      case TOK_SLASH:
      case TOK_STAR:
      case TOK_MOD:
        op=p->children[i]->node_type;
      break;
      default:
        reduce_expression(ns,p->children[i],false);

        // check on the expression type.
        if(pexpr_type)
          if(!is_equal_type(pexpr_type,get_expr_type(p->children[i])))
            throw CSyntaxErr(p->children[i]->getNodeLine(),"invalid expression type.");
        else
          pexpr_type=get_expr_type(p->children[i]);

        if(_m_punit->is_const(ns,p->children[i]))
        {
          if(!pconst)
            pconst=p->children[i];
          else
          {
            // add the values...
            exp_mult(ns,pconst,p->children[i],op);
            // shift the children...
            mov_count=(p->count-i-1);
            memmove(&p->children[i-1],&p->children[i+1],(mov_count)*sizeof(void*));
            p->count-=2;
            i-=2;
          }
        }
        else
        { 
          if(pconst)
            _m_punit->_m_constants->add(pconst);
          pconst=0;
        }
    }
  }

  // all expressions have been reduced
  if(p->count==1)
  {
    *p = *p->children[0];
  }
  else
  {
    if(pconst)
      _m_punit->_m_constants->add(pconst);
    p->data_type = get_expr_type(p->children[0]);
  }
}

void CsyntaxValidator::reduce_additive(symbol_context *ns, syntaxNode *p)
{
  size_t i;
  syntaxNode *pconst=0;
  sym_type  *pexpr_type=0;
  int op;
  size_t mov_count;
  std::vector<syntaxNode *> &pchildren=p->children;

  for(i=0;i<p->count;i++)
  {
    switch(p->children[i]->node_type)
    {
      case TOK_MINUS:
      case TOK_PLUS:
        op=p->children[i]->node_type;
      break;
      default:
        reduce_expression(ns,p->children[i],false);

        // check on the expression type.
        if(pexpr_type)
        {
          if(!is_equal_type(pexpr_type,get_expr_type(p->children[i])))
            throw CSyntaxErr(p->children[i]->getNodeLine(),"invalid expression type.");
        }
        else
          pexpr_type=get_expr_type(p->children[i]);

        if(_m_punit->is_const(ns,p->children[i]))
        {
          if(!pconst)
            pconst=p->children[i];
          else
          {
            // add the values...
            exp_add(ns,pconst,p->children[i],op);
            // shift the children...
            mov_count=(p->count-i-1);
            memmove(&p->children[i-1],&p->children[i+1],(mov_count)*sizeof(void*));
            p->count-=2;
            i-=2;
          }
        }
        else
        {
          if(pconst&&pconst->node_type==TOK_STRING)
          {
            _m_punit->_m_constants->add(pconst);
            pconst=0;
          }
        }
    }
  }

  // all expressions have been reduced
  if(p->count==1)
  {
    *p = *p->children[0];
  }
  else
  {
    if(pconst)
      _m_punit->_m_constants->add(pconst);
    p->data_type = get_expr_type(p->children[0]);
  }
}

void CsyntaxValidator::reduce_as_expression(symbol_context *ns, syntaxNode *p)
{
  sym_type *ptype1;
  sym_type *ptype2;

  reduce_expression(ns,p->children[0],false);
  reduce_expression(ns,p->children[1],false);

  ptype1 = get_expr_type(p->children[0]);
  ptype2 = get_expr_type(p->children[1]);

  if((ptype1->tycode != ty_interface)&&(ptype1->tycode!=ty_class))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"Operator not applicable to this operand type.");
  
  if((ptype2->tycode != ty_interface)&&(ptype2->tycode!=ty_class))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"Operator not applicable to this operand type.");

  if(ptype1->tycode != ptype2->tycode)
    throw CSyntaxErr(p->children[0]->getNodeLine(),"Incompatible types : %s and %s.",ptype1->name, ptype2->name);

  switch(ptype1->tycode)
  {
    case ty_class:
      if(!is_equal_type(ptype1,ptype2))
        throw CSyntaxErr(p->children[0]->getNodeLine(),"Incompatible types : %s and %s.",ptype1->name, ptype2->name);
    break;
  }

  p->data_type = ptype2;
}

void CsyntaxValidator::reduce_expression(symbol_context *ns, syntaxNode *p, bool is_record )
{
  switch(p->node_type)
  {
    case e_as_expression:
      reduce_as_expression(ns,p);
    break;
    case e_shift_expression:
			throw CSyntaxErr(p->children[0]->getNodeLine(),"Not implemented");
    break;
    case e_multiplicative_expression:
      reduce_multiplicative(ns,p);
    break;
    case e_additive_expression:
      reduce_additive(ns,p);
    break;
    case e_relational_expression:
      reduce_relational(ns,p);
    break;
    case e_equality_expression:
      reduce_equality(ns,p);
    break;
    case e_and_expression:
      throw CSyntaxErr(p->getNodeLine(),"Not Implemented");
    break;
    case e_exclusive_or_expression:
      throw CSyntaxErr(p->getNodeLine(),"Not Implemented");
    break;
    case e_inclusive_or_expression:
      throw CSyntaxErr(p->getNodeLine(),"Not Implemented");
    break;
    case e_logical_and_expression:
      reduce_logical_and(ns,p);
    break;
    case e_logical_or_expression:
      reduce_logical_or(ns,p);
    break;
    case e_simple_expression:
      reduce_simple(ns,p,is_record);
    break;
    case e_sub_expression:
      *p = *p->children[0];
      reduce_expression(ns,p,is_record);
    break;
  }
}

void CsyntaxValidator::reduce_properties_read(syntaxNode *p)
{
  size_t i,j;
  syntaxNode *pread;
  syntaxNode *pcurr;
  token_item ti;

  switch(p->node_type)
  {
    case e_as_expression:
    case e_shift_expression:
    case e_multiplicative_expression:
    case e_additive_expression:
    case e_relational_expression:
    case e_equality_expression:
    case e_and_expression:
    case e_exclusive_or_expression:
    case e_inclusive_or_expression:
    case e_logical_and_expression:
    case e_logical_or_expression:
      for(i=0;i<p->count;i++)
        reduce_properties_read(p->children[i]);
    break;
    case TOK_IDENTIFIER:
      // we must have a valid read property...
      if(p->variable->scode == sy_variable && p->variable->prop_info)
      {
        pread = p->variable->prop_info->pread;

        if(!pread)
          throw CSyntaxErr(p->getNodeLine(),"write only property.");

        if(pread->count>1)
        {
          ti.name = pread->children[0]->node_name;
          ti.line = p->node_line;

          pcurr=_m_punit->get_syntax_tree()->createNode(TOK_IDENTIFIER,ti);
          pcurr->variable=pread->children[0]->variable;
          pcurr->implied_var=p->implied_var;
          pcurr->variable->prop_info = 0;
          
          p->node_type=e_record_expression;
          p->node_name=duplicateString("record_expression");
          p->implied_var=0;

          p->appendChild(pcurr);

          for(j=1;j<pread->count;j++)
            p->insertChild(pread->children[j],j);

          p->variable=pread->children[pread->count-1]->variable;
        }
        else
        {
          p->node_name = pread->children[0]->node_name;
          p->variable=pread->children[0]->variable;
        }
      }
    break;
    case e_record_expression:
      pread = 0;

      for(i=0;i<p->count;i++)
      {
        pread = 0;
        pcurr = p->children[i];
       
        if((pcurr->variable)&&(pcurr->variable->scode == sy_variable && pcurr->variable->prop_info))
        {
          pread = pcurr->variable->prop_info->pread;
          
          if(!pread)
            throw CSyntaxErr(p->children[i]->getNodeLine(),"write only property.");

          pcurr->node_name=pread->children[0]->node_name;
          pcurr->variable=pread->children[0]->variable;

          for(j=1;j<pread->count;j++)
          {
            i++;
            p->insertChild(pread->children[j],i);
          }
        }
      }  

      if(pread)
        p->variable=pread->children[pread->count-1]->variable;   

    break;
  }
}

#ifdef _WIN32
void CsyntaxValidator::reduce_dispatch_write(syntaxNode *p)
{
//  int i,j;
//  syntaxNode *pprop;
  syntaxNode *pcurr;
  syntaxNode *pproc_expr,*pexpr_list;
//  sym_procedure *pproc;
//  token_item ti;

//  if(!p->children[0]->variable->prop_info->pwrite)
  //  throw CSyntaxErr(p->children[0]->getNodeLine(),"left side cannot be assigned to.");

  //pprop=p->children[0]->variable->prop_info->pwrite;
  //pproc=pprop->children[pprop->count-1]->procedure;

  //reduce_properties_read(p->children[1]);       

  switch(p->children[0]->node_type)
  {
    /*
    case TOK_IDENTIFIER:
      if(pproc->scode==sy_procedure)
      {
        // this changes the statement...
        p->node_type = e_statement;
        p->node_name = "statement";
        p->count = 1;

        pproc_expr=_m_punit->get_syntax_tree()->createNode(e_proc_func_expression);
        pproc_expr->implied_var=p->children[0]->implied_var;
        pproc_expr->procedure=pproc;
        pexpr_list=_m_punit->get_syntax_tree()->createNode(e_expression_list);
        
        pproc_expr->appendChild(pprop->children[0]);
        pproc_expr->appendChild(pexpr_list);
        pexpr_list->appendChild(p->children[1]);

        p->children[0]=pproc_expr;
      }
      else
      {
        if(pprop->count>1)
        {
          ti.name = pprop->children[0]->node_name;
          ti.line = p->children[0]->node_line;

          pcurr=_m_punit->get_syntax_tree()->createNode(TOK_IDENTIFIER,ti);
          pcurr->variable=pprop->children[0]->variable;
          pcurr->implied_var=p->children[0]->implied_var;
          pcurr->variable->prop_info = 0;
       
          p->children[0]->node_type=e_record_expression;
          p->children[0]->node_name=duplicateString("record_expression");
          p->children[0]->implied_var=0;

          p->children[0]->appendChild(pcurr);
    
          for(j=1;j<pprop->count;j++)
            p->children[0]->insertChild(pprop->children[j],j);

          p->children[0]->variable=pprop->children[pprop->count-1]->variable;
        }
        else
        {
          p->children[0]->node_name = pprop->children[0]->node_name;
          p->children[0]->variable = pprop->children[0]->variable;
        }
      }
    break;
    */
    case e_record_expression:
      /*
      for(i=0;i<p->children[0]->count-1;i++)
      {
        pcurr=p->children[0]->children[i];
        if(pcurr->variable->scode == sy_variable && pcurr->variable->prop_info)
        {
          pprop = pcurr->variable->prop_info->pread;
        
          if(!pprop)
            throw CSyntaxErr(p->children[0]->getNodeLine(),"write only property.");

          pcurr->node_name=pprop->children[0]->node_name;
          pcurr->variable=pprop->children[0]->variable;

          for(j=1;j<pprop->count;j++)
          {
            i++;
            p->children[0]->insertChild(pprop->children[j],i);
          }
        }
      }  

      pprop=p->children[0]->variable->prop_info->pwrite;
      pcurr=p->children[0]->children[i];
    
      // do the final variable...   
      switch(pproc->scode)
      {
        case sy_procedure:
          // this changes the statement...
          p->node_type = e_statement;
          p->node_name = "statement";
          p->count = 1;

          pproc_expr=_m_punit->get_syntax_tree()->createNode(e_proc_func_expression);
          pproc_expr->procedure=pproc;

          pexpr_list=pcurr->get_node(e_expression_list);

          if(!pexpr_list)
            pexpr_list=_m_punit->get_syntax_tree()->createNode(e_expression_list);

          pproc_expr->appendChild(pcurr);
          pproc_expr->appendChild(pexpr_list);
          pexpr_list->appendChild(p->children[1]);

          p->children[0]->children[i]=pproc_expr;
        break;
        case sy_variable:
          pcurr->node_name=pprop->children[0]->node_name;
          pcurr->variable=pprop->children[0]->variable;

          for(j=1;j<pprop->count;j++)
          {
            i++;
            p->children[0]->insertChild(pprop->children[j],i);
          }     
          
          p->children[0]->variable = pprop->children[pprop->count-1]->variable;   
        break;
      }
      */
      // this changes the statement...
      p->node_type = e_statement;
      p->node_name = "statement";
      p->count = 1;
      
      pproc_expr=_m_punit->get_syntax_tree()->createNode(e_proc_func_expression);
      pcurr=p->children[0]->children[p->children[0]->count-1];
      pproc_expr->flags = DISPATCH_PROPERTYPUT;

      //pproc_expr->procedure=pproc;

      pexpr_list=pcurr->get_node(e_expression_list);

      if(!pexpr_list)
        pexpr_list=_m_punit->get_syntax_tree()->createNode(e_expression_list);

      pproc_expr->appendChild(pcurr);
      pproc_expr->appendChild(pexpr_list);
      pexpr_list->appendChild(p->children[1]);

      p->children[0]->children[p->children[0]->count-1]=pproc_expr;
    break;
  }
}
#endif


void CsyntaxValidator::reduce_properties_write(syntaxNode *p)
{
  size_t i,j;
  syntaxNode *pprop;
  syntaxNode *pcurr;
  syntaxNode *pproc_expr,*pexpr_list;
  sym_procedure *pproc;
  token_item ti;

  if(!p->children[0]->variable->prop_info->pwrite)
    throw CSyntaxErr(p->children[0]->getNodeLine(),"left side cannot be assigned to.");

  pprop=p->children[0]->variable->prop_info->pwrite;
  pproc=pprop->children[pprop->count-1]->procedure;

  reduce_properties_read(p->children[1]);       

  switch(p->children[0]->node_type)
  {
    case TOK_IDENTIFIER:
      if(pproc->scode==sy_procedure)
      {
        // this changes the statement...
        p->node_type = e_statement;
        p->node_name = "statement";
        p->count = 1;

        pproc_expr=_m_punit->get_syntax_tree()->createNode(e_proc_func_expression);
        pproc_expr->implied_var=p->children[0]->implied_var;
        pproc_expr->procedure=pproc;
        pexpr_list=_m_punit->get_syntax_tree()->createNode(e_expression_list);
        
        pproc_expr->appendChild(pprop->children[0]);
        pproc_expr->appendChild(pexpr_list);
        pexpr_list->appendChild(p->children[1]);

        p->children[0]=pproc_expr;
      }
      else
      {
        if(pprop->count>1)
        {
          ti.name = pprop->children[0]->node_name;
          ti.line = p->children[0]->node_line;

          pcurr=_m_punit->get_syntax_tree()->createNode(TOK_IDENTIFIER,ti);
          pcurr->variable=pprop->children[0]->variable;
          pcurr->implied_var=p->children[0]->implied_var;
          pcurr->variable->prop_info = 0;
       
          p->children[0]->node_type=e_record_expression;
          p->children[0]->node_name=duplicateString("record_expression");
          p->children[0]->implied_var=0;

          p->children[0]->appendChild(pcurr);
    
          for(j=1;j<pprop->count;j++)
            p->children[0]->insertChild(pprop->children[j],j);

          p->children[0]->variable=pprop->children[pprop->count-1]->variable;
        }
        else
        {
          p->children[0]->node_name = pprop->children[0]->node_name;
          p->children[0]->variable = pprop->children[0]->variable;
        }
      }
    break;
    case e_record_expression:
      for(i=0;i<p->children[0]->count-1;i++)
      {
        pcurr=p->children[0]->children[i];
        if(pcurr->variable->scode == sy_variable && pcurr->variable->prop_info)
        {
          pprop = pcurr->variable->prop_info->pread;
        
          if(!pprop)
            throw CSyntaxErr(p->children[0]->getNodeLine(),"write only property.");

          pcurr->node_name=pprop->children[0]->node_name;
          pcurr->variable=pprop->children[0]->variable;

          for(j=1;j<pprop->count;j++)
          {
            i++;
            p->children[0]->insertChild(pprop->children[j],i);
          }
        }
      }  

      pprop=p->children[0]->variable->prop_info->pwrite;
      pcurr=p->children[0]->children[i];
    
      // do the final variable...   
      switch(pproc->scode)
      {
        case sy_procedure:
          // this changes the statement...
          p->node_type = e_statement;
          p->node_name = "statement";
          p->count = 1;

          pproc_expr=_m_punit->get_syntax_tree()->createNode(e_proc_func_expression);
          pproc_expr->procedure=pproc;

          pexpr_list=pcurr->get_node(e_expression_list);

          if(!pexpr_list)
            pexpr_list=_m_punit->get_syntax_tree()->createNode(e_expression_list);

          pproc_expr->appendChild(pcurr);
          pproc_expr->appendChild(pexpr_list);
          pexpr_list->appendChild(p->children[1]);

          p->children[0]->children[i]=pproc_expr;
        break;
        case sy_variable:
          pcurr->node_name=pprop->children[0]->node_name;
          pcurr->variable=pprop->children[0]->variable;

          for(j=1;j<pprop->count;j++)
          {
            i++;
            p->children[0]->insertChild(pprop->children[j],i);
          }     
          
          p->children[0]->variable = pprop->children[pprop->count-1]->variable;   
        break;
      }
    break;
  }
}

//*************************************************************************************
// Statement Expansion
//*************************************************************************************
void CsyntaxValidator::process_if(symbol_context *pns, syntaxNode *p)
{
  syntaxNode *p1=p->children[0];
  sym_type *ptype;

  reduce_expression(pns,p1,false);

  ptype = get_expr_type(p1);

  if((ptype->tycode != ty_boolean) && (ptype->tycode != ty_variant))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"expression is not boolean.");

  reduce_properties_read(p1);

  process_statement(pns,p->children[1]);

  if(p->count > 2)
    process_statement(pns,p->children[2]);
}

void CsyntaxValidator::process_repeat(symbol_context *pns, syntaxNode *p)
{
  syntaxNode *p1 = p->children[1];
  syntaxNode *p2 = p->children[0];
  sym_type *ptype;

  reduce_expression(pns,p1,false);

  ptype = get_expr_type(p1);

  if((ptype->tycode != ty_boolean)&& (ptype->tycode != ty_variant))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"expression is not boolean.");

  reduce_properties_read(p1);

  _m_loopContext.push(1);

  process_statement(pns,p2);

  _m_loopContext.pop();
}

void CsyntaxValidator::process_for(symbol_context *pns, syntaxNode *p)
{
  syntaxNode *loop_var = p->children[0];
  syntaxNode *init_expr = p->children[1];
  syntaxNode *loop_expr = p->children[3];
  syntaxNode *statement = p->children[4];
  sym_location location;
  sym_type *ptype;

  loop_var->data_type = (sym_type*)_m_punit->get_symbol(loop_var->node_name,pns,true,&location);
  loop_var->implied_var = (location.ns)?location.ns->ns_var:0;

  if(!loop_var->data_type)
    throw CSyntaxErr(loop_var->getNodeLine(),"Undeclared identifier : %s.", loop_var->node_name);

  if((loop_var->implied_var) || (loop_var->data_type->scode != sy_variable) || loop_var->variable->const_)
    throw CSyntaxErr(loop_var->getNodeLine(),"For loop control variable must be simple local variable");

  if(loop_var->variable->type->tycode != ty_integer)
    throw CSyntaxErr(loop_var->getNodeLine(),"For loop control variable must have ordinal type.");

  reduce_expression(pns,init_expr,false);

  ptype = get_expr_type(init_expr);
  if(!ptype)
    throw CSyntaxErr(init_expr->getNodeLine(),"Incompatible types : Integer and Procedure.");
  if((ptype->tycode != ty_integer) && (ptype->tycode != ty_real) && (ptype->tycode != ty_variant))
    throw CSyntaxErr(init_expr->getNodeLine(),"Incompatible types : Integer and %s.",ptype->name);
  if(_m_punit->is_const(pns,init_expr)&&(init_expr->variable->scode != sy_variable))
    _m_punit->_m_constants->add(init_expr);

  reduce_expression(pns,loop_expr,false);

  ptype = get_expr_type(loop_expr);
  if(!ptype)
    throw CSyntaxErr(loop_expr->getNodeLine(),"Incompatible types : Integer and Procedure.");
  if((ptype->tycode != ty_integer) && (ptype->tycode != ty_real) && (ptype->tycode != ty_variant))
    throw CSyntaxErr(loop_expr->getNodeLine(),"Incompatible types : Integer and %s.",ptype->name);
  if(_m_punit->is_const(pns,loop_expr)&&(loop_expr->variable->scode != sy_variable))
    _m_punit->_m_constants->add(loop_expr);

  _m_loopContext.push(1);

  process_statement(pns,statement);

  _m_loopContext.pop();
}

void CsyntaxValidator::process_while(symbol_context *pns, syntaxNode *p)
{
  syntaxNode *p1 = p->children[0];
  syntaxNode *p2 = p->children[1];
  sym_type *ptype;

  reduce_expression(pns,p1,false);

  ptype = get_expr_type(p1);

  if((ptype->tycode != ty_boolean)&& (ptype->tycode != ty_variant))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"expression is not boolean.");

  reduce_properties_read(p1);

  _m_loopContext.push(1);

  process_statement(pns,p2);

  _m_loopContext.pop();
}

void CsyntaxValidator::process_assign(symbol_context *pns, syntaxNode *p)
{
  syntaxNode *p1 = p->children[0];
  syntaxNode *p2 = p->children[1];

  reduce_expression(pns,p1,false);
  reduce_expression(pns,p2,false);

  // special case for the create_object function...
  if((p1->data_type->scode != sy_variable)&&(p1->data_type != VARIANT_TYPE))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"cannot assign to lvalue.");
  
  if(_m_punit->is_const(pns,p1))
    throw CSyntaxErr(p->children[0]->getNodeLine(),"cannot assign to const expr.");
   
  sym_type *ptype1=get_expr_type(p1);
  sym_type *ptype2=get_expr_type(p2);

  if((!((ptype1->tycode == ty_interface)&&(p2->node_type == TOK_NIL)))
    &&(!is_equal_type(ptype1,ptype2)))
  {
    if((ptype1->tycode==ty_char)&&(ptype2->tycode==ty_string)&&_m_punit->is_const(pns,p2))
    {
      if(strlen(p2->node_name)!=1)
        throw CSyntaxErr(p1->getNodeLine(),"Incompatible types : %s and %s.",ptype1->name, ptype2->name);
      p2->data_type = CHAR_TYPE;
    }
    else
      throw CSyntaxErr(p1->getNodeLine(),"Incompatible types : %s and %s.",ptype1->name, ptype2->name);
  }

  if(_m_punit->is_const(pns,p2)&&(p2->variable->scode != sy_variable))
    _m_punit->_m_constants->add(p2);

  // here's where we deternmine if it's a property assignment
  if((p1->variable->scode == sy_variable)&&(p1->variable->prop_info))
    reduce_properties_write(p);
  #ifdef _WIN32
  else
  if(p1->data_type == VARIANT_TYPE)
    reduce_dispatch_write(p);
  #endif
  else
  {
    reduce_properties_read(p1);
    reduce_properties_read(p2);
  }  
}

void CsyntaxValidator::process_statement(symbol_context *pns, syntaxNode *p)
{
  try
  {
    switch(p->node_type)
    {
      case e_statement_raise:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
      case e_statement_with:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
      case e_statement_assign:
        process_assign(pns,p);
      break;
      case e_statement_repeat:
        process_repeat(pns,p);
      break;
      case e_statement_while:
        process_while(pns,p);
      break;
      case e_statement_if:
        process_if(pns,p);
      break;
      case e_statement_for:
        process_for(pns,p);
      break;
      case e_statement_case:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
      case e_statement_list:
        process_statements(pns,p);
      break;
      case e_statement_try:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
      case e_statement:
         reduce_expression(pns,p->children[0],false);          
         reduce_properties_read(p->children[0]);
      break;
      case e_statement_goto:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
      case e_statement_label:
        throw CSyntaxErr(p->getNodeLine(),"Not implemented");
      break;
    }
  }
  catch(CSyntaxErr err)
  {
    _m_punit->handle_error(&err);
  }
}

void CsyntaxValidator::process_statements(symbol_context *pns, syntaxNode *p)
{
  size_t i;

  if(!p)
    return;

  for(i=0;i<p->count;i++)
    process_statement(pns,p->children[i]);
}

//*************************************************************************************
// Declarations
//*************************************************************************************
void CsyntaxValidator::add_definitions(symbol_context *ns, sym_namespace *ps, syntaxNode *p)
{
  size_t i;
  syntaxNode *declarations;
  
  declarations=p->get_node(e_implementation_declarations);

  if(declarations)
  {
    for(i=0;i<declarations->count;i++)
    {
      try
      {
        switch(declarations->children[i]->node_type)
        {
          case e_type_declaration_list:
            add_type_declarations(ns,ps,declarations->children[i]);
          break;
          case e_const_definition_list:
            add_const_definitions(ns,ps,declarations->children[i]);
          break;
          case e_list_var_declaration:
            add_var_declarations(ns,ps,declarations->children[i]);
          break;
          case e_procedure_definition:
            add_procedure_definition(ns,ps,declarations->children[i]);
          break;
          case e_function_definition:
            add_function_definition(ns,ps,declarations->children[i]);
          break;
          case e_constructor_definition:
            add_constructor_definition(ns,ps,declarations->children[i]);
          break;
          case e_destructor_definition:
            throw CSyntaxErr(p->getNodeLine(),"Not implemented");
          break;
          case TOK_LABEL:
            throw CSyntaxErr(p->getNodeLine(),"Not implemented");
          break;
        }
      }
      catch(CSyntaxErr err)
      {
        _m_punit->handle_error(&err);
      }
    }
  }
}

void CsyntaxValidator::add_type_declaration(symbol_context *ns, sym_namespace *ps, syntaxNode *p)
{
  syntaxNode *ptype_info;
  syntaxNode *ptype_name;
  sym_type  *ptype;

  ptype_name = p->children[0];
  ptype_info = p->children[1];

	ptype = get_type(ns,ptype_info,ps,ptype_name);
    
  if(ptype)
  {
    ps->add_symbol(ptype);
    ps->types.insert(ptype_name->node_name,ptype);
  }
}

void CsyntaxValidator::add_type_declarations(symbol_context *ns, sym_namespace *ps,syntaxNode *p)
{
  size_t i;

	for(i=0;i<p->count;i++)
  {

    try
    {
			add_type_declaration(ns,ps,p->children[i]);
    }
    catch(CSyntaxErr err)
    {
      _m_punit->handle_error(&err);
    }
  }
}

void CsyntaxValidator::add_const_definition(symbol_context *ns, sym_namespace *ps,syntaxNode *pl)
{
  syntaxNode  *pexpression;
  syntaxNode  *pconstant;

  sym_type *ptype=0;
  sym_variable *pvar;
    
    pvar = new sym_variable;
    pvar->name = pl->children[0]->node_name;
    pvar->const_ = 1;
    pvar->default_ = 0;
    pvar->scode = sy_variable;
    pvar->param = 0;
    pvar->byref = 0;
    pvar->default_ = 0;
    pvar->static_var = 0;
    pvar->prop_info = 0;
    pvar->parent = ps;
    pvar->unit_index = _m_punit->get_index();

		_m_punit->registerSymbol(pvar);

    if(pl->count>2)
    {
      pexpression = 0;
      pconstant = pl->children[2];

      switch(pconstant->children[0]->node_type)
      {
        case e_record_field_constant_list:
          throw CSyntaxErr(pl->getNodeLine(),"Not Implemented");
        break;
        case e_array_constant:
          throw CSyntaxErr(pl->getNodeLine(),"Not Implemented");
        break;
        default:
          pexpression = pconstant->children[0];
      }

      ptype = get_type(ns,pl->children[1],ps);
    }
    else
      pexpression = pl->children[1];

		reduce_expression(ns,pexpression,false);

		if(!_m_punit->is_const(ns,pexpression))
			throw CSyntaxErr(pexpression->getNodeLine(),"not a constant expression.");

		pvar->type = get_expr_type(pexpression);

		// make sure the declared type and the const type agree.
		if(ptype)
		{
			if(!is_equal_type(ptype,pvar->type))
			{
				if(ptype->tycode == ty_record && pvar->type->tycode == ty_string)
				{
					if(strcmp(ptype->name ,"tguid")==0)
					{
						if(false==check_guid_syntax(pexpression->node_name))
							throw CSyntaxErr(pexpression->getNodeLine(),"Improper GUID syntax.");
						pexpression->node_name = duplicateString(get_guid_memory(pexpression->node_name).c_str());
					}
					else
						throw CSyntaxErr(pexpression->getNodeLine(),"Incompatible types %s and %s.",ptype->name,pvar->type->name);
				}
				else
					throw CSyntaxErr(pexpression->getNodeLine(),"Incompatible types %s and %s.",ptype->name,pvar->type->name);
			}
			pvar->type = ptype;
			pexpression->data_type = ptype;
		}

		switch(pvar->type->tycode)
		{
		case ty_integer:
			pvar->value.int_val = expr_val_int(ns,pexpression);
			break;
		case ty_real:
			pvar->value.real_val = expr_val_dbl(ns,pexpression);
			break;
		case ty_string:
		case ty_widestring:
			pvar->value.string_val = pexpression->node_name;
			break;
		}

		pvar->offset = (int)_m_punit->_m_constants->add(pexpression);
		ps->variables.insert(pvar->name,pvar);
		
}

void CsyntaxValidator::add_const_definitions(symbol_context *ns, sym_namespace *ps,syntaxNode *p)
{

  for(size_t i=0;i<p->count;i++)
  {
		try
		{
			add_const_definition(ns,ps,p->children[i]);
    }
    catch(CSyntaxErr err)
    {
      _m_punit->handle_error(&err);
    }
  }
}

void CsyntaxValidator::add_var_declaration(symbol_context *ns, sym_namespace *ps, syntaxNode *pdecl)
{
  syntaxNode *pname_list;
  syntaxNode *ptype;

	pname_list = pdecl->children[0];
  ptype  = pdecl->children[1];

	add_variable(ns,ps,pname_list,ptype,VIS_PUBLIC);
}

void CsyntaxValidator::add_var_declarations(symbol_context *ns, sym_namespace *ps, syntaxNode *p)
{
  size_t i;

  for(i=0;i<p->count;i++)
  { try
    {
			add_var_declaration(ns,ps,p->children[i]);
    }
    catch(CSyntaxErr err)
    {
       _m_punit->handle_error(&err);
    }
  }
}

void CsyntaxValidator::add_variable(symbol_context *ns, sym_namespace *ps, syntaxNode *pname_list, syntaxNode *ptype, BYTE visibility)
{
  size_t j;
  sym_type  *stype;
  sym_variable *svar;
  size_t size;

  symbol_context stack_node;

  stack_node.ns_var =0;
  stack_node.last = 0;
  stack_node.ps = ps;
  

  stype = get_type(ns,ptype,ps);
    
  if(!stype)
  {
    throw CSyntaxErr(ptype->children[0]->getNodeLine(),"Undeclared identifier : %s.",ptype->children[0]->node_name);
  }

  for(j=0;j<pname_list->count;j++)
  {
    svar = new sym_variable;
    svar->name = pname_list->children[j]->node_name;
    svar->parent = ps;
    svar->const_ = 0;
    svar->prop_info = 0;
    svar->static_var = (ps==&_m_punit->global)?1:0;
    svar->param = 0;
    svar->byref = 0;
    svar->default_ = 0;
    svar->scode = sy_variable;
    svar->visibility = visibility;
    svar->unit_index = _m_punit->get_index();

		_m_punit->registerSymbol(svar);

    // see if the item was already declared...
    if(ps->get_member(svar->name))
      throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"identifier redeclared : %s.",svar->name);

    size = (stype->tycode==ty_class)?4:stype->size;
    svar->offset = (int)ps->size;
    svar->type = stype;
    ps->size += size;

    ps->add_symbol(svar);
    ps->variables.insert(svar->name,svar);
  }
}

void CsyntaxValidator::add_property(symbol_context *ns, sym_namespace *ps, syntaxNode *pproperty, BYTE visibility)
{
  // property name...
  sym_variable *pvar;
  sym_base *tmp_var;
  sym_type *ptype;
  sym_type *tmp_type;
  char *pname;

  syntaxNode *pnames;
  syntaxNode *pread;
  syntaxNode *pwrite;
  syntaxNode *pparams;
  sym_procedure *pproc;
  sym_procedure *tmp_proc;
  sym_procedure *overloaded_proc;
  sym_procedure *overloaded_proc_tmp;
  sym_namespace *tmp_ps;

  symbol_context stack_node;

  stack_node.ns_var =0;
  stack_node.last = 0;
  stack_node.ps = ps;
    
  size_t i;

  try
  {
    pname = pproperty->children[0]->node_name;
    ptype = (sym_type*)_m_punit->get_symbol(pproperty->children[1]->children[0]->node_name,ns,true);
    pwrite = pproperty->children[2]->get_node(e_opt_write);
    pread = pproperty->children[2]->get_node(e_opt_read);
    pparams = pproperty->children[1]->get_node(e_parameter_list);

    if(!ptype)
      throw CSyntaxErr(pproperty->children[1]->getNodeLine(),"Undeclared identifier: '%s'.",pproperty->children[1]->children[0]->node_name);

    if(ptype->scode != sy_type)
      throw CSyntaxErr(pproperty->children[1]->getNodeLine(),"'%s' is not a type identifier.",pproperty->children[1]->children[0]->node_name);

    pvar = new sym_variable;
    pvar->name = duplicateString(pname);
    pvar->parent = ps;
    pvar->const_ = 0;
    pvar->static_var=0;
    pvar->param = 0;
    pvar->byref = 0;
    pvar->default_ = 0;
    pvar->scode = sy_variable;
    pvar->visibility = visibility;
    pvar->prop_info = new sym_property;
    pvar->offset = 0;
    pvar->type = ptype;
    pvar->unit_index = _m_punit->get_index();

		_m_punit->registerSymbol(pvar);

    // see if the item was already declared...
    if(ps->get_member(pvar->name))
      throw CSyntaxErr(pproperty->children[0]->getNodeLine(),"identifier redeclared : '%s'.",pvar->name);

    pvar->prop_info->array_proc = (pparams)?new sym_procedure:0;
    pvar->prop_info->pread = (pread)?pread->children[0]:0;
    pvar->prop_info->pwrite = (pwrite)?pwrite->children[0]:0;

		if(pvar->prop_info->array_proc)
		{
			pvar->prop_info->array_proc->psyntax = 0;
			_m_punit->registerSymbol(pvar->prop_info->array_proc);
		}

    if(!pread && !pwrite)
      throw CSyntaxErr(pproperty->children[1]->getNodeLine(),"must be at least read-only or write-only.");

    if(pparams)
      add_params(ns,pvar->prop_info->array_proc,pparams);

    // we have to validate the items that the property references...
    if(pread)
    {
      pnames=pread->children[0];
      tmp_ps=ps;
      tmp_type=ps->space_type;

      // there may be only one item, or they may be referenced from a structure
      for(i=0;i<pnames->count;i++)
      {
        switch(tmp_type->tycode)
        {
          case ty_interface:
          case ty_record:
          case ty_class:
          break;
          default:
            throw CSyntaxErr(pproperty->children[1]->getNodeLine(),"Record, object, or class type required.");
        }

        tmp_var=tmp_ps->get_member(pnames->children[i]->node_name);        
        
        if(!tmp_var)
          throw CSyntaxErr(pnames->children[i]->getNodeLine(),"Undeclared identifier.");

        pnames->children[i]->variable = (sym_variable*)tmp_var;

        switch(tmp_var->scode)
        {
          case sy_variable:
            tmp_type=((sym_variable*)tmp_var)->type;
          break;
          case sy_procedure:
            overloaded_proc=(sym_procedure*)tmp_var;
            /*
            if(overloaded_proc->overloaded)
            {
              overloaded_proc=get_overloaded_declaration(overloaded_proc,pparams);

              if(!overloaded_proc)
                throw CSyntaxErr(pnames->children[i]->getNodeLine(),"None of the overloads matches the given parameters.");
            }
            */
            tmp_type=overloaded_proc->ret_type;
          break;
          default:
            tmp_type=0;
        }

        if(!tmp_type)
          throw CSyntaxErr(pnames->children[i]->getNodeLine(),"incompatible type.");
        
        switch(tmp_type->tycode)
        {
          case ty_interface:
            tmp_ps=&((interface_type*)tmp_type)->members;
          break;
          case ty_record:
            tmp_ps=&((record_type*)tmp_type)->members;
          break;
          case ty_class:
            tmp_ps=&((class_type*)tmp_type)->members;
          break;
        }

        // the types must match..
        if(!is_equal_type(tmp_type,ptype,true))
         throw CSyntaxErr(pnames->children[i]->getNodeLine(),"Types of actual and formal parameter must be equal.");
      }

      // the property parameters must match the read function...
      if(pparams)
      {
        if(tmp_var->scode != sy_procedure)
          throw CSyntaxErr(pread->getNodeLine(),"");
      
        pproc=(sym_procedure*)tmp_var;
        tmp_proc=pvar->prop_info->array_proc;

        if(pproc->param_count!=tmp_proc->param_count)
          throw CSyntaxErr(pread->getNodeLine(),"");      
        
        for(i=0;i<pproc->param_count;i++)
        {
          if(!is_equal_type(pproc->parameters[i]->type,tmp_proc->parameters[i]->type,true))
            throw CSyntaxErr(pparams->getNodeLine(),"Types of actual and formal parameter must be equal.");
        }        
      }
    }

    if(pwrite)
    {
      pnames=pwrite->children[0];
      tmp_ps=ps;
      tmp_type=ps->space_type;

      for(i=0;i<pnames->count;i++)
      {
        switch(tmp_type->tycode)
        {
          case ty_interface:
          case ty_record:
          case ty_class:
          break;
          default:
            throw CSyntaxErr(pwrite->children[0]->getNodeLine(),"Record, object, or class type required.");
        }

        tmp_var=tmp_ps->get_member(pnames->children[i]->node_name);        
        
        if(!tmp_var)
          throw CSyntaxErr(pnames->children[i]->getNodeLine(),"Undeclared identifier.");

        pnames->children[i]->variable = (sym_variable*)tmp_var;

        switch(tmp_var->scode)
        {
          case sy_variable:
            tmp_type=((sym_variable*)tmp_var)->type;
          break;
          case sy_procedure:
            overloaded_proc=(sym_procedure*)tmp_var;
            if(overloaded_proc->overloaded)
            {
              overloaded_proc_tmp = 0;
              

              while(overloaded_proc)
              {
                tmp_type = 0;

                if(overloaded_proc->param_count>0)
                  tmp_type = overloaded_proc->parameters[overloaded_proc->param_count-1]->type;

                if(tmp_type && is_equal_type(ptype,tmp_type,true))
                {
                  if(overloaded_proc_tmp)
                    throw CSyntaxErr(pnames->children[i]->getNodeLine(),"ambiguous method '%s'.", overloaded_proc->name); 

                  overloaded_proc_tmp=overloaded_proc;
                }

                overloaded_proc=overloaded_proc->next_overload;
              }

              if(!overloaded_proc_tmp)
                throw CSyntaxErr(pnames->children[i]->getNodeLine(),"None of the overloads matches the given parameters.");
              overloaded_proc=overloaded_proc_tmp;
            }

            pnames->children[i]->procedure=overloaded_proc;
            tmp_var=(sym_variable*)overloaded_proc;
            tmp_type=overloaded_proc->ret_type;
          break;
          default:
            tmp_type=0;
        }

        if(!tmp_type && (i!=pnames->count-1))
          throw CSyntaxErr(pnames->children[i]->getNodeLine(),"Invalid type.");
        
        if(tmp_type)
          switch(tmp_type->tycode)
          {
            case ty_interface:
              tmp_ps=&((interface_type*)tmp_type)->members;
            break;
            case ty_record:
              tmp_ps=&((record_type*)tmp_type)->members;
            break;
            case ty_class:
              tmp_ps=&((class_type*)tmp_type)->members;
            break;
          }
      }

      pproc=pnames->children[pnames->count-1]->procedure;
      
      switch(pproc->scode)
      {
        case sy_variable:
          tmp_type=pnames->children[pnames->count-1]->variable->type;
        break;
        case sy_procedure:
          if(pproc->param_count==0)
            throw CSyntaxErr(pnames->children[pnames->count-1]->getNodeLine(),"Types of actual and formal parameter must be equal.");
          tmp_type=pproc->parameters[pproc->param_count-1]->type;
        break;
      }
      // the types must match..
      if(!is_equal_type(tmp_type,ptype,true))
       throw CSyntaxErr(pnames->children[pnames->count-1]->getNodeLine(),"Types of actual and formal parameter must be equal.");

      // the property parameters must match the write function...
      if(pparams)
      {
        if(tmp_var->scode != sy_procedure)
          throw CSyntaxErr(pread->getNodeLine(),"");
      
        pproc=(sym_procedure*)tmp_var;
        tmp_proc=pvar->prop_info->array_proc;

        if((pproc->param_count-1)!=tmp_proc->param_count)
          throw CSyntaxErr(pread->getNodeLine(),"");      
        
        for(i=0;i<pproc->param_count-1;i++)
        {
          if(!is_equal_type(pproc->parameters[i]->type,tmp_proc->parameters[i]->type,true))
            throw CSyntaxErr(pparams->getNodeLine(),"Types of actual and formal parameter must be equal.");
        }        
      }
    }
    
    // insert this variable
    ps->add_symbol(pvar);
    ps->variables.insert(pvar->name,pvar);

  }
  catch(CSyntaxErr err)
  {
    _m_punit->handle_error(&err);
  }
}

void CsyntaxValidator::reduce_params(symbol_context *ns,syntaxNode *p)
{
  // add the rest of the parameters...
  if(!p)
    return;

  size_t j;
  syntaxNode *param;
  syntaxNode *pname_list;
  syntaxNode *ptype;
  sym_type  *stype;
  syntaxNode *pdef_value;

  bool has_default;
  bool seen_default=false;

  for(size_t i=0;i<p->count;i++)
  {
    has_default = false;

    switch(p->children[i]->node_type)
    {
      case e_parameter:
        param = p->children[i];
      break;
      case e_formal_parameter:
        param = p->children[i]->children[1];
      break;
    }

    pname_list=param->children[0];

    if(param->count>1)
    {
      // check for default...
      if(param->children[1]->node_type != TOK_ARRAY)
      {
        ptype=param->children[1];
        stype=get_type(ns,ptype,0);

        if(param->count > 2)
        {
          has_default=true;
          seen_default=true;
          pdef_value = param->children[3];
        }
      }
    }
    else
      stype = VOID_TYPE;

    if(!stype)
      throw CSyntaxErr(param->children[1]->getNodeLine(),"Undeclared identifier : %s.",param->children[1]->children[0]->node_name);

    for(j=0;j<pname_list->count;j++)
    {
      // remember the variable so we can figure out the signature later on...
      pname_list->children[j]->data_type = stype;

      if(has_default)
      {
        if(j!=0)
          throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"%s not allowed here due to default value.",pname_list->children[j]->node_name);

        // add the default value...
        reduce_expression(ns,pdef_value,false);

        if(!_m_punit->is_const(ns,pdef_value))
          throw CSyntaxErr(pdef_value->getNodeLine(),"not a constant expression.");

        // make sure the declared type and the const type agree.
        if(!is_equal_type(stype,get_expr_type(pdef_value)))
          throw CSyntaxErr(pdef_value->getNodeLine(),"types do not match.");
      }
      else
      {
        if(seen_default)
          throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"default value required for %s.",pname_list->children[j]->node_name);
      }
    }
  }
}

size_t CsyntaxValidator::add_params(symbol_context *ns, sym_procedure *proc_info, syntaxNode *p)
{
  sym_variable *pvar;
  sym_namespace *ps=&proc_info->members;
  sym_type *class_type=proc_info->parent;
  size_t offset = 8;
  bool expand_param = (proc_info->parent && proc_info->parent->tycode == ty_interface);

  symbol_context stack_node;

  stack_node.ns_var = 0;
  stack_node.last = 0;
  stack_node.ps = ps;
  

  proc_info->param_count=0;
  proc_info->parameters=0;

  if(class_type && 
     class_type->tycode == ty_class &&
     proc_info->constructor==0 &&
     proc_info->static_proc==0 )
  {
    offset+=4;
    pvar = new sym_variable;
    pvar->name = duplicateString("self");
    pvar->const_ = 0;
    pvar->scode = sy_variable;
    pvar->param = 1;
    pvar->byref = 1;
    pvar->default_ = 0;
    pvar->static_var = 0;
    pvar->prop_info = 0;
    pvar->parent = &proc_info->members;
    pvar->type = class_type;
    pvar->offset = (int)offset;
    pvar->unit_index = _m_punit->get_index();

		_m_punit->registerSymbol(pvar);

    proc_info->members.variables.insert(pvar->name, pvar);
  }

  // add the rest of the parameters...
  if(!p)
    return offset;

  size_t j;
  bool bref;
  size_t size;
  syntaxNode *param;
  syntaxNode *pname_list;
  syntaxNode *ptype;
  syntaxNode *pdef_value;
  sym_type  *stype;
  sym_variable *svar;

  bool has_default;
  bool seen_default = false;

  for(size_t i=0;i<p->count;i++)
  {
    bref = false;
    has_default = false;
    
    switch(p->children[i]->node_type)
    {
      case e_parameter:
        param = p->children[i];
      break;
      case e_formal_parameter:
        if((p->children[i]->children[0]->node_type == TOK_VAR)||
           (p->children[i]->children[0]->node_type == TOK_OUT))
          bref = true;
        param = p->children[i]->children[1];
      break;
    }

    pname_list=param->children[0];

    if(param->count>1)
    {
      // check for default...
      if(param->children[1]->node_type != TOK_ARRAY)
      {
        ptype=param->children[1];
        stype=get_type(ns,ptype,ps);

        if(param->count > 2)
        {
          has_default=true;
          seen_default=true;
          pdef_value = param->children[3];
        }
      }
    }
    else
      stype = VOID_TYPE;

    if(!stype)
      throw CSyntaxErr(param->children[1]->getNodeLine(),"Undeclared identifier : %s.",ptype->children[0]->node_name);

    for(j=0;j<pname_list->count;j++)
    {
      svar = new sym_variable;
      svar->name = pname_list->children[j]->node_name;
      svar->parent = ps;
      svar->const_ = 0;
      svar->prop_info = 0;
      svar->static_var = 0;
      svar->param = 1;
      svar->byref = bref?1:0;
      svar->scode = sy_variable;
      svar->visibility = VIS_PUBLIC;
      svar->unit_index = _m_punit->get_index();

			_m_punit->registerSymbol(svar);

      // remember the variable so we can figure out the signature later on...
      pname_list->children[j]->variable = svar;

      if(has_default)
      {
        if(bref)
          throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"Default parameter "
          "'%s' must be by-value or const.", svar->name);

        svar->default_ = 1;
        svar->value.def_val = pdef_value;
        
        if(j!=0)
          throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"%s not allowed here due to default value.",svar->name);

        // add the default value...
        reduce_expression(ns,pdef_value,false);

        if(!_m_punit->is_const(ns,pdef_value))
          throw CSyntaxErr(pdef_value->getNodeLine(),"not a constant expression.");

        // make sure the declared type and the const type agree.
        if(!is_equal_type(stype,get_expr_type(pdef_value)))
          throw CSyntaxErr(pdef_value->getNodeLine(),"types do not match.");

        _m_punit->_m_constants->add(pdef_value);
        proc_info->default_count++;
      }
      else
      {
        svar->default_ = 0;
        if(seen_default)
          throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"default value required for %s.",svar->name);
      }

      // see if the item was already declared...
      if(ps->get_member(svar->name))
        throw CSyntaxErr(pname_list->children[j]->getNodeLine(),"identifier redeclared : %s.",svar->name);

      size = (((stype->size<4)&&(expand_param))||(stype->tycode==ty_class))?4:stype->size;
      offset += (bref)?4:size;
      svar->offset = (int)offset;
      svar->type = stype;
      
      ps->variables.insert(svar->name,svar);
      proc_info->parameters=(sym_variable**)realloc(proc_info->parameters, sizeof(void*)*(proc_info->param_count+1));
      proc_info->parameters[proc_info->param_count]=svar;
      proc_info->param_count++;
    }
  }

  return offset;  
}

void CsyntaxValidator::add_function_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility)
{
  syntaxNode *pheader;
  syntaxNode *prtype;
  syntaxNode *pparams;
  syntaxNode *pdirectives;
  size_t size;
  sym_variable *presult;
  sym_procedure *proc_info;
  sym_procedure *overloaded_proc;

  pheader = p->get_node(e_function_header);
  pdirectives = p->get_node(e_directive_list);
  prtype = pheader->get_node(e_function_return);
  pparams = pheader->get_node(e_parameter_list);
   
  if(pheader->count>1)
    if(pheader->children[1]->node_type == TOK_IDENTIFIER)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"invalid declaration.");
  
  overloaded_proc = (sym_procedure*)ps->get_member(pheader->children[0]->node_name);

  proc_info=new sym_procedure;
	proc_info->psyntax = pheader;
  proc_info->next_overload=0;
  proc_info->default_count = 0;
  proc_info->safecall = (pdirectives && pdirectives->get_node(TOK_SAFECALL))?1:0;
  proc_info->overloaded = (pdirectives && pdirectives->get_node(TOK_OVERLOAD))?1:0;
  proc_info->name = pheader->children[0]->node_name;

  proc_info->built_in = (_m_punit->get_index())?0:1;
  
  proc_info->scode = sy_procedure;
  proc_info->static_proc = pheader->static_;
  proc_info->function = 1;
  proc_info->implemented = 0;
  proc_info->members.name = proc_info->name;
  proc_info->members.parent = 0;
  proc_info->members.size = 0;
  proc_info->members.space_type = 0;
  proc_info->members.first_symbol = 0;
  proc_info->members.last_symbol = 0;
  proc_info->parent=ps->space_type;
  proc_info->visibility = visibility;
  proc_info->constructor = 0;
  proc_info->ret_type = 0;
  proc_info->proc_number=ps->proc_count++;
  proc_info->proc_gnumber = _m_punit->get_procedure_count();
  proc_info->unit_index = _m_punit->get_index();

	_m_punit->registerSymbol(proc_info);

  if(prtype)
    proc_info->ret_type = get_type(ns,prtype->children[0],ps);

  if(!proc_info->ret_type)
    throw CSyntaxErr(prtype->children[0]->getNodeLine(),"Function needs result type.");

  size = add_params(ns,proc_info,pparams);
  
  // add the return type...
  presult = new sym_variable;
  presult->name = duplicateString("result");
  presult->const_ = 0;
  presult->scode = sy_variable;
  presult->param = 1;
  presult->byref = 0;
  presult->default_ = 0;
  presult->static_var = 0;
  presult->prop_info = 0;
  presult->parent = &proc_info->members;
  presult->type = proc_info->ret_type;
  presult->offset = (int)(size+proc_info->ret_type->size);
  presult->unit_index = _m_punit->get_index();

	_m_punit->registerSymbol(presult);

  proc_info->param_size = size - 8;

  proc_info->members.variables.insert(presult->name,presult);
  
  if(overloaded_proc)
  {
    if(overloaded_proc->scode != sy_procedure)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s has already been declared.",pheader->children[0]->node_name);
  
    if(!overloaded_proc->overloaded)
    {
      // allow interfaces to be overloaded without declaring them as such...
      if(!ps->space_type || (ps->space_type->tycode != ty_interface))
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"previous declaration of %s was not marked with the 'overload' directive.",proc_info->name);
      else
        overloaded_proc->overloaded = 1;
    }

    if(!proc_info->overloaded)
    {
      // allow interfaces to be overloaded without declaring them as such...
      if(!ps->space_type || (ps->space_type->tycode != ty_interface))
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"overloaded procedure %s must be marked with the 'overload' directive.",proc_info->name);
      else
        proc_info->overloaded = 1;
    }
  
    if(get_overloaded_declaration(overloaded_proc,pparams))
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"Method '%s' with identical parameters exists already.", proc_info->name );

    proc_info->next_overload = overloaded_proc->next_overload;
    overloaded_proc->next_overload = proc_info;
  } 
  else
  {
    ps->add_symbol(proc_info);
    ps->procedures.insert(proc_info->name,proc_info);
  }

  proc_info->code_name = duplicateString(proc_name(_m_punit,proc_info).c_str());
  _m_punit->add_procedure(proc_info->code_name,proc_info);
  //m_punit->proc_dict.insert(proc_info->code_name,proc_info);
}

void CsyntaxValidator::add_constructor_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility)
{
  syntaxNode *pheader;
  syntaxNode *pparams;
  syntaxNode *pdirectives;

  size_t size;
  sym_variable *presult;
  sym_procedure *proc_info;
  sym_procedure *overloaded_proc;

  pheader = p->get_node(e_constructor_header);
  pparams = pheader->get_node(e_parameter_list);
  pdirectives = p->get_node(e_directive_list);
 
  if(pheader->count>1)
    if(pheader->children[1]->node_type == TOK_IDENTIFIER)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"invalid declaration.");

  overloaded_proc = (sym_procedure*)ps->get_member(pheader->children[0]->node_name);
  
  proc_info=new sym_procedure;
	proc_info->psyntax = pheader;
  proc_info->static_proc = 0;
  proc_info->next_overload=0;
  proc_info->default_count = 0;
  proc_info->overloaded = (pdirectives && pdirectives->get_node(TOK_OVERLOAD))?1:0;
  proc_info->built_in = (_m_punit->get_index())?0:1;
  proc_info->name = pheader->children[0]->node_name;
  proc_info->scode = sy_procedure;
  proc_info->function = 1;
  proc_info->implemented = 0;
  proc_info->members.name = proc_info->name;
  proc_info->members.parent = 0;
  proc_info->members.size = 0;
  proc_info->members.space_type = 0;
  proc_info->members.first_symbol = 0;
  proc_info->members.last_symbol = 0;
  proc_info->parent=ps->space_type;
  proc_info->visibility = visibility;
  proc_info->constructor = 1;
  proc_info->proc_number=ps->proc_count++;
  proc_info->ret_type = ps->space_type;
  proc_info->proc_gnumber = _m_punit->get_procedure_count();
  proc_info->unit_index = _m_punit->get_index();
  
	_m_punit->registerSymbol(proc_info);

  if(!proc_info->ret_type)
    throw CSyntaxErr(pheader->children[1]->getNodeLine(),"Function needs a result type.");

  size = add_params(ns,proc_info,pparams);
  
  // add the return type...
  presult = new sym_variable;
  presult->name = duplicateString("result");
  presult->const_ = 0;
  presult->scode = sy_variable;
  presult->param = 1;
  presult->byref = 0;
  presult->default_ = 0;
  presult->static_var = 0;
  presult->prop_info = 0;
  presult->parent = &proc_info->members;
  presult->type = proc_info->ret_type;
  presult->offset = (int)size+4;// 4 is the size of the class (retgurn type)
  presult->unit_index = _m_punit->get_index();

	_m_punit->registerSymbol(presult);

  proc_info->param_size = size - 8;

  proc_info->members.variables.insert(presult->name,presult);
  proc_info->members.variables.insert("self",presult);
  
  if(overloaded_proc)
  {
    if(overloaded_proc->scode != sy_procedure)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s has already been declared.",pheader->children[0]->node_name);
  
    if(!overloaded_proc->overloaded)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"previous declaration of %s was not marked with the 'overload' directive.",proc_info->name);

    if(!proc_info->overloaded)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"overloaded procedure %s must be marked with the 'overload' directive.",proc_info->name);
  
    if(get_overloaded_declaration(overloaded_proc,pparams))
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"Method '%s' with identical parameters exists already.", proc_info->name );

    proc_info->next_overload = overloaded_proc->next_overload;
    overloaded_proc->next_overload = proc_info;
  } 
  else
  {
    ps->add_symbol(proc_info);
    ps->procedures.insert(proc_info->name,proc_info);
  }

  proc_info->code_name = duplicateString(proc_name(_m_punit,proc_info).c_str());
  _m_punit->add_procedure(proc_info->code_name,proc_info);
}

void CsyntaxValidator::add_procedure_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility)
{
  syntaxNode *pheader;
  syntaxNode *pparams;
  syntaxNode *pdirectives;

  sym_procedure *proc_info;
  sym_procedure *overloaded_proc;

  pheader = p->get_node(e_procedure_header);
  pparams = pheader->get_node(e_parameter_list);
  pdirectives = p->get_node(e_directive_list);

  if(pheader->count>1)
    if(pheader->children[1]->node_type == TOK_IDENTIFIER)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"invalid declaration.");
  
  overloaded_proc = (sym_procedure*)ps->get_member(pheader->children[0]->node_name);

  proc_info=new sym_procedure;
	proc_info->psyntax = pheader;
  proc_info->static_proc = pheader->static_;
  proc_info->next_overload=0;
  proc_info->default_count = 0;
  proc_info->safecall = (pdirectives && pdirectives->get_node(TOK_SAFECALL))?1:0;
  proc_info->overloaded = (pdirectives && pdirectives->get_node(TOK_OVERLOAD))?1:0;
  proc_info->built_in = (_m_punit->get_index())?0:1;
  proc_info->name = pheader->children[0]->node_name;
  proc_info->scode = sy_procedure;
  proc_info->function = 0;
  proc_info->ret_type = 0;
  proc_info->implemented = 0;
  proc_info->parent=ps->space_type;
  proc_info->members.name = proc_info->name;
  proc_info->members.parent = 0;
  proc_info->members.size = 0;
  proc_info->members.space_type = 0;
  proc_info->members.first_symbol =0;
  proc_info->members.last_symbol = 0;
  proc_info->visibility = visibility;
  proc_info->constructor = 0;
  proc_info->param_size = add_params(ns,proc_info,pparams) - 8;
  proc_info->proc_number=ps->proc_count++;
  proc_info->proc_gnumber = _m_punit->get_procedure_count();
  proc_info->unit_index = _m_punit->get_index();

	_m_punit->registerSymbol(proc_info);

  if(overloaded_proc)
  {
    if(overloaded_proc->scode != sy_procedure)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s has already been declared.",pheader->children[0]->node_name);
  
    if(!overloaded_proc->overloaded)
    {
      // allow interfaces to be overloaded without declaring them as such...
      if(!ps->space_type || (ps->space_type->tycode != ty_interface))
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"previous declaration of %s was not marked with the 'overload' directive.",proc_info->name);
      else
        overloaded_proc->overloaded = 1;
    }

    if(!proc_info->overloaded)
    {
      // allow interfaces to be overloaded without declaring them as such...
      if(!ps->space_type || (ps->space_type->tycode != ty_interface))
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"overloaded procedure %s must be marked with the 'overload' directive.",proc_info->name);
      else
        proc_info->overloaded = 1;
    }
  
    if(get_overloaded_declaration(overloaded_proc,pparams))
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"Method '%s' with identical parameters exists already.", proc_info->name );

    proc_info->next_overload = overloaded_proc->next_overload;
    overloaded_proc->next_overload = proc_info;
  } 
  else
  {
    ps->add_symbol(proc_info);
    ps->procedures.insert(proc_info->name,proc_info);
  }

  proc_info->code_name = duplicateString(proc_name(_m_punit,proc_info).c_str());
  _m_punit->add_procedure(proc_info->code_name,proc_info);
  //m_punit->proc_dict.insert(proc_info->code_name,proc_info);
}

//***************************************************************************
// Method Definitions.
//***************************************************************************
void CsyntaxValidator::add_function_definition(symbol_context *ns, sym_namespace *ps, syntaxNode *p)
{
  class_type *ptype; //only classes are allowed to have member functions
  sym_procedure *proc_info=0;
  sym_procedure *overloaded_proc;
  syntaxNode *pheader;
  syntaxNode *pblock;
  syntaxNode *prtype;
  syntaxNode *pparams;
  syntaxNode *pdirectives;
  tlist<sym_procedure*> procedures;
  
  pheader = p->get_node(e_function_header);
  pblock  = p->get_node(e_block);
  prtype = pheader->get_node(e_function_return);
  pparams = pheader->get_node(e_parameter_list);
  pdirectives = p->get_node(e_directive_list);

  symbol_context proc_stack,class_stack;

  proc_stack.ns_var = 0;
  proc_stack.last = ns;
  
  if((pheader->count>1)&&(pheader->children[1]->node_type == TOK_IDENTIFIER))
  {
    ptype = (class_type*)_m_punit->get_symbol(pheader->children[0]->node_name,ns);

    if(!ptype)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    if(ptype->scode != sy_type||ptype->tycode != ty_class)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    class_stack.last = 0;
    class_stack.ps = &ptype->members;
    // find the member function ...
    // it has to be declared as a member of this class
    proc_info = (sym_procedure*)_m_punit->get_symbol(pheader->children[1]->node_name,&class_stack);
    
    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member of %s.", pheader->children[1]->node_name,pheader->children[0]->node_name);

    if(proc_info->scode != sy_procedure)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member function.", pheader->children[1]->node_name);

    if(proc_info->overloaded || pparams)
    {
      reduce_params(&class_stack,pparams);
      proc_info=get_overloaded_declaration(proc_info,pparams);
    }

    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s differs from previous declaration.", pheader->children[1]->node_name);

    tlist<sym_variable*> l;

    if(proc_info->members.variables.find("self",l))
    {
      class_stack.ns_var = *l.begin();
      class_stack.last = ns;
      proc_stack.last = &class_stack;
    }
  }
  else
  {
    // regular object...
    overloaded_proc = (sym_procedure*)ps->get_member(pheader->children[0]->node_name);

    if(overloaded_proc)
    {
      if(overloaded_proc->scode != sy_procedure)
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s has already been declared.",pheader->children[0]->node_name);

      if(!overloaded_proc->overloaded && !pparams)
      {
        proc_info = overloaded_proc;
        overloaded_proc = 0;
      }
      else
      {
        reduce_params(ns,pparams);
         proc_info = get_overloaded_declaration(overloaded_proc,pparams);
      }
    }

    if(!proc_info)
    {
      sym_variable *presult;

      proc_info = new sym_procedure;
			proc_info->psyntax = pheader;
      proc_info->static_proc = 0;
      proc_info->next_overload=0;
      proc_info->default_count = 0;
      proc_info->overloaded = (pdirectives && pdirectives->get_node(TOK_OVERLOAD))?1:0;
      proc_info->implemented = 0;
      proc_info->built_in = 0;
      proc_info->name = pheader->children[0]->node_name;
      proc_info->scode = sy_procedure;
      proc_info->parent = 0;
      proc_info->members.name = proc_info->name;
      proc_info->members.parent = 0;
      proc_info->members.size = 0;
      proc_info->members.space_type = 0;
      proc_info->members.first_symbol = 0;
      proc_info->members.last_symbol = 0;
      proc_info->function = 1;
      proc_info->visibility = VIS_PRIVATE;
      proc_info->constructor = 0;
      proc_info->param_size = add_params(ns,proc_info,pparams)-8;
      proc_info->proc_number=ps->procedures.size();
      proc_info->proc_gnumber = _m_punit->get_procedure_count();
      proc_info->unit_index = _m_punit->get_index();
      
			_m_punit->registerSymbol(proc_info);

      if(prtype)
        proc_info->ret_type = get_type(ns,prtype->children[0],ps);
      else
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"Function needs return type.");

      if(!proc_info->ret_type)
        throw CSyntaxErr(prtype->children[0]->getNodeLine(),"Function needs return type.");

      presult = new sym_variable;
      presult->name = duplicateString("result");
      presult->const_ = 0;
      presult->scode = sy_variable;
      presult->param = 1;
      presult->byref = 0;
      presult->default_ = 0;
      presult->static_var = 0;
      presult->prop_info = 0;
      presult->parent = &proc_info->members;
      presult->type = proc_info->ret_type;
      presult->offset = (int)(proc_info->param_size+proc_info->ret_type->size+8);
      presult->unit_index = _m_punit->get_index();

			_m_punit->registerSymbol(presult);

      proc_info->members.variables.insert(presult->name,presult);

      proc_info->code_name = duplicateString(proc_name(_m_punit,proc_info).c_str());
      _m_punit->add_procedure(proc_info->code_name,proc_info);
      //m_punit->proc_dict.insert(proc_info->code_name,proc_info);

      if(overloaded_proc)
      {
        if(!overloaded_proc->overloaded)
          throw CSyntaxErr(pheader->children[0]->getNodeLine(),"previous declaration of %s was not marked with the 'overload' directive.",proc_info->name);

        if(!proc_info->overloaded)
          throw CSyntaxErr(pheader->children[0]->getNodeLine(),"overloaded procedure %s must be marked with the 'overload' directive.",proc_info->name);
  
        proc_info->next_overload = overloaded_proc->next_overload;
        overloaded_proc->next_overload = proc_info;
      }
      else
      {
        // we are defining a new function...
        ps->procedures.insert(proc_info->name,proc_info);
      }

      ps->add_symbol(proc_info);
    }
  }

  // make sure we haven't defined the function yet...
  if(proc_info->implemented)
    throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s already has a body",proc_info->name);

  if(!proc_info->function)
    throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s differs from previous declaration.", pheader->children[1]->node_name);

  proc_info->implemented = 1;
  proc_stack.ps = &proc_info->members;

  add_definitions(&proc_stack,&proc_info->members,pblock);
  process_statements(&proc_stack,pblock->get_node(e_statement_list));

  p->procedure=proc_info;
}

void CsyntaxValidator::add_constructor_definition(symbol_context *ns,  sym_namespace *ps, syntaxNode *p)
{
  class_type *ptype;//only classes are allowed to have member functions
  sym_procedure *proc_info=0;
  syntaxNode *pheader;
  syntaxNode *pblock;
  syntaxNode *pparams;
  tlist<sym_procedure*> procedures;
  
  pheader = p->get_node(e_constructor_header);
  pblock  = p->get_node(e_block);
  pparams = pheader->get_node(e_parameter_list);

  symbol_context proc_stack,class_stack;

  proc_stack.ns_var = 0;
  proc_stack.last = ns;

  if((pheader->count>1)&&(pheader->children[1]->node_type == TOK_IDENTIFIER))
  {
    ptype = (class_type*)_m_punit->get_symbol(pheader->children[0]->node_name,ns);

    if(!ptype)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    if(ptype->scode != sy_type||ptype->tycode != ty_class)
      throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    class_stack.last = 0;
    class_stack.ps = &ptype->members;
    // find the member function ...
    // it has to be declared as a member of this class
    proc_info = (sym_procedure*)_m_punit->get_symbol(pheader->children[1]->node_name,&class_stack);
    
    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member of %s.", pheader->children[1]->node_name,pheader->children[0]->node_name);

    if((proc_info->scode != sy_procedure)||(proc_info->function==0))
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member function.", pheader->children[1]->node_name);

    if(proc_info->constructor!=1)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a constructor member.",pheader->children[1]->node_name);

    if(proc_info->overloaded || pparams)
    {
      reduce_params(&class_stack,pparams);
      proc_info=get_overloaded_declaration(proc_info,pparams);
    }

    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s differs from previous declaration.", pheader->children[1]->node_name);

    tlist<sym_variable*> l;
    proc_info->members.variables.find("result",l);
        
    class_stack.ns_var = *l.begin();
    class_stack.last = ns;
    proc_stack.last = &class_stack;
  }
  else
    throw CSyntaxErr(pheader->getNodeLine(),"constructor must be a class member.");
  
  proc_info->implemented = 1;
  proc_stack.ps = &proc_info->members;

  add_definitions(&proc_stack,&proc_info->members,pblock);
  process_statements(&proc_stack,pblock->get_node(e_statement_list));

  p->procedure=proc_info;
}

void CsyntaxValidator::add_procedure_definition(symbol_context *ns,  sym_namespace *ps, syntaxNode *p)
{
  class_type *ptype;
  sym_procedure *proc_info=0;
  sym_procedure *overloaded_proc;
  syntaxNode *pheader;
  syntaxNode *pblock;
  syntaxNode *pparams;
  syntaxNode *pdirectives;
  tlist<sym_procedure*> procedures;

  pheader = p->get_node(e_procedure_header);
  pblock  = p->get_node(e_block);
  pparams = pheader->get_node(e_parameter_list);
  pdirectives = p->get_node(e_directive_list);

  symbol_context class_stack,proc_stack;

  proc_stack.ns_var = 0;
  proc_stack.last = ns;

  if((pheader->count>1)&&(pheader->children[1]->node_type == TOK_IDENTIFIER))
  {
    // it has to be declared as a member of the class
    ptype = (class_type*)_m_punit->get_symbol(pheader->children[0]->node_name,ns);

    if(!ptype)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    if(ptype->scode != sy_type||ptype->tycode != ty_class)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a valid class obj.", pheader->children[0]->node_name);

    class_stack.last = 0;
    class_stack.ps = &ptype->members;
    
    // find the member function ...
    proc_info = (sym_procedure*)_m_punit->get_symbol(pheader->children[1]->node_name,&class_stack);

    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member of %s.", pheader->children[1]->node_name,pheader->children[0]->node_name);

    if(proc_info->scode != sy_procedure)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s is not a member function.", pheader->children[1]->node_name);

    if(proc_info->overloaded || pparams)
    {
      reduce_params(&class_stack,pparams);
      proc_info=get_overloaded_declaration(proc_info,pparams);
    }

    if(!proc_info)
      throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s differs from previous declaration.", pheader->children[1]->node_name);

    tlist<sym_variable*> l;
    
    if(proc_info->members.variables.find("self",l))
    {
      class_stack.ns_var = *l.begin();
      class_stack.last = ns;
      proc_stack.last = &class_stack;
    }
  }
  else
  {
    // regular object...
    overloaded_proc = (sym_procedure*)ps->get_member(pheader->children[0]->node_name);

    if(overloaded_proc)
    {
      if(overloaded_proc->scode != sy_procedure)
        throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s has already been declared.",pheader->children[0]->node_name);

      if(!overloaded_proc->overloaded && !pparams)
      {
        proc_info = overloaded_proc;
        overloaded_proc = 0;
      }
      else
      {
        reduce_params(ns,pparams);
         proc_info = get_overloaded_declaration(overloaded_proc,pparams);
      }
    }

    // we are defining a new procedure...
    if(!proc_info)
    {
      proc_info = new sym_procedure;
			proc_info->psyntax = pheader;
      proc_info->static_proc = 0;
      proc_info->next_overload=0;
      proc_info->default_count = 0;
      proc_info->overloaded = (pdirectives && pdirectives->get_node(TOK_OVERLOAD))?1:0;
      proc_info->built_in = 0;
      proc_info->name = pheader->children[0]->node_name;
      proc_info->scode = sy_procedure;
      proc_info->parent = 0;
      proc_info->members.name = proc_info->name;
      proc_info->members.parent = 0;
      proc_info->members.size = 0;
      proc_info->members.space_type = 0;
      proc_info->members.first_symbol = 0;
      proc_info->members.last_symbol = 0;
      proc_info->constructor = 0;
      proc_info->function = 0;
      proc_info->param_size = add_params(ns,proc_info,pparams)-8;
      proc_info->visibility = VIS_PRIVATE;
      proc_info->proc_number=ps->procedures.size();
      proc_info->proc_gnumber = _m_punit->get_procedure_count();
      proc_info->unit_index = _m_punit->get_index();
      
      proc_info->code_name = duplicateString(proc_name(_m_punit,proc_info).c_str());
      _m_punit->add_procedure(proc_info->code_name,proc_info);
      _m_punit->registerSymbol(proc_info);

      if(overloaded_proc)
      {
        if(!overloaded_proc->overloaded)
          throw CSyntaxErr(pheader->children[0]->getNodeLine(),"previous declaration of %s was not marked with the 'overload' directive.",proc_info->name);

        if(!proc_info->overloaded)
          throw CSyntaxErr(pheader->children[0]->getNodeLine(),"overloaded procedure %s must be marked with the 'overload' directive.",proc_info->name);
  
        proc_info->next_overload = overloaded_proc->next_overload;
        overloaded_proc->next_overload = proc_info;
      }
      else
      {
        // we are defining a new function...
        ps->procedures.insert(proc_info->name,proc_info);
      }

      ps->add_symbol(proc_info);
    }
  }

  // make sure we haven't defined the function yet...
  if(proc_info->implemented)
    throw CSyntaxErr(pheader->children[0]->getNodeLine(),"%s already has a body",proc_info->name);

   if(proc_info->function)
    throw CSyntaxErr(pheader->children[1]->getNodeLine(),"%s differs from previous declaration.", pheader->children[1]->node_name);

  proc_info->implemented = 1;
  proc_stack.ps = &proc_info->members;

  add_definitions(&proc_stack, &proc_info->members,pblock);
  process_statements(&proc_stack, pblock->get_node(e_statement_list));

  p->procedure=proc_info;
}

//***************************************************************************
// High Level Definitions
//***************************************************************************
void CsyntaxValidator::process_uses_clause(syntaxNode *p)
{
  // loop trough all of the units...
  syntaxNode *pnames = p->children[0];
  syntaxNode *pUnitName;
  Cmodule *pUnit;
  int rc;

  for(size_t i=0;i<pnames->count;i++)
  {
    try
    {
      pUnitName = pnames->children[i];

      // fist, make sure the unit does not reference itself, because that woule be an error...
      if(strcmp(pUnitName->node_name,_m_punit->get_name())==0)
        throw CSyntaxErr(pUnitName->getNodeLine(),"unit inclusion causes a circular reference.");

      // try to get a copy of the unit...
      pUnit=_m_punit->get_manager()->get_unit(pUnitName->node_name);

      if(!pUnit)
      {
        CpascalParser pP;
        CsyntaxValidator analyze;
  
        // parse the unit...
        pUnit=_m_punit->get_manager()->create_unit(pUnitName->node_name);
        _m_punit->add_unit(pUnit);

				rc = pP.parseFile(pUnit->get_filename(),pUnit->get_syntax_tree(),pUnit);

        if(!rc)
        {
          // first, make sure it's actually a unit... 
          if(pUnit->get_syntax_tree()->getRoot()->children[0]->node_type != e_unit)
            throw CSyntaxErr(pUnitName->getNodeLine(),"%s is not a valid unit.",pUnitName->node_name);

          // do a syntax check on the unit...
          analyze.validateTree(pUnit);
        }
        else
          throw CSyntaxErr(pUnitName->getNodeLine(),"unable to parse %s.pas.",pUnitName->node_name);
      }
      else
        _m_punit->add_unit(pUnit);
    }
    catch(CSyntaxErr err)
    {
      _m_punit->handle_error(&err);
    }
  }
}

void CsyntaxValidator::validate_unit(syntaxNode *p)
{
  // eventually we' have to parse uses clauses...
  // first get all decratations from the interface section...
  syntaxNode *psection;
  syntaxNode *puses;
  size_t i;
  
  //  interface section...
  psection = p->get_node(e_interface_section);

  if(psection)
  {
    // process the uses clauses...
    puses = psection->get_node(e_uses_clause);

    if(puses)
      process_uses_clause(puses);

    psection=psection->get_node(e_interface_declarations);

    for(i=0;i<psection->count;i++)
    {
      switch(psection->children[i]->node_type)
      {
        case e_type_declaration_list:
          add_type_declarations(&_m_punit->name_stack,&_m_punit->global,psection->children[i]);
        break;
        case e_const_definition_list:
          add_const_definitions(&_m_punit->name_stack,&_m_punit->global,psection->children[i]);
        break;
        case e_list_var_declaration:
          add_var_declarations(&_m_punit->name_stack,&_m_punit->global,psection->children[i]);
        break;
        case e_procedure_declaration:
          add_procedure_declaration(&_m_punit->name_stack,&_m_punit->global,psection->children[i],VIS_PUBLIC);
        break;
        case e_function_declaration:
          add_function_declaration(&_m_punit->name_stack,&_m_punit->global,psection->children[i],VIS_PUBLIC);
        break;
      }
    }
  }

  // implementation section...
  psection = p->get_node(e_implementation_section);

  if(psection)
   add_definitions(&_m_punit->name_stack,&_m_punit->global,psection);
}

void CsyntaxValidator::validate_program(syntaxNode *p)
{
  syntaxNode *pblock_program;
  syntaxNode *pblock;
  syntaxNode *puses;

  pblock_program=p->get_node(e_program_block);
  puses=pblock_program->get_node(e_uses_clause);
  pblock=pblock_program->get_node(e_block);

  if(puses)
    process_uses_clause(puses);

  add_definitions(&_m_punit->name_stack,&_m_punit->global,pblock);
  process_statements(&_m_punit->name_stack,pblock->get_node(e_statement_list));
}

void CsyntaxValidator::validate_script(syntaxNode *p)
{
  //syntaxNode *pscript_uses;
  //syntaxNode *pscript_block;

  //pscript_uses = p->get_node(e_uses_clause);
  //pscript_block = p->get_node(e_script_block);

  _m_punit->is_script = true;

  //if(!pscript_uses && !pscript_block)
    //return;

  //if(pscript_uses)
    //process_uses_clause(pscript_uses);

  //if(pscript_block)
 // {
   // add_definitions(&_m_punit->name_stack,&_m_punit->global,pscript_block);
    //process_statements(&_m_punit->name_stack,pscript_block->get_node(e_statement_list));
 // }

	syntaxNode *pCurr;

	for(size_t i = 1; i < p->children.size();i++){
		pCurr = p->children[i];

	  switch(pCurr->node_type)
		{
		  case e_type_declaration:
				add_type_declaration(&_m_punit->name_stack,&_m_punit->global,pCurr);
			break;
			case e_varfield_declaration:
				add_var_declaration(&_m_punit->name_stack,&_m_punit->global,pCurr);
			break;
			case e_const_definition:
				add_const_definition(&_m_punit->name_stack,&_m_punit->global,pCurr);
			break;
			case e_procedure_definition:
        add_procedure_definition(&_m_punit->name_stack,&_m_punit->global,pCurr);
      break;
      case e_function_definition:
        add_function_definition(&_m_punit->name_stack,&_m_punit->global,pCurr);
      break;
      case e_constructor_definition:
        add_constructor_definition(&_m_punit->name_stack,&_m_punit->global,pCurr);
      break;
      case e_destructor_definition:
        throw CSyntaxErr(pCurr->getNodeLine(),"Not implemented");
      break;
			case e_uses_clause:
				process_uses_clause(pCurr);
			break;
		}
	}

	process_statements(&_m_punit->name_stack,p->children[0]);

}

bool CsyntaxValidator::validateTree(Cmodule *unit)
{
  syntaxNode *p = unit->get_syntax_tree()->getRoot();

	Cmodule *pSystem;

  if(unit->get_index()==0){
		pSystem=unit;
	} else {
		pSystem=unit->get_manager()->get_unit("system");
	}
	
	if(pSystem)
	{
		// get the system unit and initialize the native types..
		INTEGER_TYPE=(sym_type*)pSystem->global.get_member("integer");
		REAL_TYPE=(sym_type*)pSystem->global.get_member("extended");
		BOOLEAN_TYPE=(sym_type*)pSystem->global.get_member("boolean");
		STRING_TYPE=(sym_type*)pSystem->global.get_member("string");
		WIDESTRING_TYPE=(sym_type*)pSystem->global.get_member("widestring");
		VOID_TYPE=(sym_type*)pSystem->global.get_member("pointer");
		INTERFACE_TYPE=(sym_type*)pSystem->global.get_member("interface");
		CHAR_TYPE=(sym_type*)pSystem->global.get_member("char");
		VARIANT_TYPE=(sym_type*)pSystem->global.get_member("olevariant");
		DISPATCH_TYPE=(sym_type*)pSystem->global.get_member("idispatch");
	}

  _m_punit = unit;
  
  if(p->count>0)
  {
    switch(p->children[0]->node_type)
    {
      case e_unit:
        validate_unit(p->children[0]);
      break;
      case e_program:
        validate_program(p->children[0]);
      break;
      case e_script:
        validate_script(p->children[0]);
      break;
    }
  }

  return (_m_punit->error_count==0);
}

