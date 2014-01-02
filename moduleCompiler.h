/* 
 * moduleCompiler.h 
 * 
 * Declaration of the moduleCompiler object. It takes a validated module and 
 * creates byte code, which can then be executed by the interpreter.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __moduleCompiler_h__
#define __moduleCompiler_h__

#define REG_COUNT 128

class CmoduleCompiler
{
private:
  FILE *pf;
  int reg_array[REG_COUNT];
  int max_reg;
  int label_count;
  Cmodule *_m_punit;
  std::stack<int> _m_loopContext;

	// native types
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
  int  alloc_register();
  void free_register(int i);
  void free_location(syntaxNode *p);
  void free_location(reg_location &r);
  void free_variables(sym_namespace *p);
  void FreeObject(reg_location *p, sym_base *);
  void CopyObject(reg_location *p, sym_base *);
  void push_parameter(std::string &expr, sym_type *expr_type, bool ref);
  
  void release_interface(const char *ploc);
  void addref_interface(const char *ploc);

	void emit_parameter(bool ref, sym_type *pparam_type, syntaxNode *pparam_expr);
  void emit_dispatch_expression(reg_location reg_l, size_t i, sym_type *ptype, syntaxNode *pexpr);
  void emit_as_expression(syntaxNode *pexpr);
  void emit_array_expression(reg_location *ploc, syntaxNode *pexpr);
  void emit_multiplicative_expression(syntaxNode *pexpr);
  void emit_expression(syntaxNode *pexpr);
  void emit_method_call(syntaxNode *pexpr,bool);
  void emit_additive_expression(syntaxNode*);
  void emit_record_expression(syntaxNode *pexpr,bool save);
  void emit_relational(syntaxNode *pexpr);
  void emit_equality(syntaxNode *pexpr);
  void emit_logical_or(syntaxNode *pexpr);
  void emit_logical_and(syntaxNode *pexpr);
  void emit_builtin(sym_procedure *pproc, syntaxNode *pparams);
  void emit_interface_call(syntaxNode *pexpr,syntaxNode *p2,interface_type *pitype,reg_location reg_l,bool alloc_r);
  void emit_class_call(syntaxNode *pexpr,syntaxNode *p2,class_type *pitype,reg_location reg_l,bool alloc_r);

  void emit_function_definition(syntaxNode *p);
  void emit_procedure_definition(syntaxNode *p);
  void emit_constructor_definition(syntaxNode *p);
  void emit_destructor_definition(syntaxNode *p);
  void emit_definitions(syntaxNode *p);

  void emit_statement_raise(syntaxNode *p);
  void emit_statement_with(syntaxNode *p);
  void emit_statement_assign (syntaxNode *p);
  void emit_statement_repeat (syntaxNode *p);
  void emit_statement_while (syntaxNode *p);
  void emit_statement_if (syntaxNode *p);
  void emit_statement_for (syntaxNode *p);
  void emit_statement_case (syntaxNode *p);
  void emit_compound_statement (syntaxNode *p);
  void emit_statement_try (syntaxNode *p);
  void emit_simple_statement( syntaxNode *p);
  void emit_statement_goto( syntaxNode *p);
  void emit_statement_label( syntaxNode *p);
  void emit_statement(syntaxNode *p);

  bool emit_module(syntaxNode *p);
  bool emit_program(syntaxNode *p);
  bool emit_script(syntaxNode *p);
public:
  bool compileModule(Cmodule *punit);
};

void compile_unit(Cmodule *cu);

#endif//__ipascal_compiler_h_