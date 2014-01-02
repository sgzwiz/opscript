/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_CONST = 258,
     TOK_TYPE = 259,
     TOK_VAR = 260,
     TOK_SEMICOLON = 261,
     TOK_IDENTIFIER = 262,
     TOK_EQ = 263,
     TOK_NE = 264,
     TOK_LT = 265,
     TOK_LE = 266,
     TOK_GT = 267,
     TOK_GE = 268,
     TOK_THREADVAR = 269,
     TOK_PLUS = 270,
     TOK_MINUS = 271,
     TOK_STAR = 272,
     TOK_SLASH = 273,
     TOK_STRING = 274,
     TOK_REAL = 275,
     TOK_INTEGER = 276,
     TOK_HEX = 277,
     TOK_ARRAY = 278,
     TOK_LBRACKET = 279,
     TOK_RBRACKET = 280,
     TOK_OF = 281,
     TOK_RECORD = 282,
     TOK_END = 283,
     TOK_LPAREN = 284,
     TOK_RPAREN = 285,
     TOK_DOTDOT = 286,
     TOK_COLON = 287,
     TOK_COMMA = 288,
     TOK_ASSIGN = 289,
     TOK_REPEAT = 290,
     TOK_UNTIL = 291,
     TOK_WHILE = 292,
     TOK_DO = 293,
     TOK_AS = 294,
     TOK_IF = 295,
     TOK_THEN = 296,
     TOK_ELSE = 297,
     TOK_FOR = 298,
     TOK_TO = 299,
     TOK_DOWNTO = 300,
     TOK_RAISE = 301,
     TOK_CASE = 302,
     TOK_BEGIN = 303,
     TOK_AND = 304,
     TOK_OR = 305,
     TOK_XOR = 306,
     TOK_ON = 307,
     TOK_IN = 308,
     TOK_DIV = 309,
     TOK_MOD = 310,
     TOK_NOT = 311,
     TOK_PROGRAM = 312,
     TOK_DOT = 313,
     TOK_WITH = 314,
     TOK_SHL = 315,
     TOK_SHR = 316,
     TOK_FUNCTION = 317,
     TOK_PROCEDURE = 318,
     TOK_FORWARD = 319,
     TOK_TRUE = 320,
     TOK_FALSE = 321,
     TOK_USES = 322,
     TOK_CDECL = 323,
     TOK_DYNAMIC = 324,
     TOK_VIRTUAL = 325,
     TOK_LABEL = 326,
     TOK_EXPORT = 327,
     TOK_EXTERNAL = 328,
     TOK_FAR = 329,
     TOK_OVERRIDE = 330,
     TOK_OVERLOAD = 331,
     TOK_PASCAL = 332,
     TOK_MESSAGE = 333,
     TOK_REINTRODUCE = 334,
     TOK_SET = 335,
     TOK_SAFECALL = 336,
     TOK_STDCALL = 337,
     TOK_OUT = 338,
     TOK_FILE = 339,
     TOK_PACKED = 340,
     TOK_NIL = 341,
     TOK_OBJECT = 342,
     TOK_INHERITED = 343,
     TOK_GOTO = 344,
     TOK_CLASS = 345,
     TOK_PROTECTED = 346,
     TOK_PUBLIC = 347,
     TOK_PRIVATE = 348,
     TOK_PUBLISHED = 349,
     TOK_PROPERTY = 350,
     TOK_AT = 351,
     TOK_READ = 352,
     TOK_WRITE = 353,
     TOK_DEFAULT = 354,
     TOK_STORED = 355,
     TOK_NODEFAULT = 356,
     TOK_IMPLEMENTS = 357,
     TOK_INTERFACE = 358,
     TOK_UNIT = 359,
     TOK_IMPLEMENTATION = 360,
     TOK_CONSTRUCTOR = 361,
     TOK_DESTRUCTOR = 362,
     TOK_IS = 363,
     TOK_EXCEPT = 364,
     TOK_TRY = 365,
     TOK_FINALLY = 366,
     TOK_INITIALIZATION = 367,
     TOK_FINALIZATION = 368,
     TOK_ABSTRACT = 369,
     TOK_ASSEMBLER = 370,
     TOK_DEREF = 371,
     TOK_LAND = 372,
     TOK_LOR = 373,
     TOK_ADDR = 374,
     TOK_DISPID = 375,
     TOK_DISPINTERFACE = 376,
     TOK_READONLY = 377,
     TOK_WRITEONLY = 378,
     TOK_EXPRESSION = 379,
     TOK_PROCEDURE_EXPRESSION = 380,
     PREC_UNARYOP = 381,
     PREC_LOWERTHANELSE = 382,
     TOK_FINAL_TOKEN = 383
   };
