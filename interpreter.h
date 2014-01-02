/* 
 * interpreter.h - Declaration of the interpreter object. 
 *
 * Copyright (c) 2004-2005, Felix Deschamps. All rights reserved. 
 *
 * This file is part of the OpScript Active Script Engine. 
 * This program is made available under the terms of the 
 * Common Public License v1.0 which accompanies this distribution, 
 * and is available at http://opensource.org/licenses/cpl.php
 *
 */
#ifndef __interpreter_h__
#define __interpreter_h__

#pragma pack(push,1)

#define REG_COUNT 128
#define EBP 0
#define ESP 1
#define EDS 2

union _register
{
  char    vc;
  short   vshort;
  int     vint;
  float   vf;
  double  vdbl;
  char   *pvchar;
  short  *pvshort;
  int    *pvint;
  float  *pfloat;
  double *pvdbl;
};

struct _reg_desc
{
  BYTE reg;
  unsigned offset;
  BYTE deref  : 1;
  BYTE size   : 4;
  BYTE unused : 3;
};

union operand
{
  unsigned value;
  _reg_desc regop;
};

enum VmInstructions
{
  // mem access
  i_mov,
  i_push,
  i_pop,
  i_push_rgs,
  i_pop_rgs,
  i_mov_ds,
  i_mv_sg,
  // arithmetic
  i_add,
  i_sub,
  i_mult,
  i_div,
  i_mod,
  i_inc,
  i_dec,
  // flow control
  i_call,
  i_calln,
  i_callu,
  i_jmp,
  i_ret,
  // boolean functinality
  i_lt,
  i_le,
  i_ge,
  i_gt,
  i_eq,
  i_ne,
  // bit operations
  i_shl,
  i_shr,
  i_and,
  i_or,
  i_xor,
};

struct _instruction
{ 
  BYTE inst   : 7;
  BYTE const_ : 1;
  operand dest;
  operand src;
};

struct cpu_state
{
  //base pointers.
  BYTE *cs;
  BYTE *stack;
  BYTE *ds;
  
  size_t curr_unit;
  Cmodule **units;
  _instruction *ip;
  _register regs[REG_COUNT];
  _register *saved_regs;
  size_t saved_count;
};

#pragma pack(pop)





typedef void (*_BUILTIN_FN)(_register *);



//********************************************************************
// built in functions 
//********************************************************************
// input/output functions
void read(_register *);
void writeb(_register *);
void writec(_register *);
void writey(_register *);
void writem(_register *);
void writei(_register *);
void writef(_register *);
void writer(_register *);
void writes(_register *);
void writew(_register *);
#ifdef _WIN32
void writev(_register *);
#endif
void writelnb(_register *);
void writelnc(_register *);
void writelny(_register *);
void writelnm(_register *);
void writelni(_register *);
void writelnf(_register *);
void writelnr(_register *);
void writelns(_register *);
void writelnw(_register *);
#ifdef _WIN32
void writelnv(_register *);
void showmessage(_register *);
#endif
// memory functions 
void GetMem(_register *);
void FreeMem(_register *);
void mcpy(_register *);
void mset(_register *);
#ifdef _WIN32
// COM support
void dispcall(_register *);
void interface_thunk(_register *);
void safecall_thunk(_register *);
void create_object(_register *);
void createcomobjects(_register *regs);
void createcomobjectetguid29(_register *regs);
// variant support
void varcmp(_register*);
void varclear(_register*);
void varcopy(_register *);
void vartobool(_register *);
void vartochar(_register *);
void vartobyte(_register *);
void vartoshort(_register *);
void vartoint(_register *);
void vartofloat(_register *);
void vartodouble(_register *);
void vartostring(_register *);
void vartowidestr(_register *);
void vartodispatch(_register *);
void booltovar(_register *);
void chartovar(_register *);
void bytetovar(_register *);
void shorttovar(_register *);
void inttovar(_register *);
void floattovar(_register *);
void doubletovar(_register *);
void stringtovar(_register *);
void widestrtovar(_register *);
void interfacetovar(_register *);
#endif
// string support
void strcmp(_register *);
void wstrcmp(_register *);
void strtowide(_register *);
void free_widestring(_register*);
void _cpystr(_register*);
void _cpyregstr(_register*);
void _freestr(_register*);
void _catstr(_register*);
void _cpybstr(_register*);
void _cpyregbstr(_register*);
void _freebstr(_register*);
void _catbstr(_register*);
void _cnvbstrtostr(_register*);
void _cnvregbstrtostr(_register*);
void _cnvstrtobstr(_register*);
void _cnvregstrtobstr(_register*);
void _catbstrtostr(_register*);
void _catstrtobstr(_register*);
void inci(_register*);
void deci(_register*);

//********************************************************************
// CPU handling functions 
//********************************************************************
void init_cpu(cpu_state *p);
void clear_cpu(cpu_state *p);
void exec_function( unsigned offset, cpu_state *p );

#endif//_pascal_interpreter_h_