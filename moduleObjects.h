/* 
 * moduleObjects.h - Declaration of module objects, which form the basis of
 * execution.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __moduleObjects_h__
#define __moduleObjects_h__

#include "syntaxTree.h"

class Cconstants;
class CModuleManager;
class Cmodule;

#include "scriptErrors.h"

#ifdef _DEBUG_ASSEMBLY_
  typedef std::pair<int,int>  IntPair;
#endif

class Cmodule : public scriptErrorHandler
{
public:
  size_t code_size;
  size_t data_size;
  size_t error_count;
  sym_namespace global;
  Cconstants *_m_constants;
  symbol_context name_stack;
	std::vector<sym_base*> _m_symbols;

  bool is_script;

#ifdef _DEBUG_ASSEMBLY_
  std::vector<std::string> _m_asm_lines;
  std::map<int,int> _m_asm_line_map;
#endif

public: 

  Cmodule();
  virtual ~Cmodule();

  unsigned main_offset;
  
  void *cs;
  void *ds;

  virtual size_t get_index() = 0;
  virtual void set_compiled(bool) = 0;
  virtual void set_executed(bool) = 0;
  virtual bool is_compiled() = 0;
  virtual bool is_executed() = 0;
  virtual size_t get_procedure_count() = 0;
  virtual void add_procedure(const char *p1, sym_procedure *p2) = 0;

  virtual CModuleManager *get_manager() = 0;
  virtual sym_procedure *get_procedure(char *pname) = 0;
  virtual sym_base *get_symbol(const char *name, symbol_context *ns, bool recursive=false, sym_location *sloc=0) = 0;  
  virtual sym_type *add_undeclared(syntaxNode *p, sym_type **) = 0;
  virtual void update_undeclared(sym_type *) = 0;
  virtual void output_constants(FILE *p) = 0;

  virtual const char *get_name() = 0;
  virtual const char *get_filename() = 0;
  virtual const char *get_asmname() = 0;
  virtual CsyntaxTree *get_syntax_tree() = 0;
  virtual bool is_const(symbol_context *ns, syntaxNode *p) = 0;
  virtual void add_unit(Cmodule *) = 0;
  virtual int parseSystem() = 0;

  void loadNativeTypes();
  void registerSymbol(sym_base *pSym);
};

class CModuleManager
{
public:
  virtual Cmodule *get_unit(size_t) = 0;
  virtual Cmodule *get_unit(const char *) = 0;
  virtual Cmodule *create_unit(const char *) = 0;
  virtual size_t get_count() = 0;
  virtual size_t get_unit_index(const char *) = 0;  
  virtual void CompileAll() = 0;
};

#endif