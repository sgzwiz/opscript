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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse pascalparse
#define yylex   pascallex
#define yyerror pascalerror
#define yylval  pascallval
#define yychar  pascalchar
#define yydebug pascaldebug
#define yynerrs pascalnerrs


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




/* Copy the first part of user declarations.  */


#include "stdafx.h"
#include "syntaxTree.h"
#include "utils.h"
#ifdef _DEBUG
#define YYDEBUG 1
#endif

#pragma warning(disable : 4065) //default used with no case lables
#pragma warning(disable : 4102) // unreferenced label


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)

typedef union YYSTYPE {
  token_item name;
  syntaxNode *node;
} YYSTYPE;
/* Line 191 of yacc.c.  */

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */



#undef yylex
#undef yyerror

#include "opascalParser.h"

#define YYPARSE_PARAM lparam
#define yylex   ((CopascalLexxer*)lparam)->yylex
#define yyerror(x) ((CopascalLexxer*)lparam)->parserError(x,(yyssp - yyss),yychar,yyss,yylval,yyvs,&yyerrstatus);
#define goal    ((CopascalLexxer*)lparam)->_m_syntaxTree->setRoot
#define create_node ((CopascalLexxer*)lparam)->_m_syntaxTree->createNode
#define addScriptItem ((CopascalLexxer*)lparam)->_m_syntaxTree->addScriptItem
#define clearSemicolonErr() ((CopascalLexxer*)lparam)->clearSemicolonErr()

#undef yyparse
#define yyparse CpascalParser::pascalparse



/* Line 214 of yacc.c.  */


