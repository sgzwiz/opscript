/* 
 * syntaxValidator.h - Declaration of the grammar syntax validator. This object 
 * will traverse the syntax tree and create symbol objects. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __syntaxValidator_h__
#define __syntaxValidator_h__

#include "moduleObjects.h"

class CsyntaxValidator
{
private:
  Cmodule *_m_punit;
  std::stack<int> _m_loopContext;
  
	sym_type *INTEGER_TYPE;
  sym_type *REAL_TYPE;
  sym_type *BOOLEAN_TYPE;
  sym_type *STRING_TYPE;
  sym_type *WIDESTRING_TYPE;
  sym_type *VOID_TYPE;
  sym_type *INTERFACE_TYPE;
  sym_type *CHAR_TYPE;
  sym_type *VARIANT_TYPE;
  sym_type *DISPATCH_TYPE;

protected:
  char *duplicateString(const char *dstr);
  char *get_unknown_name(sym_type *p);
  sym_procedure *get_overloaded_declaration( sym_procedure *pproc, syntaxNode *pparams );
  bool is_equal_type(sym_type *t1, sym_type *t2,bool strict=false);
  std::string proc_name(Cmodule *pUnit, sym_procedure *pproc);
  std::string get_signature(sym_procedure *pproc);
  bool check_guid_syntax(char *p);
  std::string get_guid_memory(char *p);

  
  void exp_mult(symbol_context *ns, syntaxNode *expr1, syntaxNode *expr2, int op);
  void exp_add(symbol_context *ns, syntaxNode *expr1, syntaxNode *expr2, int op);
  
  #ifdef _WIN32
  void reduce_dispatch_write(syntaxNode *p);
  void reduce_dispatch(size_t i, symbol_context *ps, syntaxNode *p);
  #endif
  void reduce_as_expression(symbol_context *ns, syntaxNode *p);
  void reduce_simple(symbol_context *ns, syntaxNode *p, bool is_record);
  void reduce_method_expression(symbol_context *ns, syntaxNode *p, bool is_record);
  void reduce_expression( symbol_context *ns, syntaxNode *p, bool is_record );
  void reduce_record_expr(symbol_context *ns, syntaxNode *p, bool is_record);
  void reduce_array_elt_expr(symbol_context *ns, syntaxNode *p, bool is_record);
  void reduce_relational(symbol_context *ns, syntaxNode *p);
  void reduce_multiplicative(symbol_context *ns, syntaxNode *p);
  void reduce_additive(symbol_context *ns, syntaxNode *p);
  void reduce_logical_or(symbol_context *ns, syntaxNode *p);
  void reduce_logical_and(symbol_context *ns, syntaxNode *p);
  void reduce_equality(symbol_context *ns, syntaxNode *p);
  void reduce_properties_read(syntaxNode *ps);
  void reduce_properties_write(syntaxNode *ps);
  void reduce_params(symbol_context *ns,syntaxNode *pparams);
  
  void add_property(symbol_context *ns, sym_namespace *ps, syntaxNode *pproperty, BYTE visibility);
  void add_variable(symbol_context *ns, sym_namespace *ps, syntaxNode *pname_list, syntaxNode *ptype, BYTE visibility);
  size_t add_params(symbol_context *ns, sym_procedure *proc_info, syntaxNode *p);
  
  void add_definitions(symbol_context *ns, sym_namespace *ps, syntaxNode *p);

  void add_function_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility);
  void add_constructor_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility);
  void add_procedure_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p, BYTE visibility);

  void add_procedure_definition( symbol_context *ns, sym_namespace *ps, syntaxNode *p);
  void add_function_definition(symbol_context *ns, sym_namespace *ps, syntaxNode *p);
  void add_constructor_definition(symbol_context *ns, sym_namespace *ps, syntaxNode *p);
  
  void add_var_declarations(symbol_context *ns, sym_namespace *ps, syntaxNode *p);
  void add_var_declaration(symbol_context *ns, sym_namespace *ps, syntaxNode *p);

	void add_const_definitions(symbol_context *ns, sym_namespace *ps,syntaxNode *p);
  void add_const_definition(symbol_context *ns, sym_namespace *ps,syntaxNode *p);
  
	void add_type_declarations(symbol_context *ns, sym_namespace *ps,syntaxNode *p);
  void add_type_declaration(symbol_context *ns, sym_namespace *ps,syntaxNode *p);
  
  void process_statement(symbol_context *pns, syntaxNode *p);
  void process_statements(symbol_context *pns, syntaxNode *p);
  void process_if(symbol_context *pns, syntaxNode *p);
  void process_while(symbol_context *pns, syntaxNode *p);
  void process_repeat(symbol_context *pns, syntaxNode *p);
  void process_for(symbol_context *pns, syntaxNode *p);
  void process_assign(symbol_context *pns, syntaxNode *p);

  void build_array(symbol_context *pns,sym_namespace *ps, array_type *arr, syntaxNode *bound_def, syntaxNode *type );
  void build_class(symbol_context *pns,sym_namespace *ps, class_type *cls, syntaxNode *p);
  void build_record(symbol_context *pns,record_type *rec, syntaxNode *p);
  void build_interface(symbol_context *pns,sym_namespace *ps, interface_type *cls, syntaxNode *p);

  sym_type *get_type(symbol_context *pns, syntaxNode *ptype_info, sym_namespace *ps, syntaxNode *ptype_name=0);
  sym_procedure *get_matching_overload( int line, sym_procedure *pproc, syntaxNode *pparams );

  void process_uses_clause(syntaxNode *p);

  void validate_unit(syntaxNode *p);
  void validate_program(syntaxNode *p);
  void validate_script(syntaxNode *p);
public:  
  bool validateTree(Cmodule *unit);
};

void emit_xml_code(syntaxNode *psyntax, int use_xsl,int use_indents);




#endif