#endif
#define TOK_CONST 258
#define TOK_TYPE 259
#define TOK_VAR 260
#define TOK_SEMICOLON 261
#define TOK_IDENTIFIER 262
#define TOK_EQ 263
#define TOK_NE 264
#define TOK_LT 265
#define TOK_LE 266
#define TOK_GT 267
#define TOK_GE 268
#define TOK_THREADVAR 269
#define TOK_PLUS 270
#define TOK_MINUS 271
#define TOK_STAR 272
#define TOK_SLASH 273
#define TOK_STRING 274
#define TOK_REAL 275
#define TOK_INTEGER 276
#define TOK_HEX 277
#define TOK_ARRAY 278
#define TOK_LBRACKET 279
#define TOK_RBRACKET 280
#define TOK_OF 281
#define TOK_RECORD 282
#define TOK_END 283
#define TOK_LPAREN 284
#define TOK_RPAREN 285
#define TOK_DOTDOT 286
#define TOK_COLON 287
#define TOK_COMMA 288
#define TOK_ASSIGN 289
#define TOK_REPEAT 290
#define TOK_UNTIL 291
#define TOK_WHILE 292
#define TOK_DO 293
#define TOK_AS 294
#define TOK_IF 295
#define TOK_THEN 296
#define TOK_ELSE 297
#define TOK_FOR 298
#define TOK_TO 299
#define TOK_DOWNTO 300
#define TOK_RAISE 301
#define TOK_CASE 302
#define TOK_BEGIN 303
#define TOK_AND 304
#define TOK_OR 305
#define TOK_XOR 306
#define TOK_ON 307
#define TOK_IN 308
#define TOK_DIV 309
#define TOK_MOD 310
#define TOK_NOT 311
#define TOK_PROGRAM 312
#define TOK_DOT 313
#define TOK_WITH 314
#define TOK_SHL 315
#define TOK_SHR 316
#define TOK_FUNCTION 317
#define TOK_PROCEDURE 318
#define TOK_FORWARD 319
#define TOK_TRUE 320
#define TOK_FALSE 321
#define TOK_USES 322
#define TOK_CDECL 323
#define TOK_DYNAMIC 324
#define TOK_VIRTUAL 325
#define TOK_LABEL 326
#define TOK_EXPORT 327
#define TOK_EXTERNAL 328
#define TOK_FAR 329
#define TOK_OVERRIDE 330
#define TOK_OVERLOAD 331
#define TOK_PASCAL 332
#define TOK_MESSAGE 333
#define TOK_REINTRODUCE 334
#define TOK_SET 335
#define TOK_SAFECALL 336
#define TOK_STDCALL 337
#define TOK_OUT 338
#define TOK_FILE 339
#define TOK_PACKED 340
#define TOK_NIL 341
#define TOK_OBJECT 342
#define TOK_INHERITED 343
#define TOK_GOTO 344
#define TOK_CLASS 345
#define TOK_PROTECTED 346
#define TOK_PUBLIC 347
#define TOK_PRIVATE 348
#define TOK_PUBLISHED 349
#define TOK_PROPERTY 350
#define TOK_AT 351
#define TOK_READ 352
#define TOK_WRITE 353
#define TOK_DEFAULT 354
#define TOK_STORED 355
#define TOK_NODEFAULT 356
#define TOK_IMPLEMENTS 357
#define TOK_INTERFACE 358
#define TOK_UNIT 359
#define TOK_IMPLEMENTATION 360
#define TOK_CONSTRUCTOR 361
#define TOK_DESTRUCTOR 362
#define TOK_IS 363
#define TOK_EXCEPT 364
#define TOK_TRY 365
#define TOK_FINALLY 366
#define TOK_INITIALIZATION 367
#define TOK_FINALIZATION 368
#define TOK_ABSTRACT 369
#define TOK_ASSEMBLER 370
#define TOK_DEREF 371
#define TOK_LAND 372
#define TOK_LOR 373
#define TOK_ADDR 374
#define TOK_DISPID 375
#define TOK_DISPINTERFACE 376
#define TOK_READONLY 377
#define TOK_WRITEONLY 378
#define TOK_EXPRESSION 379
#define TOK_PROCEDURE_EXPRESSION 380
#define PREC_UNARYOP 381
#define PREC_LOWERTHANELSE 382
#define TOK_FINAL_TOKEN 383




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)

typedef union YYSTYPE {
  token_item name;
  syntaxNode *node;
} YYSTYPE;
/* Line 1248 of yacc.c.  */

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