#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  151
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3247

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  138
/* YYNRULES -- Number of rules. */
#define YYNRULES  487
/* YYNRULES -- Number of states. */
#define YYNSTATES  883

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   383

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    16,    19,
      21,    24,    27,    30,    32,    34,    36,    38,    40,    42,
      45,    47,    51,    55,    60,    64,    65,    69,    71,    75,
      78,    80,    84,    87,    89,    92,    95,    98,   102,   105,
     107,   110,   114,   118,   122,   127,   131,   134,   138,   146,
     148,   152,   155,   158,   160,   163,   166,   170,   171,   173,
     178,   186,   191,   194,   198,   200,   202,   204,   206,   208,
     210,   212,   214,   216,   218,   220,   222,   224,   227,   231,
     234,   237,   240,   243,   246,   250,   254,   260,   267,   272,
     275,   279,   283,   288,   292,   296,   300,   304,   308,   315,
     320,   328,   333,   339,   343,   348,   353,   359,   362,   365,
     368,   371,   375,   378,   383,   387,   391,   395,   397,   399,
     403,   409,   413,   416,   421,   425,   429,   433,   437,   442,
     446,   451,   456,   461,   466,   473,   480,   487,   491,   495,
     499,   505,   511,   517,   521,   527,   531,   537,   538,   542,
     545,   547,   551,   554,   557,   560,   562,   564,   566,   570,
     574,   576,   580,   586,   592,   598,   599,   602,   605,   609,
     612,   616,   621,   625,   630,   636,   639,   643,   647,   652,
     654,   656,   658,   662,   664,   668,   672,   674,   678,   682,
     686,   692,   694,   696,   698,   702,   706,   708,   709,   711,
     719,   724,   729,   734,   737,   741,   746,   751,   757,   761,
     764,   766,   768,   770,   772,   775,   777,   781,   786,   789,
     792,   796,   800,   802,   806,   807,   809,   812,   814,   817,
     821,   823,   827,   829,   833,   839,   847,   852,   859,   862,
     866,   872,   874,   876,   878,   880,   882,   884,   886,   888,
     890,   892,   894,   896,   899,   903,   906,   908,   911,   916,
     921,   923,   927,   932,   938,   941,   945,   949,   954,   956,
     959,   963,   966,   971,   977,   979,   982,   986,   991,   998,
    1002,  1007,  1013,  1018,  1023,  1030,  1039,  1048,  1054,  1061,
    1069,  1078,  1087,  1097,  1099,  1102,  1106,  1111,  1114,  1116,
    1120,  1122,  1126,  1128,  1132,  1134,  1138,  1142,  1145,  1150,
    1154,  1158,  1159,  1161,  1163,  1166,  1171,  1177,  1180,  1184,
    1188,  1193,  1196,  1200,  1202,  1204,  1206,  1208,  1213,  1217,
    1218,  1220,  1224,  1226,  1228,  1231,  1234,  1237,  1240,  1244,
    1247,  1250,  1256,  1260,  1267,  1269,  1274,  1278,  1285,  1290,
    1298,  1301,  1307,  1309,  1312,  1314,  1317,  1319,  1321,  1323,
    1325,  1327,  1329,  1335,  1343,  1344,  1347,  1353,  1363,  1365,
    1369,  1370,  1372,  1373,  1375,  1376,  1379,  1383,  1384,  1387,
    1388,  1391,  1392,  1395,  1398,  1399,  1402,  1403,  1406,  1408,
    1409,  1412,  1416,  1418,  1420,  1423,  1426,  1430,  1432,  1436,
    1439,  1441,  1445,  1447,  1451,  1455,  1457,  1461,  1465,  1469,
    1473,  1475,  1479,  1483,  1485,  1489,  1493,  1497,  1501,  1503,
    1507,  1511,  1515,  1519,  1521,  1525,  1527,  1531,  1533,  1537,
    1539,  1542,  1546,  1551,  1553,  1557,  1561,  1563,  1569,  1573,
    1575,  1578,  1580,  1582,  1584,  1587,  1590,  1593,  1596,  1598,
    1601,  1604,  1607,  1610,  1613,  1615,  1619,  1624,  1626,  1629,
    1631,  1633,  1636,  1638,  1640,  1644,  1648,  1652,  1656,  1661,
    1666,  1671,  1676,  1679,  1684,  1690,  1694,  1696,  1700,  1705,
    1709,  1712,  1714,  1716,  1718,  1720,  1722,  1724
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     130,     0,    -1,   136,    -1,   144,    -1,   134,    -1,   131,
      -1,   132,    -1,   124,   260,    -1,   125,   151,    -1,   195,
      -1,     5,   180,    -1,     3,   177,    -1,     4,   179,    -1,
     141,    -1,   155,    -1,   152,    -1,   156,    -1,   157,    -1,
     135,    -1,   135,   210,    -1,   133,    -1,   135,   210,   133,
      -1,   135,     1,   133,    -1,   137,     6,   140,    58,    -1,
      57,     7,   138,    -1,    -1,    29,   139,    30,    -1,     7,
      -1,   139,    33,     7,    -1,   141,   142,    -1,   142,    -1,
      67,   139,     6,    -1,   143,   211,    -1,   211,    -1,     3,
     168,    -1,     4,   169,    -1,     5,   170,    -1,    71,   139,
       6,    -1,    14,   170,    -1,   151,    -1,    90,   151,    -1,
     143,     3,   168,    -1,   143,     4,   169,    -1,   143,     5,
     170,    -1,   143,    71,   139,     6,    -1,   143,    14,   170,
      -1,   143,   151,    -1,   143,    90,   151,    -1,   104,     7,
       6,   145,   146,   148,    58,    -1,   103,    -1,   103,   141,
     150,    -1,   103,   150,    -1,   103,   141,    -1,   105,    -1,
     105,   141,    -1,   105,   143,    -1,   105,   141,   143,    -1,
      -1,     6,    -1,   112,   212,   147,    28,    -1,   112,   212,
     147,   113,   212,   147,    28,    -1,    48,   212,   147,    28,
      -1,   112,    28,    -1,   112,   113,    28,    -1,    28,    -1,
      68,    -1,    72,    -1,    74,    -1,    77,    -1,    81,    -1,
      82,    -1,    70,    -1,   114,    -1,    69,    -1,    75,    -1,
      79,    -1,    76,    -1,   120,    21,    -1,   120,    16,    21,
      -1,     3,   168,    -1,     4,   169,    -1,     5,   170,    -1,
      14,   170,    -1,   159,     6,    -1,   159,   154,     6,    -1,
     159,   222,     6,    -1,   159,     6,    78,     7,     6,    -1,
     159,     6,    78,     7,   222,     6,    -1,   159,   222,   154,
       6,    -1,   158,     6,    -1,   158,   154,     6,    -1,   158,
     222,     6,    -1,   158,   222,   154,     6,    -1,   150,     3,
     168,    -1,   150,     4,   169,    -1,   150,     5,   170,    -1,
     150,    14,   170,    -1,   150,   159,     6,    -1,   150,   159,
       6,    78,     7,     6,    -1,   150,   159,   222,     6,    -1,
     150,   159,     6,    78,     7,   222,     6,    -1,   150,   159,
     154,     6,    -1,   150,   159,   222,   154,     6,    -1,   150,
     158,     6,    -1,   150,   158,   222,     6,    -1,   150,   158,
     154,     6,    -1,   150,   158,   222,   154,     6,    -1,   152,
       6,    -1,   155,     6,    -1,   156,     6,    -1,   157,     6,
      -1,   158,     6,   142,    -1,   158,   154,    -1,   158,   222,
       6,   142,    -1,   158,   222,   154,    -1,   158,     6,   115,
      -1,   158,     6,    64,    -1,     7,    -1,    19,    -1,     6,
      73,   153,    -1,     6,    73,   153,     7,   153,    -1,   159,
       6,   142,    -1,   159,   154,    -1,   159,   222,     6,   142,
      -1,   159,   222,   154,    -1,   159,     6,   115,    -1,   159,
       6,    64,    -1,   160,     6,   142,    -1,   160,   222,     6,
     142,    -1,   161,     6,   142,    -1,   161,   222,     6,   142,
      -1,    62,     7,   162,   167,    -1,    62,    97,   162,   167,
      -1,    62,    98,   162,   167,    -1,    62,     7,    58,     7,
     162,   167,    -1,    62,     7,    58,    97,   162,   167,    -1,
      62,     7,    58,    98,   162,   167,    -1,    63,     7,   162,
      -1,    63,    98,   162,    -1,    63,    97,   162,    -1,    63,
       7,    58,     7,   162,    -1,    63,     7,    58,    97,   162,
      -1,    63,     7,    58,    98,   162,    -1,   106,     7,   162,
      -1,   106,     7,    58,     7,   162,    -1,   107,     7,   162,
      -1,   107,     7,    58,     7,   162,    -1,    -1,    29,   163,
      30,    -1,    29,    30,    -1,   164,    -1,   163,     6,   164,
      -1,     5,   166,    -1,     3,   166,    -1,    83,   166,    -1,
     166,    -1,     7,    -1,    78,    -1,   165,    33,     7,    -1,
     165,    33,    78,    -1,   165,    -1,   165,    32,   186,    -1,
     165,    32,   186,     8,   260,    -1,   165,    32,    23,    26,
     186,    -1,   165,    32,    23,    26,     3,    -1,    -1,    32,
     186,    -1,   177,     6,    -1,   168,   177,     6,    -1,   179,
       6,    -1,   179,   222,     6,    -1,     7,     8,    90,     6,
      -1,   169,   179,     6,    -1,   169,   179,   222,     6,    -1,
     169,     7,     8,    90,     6,    -1,   180,     6,    -1,   180,
     222,     6,    -1,   170,   180,     6,    -1,   170,   180,   222,
       6,    -1,   260,    -1,   172,    -1,   176,    -1,    29,   173,
      30,    -1,   174,    -1,   173,     6,   174,    -1,     7,    32,
     171,    -1,   171,    -1,   175,    33,   171,    -1,    29,   175,
      30,    -1,     7,     8,   260,    -1,     7,    32,   184,     8,
     171,    -1,   224,    -1,   244,    -1,   217,    -1,     7,     8,
     184,    -1,     7,     8,   178,    -1,   191,    -1,    -1,    85,
      -1,   181,    23,    24,   187,    25,    26,   184,    -1,   181,
      23,    26,   184,    -1,   181,    27,   188,    28,    -1,   181,
      80,    26,   186,    -1,    63,   162,    -1,    62,   162,   167,
      -1,    62,   162,   167,   149,    -1,    63,   162,    26,    87,
      -1,    62,   162,   167,    26,    87,    -1,    90,    26,     7,
      -1,    90,   226,    -1,   186,    -1,   182,    -1,   183,    -1,
     116,    -1,   185,   116,    -1,     7,    -1,     7,    58,     7,
      -1,     7,    24,   250,    25,    -1,     4,     7,    -1,   185,
       7,    -1,    29,   139,    30,    -1,   250,    31,   250,    -1,
     186,    -1,   187,    33,   186,    -1,    -1,   189,    -1,   189,
       6,    -1,   192,    -1,   189,   192,    -1,   189,     6,   192,
      -1,   190,    -1,   189,     6,   190,    -1,   191,    -1,   165,
      32,   184,    -1,   165,    32,   184,     8,   260,    -1,   165,
      32,   184,     8,    29,   262,    30,    -1,    47,   186,    26,
     193,    -1,    47,     7,    32,   186,    26,   193,    -1,   194,
       6,    -1,   193,   194,     6,    -1,   262,    32,    29,   188,
      30,    -1,   196,    -1,   197,    -1,   204,    -1,   205,    -1,
     206,    -1,   207,    -1,   208,    -1,   209,    -1,   211,    -1,
     199,    -1,   259,    -1,    88,    -1,    89,     7,    -1,     7,
      32,   195,    -1,     7,    32,    -1,    46,    -1,    46,   260,
      -1,    46,   260,    96,   260,    -1,    59,   198,    38,   195,
      -1,   247,    -1,   198,    33,   247,    -1,   110,   212,   200,
      28,    -1,   110,   212,     6,   200,    28,    -1,   111,   212,
      -1,   111,     6,   212,    -1,   111,   212,     6,    -1,   111,
       6,   212,     6,    -1,   201,    -1,   109,   212,    -1,   109,
     212,     6,    -1,   109,   202,    -1,   109,   202,    42,   212,
      -1,   109,   202,    42,   212,     6,    -1,   109,    -1,   203,
       6,    -1,   202,   203,     6,    -1,    52,     7,    38,   195,
      -1,    52,     7,    32,     7,    38,   195,    -1,   259,    34,
     260,    -1,    35,   212,    36,   260,    -1,    35,   212,     6,
      36,   260,    -1,    37,   260,    38,   195,    -1,    40,   260,
      41,   195,    -1,    40,   260,    41,   195,    42,   195,    -1,
      43,     7,    34,   260,    44,   260,    38,   195,    -1,    43,
       7,    34,   260,    45,   260,    38,   195,    -1,    47,   260,
      26,   213,    28,    -1,    47,   260,    26,   213,     6,    28,
      -1,    47,   260,    26,   213,    42,   212,    28,    -1,    47,
     260,    26,   213,     6,    42,   212,    28,    -1,    47,   260,
      26,   213,    42,   212,     6,    28,    -1,    47,   260,    26,
     213,     6,    42,   212,     6,    28,    -1,     6,    -1,   210,
       6,    -1,    48,   212,    28,    -1,    48,   212,   210,    28,
      -1,    48,    28,    -1,   195,    -1,   212,   210,   195,    -1,
     216,    -1,   213,     6,   216,    -1,   215,    -1,   214,    33,
     215,    -1,   260,    -1,   260,    31,   260,    -1,   214,    32,
     195,    -1,   214,    32,    -1,    87,   218,   219,    28,    -1,
      87,   219,    28,    -1,    29,     7,    30,    -1,    -1,   220,
      -1,   221,    -1,   220,   221,    -1,   139,    32,   184,     6,
      -1,   220,   139,    32,   184,     6,    -1,   223,     6,    -1,
     223,   222,     6,    -1,   221,   223,     6,    -1,   221,   223,
     222,     6,    -1,     6,   149,    -1,   222,     6,   149,    -1,
     158,    -1,   159,    -1,   160,    -1,   161,    -1,    90,   226,
     225,    28,    -1,    90,   225,    28,    -1,    -1,   227,    -1,
      29,   139,    30,    -1,   228,    -1,   230,    -1,    92,   228,
      -1,    91,   228,    -1,    94,   228,    -1,    93,   228,    -1,
     227,   230,   228,    -1,   227,   230,    -1,   229,     6,    -1,
     229,     6,    78,     7,     6,    -1,   229,   222,     6,    -1,
     229,     6,    78,     7,   222,     6,    -1,   231,    -1,   139,
      32,   184,     6,    -1,   228,   229,     6,    -1,   228,   229,
       6,    78,     7,     6,    -1,   228,   229,   222,     6,    -1,
     228,   229,     6,    78,     7,   222,     6,    -1,   228,   231,
      -1,   228,   139,    32,   184,     6,    -1,   158,    -1,    90,
     158,    -1,   159,    -1,    90,   159,    -1,   160,    -1,   161,
      -1,    92,    -1,    91,    -1,    94,    -1,    93,    -1,    95,
       7,   232,   233,     6,    -1,    95,     7,   232,   233,     6,
      99,     6,    -1,    -1,    32,     7,    -1,    24,   163,    25,
      32,     7,    -1,   241,   238,   239,   240,   235,   236,   237,
     242,   243,    -1,     7,    -1,   234,    58,     7,    -1,    -1,
     122,    -1,    -1,   123,    -1,    -1,   120,    21,    -1,   120,
      16,    21,    -1,    -1,    97,   234,    -1,    -1,    98,   234,
      -1,    -1,   100,     7,    -1,   100,   266,    -1,    -1,     7,
     260,    -1,    -1,    99,   260,    -1,   101,    -1,    -1,   102,
       7,    -1,   103,   226,   246,    -1,   103,    -1,   121,    -1,
     103,   246,    -1,   121,   246,    -1,    24,    19,    25,    -1,
      28,    -1,   245,   227,    28,    -1,   245,    28,    -1,   259,
      -1,   247,    39,   259,    -1,   247,    -1,   248,    60,   247,
      -1,   248,    61,   247,    -1,   248,    -1,   249,    17,   248,
      -1,   249,    18,   248,    -1,   249,    55,   248,    -1,   249,
      54,   248,    -1,   249,    -1,   250,    15,   249,    -1,   250,
      16,   249,    -1,   250,    -1,   251,    12,   250,    -1,   251,
      10,   250,    -1,   251,    11,   250,    -1,   251,    13,   250,
      -1,   251,    -1,   252,     8,   251,    -1,   252,     9,   251,
      -1,   252,   108,   251,    -1,   252,    53,   251,    -1,   252,
      -1,   253,   117,   252,    -1,   253,    -1,   254,   118,   253,
      -1,   254,    -1,   255,    51,   254,    -1,   255,    -1,    56,
     255,    -1,   256,    49,   255,    -1,   256,    49,    56,   255,
      -1,   256,    -1,   257,    50,   256,    -1,   260,    31,   260,
      -1,   260,    -1,   258,    33,   260,    31,   260,    -1,   258,
      33,   260,    -1,     7,    -1,     7,   185,    -1,    78,    -1,
      99,    -1,    98,    -1,    78,   185,    -1,    99,   185,    -1,
      98,   185,    -1,    97,   185,    -1,    97,    -1,    16,     7,
      -1,    15,     7,    -1,   119,   259,    -1,    16,   266,    -1,
      15,   266,    -1,   266,    -1,    29,   260,    30,    -1,    29,
     260,    30,   185,    -1,   261,    -1,   261,   185,    -1,   265,
      -1,   264,    -1,   264,   185,    -1,   263,    -1,   257,    -1,
       7,    29,    30,    -1,    98,    29,    30,    -1,    97,    29,
      30,    -1,   264,    29,    30,    -1,     7,    29,   262,    30,
      -1,    98,    29,   262,    30,    -1,    97,    29,   262,    30,
      -1,   264,    29,   262,    30,    -1,    88,     7,    -1,    88,
       7,    29,    30,    -1,    88,     7,    29,   262,    30,    -1,
     262,    33,   260,    -1,   260,    -1,   259,    58,   259,    -1,
     259,    24,   262,    25,    -1,    24,   258,    25,    -1,    24,
      25,    -1,    22,    -1,    21,    -1,    20,    -1,    19,    -1,
      65,    -1,    66,    -1,    86,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   225,   225,   233,   240,   247,   251,   257,   263,   269,
     274,   279,   284,   289,   294,   299,   304,   309,   316,   321,
     328,   333,   338,   347,   356,   365,   369,   377,   383,   391,
     398,   406,   414,   421,   429,   435,   441,   447,   455,   461,
     467,   473,   479,   485,   491,   498,   504,   510,   518,   533,
     538,   545,   551,   559,   564,   570,   576,   585,   589,   596,
     602,   609,   615,   620,   625,   632,   637,   642,   647,   652,
     657,   662,   667,   672,   677,   682,   687,   692,   699,   708,
     714,   720,   726,   732,   741,   754,   765,   782,   798,   809,
     819,   832,   843,   855,   861,   867,   873,   879,   888,   905,
     916,   932,   942,   953,   962,   972,   983,   996,  1001,  1006,
    1011,  1018,  1025,  1036,  1044,  1052,  1057,  1066,  1071,  1079,
    1086,  1095,  1102,  1111,  1119,  1127,  1132,  1141,  1148,  1158,
    1165,  1175,  1183,  1191,  1199,  1208,  1217,  1228,  1235,  1242,
    1249,  1257,  1265,  1275,  1282,  1292,  1299,  1309,  1313,  1318,
    1325,  1331,  1339,  1346,  1353,  1360,  1367,  1373,  1379,  1385,
    1393,  1399,  1406,  1415,  1424,  1435,  1439,  1447,  1453,  1461,
    1467,  1474,  1482,  1488,  1495,  1505,  1511,  1518,  1524,  1533,
    1539,  1545,  1553,  1560,  1566,  1574,  1584,  1590,  1598,  1606,
    1613,  1623,  1628,  1633,  1640,  1647,  1656,  1663,  1667,  1674,
    1683,  1691,  1699,  1709,  1716,  1724,  1732,  1739,  1747,  1755,
    1764,  1769,  1774,  1781,  1787,  1795,  1801,  1808,  1818,  1824,
    1831,  1837,  1849,  1855,  1863,  1867,  1873,  1879,  1885,  1892,
    1901,  1907,  1915,  1922,  1929,  1938,  1949,  1958,  1971,  1977,
    1985,  1995,  2000,  2005,  2010,  2015,  2020,  2025,  2030,  2035,
    2040,  2045,  2051,  2057,  2064,  2071,  2079,  2085,  2092,  2103,
    2114,  2120,  2128,  2137,  2148,  2155,  2162,  2169,  2176,  2184,
    2191,  2198,  2205,  2214,  2223,  2231,  2237,  2245,  2254,  2267,
    2276,  2283,  2292,  2301,  2308,  2318,  2328,  2340,  2350,  2360,
    2372,  2384,  2396,  2410,  2414,  2420,  2425,  2430,  2437,  2443,
    2451,  2457,  2465,  2471,  2479,  2485,  2495,  2502,  2511,  2518,
    2526,  2534,  2538,  2544,  2550,  2559,  2567,  2577,  2583,  2590,
    2596,  2605,  2611,  2619,  2625,  2631,  2637,  2645,  2652,  2660,
    2664,  2674,  2682,  2691,  2697,  2707,  2716,  2725,  2734,  2741,
    2749,  2759,  2776,  2788,  2805,  2811,  2822,  2832,  2850,  2862,
    2878,  2884,  2899,  2904,  2910,  2915,  2921,  2926,  2933,  2938,
    2943,  2948,  2955,  2963,  2974,  2978,  2984,  2993,  3006,  3012,
    3021,  3025,  3032,  3036,  3042,  3046,  3053,  3062,  3066,  3074,
    3078,  3086,  3090,  3096,  3104,  3108,  3117,  3121,  3128,  3136,
    3140,  3149,  3156,  3161,  3166,  3172,  3180,  3188,  3193,  3200,
    3208,  3213,  3230,  3235,  3252,  3271,  3276,  3293,  3310,  3327,
    3345,  3350,  3367,  3386,  3391,  3408,  3425,  3442,  3461,  3466,
    3483,  3500,  3517,  3536,  3541,  3559,  3564,  3582,  3587,  3605,
    3610,  3617,  3632,  3651,  3656,  3673,  3677,  3681,  3685,  3691,
    3697,  3704,  3710,  3716,  3722,  3729,  3736,  3743,  3750,  3756,
    3763,  3770,  3777,  3784,  3791,  3797,  3808,  3819,  3825,  3832,
    3838,  3844,  3851,  3859,  3866,  3872,  3878,  3884,  3890,  3897,
    3904,  3911,  3918,  3925,  3932,  3942,  3948,  3957,  3981,  3990,
    3996,  4003,  4008,  4013,  4018,  4023,  4028,  4033
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_CONST", "TOK_TYPE", "TOK_VAR", 
  "TOK_SEMICOLON", "TOK_IDENTIFIER", "TOK_EQ", "TOK_NE", "TOK_LT", 
  "TOK_LE", "TOK_GT", "TOK_GE", "TOK_THREADVAR", "TOK_PLUS", "TOK_MINUS", 
  "TOK_STAR", "TOK_SLASH", "TOK_STRING", "TOK_REAL", "TOK_INTEGER", 
  "TOK_HEX", "TOK_ARRAY", "TOK_LBRACKET", "TOK_RBRACKET", "TOK_OF", 
  "TOK_RECORD", "TOK_END", "TOK_LPAREN", "TOK_RPAREN", "TOK_DOTDOT", 
  "TOK_COLON", "TOK_COMMA", "TOK_ASSIGN", "TOK_REPEAT", "TOK_UNTIL", 
  "TOK_WHILE", "TOK_DO", "TOK_AS", "TOK_IF", "TOK_THEN", "TOK_ELSE", 
  "TOK_FOR", "TOK_TO", "TOK_DOWNTO", "TOK_RAISE", "TOK_CASE", "TOK_BEGIN", 
  "TOK_AND", "TOK_OR", "TOK_XOR", "TOK_ON", "TOK_IN", "TOK_DIV", 
  "TOK_MOD", "TOK_NOT", "TOK_PROGRAM", "TOK_DOT", "TOK_WITH", "TOK_SHL", 
  "TOK_SHR", "TOK_FUNCTION", "TOK_PROCEDURE", "TOK_FORWARD", "TOK_TRUE", 
  "TOK_FALSE", "TOK_USES", "TOK_CDECL", "TOK_DYNAMIC", "TOK_VIRTUAL", 
  "TOK_LABEL", "TOK_EXPORT", "TOK_EXTERNAL", "TOK_FAR", "TOK_OVERRIDE", 
  "TOK_OVERLOAD", "TOK_PASCAL", "TOK_MESSAGE", "TOK_REINTRODUCE", 
  "TOK_SET", "TOK_SAFECALL", "TOK_STDCALL", "TOK_OUT", "TOK_FILE", 
  "TOK_PACKED", "TOK_NIL", "TOK_OBJECT", "TOK_INHERITED", "TOK_GOTO", 
  "TOK_CLASS", "TOK_PROTECTED", "TOK_PUBLIC", "TOK_PRIVATE", 
  "TOK_PUBLISHED", "TOK_PROPERTY", "TOK_AT", "TOK_READ", "TOK_WRITE", 
  "TOK_DEFAULT", "TOK_STORED", "TOK_NODEFAULT", "TOK_IMPLEMENTS", 
  "TOK_INTERFACE", "TOK_UNIT", "TOK_IMPLEMENTATION", "TOK_CONSTRUCTOR", 
  "TOK_DESTRUCTOR", "TOK_IS", "TOK_EXCEPT", "TOK_TRY", "TOK_FINALLY", 
  "TOK_INITIALIZATION", "TOK_FINALIZATION", "TOK_ABSTRACT", 
  "TOK_ASSEMBLER", "TOK_DEREF", "TOK_LAND", "TOK_LOR", "TOK_ADDR", 
  "TOK_DISPID", "TOK_DISPINTERFACE", "TOK_READONLY", "TOK_WRITEONLY", 
  "TOK_EXPRESSION", "TOK_PROCEDURE_EXPRESSION", "PREC_UNARYOP", 
  "PREC_LOWERTHANELSE", "TOK_FINAL_TOKEN", "$accept", "goal", 
  "global_expression", "global_procedure", "script_item", "script", 
  "script_item_list", "program", "program_header", 
  "opt_program_parameters", "identifier_list", "program_block", 
  "uses_clause", "block", "implementation_declarations", "unit", 
  "interface_section", "implementation_section", "opt_semicolon", 
  "init_section", "directive_el", "interface_declarations", 
  "routine_definition", "function_definition", "external_name", 
  "opt_external", "procedure_definition", "constructor_definition", 
  "destructor_definition", "function_header", "procedure_header", 
  "constructor_header", "destructor_header", "opt_parameter_list", 
  "parameter_list", "formal_parameter", "param_identifier_list", 
  "parameter", "function_return", "const_definition_list", 
  "type_declaration_list", "list_var_declaration", "typed_constant", 
  "record_constant", "record_field_constant_list", 
  "record_field_constant", "typed_constant_list", "array_constant", 
  "const_definition", "restricted_type", "type_declaration", 
  "var_declaration", "opt_packed", "struct_type", "method_type", 
  "type_spec", "defptr_list", "simple_type", "simple_type_list", 
  "opt_field_list", "field_list", "field_declaration", 
  "varfield_declaration", "variant_section", "rec_variant_list", 
  "rec_variant", "statement", "statement_raise", "statement_with", 
  "with_list", "statement_try", "try_handler", "except_handler", 
  "handler_list", "handler", "statement_assign", "statement_repeat", 
  "statement_while", "statement_if", "statement_for", "statement_case", 
  "semicolon_list", "compound_statement", "statement_list", 
  "list_case_option", "case_item_list", "case_item", "case_option", 
  "object_type", "object_inheritance", "object_body", "obj_field_list", 
  "method_list", "directive_list", "method_heading", "class_type", 
  "class_body", "class_heritage", "class_items_list", 
  "class_items_section", "class_method_heading", "class_visibility", 
  "property_definition", "opt_property_interface", "property_specifiers", 
  "identifier_rec_list", "opt_readonly", "opt_writeonly", "opt_dispid", 
  "opt_read", "opt_write", "opt_stored", "opt_index", "opt_default", 
  "opt_implements", "interface_type", "opt_guid", "interface_body", 
  "as_expression", "shift_expression", "multiplicative_expression", 
  "additive_expression", "relational_expression", "equality_expression", 
  "and_expression", "exclusive_or_expression", "inclusive_or_expression", 
  "logical_and_expression", "logical_or_expression", "range_list", 
  "simple_expression", "expression", "proc_func_expression", 
  "expression_list", "record_expression", "array_elt_expression", 
  "array_expression", "simple_const", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   129,   130,   130,   130,   130,   130,   131,   132,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     135,   135,   135,   136,   137,   138,   138,   139,   139,   140,
     140,   141,   142,   142,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   144,   145,
     145,   145,   145,   146,   146,   146,   146,   147,   147,   148,
     148,   148,   148,   148,   148,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   151,   151,   151,
     151,   152,   152,   152,   152,   152,   152,   153,   153,   154,
     154,   155,   155,   155,   155,   155,   155,   156,   156,   157,
     157,   158,   158,   158,   158,   158,   158,   159,   159,   159,
     159,   159,   159,   160,   160,   161,   161,   162,   162,   162,
     163,   163,   164,   164,   164,   164,   165,   165,   165,   165,
     166,   166,   166,   166,   166,   167,   167,   168,   168,   169,
     169,   169,   169,   169,   169,   170,   170,   170,   170,   171,
     171,   171,   172,   173,   173,   174,   175,   175,   176,   177,
     177,   178,   178,   178,   179,   179,   180,   181,   181,   182,
     182,   182,   182,   183,   183,   183,   183,   183,   183,   183,
     184,   184,   184,   185,   185,   186,   186,   186,   186,   186,
     186,   186,   187,   187,   188,   188,   188,   188,   188,   188,
     189,   189,   190,   191,   191,   191,   192,   192,   193,   193,
     194,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   196,   196,   196,   197,
     198,   198,   199,   199,   200,   200,   200,   200,   200,   201,
     201,   201,   201,   201,   201,   202,   202,   203,   203,   204,
     205,   205,   206,   207,   207,   208,   208,   209,   209,   209,
     209,   209,   209,   210,   210,   211,   211,   211,   212,   212,
     213,   213,   214,   214,   215,   215,   216,   216,   217,   217,
     218,   219,   219,   219,   219,   220,   220,   221,   221,   221,
     221,   222,   222,   223,   223,   223,   223,   224,   224,   225,
     225,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   229,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   231,   231,   232,   232,   232,   233,   234,   234,
     235,   235,   236,   236,   237,   237,   237,   238,   238,   239,
     239,   240,   240,   240,   241,   241,   242,   242,   242,   243,
     243,   244,   244,   244,   244,   244,   245,   246,   246,   246,
     247,   247,   248,   248,   248,   249,   249,   249,   249,   249,
     250,   250,   250,   251,   251,   251,   251,   251,   252,   252,
     252,   252,   252,   253,   253,   254,   254,   255,   255,   256,
     256,   256,   256,   257,   257,   258,   258,   258,   258,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   260,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   262,   262,   263,   264,   265,
     265,   266,   266,   266,   266,   266,   266,   266
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     2,     2,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       1,     3,     3,     4,     3,     0,     3,     1,     3,     2,
       1,     3,     2,     1,     2,     2,     2,     3,     2,     1,
       2,     3,     3,     3,     4,     3,     2,     3,     7,     1,
       3,     2,     2,     1,     2,     2,     3,     0,     1,     4,
       7,     4,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     2,
       2,     2,     2,     2,     3,     3,     5,     6,     4,     2,
       3,     3,     4,     3,     3,     3,     3,     3,     6,     4,
       7,     4,     5,     3,     4,     4,     5,     2,     2,     2,
       2,     3,     2,     4,     3,     3,     3,     1,     1,     3,
       5,     3,     2,     4,     3,     3,     3,     3,     4,     3,
       4,     4,     4,     4,     6,     6,     6,     3,     3,     3,
       5,     5,     5,     3,     5,     3,     5,     0,     3,     2,
       1,     3,     2,     2,     2,     1,     1,     1,     3,     3,
       1,     3,     5,     5,     5,     0,     2,     2,     3,     2,
       3,     4,     3,     4,     5,     2,     3,     3,     4,     1,
       1,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       5,     1,     1,     1,     3,     3,     1,     0,     1,     7,
       4,     4,     4,     2,     3,     4,     4,     5,     3,     2,
       1,     1,     1,     1,     2,     1,     3,     4,     2,     2,
       3,     3,     1,     3,     0,     1,     2,     1,     2,     3,
       1,     3,     1,     3,     5,     7,     4,     6,     2,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     2,     1,     2,     4,     4,
       1,     3,     4,     5,     2,     3,     3,     4,     1,     2,
       3,     2,     4,     5,     1,     2,     3,     4,     6,     3,
       4,     5,     4,     4,     6,     8,     8,     5,     6,     7,
       8,     8,     9,     1,     2,     3,     4,     2,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     2,     4,     3,
       3,     0,     1,     1,     2,     4,     5,     2,     3,     3,
       4,     2,     3,     1,     1,     1,     1,     4,     3,     0,
       1,     3,     1,     1,     2,     2,     2,     2,     3,     2,
       2,     5,     3,     6,     1,     4,     3,     6,     4,     7,
       2,     5,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     5,     7,     0,     2,     5,     9,     1,     3,
       0,     1,     0,     1,     0,     2,     3,     0,     2,     0,
       2,     0,     2,     2,     0,     2,     0,     2,     1,     0,
       2,     3,     1,     1,     2,     2,     3,     1,     3,     2,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       2,     3,     4,     1,     3,     3,     1,     5,     3,     1,
       2,     1,     1,     1,     2,     2,     2,     2,     1,     2,
       2,     2,     2,     2,     1,     3,     4,     1,     2,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     4,     4,
       4,     4,     2,     4,     5,     3,     1,     3,     4,     3,
       2,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,     0,     0,     0,   439,     0,     0,   484,   483,   482,
     481,     0,     0,     0,     0,     0,     0,   256,     0,     0,
       0,     0,     0,     0,   485,   486,     0,   441,   487,   252,
       0,   448,   443,   442,     0,     0,     0,     0,     0,     0,
       0,     0,     5,     6,    20,     4,     0,     2,     0,    13,
       3,    15,    14,    16,    17,     0,     0,     0,     0,     9,
     241,   242,   250,   243,   244,   245,   246,   247,   248,   249,
     251,   457,   462,   460,   459,   454,     0,    11,     0,    12,
     156,   157,     0,    10,   196,     0,   255,   213,   440,   450,
     453,   449,   452,   439,   480,     0,     0,   402,   405,   410,
     413,   418,   423,   425,   427,   429,   433,   463,     0,   400,
     436,     0,   298,     0,     0,     0,     0,   257,     0,   297,
       0,    25,     0,   260,   147,   147,   147,   147,   147,   147,
      27,     0,   444,   472,   253,     0,   447,     0,   446,   445,
       0,   147,   147,     0,   451,     7,     8,     0,     0,     0,
       0,     1,     0,   293,    19,     0,     0,   112,     0,     0,
     122,     0,     0,     0,     0,     0,     0,     0,     0,   458,
       0,   461,     0,   197,   197,   197,     0,   464,   476,     0,
     254,   214,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   479,     0,     0,   455,   293,
       0,     0,     0,     0,     0,     0,     0,   295,     0,     0,
      24,     0,     0,     0,     0,   165,   165,   165,     0,   137,
     139,   138,    31,     0,     0,   466,     0,   465,     0,     0,
       0,   143,     0,   145,   293,   274,     0,     0,   268,   107,
     108,   109,   110,    22,   294,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,    39,    33,   116,    65,
      73,    71,    66,     0,    67,    74,    76,    68,    75,    69,
      70,    72,   115,     0,   111,   321,     0,   114,   126,   125,
     121,     0,   124,   127,     0,   129,     0,     0,   279,   477,
     467,     0,   189,     0,   215,     0,   147,   147,   198,     0,
       0,   211,   212,     0,     0,   210,     0,   311,   329,   392,
     393,   195,   194,   193,   191,   192,   233,   158,   159,   468,
       0,   401,   403,   404,   406,   407,   409,   408,   411,   412,
     415,   416,   414,   417,   419,   420,   422,   421,   424,   426,
     428,     0,   431,   434,   438,   435,   456,     0,   280,   299,
     282,   283,     0,   258,     0,     0,   302,   300,   304,   296,
       0,   261,   259,     0,     0,   149,     0,     0,   150,   160,
     155,   147,   147,   147,     0,   131,   132,   133,   147,   147,
     147,    28,   473,     0,   470,   469,    49,     0,   147,   147,
       0,     0,   271,     0,   269,     0,   264,   262,    34,     0,
       0,    35,     0,    36,     0,    38,     0,    40,    23,    29,
       0,     0,     0,     0,     0,     0,    46,    32,   117,   118,
     119,     0,    77,   113,   322,   123,   128,   130,   478,   471,
     218,     0,     0,   439,     0,   165,   203,     0,     0,   209,
       0,   224,     0,     0,   219,     0,     0,     0,   323,   324,
     325,   326,   311,     0,   312,   313,     0,     0,   359,   358,
     361,   360,     0,     0,   352,   354,   356,   357,     0,   209,
     330,   332,     0,   333,   344,     0,   397,     0,     0,   394,
     395,     0,   475,   432,     0,   281,     0,     0,     0,     0,
     287,     0,   307,     0,     0,    26,   153,   152,   154,     0,
     148,     0,   165,   165,   165,   166,   140,   141,   142,   474,
       0,     0,     0,     0,    52,    51,     0,     0,    53,     0,
     144,   146,   263,     0,     0,     0,   275,   293,   265,   293,
       0,   167,   197,     0,     0,   169,     0,     0,   175,     0,
      37,    41,    42,    43,    45,     0,    47,     0,    78,     0,
     216,   220,   204,     0,   208,     0,     0,   197,     0,     0,
     225,   230,   232,   227,     0,     0,   190,   180,   181,   179,
     221,     0,   197,     0,   309,     0,   314,     0,   317,     0,
     353,   355,   335,   334,   337,   336,   364,   197,   328,     0,
     359,   358,   361,   360,   339,     0,     0,   350,   340,     0,
       0,   391,   399,     0,     0,   234,   437,   284,     0,     0,
     288,     0,   301,     0,   306,   303,   305,   151,     0,   161,
     134,   135,   136,    79,    80,    81,    82,    50,     0,     0,
       0,     0,     0,     0,    89,     0,     0,    83,     0,     0,
      54,    55,    64,     0,     0,     0,     0,     0,   272,   276,
     293,   168,   329,   197,   172,     0,   170,   177,     0,   176,
      44,   120,   217,     0,   205,   206,   331,   222,     0,   200,
     439,     0,   201,   226,   228,   202,   439,   186,     0,   183,
       0,   179,   310,     0,   308,   197,   319,     0,   318,     0,
       0,   384,     0,   327,   338,   197,   346,     0,     0,   342,
     396,   398,   476,     0,     0,     0,     0,   293,   289,     0,
       0,    93,    94,    95,    96,   103,     0,     0,    97,     0,
       0,    90,    91,     0,     0,    84,    85,     0,    56,    57,
      62,     0,    57,    48,     0,   277,   293,   171,   329,   173,
     178,   207,     0,     0,     0,     0,   231,   229,     0,     0,
     182,   188,     0,   315,     0,   320,     0,   365,     0,     0,
     377,   345,     0,     0,   348,     0,   235,   285,   286,   293,
     290,   291,   164,   163,   162,   105,   104,     0,     0,   101,
      99,     0,    92,     0,    88,   293,     0,    63,     0,     0,
     174,   197,   223,     0,   236,     0,     0,   185,     0,   184,
     187,   316,     0,   385,   362,     0,   379,   351,     0,   341,
       0,   292,   106,     0,   102,    86,     0,    61,    59,     0,
     278,   199,     0,     0,   238,     0,     0,     0,   368,   378,
       0,   381,   347,     0,   343,    98,     0,    87,    57,   237,
     239,   224,   366,   363,     0,   380,     0,   370,   349,   100,
       0,     0,   369,   382,   383,   371,   372,    60,   240,   373,
     374,     0,   386,     0,   375,     0,   388,   389,   376,   387,
       0,   367,   390
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    41,    42,    43,    44,    45,    46,    47,    48,   220,
     473,   262,    49,   264,   265,    50,   397,   529,   796,   655,
     285,   525,   266,   147,   430,   157,   148,   149,   150,    55,
      56,    57,    58,   225,   377,   378,    82,   380,   385,   408,
     411,   413,   576,   577,   688,   689,   690,   578,   409,   321,
     412,   414,   310,   311,   312,   322,   314,   315,   678,   569,
     570,   571,    84,   573,   804,   805,   112,    60,    61,   122,
      62,   247,   248,   402,   403,    63,    64,    65,    66,    67,
      68,   211,    69,   113,   364,   365,   366,   367,   323,   462,
     463,   464,   465,   158,   466,   324,   478,   479,   480,   481,
     482,   483,   484,   701,   769,   839,   866,   870,   872,   816,
     841,   857,   770,   877,   881,   325,   488,   489,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   178,    71,   806,    72,    73,    74,    75
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -736
static const short yypact[] =
{
    1047,   171,   218,    55,    91,   656,   706,  -736,  -736,  -736,
    -736,  1364,  2100,  2551,  2100,  2100,   254,  2100,  2100,  2381,
     332,  2951,    74,   133,  -736,  -736,   338,   195,  -736,   361,
     375,    53,    96,   195,   487,   513,   536,  2551,  2951,  2100,
     290,   326,  -736,  -736,  -736,  -736,   472,  -736,   348,  -736,
    -736,  -736,  -736,  -736,  -736,   539,   541,   548,   562,  -736,
    -736,  -736,  -736,  -736,  -736,  -736,  -736,  -736,  -736,  -736,
     421,   195,  -736,   105,  -736,  -736,   319,  -736,   507,  -736,
    -736,  -736,    63,  -736,  -736,  2270,  2551,  -736,   406,  -736,
    -736,  -736,  -736,   160,  -736,  2951,   361,   540,   449,   409,
     455,   632,    89,   467,   473,   542,   553,   567,   304,    68,
     615,   603,  -736,    54,   610,   609,   622,   605,   648,  -736,
     271,   666,   346,   540,    46,   669,   669,    64,   669,   669,
    -736,   242,   406,   673,  -736,  2442,   406,  2638,   406,   406,
     698,    81,   214,    26,    68,  -736,  -736,   704,   709,   710,
     711,  -736,  1672,  -736,  1564,   299,   866,  -736,   712,  1125,
    -736,   717,  1483,   718,  1483,   723,  2100,  2100,  2951,   406,
    2693,   406,  2100,  1800,  1201,  1800,   240,  -736,  -736,   212,
    -736,  -736,   542,  2951,  2951,  2951,  2951,  2951,  2951,  2951,
    2951,  2951,  2951,  2951,  2951,  2951,  2951,  2951,  2951,  2951,
    2951,  2951,  2951,  2185,  2100,  -736,  2100,  2100,   195,   676,
    2100,  2124,  2551,  2551,  2100,  2100,  2100,  -736,  2039,   338,
    -736,  2951,  2551,   516,   155,   701,   701,   701,   219,  -736,
    -736,  -736,  -736,   727,  2731,  -736,   410,  -736,   511,   635,
     737,  -736,   742,  -736,     4,  2466,  2209,   722,  -736,  -736,
    -736,  -736,  -736,  -736,  -736,  -736,   171,   745,    55,    55,
     338,   290,   695,   524,  -736,   825,  -736,  -736,  -736,  -736,
    -736,  -736,  -736,   273,  -736,  -736,  -736,  -736,  -736,  -736,
    -736,  -736,  -736,   398,  -736,  -736,  1403,  -736,  -736,  -736,
    -736,  1403,  -736,  -736,  1483,  -736,  1483,   347,  -736,    68,
    -736,   519,  -736,   749,   985,  2779,   669,   669,  -736,   527,
     209,  -736,  -736,   750,    44,  -736,   445,    87,  2002,   508,
     404,  -736,  -736,  -736,  -736,  -736,   751,  -736,  -736,  -736,
    2100,    68,   540,   540,   449,   449,   449,   449,   409,   409,
     455,   455,   455,   455,   632,   632,   632,   632,    89,   467,
     473,  2951,   542,   553,   729,  -736,   406,  2100,  -736,  -736,
    -736,   719,   518,  -736,   396,   568,  -736,  -736,   731,  -736,
     543,   540,  -736,    55,    55,  -736,    55,   327,  -736,   576,
    -736,   669,   669,   669,  1913,  -736,  -736,  -736,   669,   669,
     669,  -736,  -736,   545,  -736,  -736,    69,   662,   669,   669,
     740,   768,    77,   770,   771,  2551,   774,  -736,   171,   783,
     782,   784,   788,    55,   789,    55,   252,  -736,  -736,  -736,
     171,   745,    55,    55,   338,   290,  -736,  -736,  -736,  -736,
     790,   779,  -736,  -736,  -736,  -736,  -736,  -736,  -736,  -736,
    -736,  2951,   794,    76,   580,   701,   776,   796,   338,  -736,
     269,   253,   780,  2817,  -736,  2951,   800,   591,  -736,  -736,
    -736,  -736,   176,   781,   176,   290,   804,     1,   328,   328,
     328,   328,   806,   606,  -736,  -736,  -736,  -736,   786,  1442,
     589,   328,   805,  -736,  -736,   797,  -736,   404,  1574,  -736,
    -736,  2865,  -736,   542,  2100,  -736,  2551,  2100,  2100,  2599,
    -736,  2551,  2551,  2100,  2100,  -736,  -736,  -736,  -736,   333,
    -736,  1889,   701,   701,   701,  -736,  -736,  -736,  -736,  -736,
     171,   745,    55,    55,   463,   602,   809,   811,   293,    52,
    -736,  -736,  -736,   246,  2551,   812,  -736,   791,   819,   798,
     821,  -736,  1253,   824,   830,  3127,   839,   840,  3127,   841,
    -736,   171,   784,    55,    55,   268,  -736,   273,  -736,   515,
    -736,  -736,  3029,   761,  -736,   595,  1913,  1800,  1937,   823,
      65,  -736,  -736,  -736,  1913,  2903,  -736,  -736,  -736,  -736,
     455,   826,  1800,   827,  -736,   628,   290,   843,  3127,   847,
    -736,  -736,   328,   328,   328,   328,   353,  1800,  -736,   829,
    -736,  -736,  -736,  -736,   328,   634,   852,  -736,  3078,   854,
     837,  -736,  -736,   420,  2100,  -736,  -736,  -736,   834,   836,
    -736,  2551,  -736,   397,  -736,  -736,  -736,  -736,   842,   855,
    -736,  -736,  -736,   171,   784,    55,    55,   602,   171,   745,
      55,    55,   859,   869,  3093,   870,   871,  3044,   873,   876,
     578,   760,  -736,  2551,  2294,   828,   877,  2551,   879,  -736,
     861,  -736,  2950,  1338,  3127,   884,  3127,  3127,   885,  3127,
    -736,  -736,  -736,   807,  -736,  -736,  -736,  -736,   387,  -736,
     349,   857,  -736,   253,  -736,  -736,    92,  -736,   368,  -736,
     599,   603,  -736,   889,  -736,  1800,  3127,   894,  3127,   333,
     906,   910,   913,  -736,   328,  1800,  3108,   914,   915,  3127,
    -736,  -736,   603,   624,  2551,  2551,   446,   895,  -736,  1773,
    2100,   171,   784,    55,    55,  3093,   918,   920,  3059,   921,
     927,  -736,  3093,   938,   939,  -736,  3093,   943,   760,   944,
    -736,   923,   944,  -736,   916,  -736,   924,  -736,  2989,  3127,
    3127,  -736,   929,  1913,  1913,  2100,  -736,  -736,  2817,   946,
    -736,  -736,  2817,  -736,   951,  3127,    78,  -736,  2100,   952,
     863,  -736,   957,   962,  3127,   958,  -736,  -736,  -736,   942,
    -736,  -736,  -736,  -736,  -736,  -736,  3093,   965,   967,  -736,
    3093,   969,  -736,   970,  -736,    30,   954,  -736,    31,  2551,
    -736,  1800,  -736,   959,  2100,   977,   661,  -736,   955,  -736,
    -736,  -736,   956,  -736,   890,   987,   886,  -736,   989,  3127,
     990,  -736,  -736,   991,  -736,  3127,   992,  -736,  -736,  2551,
    -736,  -736,  2100,   993,  -736,   975,   998,  1000,  -736,   949,
     987,   908,  3127,  1005,  3127,  3127,  1006,  3127,   944,  2100,
    -736,   253,  -736,  -736,  1008,   949,   971,   891,  3127,  3127,
     994,   988,  -736,  -736,  -736,  -736,   896,  -736,  -736,  -736,
     897,   469,   358,   999,  -736,  2100,  -736,   925,  -736,  -736,
    1018,  -736,  -736
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -736,  -736,  -736,  -736,   381,  -736,  -736,  -736,  -736,  -736,
      27,  -736,  -141,   294,  -482,  -736,  -736,  -736,  -690,  -736,
      34,   502,   -24,    48,   471,   -31,    57,    61,    70,   373,
     437,  -200,   317,   -40,   334,   525,  -194,   124,  -191,  -379,
    -366,  -232,  -552,  -736,  -736,   279,  -736,  -736,    14,  -736,
      19,    16,  -736,  -736,  -736,  -128,     6,  -203,  -736,   191,
    -736,   364,  -434,  -520,   216,  -735,    22,  -736,  -736,  -736,
    -736,   814,  -736,  -736,   642,  -736,  -736,  -736,  -736,  -736,
    -736,     8,   150,   -17,  -736,  -736,   546,   554,  -736,  -736,
     593,  -736,   592,   -45,  -416,  -736,   581,    15,   571,  -403,
    -447,  -436,  -441,  -736,  -736,   221,  -736,  -736,  -736,  -736,
    -736,  -736,  -736,  -736,  -736,  -736,  -736,  -289,    35,   483,
     498,    -8,   509,   864,   872,   868,   -65,   874,  -736,  -736,
       5,   -11,  -736,   -59,  -736,  -736,  -736,     3
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -440
static const short yytable[] =
{
     110,   111,   120,   114,   115,    70,   117,   118,    90,    92,
      88,   161,   163,   165,   263,    77,   146,   572,    70,    83,
     143,    79,    59,   687,    70,   160,   179,   415,   145,   379,
     182,   490,   244,   132,   606,   386,   387,   136,   138,   139,
     607,   551,    70,   144,   604,   313,   651,   326,    51,   587,
     684,   454,   798,   131,   154,   552,   123,    52,   -58,   828,
     209,    53,    80,    22,    23,   592,   593,   594,   595,   833,
      54,   683,   520,   521,   522,   223,   236,   169,   238,   171,
     652,   124,   135,   523,   509,   226,   227,   229,   230,   231,
     210,    70,   166,   223,   130,   175,   176,   196,   197,    88,
     653,   241,   243,   812,   224,    85,   -27,   297,   180,   -27,
     223,   301,   568,   245,   833,   246,   456,   460,   476,   534,
      85,    85,   228,    86,   758,   137,   168,   287,   218,   401,
     292,    22,    23,    81,   170,   245,    26,   246,   352,   240,
     127,   633,   198,   -58,   829,   606,   606,   606,   606,    22,
      23,   607,   607,   607,   607,   634,   298,    70,   860,    70,
     181,   302,   381,   757,   654,   316,   316,   316,   738,    87,
     587,   125,   126,   299,    59,   393,    59,   604,    76,   379,
     379,   515,   379,   130,   340,   341,   342,   343,   331,    85,
     553,   554,    87,    35,    36,   354,   355,   199,   611,   358,
      51,   704,    51,   362,   363,   368,   807,    87,    87,    52,
     810,    52,    87,    53,   356,    53,    70,    70,    70,   332,
     333,    87,    54,    70,    54,    78,   388,    70,   404,   406,
     128,   129,   450,   359,   360,   361,   451,   417,    22,    23,
     359,   426,   329,   223,   372,   330,   370,   327,   232,   572,
      70,    70,   382,   383,   562,   524,   371,   606,   550,   721,
      80,   116,   460,   607,   460,   460,   445,   446,   476,   476,
     476,   476,   242,   722,   670,   233,    87,   153,   656,   476,
     428,   476,    35,    36,   657,   233,   493,   416,   476,   452,
     635,   636,   429,   566,   111,   567,   256,   257,   258,   217,
     568,   233,   256,   257,   258,   267,   267,   259,   629,   267,
      88,    87,   267,   259,   267,   379,   389,   390,   328,   492,
     434,   630,   631,   632,   449,   434,   151,   172,   434,   205,
     434,    81,   444,   509,   487,   130,   373,   206,   374,   121,
      80,   512,   513,   514,   457,   130,   495,    19,   516,   517,
     518,   173,    22,    23,   155,    22,    23,   510,   530,   531,
      26,    22,    23,   677,   260,   681,    26,   546,   133,   549,
     260,   685,   438,   441,   759,  -215,   316,   699,    85,   221,
     330,   754,   134,   261,   222,   700,   460,   650,   538,   261,
      22,    23,   476,   476,   476,   476,    35,    36,   760,    35,
      36,   556,   499,   717,   476,    35,    36,   442,   723,   724,
      70,    81,   752,   267,   431,   427,   376,   572,   467,   432,
     753,   589,   540,   472,   500,   718,   186,   187,   485,   547,
     544,   547,   486,   559,    35,    36,   267,   609,   501,   679,
     394,   267,   579,   330,   267,   166,   267,   580,   711,    88,
     284,   555,   779,   290,   693,   167,   293,   875,   295,   876,
     190,   191,   476,   188,   189,    87,   520,   521,   522,   702,
     190,   191,   -18,   152,   780,   565,   455,   523,   153,   168,
     615,   646,   649,   616,   623,   873,   618,   619,   368,   457,
     874,   585,   368,   626,   140,   645,   648,   506,   507,   665,
     508,    70,   668,   316,   476,   379,    70,    70,   605,   184,
     185,   600,   601,   602,   603,   174,   783,   658,   617,   373,
     141,   374,   181,    80,   624,    22,    23,   256,   257,   258,
     190,   191,   485,   253,   316,   255,   486,   448,   259,    70,
     672,   395,   697,   142,   330,   156,   375,   159,   476,   439,
     802,   803,   330,   447,   162,   713,   448,   419,   316,   316,
     316,   707,   497,   498,   691,   540,   316,   764,   164,   547,
     547,   544,    19,   505,   316,   519,   233,   772,   330,   183,
     433,   256,   257,   258,   200,   435,    22,    23,   436,   316,
     437,   201,   259,   202,    81,   260,   674,   727,   730,   376,
     502,   503,   203,   712,   716,   638,   639,   640,   511,   176,
     561,   726,   729,   233,   261,   733,   641,   204,   737,   605,
     605,   605,   605,   582,   233,   676,    70,   426,   233,   761,
      35,    36,   762,   208,   461,   477,   739,   742,   597,   233,
      22,    23,   192,   193,   194,   195,   207,   540,   212,   260,
     213,   547,   547,   544,   776,   316,   214,   330,    70,    70,
     695,   233,    70,    89,    22,    23,   705,   233,   261,   334,
     335,   336,   337,   831,   216,     7,     8,     9,    10,   745,
     600,   601,   602,   603,    35,    36,    88,   316,   338,   339,
     458,   474,    88,   835,   330,   219,   787,   316,   223,   791,
     434,   215,   234,   434,   239,   344,   345,   346,   347,   784,
     249,   316,   357,    91,   426,   250,   251,   252,   286,    70,
      70,    24,    25,   291,   294,     7,     8,     9,    10,   296,
     820,   605,   434,   384,   391,   540,   777,   778,   396,   547,
     547,   544,    28,   434,   398,   316,   316,   579,   826,   399,
     407,   579,   410,   418,   459,   475,   440,   813,   453,   491,
     494,   496,   504,   420,   421,   422,   434,   528,   532,   526,
     434,    24,    25,   843,   423,   533,   536,   537,   846,   461,
     539,   461,   461,   434,   434,   477,   477,   477,   477,   541,
     542,   543,    28,   316,   545,   548,   477,   557,   477,   434,
     558,   560,   563,   564,    70,   477,   574,   581,   434,   584,
     588,   608,   848,   596,   598,   644,   610,   647,   659,  -270,
     434,   830,    22,    23,   434,   660,  -266,   661,   420,   421,
     422,   424,   663,   527,    70,   458,   664,   458,   458,   423,
     590,   474,   474,   474,   474,   666,   667,   669,   675,   696,
     425,   682,   474,   698,   474,   694,   692,   703,   706,   864,
     709,   474,   710,   720,   879,   725,    35,    36,   719,   256,
     257,   258,   714,    19,   715,   728,   731,   732,   434,   735,
     259,   434,   736,   755,   744,   746,   743,    22,    23,  -267,
     749,   750,   434,   434,   751,   763,   424,   526,   642,   459,
     765,   459,   459,   461,   591,   475,   475,   475,   475,   477,
     477,   477,   477,   767,    19,   425,   475,   768,   475,   771,
     774,   477,   775,   781,   785,   475,   786,   789,    22,    23,
     268,    35,    36,   790,   269,   270,   271,   260,   272,   273,
     274,   275,   276,   277,   792,   278,   793,   279,   280,   794,
     795,   797,  -273,   808,   799,   801,   261,   811,   814,   458,
     815,   527,   643,   817,   819,   474,   474,   474,   474,   818,
     821,   822,    35,    36,   823,   824,   825,   474,   863,   477,
     281,   282,   827,   834,   840,   832,   283,   758,   836,   837,
       7,     8,     9,    10,   838,   842,   844,   845,   847,   850,
    -439,  -439,  -439,  -439,   851,   852,   853,   854,   856,   441,
     642,   858,   859,   865,    85,   862,  -439,   871,   868,   869,
     878,   477,   867,   459,  -439,   882,   637,   880,   671,   475,
     475,   475,   475,   766,   627,   474,    24,    25,   809,  -439,
    -439,   475,   861,   442,   535,  -439,  -439,   756,   849,   625,
       1,     2,     3,   622,     4,   583,   586,    28,   400,   613,
     599,   855,     5,     6,   348,   477,     7,     8,     9,    10,
     350,    11,     0,   349,   643,     0,    12,   474,   353,     0,
       0,     0,    13,     0,    14,     0,     0,    15,     0,     0,
      16,     0,     0,    17,    18,    19,     0,     0,     0,   475,
       0,    87,     0,     0,    20,     0,    21,     0,     0,    22,
      23,     0,    24,    25,    26,     0,     0,     0,     0,     0,
       0,   474,     0,     0,     0,    27,     0,     0,   256,   257,
     258,     0,     0,    28,     0,    29,    30,     0,     0,   259,
       0,   475,     0,     0,    31,    32,    33,     0,     0,     0,
       0,    34,     0,    35,    36,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,    39,    40,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   475,     0,    22,    23,   288,
       0,     0,     0,   269,   270,   271,   260,   272,   273,   274,
     275,   276,   277,     0,   278,   303,   279,   280,   304,     0,
       0,     0,     0,     0,     0,   261,     5,     6,     0,     0,
       7,     8,     9,    10,     0,    11,     0,     0,     0,     0,
     305,    35,    36,     0,     0,     0,     0,     0,     0,   281,
     289,     0,     0,     0,     0,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
     304,     0,     0,   306,   307,     0,    24,    25,     5,     6,
       0,     0,     7,     8,     9,    10,     0,    11,     0,    27,
       0,     0,   305,     0,     0,     0,   308,    28,   317,    96,
       0,   318,     0,     0,     0,     0,     0,     0,    31,    32,
      33,     0,     0,     0,   319,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,   307,    87,    24,    25,
      38,     0,   320,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,   308,    28,
     317,    96,   303,   662,     0,   304,     0,     0,     0,     0,
      31,    32,    33,     5,     6,     0,   319,     7,     8,     9,
      10,     0,    11,     0,     0,     0,     0,   305,     0,    87,
       0,    93,    38,     0,   320,     0,     0,     0,     0,     5,
       6,     0,     0,     7,     8,     9,    10,     0,    11,    94,
       0,     0,     0,    12,     0,     0,     0,     0,     0,     0,
     306,   307,     0,    24,    25,     0,   256,   257,   258,     0,
       0,     0,     0,     0,     0,     0,    27,   259,     0,     0,
      95,     0,     0,   308,    28,   317,    96,     0,   748,    24,
      25,     0,     0,     0,     0,    31,    32,    33,     0,     0,
       0,   319,    27,     0,     0,     0,     0,     0,     0,   130,
      28,    19,    96,     0,    87,     0,     0,    38,     0,   320,
       0,    31,    32,    33,     0,    22,    23,     0,     0,     0,
    -329,   269,   270,   271,   260,   272,   273,   274,   275,   276,
     277,     0,   278,    38,   279,   280,   256,   257,   258,     0,
       0,     0,     0,   261,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,    35,
      36,     0,     0,     0,     0,     0,     0,   281,     0,     0,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
       0,    19,   467,   468,   469,   470,   471,   472,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,    35,    36,
       0,   269,   270,   271,   260,   272,     0,   274,   275,   276,
     277,     0,   278,     0,   279,   280,     0,     1,     2,     3,
     254,     4,     0,   261,     0,     0,     0,     0,     0,     5,
       6,   130,     0,     7,     8,     9,    10,     0,    11,    35,
      36,     0,     0,    12,     0,     0,     0,   281,     0,    13,
       0,    14,   612,   283,    15,     0,     0,    16,     0,     0,
      17,    18,    19,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,    22,    23,     0,    24,
      25,    26,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,     0,     0,
      28,     0,    29,    30,     0,     0,     0,     0,     0,     0,
       0,    31,    32,    33,   467,   468,   469,   470,   471,   472,
      35,    36,     0,     0,    37,     1,     2,     3,     0,     4,
      35,    36,     0,    38,     0,     0,     0,     5,     6,     0,
       0,     7,     8,     9,    10,     0,    11,     0,     0,     0,
       0,    12,     0,     0,     0,     0,     0,    13,     0,    14,
       0,     0,    15,     0,     0,    16,     0,     0,    17,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,    22,    23,     0,    24,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,     0,     0,     0,     0,     0,    28,     0,
      29,    30,     0,     0,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,     0,     0,   782,   303,    35,    36,
     304,     0,    37,     0,     0,     0,     0,     0,     5,     6,
       0,    38,     7,     8,     9,    10,     0,    11,     0,     0,
       0,     0,   305,     0,   303,     0,     0,   304,     0,     0,
       0,     0,     0,     0,     0,     5,     6,     0,     0,     7,
       8,     9,    10,     0,    11,     0,     0,     0,     0,   305,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,    28,
       0,    96,   306,   307,     0,    24,    25,     0,     0,     0,
      31,    32,    33,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,     0,     0,   308,    28,     0,    96,    87,
     309,     0,    38,   303,     0,     0,   304,    31,    32,    33,
       0,     0,     0,     0,     5,     6,     0,     0,     7,     8,
       9,    10,   628,    11,     0,     0,    87,   303,   305,    38,
     304,     0,     0,     0,     0,     0,     0,     0,     5,     6,
       0,     0,     7,     8,     9,    10,     0,    11,     0,     0,
       0,   303,   305,     0,   680,     0,     0,     0,     0,     0,
       0,     0,     5,     6,    24,    25,     7,     8,     9,    10,
       0,    11,     0,     0,     0,     0,   305,    27,     0,     0,
       0,     0,     0,     0,     0,    28,     0,    96,    24,    25,
       0,     0,     0,     0,     0,     0,    31,    32,    33,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,    28,
       0,    96,    24,    25,     0,    87,     0,     0,    38,   130,
      31,    32,    33,     0,     0,    27,     0,     0,     0,     0,
       0,     0,     0,    28,     0,    96,     0,     0,   447,    87,
       0,   448,    38,     0,    31,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,   254,     4,     0,     0,     0,
       0,     0,     0,    87,     5,     6,    38,     0,     7,     8,
       9,    10,     0,    11,    22,    23,     0,   369,    12,     0,
       0,     0,     0,     0,    13,     0,    14,     0,     0,    15,
       0,     0,    16,     0,     0,    17,    18,    19,     0,     0,
       0,     0,   467,   468,   469,   470,   471,   472,    21,     0,
       0,     0,     0,     0,    24,    25,     0,    93,    35,    36,
       0,     0,     0,     0,     0,     5,     6,    27,     0,     7,
       8,     9,    10,     0,    11,    28,     0,    29,    30,    12,
     254,     4,     0,     0,     0,     0,    31,    32,    33,     5,
       6,     0,     0,     7,     8,     9,    10,     0,    11,    37,
       0,     0,     0,    12,     0,     0,    95,     0,    38,    13,
       0,    14,     0,     0,    15,    24,    25,    16,     0,     0,
      17,    18,    19,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    21,     0,     0,    28,     0,    96,    24,
      25,     0,    93,     0,     0,     0,     0,    31,    32,    33,
       5,     6,    27,     0,     7,     8,     9,    10,     0,    11,
      28,     0,    29,    30,    12,   405,     4,     0,     0,    38,
       0,    31,    32,    33,     5,     6,     0,     0,     7,     8,
       9,    10,     0,    11,    37,     0,     0,     0,    12,     0,
       0,   351,     0,    38,    13,     0,    14,     0,     0,    15,
      24,    25,    16,     0,     0,    17,    18,    19,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,    21,     0,
       0,    28,     0,    96,    24,    25,     0,    93,     0,     0,
       0,     0,    31,    32,    33,     5,     6,    27,     0,     7,
       8,     9,    10,     0,    11,    28,     0,    29,    30,    12,
     177,     4,     0,     0,    38,     0,    31,    32,    33,     5,
       6,     0,     0,     7,     8,     9,    10,     0,    11,    37,
       0,     0,   740,    12,     0,     0,    95,     0,    38,    13,
       0,    14,     0,     0,    15,    24,    25,    16,     0,     0,
      17,    18,    19,     0,     0,     0,     0,     0,    27,     0,
       0,     0,     0,    21,     0,     0,    28,     0,    96,    24,
      25,     0,     0,     0,     0,     0,     0,    31,    32,    33,
       0,     0,    27,     0,     0,     0,     0,     0,     0,     0,
      28,     0,    29,    30,     0,     0,     0,     0,     4,    38,
       0,    31,    32,    33,     0,     0,     5,     6,     0,     0,
       7,     8,     9,    10,    37,    11,     0,   741,     0,   119,
      12,     0,     0,    38,     0,     0,    13,     0,    14,     0,
       0,    15,     0,     0,    16,     0,     0,    17,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,     0,    24,    25,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     5,     6,    27,
       0,     7,     8,     9,    10,     0,    11,    28,     0,    29,
      30,    12,   235,     4,     0,     0,     0,     0,    31,    32,
      33,     5,     6,     0,     0,     7,     8,     9,    10,     0,
      11,    37,     0,     0,     0,    12,     0,     0,    95,     0,
      38,    13,     0,    14,     0,     0,    15,    24,    25,    16,
       0,     0,    17,    18,    19,     0,     0,     0,   401,     0,
      27,     0,     0,     0,     0,    21,     0,     0,    28,     0,
      96,    24,    25,     0,     0,     0,     0,     0,     0,    31,
      32,    33,     0,     0,    27,     0,     0,     0,     0,     0,
       0,     0,    28,     0,    29,    30,     0,     0,     4,     0,
       0,    38,     0,    31,    32,    33,     5,     6,     0,     0,
       7,     8,     9,    10,     0,    11,    37,     0,     0,     0,
      12,     0,     0,     0,     0,    38,    13,     0,    14,     0,
       0,    15,     0,     0,    16,     0,     0,    17,    18,    19,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
      21,     0,     0,     0,     5,     6,    24,    25,     7,     8,
       9,    10,     0,    11,     0,     0,     0,   620,    12,    27,
       0,     0,     0,     0,     0,     0,     0,    28,     0,    29,
      30,   621,     0,     0,     0,    93,     0,     0,    31,    32,
      33,     0,     0,     5,     6,    95,     0,     7,     8,     9,
      10,    37,    11,     0,    24,    25,     0,    12,   237,     0,
      38,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,     0,     0,     0,    28,     0,    96,     0,     0,
       0,     0,     0,     0,    95,     0,    31,    32,    33,     0,
      93,     0,     0,    24,    25,     0,     0,     0,     5,     6,
       0,     0,     7,     8,     9,    10,    27,    11,    38,     0,
       0,     0,    12,   300,    28,     0,    96,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    32,    33,    93,     0,
       0,     0,     0,     0,     0,     0,     5,     6,     0,    95,
       7,     8,     9,    10,     0,    11,     0,    38,    24,    25,
      12,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,    28,
       0,    96,     0,     0,     0,     0,   443,    95,     0,     0,
      31,    32,    33,     0,     5,     6,    24,    25,     7,     8,
       9,    10,     0,    11,     0,     0,     0,     0,    12,    27,
       0,     0,    38,     0,     0,     0,     0,    28,     0,    96,
       0,     0,     0,     0,    93,     0,     0,     0,    31,    32,
      33,     0,     5,     6,     0,    95,     7,     8,     9,    10,
       0,    11,     0,     0,    24,    25,   575,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,    27,     0,     0,
       0,     0,     0,     0,     0,    28,     0,    96,     0,     0,
       0,     0,    93,    95,     0,     0,    31,    32,    33,     0,
       5,     6,    24,    25,     7,     8,     9,    10,     0,    11,
       0,     0,     0,     0,   614,    27,     0,     0,    38,     0,
       0,     0,     0,    28,     0,    96,     0,     0,     0,     0,
     686,     0,     0,     0,    31,    32,    33,     0,     5,     6,
       0,    95,     7,     8,     9,    10,     0,    11,     0,     0,
      24,    25,   575,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,     0,     0,
       0,    28,     0,    96,     0,     0,   747,   130,    93,    95,
       0,     0,    31,    32,    33,     0,     5,     6,    24,    25,
       7,     8,     9,    10,     0,    11,   447,     0,     0,   448,
      12,    27,     0,     0,    38,     0,     0,     0,     0,    28,
       0,    96,     0,     0,     0,   800,   130,     0,     0,     0,
      31,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    22,    23,     0,   447,    24,    25,   448,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,    27,
       0,     0,     0,     0,     0,     0,     0,    28,     0,    96,
     467,   468,   469,   470,   471,   472,     0,     0,    31,    32,
      33,    22,    23,     0,     0,   673,    35,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,   467,
     468,   469,   470,   471,   472,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,   269,   270,   271,
       0,   272,     0,   274,   275,   276,   277,     0,   278,     0,
     279,   280,   269,   270,   271,     0,   272,   273,   274,   275,
     276,   277,   734,   278,     0,   279,   280,   269,   270,   271,
       0,   272,   273,   274,   275,   276,   277,   788,   278,     0,
     279,   280,     0,   281,     0,     0,   269,   270,   271,   283,
     272,     0,   274,   275,   276,   277,   708,   278,   281,   279,
     280,   269,   270,   271,   283,   272,   273,   274,   275,   276,
     277,     0,   278,   281,   279,   280,   269,   270,   271,   283,
     272,     0,   274,   275,   276,   277,   773,   278,     0,   279,
     280,     0,   281,     0,     0,   269,   270,   271,   283,   272,
       0,   274,   275,   276,   277,     0,   278,   281,   279,   280,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
       0,     0,   281,     0,     0,     0,     0,     0,   283,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   281,     0,     0,     0,     0,     0,   283
};

static const short yycheck[] =
{
      11,    12,    19,    14,    15,     0,    17,    18,     5,     6,
       4,    56,    57,    58,   155,     1,    40,   451,    13,     3,
      37,     2,     0,   575,    19,    56,    85,   259,    39,   223,
      95,   320,     6,    27,   481,   226,   227,    31,    32,    33,
     481,   420,    37,    38,   480,   173,   528,   175,     0,   465,
     570,     7,   742,    26,    46,   421,    21,     0,    28,    28,
       6,     0,     7,    62,    63,   468,   469,   470,   471,   804,
       0,     6,     3,     4,     5,    29,   135,    71,   137,    73,
      28,     7,    29,    14,     6,   125,   126,   127,   128,   129,
      36,    86,    24,    29,     7,    32,    33,     8,     9,    93,
      48,   141,   142,    25,    58,    29,    30,   166,    86,    33,
      29,   170,    47,   109,   849,   111,    29,   317,   318,    42,
      29,    29,    58,    32,    32,    29,    58,   158,   120,    52,
     161,    62,    63,    78,    29,   109,    67,   111,   203,    58,
       7,   520,    53,   113,   113,   592,   593,   594,   595,    62,
      63,   592,   593,   594,   595,   521,   167,   152,   848,   154,
     116,   172,     7,   683,   112,   173,   174,   175,   650,   116,
     586,    97,    98,   168,   152,   234,   154,   613,     7,   373,
     374,   384,   376,     7,   192,   193,   194,   195,   183,    29,
     422,   423,   116,   106,   107,   206,   207,   108,   487,   210,
     152,   604,   154,   214,   215,   216,   758,   116,   116,   152,
     762,   154,   116,   152,   208,   154,   211,   212,   213,   184,
     185,   116,   152,   218,   154,     7,     7,   222,   245,   246,
      97,    98,    23,   211,   212,   213,    27,   261,    62,    63,
     218,   265,    30,    29,   222,    33,   219,     7,     6,   683,
     245,   246,    97,    98,   445,   396,   221,   704,     6,   638,
       7,     7,   462,   704,   464,   465,   306,   307,   468,   469,
     470,   471,    58,   639,     6,    33,   116,     6,    32,   479,
       7,   481,   106,   107,    38,    33,   351,   260,   488,    80,
     522,   523,    19,    24,   305,    26,     3,     4,     5,    28,
      47,    33,     3,     4,     5,   155,   156,    14,   511,   159,
     304,   116,   162,    14,   164,   509,    97,    98,    78,   330,
     286,   512,   513,   514,   309,   291,     0,     8,   294,    25,
     296,    78,   305,     6,   319,     7,     3,    33,     5,     7,
       7,   381,   382,   383,   317,     7,   357,    48,   388,   389,
     390,    32,    62,    63,     6,    62,    63,    30,   398,   399,
      67,    62,    63,   566,    71,   568,    67,   412,     7,   414,
      71,   574,    25,    24,     6,    26,   384,    24,    29,    33,
      33,    32,     7,    90,    38,    32,   586,   528,   405,    90,
      62,    63,   592,   593,   594,   595,   106,   107,    30,   106,
     107,   425,     6,     6,   604,   106,   107,    58,   640,   641,
     405,    78,    25,   263,    16,   265,    83,   851,    90,    21,
      33,   466,   408,    95,    28,    28,    17,    18,    24,   413,
     411,   415,    28,   441,   106,   107,   286,   482,    42,   567,
      30,   291,   453,    33,   294,    24,   296,   455,    28,   443,
     156,   424,     6,   159,   582,    34,   162,    99,   164,   101,
      15,    16,   662,    54,    55,   116,     3,     4,     5,   597,
      15,    16,     0,     1,    28,   448,    31,    14,     6,    58,
     491,   526,   527,   494,   501,    16,   497,   498,   499,   462,
      21,   464,   503,   504,     7,   526,   527,   373,   374,   544,
     376,   496,   547,   511,   704,   699,   501,   502,   481,    60,
      61,    91,    92,    93,    94,     8,   719,   534,   496,     3,
       7,     5,   116,     7,   502,    62,    63,     3,     4,     5,
      15,    16,    24,   152,   542,   154,    28,    29,    14,   534,
      25,    30,   587,     7,    33,     6,    30,     6,   748,    30,
     753,   754,    33,    26,     6,   614,    29,   263,   566,   567,
     568,   606,    44,    45,   575,   551,   574,   695,     6,   553,
     554,   552,    48,    30,   582,    30,    33,   705,    33,    39,
     286,     3,     4,     5,   117,   291,    62,    63,   294,   597,
     296,   118,    14,    51,    78,    71,   562,   642,   643,    83,
      32,    33,    49,   614,   621,     3,     4,     5,    32,    33,
      30,   642,   643,    33,    90,   646,    14,    50,   649,   592,
     593,   594,   595,    32,    33,    30,   621,   651,    33,    30,
     106,   107,    33,    30,   317,   318,   653,   654,    32,    33,
      62,    63,    10,    11,    12,    13,    31,   633,    38,    71,
      41,   635,   636,   634,    30,   663,    34,    33,   653,   654,
      32,    33,   657,     7,    62,    63,    32,    33,    90,   186,
     187,   188,   189,   801,    26,    19,    20,    21,    22,   657,
      91,    92,    93,    94,   106,   107,   680,   695,   190,   191,
     317,   318,   686,    32,    33,    29,   727,   705,    29,   730,
     666,    96,    29,   669,     6,   196,   197,   198,   199,   720,
       6,   719,    36,     7,   738,     6,     6,     6,     6,   714,
     715,    65,    66,     6,     6,    19,    20,    21,    22,     6,
     775,   704,   698,    32,     7,   721,   714,   715,   103,   723,
     724,   722,    86,   709,     7,   753,   754,   758,   793,     7,
      28,   762,     7,    58,   317,   318,     7,   768,     8,     8,
      31,    42,    31,     3,     4,     5,   732,   105,    28,   396,
     736,    65,    66,   818,    14,     7,     6,     6,   823,   462,
       6,   464,   465,   749,   750,   468,   469,   470,   471,     6,
       8,     7,    86,   801,     6,     6,   479,     7,   481,   765,
      21,     7,    26,     7,   799,   488,    26,     7,   774,    28,
       6,     6,   829,     7,    28,     6,    19,     6,     6,    28,
     786,   799,    62,    63,   790,     6,    28,     6,     3,     4,
       5,    71,     8,   396,   829,   462,     6,   464,   465,    14,
     467,   468,   469,   470,   471,     6,     6,     6,    87,     6,
      90,    28,   479,     6,   481,    28,    30,    28,     6,   856,
       6,   488,    25,     8,   875,     6,   106,   107,    26,     3,
       4,     5,    38,    48,    38,     6,     6,     6,   844,     6,
      14,   847,     6,    26,     7,     6,    58,    62,    63,    28,
       6,     6,   858,   859,    87,     6,    71,   524,   525,   462,
       6,   464,   465,   586,   467,   468,   469,   470,   471,   592,
     593,   594,   595,     7,    48,    90,   479,     7,   481,     6,
       6,   604,     7,    28,     6,   488,     6,     6,    62,    63,
      64,   106,   107,     6,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     6,    79,     7,    81,    82,     6,
       6,    28,    28,     7,    38,    26,    90,     6,     6,   586,
      97,   524,   525,     6,     6,   592,   593,   594,   595,     7,
      28,     6,   106,   107,     7,     6,     6,   604,     7,   662,
     114,   115,    28,     6,    98,    26,   120,    32,    32,    99,
      19,    20,    21,    22,     7,     6,     6,     6,     6,     6,
      15,    16,    17,    18,    29,     7,     6,    58,   100,    24,
     637,     6,     6,   122,    29,     7,    31,   120,    30,   123,
      21,   704,    28,   586,    39,     7,   524,   102,   557,   592,
     593,   594,   595,   699,   509,   662,    65,    66,   759,    54,
      55,   604,   851,    58,   402,    60,    61,   683,   832,   503,
       3,     4,     5,   499,     7,   462,   464,    86,   244,   488,
     479,   840,    15,    16,   200,   748,    19,    20,    21,    22,
     202,    24,    -1,   201,   637,    -1,    29,   704,   204,    -1,
      -1,    -1,    35,    -1,    37,    -1,    -1,    40,    -1,    -1,
      43,    -1,    -1,    46,    47,    48,    -1,    -1,    -1,   662,
      -1,   116,    -1,    -1,    57,    -1,    59,    -1,    -1,    62,
      63,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,   748,    -1,    -1,    -1,    78,    -1,    -1,     3,     4,
       5,    -1,    -1,    86,    -1,    88,    89,    -1,    -1,    14,
      -1,   704,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,   104,    -1,   106,   107,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,   124,   125,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   748,    -1,    62,    63,    64,
      -1,    -1,    -1,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    -1,    79,     4,    81,    82,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    24,    -1,    -1,    -1,    -1,
      29,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,
       7,    -1,    -1,    62,    63,    -1,    65,    66,    15,    16,
      -1,    -1,    19,    20,    21,    22,    -1,    24,    -1,    78,
      -1,    -1,    29,    -1,    -1,    -1,    85,    86,    87,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,   116,    65,    66,
     119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,     4,    90,    -1,     7,    -1,    -1,    -1,    -1,
      97,    98,    99,    15,    16,    -1,   103,    19,    20,    21,
      22,    -1,    24,    -1,    -1,    -1,    -1,    29,    -1,   116,
      -1,     7,   119,    -1,   121,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    24,    25,
      -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    -1,    65,    66,    -1,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    14,    -1,    -1,
      56,    -1,    -1,    85,    86,    87,    88,    -1,    90,    65,
      66,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,
      -1,   103,    78,    -1,    -1,    -1,    -1,    -1,    -1,     7,
      86,    48,    88,    -1,   116,    -1,    -1,   119,    -1,   121,
      -1,    97,    98,    99,    -1,    62,    63,    -1,    -1,    -1,
      28,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    -1,    79,   119,    81,    82,     3,     4,     5,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    -1,    -1,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    -1,   106,   107,
      -1,    68,    69,    70,    71,    72,    -1,    74,    75,    76,
      77,    -1,    79,    -1,    81,    82,    -1,     3,     4,     5,
       6,     7,    -1,    90,    -1,    -1,    -1,    -1,    -1,    15,
      16,     7,    -1,    19,    20,    21,    22,    -1,    24,   106,
     107,    -1,    -1,    29,    -1,    -1,    -1,   114,    -1,    35,
      -1,    37,    28,   120,    40,    -1,    -1,    43,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    62,    63,    -1,    65,
      66,    67,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    90,    91,    92,    93,    94,    95,
     106,   107,    -1,    -1,   110,     3,     4,     5,    -1,     7,
     106,   107,    -1,   119,    -1,    -1,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    -1,    24,    -1,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      -1,    -1,    40,    -1,    -1,    43,    -1,    -1,    46,    47,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    62,    63,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    -1,    -1,     3,     4,   106,   107,
       7,    -1,   110,    -1,    -1,    -1,    -1,    -1,    15,    16,
      -1,   119,    19,    20,    21,    22,    -1,    24,    -1,    -1,
      -1,    -1,    29,    -1,     4,    -1,    -1,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,
      20,    21,    22,    -1,    24,    -1,    -1,    -1,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    88,    62,    63,    -1,    65,    66,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    85,    86,    -1,    88,   116,
      90,    -1,   119,     4,    -1,    -1,     7,    97,    98,    99,
      -1,    -1,    -1,    -1,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    -1,    -1,   116,     4,    29,   119,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,
      -1,    -1,    19,    20,    21,    22,    -1,    24,    -1,    -1,
      -1,     4,    29,    -1,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    16,    65,    66,    19,    20,    21,    22,
      -1,    24,    -1,    -1,    -1,    -1,    29,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    88,    65,    66,    -1,   116,    -1,    -1,   119,     7,
      97,    98,    99,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    88,    -1,    -1,    26,   116,
      -1,    29,   119,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,   116,    15,    16,   119,    -1,    19,    20,
      21,    22,    -1,    24,    62,    63,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    -1,    -1,    40,
      -1,    -1,    43,    -1,    -1,    46,    47,    48,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    94,    95,    59,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,     7,   106,   107,
      -1,    -1,    -1,    -1,    -1,    15,    16,    78,    -1,    19,
      20,    21,    22,    -1,    24,    86,    -1,    88,    89,    29,
       6,     7,    -1,    -1,    -1,    -1,    97,    98,    99,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    24,   110,
      -1,    -1,    -1,    29,    -1,    -1,    56,    -1,   119,    35,
      -1,    37,    -1,    -1,    40,    65,    66,    43,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    86,    -1,    88,    65,
      66,    -1,     7,    -1,    -1,    -1,    -1,    97,    98,    99,
      15,    16,    78,    -1,    19,    20,    21,    22,    -1,    24,
      86,    -1,    88,    89,    29,     6,     7,    -1,    -1,   119,
      -1,    97,    98,    99,    15,    16,    -1,    -1,    19,    20,
      21,    22,    -1,    24,   110,    -1,    -1,    -1,    29,    -1,
      -1,    56,    -1,   119,    35,    -1,    37,    -1,    -1,    40,
      65,    66,    43,    -1,    -1,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    86,    -1,    88,    65,    66,    -1,     7,    -1,    -1,
      -1,    -1,    97,    98,    99,    15,    16,    78,    -1,    19,
      20,    21,    22,    -1,    24,    86,    -1,    88,    89,    29,
      30,     7,    -1,    -1,   119,    -1,    97,    98,    99,    15,
      16,    -1,    -1,    19,    20,    21,    22,    -1,    24,   110,
      -1,    -1,    28,    29,    -1,    -1,    56,    -1,   119,    35,
      -1,    37,    -1,    -1,    40,    65,    66,    43,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    86,    -1,    88,    65,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    88,    89,    -1,    -1,    -1,    -1,     7,   119,
      -1,    97,    98,    99,    -1,    -1,    15,    16,    -1,    -1,
      19,    20,    21,    22,   110,    24,    -1,   113,    -1,    28,
      29,    -1,    -1,   119,    -1,    -1,    35,    -1,    37,    -1,
      -1,    40,    -1,    -1,    43,    -1,    -1,    46,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    78,
      -1,    19,    20,    21,    22,    -1,    24,    86,    -1,    88,
      89,    29,    30,     7,    -1,    -1,    -1,    -1,    97,    98,
      99,    15,    16,    -1,    -1,    19,    20,    21,    22,    -1,
      24,   110,    -1,    -1,    -1,    29,    -1,    -1,    56,    -1,
     119,    35,    -1,    37,    -1,    -1,    40,    65,    66,    43,
      -1,    -1,    46,    47,    48,    -1,    -1,    -1,    52,    -1,
      78,    -1,    -1,    -1,    -1,    59,    -1,    -1,    86,    -1,
      88,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    88,    89,    -1,    -1,     7,    -1,
      -1,   119,    -1,    97,    98,    99,    15,    16,    -1,    -1,
      19,    20,    21,    22,    -1,    24,   110,    -1,    -1,    -1,
      29,    -1,    -1,    -1,    -1,   119,    35,    -1,    37,    -1,
      -1,    40,    -1,    -1,    43,    -1,    -1,    46,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    15,    16,    65,    66,    19,    20,
      21,    22,    -1,    24,    -1,    -1,    -1,    28,    29,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    88,
      89,    42,    -1,    -1,    -1,     7,    -1,    -1,    97,    98,
      99,    -1,    -1,    15,    16,    56,    -1,    19,    20,    21,
      22,   110,    24,    -1,    65,    66,    -1,    29,    30,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    97,    98,    99,    -1,
       7,    -1,    -1,    65,    66,    -1,    -1,    -1,    15,    16,
      -1,    -1,    19,    20,    21,    22,    78,    24,   119,    -1,
      -1,    -1,    29,    30,    86,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    16,    -1,    56,
      19,    20,    21,    22,    -1,    24,    -1,   119,    65,    66,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    88,    -1,    -1,    -1,    -1,     7,    56,    -1,    -1,
      97,    98,    99,    -1,    15,    16,    65,    66,    19,    20,
      21,    22,    -1,    24,    -1,    -1,    -1,    -1,    29,    78,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    86,    -1,    88,
      -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,    97,    98,
      99,    -1,    15,    16,    -1,    56,    19,    20,    21,    22,
      -1,    24,    -1,    -1,    65,    66,    29,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,    -1,
      -1,    -1,     7,    56,    -1,    -1,    97,    98,    99,    -1,
      15,    16,    65,    66,    19,    20,    21,    22,    -1,    24,
      -1,    -1,    -1,    -1,    29,    78,    -1,    -1,   119,    -1,
      -1,    -1,    -1,    86,    -1,    88,    -1,    -1,    -1,    -1,
       7,    -1,    -1,    -1,    97,    98,    99,    -1,    15,    16,
      -1,    56,    19,    20,    21,    22,    -1,    24,    -1,    -1,
      65,    66,    29,    -1,    -1,    -1,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    -1,     6,     7,     7,    56,
      -1,    -1,    97,    98,    99,    -1,    15,    16,    65,    66,
      19,    20,    21,    22,    -1,    24,    26,    -1,    -1,    29,
      29,    78,    -1,    -1,   119,    -1,    -1,    -1,    -1,    86,
      -1,    88,    -1,    -1,    -1,     6,     7,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    -1,    26,    65,    66,    29,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    88,
      90,    91,    92,    93,    94,    95,    -1,    -1,    97,    98,
      99,    62,    63,    -1,    -1,    26,   106,   107,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    94,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,    68,    69,    70,
      -1,    72,    -1,    74,    75,    76,    77,    -1,    79,    -1,
      81,    82,    68,    69,    70,    -1,    72,    73,    74,    75,
      76,    77,    78,    79,    -1,    81,    82,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    -1,
      81,    82,    -1,   114,    -1,    -1,    68,    69,    70,   120,
      72,    -1,    74,    75,    76,    77,    78,    79,   114,    81,
      82,    68,    69,    70,   120,    72,    73,    74,    75,    76,
      77,    -1,    79,   114,    81,    82,    68,    69,    70,   120,
      72,    -1,    74,    75,    76,    77,    78,    79,    -1,    81,
      82,    -1,   114,    -1,    -1,    68,    69,    70,   120,    72,
      -1,    74,    75,    76,    77,    -1,    79,   114,    81,    82,
      -1,    -1,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,   120
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     3,     4,     5,     7,    15,    16,    19,    20,    21,
      22,    24,    29,    35,    37,    40,    43,    46,    47,    48,
      57,    59,    62,    63,    65,    66,    67,    78,    86,    88,
      89,    97,    98,    99,   104,   106,   107,   110,   119,   124,
     125,   130,   131,   132,   133,   134,   135,   136,   137,   141,
     144,   152,   155,   156,   157,   158,   159,   160,   161,   195,
     196,   197,   199,   204,   205,   206,   207,   208,   209,   211,
     259,   261,   263,   264,   265,   266,     7,   177,     7,   179,
       7,    78,   165,   180,   191,    29,    32,   116,   185,     7,
     266,     7,   266,     7,    25,    56,    88,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   260,   195,   212,   260,   260,     7,   260,   260,    28,
     212,     7,   198,   247,     7,    97,    98,     7,    97,    98,
       7,   139,   185,     7,     7,    29,   185,    29,   185,   185,
       7,     7,     7,   212,   259,   260,   151,   152,   155,   156,
     157,     0,     1,     6,   210,     6,     6,   154,   222,     6,
     154,   222,     6,   222,     6,   222,    24,    34,    58,   185,
      29,   185,     8,    32,     8,    32,    33,    30,   260,   262,
     195,   116,   255,    39,    60,    61,    17,    18,    54,    55,
      15,    16,    10,    11,    12,    13,     8,     9,    53,   108,
     117,   118,    51,    49,    50,    25,    33,    31,    30,     6,
      36,   210,    38,    41,    34,    96,    26,    28,   210,    29,
     138,    33,    38,    29,    58,   162,   162,   162,    58,   162,
     162,   162,     6,    33,    29,    30,   262,    30,   262,     6,
      58,   162,    58,   162,     6,   109,   111,   200,   201,     6,
       6,     6,     6,   133,     6,   133,     3,     4,     5,    14,
      71,    90,   140,   141,   142,   143,   151,   211,    64,    68,
      69,    70,    72,    73,    74,    75,    76,    77,    79,    81,
      82,   114,   115,   120,   142,   149,     6,   154,    64,   115,
     142,     6,   154,   142,     6,   142,     6,   262,   260,   259,
      30,   262,   260,     4,     7,    29,    62,    63,    85,    90,
     181,   182,   183,   184,   185,   186,   250,    87,    90,   103,
     121,   178,   184,   217,   224,   244,   184,     7,    78,    30,
      33,   259,   247,   247,   248,   248,   248,   248,   249,   249,
     250,   250,   250,   250,   251,   251,   251,   251,   252,   253,
     254,    56,   255,   256,   260,   260,   185,    36,   260,   195,
     195,   195,   260,   260,   213,   214,   215,   216,   260,    28,
     139,   247,   195,     3,     5,    30,    83,   163,   164,   165,
     166,     7,    97,    98,    32,   167,   167,   167,     7,    97,
      98,     7,    30,   262,    30,    30,   103,   145,     7,     7,
     200,    52,   202,   203,   212,     6,   212,    28,   168,   177,
       7,   169,   179,   170,   180,   170,   139,   151,    58,   142,
       3,     4,     5,    14,    71,    90,   151,   211,     7,    19,
     153,    16,    21,   142,   149,   142,   142,   142,    25,    30,
       7,    24,    58,     7,   139,   162,   162,    26,    29,   226,
      23,    27,    80,     8,     7,    31,    29,   139,   158,   159,
     160,   161,   218,   219,   220,   221,   223,    90,    91,    92,
      93,    94,    95,   139,   158,   159,   160,   161,   225,   226,
     227,   228,   229,   230,   231,    24,    28,   226,   245,   246,
     246,     8,   260,   255,    31,   260,    42,    44,    45,     6,
      28,    42,    32,    33,    31,    30,   166,   166,   166,     6,
      30,    32,   162,   162,   162,   186,   162,   162,   162,    30,
       3,     4,     5,    14,   141,   150,   158,   159,   105,   146,
     162,   162,    28,     7,    42,   203,     6,     6,   212,     6,
     177,     6,     8,     7,   179,     6,   222,   180,     6,   222,
       6,   168,   169,   170,   170,   139,   151,     7,    21,   250,
       7,    30,   167,    26,     7,   139,    24,    26,    47,   188,
     189,   190,   191,   192,    26,    29,   171,   172,   176,   260,
     250,     7,    32,   219,    28,   139,   221,   223,     6,   222,
     158,   159,   228,   228,   228,   228,     7,    32,    28,   225,
      91,    92,    93,    94,   230,   139,   229,   231,     6,   222,
      19,   246,    28,   227,    29,   260,   260,   195,   260,   260,
      28,    42,   216,   212,   195,   215,   260,   164,    23,   186,
     167,   167,   167,   168,   169,   170,   170,   150,     3,     4,
       5,    14,   158,   159,     6,   154,   222,     6,   154,   222,
     141,   143,    28,    48,   112,   148,    32,    38,   212,     6,
       6,     6,    90,     8,     6,   222,     6,     6,   222,     6,
       6,   153,    25,    26,   149,    87,    30,   186,   187,   184,
       7,   186,    28,     6,   192,   186,     7,   171,   173,   174,
     175,   260,    30,   184,    28,    32,     6,   222,     6,    24,
      32,   232,   184,    28,   228,    32,     6,   222,    78,     6,
      25,    28,   260,   262,    38,    38,   212,     6,    28,    26,
       8,   168,   169,   170,   170,     6,   154,   222,     6,   154,
     222,     6,     6,   154,    78,     6,     6,   154,   143,   212,
      28,   113,   212,    58,     7,   195,     6,     6,    90,     6,
       6,    87,    25,    33,    32,    26,   190,   192,    32,     6,
      30,    30,    33,     6,   184,     6,   163,     7,     7,   233,
     241,     6,   184,    78,     6,     7,    30,   195,   195,     6,
      28,    28,     3,   186,   260,     6,     6,   154,    78,     6,
       6,   154,     6,     7,     6,     6,   147,    28,   147,    38,
       6,    26,   186,   186,   193,   194,   262,   171,     7,   174,
     171,     6,    25,   260,     6,    97,   238,     6,     7,     6,
     222,    28,     6,     7,     6,     6,   222,    28,    28,   113,
     195,   184,    26,   194,     6,    32,    32,    99,     7,   234,
      98,   239,     6,   222,     6,     6,   222,     6,   212,   193,
       6,    29,     7,     6,    58,   234,   100,   240,     6,     6,
     147,   188,     7,     7,   266,   122,   235,    28,    30,   123,
     236,   120,   237,    16,    21,    99,   101,   242,    21,   260,
     102,   243,     7
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    {
    yyval.node = create_node(e_goal );
    yyval.node->appendChild(yyvsp[0].node);
  
    goal(yyval.node);
  ;}
    break;

  case 3:

    {
    yyval.node = create_node(e_goal );
    yyval.node->appendChild(yyvsp[0].node);
    goal(yyval.node);
  ;}
    break;

  case 4:

    {
    yyval.node = create_node(e_goal );
    yyval.node->appendChild(yyvsp[0].node);
    goal(yyval.node);
  ;}
    break;

  case 5:

    {
  ;}
    break;

  case 6:

    {
  ;}
    break;

  case 7:

    {
  ;}
    break;

  case 8:

    {
  ;}
    break;

  case 9:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 10:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 11:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 12:

    {
   yyval.node=yyvsp[0].node;
  ;}
    break;

  case 13:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 14:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 15:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 16:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 17:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 18:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 19:

    {
    yyval.node=yyvsp[-1].node;
  ;}
    break;

  case 20:

    {
    yyval.node=addScriptItem(yyvsp[0].node);
  ;}
    break;

  case 21:

    {
    yyval.node=addScriptItem(yyvsp[0].node);
  ;}
    break;

  case 22:

    {
    // needed for ECMA compliance.
    clearSemicolonErr();
    yyval.node=addScriptItem(yyvsp[0].node);
  ;}
    break;

  case 23:

    {
    yyval.node = create_node(e_program);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 24:

    {
    yyval.node = create_node(e_program_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 25:

    {
    yyval.node = 0;
  ;}
    break;

  case 26:

    {
    yyval.node = create_node(e_opt_program_parameters);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 27:

    {
    yyval.node = create_node(e_identifier_list);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 28:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 29:

    {
    yyval.node = create_node(e_program_block);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 30:

    {
    yyval.node = create_node(e_program_block);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 31:

    {
    yyval.node = create_node(e_uses_clause);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 32:

    {
    yyval.node = create_node(e_block);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 33:

    {
    yyval.node = create_node(e_block);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 34:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 35:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 36:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 37:

    {
    syntaxNode *ptmp = create_node(TOK_LABEL,yyvsp[-2].name); 
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(ptmp);
    ptmp->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 38:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 39:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 40:

    {
    yyval.node = create_node(e_implementation_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 41:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 42:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 43:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 44:

    {
    yyval.node = yyvsp[-3].node;
    yyval.node->appendChild(create_node(TOK_LABEL,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 45:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 46:

    {
    yyval.node = yyvsp[-1].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 47:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 48:

    {
    yyval.node = create_node(e_unit);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 49:

    {
    yyval.node = create_node(e_interface_section);
  ;}
    break;

  case 50:

    {
    yyval.node = create_node(e_interface_section);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 51:

    {
    yyval.node = create_node(e_interface_section);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 52:

    {
    yyval.node = create_node(e_interface_section);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 53:

    {
    yyval.node = create_node(e_implementation_section);
  ;}
    break;

  case 54:

    {
    yyval.node = create_node(e_implementation_section);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 55:

    {
    yyval.node = create_node(e_implementation_section);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 56:

    {
    yyval.node = create_node(e_implementation_section);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 57:

    {
    yyval.node = 0;
  ;}
    break;

  case 58:

    {
    yyval.node = 0;
  ;}
    break;

  case 59:

    {
    yyval.node = create_node(e_init_section);
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 60:

    {
    yyval.node = create_node(e_init_section);
    yyval.node->appendChild(yyvsp[-5].node);
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 61:

    {
    yyval.node = create_node(e_init_section);
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 62:

    {
    yyval.node = 0;
  ;}
    break;

  case 63:

    {
    yyval.node = 0;
  ;}
    break;

  case 64:

    {
    yyval.node = 0;
  ;}
    break;

  case 65:

    {
    yyval.node = create_node(TOK_CDECL,yyvsp[0].name);
  ;}
    break;

  case 66:

    {
    yyval.node = create_node(TOK_EXPORT,yyvsp[0].name);
  ;}
    break;

  case 67:

    {
    yyval.node = create_node(TOK_FAR,yyvsp[0].name);
  ;}
    break;

  case 68:

    {
    yyval.node = create_node(TOK_PASCAL,yyvsp[0].name);
  ;}
    break;

  case 69:

    {
    yyval.node = create_node(TOK_SAFECALL,yyvsp[0].name);
  ;}
    break;

  case 70:

    {
    yyval.node = create_node(TOK_STDCALL,yyvsp[0].name);
  ;}
    break;

  case 71:

    {
    yyval.node = create_node(TOK_VIRTUAL,yyvsp[0].name);
  ;}
    break;

  case 72:

    {
    yyval.node = create_node(TOK_ABSTRACT,yyvsp[0].name);
  ;}
    break;

  case 73:

    {
    yyval.node = create_node(TOK_DYNAMIC,yyvsp[0].name);
  ;}
    break;

  case 74:

    {
    yyval.node = create_node(TOK_OVERRIDE,yyvsp[0].name);
  ;}
    break;

  case 75:

    {
    yyval.node = create_node(TOK_REINTRODUCE,yyvsp[0].name);
  ;}
    break;

  case 76:

    {
    yyval.node = create_node(TOK_OVERLOAD,yyvsp[0].name);
  ;}
    break;

  case 77:

    {
    yyval.node = create_node(TOK_DISPID,yyvsp[-1].name);
    yyval.node->appendChild(create_node(TOK_INTEGER,yyvsp[0].name));
    yyval.node->children[0]->negate_ = 0;
  ;}
    break;

  case 78:

    {
    yyval.node = create_node(TOK_DISPID,yyvsp[-2].name);
    yyval.node->appendChild(create_node(TOK_INTEGER,yyvsp[-1].name));
    yyval.node->children[0]->negate_ = 1;
  ;}
    break;

  case 79:

    {
    yyval.node = create_node(e_interface_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 80:

    {
    yyval.node = create_node(e_interface_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 81:

    {
    yyval.node = create_node(e_interface_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 82:

    {
    yyval.node = create_node(e_interface_declarations);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 83:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    yyval.node = create_node(e_interface_declarations);
    yyval.node->appendChild(pmethod);
      pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 84:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(pdirective);
    pdirective->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 85:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 86:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    ptmp = create_node(TOK_MESSAGE,yyvsp[-2].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    pdirective->appendChild(ptmp);

    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-4].node);
    pmethod->appendChild(pdirective);
  ;}
    break;

  case 87:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    ptmp=create_node(TOK_MESSAGE,yyvsp[-3].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyvsp[-1].node->appendChild(ptmp);

    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-5].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 88:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-3].node);
    pmethod->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 89:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);

    yyval.node = create_node(e_interface_declarations);
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 90:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    yyval.node = create_node(e_interface_declarations);
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(pdirective);
    pdirective->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 91:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);

    yyval.node = create_node(e_interface_declarations);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 92:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);

    yyval.node = create_node(e_interface_declarations);
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-3].node);
    pmethod->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 93:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 94:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 95:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 96:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 97:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    yyval.node = yyvsp[-2].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 98:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    ptmp = create_node(TOK_MESSAGE,yyvsp[-2].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    pdirective->appendChild(ptmp);

    yyval.node = yyvsp[-5].node;
   
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-4].node);
    pmethod->appendChild(pdirective);
  ;}
    break;

  case 99:

    {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    
    yyval.node = yyvsp[-3].node;
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 100:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    ptmp = create_node(TOK_MESSAGE,yyvsp[-3].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));

    yyval.node = yyvsp[-6].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-5].node);
    pmethod->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->appendChild(ptmp);
  ;}
    break;

  case 101:

    {
    yyval.node = yyvsp[-3].node;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 102:

    {
    yyval.node = yyvsp[-4].node;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    yyval.node->appendChild(pmethod );
    pmethod->appendChild(yyvsp[-3].node);
    pmethod->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 103:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);
    yyval.node = yyvsp[-2].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 104:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);
    yyval.node = yyvsp[-3].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 105:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);
    yyval.node = yyvsp[-3].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pdirective->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 106:

    {
    syntaxNode *pmethod = create_node(e_function_declaration);
    yyval.node = yyvsp[-4].node;

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-3].node);
    pmethod->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 107:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 108:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 109:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 110:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 111:

    {
    yyval.node = create_node(e_function_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 112:

    {
    syntaxNode *pdirective=create_node(e_directive_list);
    
    yyval.node = create_node(e_function_definition);
    
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(pdirective);
    pdirective->appendChild(yyvsp[0].node);
  ;}
    break;

  case 113:

    {
    yyval.node = create_node(e_function_definition);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 114:

    {
    yyval.node = create_node(e_function_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 115:

    {
    yyval.node = 0;
  ;}
    break;

  case 116:

    {
    yyval.node = create_node(e_function_declaration);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_FORWARD,yyvsp[0].name));
  ;}
    break;

  case 117:

    {
    yyval.node = create_node(TOK_IDENTIFIER,yyvsp[0].name)
  ;}
    break;

  case 118:

    {
   yyval.node = create_node(TOK_STRING ,yyvsp[0].name)
  ;}
    break;

  case 119:

    {
    yyval.node=create_node(TOK_EXTERNAL,yyvsp[-1].name);
    yyval.node->children[0]->appendChild(yyvsp[0].node);
  ;}
    break;

  case 120:

    {
    yyval.node=create_node(TOK_EXTERNAL,yyvsp[-3].name);
    yyval.node->children[0]->appendChild(yyvsp[-2].node);
    yyval.node->children[0]->appendChild(yyvsp[0].node);
  ;}
    break;

  case 121:

    {
    yyval.node = create_node(e_procedure_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 122:

    {
    syntaxNode *pdirective=create_node(e_directive_list);
    yyval.node = create_node(e_procedure_definition);
    yyval.node->appendChild(yyvsp[-1].node);
    pdirective->appendChild(yyvsp[0].node);
    yyval.node->appendChild(pdirective);
  ;}
    break;

  case 123:

    {
    yyval.node = create_node(e_procedure_definition);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 124:

    {
    yyval.node = create_node(e_procedure_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 125:

    {
    yyval.node = 0;
  ;}
    break;

  case 126:

    {
    yyval.node = create_node(e_procedure_declaration);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_FORWARD,yyvsp[0].name));
  ;}
    break;

  case 127:

    {
    yyval.node = create_node(e_constructor_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 128:

    {
    yyval.node = create_node(e_constructor_definition);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 129:

    {
    yyval.node = create_node(e_destructor_definition);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 130:

    {
    yyval.node = create_node(e_destructor_definition);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 131:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 132:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 133:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 134:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 135:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 136:

    {
    yyval.node = create_node(e_function_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 137:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 138:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 139:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 140:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 141:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 142:

    {
    yyval.node = create_node(e_procedure_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 143:

    {
    yyval.node = create_node(e_constructor_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 144:

    {
    yyval.node = create_node(e_constructor_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 145:

    {
    yyval.node = create_node(e_destructor_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 146:

    {
    yyval.node = create_node(e_destructor_header);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 147:

    {
    yyval.node = 0;
  ;}
    break;

  case 148:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 149:

    {
    yyval.node = 0;
  ;}
    break;

  case 150:

    {
    yyval.node = create_node(e_parameter_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 151:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 152:

    {
    yyval.node = create_node(e_formal_parameter );
    yyval.node->appendChild(create_node(TOK_VAR,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 153:

    {
    yyval.node = create_node(e_formal_parameter );
    yyval.node->appendChild(create_node(TOK_CONST,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 154:

    {
    yyval.node = create_node(e_formal_parameter );
    yyval.node->appendChild(create_node(TOK_OUT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 155:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 156:

    {
    yyval.node = create_node(e_identifier_list);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 157:

    {
    yyval.node = create_node(e_identifier_list);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 158:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 159:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 160:

    {
    yyval.node = create_node(e_parameter );
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 161:

    {
    yyval.node = create_node(e_parameter );
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 162:

    {
    yyval.node = create_node(e_parameter );
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 163:

    {
    yyval.node = create_node(e_parameter );
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_ARRAY,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 164:

    {
    yyval.node = create_node(e_parameter );
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_ARRAY,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_CONST,yyvsp[0].name));
  ;}
    break;

  case 165:

    {
    yyval.node = 0;
  ;}
    break;

  case 166:

    {
    yyval.node = create_node(e_function_return);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 167:

    {
    yyval.node = create_node(e_const_definition_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 168:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 169:

    {
    yyval.node = create_node(e_type_declaration_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 170:

    {
    yyval.node = create_node(e_type_declaration_list);
    yyval.node->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 171:

    {
    yyval.node = create_node(e_type_declaration_list);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_CLASS,yyvsp[-1].name));
  ;}
    break;

  case 172:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 173:

    {
    yyval.node = yyvsp[-3].node;
    yyval.node->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 174:

    {
    yyval.node = yyvsp[-4].node;
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_CLASS,yyvsp[-1].name));
  ;}
    break;

  case 175:

    {
    yyval.node = create_node(e_list_var_declaration);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 176:

    {
    yyval.node = create_node(e_list_var_declaration);
    yyval.node->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 177:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 178:

    {
    yyval.node = yyvsp[-3].node;
    yyval.node->appendChild(yyvsp[-2].node);
    yyvsp[-2].node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 179:

    {
    yyval.node = create_node(e_typed_constant);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 180:

    {
    yyval.node = create_node(e_typed_constant);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 181:

    {
    yyval.node = create_node(e_typed_constant);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 182:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 183:

    {
    yyval.node = create_node(e_record_field_constant_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 184:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 185:

    {
    yyval.node = create_node(e_record_field_constant);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 186:

    {
    yyval.node = create_node(e_typed_constant_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 187:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 188:

    {
    yyval.node = create_node(e_array_constant);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 189:

    {
    yyval.node = create_node(e_const_definition);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 190:

    {
    yyval.node = create_node(e_const_definition);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 191:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 192:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 193:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 194:

    {
    yyval.node = create_node(e_type_declaration);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 195:

    {
    yyval.node = create_node(e_type_declaration);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 196:

    {
      yyval.node = yyvsp[0].node;
  ;}
    break;

  case 197:

    {
    yyval.node = 0;
  ;}
    break;

  case 198:

    {
    yyval.node = create_node(TOK_PACKED,yyvsp[0].name);
  ;}
    break;

  case 199:

    {
    yyval.node = create_node(e_struct_type );
    yyval.node->appendChild(yyvsp[-6].node);
    yyval.node->appendChild(create_node(TOK_ARRAY,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 200:

    {
    yyval.node = create_node(e_struct_type );
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_ARRAY,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 201:

    {
    yyval.node = create_node(e_struct_type );
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_RECORD,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 202:

    {
    yyval.node = create_node(e_struct_type );
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_SET,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 203:

    {
    yyval.node = create_node(e_procedure_type);
    yyval.node->appendChild(create_node(TOK_PROCEDURE,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 204:

    {
    yyval.node = create_node(e_function_type);
    yyval.node->appendChild(create_node(TOK_FUNCTION,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 205:

    {
    yyval.node = create_node(e_function_type);
    yyval.node->appendChild(create_node(TOK_FUNCTION,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 206:

    {
    yyval.node = create_node(e_object_procedure_type);
    yyval.node->appendChild(create_node(TOK_PROCEDURE,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 207:

    {
    yyval.node = create_node(e_object_function_type);
    yyval.node->appendChild(create_node(TOK_FUNCTION,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 208:

    {
    yyval.node = create_node(e_type_spec);
    yyval.node->appendChild(create_node(TOK_CLASS,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 209:

    {
    yyval.node = create_node(e_type_spec);
    yyval.node->appendChild(create_node(TOK_CLASS,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 210:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 211:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 212:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 213:

    {
    yyval.node = create_node(e_defptr_list);
    yyval.node->appendChild(create_node(TOK_DEREF,yyvsp[0].name));
  ;}
    break;

  case 214:

    {
    yyval.node = yyvsp[-1].node;
    yyval.node->appendChild(create_node(TOK_DEREF,yyvsp[0].name));
  ;}
    break;

  case 215:

    {
    yyval.node = create_node(e_simple_type);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 216:

    {
    yyval.node = create_node(e_simple_type);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 217:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_simple_type);
    ptmp = create_node(e_sized_string);
    yyval.node->appendChild(ptmp);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    ptmp->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 218:

    {
    yyval.node = create_node(e_simple_type);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 219:

    {
    yyval.node = create_node(e_simple_type);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 220:

    {
    yyval.node = create_node(e_simple_type);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 221:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_simple_type);
    ptmp = create_node(e_ranged_item);
    yyval.node->appendChild(ptmp);
    ptmp->appendChild(yyvsp[-2].node);
    ptmp->appendChild(yyvsp[0].node);
  ;}
    break;

  case 222:

    {
    yyval.node = create_node(e_simple_type_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 223:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 224:

    {
    yyval.node = create_node(e_opt_field_list);
  ;}
    break;

  case 225:

    {
    yyval.node = create_node(e_opt_field_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 226:

    {
    yyval.node = create_node(e_opt_field_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 227:

    {
    yyval.node = create_node(e_opt_field_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 228:

    {
    yyval.node = create_node(e_opt_field_list);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 229:

    {
    yyval.node = create_node(e_opt_field_list);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 230:

    {
    yyval.node = create_node(e_field_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 231:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 232:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 233:

    {
    yyval.node = create_node(e_varfield_declaration);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 234:

    {
    yyval.node = create_node(e_varfield_declaration);
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 235:

    {
    yyval.node = create_node(e_varfield_declaration);
    yyval.node->appendChild(yyvsp[-6].node);
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 236:

    {
    yyval.node = create_node(e_variant_section);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 237:

    {
    yyval.node = create_node(e_variant_section);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-5].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 238:

    {
    yyval.node = create_node(e_rec_variant_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 239:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 240:

    {
    yyval.node = create_node(e_rec_variant);
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 241:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 242:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 243:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 244:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 245:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 246:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 247:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 248:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 249:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 250:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 251:

    {
    yyval.node = create_node(e_statement);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 252:

    {
    yyval.node = create_node(e_statement);
    yyval.node->appendChild(create_node(TOK_INHERITED,yyvsp[0].name));
  ;}
    break;

  case 253:

    {
    yyval.node = create_node(e_statement_goto);
    yyval.node->appendChild(create_node(TOK_GOTO,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 254:

    {
    yyval.node = create_node(e_statement_label);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 255:

    {
    yyval.node = create_node(e_statement_label);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
  ;}
    break;

  case 256:

    {
    yyval.node = create_node(e_statement_raise);
    yyval.node->appendChild(create_node(TOK_RAISE,yyvsp[0].name));
  ;}
    break;

  case 257:

    {
    yyval.node = create_node(e_statement_raise);
    yyval.node->appendChild(create_node(TOK_RAISE,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 258:

    {
    yyval.node = create_node(e_statement_raise);
    yyval.node->appendChild(create_node(TOK_RAISE,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_AT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 259:

    {
    yyval.node = create_node(e_statement_with);
    yyval.node->appendChild(create_node(TOK_WITH,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_DO,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 260:

    {
    yyval.node = create_node(e_with_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 261:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 262:

    {
    yyval.node = create_node(e_statement_try);
    yyval.node->appendChild(create_node(TOK_TRY,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 263:

    {
    yyval.node = create_node(e_statement_try);
    yyval.node->appendChild(create_node(TOK_TRY,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 264:

    {
    yyval.node = create_node(e_try_handler);
    yyval.node->appendChild(create_node(TOK_FINALLY,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 265:

    {
    yyval.node = create_node(e_try_handler);
    yyval.node->appendChild(create_node(TOK_FINALLY,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 266:

    {
    yyval.node = create_node(e_try_handler);
    yyval.node->appendChild(create_node(TOK_FINALLY,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 267:

    {
    yyval.node = create_node(e_try_handler);
    yyval.node->appendChild(create_node(TOK_FINALLY,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 268:

    {
    yyval.node = create_node(e_try_handler);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 269:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 270:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 271:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 272:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 273:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 274:

    {
    yyval.node = create_node(e_except_handler);
    yyval.node->appendChild(create_node(TOK_EXCEPT,yyvsp[0].name));
  ;}
    break;

  case 275:

    {
    yyval.node = create_node(e_handler_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 276:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 277:

    {
    yyval.node = create_node(e_handler);
    yyval.node->appendChild(create_node(TOK_ON,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_DO,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 278:

    {
    yyval.node = create_node(e_handler);
    yyval.node->appendChild(create_node(TOK_ON,yyvsp[-5].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    yyval.node->appendChild(create_node(TOK_DO,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 279:

    {
    yyval.node = create_node(e_statement_assign);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 280:

    {
    yyval.node = create_node(e_statement_repeat);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 281:

    {
    yyval.node = create_node(e_statement_repeat);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 282:

    {
    yyval.node = create_node(e_statement_while);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 283:

    {
    yyval.node = create_node(e_statement_if);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 284:

    {
    yyval.node = create_node(e_statement_if);
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 285:

    {
    yyval.node = create_node(e_statement_for);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-6].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_TO,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 286:

    {
    yyval.node = create_node(e_statement_for);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-6].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_DOWNTO,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 287:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 288:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 289:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-6].name));
    yyval.node->appendChild(yyvsp[-5].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-4].name));
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 290:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-7].name));
    yyval.node->appendChild(yyvsp[-6].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 291:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-7].name));
    yyval.node->appendChild(yyvsp[-6].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 292:

    {
    yyval.node = create_node(e_statement_case);
    yyval.node->appendChild(create_node(TOK_CASE,yyvsp[-8].name));
    yyval.node->appendChild(yyvsp[-7].node);
    yyval.node->appendChild(create_node(TOK_OF,yyvsp[-6].name));
    yyval.node->appendChild(yyvsp[-5].node);
    yyval.node->appendChild(create_node(TOK_ELSE,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_END,yyvsp[0].name));
  ;}
    break;

  case 293:

    {
   ;}
    break;

  case 294:

    {
   ;}
    break;

  case 295:

    {
    yyval.node = yyvsp[-1].node;
  ;}
    break;

  case 296:

    {
    yyval.node = yyvsp[-2].node;
  ;}
    break;

  case 297:

    {
    yyval.node = create_node(e_statement_list);
  ;}
    break;

  case 298:

    {
    yyval.node = create_node(e_statement_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 299:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 300:

    {
    yyval.node = create_node(e_list_case_option);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 301:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 302:

    {
    yyval.node = create_node(e_case_item_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 303:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 304:

    {
    yyval.node = create_node(e_case_item);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 305:

    {
    yyval.node = create_node(e_case_item);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(create_node(TOK_DOTDOT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 306:

    {
    yyval.node = create_node(e_case_option);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 307:

    {
    yyval.node = create_node(e_case_option);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 308:

    {
    yyval.node = create_node(e_object_type);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 309:

    {
    yyval.node = create_node(e_object_type);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 310:

    {
    yyval.node = create_node(e_object_inheritance);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
  ;}
    break;

  case 311:

    {
    yyval.node = create_node(e_object_body);
  ;}
    break;

  case 312:

    {
    yyval.node = create_node(e_object_body);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 313:

    {
    yyval.node = create_node(e_object_body);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 314:

    {
    yyval.node = create_node(e_object_body);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 315:

    {
    yyval.node = create_node(e_obj_field_list);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 316:

    {
    yyval.node = yyvsp[-4].node;
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_COLON,yyvsp[-2].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 317:

    {
    yyval.node = create_node(e_method_list);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 318:

    {
    yyval.node = create_node(e_method_list);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 319:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 320:

    {
    yyval.node = yyvsp[-3].node;
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 321:

    {
    yyval.node = create_node(e_directive_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 322:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 323:

    {
    yyval.node = create_node(e_method_heading);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 324:

    {
    yyval.node = create_node(e_method_heading);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 325:

    {
    yyval.node = create_node(e_method_heading);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 326:

    {
    yyval.node = create_node(e_method_heading);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 327:

    {
    yyval.node = create_node(e_class_type);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 328:

    {
    yyval.node = create_node(e_class_type);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 329:

    {
    yyval.node = create_node(e_class_body);
  ;}
    break;

  case 330:

    {
    yyval.node = create_node(e_class_body);
    yyval.node->appendChild(yyvsp[0].node);

    yyval.node->copyNode(yyvsp[0].node);
  ;}
    break;

  case 331:

    {
    yyval.node = create_node(e_class_heritage);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 332:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_class_items_list);
    ptmp = create_node(e_unqualified_section);
    ptmp->copyNode(yyvsp[0].node);
    yyval.node->appendChild(ptmp);
  ;}
    break;

  case 333:

    {
    yyval.node = create_node(e_class_items_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 334:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_class_items_list);
    ptmp=create_node(e_public_section);
    yyval.node->appendChild(ptmp);
    ptmp->copyNode(yyvsp[0].node);
    
  ;}
    break;

  case 335:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_class_items_list);
    ptmp = create_node(e_protected_section);
    yyval.node->appendChild(ptmp);
    ptmp->copyNode(yyvsp[0].node);
  ;}
    break;

  case 336:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_class_items_list);
    ptmp = create_node(e_published_section);
    yyval.node->appendChild(ptmp);
    ptmp->copyNode(yyvsp[0].node);
  ;}
    break;

  case 337:

    {
    syntaxNode *ptmp;
    yyval.node = create_node(e_class_items_list);
    ptmp = create_node(e_private_section);
    yyval.node->appendChild(ptmp);
    ptmp->copyNode(yyvsp[0].node);
  ;}
    break;

  case 338:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->copyNode(yyvsp[0].node);
  ;}
    break;

  case 339:

    {
    yyval.node = yyvsp[-1].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 340:

    {
    //hello
    syntaxNode *pmethod;
    yyval.node = create_node(e_class_items_section);
    pmethod = create_node(e_method_member);
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 341:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_method_member);
    syntaxNode *pdirective=create_node(e_directive_list);

    yyval.node = create_node(e_class_items_section);

    ptmp = create_node(TOK_MESSAGE,yyvsp[-2].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
  
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-4].node);
    pmethod->appendChild(pdirective);
    pdirective->appendChild(ptmp);
  ;}
    break;

  case 342:

    {
    //hello2
    syntaxNode *pmethod;
    yyval.node = create_node(e_class_items_section);
    pmethod = create_node(e_method_member);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 343:

    {
    syntaxNode *ptmp,*pmethod;

    ptmp = create_node(TOK_MESSAGE,yyvsp[-3].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));

    pmethod = create_node(e_method_member);
      
    yyval.node = create_node(e_class_items_section); 
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-5].node);
    pmethod->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->appendChild(ptmp);
  ;}
    break;

  case 344:

    {
    yyval.node = create_node(e_class_items_section);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 345:

    {
    syntaxNode *pvar;
    yyval.node = create_node(e_class_items_section);
    pvar = create_node(e_variable_member);
    
    yyval.node->appendChild(pvar);
    pvar->appendChild(yyvsp[-3].node);
    pvar->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 346:

    {
    //hello3
    syntaxNode *pmethod;
    yyval.node = yyvsp[-2].node;
    pmethod = create_node(e_method_member);
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 347:

    {
    syntaxNode *ptmp,*pmethod;
    syntaxNode *pdirective = create_node(e_directive_list);

    yyval.node = yyvsp[-5].node;

    ptmp = create_node(TOK_MESSAGE,yyvsp[-2].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    pdirective->appendChild(ptmp);

    pmethod = create_node(e_method_member);

    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-4].node);
    pmethod->appendChild(pdirective);
  ;}
    break;

  case 348:

    {
    // hello6
    syntaxNode *pmethod;
    yyval.node = yyvsp[-3].node;

    pmethod = create_node(e_method_member);
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-2].node);
    pmethod->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 349:

    {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_method_member);

    yyval.node = yyvsp[-6].node;
    
    ptmp = create_node(TOK_MESSAGE,yyvsp[-3].name);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
    
    yyval.node->appendChild(pmethod);
    pmethod->appendChild(yyvsp[-5].node);
    pmethod->appendChild(yyvsp[-1].node);
    yyvsp[-1].node->appendChild(ptmp);
  ;}
    break;

  case 350:

    {
    yyval.node = yyvsp[-1].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 351:

    {
    syntaxNode *pvar;

    yyval.node = yyvsp[-4].node;

    pvar = create_node(e_variable_member);
    yyval.node->appendChild(pvar);

    pvar->appendChild(yyvsp[-3].node);
    pvar->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 352:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 353:

    {
    yyval.node = yyvsp[0].node;
    yyval.node->static_ = 1;
  ;}
    break;

  case 354:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 355:

    {
    yyval.node = yyvsp[0].node;
    yyval.node->static_ = 1;
  ;}
    break;

  case 356:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 357:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 358:

    {
    yyval.node = create_node(e_public_section)
  ;}
    break;

  case 359:

    {
    yyval.node = create_node(e_protected_section)
  ;}
    break;

  case 360:

    {
    yyval.node = create_node(e_published_section)
  ;}
    break;

  case 361:

    {
    yyval.node = create_node(e_private_section)
  ;}
    break;

  case 362:

    {
    yyval.node = create_node(e_property_definition);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 363:

    {
    yyval.node = create_node(e_property_definition);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-5].name));
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(create_node(TOK_DEFAULT,yyvsp[-1].name));
  ;}
    break;

  case 364:

    {
    yyval.node = 0;
  ;}
    break;

  case 365:

    {
    yyval.node = create_node(e_opt_property_interface);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 366:

    {
    yyval.node = create_node(e_opt_property_interface);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
    yyval.node->appendChild(yyvsp[-3].node);
  ;}
    break;

  case 367:

    {
    yyval.node = create_node(e_property_specifiers);
    yyval.node->appendChild(yyvsp[-8].node);
    yyval.node->appendChild(yyvsp[-7].node);
    yyval.node->appendChild(yyvsp[-6].node);
    yyval.node->appendChild(yyvsp[-5].node);
    yyval.node->appendChild(yyvsp[-4].node);
    yyval.node->appendChild(yyvsp[-3].node);
  ;}
    break;

  case 368:

    {
    yyval.node = create_node(e_identifier_list);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 369:

    {
    yyval.node=yyvsp[-2].node;
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 370:

    {
   yyval.node = 0;
  ;}
    break;

  case 371:

    {
    yyval.node = create_node(TOK_READONLY,yyvsp[0].name);
  ;}
    break;

  case 372:

    {
   yyval.node = 0;
  ;}
    break;

  case 373:

    {
    yyval.node = create_node(TOK_READONLY,yyvsp[0].name);
  ;}
    break;

  case 374:

    {
    yyval.node = 0;
  ;}
    break;

  case 375:

    {
    yyval.node = create_node(TOK_DISPID,yyvsp[-1].name);
    yyval.node->appendChild(create_node(TOK_INTEGER,yyvsp[0].name));
    yyval.node->children[0]->negate_ = 0;
  ;}
    break;

  case 376:

    {
    yyval.node = create_node(TOK_DISPID,yyvsp[-2].name);
    yyval.node->appendChild(create_node(TOK_INTEGER,yyvsp[-1].name));
    yyval.node->children[0]->negate_ = 1;
  ;}
    break;

  case 377:

    {
      yyval.node = 0;
  ;}
    break;

  case 378:

    {
    yyval.node = create_node(e_opt_read);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 379:

    {
    yyval.node = 0;
  ;}
    break;

  case 380:

    {
    yyval.node = create_node(e_opt_write);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 381:

    {
    yyval.node = 0;
  ;}
    break;

  case 382:

    {
    yyval.node = create_node(e_opt_stored);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 383:

    {
    yyval.node = create_node(e_opt_stored);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 384:

    {
    yyval.node = 0;
  ;}
    break;

  case 385:

    {
    yyval.node = create_node(e_opt_index);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 386:

    {
    yyval.node = 0;
  ;}
    break;

  case 387:

    {
    yyval.node = create_node(e_opt_default);
    yyval.node->appendChild(create_node(TOK_DEFAULT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 388:

    {
    yyval.node = create_node(e_opt_default);
    yyval.node->appendChild(create_node(TOK_NODEFAULT,yyvsp[0].name));
  ;}
    break;

  case 389:

    {
    yyval.node = 0;
  ;}
    break;

  case 390:

    {
    yyval.node = create_node(e_opt_implements);
    yyval.node->appendChild(create_node(TOK_IMPLEMENTS,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 391:

    {
    yyval.node = create_node(e_interface_type);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 392:

    {
    yyval.node = create_node(e_interface_forward_decl);
  ;}
    break;

  case 393:

    {
    yyval.node = create_node(e_disp_forward_decl);
  ;}
    break;

  case 394:

    {
    yyval.node = create_node(e_interface_type);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 395:

    {
    yyval.node = create_node(e_dispinterface_type);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 396:

    {
    yyval.node = create_node(e_opt_guid);
    yyval.node->appendChild(create_node(TOK_STRING,yyvsp[-1].name));
  ;}
    break;

  case 397:

    {
    yyval.node = create_node(e_interface_body);
  ;}
    break;

  case 398:

    {
    yyval.node = create_node(e_interface_body);
    yyval.node->appendChild(yyvsp[-2].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 399:

    {
    yyval.node = create_node(e_interface_body);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 400:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 401:

    {
    if(yyvsp[-2].node->node_type == e_as_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_as_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 402:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 403:

    {
    if(yyvsp[-2].node->node_type == e_shift_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_SHL,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_shift_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_SHL,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 404:

    {
    if(yyvsp[-2].node->node_type == e_shift_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_SHR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_shift_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_SHR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 405:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 406:

    {
    if(yyvsp[-2].node->node_type == e_multiplicative_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_STAR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_multiplicative_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_STAR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 407:

    {
    if(yyvsp[-2].node->node_type == e_multiplicative_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_SLASH,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_multiplicative_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_SLASH,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 408:

    {
    if(yyvsp[-2].node->node_type == e_multiplicative_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_MOD,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_multiplicative_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_MOD,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 409:

    {
    if(yyvsp[-2].node->node_type == e_multiplicative_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_DIV,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_multiplicative_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_DIV,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 410:

    {
    yyval.node=yyvsp[0].node;
  ;}
    break;

  case 411:

    {
    if(yyvsp[-2].node->node_type == e_additive_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_PLUS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_additive_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_PLUS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 412:

    {
    if(yyvsp[-2].node->node_type == e_additive_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_MINUS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_additive_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_MINUS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 413:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 414:

    {
    if(yyvsp[-2].node->node_type == e_relational_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_GT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_relational_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_GT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 415:

    {
    if(yyvsp[-2].node->node_type == e_relational_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_LT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_relational_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_LT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 416:

    {
    if(yyvsp[-2].node->node_type == e_relational_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_LE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_relational_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_LE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 417:

    {
    if(yyvsp[-2].node->node_type == e_relational_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_GE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_relational_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_GE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 418:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 419:

    {
    if(yyvsp[-2].node->node_type == e_equality_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_equality_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_EQ,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 420:

    {
    if(yyvsp[-2].node->node_type == e_equality_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_NE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_equality_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_NE,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 421:

    {
    if(yyvsp[-2].node->node_type == e_equality_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_IS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_equality_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_IS,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 422:

    {
    if(yyvsp[-2].node->node_type == e_equality_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_IN,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_equality_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_IN,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 423:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 424:

    {
    if(yyvsp[-2].node->node_type == e_and_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_LAND,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_and_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_LAND,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 425:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 426:

    {
    if(yyvsp[-2].node->node_type == e_exclusive_or_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_LOR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_exclusive_or_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_LOR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 427:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 428:

    {
    if(yyvsp[-2].node->node_type == e_inclusive_or_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(create_node(TOK_XOR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_inclusive_or_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(create_node(TOK_XOR,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 429:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 430:

    {
    yyval.node = create_node(e_logical_and_expression);
    yyval.node->appendChild(create_node(TOK_NOT,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 431:

    {
    if(yyvsp[-2].node->node_type == e_logical_and_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_logical_and_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 432:

    {
    if(yyvsp[-3].node->node_type == e_logical_and_expression)
    {
      yyval.node = yyvsp[-3].node;
      yyval.node->appendChild(create_node(TOK_NOT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_logical_and_expression);
      yyval.node->appendChild(yyvsp[-3].node);
      yyval.node->appendChild(create_node(TOK_NOT,yyvsp[-1].name));
      yyval.node->appendChild(yyvsp[0].node); 
    }
  ;}
    break;

  case 433:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 434:

    {
    if(yyvsp[-2].node->node_type == e_logical_or_expression)
    {
      yyval.node = yyvsp[-2].node;
      yyval.node->appendChild(yyvsp[0].node);
    }
    else
    {
      yyval.node = create_node(e_logical_or_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(yyvsp[0].node);
    }
  ;}
    break;

  case 435:

    {
    ;}
    break;

  case 436:

    {
    ;}
    break;

  case 437:

    {
    ;}
    break;

  case 438:

    {
    ;}
    break;

  case 439:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 440:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 441:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 442:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 443:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 444:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 445:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 446:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 447:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 448:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 449:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_MINUS,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 450:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_PLUS,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 451:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_ADDR,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 452:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_MINUS,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 453:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(create_node(TOK_PLUS,yyvsp[-1].name));
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 454:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 455:

    {
    if(yyvsp[-1].node->node_type == e_sub_expression)
      yyval.node = yyvsp[-1].node;
    else
    {
      yyval.node = create_node(e_sub_expression);
      yyval.node->appendChild(yyvsp[-1].node);
    }
  ;}
    break;

  case 456:

    {
    if(yyvsp[-2].node->node_type == e_sub_expression)
      yyval.node = yyvsp[-2].node;
    else
    {
      yyval.node = create_node(e_sub_expression);
      yyval.node->appendChild(yyvsp[-2].node);
    }
  ;}
    break;

  case 457:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 458:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 459:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 460:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 461:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[-1].node);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 462:

    {
    yyval.node = create_node(e_simple_expression);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 463:

    {
    yyval.node = yyvsp[0].node;
  ;}
    break;

  case 464:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
  ;}
    break;

  case 465:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
  ;}
    break;

  case 466:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
  ;}
    break;

  case 467:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(yyvsp[-2].node);
  ;}
    break;

  case 468:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 469:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 470:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 471:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 472:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_INHERITED,yyvsp[-1].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[0].name));
  ;}
    break;

  case 473:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_INHERITED,yyvsp[-3].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-2].name));
  ;}
    break;

  case 474:

    {
    yyval.node = create_node(e_proc_func_expression);
    yyval.node->appendChild(create_node(TOK_INHERITED,yyvsp[-4].name));
    yyval.node->appendChild(create_node(TOK_IDENTIFIER,yyvsp[-3].name));
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 475:

    {
    yyval.node = yyvsp[-2].node;
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 476:

    {
    yyval.node = create_node(e_expression_list);
    yyval.node->appendChild(yyvsp[0].node);
  ;}
    break;

  case 477:

    {
    syntaxNode *pnode = yyvsp[0].node->children[0];

    if(pnode->node_type==e_record_expression)
    {
      pnode->prependChild(yyvsp[-2].node);
      yyval.node=pnode;
      //$$=$3;
    }
    else
    {
      yyval.node = create_node(e_record_expression);
      yyval.node->appendChild(yyvsp[-2].node);
      yyval.node->appendChild(yyvsp[0].node);
    }
    //$$ = create_node(e_record_expression);
    //$$->appendChild($1);
    //$$->appendChild($3);
  ;}
    break;

  case 478:

    {
    yyval.node = create_node(e_array_elt_expression);
    yyval.node->appendChild(yyvsp[-3].node);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 479:

    {
    yyval.node = create_node(e_array_expression);
    yyval.node->appendChild(yyvsp[-1].node);
  ;}
    break;

  case 480:

    {
    yyval.node = create_node(e_array_expression);
  ;}
    break;

  case 481:

    {
    yyval.node = create_node(TOK_HEX,yyvsp[0].name);
  ;}
    break;

  case 482:

    {
    yyval.node = create_node(TOK_INTEGER,yyvsp[0].name);
  ;}
    break;

  case 483:

    {
    yyval.node = create_node(TOK_REAL,yyvsp[0].name);
  ;}
    break;

  case 484:

    {
    yyval.node = create_node(TOK_STRING,yyvsp[0].name);
  ;}
    break;

  case 485:

    {
    yyval.node = create_node(TOK_TRUE,yyvsp[0].name);
  ;}
    break;

  case 486:

    {
    yyval.node = create_node(TOK_FALSE,yyvsp[0].name);
  ;}
    break;

  case 487:

    {
    yyval.node = create_node(TOK_NIL,yyvsp[0].name);
  ;}
    break;


    }

/* Line 991 of yacc.c.  */


  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}






