/* 
 * symbolObjects.h - Declaration of symbol table objects.
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __symbolObjects_h_
#define __symbolObjects_h_

enum NativeTypeCodes
{
  ty_dummy,
  ty_integer,
  ty_boolean,
  ty_char,
  ty_real,
  ty_string,
  ty_widestring,
  ty_record,
  ty_class,
  ty_array,
  ty_set,
  ty_function,
  ty_procedure,
  ty_enumeration,
  ty_pointer,
  ty_interface,
  ty_variant,
  ty_undeclared,
};

enum SymbolTypes
{
  sy_type,
  sy_procedure,
  sy_variable,
  sy_namespace,
};

class sym_procedure;
class sym_namespace;
struct syntaxNode;

class sym_base
{
public:
	sym_base();
  virtual ~sym_base();

	int scode;
  size_t unit_index;
  sym_base *next;
};

#define TTYPE_UNSIGNED    0x01
#define TTYPE_IMPLEMENTED 0x02

class sym_type : public sym_base
{
public:
  sym_type();
  sym_type(const char *n, NativeTypeCodes code, unsigned s, unsigned char f=0);

	const char *name;
  NativeTypeCodes tycode;
  size_t size;
  unsigned tnumber;
  unsigned char flags;
  sym_type *base_type;
  size_t ptr_level;
};

struct sym_value
{
  union
  {
    unsigned int_val;
    double real_val;
    char *string_val;
    syntaxNode *def_val;
  };
};

struct string_data
{
  BYTE types;
  int  delphi_ref;
  size_t  len;
  char strdata[1];
};

struct wstring_data
{
  BYTE  type;
  wchar_t *str;
};

struct sym_property
{
  unsigned short dispid; // used for automation objects
  sym_procedure *array_proc;
  syntaxNode *pread;
  syntaxNode *pwrite;
};

#define VIS_PUBLIC 0
#define VIS_PRIVATE 1
#define VIS_PROTECTED 2
#define VIS_PUBLISHED 3

class sym_variable : public sym_base
{
public:
	sym_variable();
	~sym_variable();
  char *name;
  sym_property *prop_info;
  sym_type *type;
  sym_value value;
  sym_namespace *parent;
  int offset;
  unsigned char default_ : 1;
  unsigned char const_ : 1;
  unsigned char param  : 1;
  unsigned char static_var : 1;
  unsigned char visibility : 2;
  unsigned char byref : 1;
};

class sym_namespace : public sym_base
{
public:
  size_t size;
  const char *name;
  size_t proc_count;
  sym_base *first_symbol;// fist_,last_ symbol both allow the space to
  sym_base *last_symbol; // be transversed in the order of declaration
  sym_type *space_type; 
  sym_namespace *parent;
  thash<sym_procedure*,17> procedures;
  thash<sym_type*,17> types;
  thash<sym_variable*,17> variables;
public:
  virtual sym_base *get_member(const char *name );
  virtual void add_symbol(sym_base *b);
};

struct symbol_context
{
  symbol_context *last;
  sym_namespace *ps;
  sym_variable *ns_var;
};

class cundeclared_type : public sym_type
{
public:
  struct ref_location
  {
    sym_type **pref;
    unsigned line;
  };
public:
  std::vector<ref_location> _m_locations;
};

class class_type : public sym_type
{
public:
  sym_namespace members; 
};

class record_type : public sym_type
{
public:
  sym_namespace members; 
};

class interface_type : public sym_type
{
public:
  sym_namespace members;
  syntaxNode *guid;
  size_t guid_index;
  unsigned char disponly : 1;
};

class  function_type : public sym_type
{
public:
};

class  procedure_type : public sym_type
{
public:
};

struct array_bound
{
  sym_type *bound_type;
  int high;
  int low;
};

class  array_type : public sym_type
{
public:
	array_type();
	~array_type();
  
	unsigned count;
  unsigned element_count;
  array_bound **dimentions; 
  sym_type *base_type;
  sym_variable *pvar;
};

class  enum_type : public sym_type
{
public:
};

struct sym_constant
{
  sym_type *type;
  char *value;
  size_t offset;
};

class sym_procedure : public sym_base
{
public:
	sym_procedure();
  ~sym_procedure();
	syntaxNode *psyntax;
  char *name;
  char *code_name;
  sym_type *ret_type;
  sym_namespace members; 
  sym_variable **parameters;
  sym_type *parent;
  sym_procedure *next_overload; // needed for overloaded members
  unsigned short dispid; // used for automation objects
  size_t param_count;
  size_t default_count;
  size_t param_size;
  size_t proc_number;
  int proc_offset;
  size_t proc_gnumber;
  unsigned char static_proc : 1;
  unsigned char safecall : 1;
  unsigned char overloaded : 1;
  unsigned char built_in : 1;
  unsigned char function : 1;
  unsigned char inline_fn : 1;
  unsigned char implemented : 1;
  unsigned char visibility : 2;
  unsigned char constructor : 2;
};

struct sym_location
{
  sym_namespace *ps;
  symbol_context *ns;
};

struct reg_location
{
  BYTE reg;
  int offset;
  BYTE deref;
  size_t size;
  BYTE const_;
  size_t stack;
  unsigned char free_string : 1;
  unsigned char free_bstr : 1;
  unsigned char free_interface : 1;
  unsigned char free_object : 1;
};

class Cconstants
{
private:
  std::vector<string_data*> _m_strings;
	std::vector<wstring_data*> _m_wstrings;
  std::vector<sym_constant> _m_array;
  size_t _m_size;
public:
  Cconstants();
  ~Cconstants();
  size_t get_size(){return _m_size;}
  void print_consts(FILE *pf);
  size_t add(syntaxNode *p);
  void clear();
  size_t count() {return _m_array.size();}
  sym_constant *operator[](size_t i){return &_m_array[i];}
};

sym_type *get_expr_type(syntaxNode *p);
bool is_const(symbol_context *ns, syntaxNode *p);
int expr_val_int(symbol_context *ns,syntaxNode *p);

#endif//__symbolObjects_h_