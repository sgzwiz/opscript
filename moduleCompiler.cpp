/* 
 * moduleCompiler.cpp - Implementation of the module compiler object. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#include "common.h"
#include "opascalParser.h"
#include "moduleCompiler.h"

#pragma warning( disable : 4267)

bool const_expr(syntaxNode*p)
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
      pvar=p->variable;
      if((pvar->scode == sy_variable)&&pvar->const_)
        return true;
    break;
  }

  return false;
}

std::string loc_text(reg_location l, sym_type *ptype = 0)
{
  std::string s("");
  char buffer[20];

  if(l.const_)
  {
    if(l.deref)
      s += "^";
    else
      s += "@";
    sprintf(buffer,"%i", l.offset+1);
    s += buffer;
    return s; 
  }

  if((l.deref)&&(l.size<=8))
    s+="[";

  if(ptype)
  {
    switch(ptype->tycode)
    {
      case ty_real:
        switch(ptype->size)
        {
          case 4:
            s += 'f';
          break;
          case 8:
            s += 'd';
          break;
        }
      break;
      default:
        ptype = 0;
    }
  }

  if(!ptype)
  {
    switch(l.size)
    {
      case 1:
        s+='c';
      break;
      case 2:
        s+='s';
      break;
      case 4:
        s+='i';
      break;
      case 6:
        s+='f';
      break;
      case 8:
        s+='d';
      break;
      default:
        s+='i';
    }
  }

  sprintf(buffer,"r%03i",l.reg);

  s += buffer;

  if(l.offset != 0)
  {
    if(l.offset>0)
      sprintf(buffer,"+%i",l.offset);
    else
      sprintf(buffer,"%i",l.offset);

    s += buffer;
  }

  if((l.deref)&&(l.size<=8))
    s+="]";

  return s;
}

//*****************************************************************************
// Misc Operations.
//*****************************************************************************
int CmoduleCompiler::alloc_register()
{
  for(int i=3;i<REG_COUNT;i++)
  {
    if(reg_array[i] == -1)
    {
      reg_array[i] = 0;
      if(i>max_reg)
        max_reg = i;
      return i;
    }
  }

  return -1;
}

void CmoduleCompiler::free_register(int i)
{
  reg_array[i]=-1;
}

void CmoduleCompiler::free_location(syntaxNode *p)
{
  free_location(p->reg_loc);
}

void CmoduleCompiler::free_location(reg_location &reg_loc)
{
  if(reg_loc.reg>2)
    free_register(reg_loc.reg);
  if(reg_loc.free_string)
  {
    fprintf(pf,"i push,%s\n", loc_text(reg_loc).c_str());
    fprintf(pf,"i calln,$__freestr\n");
  }
  if(reg_loc.free_bstr)
  {
    fprintf(pf,"i push,%s\n", loc_text(reg_loc).c_str());
    fprintf(pf,"i calln,$__freebstr\n");
  }
  if(reg_loc.free_interface)
  {
    release_interface(loc_text(reg_loc).c_str());
  }
  if(reg_loc.stack>0)
    fprintf(pf,"i sub,ir001,%i\n", reg_loc.stack);
}

void CmoduleCompiler::free_variables(sym_namespace *p)
{
  thash<sym_variable*,17>::iterator it;
  reg_location reg;
  sym_variable *pvar;
  std::string s1;

  reg.deref = 1;
  reg.const_ = 0;
  reg.stack  = 0;
  reg.free_string = 0;
  reg.free_bstr = 0;
  reg.free_interface = 0;
  reg.free_object = 0;

  for(it=p->variables.begin();it!=p->variables.end();it++)
  {
    pvar=(*it);

    if((!pvar->byref)&&(strcmp(pvar->name,"result"))&&strcmp(pvar->name,"self"))
    {
      reg.size = pvar->type->size;
      reg.offset = (pvar->param)?-1 * pvar->offset:pvar->offset;
      reg.reg = (pvar->static_var)?2:0;

      switch(pvar->type->tycode)
      {
        case ty_record:
          FreeObject(&reg,pvar->type);
        break;
        case ty_string:
          // free the string...
          s1=loc_text(reg);
          fprintf(pf,"i push,%s\n",s1.c_str());
          fprintf(pf,"i calln,$__freestr\n");
        break;
        case ty_widestring:
          s1=loc_text(reg);
          fprintf(pf,"i push,%s\n",s1.c_str());
          fprintf(pf,"i calln,$__freebstr\n");
        break;
        case ty_interface:
          s1=loc_text(reg);
          release_interface(s1.c_str());
        break;
      }
    }
  }
}

void CmoduleCompiler::CopyObject(reg_location *p, sym_base *item)
{
  sym_namespace *pns=0;
  sym_base *pb;
  sym_variable *pvar;
  sym_type *ptype;
  std::string s1;
  
  if(item->scode == sy_type)
  {
    switch(((sym_type*)item)->tycode)
    {
      case ty_record:
        pns=&((record_type*)item)->members;
      break;
    }
  }

  if(!pns)
    return;

  for( pb = pns->first_symbol; pb ; pb = pb->next )
  {
    pvar = (sym_variable*)pb;

    if(pvar->scode != sy_variable)
      continue;

    if(pvar->prop_info)
      continue;

    ptype = pvar->type;

    switch(ptype->tycode)
    {
      case ty_interface:
        p->size = 4;
        p->deref = 1;
        s1 = loc_text(*p);
        addref_interface(s1.c_str());
        p->offset+= ptype->size;
      break;
      case ty_string:
        p->size = 4;
        p->deref = 0;
        s1 = loc_text(*p);
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$__cpystr\n");
        p->offset+= ptype->size;
      break;
      case ty_widestring:
        p->size = 4;
        p->deref = 0;
        s1 = loc_text(*p);
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$__cpybstr\n");
        p->offset+= ptype->size;
      break;
      case ty_record:
        CopyObject(p,ptype);
      break;
      default:
        p->offset += ptype->size;
    }
  }
}

void CmoduleCompiler::FreeObject(reg_location *p, sym_base *item)
{
  sym_namespace *pns=0;
  sym_base *pb;
  sym_variable *pvar;
  sym_type *ptype;
  std::string s1;
  
  if(item->scode == sy_type)
  {
    switch(((sym_type*)item)->tycode)
    {
      case ty_record:
        pns=&((record_type*)item)->members;
      break;
    }
  }

  if(!pns)
    return;

  for( pb = pns->first_symbol; pb ; pb = pb->next )
  {
    pvar = (sym_variable*)pb;

    if(pvar->scode != sy_variable)
      continue;

    if(pvar->prop_info)
      continue;

    ptype = pvar->type;

    switch(ptype->tycode)
    {
      case ty_interface:
        p->size = 4;
        p->deref = 1;
        s1 = loc_text(*p);
        release_interface(s1.c_str());
        p->offset+= ptype->size;
      break;
      case ty_string:
        p->size = 4;
        p->deref = 1;
        s1 = loc_text(*p);
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$__freestr\n");
        p->offset+= ptype->size;
      break;
      case ty_widestring:
        p->size = 4;
        p->deref = 1;
        s1 = loc_text(*p);
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$__freebstr\n");
        p->offset+= ptype->size;
      break;
      case ty_record:
        FreeObject(p,ptype);
      break;
      default:
        p->offset += ptype->size;
    }
  }
}

void CmoduleCompiler::release_interface(const char *ploc)
{
  int l1=++label_count;

  fprintf(pf,"i ne,%s,0\n",ploc);
  fprintf(pf,"i jmp,l%i\n",l1);
  fprintf(pf,"i add,ir001,4\n");
  fprintf(pf,"i push,%s\n",ploc);
  fprintf(pf,"i push,4\n");
  fprintf(pf,"i push,4\n");
  fprintf(pf,"i push,2\n");
  fprintf(pf,"i calln,$_interface_thunk\n");
  fprintf(pf,"i sub,ir001,4\n");
  fprintf(pf,"l l%i\n",l1);
  fprintf(pf,"i mov,%s,0\n", ploc);
}
void CmoduleCompiler::addref_interface(const char *ploc)
{
  int l1=++label_count;
  fprintf(pf,"i ne,%s,0\n",ploc);
  fprintf(pf,"i jmp,l%i\n",l1);

  fprintf(pf,"i add,ir001,4\n");
  fprintf(pf,"i push,%s\n",ploc);
  fprintf(pf,"i push,4\n");
  fprintf(pf,"i push,4\n");
  fprintf(pf,"i push,1\n");
  fprintf(pf,"i calln,$_interface_thunk\n");
  fprintf(pf,"i sub,ir001,4\n");
  fprintf(pf,"l l%i\n",l1);
} 
//*****************************************************************************
// Functions.
//*****************************************************************************
void CmoduleCompiler::emit_definitions(syntaxNode *p)
{
  if(!p)
    return;

	try
	{
		size_t i=0;
		for(;i<p->count;i++)
		{
			switch(p->children[i]->node_type)
			{
				case e_function_definition:
					emit_function_definition(p->children[i]);
				break;
				case e_procedure_definition:
					emit_procedure_definition(p->children[i]);
				break;
				case e_constructor_definition:
					emit_constructor_definition(p->children[i]);
				break;
				case e_destructor_definition:
					throw CCompilerErr(p->children[0]->getNodeLine(),"Not implemented");
				break;
			}
		}
	}
	catch(CCompilerErr err)
	{
		  _m_punit->handle_error(&err);
	}
}

void CmoduleCompiler::emit_function_definition(syntaxNode *p)
{
  syntaxNode *block = p->get_node(e_block);
  label_count = 1;
  reg_location reg;
  std::string s1;

  sym_procedure *proc = p->procedure;
  sym_variable *presult;
  long pos1,pos2;

  fprintf(pf,"%s", proc->code_name);
  pos1 = ftell(pf);
  fprintf(pf,"               \n");
  
  // cleanup the stack...
  if(proc->members.size>0)
  {
    fprintf(pf,"i add,ir001,%i\n",proc->members.size);
    fprintf(pf,"i push,%i\n",proc->members.size);
    fprintf(pf,"i push,0\n");
    fprintf(pf,"i push,ir000\n");
    fprintf(pf,"i calln,$_mset\n");
  }

  // initialize the return variable...
  presult=(sym_variable*)proc->members.get_member("result");  
  
  reg.reg = 0;
  reg.const_ = 0;
  reg.stack  = 0;
  reg.free_string = 0;
  reg.free_bstr = 0;
  reg.free_interface = 0;
  reg.free_object = 0;
  reg.offset = -1 * presult->offset;
  reg.size = presult->type->size;
  reg.deref = 1;
  s1 = loc_text(reg);

  if(presult->type->size>8)
  {
    fprintf(pf,"i push,%i\n",presult->type->size);
    fprintf(pf,"i push,0\n");
    fprintf(pf,"i push,%s\n",s1.c_str());
    fprintf(pf,"i calln,$_mset\n");
  }
  else
    fprintf(pf,"i mov,%s,0\n",s1.c_str());

  emit_compound_statement(block->get_node(e_statement_list));
  
  // run any cleanup code necessary..
  fprintf(pf,"l l1\n");

  free_variables(&proc->members);

  if(max_reg-2>0)
    fprintf(pf,"i pop_rgs,%i\n", max_reg-2);  
  fprintf(pf,"i ret,%i\n",proc->param_size);
  fprintf(pf,"\n");
  
  if(max_reg-2>0)
  {
    pos2=ftell(pf);
    fseek(pf,pos1,SEEK_SET);
    fprintf(pf,"\ni push_rgs,%i",max_reg-2);
    fseek(pf,pos2,SEEK_SET);
  }
  max_reg=2;
}

void CmoduleCompiler::emit_procedure_definition(syntaxNode *p)
{
  syntaxNode *block = p->get_node(e_block);
  sym_procedure *proc = p->procedure;

  label_count = 1;

  fprintf(pf,"%s", proc->code_name);

  long pos1,pos2;

  pos1 = ftell(pf);
  fprintf(pf,"               \n");

  // setup local variables
  if(proc->members.size>0)
  {
    fprintf(pf,"i add,ir001,%i\n",proc->members.size);
    fprintf(pf,"i push,%i\n",proc->members.size);
    fprintf(pf,"i push,0\n");
    fprintf(pf,"i push,ir000\n");
    fprintf(pf,"i calln,$_mset\n");
  }

  emit_compound_statement(block->get_node(e_statement_list));

  // run any cleanup code.
  fprintf(pf,"l l1\n");

  free_variables(&proc->members);

  if(max_reg-2>0)
    fprintf(pf,"i pop_rgs,%i\n", max_reg-2);  
  fprintf(pf,"i ret,%i\n",proc->param_size);
  fprintf(pf,"\n");

  if(max_reg-2>0)
  {
    pos2=ftell(pf);
    fseek(pf,pos1,SEEK_SET);
    fprintf(pf,"\ni push_rgs,%i",max_reg-2);
    fseek(pf,pos2,SEEK_SET);
  }
  max_reg=2;
}

void CmoduleCompiler::emit_constructor_definition(syntaxNode *p)
{
  syntaxNode *block = p->get_node(e_block);
  label_count = 1;
  tlist<sym_variable*> litem;
  reg_location reg;
  sym_variable *pvar;
  class_type *pclstype;
  std::string s1;

  reg.deref = 0;
  reg.reg = 0;
  reg.const_ = 0;
  reg.stack  = 0;
  reg.free_string = 0;
  reg.free_bstr = 0;
  reg.free_interface = 0;
  reg.free_object = 0;

  sym_procedure *proc = p->procedure;
  long pos1,pos2;

  fprintf(pf,"%s", proc->code_name);
  pos1 = ftell(pf);
  fprintf(pf,"               \n");

  if(proc->members.size>0)
  {
    fprintf(pf,"i add,ir001,%i\n",proc->members.size);
    fprintf(pf,"i push,%i\n",proc->members.size);
    fprintf(pf,"i push,0\n");
    fprintf(pf,"i push,ir000\n");
    fprintf(pf,"i calln,$_mset\n");
  }

  pclstype=(class_type*)proc->parent;
  proc->members.variables.find("result",litem);
  pvar = (sym_variable*)*litem.begin();
  
  reg.size = pvar->type->size;
  reg.offset = (pvar->param)?-1 * pvar->offset:pvar->offset;

  s1=loc_text(reg);
  
  // initialize the memory for the object...
  fprintf(pf,"i push,%s\n",s1.c_str());
  fprintf(pf,"i push,%i\n",pclstype->members.size);
  fprintf(pf,"i calln,$_GetMem\n");

  emit_compound_statement(block->get_node(e_statement_list));
  
  // run any cleanup code necessary..
  fprintf(pf,"l l1\n");

  free_variables(&proc->members);

  if(max_reg-2>0)
    fprintf(pf,"i pop_rgs,%i\n", max_reg-2);  
  fprintf(pf,"i ret,%i\n",proc->param_size);
  fprintf(pf,"\n");
  
  if(max_reg-2>0)
  {
    pos2=ftell(pf);
    fseek(pf,pos1,SEEK_SET);
    fprintf(pf,"\ni push_rgs,%i",max_reg-2);
    fseek(pf,pos2,SEEK_SET);
  }
  
  max_reg=2;
}

void CmoduleCompiler::emit_destructor_definition(syntaxNode *p)
{
  syntaxNode *block = p->get_node(e_block);
  emit_compound_statement(block->get_node(e_statement_list));
}

//*****************************************************************************
// statements
//*****************************************************************************
void CmoduleCompiler::emit_statement(syntaxNode *p)
{
	try
	{
		switch(p->node_type)
		{
			case e_statement_raise:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
			break;
			case e_statement_with:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
			break;
			case e_statement_assign:
				emit_statement_assign(p);
			break;
			case e_statement_repeat:
				emit_statement_repeat(p);
			break;
			case e_statement_while:
				emit_statement_while(p);
			break;
			case e_statement_if:
				emit_statement_if(p);
			break;
			case e_statement_for:
				emit_statement_for(p);
			break;
			case e_statement_case:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
			break;
			case e_statement_list:
				emit_compound_statement(p);
			break;
			case e_statement_try:
			break;
			case e_statement:
				emit_simple_statement(p);
			break;
			case e_statement_goto:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
			break;
			case e_statement_label:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
			break;
			default:
				throw CCompilerErr(p->getNodeLine(),"Not implemented");
		}
	}
	catch(CCompilerErr err)
	{
	  _m_punit->handle_error(&err);
	}
}

void CmoduleCompiler::emit_statement_assign(syntaxNode *p)
{
  std::string s1,s2;
  reg_location tmp_reg;

  syntaxNode *pdest=p->children[0];
  syntaxNode *psrc=p->children[1];
  sym_type *ptype1 = get_expr_type(pdest);
  sym_type *ptype2 = get_expr_type(psrc);

  emit_expression(psrc);
  s2 = loc_text(psrc->reg_loc,ptype2);

  emit_expression(pdest);
  s1 = loc_text(pdest->reg_loc,ptype1);

  // free the existing object...
  switch(ptype1->tycode)
  {
    case ty_interface:
      if(psrc->node_type==TOK_NIL)
      {
        release_interface(s1.c_str());
        return;
      }
      
      if(ptype2->tycode==ty_interface)
      {
        release_interface(s1.c_str());
        // if we are calling a function, we should not call
        // addref on the object, because the assignment to result 
        // within the procedure will do that...
        if(psrc->reg_loc.free_interface)
          psrc->reg_loc.free_interface=0;
        else
          addref_interface(s2.c_str());
      }
    break;
    case ty_string:
      // we have to free the string before we copy it...
      fprintf(pf,"i push,%s\n",s1.c_str());
      fprintf(pf,"i calln,$__freestr\n");
    break;
    case ty_widestring:
      // we have to free the string before we copy it...
      fprintf(pf,"i push,%s\n",s1.c_str());
      fprintf(pf,"i calln,$__freebstr\n");
    break;
    case ty_record:
      tmp_reg = pdest->reg_loc;
      FreeObject(&tmp_reg,ptype1);
    break;
  }

  if(pdest->reg_loc.size>8)
  {
    if(ptype1->tycode==ty_variant)
    {
      // we are assigning a structure (or a variant)
      fprintf(pf,"i push,%s\n",s2.c_str());
      fprintf(pf,"i push,%s\n",s1.c_str());

      switch(ptype2->tycode)
      {
        case ty_char:
          fprintf(pf,"i calln,$_chartovar\n");
        break;
        case ty_integer:
          switch(ptype2->size){
            case 1: fprintf(pf,"i calln,$_chartovar\n");break;
            case 2: fprintf(pf,"i calln,$_shorttovar\n");break;
            case 4: fprintf(pf,"i calln,$_inttovar\n");break;
          }
        break;
        case ty_real:
          switch(ptype2->size){
            case 4: fprintf(pf,"i calln,$_floattovar\n");break;
            case 8: fprintf(pf,"i calln,$_doubletovar\n");break;
          }
        break;
        case ty_boolean:
          fprintf(pf,"i calln,$_booltovar\n");
        break;
        case ty_string:
          fprintf(pf,"i calln,$_stringtovar\n");
        break;
        case ty_widestring:
          fprintf(pf,"i calln,$_widestrtovar\n");
        break;
        case ty_variant:
          fprintf(pf,"i calln,$_varcopy\n");          
        break;
        case ty_interface:
          fprintf(pf,"i calln,$_interfacetovar\n");
        break;
      }
    }
    else
    {
      fprintf(pf,"i push,%i\n", pdest->reg_loc.size);
      fprintf(pf,"i push,%s\n", s2.c_str());
      fprintf(pf,"i push,%s\n", s1.c_str());
      fprintf(pf,"i calln,$_mcpy\n");
    }
  }
  else
  {
    if(ptype2->tycode == ty_variant)
    {
      char c;

      fprintf(pf,"i add,ir001,%i\n",ptype1->size);
      fprintf(pf,"i push,%s\n",s2.c_str());

      switch(ptype1->tycode)
      {
        case ty_char:
          c = 'c';
          fprintf(pf,"i calln,$_vartochar\n");
        break;
        case ty_integer:
          switch(ptype1->size)
          {
            case 1:
              c='c';
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case 2:
              c='s';
              fprintf(pf,"i calln,$_vartoshort\n");
            break;
            case 4:
              c='i';
              fprintf(pf,"i calln,$_vartoint\n");
            break;
          }
        break;
        case ty_real:
          switch(ptype1->size)
          {
            case 4:
              c='f';
              fprintf(pf,"i calln,$_vartofloat\n");
            break;
            case 8:
              c='d';
              fprintf(pf,"i calln,$_vartodouble\n");
            break;
          }
        break;
        case ty_boolean:
          c = 'c';
          fprintf(pf,"i calln,$_vartobool\n");
        break;
        case ty_string:
          c = 'i';
          fprintf(pf,"i calln,$_vartostring\n");
        break;
        case ty_widestring:
          c = 'i';
          fprintf(pf,"i calln,$_vartowidestr\n");
        break;
        case ty_interface:
          c = 'i';
          fprintf(pf,"i calln,$_vartodispatch\n");
        break;
      }

      fprintf(pf,"i sub,ir001,%i\n",ptype1->size);
      fprintf(pf,"i mov,%s,[%cr001]\n", s1.c_str(),c);
    }
    else
    {
      fprintf(pf,"i mov,%s,%s\n", s1.c_str(),s2.c_str());

      switch(ptype1->tycode)
      {
        case ty_string:
          if(ptype2->tycode == ty_string)
          {
            if(psrc->reg_loc.free_string)
              psrc->reg_loc.free_string=0;
            else
            {
              tmp_reg = pdest->reg_loc;
              tmp_reg.deref = 0;
              s2 = loc_text(tmp_reg);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__cpystr\n");
            }
          }
          if(ptype2->tycode == ty_widestring)
          {
            tmp_reg = pdest->reg_loc;
            tmp_reg.deref = 0;
            s2 = loc_text(tmp_reg);
            fprintf(pf,"i push,0\n");
            fprintf(pf,"i push,%s\n",s2.c_str());
            fprintf(pf,"i calln,$__cnvbstrtostr\n");
          }
        break;
        case ty_widestring:
          if(ptype2->tycode == ty_widestring)
          {
            if(psrc->reg_loc.free_bstr)
              psrc->reg_loc.free_bstr=0;
            else
            {
              tmp_reg = pdest->reg_loc;
              tmp_reg.deref = 0;
              s2 = loc_text(tmp_reg);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__cpybstr\n");
            }
          }
          if(ptype2->tycode == ty_string)
          {
            tmp_reg = pdest->reg_loc;
            tmp_reg.deref = 0;
            s2 = loc_text(tmp_reg);
            fprintf(pf,"i push,0\n");
            fprintf(pf,"i push,%s\n",s2.c_str());
            fprintf(pf,"i calln,$__cnvstrtobstr\n");
          }
        break;
      }
    }
  }

  // make sure all elements in the object are copied properly.
  if(ptype2->tycode == ty_record)
  {
    reg_location rg = pdest->reg_loc;
    CopyObject(&rg,ptype2);
  }
  
  free_location(pdest);
  free_location(psrc);
}
void CmoduleCompiler::emit_statement_raise(syntaxNode *p)
{
}

void CmoduleCompiler::emit_statement_with(syntaxNode *p)
{
}

void CmoduleCompiler::emit_statement_repeat(syntaxNode *p)
{
  syntaxNode *p1=p->children[1];
  int l1=++label_count;
  int l2=++label_count;
  int l3;
  syntaxNode *expr1;
  syntaxNode *expr2;
  std::string s1;
  std::string s2;
  sym_type *ptype;
  int r;
  int r2;
  char c;
  sym_type *ptype1,*ptype2;

  _m_loopContext.push(l2);

  fprintf(pf,"l l%i\n",l1);

  switch(p1->node_type)
  {
    case e_relational_expression:
      emit_statement(p->children[0]);
      
      expr1=p1->children[0];
      expr2=p1->children[2];
      
      ptype1 = get_expr_type(expr1);
      ptype2 = get_expr_type(expr2);

      if(ptype1->tycode == ty_variant || ptype2->tycode == ty_variant)
      {
        if(ptype1->tycode == ptype2->tycode)
        {
          emit_expression(expr1);
          emit_expression(expr2);
        }
        else
        {
          if(ptype1->tycode==ty_variant)
          {
            // switch the expressions
            ptype1=ptype2;
            ptype2=get_expr_type(expr1);
            expr1=p1->children[2];
            expr2=p1->children[0];
          }

          emit_expression(expr1);
          emit_expression(expr2);

          expr2->reg_loc.deref = 0;
          s2 = loc_text(expr2->reg_loc);

          fprintf(pf,"i add,ir001,%i\n",ptype1->size);
          fprintf(pf,"i push,%s\n",s2.c_str());

          // convert the second expression to a nonvariant type...
          switch(ptype1->tycode)
          {
            case ty_char:
              c = 'c';
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case ty_integer:
              switch(ptype1->size)
              {
                case 1:
                  c='c';
                  fprintf(pf,"i calln,$_vartochar\n");
                break;
                case 2:
                  c='s';
                  fprintf(pf,"i calln,$_vartoshort\n");
                break;
                case 4:
                  c='i';
                  fprintf(pf,"i calln,$_vartoint\n");
                break;
              }
            break;
            case ty_real:
              switch(ptype1->size)
              {
                case 4:
                  c='f';
                  fprintf(pf,"i calln,$_vartofloat\n");
                break;
                case 8:
                  c='d';
                  fprintf(pf,"i calln,$_vartodouble\n");
                break;
              }
            break;
            case ty_boolean:
              c = 'c';
              fprintf(pf,"i calln,$_vartobool\n");
            break;
            case ty_string:
                c = 'i';
              fprintf(pf,"i calln,$_vartostring\n");
            break;
            case ty_widestring:
              c = 'i';
              fprintf(pf,"i calln,$_vartowidestr\n");
            break;
            case ty_interface:
              c = 'i';
              fprintf(pf,"i calln,$_vartodispatch\n");
            break;
          }

          r2 = alloc_register();

          fprintf(pf,"i sub,ir001,%i\n",ptype1->size);
          fprintf(pf,"i mov,%cr%03i,[%cr001]\n",c,r2,c);
          free_location(expr2);

          memset(&expr2->reg_loc,0,sizeof(expr2->reg_loc));

          expr2->reg_loc.reg = r2;
          expr2->reg_loc.size = ptype1->size;
          expr1=p1->children[0];
          expr2=p1->children[2];
        }
      }
      else
      {
        emit_expression(expr1);
        emit_expression(expr2);
      }

      switch(ptype1->tycode)
      {
        case ty_variant:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        case ty_string:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        case ty_widestring:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        default:
          s1=loc_text(expr1->reg_loc);
          s2=loc_text(expr2->reg_loc);
      }

      switch(p1->children[1]->node_type)
      {
        case TOK_LT:
          fprintf(pf,"i lt,");
        break;
        case TOK_GT:
          fprintf(pf,"i gt,");
        break;
        case TOK_LE:
          fprintf(pf,"i le,");
        break;
        case TOK_GE:
          fprintf(pf,"i ge,");
        break;
      }
      
      l3 = ++label_count;
      fprintf(pf,"%s,%s\n",s1.c_str(),s2.c_str());
      fprintf(pf,"i jmp,l%i\n",l3);
      fprintf(pf,"i jmp,l%i\n",l2);          
      fprintf(pf,"l l%i\n",l3);
    break;
    default:
      emit_statement(p->children[0]);

      emit_expression(p1);
      ptype = get_expr_type(p1);

      if(ptype->tycode == ty_variant)
      {
        p1->reg_loc.deref = 0;
        s1 = loc_text(p1->reg_loc);
        r = alloc_register();

        fprintf(pf,"i add,ir001,4\n");
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$_vartoint\n");
        fprintf(pf,"i sub,ir001,4\n");
        fprintf(pf,"i mov,ir%03i,[ir001]\n",r);
        free_location(p1);
        memset(&p1->reg_loc,0,sizeof(p1->reg_loc));
        p1->reg_loc.size = 4;
        p1->reg_loc.reg = r;
        s1 = loc_text(p1->reg_loc);
        free_register(r);
      }
      else
      {
        s1 = loc_text(p1->reg_loc); 
        free_location(p1);
      }
      
      fprintf(pf,"i eq,%s,0\n",s1.c_str());
      fprintf(pf,"i jmp,l%i\n",l2);          
  }
 
  fprintf(pf,"i jmp,l%i\n",l1);
  fprintf(pf,"l l%i\n",l2);

  _m_loopContext.pop();
}

void CmoduleCompiler::emit_statement_for (syntaxNode *p)
{
  syntaxNode *loop_var = p->children[0];
  syntaxNode *init_expr = p->children[1];
  syntaxNode *inc_expr = p->children[2];
  syntaxNode *loop_expr = p->children[3];
  syntaxNode *statement = p->children[4];
  sym_type *ptype;
  int r1=-1;

  int l1=++label_count;
  int l2=++label_count;
  std::string s1,s2;

  _m_loopContext.push(l2);

  // initialize the control variable
  emit_expression(loop_var);
  emit_expression(init_expr);

  ptype = get_expr_type(init_expr);

  s1 = loc_text(loop_var->reg_loc);
  s2 = loc_text(init_expr->reg_loc);

  if(ptype->tycode==ty_variant)
  {
    fprintf(pf,"i add,ir001,4\n");
    fprintf(pf,"i push,%s\n",s2.c_str());
    fprintf(pf,"i calln,$_vartoint\n");
    fprintf(pf,"i sub,ir001,4\n");
    s2 = "[ir001]";
  }
  else
    s2 = loc_text(init_expr->reg_loc);

  fprintf(pf,"i mov,%s,%s\n", s1.c_str(), s2.c_str());
  free_location(init_expr);

  // start the loop
  fprintf(pf,"l l%i\n",l1);

  // emit the control expression
  emit_expression(loop_expr);
  s2 = loc_text(loop_expr->reg_loc);

  ptype = get_expr_type(loop_expr);

  if(ptype->tycode == ty_variant)
  {
    reg_location rg;
    memset(&rg,0,sizeof(rg));
    r1 = alloc_register();
    fprintf(pf,"i add,ir001,4\n");
    fprintf(pf,"i push,%s\n",s2.c_str());
    fprintf(pf,"i calln,$_vartoint\n");
    fprintf(pf,"i sub,ir001,4\n");

    rg.deref = 0;
    rg.reg = r1;
    rg.size = 4;

    s2 = loc_text(rg);

    fprintf(pf,"i mov,%s,[ir001]\n",s2.c_str());
    free_register(r1);
  }
  
  free_location(loop_expr->reg_loc);

  fprintf(pf,"i %s,%s,%s\n",(inc_expr->node_type == TOK_TO)?"le":"ge",s1.c_str(),s2.c_str());
  fprintf(pf,"i jmp,l%i\n",l2);

  // emit the statement
  emit_statement(statement);

  // increment the control variable...
  fprintf(pf,"i %s,%s,1\n",(inc_expr->node_type == TOK_TO)?"add":"sub",s1.c_str());
  fprintf(pf,"i jmp,l%i\n",l1);

  // end the loop...
  fprintf(pf,"l l%i\n",l2);

  _m_loopContext.pop();
}

void CmoduleCompiler::emit_statement_while (syntaxNode *p)
{
  syntaxNode *p1=p->children[0];
  int l1=++label_count;
  int l2=++label_count;
  syntaxNode *expr1;
  syntaxNode *expr2;
  std::string s1;
  std::string s2;
  sym_type *ptype;
  int r;
  int r2;
  char c;
  sym_type *ptype1,*ptype2;

  _m_loopContext.push(l2);

  fprintf(pf,"l l%i\n",l1);

  switch(p1->node_type)
  {
    case e_relational_expression:
      expr1=p1->children[0];
      expr2=p1->children[2];
      
      ptype1 = get_expr_type(expr1);
      ptype2 = get_expr_type(expr2);

      if(ptype1->tycode == ty_variant || ptype2->tycode == ty_variant)
      {
        if(ptype1->tycode == ptype2->tycode)
        {
          emit_expression(expr1);
          emit_expression(expr2);
        }
        else
        {
          if(ptype1->tycode==ty_variant)
          {
            // switch the expressions
            ptype1=ptype2;
            ptype2=get_expr_type(expr1);
            expr1=p1->children[2];
            expr2=p1->children[0];
          }

          emit_expression(expr1);
          emit_expression(expr2);

          expr2->reg_loc.deref = 0;
          s2 = loc_text(expr2->reg_loc);

          fprintf(pf,"i add,ir001,%i\n",ptype1->size);
          fprintf(pf,"i push,%s\n",s2.c_str());

          // convert the second expression to a nonvariant type...
          switch(ptype1->tycode)
          {
            case ty_char:
              c = 'c';
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case ty_integer:
              switch(ptype1->size)
              {
                case 1:
                  c='c';
                  fprintf(pf,"i calln,$_vartochar\n");
                break;
                case 2:
                  c='s';
                  fprintf(pf,"i calln,$_vartoshort\n");
                break;
                case 4:
                  c='i';
                  fprintf(pf,"i calln,$_vartoint\n");
                break;
              }
            break;
            case ty_real:
              switch(ptype1->size)
              {
                case 4:
                  c='f';
                  fprintf(pf,"i calln,$_vartofloat\n");
                break;
                case 8:
                  c='d';
                  fprintf(pf,"i calln,$_vartodouble\n");
                break;
              }
            break;
            case ty_boolean:
              c = 'c';
              fprintf(pf,"i calln,$_vartobool\n");
            break;
            case ty_string:
                c = 'i';
              fprintf(pf,"i calln,$_vartostring\n");
            break;
            case ty_widestring:
              c = 'i';
              fprintf(pf,"i calln,$_vartowidestr\n");
            break;
            case ty_interface:
              c = 'i';
              fprintf(pf,"i calln,$_vartodispatch\n");
            break;
          }

          r2 = alloc_register();

          fprintf(pf,"i sub,ir001,%i\n",ptype1->size);
          fprintf(pf,"i mov,%cr%03i,[%cr001]\n",c,r2,c);
          free_location(expr2);

          memset(&expr2->reg_loc,0,sizeof(expr2->reg_loc));

          expr2->reg_loc.reg = r2;
          expr2->reg_loc.size = ptype1->size;
          expr1=p1->children[0];
          expr2=p1->children[2];
        }
      }
      else
      {
        emit_expression(expr1);
        emit_expression(expr2);
      }

      switch(ptype1->tycode)
      {
        case ty_variant:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        case ty_string:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        case ty_widestring:
					throw CCompilerErr(p->getNodeLine(),"Not implemented");
        break;
        default:
          s1=loc_text(expr1->reg_loc);
          s2=loc_text(expr2->reg_loc);
      }

      switch(p1->children[1]->node_type)
      {
        case TOK_LT:
          fprintf(pf,"i lt,");
        break;
        case TOK_GT:
          fprintf(pf,"i gt,");
        break;
        case TOK_LE:
          fprintf(pf,"i le,");
        break;
        case TOK_GE:
          fprintf(pf,"i ge,");
        break;
      }

      fprintf(pf,"%s,%s\n",s1.c_str(),s2.c_str());

      fprintf(pf,"i jmp,l%i\n",l2);          
 
      emit_statement(p->children[1]);
    break;
    default:
      emit_expression(p1);
      ptype = get_expr_type(p1);

      if(ptype->tycode == ty_variant)
      {
        p1->reg_loc.deref = 0;
        s1 = loc_text(p1->reg_loc);
        r = alloc_register();

        fprintf(pf,"i add,ir001,4\n");
        fprintf(pf,"i push,%s\n",s1.c_str());
        fprintf(pf,"i calln,$_vartoint\n");
        fprintf(pf,"i sub,ir001,4\n");
        fprintf(pf,"i mov,ir%03i,[ir001]\n",r);
        free_location(p1);
        memset(&p1->reg_loc,0,sizeof(p1->reg_loc));
        p1->reg_loc.size = 4;
        p1->reg_loc.reg = r;
        s1 = loc_text(p1->reg_loc);
        free_register(r);
      }
      else
      {
        s1 = loc_text(p1->reg_loc); 
        free_location(p1);
      }
      
      fprintf(pf,"i ne,%s,0\n",s1.c_str());
      fprintf(pf,"i jmp,l%i\n",l2);          
 
      emit_statement(p->children[1]);
  }
 
  fprintf(pf,"i jmp,l%i\n",l1);
  fprintf(pf,"l l%i\n",l2);

  _m_loopContext.pop();
}

void CmoduleCompiler::emit_statement_if (syntaxNode *p)
{
  int l1 = ++label_count;
  syntaxNode *p1=p->children[0];
  std::string s1;
  sym_type *ptype;
  int r;
  
  emit_expression(p1);
  ptype = get_expr_type(p1);
  
  if(ptype->tycode == ty_variant)
  {
    p1->reg_loc.deref = 0;
    s1 = loc_text(p1->reg_loc);
    r = alloc_register();

    fprintf(pf,"i add,ir001,4\n");
    fprintf(pf,"i push,%s\n",s1.c_str());
    fprintf(pf,"i calln,$_vartoint\n");
    fprintf(pf,"i sub,ir001,4\n");
    fprintf(pf,"i mov,ir%03i,[ir001]\n",r);
    free_location(p1);
    memset(&p1->reg_loc,0,sizeof(p1->reg_loc));
    p1->reg_loc.size = 4;
    p1->reg_loc.reg = r;
    s1 = loc_text(p1->reg_loc);
    free_register(r);
  }
  else
  {
    s1 = loc_text(p1->reg_loc); 
    free_location(p1);
  }
  
  fprintf(pf,"i ne,%s,0\n", s1.c_str());
  fprintf(pf,"i jmp,l%i\n", l1);

  // emit the statement...
  emit_statement(p->children[1]);

  if(p->count>2)
  {
    int l2 = ++label_count;

    fprintf(pf,"i jmp,l%i\n", l2);
    fprintf(pf,"l l%i\n", l1);
    emit_statement(p->children[2]);
    fprintf(pf,"l l%i\n", l2);
  }
  else
    fprintf(pf,"l l%i\n", l1);
}

void CmoduleCompiler::emit_statement_case (syntaxNode *p)
{
  throw CSyntaxErr(p->getNodeLine(),"Not implemented");
}

void CmoduleCompiler::emit_compound_statement(syntaxNode *p)
{
  size_t i = 0;

  if(!p)
    return;
  for(i=0;i<p->count;i++)
    emit_statement(p->children[i]);
}

void CmoduleCompiler::emit_statement_try (syntaxNode *p)
{
  throw CSyntaxErr(p->getNodeLine(),"Not implemented");
}

void CmoduleCompiler::emit_simple_statement(syntaxNode *p)
{
  switch(p->children[0]->node_type)
  {
    case TOK_IDENTIFIER:
      if(p->children[0]->variable->scode==sy_procedure)
      {
        emit_method_call(p->children[0],false);
        free_location(p->children[0]);
      }
    break;
    case e_proc_func_expression:
      emit_method_call(p->children[0],false);
      free_location(p->children[0]);
    break;
    case e_record_expression:
      emit_record_expression(p->children[0],false);
      free_location(p->children[0]);
    break;
    case e_simple_expression:
      syntaxNode *pexpr=p->children[0];

      switch(pexpr->children[0]->node_type)
      {
        case TOK_IDENTIFIER:
          if(strcmp(pexpr->children[0]->node_name,"break")==0)
            fprintf(pf,"i jmp,l%i\n",_m_loopContext.top());

          if(strcmp(pexpr->children[0]->node_name,"exit")==0)
            fprintf(pf,"i jmp,l1\n");
        break;
      }
    break;
  }
}

void CmoduleCompiler::emit_statement_goto( syntaxNode *p)
{
  throw CSyntaxErr(p->getNodeLine(),"Not implemented");
}

void CmoduleCompiler::emit_statement_label( syntaxNode *p)
{
  throw CSyntaxErr(p->getNodeLine(),"Not implemented");
}

//****************************************************************
// Expressions.
//****************************************************************
void CmoduleCompiler::emit_expression(syntaxNode *pexpr)
{ 
  sym_variable *pvar;
  int r;
  reg_location tmp_reg;
  std::string s1;
  Cmodule *pUnit;

  if(const_expr(pexpr))
  {
    if(pexpr->variable->scode == sy_variable)
    {
      pvar = pexpr->variable;

      if((pvar->unit_index!=_m_punit->get_index())||
         (pvar->type->tycode == ty_record) ||
         (pvar->type->tycode == ty_array))
      {
        pUnit=_m_punit->get_manager()->get_unit(pvar->unit_index);

        pexpr->reg_loc.const_=0;
        pexpr->reg_loc.deref = 1;
        pexpr->reg_loc.reg = alloc_register();
        pexpr->reg_loc.offset = (int)(pvar->offset + pUnit->global.size + 4 * (pUnit->get_procedure_count() + 2));

        fprintf(pf,"i mov_ds,ir%03i,%i\n",pexpr->reg_loc.reg,pvar->unit_index);
      }
      else
      {
        pexpr->reg_loc.const_ = 1;
        pexpr->reg_loc.offset = (int)pexpr->cindex;
        pexpr->reg_loc.deref = 1;
      }

      pexpr->reg_loc.size = pvar->type->size;
    }
    else
    {
      pexpr->reg_loc.const_ = 1;
      pexpr->reg_loc.offset = (int)pexpr->cindex;
      pexpr->reg_loc.deref = 1;
    }
    
    pexpr->reg_loc.stack = 0;
    pexpr->reg_loc.free_string = 0;
    pexpr->reg_loc.free_bstr = 0;
    pexpr->reg_loc.free_interface = 0;
    pexpr->reg_loc.free_object = 0;
    return;
  }

  pexpr->reg_loc.const_ = 0;
  pexpr->reg_loc.offset = 0;
  pexpr->reg_loc.stack  = 0;
  pexpr->reg_loc.free_string = 0;
  pexpr->reg_loc.free_bstr = 0;
  pexpr->reg_loc.free_interface = 0;
  pexpr->reg_loc.free_object = 0;

  switch(pexpr->node_type)
  {
    case TOK_IDENTIFIER:
      switch(pexpr->variable->scode)
      { 
        case sy_variable:
          pvar=pexpr->variable;

          if(pvar->param)
          {
            // parameters will obviously never be in another unit...
            if(pvar->byref)
            {
              r = alloc_register();
              pexpr->reg_loc.reg = r;
              pexpr->reg_loc.offset = 0;
              
              fprintf(pf,"i mov,ir%03i,[ir000-%i]\n",r,pvar->offset);
            }
            else
            {
              pexpr->reg_loc.reg = 0;
              pexpr->reg_loc.offset = -1 * pvar->offset;
            }

            pexpr->reg_loc.deref = 1;
            pexpr->reg_loc.size = pvar->type->size;
          }
          else
          {
            // these variables may be in another unit.
            if(pexpr->implied_var)
            {
              int sign = (pexpr->implied_var->param)?-1:1;
              r = alloc_register();
              tmp_reg.const_ = 0;
              tmp_reg.offset = sign * pexpr->implied_var->offset;
              tmp_reg.stack = 0;
              tmp_reg.reg = (pexpr->implied_var->static_var)?2:0;
              tmp_reg.deref = 1;
              tmp_reg.size = 4;
              tmp_reg.free_string = 0;
              tmp_reg.free_bstr = 0;
              tmp_reg.free_interface = 0;
              tmp_reg.free_object = 0;

              s1 = loc_text(tmp_reg);
              fprintf(pf,"i mov,ir%03i,%s\n",r,s1.c_str());
              
              pexpr->reg_loc.reg = r;
              pexpr->reg_loc.offset = pvar->offset;
              pexpr->reg_loc.deref = 1;
              pexpr->reg_loc.size = pvar->type->size;
              pexpr->reg_loc.free_string = 0;
              pexpr->reg_loc.free_bstr = 0;
              pexpr->reg_loc.free_interface = 0;
              pexpr->reg_loc.free_object = 0;
            }
            else
            {
              pexpr->reg_loc.reg = (pvar->static_var)?2:0;
              pexpr->reg_loc.offset = pvar->offset;
              pexpr->reg_loc.deref = 1;
              pexpr->reg_loc.size = pvar->type->size;
              pexpr->reg_loc.free_string = 0;
              pexpr->reg_loc.free_bstr = 0;
              pexpr->reg_loc.free_interface = 0;
              pexpr->reg_loc.free_object = 0;

              if(pvar->unit_index != _m_punit->get_index())
              {
                //load the value of the variable into a register...
                pexpr->reg_loc.reg = alloc_register();
                fprintf(pf,"i mov_ds,ir%03i,%i\n",pexpr->reg_loc.reg,pvar->unit_index);
              }
            }
          }
        break;
        case sy_procedure:
          emit_method_call(pexpr,true);
        break;
      }
    break;
    case e_proc_func_expression:
      emit_method_call(pexpr,true);
    break;
    case e_as_expression:
      emit_as_expression(pexpr);
    break;
    case e_multiplicative_expression:
      emit_multiplicative_expression(pexpr);
    break;
    case e_additive_expression:
      emit_additive_expression(pexpr);
    break;
    case e_record_expression:
      emit_record_expression(pexpr,true);
    break;
    case e_relational_expression:
      emit_relational(pexpr);
    break;
    case e_equality_expression:
      emit_equality(pexpr);
    break;
    case e_logical_or_expression:
      emit_logical_or(pexpr);
    break;
    case e_logical_and_expression:
      emit_logical_and(pexpr);
    break;
    case e_array_elt_expression:
      emit_expression(pexpr->children[0]);
      emit_array_expression(&pexpr->children[0]->reg_loc,pexpr);
    break;
  }
}
void CmoduleCompiler::emit_multiplicative_expression(syntaxNode *pexpr)
{
  char *inst;
  int r = alloc_register();
  reg_location reg;
  std::string s1;
  std::string s2;
  sym_type *ptype = get_expr_type(pexpr);

  reg.const_ = 0;
  reg.deref = 0;
  reg.offset = 0;
  reg.size = ptype->size;
  reg.stack = 0;
  reg.reg = r;
  reg.free_string = 0;
  reg.free_bstr = 0;
  reg.free_interface = 0;
  reg.free_object = 0;

  // assign the register the first expression
  emit_expression(pexpr->children[0]);
  
  s1 = loc_text(reg);
  s2 = loc_text(pexpr->children[0]->reg_loc);

  fprintf(pf,"i mov,%s,%s\n", s1.c_str(), s2.c_str());
  free_location(pexpr->children[0]);

  for(size_t i=1;i<pexpr->count;i++)
  {
    switch(pexpr->children[i]->node_type)
    {
      case TOK_SLASH:
        inst = "div";
      break;
      case TOK_STAR:
        inst = "mult";
      break;
      case TOK_DIV:
        inst = "div";
      break;
      case TOK_MOD:
        inst = "mod";
      break;
      default:
        emit_expression(pexpr->children[i]);
        s2 = loc_text(pexpr->children[i]->reg_loc);
        fprintf(pf,"i %s,%s,%s\n",inst,s1.c_str(),s2.c_str());
        free_location(pexpr->children[i]);
    }
  }

  // add all the following expressions to the same location...
  pexpr->reg_loc = reg;
}

void CmoduleCompiler::emit_additive_expression(syntaxNode *pexpr)
{
  char *inst;
  int r = alloc_register();
  sym_type *ptmptype;
  sym_type *pexpr_type;
  reg_location reg;
  std::string s1;
  std::string s2;
  std::string s3;

  pexpr_type = get_expr_type(pexpr);

  reg.const_ = 0;
  reg.deref = 0;
  reg.offset = 0;
  reg.size = pexpr_type->size;
  reg.stack = 0;
  reg.reg = r;
  reg.free_string = 0;
  reg.free_bstr = 0;
  reg.free_interface = 0;
  reg.free_object = 0;

  // assign the first expression to the register 
  emit_expression(pexpr->children[0]);
  
  s1 = loc_text(reg);
  s2 = loc_text(pexpr->children[0]->reg_loc);
  
  ptmptype = get_expr_type(pexpr->children[0]);

  fprintf(pf,"i mov,%s,%s\n", s1.c_str(), s2.c_str());

  // we have to make a copy of the string...
  if(pexpr_type->tycode == ty_string)
  {
    reg.free_string = 1;

    // the types will always match, because the expression type is set
    // to the type of the first element of the expression
    if(ptmptype->tycode == ty_string)
    {
      if(pexpr->children[0]->reg_loc.free_string)
        pexpr->children[0]->reg_loc.free_string=0;
      else
      {
        fprintf(pf,"i push,%i\n", r);
        fprintf(pf,"i calln,$__cpyregstr\n");
      }
    }
  }

  // we have to make a copy of the string...
  if(pexpr_type->tycode == ty_widestring)
  {
    reg.free_bstr = 1;

    // the types will always match, because the expression type is set
    // to the type of the first element of the expression
    if(ptmptype->tycode == ty_widestring)
    {
      if(pexpr->children[0]->reg_loc.free_bstr)
        pexpr->children[0]->reg_loc.free_bstr=0;
      else
      {
        fprintf(pf,"i push,%i\n", r);
        fprintf(pf,"i calln,$__cpyregbstr\n");
      }
    }
  }

  free_location(pexpr->children[0]);

  for(size_t i=1;i<pexpr->count;i++)
  {
    switch(pexpr->children[i]->node_type)
    {
      case TOK_PLUS:
        inst = "add";
      break;
      case TOK_MINUS:
        inst = "sub";
      break;
      default:
        ptmptype=get_expr_type(pexpr->children[i]);

        emit_expression(pexpr->children[i]);
        s2 = loc_text(pexpr->children[i]->reg_loc);

        switch(pexpr_type->tycode)
        {
          case ty_string:
            if(ptmptype->tycode==ty_string)
            {
              fprintf(pf,"i push,%i\n",r);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__catstr\n");
            }

            if(ptmptype->tycode==ty_widestring)
            {
              fprintf(pf,"i push,%i\n",r);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__catbstrtostr\n");
            }
          break;
          case ty_widestring:
            if(ptmptype->tycode==ty_widestring)
            {
              fprintf(pf,"i push,%i\n",r);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__catbstr\n");
            }
            if(ptmptype->tycode==ty_string)
            {
              fprintf(pf,"i push,%i\n",r);
              fprintf(pf,"i push,%s\n",s2.c_str());
              fprintf(pf,"i calln,$__catstrtobstr\n");
            }
          break;
          default:
            fprintf(pf,"i %s,%s,%s\n",inst,s1.c_str(),s2.c_str());
        }

        free_location(pexpr->children[i]);
    }
  }

  // add all the following expressions to the same location...
  pexpr->reg_loc = reg;
}
            
void CmoduleCompiler::push_parameter(std::string &expr, sym_type *expr_type, bool ref)
{
  // push onto the stack...
  if((expr_type->size>8)&&(!ref))
  {
    fprintf(pf,"i push,%i\n",expr_type->size);
    fprintf(pf,"i push,%s\n",expr.c_str());
    fprintf(pf,"i push,ir001-8\n");
    fprintf(pf,"i calln, $_mcpy\n");
    fprintf(pf,"i add,ir001,%i\n",expr_type->size);
  }
  else
    fprintf(pf,"i push,%s\n",expr.c_str());
}

void CmoduleCompiler::emit_parameter(bool ref, sym_type *pparam_type, syntaxNode *pparam_expr)
{
  std::string s1,s2;
  reg_location rg;
  sym_type *ptype = get_expr_type(pparam_expr);

  if(pparam_expr->variable->scode!=sy_procedure)
  {
    // the expression is either an inline constant (in which case pvar holds a type) or
    // a variable (which may or may not be a constant)
    emit_expression(pparam_expr);

    // make sure that we are pasing a pointer.
    if(ref)
    {
      pparam_expr->reg_loc.deref=0;
      pparam_expr->reg_loc.size=4;
    }

    s1 = loc_text(pparam_expr->reg_loc,(ref)?NULL:ptype);

    if(ref)
      push_parameter(s1,ptype,ref);// no conversions needed for vars
    else
    {
      // do the necessary conversions before we push the parameter onto 
      // the stack...
      if(ptype->tycode != pparam_type->tycode)
      {
        // the following inline conversions are possible:
        // ty_variant -> ty_integer
        // ty_variant -> ty_real
        // ty_variant -> ty_boolean
        // ty_variant -> ty_char
        // ty_variant -> ty_string
        // ty_variant -> ty_widestring
        // ty_variant -> ty_interface
        // ty_integer -> ty_variant
        // ty_real    -> ty_variant
        // ty_boolean -> ty_variant
        // ty_char    -> ty_variant
        // ty_string  -> ty_variant
        // ty_widestring -> ty_variant
        // ty_interface  -> ty_variant
        // all other ones are either not supported, or are automatically
        // handled by push_parameter
        if(ptype->tycode==ty_variant||pparam_type->tycode==ty_variant)
        {
          switch(pparam_type->tycode)
          {
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n",pparam_type->size);
              fprintf(pf,"i push,%s\n",s1.c_str());
              fprintf(pf,"i push,ir001-%i\n",ptype->size + pparam_type->size);
               
              switch(ptype->tycode)
              {
                case ty_boolean:
                  fprintf(pf,"i calln,$_booltovar\n");
                break;
                case ty_char:
                  fprintf(pf,"i calln,$_chartovar\n");
                break;
                case ty_integer:
                  switch(ptype->size){
                    case 1:fprintf(pf,"i calln,$_bytetovar\n");break;
                    case 2:fprintf(pf,"i calln,$_shorttovar\n");break;
                    case 4:fprintf(pf,"i calln,$_inttovar\n");break;
                  }
                break;
                case ty_real:
                  switch(ptype->size){
                    case 4:fprintf(pf,"i calln,$floattovar\n");break;
                    case 8:fprintf(pf,"i calln,$_doubletovar\n");break;
                  }
                break;
                case ty_string:
                  fprintf(pf,"i calln,$_stringtovar\n");
                break;
                case ty_widestring:
                  fprintf(pf,"i calln,$_widestrtovar\n");
                break;
                case ty_interface:
                  fprintf(pf,"i calln,$_interfacetovar\n");
                break;
              }
            break;
            default:
              fprintf(pf,"i add,ir001,%i\n",pparam_type->size);
              fprintf(pf,"i push,%s\n",s1.c_str());
              switch(pparam_type->tycode)
              {
                case ty_boolean:
                  fprintf(pf,"i calln,$_vartobool\n");
                break;
                case ty_char:
                  fprintf(pf,"i calln,$_vartochar\n");
                break;
                case ty_integer:
                  switch(pparam_type->size){
                    case 1:fprintf(pf,"i calln,$_vartobyte\n");break;
                    case 2:fprintf(pf,"i calln,$_vartoshort\n");break;
                    case 4:fprintf(pf,"i calln,$_vartoint\n");break;
                  }
                break;
                case ty_real:
                  switch(pparam_type->size){
                    case 4:fprintf(pf,"i calln,$_vartofloat\n");break;
                    case 8:fprintf(pf,"i calln,$_vartodouble\n");break;
                  }
                break;
                case ty_string:
                  fprintf(pf,"i calln,$_vartostring\n");
                break;
                case ty_widestring:
                  fprintf(pf,"i calln,$_vartowidestr\n");
                break;
                case ty_interface:
                  printf("");
                break;
              }
          }
        }
        else
          push_parameter(s1,ptype,ref);
      }
      else
        push_parameter(s1,ptype,ref);
      // if necessary, copy the parameter. the copy functions do inline 
      // conversions.
      switch(pparam_type->tycode)
      {
        case ty_string:
          switch(ptype->tycode)
          {
            case ty_string:
            {
              if(pparam_expr->reg_loc.free_string)
                pparam_expr->reg_loc.free_string=0;
              else
              {
                fprintf(pf,"i push,ir001-4\n");
                fprintf(pf,"i calln,$__cpystr\n");
              }
            }
            break;
            case ty_widestring:
            {
              fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_bstr);
              fprintf(pf,"i push,ir001-4\n");
              fprintf(pf,"i calln,$__cnvbstrtostr\n");
              pparam_expr->reg_loc.free_bstr=0;
            }
            break;
          }
        break;
        case ty_widestring:
          switch(ptype->tycode)
          {
            case ty_widestring:
            {
              if(pparam_expr->reg_loc.free_bstr)
                pparam_expr->reg_loc.free_bstr=0;
              else
              {
                fprintf(pf,"i push,ir001-4\n");
                fprintf(pf,"i calln,$__cpybstr\n");
              }
            }
            break;
            case ty_string:
            {
              fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_string);
              fprintf(pf,"i push,ir001-4\n");
              fprintf(pf,"i calln,$__cnvstrtobstr\n");
              pparam_expr->reg_loc.free_string=0;              
            }
            break;
          }
        break;
        case ty_record:
          rg.const_ = 0;
          rg.deref = 1;
          rg.free_string = 0;
          rg.free_bstr = 0;
          rg.free_interface = 0;
          rg.free_object = 0;
          rg.reg = 1;
          rg.offset = -1 * ptype->size;
          rg.size = 0;
          rg.stack = 0;
          
          CopyObject(&rg,pparam_type);
        break;
        case ty_interface:
          if(pparam_expr->reg_loc.free_interface)
            pparam_expr->reg_loc.free_interface=0;
          else
          {
            s1="[ir001-4]";
            addref_interface(s1.c_str());
          }
        break;
      }
    }
      
    free_location(pparam_expr);
  }
  // it's a procedure..
  else
  {
    // do any necessary conversion
    if(ptype->tycode != pparam_type->tycode)
    {
      rg.const_ = 0;
      rg.deref = 1;
      rg.reg = 1;
      rg.size = pparam_type->size;
      switch(ptype->tycode)
      {
        case ty_char:
          switch(pparam_type->tycode)
          {
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              fprintf(pf,"i calln,$_chartovar\n");break;
            break;
          }
        break;  
        case ty_boolean:
          switch(pparam_type->tycode)
          {
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              //emit_method_call(pparam_expr,true);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              fprintf(pf,"i calln,$_booltovar\n");break;
            break;
          }
        break;
        case ty_integer:
          switch(pparam_type->tycode)
          {
            case ty_real:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              rg.offset = -1 * (pparam_type->size + ptype->size);
              s1 = loc_text(rg,pparam_type);
              s2 = loc_text(pparam_expr->reg_loc,ptype);
              fprintf(pf,"i mov,%s,%s\n",s1.c_str(),s2.c_str());
              free_location(pparam_expr->reg_loc);
            break;
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              switch(ptype->size){
                case 1:fprintf(pf,"i calln,$_bytetovar\n");break;
                case 2:fprintf(pf,"i calln,$_shorttovar\n");break;
                case 4:fprintf(pf,"i calln,$_inttovar\n");break;
              }
            break;
          }
        break;
        case ty_real:
          switch(pparam_type->tycode)
          {
            case ty_integer:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              rg.offset = -1 * (pparam_type->size + ptype->size);
              s1 = loc_text(rg,pparam_type);
              s2 = loc_text(pparam_expr->reg_loc,ptype);
              fprintf(pf,"i mov,%s,%s\n",s1.c_str(),s2.c_str());
              free_location(pparam_expr->reg_loc);
            break;
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              //emit_method_call(pparam_expr,true);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              switch(ptype->size){
                case 4:fprintf(pf,"i calln,$_floattovar\n");break;
                case 8:fprintf(pf,"i calln,$_doubletovar\n");break;
              }
            break;
          }
        break;
        case ty_variant:
          fprintf(pf,"i add,ir001,%i\n", pparam_type->size + 4);
          emit_expression(pparam_expr);
          fprintf(pf,"i sub,ir001,%i\n", ptype->size+4);
          fprintf(pf,"i push,ir001+4\n");
          switch(pparam_type->tycode)
          {
            case ty_char:
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case ty_boolean:
              fprintf(pf,"i calln,$_vartobool\n");
            break;
            case ty_integer:
              switch(pparam_type->size){
                case 1:fprintf(pf,"i calln,$_vartobyte\n");break;
                case 2:fprintf(pf,"i calln,$_vartoshort\n");break;
                case 4:fprintf(pf,"i calln,$_vartoint\n");break;
              }
            break;
            case ty_real:
              switch(pparam_type->size){
                case 4:fprintf(pf,"i calln,$_vartofloat\n");break;
                case 8:fprintf(pf,"i calln,$_vartodouble\n");break;
              }
            break;
            case ty_string:
              fprintf(pf,"i calln,$_vartostring\n");
            break;
            case ty_widestring:
              fprintf(pf,"i calln,$_vartowidestr\n");
            break;
          }
          free_register(pparam_expr->reg_loc.reg);
        break;
        case ty_string:
          switch(pparam_type->tycode)
          {
            case ty_widestring:
              emit_expression(pparam_expr);
              fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_string);
              fprintf(pf,"i push,ir001-8\n");
              fprintf(pf,"i calln,$__cnvstrtobstr\n");
              free_register(pparam_expr->reg_loc.reg);
            break;
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              fprintf(pf,"i calln,$_stringtovar\n");
              free_register(pparam_expr->reg_loc.reg);
            break;                                           
          }
        break;
        case ty_widestring:
          switch(pparam_type->tycode)
          {
            case ty_string:
              emit_expression(pparam_expr);
              fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_bstr);
              fprintf(pf,"i push,ir001-8\n");
              fprintf(pf,"i calln,$__cnvbstrtostr\n");
              free_register(pparam_expr->reg_loc.reg);
            break;
            case ty_variant:
              fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
              emit_expression(pparam_expr);
              fprintf(pf,"i push,ir001-%i\n",pparam_type->size+ptype->size);
              fprintf(pf,"i calln,$_widestrtovar\n");
              free_register(pparam_expr->reg_loc.reg);
            break;
          }
        break;
      }
    }
    else 
    if(ptype->size != pparam_type->size)
    {
      rg.const_ = 0;
      rg.deref = 1;
      rg.reg = 1;
      rg.size = pparam_type->size;
      switch(ptype->tycode)
      {
        case ty_integer:
        case ty_real:
          fprintf(pf,"i add,ir001,%i\n", pparam_type->size);
          emit_expression(pparam_expr);
          rg.offset = -1 * (pparam_type->size + ptype->size);
          s1 = loc_text(rg,pparam_type);
          s2 = loc_text(pparam_expr->reg_loc,ptype);
          fprintf(pf,"i mov,%s,%s\n",s1.c_str(),s2.c_str());
          free_location(pparam_expr->reg_loc);
        break;
        case ty_record:
          emit_expression(pparam_expr);
          free_register(pparam_expr->reg_loc.reg);
        break;
      }
    }
    else
    {
      emit_expression(pparam_expr);
      free_register(pparam_expr->reg_loc.reg);
    }
  } 
}


void CmoduleCompiler::emit_builtin(sym_procedure *pproc, syntaxNode *pparams)
{
  fprintf(pf,"i calln,$_%s\n", pproc->name);
}

void CmoduleCompiler::emit_method_call(syntaxNode *pexpr, bool alloc_r)
{
  sym_procedure *pproc = pexpr->procedure;
  sym_type *ptype,*pparam_type;
  sym_variable *pvar;

  int r;
  size_t i;
  std::string s1,s2;

  //if(!pproc->implemented)
  //{
  // should be flagged as a compiler error.
  //}

  if(pexpr->implied_var)
  {
    reg_location tmp_reg;
    pvar=pexpr->implied_var;
    int sign = (pvar->param)?-1:1;
    
    tmp_reg.const_ = 0;
    tmp_reg.offset = sign * pvar->offset;
    tmp_reg.stack = 0;
    tmp_reg.reg = (pvar->static_var)?2:0;
    tmp_reg.deref = 1;
    tmp_reg.size = 4;
    tmp_reg.free_string = 0;
    tmp_reg.free_bstr = 0;
    tmp_reg.free_interface = 0;
    tmp_reg.free_object = 0;

    // it's either an interface call or a class call...
    switch(pvar->type->tycode)
    {
      case ty_class:
        emit_class_call(pexpr,pexpr,(class_type*)pvar->type,tmp_reg,alloc_r);
      break;
      case ty_interface:
        emit_interface_call(pexpr,pexpr,(interface_type*)pvar->type,tmp_reg,alloc_r);
      break;
      case ty_variant:
				throw CCompilerErr(pexpr->getNodeLine(),"Not implemented");
      break;
    }

    return;
  }

  pexpr->reg_loc.reg = 0;
  pexpr->reg_loc.const_ = 0;
  pexpr->reg_loc.offset = 0;
  pexpr->reg_loc.deref = 1;

  if(pproc->function)
  {
    ptype = pproc->ret_type;

    // setup the return size and also the set up the variables needed to 
    // clean up the result on return.
    switch(ptype->tycode)
    {
      case ty_string:
      case ty_widestring:
      case ty_record:
      case ty_interface:
        alloc_r=true;
      break;
    }

    if(alloc_r)
    {
      r = alloc_register();
      pexpr->reg_loc.reg = r;
      fprintf(pf,"i mov,ir%03i,ir001\n",r);
    }

    fprintf(pf,"i add,ir001,%i\n", ptype->size);
    pexpr->reg_loc.stack=ptype->size;
    pexpr->reg_loc.size=ptype->size;
    pexpr->reg_loc.free_string = (ptype->tycode==ty_string)?1:0;
    pexpr->reg_loc.free_bstr = (ptype->tycode==ty_widestring)?1:0;
    pexpr->reg_loc.free_interface = (ptype->tycode==ty_interface)?1:0;
    pexpr->reg_loc.free_object = (ptype->tycode==ty_record)?1:0;
  }
  else
  {
    // it's a procedure, we don't need to clean up the stack.
    pexpr->reg_loc.size = 0;
    pexpr->reg_loc.stack = 0;
    pexpr->reg_loc.free_string = 0;
    pexpr->reg_loc.free_bstr = 0;
    pexpr->reg_loc.free_interface = 0;
    pexpr->reg_loc.free_object = 0;
  }

  // push all the parameters onto the stack...
  syntaxNode *pparams=pexpr->get_node(e_expression_list);
  syntaxNode *pparam_expr;
  bool bref;

  if(pproc->param_count>0)
  {
    // for functions created in the script the parameters
    // are pushed in reverse order.
    for(i=pproc->param_count;i>0;i--)
    {
      // see if we are using a default parameter...
      if((pparams) && pparams->count>=i)
      {
        bref = pproc->parameters[i-1]->byref;
        pparam_expr=pparams->children[i-1];
      }
      else
      {
        bref = false;
        pparam_expr=pproc->parameters[i-1]->value.def_val;
      }

      pparam_type = pproc->parameters[i-1]->type;

      emit_parameter(bref,pparam_type,pparam_expr);
    }
  }

  if(pproc->built_in)
     fprintf(pf,"i calln,%s\n", pproc->code_name);
  else
  if(pproc->unit_index != _m_punit->get_index())
  {
    fprintf(pf,"i callu,%s,%i\n", pproc->code_name, pproc->unit_index);
    fprintf(pf,"i mv_sg,%i\n",_m_punit->get_index());
  }
  else
    fprintf(pf,"i call,%s\n", pproc->code_name);
}


void CmoduleCompiler::emit_class_call(syntaxNode *pexpr, 
                         syntaxNode *p2, 
                         class_type *pitype, 
                         reg_location reg_l,
                         bool alloc_r)
{
  int r;
  size_t i;

  reg_location tmp_reg;
  std::string s1;
  std::string s2;

  sym_procedure *pproc = p2->procedure; 
  sym_type *ptype;
  sym_variable *pvar;

  tmp_reg.reg = 0;
  tmp_reg.const_ = 0;
  tmp_reg.offset = 0;
  tmp_reg.deref = 1;
  tmp_reg.size = 4;
  tmp_reg.free_string = 0;
  tmp_reg.free_bstr = 0;
  tmp_reg.free_interface = 0;
  tmp_reg.free_object = 0;

  //reserve space for the the result type...
  if(pproc->function)
  {
    if(alloc_r)
    {
      r = alloc_register();
      tmp_reg.reg = r;
      fprintf(pf,"i mov,ir%03i,ir001\n",r);
    }
    fprintf(pf,"i add,ir001,%i\n", pproc->ret_type->size);
    tmp_reg.stack=pproc->ret_type->size;
  }
  else
    tmp_reg.stack=0;

  // push all the parameters onto the stack...
  syntaxNode *pparams=pexpr->get_node(e_expression_list);
  syntaxNode *pparam_expr;

  if(pproc->param_count>0)
  {
    // they are pushed in reverse order.
    for(i=pproc->param_count;i>0;i--)
    {
      // see if we are using a default parameter...
      if((pparams) && pparams->count>=i)
        pparam_expr=pparams->children[i-1];
      else
        pparam_expr=pproc->parameters[i-1]->value.def_val;

      ptype = get_expr_type(pparam_expr);
      pvar = pparam_expr->variable;

      if(pvar->scode!=sy_procedure)
      {
        emit_expression(pparam_expr);

        if(pproc->parameters[i-1]->byref)
        {
          pparam_expr->reg_loc.deref=0;
          pparam_expr->reg_loc.size=4;
        }

        s1 = loc_text(pparam_expr->reg_loc);

        if((ptype->size>8)&&(!pproc->parameters[i-1]->byref))
        {
          fprintf(pf,"i push,%i\n",ptype->size);
          fprintf(pf,"i push,%s\n",s1.c_str());
          fprintf(pf,"i push,ir001-8\n");
          fprintf(pf,"i calln, $_mcpy\n");
          fprintf(pf,"i add,ir001,%i\n",ptype->size);
        }
        else
          fprintf(pf,"i push,%s\n",s1.c_str());

        if(!pproc->parameters[i-1]->byref)
        {
          // gotta copy the parameter ...
          switch(pproc->parameters[i-1]->type->tycode)
          {
            case ty_string:
              if(ptype->tycode == ty_string)
              {
                if(pparam_expr->reg_loc.free_string)
                  pparam_expr->reg_loc.free_string=0;
                else
                {
                  fprintf(pf,"i push,ir001-4\n");
                  fprintf(pf,"i calln,$__cpystr\n");
                }
              }

              if(ptype->tycode == ty_widestring )
              {
                fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_bstr);
                fprintf(pf,"i push,ir001-4\n");
                fprintf(pf,"i calln,$__cnvbstrtostr\n");
                pparam_expr->reg_loc.free_bstr=0;
              }
            break;
            case ty_widestring:
              if(ptype->tycode == ty_widestring)
              {
                if(pparam_expr->reg_loc.free_bstr)
                  pparam_expr->reg_loc.free_bstr=0;
                else
                {
                  fprintf(pf,"i push,ir001-4\n");
                  fprintf(pf,"i calln,$__cpybstr\n");
                }
              }

              if(ptype->tycode == ty_string)
              {
                fprintf(pf,"i push,%i\n",(int)pparam_expr->reg_loc.free_string);
                fprintf(pf,"i push,ir001-4\n");
                fprintf(pf,"i calln,$__cnvstrtobstr\n");
                pparam_expr->reg_loc.free_string=0;              
              }
            break;
            case ty_record:
              reg_location rg;
              rg.const_ = 0;
              rg.deref = 1;
              rg.free_string = 0;
              rg.free_bstr = 0;
              rg.free_interface = 0;
              rg.free_object = 0;
              rg.reg = 1;
              rg.offset = -1 * ptype->size;
              rg.size = 0;
              rg.stack = 0;
              
              CopyObject(&rg,pproc->parameters[i-1]->type);
            break;
            case ty_interface:
              addref_interface(s1.c_str());
            break;
          }
        }

        free_location(pparam_expr);
      }
      else
      {
        emit_expression(pparam_expr);
      }
    }
  }

  // finally push the object onto the stack.
  s1 = loc_text(reg_l);
  pexpr->reg_loc = reg_l;
  free_location(pexpr);

  fprintf(pf,"i push,%s\n",s1.c_str());
  if(pproc->unit_index != _m_punit->get_index())
  {
    fprintf(pf,"i callu,%s,%i\n", pproc->code_name, pproc->unit_index);
    fprintf(pf,"i mv_sg,%i\n",_m_punit->get_index());
  }
  else
    fprintf(pf,"i call,%s\n", pproc->code_name);
  pexpr->reg_loc = tmp_reg;
}

void CmoduleCompiler::emit_interface_call(syntaxNode *pexpr, 
                         syntaxNode *p2, 
                         interface_type *pitype, 
                         reg_location reg_l,
                         bool alloc_r)
{
  int r;
  int expr_r;
  size_t i;
  unsigned j;
  unsigned scount;
  int rs;
  int result_stack=0;

  reg_location tmp_reg;
  std::string s1;
  std::string s2;

  sym_procedure *pproc = p2->procedure; 
  sym_type *ptype;
  sym_type *pptype;
  sym_type *prettype;
  sym_variable *pvar;

  tmp_reg.reg = 0;
  tmp_reg.const_ = 0;
  tmp_reg.offset = 0;
  tmp_reg.deref = 1;
  tmp_reg.size = 4;
  tmp_reg.free_string = 0;
  tmp_reg.free_bstr = 0;
  tmp_reg.free_interface = 0;
  tmp_reg.free_object = 0;
  tmp_reg.stack = 0;

  // find out how many parameters are strings, and reserve the space for the unicode 
  // strings.
  syntaxNode *pparams=p2->get_node(e_expression_list);

  for(i=0,scount=0;i<(int)pproc->param_count;i++)
  {
    ptype = get_expr_type(pparams->children[i]);

    if(ptype->tycode == ty_string)
    {
      scount++;
      tmp_reg.stack += 4;// each string is 4 bytes.
    }
  }

  if(scount>0)
  {
    rs = alloc_register();
    fprintf(pf,"i mov,ir%03i,ir001\n",rs);
    fprintf(pf,"i add,ir001,%i\n",tmp_reg.stack);
  }

  //reserve space for the the result type...
  if(pproc->function||pproc->safecall)
  {
    prettype=pproc->ret_type;

    if(pproc->safecall)
    {
      result_stack=4;
      // we will always need to know where the result is on 
      // stack, because we pass this pointer to the function...
      if(prettype)
        alloc_r=true;
    }

    if(prettype)
    {
      result_stack+=prettype->size;
      tmp_reg.stack+=prettype->size;
    }

    if(alloc_r)
    {
      r = alloc_register();
      tmp_reg.reg = r;
      fprintf(pf,"i mov,ir%03i,ir001\n",r);
    }
  
    fprintf(pf,"i add,ir001,%i\n", result_stack);
  }

  // push the object onto the stack.
  s1 = loc_text(reg_l);
  pexpr->reg_loc = reg_l;
  free_location(pexpr);

  fprintf(pf,"i push,%s\n",s1.c_str());

  // push all parameters...
  if(pparams)
  {
    for(i=0,j=0;i<pparams->count;i++)
    {
      ptype = get_expr_type(pparams->children[i]);
      pptype = pproc->parameters[i]->type;
      pvar = pparams->children[i]->variable;

      if(pvar->scode!=sy_procedure)
      {
        emit_expression(pparams->children[i]);

        if(pproc->parameters[i]->byref)
        {
          pparams->children[i]->reg_loc.deref=0;
          pparams->children[i]->reg_loc.size=4;  
        }

        s1 = loc_text(pparams->children[i]->reg_loc);

        if((ptype->size>8)&&(!pproc->parameters[i]->byref))
        {
          fprintf(pf,"i push,%i\n",ptype->size);
          fprintf(pf,"i push,%s\n",s1.c_str());
          fprintf(pf,"i push,ir001-8\n");
          fprintf(pf,"i calln, $_mcpy\n");
          fprintf(pf,"i add,ir001,%i\n",ptype->size);
        }
        else
          fprintf(pf,"i push,%s\n",s1.c_str());

        free_location(pparams->children[i]);
      }
      else
      {
        // we have to make sure the size on the stack coincides with the
        // the size the call expects
        if((pptype->size>8)||(pptype->tycode==ty_record)) 
          emit_method_call(pparams->children[i],false);
        else
        {
          if((ptype->size!=pptype->size)||(pptype->size<4))
          {
            expr_r = alloc_register();
            fprintf(pf,"i mov,ir%03i,ir001\n",expr_r);
            
            switch(pptype->tycode)
            {
              case ty_integer:
                fprintf(pf,"i add,ir001,4\n");
              break;
              case ty_real:
                switch(pptype->size)
                {
                  case 6:fprintf(pf,"i add,ir001,4\n");break;
                  case 8:fprintf(pf,"i add,ir001,8\n");break;
                }
              break;
            }

            emit_method_call(pparams->children[i],true);
            
            s1 = loc_text(pparams->children[i]->reg_loc);

            switch(pptype->tycode)
            {
              case ty_integer:
                fprintf(pf,"i mov,[ir%03i],%s\n",r,s1.c_str());
              break;
              case ty_real:
                switch(pptype->size)
                {
                  case 6:fprintf(pf,"i mov,[fr%03i],%s\n",r,s1.c_str());break;
                  case 8:fprintf(pf,"i mov,[dr%03i],%s\n",r,s1.c_str());break;
                }
              break;
            }

            free_location(pparams->children[i]);
            free_register(expr_r);
          }
          else
            emit_method_call(pparams->children[i],false);
        }
      }
      
      if(ptype->tycode == ty_string)
      {
        fprintf(pf,"i push,ir%03i+%i\n",rs,j*4);
        fprintf(pf,"i calln,$_strtowide\n");
        fprintf(pf,"i push,[ir%03i+%i]\n",rs,j*4);
        j++;
      }
    }
  }
 
  int stack_ret_size;
  int stack_param_size;

  if(pproc->safecall)
  { 
    stack_ret_size = 4; // the result is always HRESULT (4 bytes).
    stack_param_size = pproc->param_size + 4; // parameters plus the object ptr.
   
    // if we do have a result to 'return' push this parameter onto 
    // the stack also...
    if(prettype)
    {
      reg_location rg2=tmp_reg;

      rg2.size = 4; // the variable will always be a pointer
      rg2.deref = 0;

      stack_param_size+=4;// add the size of the variable ptr (4 bytes)
      s1=loc_text(rg2);
      fprintf(pf, "i push,%s\n",s1.c_str());
    }
  }
  else
  {
    stack_param_size=pproc->param_size+4;
    stack_ret_size=(pproc->function)?pproc->ret_type->size:0;
  }

  // push the stack size, return size and function index...
  fprintf(pf,"i push,%i\n",stack_param_size);
  fprintf(pf,"i push,%i\n",stack_ret_size);
  fprintf(pf,"i push,%i\n",pproc->proc_number);
  if(pproc->safecall)
    fprintf(pf,"i calln,$_safecall_thunk\n");
  else
    fprintf(pf,"i calln,$_interface_thunk\n");
  
  if(scount>0)
  {
    for(j=0;j<scount;j++)
    {
      fprintf(pf,"i push,[ir%03i+%i]\n",rs,j*4);
      fprintf(pf,"i calln,$_free_widestring\n");
    }
    // free the strings
    free_register(rs);
  }

  // ***** call any exceptions here *****
  if(pproc->safecall)
    fprintf(pf,"i sub,ir001,4\n");// remove the HRESULT from the stack

  pexpr->reg_loc = tmp_reg;
}

void CmoduleCompiler::emit_array_expression(reg_location *ploc, syntaxNode *pexpr)
{
  size_t i;
  int opcount1,opcount2;
  size_t j;
  int mult,const_val,curr_val;;
  syntaxNode *plist;
  array_type *parr_type;
  array_bound *pbound,*pnbound;
  int rgs[2];
  size_t rcount=0;
  std::string s;

  plist=pexpr->children[1]; 

  i = plist->count - pexpr->cindex;

  switch(i)
  {
    case 0:
      opcount1=1;
    break;
    case 1:
      opcount1=3;
    break;
    default:
      opcount1=3+(i-2)*3;
  }
  
  opcount2=(plist->count-1)*2;

  parr_type=(array_type*)get_expr_type(pexpr->children[0]);

  if(opcount2<opcount1)
  {
    // arr[D1,D2,D3..,E(N-1),DN]
    // exp[E1,E2,E3..,E(N-1),EN]
    // INDX = (((E1*D2)+E2)*D3+E3)*...)*D(N-1) + EN
    const_val=0;

    for(j=0;j<plist->count;j++)
    {
      pbound=parr_type->dimentions[j];

      if(const_expr(plist->children[j]))
      {
        switch(pbound->bound_type->tycode)
        {
          case ty_char:
            curr_val=plist->children[j]->node_name[0];
          break;
          case ty_integer:
            curr_val=expr_val_int(0,plist->children[j]);
          break;
          case ty_boolean:
            curr_val=(plist->children[j]->node_name[0]=='t')?1:0;
          break;
        }
        
        curr_val-= pbound->low;
     
        if(j==plist->count-1)
        {
          const_val+=curr_val;
          continue;
        }

        pnbound = parr_type->dimentions[j+1];
        if(rcount==0)
        {
          rgs[rcount++]=alloc_register();
          fprintf(pf,"i mov,ir%03i,%i\n",rgs[0],curr_val*(1+pnbound->high-pnbound->low));
          continue;
        }
          
        fprintf(pf,"i add,ir%03i,%i\n",rgs[0],curr_val);
        fprintf(pf,"i mult,ir%03i,%i\n",rgs[0],(1+pnbound->high-pnbound->low));
      }
      else
      {
        emit_expression(plist->children[j]);
        s=loc_text(plist->children[j]->reg_loc);

        pnbound = (j!=plist->count-1)?parr_type->dimentions[j+1]:0;

        if(rcount==0)
        {
          rgs[rcount++]=alloc_register();
          fprintf(pf,"i mov,ir%03i,%s\n",rgs[0],s.c_str());
          if(pbound->low)
            fprintf(pf,"i sub,ir%03i,%i\n",rgs[0],pbound->low);
          if(pnbound)
            fprintf(pf,"i mult,ir%03i,%i\n",rgs[0],1+pnbound->high - pnbound->low);
          continue;
        }
        fprintf(pf,"i add,ir%03i,%s\n",rgs[0],s.c_str());
        if(pbound->low)
          fprintf(pf,"i sub,ir%03i,%i\n",rgs[0],pbound->low);
        if(pnbound)
          fprintf(pf,"i mult,ir%03i,%i\n",rgs[0],1+pnbound->high - pnbound->low);
      }
    }
  }
  else
  {
    // arr[D1,D2,D3..,DN-1,DN]
    // exp[E1,E2,E3..,EN-1,EN]
    // INDX = E1*D2*D3*..*DN+E2*D3*..*D(N-1)*DN+..+EN
    mult = 1;
    const_val = 0;

    for(j=plist->count;j>0;j--)
    {
      pbound=parr_type->dimentions[j-1];

      if(const_expr(plist->children[j-1]))
      {
        switch(pbound->bound_type->tycode)
        {
          case ty_char:
            curr_val=plist->children[j-1]->node_name[0];
          break;
          case ty_integer:
            curr_val=expr_val_int(0,plist->children[j-1]);
          break;
          case ty_boolean:
            curr_val=(plist->children[j-1]->node_name[0]=='t')?1:0;
          break;
        }
        
        curr_val-= pbound->low;
        const_val+=curr_val*mult;
      }
      else
      {
        if(rcount<2)
          rgs[rcount++]=alloc_register();
        emit_expression(plist->children[j-1]);
        s = loc_text(plist->children[j-1]->reg_loc);
        fprintf(pf,"i mov,ir%03i,%s\n",rgs[rcount-1],s.c_str());
        if(pbound->low!=0)
          fprintf(pf,"i sub,ir%03i,%i\n",rgs[rcount-1],pbound->low);
        fprintf(pf,"i mult,ir%03i,%i\n",rgs[rcount-1],mult);
        if(rcount>1)
          fprintf(pf,"i add,ir%03i,ir%03i\n",rgs[0],rgs[1]);
        free_location(plist->children[j-1]);
      }

      mult*=(1+pbound->high-pbound->low);
    }
  }

  if(i==0)
  {
    const_val *= parr_type->base_type->size;
    pexpr->reg_loc = *ploc;
    pexpr->reg_loc.offset += const_val;
    pexpr->reg_loc.deref = 1;
    pexpr->reg_loc.size = parr_type->base_type->size;
  }
  else
  {
    reg_location rg = *ploc;
    rg.deref = 0;
    s = loc_text(rg);
    if(const_val>0)
      fprintf(pf,"i add,ir%03i,%i\n",rgs[0],const_val);
    fprintf(pf,"i mult,ir%03i,%i\n",rgs[0],parr_type->base_type->size);
    fprintf(pf,"i add,ir%03i,%s\n",rgs[0],s.c_str());
    
    free_register(ploc->reg);
    pexpr->reg_loc = *ploc;
    pexpr->reg_loc.size = parr_type->base_type->size;
    pexpr->reg_loc.const_ = 0;
    pexpr->reg_loc.deref = 1;
    pexpr->reg_loc.free_bstr = 0;
    pexpr->reg_loc.free_string = 0;
    pexpr->reg_loc.free_interface = 0;
    pexpr->reg_loc.free_object = 0;
    pexpr->reg_loc.offset = 0;
    pexpr->reg_loc.reg = rgs[0];
  }

  for(j=1;j<rcount;j++)
    free_register(rgs[j]);
}

void CmoduleCompiler::emit_dispatch_expression(reg_location reg_l, size_t i, sym_type *ptype, syntaxNode *pexpr)
{
  syntaxNode *pnode;
  syntaxNode *pparameters;
  std::string s;
  int k;
  int pcount;
  int init_index = i;
  
  for(;i<pexpr->count-1;i++)
  {
    fprintf(pf,"i add,ir001,%i\n",VARIANT_TYPE->size);

    if(reg_l.reg == 1)
      reg_l.offset-=VARIANT_TYPE->size;
    
    // push the object onto the stack...
    if(ptype!= DISPATCH_TYPE)
    {
      // convert the object to idispatch...
      reg_l.deref = 0;
      if(reg_l.reg == 1)
        reg_l.offset -= sizeof(void*);

      fprintf(pf,"i add,ir001,4\n");

      s = loc_text(reg_l);

      fprintf(pf,"i push,%s\n",s.c_str());
      fprintf(pf,"i calln,$_vartodispatch\n");
    }
    else
    {
      s = loc_text(reg_l);
      fprintf(pf,"i push,%s\n", s.c_str());
    }

    pnode = pexpr->children[i+1];
    
    switch(pnode->node_type)
    {
      case TOK_IDENTIFIER:
        pcount=0;
        fprintf(pf,"i push,^%i\n", pexpr->children[i+1]->cindex+1);
      break;
      case e_proc_func_expression:
        fprintf(pf,"i push,^%i\n", pnode->children[0]->cindex+1);
       
        pparameters=pnode->get_node(e_expression_list);

        if(pparameters)
        {
          pcount=pparameters->count;
          for(k=pcount;k>0;k--)
            emit_parameter(false,VARIANT_TYPE,pparameters->children[k-1]);
        }
        else
          pcount = 0;
      break;
      case e_array_elt_expression:
        fprintf(pf,"i push,^%i\n", pnode->children[0]->cindex+1);
       
        pparameters=pnode->get_node(e_expression_list);

        if(pparameters)
        {
          pcount=pparameters->count;
          for(k=pcount;k>0;k--)
            emit_parameter(false,VARIANT_TYPE,pparameters->children[k-1]);
        }
        else
          pcount = 0;
      break;
    }

    fprintf(pf,"i push,%i\n",pcount);
    fprintf(pf,"i push,%i\n",pnode->flags);

    fprintf(pf,"i calln,$_dispcall\n");

    ptype = VARIANT_TYPE;
    
    reg_l.reg = 1;
    reg_l.offset = -1 * ptype->size;
  }

  //clean up hte objects
  if((pexpr->count-init_index)>2)
  {
    int r1=alloc_register();
    int r2=alloc_register();
    int l=++label_count;

    pcount=pexpr->count-init_index-1;

    // invoke clear on all the objects
    fprintf(pf,"i mov,ir%03i,ir001-%i\n",r1,VARIANT_TYPE->size * pcount);
    fprintf(pf,"i mov,ir%03i,ir001-%i\n",r2,VARIANT_TYPE->size);
    fprintf(pf,"l l%i\n",l);
    fprintf(pf,"i sub,ir%03i,%i\n",r2,VARIANT_TYPE->size);
    fprintf(pf,"i push,ir%03i\n",r2);
    fprintf(pf,"i calln,$_varclear\n");
    fprintf(pf,"i eq ir%03i,ir%03i\n",r1,r2);
    fprintf(pf,"i jmp,l%i\n",l);
    // copy the final result up the stack...
    fprintf(pf,"i push,%i\n",VARIANT_TYPE->size);
    fprintf(pf,"i push,ir001-%i\n",VARIANT_TYPE->size+4);
    fprintf(pf,"i push,ir%03i\n",r1);
    fprintf(pf,"i calln,$_mcpy\n");
    // clean up the stack...
    fprintf(pf,"i sub,ir001,%i\n",(pcount-1) * VARIANT_TYPE->size);

    free_register(r1);
    free_register(r2);
  }
}

void CmoduleCompiler::emit_record_expression(syntaxNode *pexpr,bool save)
{
  size_t i=0;
  sym_type *ptype;
  syntaxNode *p1=0,*p2=0;
  std::string s1,s2;
  int r;
  int sign;
  int total_stack;
  reg_location reg_l1,reg_l2;
  
  // account for static variables and procedures. 
  if(pexpr->children[0]->variable->scode == sy_type)
    i++;

  emit_expression(pexpr->children[i]);

  reg_l1=pexpr->children[i]->reg_loc;
  total_stack=reg_l1.stack;
  reg_l1.stack = 0;

  for(;i<pexpr->count-1;i++)
  {
    p1 = pexpr->children[i];
    p2 = pexpr->children[i+1];

    ptype=get_expr_type(p1);

    switch(ptype->tycode)
    {
      case ty_record:
        // records don't have containing methods...
        if(p2->node_type==e_array_elt_expression)
        {
          reg_l1.offset += p2->children[0]->variable->offset;
          emit_array_expression(&reg_l1,p2);
          
          reg_l1 = p2->reg_loc;
          
          total_stack += reg_l1.stack;
          reg_l1.stack=0;
        }
        else
        {
          reg_l1.offset += p2->variable->offset;
          reg_l1.size = p2->variable->type->size;
          reg_l1.deref = 1;
        }
      break;
      case ty_variant:
          emit_dispatch_expression(reg_l1,i,ptype,pexpr);

          ptype = VARIANT_TYPE;
          free_register(reg_l1.reg);

          memset(&reg_l1,0,sizeof(reg_l1));

          reg_l1.reg = 1;
          reg_l1.stack = ptype->size;
          reg_l1.offset = -1 * ptype->size;
          total_stack += ptype->size;
          
          if(save)
          {
            s1 = loc_text(reg_l1);
            reg_l1.reg = alloc_register();
            reg_l1.offset = 0;
            fprintf(pf,"i mov,ir%03i,%s\n",reg_l1.reg,s1.c_str());
          }
          
          goto expr_end;
      break;
      case ty_interface:
        // interfaces don't have containing variables...
        if(ptype==DISPATCH_TYPE && !p2->variable)
        {
          reg_l1.deref = 1;
          reg_l1.size = 4;
          
          emit_dispatch_expression(reg_l1,i,ptype,pexpr);

          ptype = VARIANT_TYPE;
          free_register(reg_l1.reg);

          memset(&reg_l1,0,sizeof(reg_l1));

          reg_l1.reg = 1;
          reg_l1.stack = ptype->size;
          reg_l1.offset = -1 * ptype->size;
          total_stack += ptype->size;
          
          if(save)
          {
            s1 = loc_text(reg_l1);
            reg_l1.reg = alloc_register();
            reg_l1.offset = 0;
            fprintf(pf,"i mov,ir%03i,%s\n",reg_l1.reg,s1.c_str());
          }
          
          goto expr_end;
        }
        else
        {
          reg_l1.deref = 1;
          reg_l1.size = 4;
          emit_interface_call(p2,p2,(interface_type*)ptype,reg_l1,true);
        
          reg_l1=p2->reg_loc;
        
          ptype=p2->procedure->ret_type;
          total_stack += reg_l1.stack;
          reg_l1.stack=0;
          reg_l1.size = (ptype)?ptype->size:0;
        }
      break;
      case ty_class:
        // classes have both methods and variables...
        switch(p2->variable->scode)
        {
          case sy_variable:
            sign = (p2->variable->param)?-1:1;
            s1=loc_text(reg_l1);
        
            free_location(p1);
            r=alloc_register();
        
            fprintf(pf,"i mov,ir%03i,%s\n",r,s1.c_str());
            
            p1->reg_loc=reg_l1;
            
            reg_l1.const_ = 0;
            reg_l1.offset = sign * p2->variable->offset;
            reg_l1.stack = 0;
            reg_l1.reg = r;
            reg_l1.deref = 1;
            reg_l1.size = p2->variable->type->size;
          break;
          case sy_procedure:
            emit_class_call(p2,p2,(class_type*)ptype,reg_l1,true);
            
            reg_l1=p2->reg_loc;
            
            total_stack += reg_l1.stack;
            ptype=p2->procedure->ret_type;
            reg_l1.stack=0;
            reg_l1.size = (ptype)?ptype->size:0;
          break;
        }
      break;
    }
  }

expr_end:

  if(p2)
  {
    ptype = get_expr_type(p2);

    if(ptype &&  ptype->size < (size_t)total_stack)
    {
      // move the result to the beginning of the 
      // stack.
      s1 = loc_text(reg_l1);
      
      memset(&reg_l2,0,sizeof(reg_l2));
      reg_l2.deref = 1;
      reg_l2.reg = 1;
      reg_l2.offset = -1 * total_stack;
         
      if(reg_l1.size>8)
      {
        reg_l2.deref = 0;
        reg_l2.offset -= 8;// the size and the destination are on the stack.
        s2 = loc_text(reg_l2);
    
        fprintf(pf,"i push,%i\n", reg_l1.size);
        fprintf(pf,"i push,%s\n", s1.c_str());
        fprintf(pf,"i push,%s\n", s2.c_str());
        fprintf(pf,"i calln,$_mcpy\n");
      }
      else
      {
        s2 = loc_text(reg_l2);
        fprintf(pf,"i mov,%s,%s\n",s2.c_str(),s1.c_str());
      }
      
      if(reg_l1.reg != 1)
        fprintf(pf,"i mov,ir%03i,ir001-%i\n",reg_l1.reg,total_stack);
      fprintf(pf,"i sub,ir001,%i\n",total_stack-ptype->size);
      reg_l1.offset = 0;
      reg_l1.stack = ptype->size;    
    }
    else
      reg_l1.stack = total_stack;
  }
  else
    reg_l1.stack = total_stack;

  pexpr->reg_loc = reg_l1;
}

void CmoduleCompiler::emit_logical_or(syntaxNode *pexpr)
{
  int r = alloc_register();
  int l = ++label_count;
  size_t i;

  pexpr->reg_loc.const_ = 0;
  pexpr->reg_loc.offset = 0;
  pexpr->reg_loc.stack  = 0;
  pexpr->reg_loc.reg = r;
  pexpr->reg_loc.size = 1;
  pexpr->reg_loc.deref = 0;
  pexpr->reg_loc.free_string = 0;
  pexpr->reg_loc.free_bstr = 0;
  pexpr->reg_loc.free_interface = 0;
  pexpr->reg_loc.free_object = 0;

  syntaxNode *pexpr1;
  std::string s1;
  int r2;
  sym_type *ptype;
  
  fprintf(pf,"i mov,cr%03i,1\n",r);
  
  for(i=0;i<pexpr->count;i++)
  {
    pexpr1=pexpr->children[i];
    emit_expression(pexpr1);
    
    ptype = get_expr_type(pexpr1);

    if(ptype->tycode == ty_variant)
    {
      pexpr1->reg_loc.deref = 0;
      s1 = loc_text(pexpr1->reg_loc);
      r2 = alloc_register();

      fprintf(pf,"i add,ir001,4\n");
      fprintf(pf,"i push,%s\n",s1.c_str());
      fprintf(pf,"i calln,$_vartoint\n");
      fprintf(pf,"i sub,ir001,4\n");
      fprintf(pf,"i mov,ir%03i,[ir001]\n",r2);
      free_location(pexpr1);
      memset(&pexpr1->reg_loc,0,sizeof(pexpr1->reg_loc));
      pexpr1->reg_loc.size = 4;
      pexpr1->reg_loc.reg = r2;
      s1 = loc_text(pexpr1->reg_loc);
      free_register(r2);
    }
    else
    {
      s1 = loc_text(pexpr1->reg_loc); 
      free_location(pexpr1);
    }

    fprintf(pf,"i eq,%s,0\n", s1.c_str());
    fprintf(pf,"i jmp,l%i\n",l);
  }

  fprintf(pf,"i mov,cr%03i,0\n",r);
  fprintf(pf,"l l%i\n",l);
}

void CmoduleCompiler::emit_logical_and(syntaxNode *pexpr)
{
  int r = alloc_register();
  int l = ++label_count;
  size_t i;
  char *oper;

  pexpr->reg_loc.const_ = 0;
  pexpr->reg_loc.offset = 0;
  pexpr->reg_loc.stack  = 0;
  pexpr->reg_loc.reg = r;
  pexpr->reg_loc.size = 1;
  pexpr->reg_loc.deref = 0;
  pexpr->reg_loc.free_string = 0;
  pexpr->reg_loc.free_bstr = 0;
  pexpr->reg_loc.free_interface = 0;
  pexpr->reg_loc.free_object = 0;

  syntaxNode *pexpr1;
  std::string s1;
  int r2;
  sym_type *ptype;
  
  fprintf(pf,"i mov,cr%03i,0\n",r);
  
  for(i=0;i<pexpr->count;i++)
  {
    oper="ne";

    pexpr1=pexpr->children[i];
    
    if(pexpr1->node_type==TOK_NOT)
    {
      i++;
      pexpr1=pexpr->children[i];
      oper="eq";
    }
    
    emit_expression(pexpr1);
    ptype = get_expr_type(pexpr1);

    if(ptype->tycode == ty_variant)
    {
      pexpr1->reg_loc.deref = 0;
      s1 = loc_text(pexpr1->reg_loc);
      r2 = alloc_register();

      fprintf(pf,"i add,ir001,4\n");
      fprintf(pf,"i push,%s\n",s1.c_str());
      fprintf(pf,"i calln,$_vartoint\n");
      fprintf(pf,"i sub,ir001,4\n");
      fprintf(pf,"i mov,ir%03i,[ir001]\n",r2);
      free_location(pexpr1);
      memset(&pexpr1->reg_loc,0,sizeof(pexpr1->reg_loc));
      pexpr1->reg_loc.size = 4;
      pexpr1->reg_loc.reg = r2;
      s1 = loc_text(pexpr1->reg_loc);
      free_register(r2);
    }
    else
    {
      s1 = loc_text(pexpr1->reg_loc); 
      free_location(pexpr1);
    }

    fprintf(pf,"i %s,%s,0\n", oper, s1.c_str());
    fprintf(pf,"i jmp,l%i\n",l);
  }

  fprintf(pf,"i mov,cr%03i,1\n",r);
  fprintf(pf,"l l%i\n",l);
}

void CmoduleCompiler::emit_relational(syntaxNode *pexpr)
{
  int r = alloc_register();
  pexpr->reg_loc.const_ = 0;
  pexpr->reg_loc.offset = 0;
  pexpr->reg_loc.stack  = 0;
  pexpr->reg_loc.reg = r;
  pexpr->reg_loc.size = 1;
  pexpr->reg_loc.deref = 0;
  pexpr->reg_loc.free_string = 0;
  pexpr->reg_loc.free_bstr = 0;
  pexpr->reg_loc.free_interface = 0;
  pexpr->reg_loc.free_object = 0;

  syntaxNode *pexpr1,*pexpr2;
  std::string s1,s2;
  sym_type *ptype1,*ptype2;
  char c;
  int r2;
  
  pexpr1=pexpr->children[0];
  pexpr2=pexpr->children[2];

  ptype1 = get_expr_type(pexpr1);
  ptype2 = get_expr_type(pexpr2);

  if(ptype1->tycode == ty_variant || ptype2->tycode == ty_variant)
  {
    if(ptype1->tycode == ptype2->tycode)
    {
      emit_expression(pexpr1);
      emit_expression(pexpr2);
    }
    else
    {
      if(ptype1->tycode==ty_variant)
      {
        // switch the expressions
        ptype1=ptype2;
        ptype2=get_expr_type(pexpr1);
        pexpr1=pexpr->children[2];
        pexpr2=pexpr->children[0];
      }

      emit_expression(pexpr1);
      emit_expression(pexpr2);

      pexpr2->reg_loc.deref = 0;
      s2 = loc_text(pexpr2->reg_loc);

      fprintf(pf,"i add,ir001,%i\n",ptype1->size);
      fprintf(pf,"i push,%s\n",s2.c_str());

      // convert the second expression to a nonvariant type...
      switch(ptype1->tycode)
      {
        case ty_char:
          c = 'c';
          fprintf(pf,"i calln,$_vartochar\n");
        break;
        case ty_integer:
          switch(ptype1->size)
          {
            case 1:
              c='c';
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case 2:
              c='s';
              fprintf(pf,"i calln,$_vartoshort\n");
            break;
            case 4:
              c='i';
              fprintf(pf,"i calln,$_vartoint\n");
            break;
          }
        break;
        case ty_real:
          switch(ptype1->size)
          {
            case 4:
              c='f';
              fprintf(pf,"i calln,$_vartofloat\n");
            break;
            case 8:
              c='d';
              fprintf(pf,"i calln,$_vartodouble\n");
            break;
          }
        break;
        case ty_boolean:
          c = 'c';
          fprintf(pf,"i calln,$_vartobool\n");
        break;
        case ty_string:
            c = 'i';
          fprintf(pf,"i calln,$_vartostring\n");
        break;
        case ty_widestring:
          c = 'i';
          fprintf(pf,"i calln,$_vartowidestr\n");
        break;
        case ty_interface:
          c = 'i';
          fprintf(pf,"i calln,$_vartodispatch\n");
        break;
      }

      r2 = alloc_register();

      fprintf(pf,"i sub,ir001,%i\n",ptype1->size);
      fprintf(pf,"i mov,%cr%03i,[%cr001]\n",c,r2,c);
      free_location(pexpr2);

      memset(&pexpr2->reg_loc,0,sizeof(pexpr2->reg_loc));

      pexpr2->reg_loc.reg = r2;
      pexpr2->reg_loc.size = ptype1->size;

      pexpr1=pexpr->children[0];
      pexpr2=pexpr->children[2];
    }
  }
  else
  {
    emit_expression(pexpr1);
    emit_expression(pexpr2);
  }

  switch(ptype1->tycode)
  {
    case ty_variant:
			throw CCompilerErr(pexpr1->getNodeLine(),"Not implemented");
    break;
    case ty_string:
			throw CCompilerErr(pexpr1->getNodeLine(),"Not implemented");
    break;
    case ty_widestring:
			throw CCompilerErr(pexpr1->getNodeLine(),"Not implemented");
    break;
    default:
      s1=loc_text(pexpr1->reg_loc);
      s2=loc_text(pexpr2->reg_loc);
  }

  fprintf(pf,"i mov,cr%03i,1\n",r);

  switch(pexpr->children[1]->node_type)
  {
    case TOK_LT:
      fprintf(pf,"i lt,%s,%s\n",s1.c_str(),s2.c_str());
    break;
    case TOK_GT:
      fprintf(pf,"i gt,%s,%s\n",s1.c_str(),s2.c_str());
    break;
    case TOK_LE:
      fprintf(pf,"i le,%s,%s\n",s1.c_str(),s2.c_str());
    break;
    case TOK_GE:
      fprintf(pf,"i ge,%s,%s\n",s1.c_str(),s2.c_str());
    break;
  }

  fprintf(pf,"i mov,cr%03i,0\n",r);

  free_location(pexpr1);
  free_location(pexpr2);
}

void CmoduleCompiler::emit_equality(syntaxNode *pexpr)
{
  int r1,r2;
  std::string s1,s2;
  syntaxNode *pop;
  syntaxNode *exp1,*exp2;
  sym_type *ptype1,*ptype2;
  size_t i;
  char c;

  exp1=pexpr->children[0];
  exp2=pexpr->children[2];
  pop=pexpr->children[1];

  switch(pop->node_type)
  {
    case TOK_EQ:
    case TOK_NE:
      r1 = alloc_register();

      pexpr->reg_loc.const_ = 0;
      pexpr->reg_loc.offset = 0;
      pexpr->reg_loc.stack  = 0;
      pexpr->reg_loc.reg = r1;
      pexpr->reg_loc.size = 1;
      pexpr->reg_loc.deref = 0;
      pexpr->reg_loc.free_string = 0;
      pexpr->reg_loc.free_bstr = 0;
      pexpr->reg_loc.free_interface = 0;
      pexpr->reg_loc.free_object = 0;
  
      ptype1 = get_expr_type(exp1);
      ptype2 = get_expr_type(exp2);

      if(ptype1->tycode == ty_variant || ptype2->tycode == ty_variant)
      {
        if(ptype1->tycode == ptype2->tycode)
        {
          emit_expression(exp1);
          emit_expression(exp2);
        }
        else
        {
          if(ptype1->tycode==ty_variant)
          {
            // switch the expressions
            ptype1=ptype2;
            ptype2=get_expr_type(exp1);
            exp1=pexpr->children[2];
            exp2=pexpr->children[0];
          }

          emit_expression(exp1);
          emit_expression(exp2);

          exp2->reg_loc.deref = 0;
          s2 = loc_text(exp2->reg_loc);

          fprintf(pf,"i add,ir001,%i\n",ptype1->size);
          fprintf(pf,"i push,%s\n",s2.c_str());

          // convert the second expression to a nonvariant type...
          switch(ptype1->tycode)
          {
            case ty_char:
              c = 'c';
              fprintf(pf,"i calln,$_vartochar\n");
            break;
            case ty_integer:
              switch(ptype1->size)
              {
                case 1:
                  c='c';
                  fprintf(pf,"i calln,$_vartochar\n");
                break;
                case 2:
                  c='s';
                  fprintf(pf,"i calln,$_vartoshort\n");
                break;
                case 4:
                  c='i';
                  fprintf(pf,"i calln,$_vartoint\n");
                break;
              }
            break;
            case ty_real:
              switch(ptype1->size)
              {
                case 4:
                  c='f';
                  fprintf(pf,"i calln,$_vartofloat\n");
                break;
                case 8:
                  c='d';
                  fprintf(pf,"i calln,$_vartodouble\n");
                break;
              }
            break;
            case ty_boolean:
              c = 'c';
              fprintf(pf,"i calln,$_vartobool\n");
            break;
            case ty_string:
              c = 'i';
              fprintf(pf,"i calln,$_vartostring\n");
            break;
            case ty_widestring:
              c = 'i';
              fprintf(pf,"i calln,$_vartowidestr\n");
            break;
            case ty_interface:
              c = 'i';
              fprintf(pf,"i calln,$_vartodispatch\n");
            break;
          }

          r2 = alloc_register();

          fprintf(pf,"i sub,ir001,%i\n",ptype1->size);
          fprintf(pf,"i mov,%cr%03i,[%cr001]\n",c,r2,c);
          free_location(exp2);

          memset(&exp2->reg_loc,0,sizeof(exp2->reg_loc));

          exp2->reg_loc.reg = r2;
          exp2->reg_loc.size = ptype1->size;
        }
      }
      else
      {
        emit_expression(exp1);
        emit_expression(exp2);
      }

      switch(ptype1->tycode)
      {
        case ty_variant:
					throw CCompilerErr(exp1->getNodeLine(),"Not implemented");
        break;
        case ty_string:
					throw CCompilerErr(exp1->getNodeLine(),"Not implemented");
        break;
        case ty_widestring:
					throw CCompilerErr(exp1->getNodeLine(),"Not implemented");
        break;
        default:
          s1=loc_text(exp1->reg_loc);
          s2=loc_text(exp2->reg_loc);
      }

      fprintf(pf,"i mov,cr%03i,0\n",r1);
      fprintf(pf,"i %s,%s,%s\n", (pop->node_type==TOK_EQ)?"ne":"eq",s1.c_str(),s2.c_str());
      fprintf(pf,"i mov,cr%03i,1\n",r1);

      free_location(exp1);
      free_location(exp2);

      if(pexpr->count>3)
      {
        r2 = alloc_register();
        for(i=3;i<pexpr->count;i+=2)
        {
          pop=pexpr->children[i];
          exp1=pexpr->children[i+1];

          emit_expression(exp1);
          s1=loc_text(exp1->reg_loc);

          fprintf(pf,"i mov,cr%03i,0\n",r2);
          fprintf(pf,"i %s,%s,cr%03i\n",(pop->node_type==TOK_EQ)?"ne":"eq",s1.c_str(),r1);
          fprintf(pf,"i mov,cr%03i,1\n",r2);
          fprintf(pf,"i mov,cr%03i,cr%03i\n",r1,r2);        
          free_location(exp1);
        }
        free_register(r2);
      }
    break;
  }  
}

void CmoduleCompiler::emit_as_expression(syntaxNode *pexpr)
{
  sym_type *ptype;
  interface_type *pint_type;
  reg_location reg;
  int r;
  std::string s1;
  Cmodule *pUnit;

  ptype = get_expr_type(pexpr);

  switch(ptype->tycode)
  {
    case ty_interface:
      pint_type=(interface_type*)ptype;
      pUnit = _m_punit->get_manager()->get_unit(ptype->unit_index);
       
      r = alloc_register();

      fprintf(pf,"i mov,ir%03i,ir001\n",r);
      fprintf(pf,"i add,ir001,4\n");

      emit_expression(pexpr->children[0]);

      reg.const_=0;
      reg.deref = 0;
      reg.reg = alloc_register();
      reg.offset = pint_type->guid_index + pUnit->global.size + 4 * (pUnit->get_procedure_count() + 2);

      pexpr->reg_loc.free_interface = 1;
      pexpr->reg_loc.reg = r;
      pexpr->reg_loc.size = 4;
      pexpr->reg_loc.stack = 4;
      
      s1 = loc_text(pexpr->children[0]->reg_loc);
  
      fprintf(pf,"i add,ir001,4\n");
      fprintf(pf,"i push,%s\n", s1.c_str());
      fprintf(pf,"i mov_ds,ir%03i,%i\n",reg.reg,pUnit->get_index());
      
      s1 = loc_text(reg);
      fprintf(pf,"i push,%s\n", s1.c_str());

      fprintf(pf,"i push,ir%03i\n",r );

      fprintf(pf,"i push,12\n");
      fprintf(pf,"i push,4\n");
      fprintf(pf,"i push,0\n");
      fprintf(pf,"i calln,$_interface_thunk\n");
      fprintf(pf,"i sub,ir001,4\n");

      pexpr->reg_loc.deref = 1;
      free_register(reg.reg);
      free_location(pexpr->children[0]);
    break;
  }
}

//****************************************************************
// High Level Constructs.
//****************************************************************
bool CmoduleCompiler::emit_module(syntaxNode *p)
{
  pf = fopen(_m_punit->get_asmname(),"wt");
  
  _m_punit->output_constants(pf);
  // all code is found in the implementation section...
  syntaxNode *pdecls = p->children[2]->get_node(e_implementation_declarations);

  emit_definitions(pdecls);

  fclose(pf);

  return true;
}            
bool CmoduleCompiler::emit_program(syntaxNode *p)
{
  syntaxNode *pprog_block=p->get_node(e_program_block);
  syntaxNode *pblock=pprog_block->get_node(e_block);
  
  pf = fopen(_m_punit->get_asmname(),"wt");
  
  _m_punit->output_constants(pf);

  emit_definitions(pblock->get_node(e_implementation_declarations));

  label_count = 0;

  // begin the program...
  fprintf(pf,"$@__main\n");
  emit_compound_statement(pblock->get_node(e_statement_list));

  // free the globals...
  free_variables(&_m_punit->global);
  fprintf(pf,"i ret,0\n");
  
  fclose(pf);

  return true;
}           
bool CmoduleCompiler::emit_script(syntaxNode *p)
{
  //syntaxNode *pscript_uses;
  syntaxNode *pscript_block;

  //pscript_uses = p->get_node(e_uses_clause);
  pscript_block = p->children[0];

  pf = fopen(_m_punit->get_asmname(),"wt");
  
  _m_punit->output_constants(pf);

  emit_definitions(p);

  label_count = 0;
  
  fprintf(pf,"$@__main\n");

  if(pscript_block)
  {
    emit_compound_statement(pscript_block);
    free_variables(&_m_punit->global);
  }

  fprintf(pf,"i ret,0\n");

  fclose(pf);

  return true;
}
bool CmoduleCompiler::compileModule(Cmodule *punit)
{
  syntaxNode *p=punit->get_syntax_tree()->getRoot();
  _m_punit = punit;

	Cmodule *pSystem;
	
	if(punit->get_index()==0)
	{
		pSystem=punit;
	} else {
		pSystem=punit->get_manager()->get_unit("system");
	}

	if(pSystem)
	{
		// get the system unit and initialize the native types..
		INTEGER_TYPE=(sym_type*)pSystem->global.get_member("integer");
		REAL_TYPE=(sym_type*)pSystem->global.get_member("extended");
		BOOLEAN_TYPE=(sym_type*)pSystem->global.get_member("bool");
		STRING_TYPE=(sym_type*)pSystem->global.get_member("string");
		WIDESTRING_TYPE=(sym_type*)pSystem->global.get_member("widestring");
		VOID_TYPE=(sym_type*)pSystem->global.get_member("pointer");
		INTERFACE_TYPE=(sym_type*)pSystem->global.get_member("interface");
		CHAR_TYPE=(sym_type*)pSystem->global.get_member("char");
		VARIANT_TYPE=(sym_type*)pSystem->global.get_member("olevariant");
		DISPATCH_TYPE=(sym_type*)pSystem->global.get_member("idispatch");
	}

	for(int i=0;i<REG_COUNT;i++)
    reg_array[i] = -1;
  
  max_reg = 2;

  if(p->count>0)
  {
    switch(p->children[0]->node_type)
    {
      case e_unit:
        return emit_module(p->children[0]);
      break;
      case e_program:
        return emit_program(p->children[0]);
      break;
      case e_script:
        return emit_script(p->children[0]);
      break;
    }
  }

  return false;
}

