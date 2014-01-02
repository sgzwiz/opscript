%{
#include "stdafx.h"
#include "syntaxTree.h"
#include "utils.h"
#ifdef _DEBUG
#define YYDEBUG 1
#endif

#pragma warning(disable : 4065) //default used with no case lables
#pragma warning(disable : 4102) // unreferenced label
%}

%union
{
  token_item name;
  syntaxNode *node;
}

%{

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

%}

%pure_parser

%token <name> TOK_CONST TOK_TYPE TOK_VAR TOK_SEMICOLON TOK_IDENTIFIER
%token <name> TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE TOK_THREADVAR
%token <name> TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_STRING TOK_REAL TOK_INTEGER TOK_HEX
%token <name> TOK_ARRAY TOK_LBRACKET TOK_RBRACKET TOK_OF TOK_RECORD TOK_END
%token <name> TOK_LPAREN TOK_RPAREN TOK_DOTDOT TOK_COLON TOK_COMMA
%token <name> TOK_ASSIGN TOK_REPEAT TOK_UNTIL TOK_WHILE TOK_DO TOK_AS
%token <name> TOK_IF TOK_THEN TOK_ELSE TOK_FOR TOK_TO TOK_DOWNTO TOK_RAISE
%token <name> TOK_CASE TOK_BEGIN TOK_AND TOK_OR TOK_XOR TOK_ON TOK_IN
%token <name> TOK_DIV TOK_MOD TOK_NOT TOK_PROGRAM TOK_DOT TOK_WITH TOK_SHL TOK_SHR
%token <name> TOK_FUNCTION TOK_PROCEDURE TOK_FORWARD TOK_TRUE TOK_FALSE
%token <name> TOK_USES TOK_CDECL TOK_DYNAMIC TOK_VIRTUAL TOK_LABEL
%token <name> TOK_EXPORT TOK_EXTERNAL TOK_FAR TOK_OVERRIDE
%token <name> TOK_OVERLOAD TOK_PASCAL TOK_MESSAGE TOK_REINTRODUCE TOK_SET 
%token <name> TOK_SAFECALL TOK_STDCALL TOK_OUT TOK_FILE TOK_PACKED  TOK_NIL
%token <name> TOK_OBJECT TOK_INHERITED TOK_GOTO TOK_CLASS TOK_PROTECTED 
%token <name> TOK_PUBLIC TOK_PRIVATE TOK_PUBLISHED TOK_PROPERTY TOK_AT
%token <name> TOK_READ TOK_WRITE TOK_DEFAULT TOK_STORED TOK_NODEFAULT 
%token <name> TOK_IMPLEMENTS TOK_INTERFACE TOK_UNIT TOK_IMPLEMENTATION
%token <name> TOK_CONSTRUCTOR TOK_DESTRUCTOR TOK_IS TOK_EXCEPT TOK_TRY TOK_FINALLY
%token <name> TOK_INITIALIZATION TOK_FINALIZATION TOK_ABSTRACT TOK_ASSEMBLER
%token <name> TOK_DEREF TOK_LAND TOK_LOR TOK_ADDR TOK_DISPID TOK_DISPINTERFACE
%token <name> TOK_READONLY TOK_WRITEONLY TOK_EXPRESSION TOK_PROCEDURE_EXPRESSION



%left TOK_OR TOK_XOR
%left TOK_AND

%nonassoc TOK_EQ TOK_NE TOK_LT TOK_GT TOK_LE TOK_GE

%left TOK_PLUS TOK_MINUS
%left TOK_STAR TOK_SLASH TOK_DIV TOK_MOD

%nonassoc TOK_NOT
%nonassoc PREC_UNARYOP

%nonassoc PREC_LOWERTHANELSE
%nonassoc TOK_ELSE

%token <name> TOK_FINAL_TOKEN

%start goal

%type <node> goal 
%type <node> program
%type <node> program_header
%type <node> opt_program_parameters
%type <node> identifier_list
%type <node> program_block
%type <node> uses_clause
%type <node> block
%type <node> implementation_declarations
%type <node> unit
%type <node> interface_section
%type <node> implementation_section
%type <node> opt_semicolon
%type <node> init_section
%type <node> directive_el
%type <node> interface_declarations
%type <node> routine_definition
%type <node> opt_external
%type <node> function_header
%type <node> procedure_header
%type <node> constructor_header
%type <node> destructor_header
%type <node> opt_parameter_list
%type <node> parameter_list
%type <node> formal_parameter 
%type <node> parameter 
%type <node> function_return
%type <node> const_definition_list
%type <node> type_declaration_list
%type <node> list_var_declaration
%type <node> typed_constant
%type <node> record_constant
%type <node> record_field_constant_list
%type <node> record_field_constant
%type <node> typed_constant_list
%type <node> array_constant
%type <node> const_definition
%type <node> restricted_type
%type <node> type_declaration
%type <node> var_declaration
%type <node> opt_packed
%type <node> struct_type 
%type <node> type_spec
%type <node> defptr_list
%type <node> simple_type
%type <node> simple_type_list
%type <node> field_list
%type <node> rec_variant
%type <node> rec_variant_list
%type <node> variant_section
%type <node> opt_field_list
%type <node> field_declaration
%type <node> varfield_declaration
%type <node> statement_raise
%type <node> statement
%type <node> with_list
%type <node> statement_with
%type <node> try_handler
%type <node> handler
%type <node> handler_list
%type <node> except_handler
%type <node> statement_try
%type <node> statement_assign
%type <node> statement_repeat
%type <node> statement_while
%type <node> statement_if
%type <node> statement_for
%type <node> statement_case
%type <node> compound_statement
%type <node> statement_list
%type <node> list_case_option
%type <node> case_item_list
%type <node> case_item
%type <node> case_option
%type <node> object_type
%type <node> object_inheritance
%type <node> object_body
%type <node> obj_field_list
%type <node> method_list
%type <node> directive_list
%type <node> method_heading
%type <node> class_type
%type <node> class_body
%type <node> class_heritage
%type <node> class_items_list
%type <node> class_items_section
%type <node> class_method_heading
%type <node> class_visibility
%type <node> property_definition
%type <node> opt_property_interface
%type <node> property_specifiers
%type <node> opt_read
%type <node> opt_write
%type <node> opt_stored
%type <node> opt_index
%type <node> opt_default
%type <node> opt_implements
%type <node> interface_type
%type <node> opt_guid
%type <node> interface_body
%type <node> as_expression
%type <node> shift_expression
%type <node> multiplicative_expression
%type <node> additive_expression
%type <node> relational_expression
%type <node> equality_expression
%type <node> and_expression
%type <node> exclusive_or_expression
%type <node> inclusive_or_expression
%type <node> logical_and_expression
%type <node> logical_or_expression
%type <node> simple_expression
%type <node> expression
%type <node> proc_func_expression
%type <node> expression_list
%type <node> record_expression
%type <node> array_elt_expression
%type <node> array_expression
%type <node> simple_const
%type <node> opt_dispid

%type <node> param_identifier_list
%type <node> external_name
%type <node> procedure_definition
%type <node> function_definition
%type <node> constructor_definition
%type <node> destructor_definition
%type <node> range_list
%type <node> script
%type <node> script_item_list
%type <node> identifier_rec_list
%type <node> opt_writeonly
%type <node> opt_readonly
%type <node> script_item
%type <node> global_expression 
%type <node> global_procedure
%type <node> method_type

%%

goal:
  program 
  {
    $$ = create_node(e_goal );
    $$->appendChild($1);
  
    goal($$);
  }
  |
  unit 
  {
    $$ = create_node(e_goal );
    $$->appendChild($1);
    goal($$);
  }
  |
  script
  {
    $$ = create_node(e_goal );
    $$->appendChild($1);
    goal($$);
  }
  |
  global_expression
  {
  }
  |
  global_procedure
  {
  }
  ;

global_expression:
  TOK_EXPRESSION expression
  {
  }
  ;

global_procedure:
  TOK_PROCEDURE_EXPRESSION routine_definition
  {
  }
  ;
  
script_item:
  statement
  {
    $$=$1;
  }
  |
  TOK_VAR var_declaration 
  {
    $$=$2;
  }
  |
  TOK_CONST const_definition
  {
    $$=$2;
  }
  |
  TOK_TYPE type_declaration
  {
   $$=$2;
  }
  |
  uses_clause
  {
    $$=$1;
  }
  |
  procedure_definition
  {
    $$=$1;
  }
  |
  function_definition
  {
    $$=$1;
  }
  |
  constructor_definition
  {
    $$=$1;
  }
  |
  destructor_definition
  {
    $$=$1;
  }
  ;
  
script:
  script_item_list 
  {
    $$=$1;
  }
  |
  script_item_list semicolon_list
  {
    $$=$1;
  } 
  ;
  
script_item_list:
  script_item
  {
    $$=addScriptItem($1);
  }
  |
  script_item_list semicolon_list script_item
  {
    $$=addScriptItem($3);
  }
  |
  script_item_list error script_item
  {
    // needed for ECMA compliance.
    clearSemicolonErr();
    $$=addScriptItem($3);
  }
  ;  

program:
  program_header TOK_SEMICOLON program_block TOK_DOT 
  {
    $$ = create_node(e_program);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  ;
  
program_header:
  TOK_PROGRAM TOK_IDENTIFIER opt_program_parameters 
  {
    $$ = create_node(e_program_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  ;
  
opt_program_parameters:
  {
    $$ = 0;
  }
  |
  TOK_LPAREN identifier_list TOK_RPAREN 
  {
    $$ = create_node(e_opt_program_parameters);
    $$->appendChild($2);
  }
  ;
  
identifier_list:
  TOK_IDENTIFIER 
  {
    $$ = create_node(e_identifier_list);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  identifier_list TOK_COMMA TOK_IDENTIFIER 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  ;
  
program_block:
  uses_clause block 
  {
    $$ = create_node(e_program_block);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  block 
  {
    $$ = create_node(e_program_block);
    $$->appendChild($1);
  }
  ;
  
uses_clause:
  TOK_USES identifier_list TOK_SEMICOLON 
  {
    $$ = create_node(e_uses_clause);
    $$->appendChild($2);
  }
  ;
  
block:
  implementation_declarations compound_statement 
  {
    $$ = create_node(e_block);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  compound_statement 
  {
    $$ = create_node(e_block);
    $$->appendChild($1);
  }
  ;
  
implementation_declarations:
  TOK_CONST const_definition_list 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($2);
  }
  |
  TOK_TYPE type_declaration_list 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($2);
  }
  |
  TOK_VAR list_var_declaration 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($2);
  }
  |
  TOK_LABEL identifier_list TOK_SEMICOLON 
  {
    syntaxNode *ptmp = create_node(TOK_LABEL,$1); 
    $$ = create_node(e_implementation_declarations);
    $$->appendChild(ptmp);
    ptmp->appendChild($2);
  }
  |
  TOK_THREADVAR list_var_declaration 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($2);
  }
  |
  routine_definition 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($1);
  }
  |
  TOK_CLASS routine_definition 
  {
    $$ = create_node(e_implementation_declarations);
    $$->appendChild($2);
  }
  |
  implementation_declarations TOK_CONST const_definition_list 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  implementation_declarations TOK_TYPE type_declaration_list 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  implementation_declarations TOK_VAR list_var_declaration 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  implementation_declarations TOK_LABEL identifier_list TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_LABEL,$2));
    $$->appendChild($3);
  }
  |
  implementation_declarations TOK_THREADVAR list_var_declaration 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  implementation_declarations routine_definition 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  |
  implementation_declarations TOK_CLASS routine_definition 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
unit:
  TOK_UNIT TOK_IDENTIFIER TOK_SEMICOLON 
  interface_section 
  implementation_section 
  init_section 
  TOK_DOT 
  {
    $$ = create_node(e_unit);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($4);
    $$->appendChild($5);
    $$->appendChild($6);
  }
  ;
    
interface_section:
  TOK_INTERFACE 
  {
    $$ = create_node(e_interface_section);
  }
  |
  TOK_INTERFACE uses_clause interface_declarations 
  {
    $$ = create_node(e_interface_section);
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_INTERFACE interface_declarations 
  {
    $$ = create_node(e_interface_section);
    $$->appendChild($2);
  }
  |
  TOK_INTERFACE uses_clause 
  {
    $$ = create_node(e_interface_section);
    $$->appendChild($2);
  }
  ;
  
implementation_section:
  TOK_IMPLEMENTATION 
  {
    $$ = create_node(e_implementation_section);
  }
  |
  TOK_IMPLEMENTATION uses_clause 
  {
    $$ = create_node(e_implementation_section);
    $$->appendChild($2);
  }
  |
  TOK_IMPLEMENTATION implementation_declarations 
  {
    $$ = create_node(e_implementation_section);
    $$->appendChild($2);
  }
  |
  TOK_IMPLEMENTATION uses_clause implementation_declarations 
  {
    $$ = create_node(e_implementation_section);
    $$->appendChild($2);
    $$->appendChild($3);
  }
  ;
  
opt_semicolon:
  {
    $$ = 0;
  }
  |
  TOK_SEMICOLON 
  {
    $$ = 0;
  }
  ;
  
init_section:
  TOK_INITIALIZATION statement_list opt_semicolon TOK_END 
  {
    $$ = create_node(e_init_section);
    $$->appendChild($2);
  }
  |
  TOK_INITIALIZATION statement_list opt_semicolon TOK_FINALIZATION statement_list opt_semicolon TOK_END 
  {
    $$ = create_node(e_init_section);
    $$->appendChild($2);
    $$->appendChild($5);
  }
  |
  TOK_BEGIN statement_list opt_semicolon TOK_END 
  {
    $$ = create_node(e_init_section);
    $$->appendChild($2);
  }
  |
  TOK_INITIALIZATION TOK_END
  {
    $$ = 0;
  }
  |
  TOK_INITIALIZATION TOK_FINALIZATION TOK_END
  {
    $$ = 0;
  }
  |
  TOK_END 
  {
    $$ = 0;
  }
  ;
  
directive_el:
  TOK_CDECL 
  {
    $$ = create_node(TOK_CDECL,$1);
  }
  |
  TOK_EXPORT 
  {
    $$ = create_node(TOK_EXPORT,$1);
  }
  |
  TOK_FAR 
  {
    $$ = create_node(TOK_FAR,$1);
  }
  |
  TOK_PASCAL 
  {
    $$ = create_node(TOK_PASCAL,$1);
  }
  |
  TOK_SAFECALL 
  {
    $$ = create_node(TOK_SAFECALL,$1);
  }
  |
  TOK_STDCALL 
  {
    $$ = create_node(TOK_STDCALL,$1);
  }
  |
  TOK_VIRTUAL 
  {
    $$ = create_node(TOK_VIRTUAL,$1);
  }
  |
  TOK_ABSTRACT 
  {
    $$ = create_node(TOK_ABSTRACT,$1);
  }
  |
  TOK_DYNAMIC 
  {
    $$ = create_node(TOK_DYNAMIC,$1);
  }
  |
  TOK_OVERRIDE 
  {
    $$ = create_node(TOK_OVERRIDE,$1);
  }
  |
  TOK_REINTRODUCE 
  {
    $$ = create_node(TOK_REINTRODUCE,$1);
  }
  |
  TOK_OVERLOAD 
  {
    $$ = create_node(TOK_OVERLOAD,$1);
  }
  |
  TOK_DISPID TOK_INTEGER
  {
    $$ = create_node(TOK_DISPID,$1);
    $$->appendChild(create_node(TOK_INTEGER,$2));
    $$->children[0]->negate_ = 0;
  }
  |
  TOK_DISPID TOK_MINUS TOK_INTEGER
  {
    $$ = create_node(TOK_DISPID,$1);
    $$->appendChild(create_node(TOK_INTEGER,$2));
    $$->children[0]->negate_ = 1;
  }
  ;
  
interface_declarations:
  TOK_CONST const_definition_list 
  {
    $$ = create_node(e_interface_declarations);
    $$->appendChild($2);
  }
  |
  TOK_TYPE type_declaration_list 
  {
    $$ = create_node(e_interface_declarations);
    $$->appendChild($2);
  }
  |
  TOK_VAR list_var_declaration 
  {
    $$ = create_node(e_interface_declarations);
    $$->appendChild($2);
  }
  |
  TOK_THREADVAR list_var_declaration 
  {
    $$ = create_node(e_interface_declarations);
    $$->appendChild($2);
  }
  |
  procedure_header TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    $$ = create_node(e_interface_declarations);
    $$->appendChild(pmethod);
      pmethod->appendChild($1);
  }
  |
  procedure_header opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild(pdirective);
    pdirective->appendChild($2);
  }
  |
  procedure_header directive_list TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($2);
  }
  |
  procedure_header TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    ptmp = create_node(TOK_MESSAGE,$3);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$4));
    pdirective->appendChild(ptmp);

    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild(pdirective);
  }
  |
  procedure_header TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER directive_list TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    ptmp=create_node(TOK_MESSAGE,$3);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$4));
    $5->appendChild(ptmp);

    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($5);
  }
  |
  procedure_header directive_list opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($2);
    $2->appendChild($3);
  }
  |
  function_header TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);

    $$ = create_node(e_interface_declarations);
    
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
  }
  |
  function_header opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    $$ = create_node(e_interface_declarations);
    
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild(pdirective);
    pdirective->appendChild($2);
  }
  |
  function_header directive_list TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);

    $$ = create_node(e_interface_declarations);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($2);
  }
  |
  function_header directive_list opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);

    $$ = create_node(e_interface_declarations);
    
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($2);
    $2->appendChild($3);
  }
  |
  interface_declarations TOK_CONST const_definition_list 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  interface_declarations TOK_TYPE type_declaration_list 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  interface_declarations TOK_VAR list_var_declaration 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  interface_declarations TOK_THREADVAR list_var_declaration 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  interface_declarations procedure_header TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
  }
  |
  interface_declarations procedure_header TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);

    ptmp = create_node(TOK_MESSAGE,$4);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$5));
    pdirective->appendChild(ptmp);

    $$ = $1;
   
    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild(pdirective);
  }
  |
  interface_declarations procedure_header directive_list TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_procedure_declaration);
    
    $$ = $1;
    
    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($3);
  }
  |
  interface_declarations procedure_header TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER directive_list TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    ptmp = create_node(TOK_MESSAGE,$4);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$5));

    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($6);
    $6->appendChild(ptmp);
  }
  |
  interface_declarations procedure_header opt_external TOK_SEMICOLON 
  {
    $$ = $1;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($3);
  }
  |
  interface_declarations procedure_header directive_list opt_external TOK_SEMICOLON 
  {
    $$ = $1;
    syntaxNode *pmethod = create_node(e_procedure_declaration);

    $$->appendChild(pmethod );
    pmethod->appendChild($2);
    pmethod->appendChild($3);
    $3->appendChild($4);
  }
  |
  interface_declarations function_header TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);
    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
  }
  |
  interface_declarations function_header directive_list TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);
    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($3);
  }
  |
  interface_declarations function_header opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);
    syntaxNode *pdirective = create_node(e_directive_list);
    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pdirective->appendChild($3);
  }
  |
  interface_declarations function_header directive_list opt_external TOK_SEMICOLON 
  {
    syntaxNode *pmethod = create_node(e_function_declaration);
    $$ = $1;

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($3);
    $3->appendChild($4);
  }
  ;
  
routine_definition:
  function_definition TOK_SEMICOLON 
  {
    $$ = $1;
  }
  |
  procedure_definition TOK_SEMICOLON 
  {
    $$ = $1;
  }
  |
  constructor_definition TOK_SEMICOLON 
  {
    $$ = $1;
  }
  |
  destructor_definition TOK_SEMICOLON 
  {
    $$ = $1;
  }
  ;
  
function_definition:
  function_header TOK_SEMICOLON block 
  {
    $$ = create_node(e_function_definition);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  function_header opt_external 
  {
    syntaxNode *pdirective=create_node(e_directive_list);
    
    $$ = create_node(e_function_definition);
    
    $$->appendChild($1);
    $$->appendChild(pdirective);
    pdirective->appendChild($2);
  }
  |
  function_header directive_list TOK_SEMICOLON block 
  {
    $$ = create_node(e_function_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  |
  function_header directive_list opt_external 
  {
    $$ = create_node(e_function_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $2->appendChild($3);
  }
  |
  function_header TOK_SEMICOLON TOK_ASSEMBLER 
  {
    $$ = 0;
  }
  |
  function_header TOK_SEMICOLON TOK_FORWARD 
  {
    $$ = create_node(e_function_declaration);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_FORWARD,$3));
  }
  ;
  
external_name:
  TOK_IDENTIFIER
  {
    $$ = create_node(TOK_IDENTIFIER,$1)
  }
  |
  TOK_STRING
  {
   $$ = create_node(TOK_STRING ,$1)
  }
  ;

opt_external:
  // external 'mydll.dll'
  TOK_SEMICOLON TOK_EXTERNAL external_name
  {
    $$=create_node(TOK_EXTERNAL,$2);
    $$->children[0]->appendChild($3);
  }
  |
  // external 'user.dll' name MessageBoxA
  TOK_SEMICOLON TOK_EXTERNAL external_name TOK_IDENTIFIER external_name
  {
    $$=create_node(TOK_EXTERNAL,$2);
    $$->children[0]->appendChild($3);
    $$->children[0]->appendChild($5);
  }
  ;
  
procedure_definition:
  procedure_header TOK_SEMICOLON block 
  {
    $$ = create_node(e_procedure_definition);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  procedure_header opt_external 
  {
    syntaxNode *pdirective=create_node(e_directive_list);
    $$ = create_node(e_procedure_definition);
    $$->appendChild($1);
    pdirective->appendChild($2);
    $$->appendChild(pdirective);
  }
  |
  procedure_header directive_list TOK_SEMICOLON block 
  {
    $$ = create_node(e_procedure_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  |
  procedure_header directive_list opt_external 
  {
    $$ = create_node(e_procedure_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $2->appendChild($3);
  }
  |
  procedure_header TOK_SEMICOLON TOK_ASSEMBLER 
  {
    $$ = 0;
  }
  |
  procedure_header TOK_SEMICOLON TOK_FORWARD 
  {
    $$ = create_node(e_procedure_declaration);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_FORWARD,$3));
  }
  ;
  
constructor_definition:
  constructor_header TOK_SEMICOLON block 
  {
    $$ = create_node(e_constructor_definition);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  constructor_header directive_list TOK_SEMICOLON block 
  {
    $$ = create_node(e_constructor_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  ;
  
destructor_definition:
  destructor_header TOK_SEMICOLON block 
  {
    $$ = create_node(e_destructor_definition);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  destructor_header directive_list TOK_SEMICOLON block 
  {
    $$ = create_node(e_destructor_definition);
    $$->appendChild($1);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  ;

function_header:
  TOK_FUNCTION TOK_IDENTIFIER opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
    $$->appendChild($4);
  }
  |
  TOK_FUNCTION TOK_READ opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
    $$->appendChild($4);
  }
  |
  TOK_FUNCTION TOK_WRITE opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
    $$->appendChild($4);
  }
  |
  TOK_FUNCTION TOK_IDENTIFIER TOK_DOT TOK_IDENTIFIER opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
    $$->appendChild($6);
  }
  |
  TOK_FUNCTION TOK_IDENTIFIER TOK_DOT TOK_READ opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
    $$->appendChild($6);
  }
  |
  TOK_FUNCTION TOK_IDENTIFIER TOK_DOT TOK_WRITE opt_parameter_list function_return 
  {
    $$ = create_node(e_function_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
    $$->appendChild($6);
  }
  ;
  
procedure_header:
  TOK_PROCEDURE TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  |
  TOK_PROCEDURE TOK_WRITE opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  |
  TOK_PROCEDURE TOK_READ opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  |
  TOK_PROCEDURE TOK_IDENTIFIER TOK_DOT TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
  }
  |
  TOK_PROCEDURE TOK_IDENTIFIER TOK_DOT TOK_READ opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
  }
  |
  TOK_PROCEDURE TOK_IDENTIFIER TOK_DOT TOK_WRITE opt_parameter_list 
  {
    $$ = create_node(e_procedure_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
  }
  ;
  
constructor_header:
  TOK_CONSTRUCTOR TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_constructor_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  |
  TOK_CONSTRUCTOR TOK_IDENTIFIER TOK_DOT TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_constructor_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
  }
  ;
  
destructor_header:
  TOK_DESTRUCTOR TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_destructor_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
  }
  |
  TOK_DESTRUCTOR TOK_IDENTIFIER TOK_DOT TOK_IDENTIFIER opt_parameter_list 
  {
    $$ = create_node(e_destructor_header);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild($5);
  }
  ;
  
opt_parameter_list:
  {
    $$ = 0;
  }
  |
  TOK_LPAREN parameter_list TOK_RPAREN 
  {
    $$ = $2;
  }
  |
  TOK_LPAREN TOK_RPAREN 
  {
    $$ = 0;
  }
  ;
  
parameter_list:
  formal_parameter 
  {
    $$ = create_node(e_parameter_list);
    $$->appendChild($1);
  }
  |
  parameter_list TOK_SEMICOLON formal_parameter 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
formal_parameter:
  TOK_VAR parameter 
  {
    $$ = create_node(e_formal_parameter );
    $$->appendChild(create_node(TOK_VAR,$1));
    $$->appendChild($2);
  }
  |
  TOK_CONST parameter 
  {
    $$ = create_node(e_formal_parameter );
    $$->appendChild(create_node(TOK_CONST,$1));
    $$->appendChild($2);
  }
  |
  TOK_OUT parameter 
  {
    $$ = create_node(e_formal_parameter );
    $$->appendChild(create_node(TOK_OUT,$1));
    $$->appendChild($2);
  }
  |
  parameter 
  {
    $$ = $1;
  }
  ;

param_identifier_list:
  TOK_IDENTIFIER 
  {
    $$ = create_node(e_identifier_list);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_MESSAGE
  {
    $$ = create_node(e_identifier_list);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  param_identifier_list TOK_COMMA TOK_IDENTIFIER 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  |
  param_identifier_list TOK_COMMA TOK_MESSAGE 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  ;

parameter:
  param_identifier_list 
  {
    $$ = create_node(e_parameter );
    $$->appendChild($1);
  }
  |
  param_identifier_list TOK_COLON simple_type 
  {
    $$ = create_node(e_parameter );
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  param_identifier_list TOK_COLON simple_type TOK_EQ expression 
  {
    $$ = create_node(e_parameter );
    $$->appendChild($1);
    $$->appendChild($3);
    $$->appendChild(create_node(TOK_EQ,$4));
    $$->appendChild($5);
  }
  |
  param_identifier_list TOK_COLON TOK_ARRAY TOK_OF simple_type 
  {
    $$ = create_node(e_parameter );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_ARRAY,$3));
    $$->appendChild(create_node(TOK_OF,$4));
    $$->appendChild($5);
  }
  |
  param_identifier_list TOK_COLON TOK_ARRAY TOK_OF TOK_CONST 
  {
    $$ = create_node(e_parameter );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_ARRAY,$3));
    $$->appendChild(create_node(TOK_OF,$4));
    $$->appendChild(create_node(TOK_CONST,$5));
  }
  ;
  
function_return:
  {
    $$ = 0;
  }
  |
  TOK_COLON simple_type 
  {
    $$ = create_node(e_function_return);
    $$->appendChild($2);
  }
  ;
  
const_definition_list:
  const_definition TOK_SEMICOLON 
  {
    $$ = create_node(e_const_definition_list);
    $$->appendChild($1);
  }
  |
  const_definition_list const_definition TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  ;
  
type_declaration_list:
  type_declaration TOK_SEMICOLON 
  {
    $$ = create_node(e_type_declaration_list);
    $$->appendChild($1);
  }
  |
  type_declaration directive_list TOK_SEMICOLON
  {
    $$ = create_node(e_type_declaration_list);
    $$->appendChild($1);
    $1->appendChild($2);
  }
  |
  TOK_IDENTIFIER TOK_EQ TOK_CLASS TOK_SEMICOLON 
  {
    $$ = create_node(e_type_declaration_list);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild(create_node(TOK_EQ,$2));
    $$->appendChild(create_node(TOK_CLASS,$3));
  }
  |
  type_declaration_list type_declaration TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  |
  type_declaration_list type_declaration directive_list TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
    $2->appendChild($3);
  }
  |
  type_declaration_list TOK_IDENTIFIER TOK_EQ TOK_CLASS TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_EQ,$3));
    $$->appendChild(create_node(TOK_CLASS,$4));
  }
  ;
  
list_var_declaration:
  var_declaration TOK_SEMICOLON 
  {
    $$ = create_node(e_list_var_declaration);
    $$->appendChild($1);
  }
  |
  var_declaration directive_list TOK_SEMICOLON 
  {
    $$ = create_node(e_list_var_declaration);
    $$->appendChild($1);
    $1->appendChild($2);
  }
  |
  list_var_declaration var_declaration TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  |
  list_var_declaration var_declaration directive_list TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
    $2->appendChild($3);
  }
  ;
  
typed_constant:
  expression 
  {
    $$ = create_node(e_typed_constant);
    $$->appendChild($1);
  }
  |
  record_constant 
  {
    $$ = create_node(e_typed_constant);
    $$->appendChild($1);
  }
  |
  array_constant 
  {
    $$ = create_node(e_typed_constant);
    $$->appendChild($1);
  }
  ;
  
record_constant:
  TOK_LPAREN record_field_constant_list TOK_RPAREN 
  {
    $$ = $2;
  }
  ;
  
record_field_constant_list:
  record_field_constant 
  {
    $$ = create_node(e_record_field_constant_list);
    $$->appendChild($1);
  }
  |
  record_field_constant_list TOK_SEMICOLON record_field_constant 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
record_field_constant:
  TOK_IDENTIFIER TOK_COLON typed_constant 
  {
    $$ = create_node(e_record_field_constant);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild(create_node(TOK_COLON,$2));
    $$->appendChild($3);
  }
  ;
  
typed_constant_list:
  typed_constant 
  {
    $$ = create_node(e_typed_constant_list);
    $$->appendChild($1);
  }
  |
  typed_constant_list TOK_COMMA typed_constant 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
array_constant:
  TOK_LPAREN typed_constant_list TOK_RPAREN 
  {
    $$ = create_node(e_array_constant);
    $$->appendChild($2);
  }
  ;
  
const_definition:
  TOK_IDENTIFIER TOK_EQ expression 
  {
    $$ = create_node(e_const_definition);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  TOK_IDENTIFIER TOK_COLON type_spec TOK_EQ typed_constant 
  {
    $$ = create_node(e_const_definition);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
    $$->appendChild($5);
  }
  ;
  
restricted_type:
  class_type 
  {
    $$ = $1;
  }
  |
  interface_type 
  {
    $$ = $1;
  }
  |
  object_type 
  {
    $$ = $1;
  }
  ;
  
type_declaration:
  TOK_IDENTIFIER TOK_EQ type_spec 
  {
    $$ = create_node(e_type_declaration);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  TOK_IDENTIFIER TOK_EQ restricted_type 
  {
    $$ = create_node(e_type_declaration);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  ;

var_declaration:
  varfield_declaration 
  {
      $$ = $1;
  }
  ;
  
opt_packed:
  {
    $$ = 0;
  }
  |
  TOK_PACKED 
  {
    $$ = create_node(TOK_PACKED,$1);
  }
  ;
  
struct_type:
  opt_packed TOK_ARRAY TOK_LBRACKET simple_type_list TOK_RBRACKET TOK_OF type_spec 
  {
    $$ = create_node(e_struct_type );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_ARRAY,$2));
    $$->appendChild($4);
    $$->appendChild($7);
  }
  |
  opt_packed TOK_ARRAY TOK_OF type_spec 
  {
    $$ = create_node(e_struct_type );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_ARRAY,$2));
    $$->appendChild($4);
  }
  |
  opt_packed TOK_RECORD opt_field_list TOK_END 
  {
    $$ = create_node(e_struct_type );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_RECORD,$2));
    $$->appendChild($3);
  }
  |
  opt_packed TOK_SET TOK_OF simple_type 
  {
    $$ = create_node(e_struct_type );
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_SET,$2));
    $$->appendChild($4);
  }
  ;
  
method_type:
  TOK_PROCEDURE opt_parameter_list 
  {
    $$ = create_node(e_procedure_type);
    $$->appendChild(create_node(TOK_PROCEDURE,$1));
    $$->appendChild($2);
  }
  |
  TOK_FUNCTION opt_parameter_list function_return 
  {
    $$ = create_node(e_function_type);
    $$->appendChild(create_node(TOK_FUNCTION,$1));
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_FUNCTION opt_parameter_list function_return directive_el
  {
    $$ = create_node(e_function_type);
    $$->appendChild(create_node(TOK_FUNCTION,$1));
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_PROCEDURE opt_parameter_list TOK_OF TOK_OBJECT 
  {
    $$ = create_node(e_object_procedure_type);
    $$->appendChild(create_node(TOK_PROCEDURE,$1));
    $$->appendChild($2);
  }
  |
  TOK_FUNCTION opt_parameter_list function_return TOK_OF TOK_OBJECT 
  {
    $$ = create_node(e_object_function_type);
    $$->appendChild(create_node(TOK_FUNCTION,$1));
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_CLASS TOK_OF TOK_IDENTIFIER 
  {
    $$ = create_node(e_type_spec);
    $$->appendChild(create_node(TOK_CLASS,$1));
    $$->appendChild(create_node(TOK_OF,$2));
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  |
  TOK_CLASS class_heritage 
  {
    $$ = create_node(e_type_spec);
    $$->appendChild(create_node(TOK_CLASS,$1));
    $$->appendChild($2);
  }
  ;
  
type_spec:
  simple_type 
  {
    $$ = $1;
  }
  |
  struct_type 
  {
    $$ = $1;
  }
  |
  method_type
  {
    $$ = $1;
  }
  ;
  
defptr_list:
  TOK_DEREF 
  {
    $$ = create_node(e_defptr_list);
    $$->appendChild(create_node(TOK_DEREF,$1));
  }
  |
  defptr_list TOK_DEREF 
  {
    $$ = $1;
    $$->appendChild(create_node(TOK_DEREF,$2));
  }
  ;
  
simple_type:
  TOK_IDENTIFIER 
  {
    $$ = create_node(e_simple_type);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_IDENTIFIER TOK_DOT TOK_IDENTIFIER
  {
    $$ = create_node(e_simple_type);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  |
  TOK_IDENTIFIER TOK_LBRACKET additive_expression TOK_RBRACKET 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_simple_type);
    ptmp = create_node(e_sized_string);
    $$->appendChild(ptmp);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$1));
    ptmp->appendChild($3);
  }
  |
  TOK_TYPE TOK_IDENTIFIER 
  {
    $$ = create_node(e_simple_type);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  defptr_list TOK_IDENTIFIER 
  {
    $$ = create_node(e_simple_type);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_LPAREN identifier_list TOK_RPAREN 
  {
    $$ = create_node(e_simple_type);
    $$->appendChild($2);
  }
  |
  additive_expression TOK_DOTDOT additive_expression 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_simple_type);
    ptmp = create_node(e_ranged_item);
    $$->appendChild(ptmp);
    ptmp->appendChild($1);
    ptmp->appendChild($3);
  }
  ;
  
simple_type_list:
  simple_type 
  {
    $$ = create_node(e_simple_type_list);
    $$->appendChild($1);
  }
  |
  simple_type_list TOK_COMMA simple_type 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
opt_field_list:
  {
    $$ = create_node(e_opt_field_list);
  }
  |
  field_list 
  {
    $$ = create_node(e_opt_field_list);
    $$->appendChild($1);
  }
  |
  field_list TOK_SEMICOLON 
  {
    $$ = create_node(e_opt_field_list);
    $$->appendChild($1);
  }
  |
  variant_section 
  {
    $$ = create_node(e_opt_field_list);
    $$->appendChild($1);
  }
  |
  field_list variant_section 
  {
    $$ = create_node(e_opt_field_list);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  field_list TOK_SEMICOLON variant_section 
  {
    $$ = create_node(e_opt_field_list);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  ;

field_list:
  field_declaration 
  {
    $$ = create_node(e_field_list);
    $$->appendChild($1);
  }
  |
  field_list TOK_SEMICOLON field_declaration 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
field_declaration:
  varfield_declaration 
  {
    $$=$1;
  }
  ;
  
varfield_declaration:
  param_identifier_list TOK_COLON type_spec 
  {
    $$ = create_node(e_varfield_declaration);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  param_identifier_list TOK_COLON type_spec TOK_EQ expression 
  {
    $$ = create_node(e_varfield_declaration);
    $$->appendChild($1);
    $$->appendChild($3);
    $$->appendChild(create_node(TOK_EQ,$4));
    $$->appendChild($5);
  }
  |
  param_identifier_list TOK_COLON type_spec TOK_EQ TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_varfield_declaration);
    $$->appendChild($1);
    $$->appendChild($3);
    $$->appendChild(create_node(TOK_EQ,$4));
    $$->appendChild($6);
  }
  ;

variant_section:
  TOK_CASE simple_type TOK_OF rec_variant_list 
  {
    $$ = create_node(e_variant_section);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
  }
  |
  TOK_CASE TOK_IDENTIFIER TOK_COLON simple_type TOK_OF rec_variant_list 
  {
    $$ = create_node(e_variant_section);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_COLON,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_OF,$5));
    $$->appendChild($6);
  }
  ;

rec_variant_list:
  rec_variant TOK_SEMICOLON 
  {
    $$ = create_node(e_rec_variant_list);
    $$->appendChild($1);
  }
  |
  rec_variant_list rec_variant TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  ;

rec_variant:
  expression_list TOK_COLON TOK_LPAREN opt_field_list TOK_RPAREN 
  {
    $$ = create_node(e_rec_variant);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_COLON,$2));
    $$->appendChild($4);
  }
  ;
  
statement:
  statement_raise 
  {
    $$ = $1;
  }
  |
  statement_with 
  {
    $$ = $1;
  }
  |
  statement_assign 
  {
    $$ = $1;
  }
  |
  statement_repeat 
  {
    $$ = $1;
  }
  |
  statement_while 
  {
    $$ = $1;
  }
  |
  statement_if 
  {
    $$ = $1;
  }
  |
  statement_for 
  {
    $$ = $1;
  }
  |
  statement_case 
  {
    $$ = $1;
  }
  |
  compound_statement 
  {
    $$ = $1;
  }
  |
  statement_try 
  {
    $$ = $1;
  }
  |
  simple_expression 
  {
    $$ = create_node(e_statement);
    $$->appendChild($1);
  }
  |
  TOK_INHERITED 
  {
    $$ = create_node(e_statement);
    $$->appendChild(create_node(TOK_INHERITED,$1));
  }
  |
  TOK_GOTO TOK_IDENTIFIER 
  {
    $$ = create_node(e_statement_goto);
    $$->appendChild(create_node(TOK_GOTO,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_IDENTIFIER TOK_COLON statement
  {
    $$ = create_node(e_statement_label);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  TOK_IDENTIFIER TOK_COLON 
  {
    $$ = create_node(e_statement_label);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  ;

statement_raise:
  TOK_RAISE 
  {
    $$ = create_node(e_statement_raise);
    $$->appendChild(create_node(TOK_RAISE,$1));
  }
  |
  TOK_RAISE expression 
  {
    $$ = create_node(e_statement_raise);
    $$->appendChild(create_node(TOK_RAISE,$1));
    $$->appendChild($2);
  }
  |
  TOK_RAISE expression TOK_AT expression 
  {
    $$ = create_node(e_statement_raise);
    $$->appendChild(create_node(TOK_RAISE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_AT,$3));
    $$->appendChild($4);
  }
  ;
  
statement_with:
  TOK_WITH with_list TOK_DO statement 
  {
    $$ = create_node(e_statement_with);
    $$->appendChild(create_node(TOK_WITH,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_DO,$3));
    $$->appendChild($4);
  }
  ;
  
with_list:
  as_expression 
  {
    $$ = create_node(e_with_list);
    $$->appendChild($1);
  }
  |
  with_list TOK_COMMA as_expression 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;

statement_try:
  TOK_TRY statement_list try_handler TOK_END 
  {
    $$ = create_node(e_statement_try);
    $$->appendChild(create_node(TOK_TRY,$1));
    $$->appendChild($2);
    $$->appendChild($3);
    $$->appendChild(create_node(TOK_END,$4));
  }
  |
  TOK_TRY statement_list TOK_SEMICOLON try_handler TOK_END 
  {
    $$ = create_node(e_statement_try);
    $$->appendChild(create_node(TOK_TRY,$1));
    $$->appendChild($2);
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_END,$5));
  }
  ;

try_handler:
  TOK_FINALLY statement_list 
  {
    $$ = create_node(e_try_handler);
    $$->appendChild(create_node(TOK_FINALLY,$1));
    $$->appendChild($2);
  }
  |
  TOK_FINALLY TOK_SEMICOLON statement_list 
  {
    $$ = create_node(e_try_handler);
    $$->appendChild(create_node(TOK_FINALLY,$1));
    $$->appendChild($3);
  }
  |
  TOK_FINALLY statement_list TOK_SEMICOLON 
  {
    $$ = create_node(e_try_handler);
    $$->appendChild(create_node(TOK_FINALLY,$1));
    $$->appendChild($2);
  }
  |
  TOK_FINALLY TOK_SEMICOLON statement_list TOK_SEMICOLON 
  {
    $$ = create_node(e_try_handler);
    $$->appendChild(create_node(TOK_FINALLY,$1));
    $$->appendChild($3);
  }
  |
  except_handler 
  {
    $$ = create_node(e_try_handler);
    $$->appendChild($1);
  }
  ;

except_handler:
  TOK_EXCEPT statement_list 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
    $$->appendChild($2);
  }
  |
  TOK_EXCEPT statement_list TOK_SEMICOLON 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
    $$->appendChild($2);
  }
  |
  TOK_EXCEPT handler_list 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
    $$->appendChild($2);
  }
  |
  TOK_EXCEPT handler_list TOK_ELSE statement_list 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_ELSE,$3));
    $$->appendChild($4);
  }
  |
  TOK_EXCEPT handler_list TOK_ELSE statement_list TOK_SEMICOLON 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_ELSE,$3));
    $$->appendChild($4);
  }
  |
  TOK_EXCEPT 
  {
    $$ = create_node(e_except_handler);
    $$->appendChild(create_node(TOK_EXCEPT,$1));
  }
  ;
  
handler_list:
  handler TOK_SEMICOLON 
  {
    $$ = create_node(e_handler_list);
    $$->appendChild($1);
  }
  |
  handler_list handler TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  ;

handler:
  TOK_ON TOK_IDENTIFIER TOK_DO statement 
  {
    $$ = create_node(e_handler);
    $$->appendChild(create_node(TOK_ON,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_DO,$3));
    $$->appendChild($4);
  }
  |
  TOK_ON TOK_IDENTIFIER TOK_COLON TOK_IDENTIFIER TOK_DO statement 
  {
    $$ = create_node(e_handler);
    $$->appendChild(create_node(TOK_ON,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild(create_node(TOK_COLON,$3));
    $$->appendChild(create_node(TOK_IDENTIFIER,$4));
    $$->appendChild(create_node(TOK_DO,$5));
    $$->appendChild($6);
  }
  ;
  
statement_assign:
  simple_expression TOK_ASSIGN expression 
  {
    $$ = create_node(e_statement_assign);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  ;
  
statement_repeat:
  TOK_REPEAT statement_list TOK_UNTIL expression 
  {
    $$ = create_node(e_statement_repeat);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  |
  TOK_REPEAT statement_list TOK_SEMICOLON TOK_UNTIL expression 
  {
    $$ = create_node(e_statement_repeat);
    $$->appendChild($2);
    $$->appendChild($5);
  }
  ;
  
statement_while:
  TOK_WHILE expression TOK_DO statement 
  {
    $$ = create_node(e_statement_while);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  ;
  
statement_if:
  TOK_IF expression TOK_THEN statement %prec PREC_LOWERTHANELSE 
  {
    $$ = create_node(e_statement_if);
    $$->appendChild($2);
    $$->appendChild($4);
  }
  |
  TOK_IF expression TOK_THEN statement TOK_ELSE statement 
  {
    $$ = create_node(e_statement_if);
    $$->appendChild($2);
    $$->appendChild($4);
    $$->appendChild($6);
  }
  ;
  
statement_for:
  TOK_FOR TOK_IDENTIFIER TOK_ASSIGN expression TOK_TO expression TOK_DO statement 
  {
    $$ = create_node(e_statement_for);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_TO,$5));
    $$->appendChild($6);
    $$->appendChild($8);
  }
  |
  TOK_FOR TOK_IDENTIFIER TOK_ASSIGN expression TOK_DOWNTO expression TOK_DO statement 
  {
    $$ = create_node(e_statement_for);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_DOWNTO,$5));
    $$->appendChild($6);
    $$->appendChild($8);
  }
  ;
  
statement_case:
  TOK_CASE expression TOK_OF list_case_option TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_END,$5));
  }
  |
  TOK_CASE expression TOK_OF list_case_option TOK_SEMICOLON TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_END,$6));
  }
  |
  TOK_CASE expression TOK_OF list_case_option TOK_ELSE statement_list TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_ELSE,$5));
    $$->appendChild($6);
    $$->appendChild(create_node(TOK_END,$7));
  }
  |
  TOK_CASE expression TOK_OF list_case_option TOK_SEMICOLON TOK_ELSE statement_list TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_ELSE,$6));
    $$->appendChild($7);
    $$->appendChild(create_node(TOK_END,$8));
  }
  |
  TOK_CASE expression TOK_OF list_case_option TOK_ELSE statement_list TOK_SEMICOLON TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_ELSE,$5));
    $$->appendChild($6);
    $$->appendChild(create_node(TOK_END,$8));
  }
  |
  TOK_CASE expression TOK_OF list_case_option TOK_SEMICOLON TOK_ELSE statement_list TOK_SEMICOLON TOK_END 
  {
    $$ = create_node(e_statement_case);
    $$->appendChild(create_node(TOK_CASE,$1));
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_OF,$3));
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_ELSE,$6));
    $$->appendChild($7);
    $$->appendChild(create_node(TOK_END,$9));
  }
  ;

semicolon_list:
   TOK_SEMICOLON
   {
   }
   |
   semicolon_list TOK_SEMICOLON
   {
   }
   ;
  
compound_statement:
  TOK_BEGIN statement_list TOK_END 
  {
    $$ = $2;
  }
  |
  TOK_BEGIN statement_list semicolon_list TOK_END 
  {
    $$ = $2;
  }
  |
  TOK_BEGIN TOK_END 
  {
    $$ = create_node(e_statement_list);
  }
  ;
  
statement_list:
  statement 
  {
    $$ = create_node(e_statement_list);
    $$->appendChild($1);
  }
  |
  statement_list semicolon_list statement 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
list_case_option:
  case_option 
  {
    $$ = create_node(e_list_case_option);
    $$->appendChild($1);
  }
  |
  list_case_option TOK_SEMICOLON case_option 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
case_item_list:
  case_item 
  {
    $$ = create_node(e_case_item_list);
    $$->appendChild($1);
  }
  |
  case_item_list TOK_COMMA case_item 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
case_item:
  expression 
  {
    $$ = create_node(e_case_item);
    $$->appendChild($1);
  }
  |
  expression TOK_DOTDOT expression 
  {
    $$ = create_node(e_case_item);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_DOTDOT,$2));
    $$->appendChild($3);
  }
  ;
  
case_option:
  case_item_list TOK_COLON statement 
  {
    $$ = create_node(e_case_option);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  case_item_list TOK_COLON 
  {
    $$ = create_node(e_case_option);
    $$->appendChild($1);
  }
  ;
  
  
object_type:
  TOK_OBJECT object_inheritance object_body TOK_END 
  {
    $$ = create_node(e_object_type);
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_OBJECT object_body TOK_END 
  {
    $$ = create_node(e_object_type);
    $$->appendChild($2);
  }
  ;
  
object_inheritance:
  TOK_LPAREN TOK_IDENTIFIER TOK_RPAREN 
  {
    $$ = create_node(e_object_inheritance);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  ;
  
object_body:
  {
    $$ = create_node(e_object_body);
  }
  |
  obj_field_list 
  {
    $$ = create_node(e_object_body);
    $$->appendChild($1);
  }
  |
  method_list 
  {
    $$ = create_node(e_object_body);
    $$->appendChild($1);
  }
  |
  obj_field_list method_list 
  {
    $$ = create_node(e_object_body);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  ;
  
obj_field_list:
  identifier_list TOK_COLON type_spec TOK_SEMICOLON 
  {
    $$ = create_node(e_obj_field_list);
    $$->appendChild($1);
    $$->appendChild(create_node(TOK_COLON,$2));
    $$->appendChild($3);
  }
  |
  obj_field_list identifier_list TOK_COLON type_spec TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
    $$->appendChild(create_node(TOK_COLON,$3));
    $$->appendChild($4);
  }
  ;
  
method_list:
  method_heading TOK_SEMICOLON 
  {
    $$ = create_node(e_method_list);
    $$->appendChild($1);
  }
  |
  method_heading directive_list TOK_SEMICOLON 
  {
    $$ = create_node(e_method_list);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  method_list method_heading TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  |
  method_list method_heading directive_list TOK_SEMICOLON 
  {
    $$ = $1;
    $$->appendChild($2);
    $$->appendChild($3);
  }
  ;
  
directive_list:
  TOK_SEMICOLON directive_el 
  {
    $$ = create_node(e_directive_list);
    $$->appendChild($2);
  }
  |
  directive_list TOK_SEMICOLON directive_el 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  ;
  
method_heading:
  function_header 
  {
    $$ = create_node(e_method_heading);
    $$->appendChild($1);
  }
  |
  procedure_header 
  {
    $$ = create_node(e_method_heading);
    $$->appendChild($1);
  }
  |
  constructor_header 
  {
    $$ = create_node(e_method_heading);
    $$->appendChild($1);
  }
  |
  destructor_header 
  {
    $$ = create_node(e_method_heading);
    $$->appendChild($1);
  }
  ;
  
class_type:
  TOK_CLASS class_heritage class_body TOK_END 
  {
    $$ = create_node(e_class_type);
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_CLASS class_body TOK_END 
  {
    $$ = create_node(e_class_type);
    $$->appendChild($2);
  }
  ;
  
class_body:
  {
    $$ = create_node(e_class_body);
  }
  |
  class_items_list 
  {
    $$ = create_node(e_class_body);
    $$->appendChild($1);

    $$->copyNode($1);
  }
  ;
  
class_heritage:
  TOK_LPAREN identifier_list TOK_RPAREN 
  {
    $$ = create_node(e_class_heritage);
    $$->appendChild($2);
  }
  ;
  
class_items_list:
  class_items_section 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_class_items_list);
    ptmp = create_node(e_unqualified_section);
    ptmp->copyNode($1);
    $$->appendChild(ptmp);
  }
  |
  class_visibility 
  {
    $$ = create_node(e_class_items_list);
    $$->appendChild($1);
  }
  |
  TOK_PUBLIC class_items_section 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_class_items_list);
    ptmp=create_node(e_public_section);
    $$->appendChild(ptmp);
    ptmp->copyNode($2);
    
  }
  |
  TOK_PROTECTED class_items_section 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_class_items_list);
    ptmp = create_node(e_protected_section);
    $$->appendChild(ptmp);
    ptmp->copyNode($2);
  }
  |
  TOK_PUBLISHED class_items_section 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_class_items_list);
    ptmp = create_node(e_published_section);
    $$->appendChild(ptmp);
    ptmp->copyNode($2);
  }
  |
  TOK_PRIVATE class_items_section 
  {
    syntaxNode *ptmp;
    $$ = create_node(e_class_items_list);
    ptmp = create_node(e_private_section);
    $$->appendChild(ptmp);
    ptmp->copyNode($2);
  }
  |
  class_items_list class_visibility class_items_section 
  {
    $$ = $1;
    $$->appendChild($2);
    $2->copyNode($3);
  }
  |
  class_items_list class_visibility 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  ;
  
class_items_section:
  class_method_heading TOK_SEMICOLON 
  {
    //hello
    syntaxNode *pmethod;
    $$ = create_node(e_class_items_section);
    pmethod = create_node(e_method_member);
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
  }
  |
  class_method_heading TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_method_member);
    syntaxNode *pdirective=create_node(e_directive_list);

    $$ = create_node(e_class_items_section);

    ptmp = create_node(TOK_MESSAGE,$3);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$4));
  
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild(pdirective);
    pdirective->appendChild(ptmp);
  }
  |
  class_method_heading directive_list TOK_SEMICOLON 
  {
    //hello2
    syntaxNode *pmethod;
    $$ = create_node(e_class_items_section);
    pmethod = create_node(e_method_member);

    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($2);
  }
  |
  class_method_heading TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER directive_list TOK_SEMICOLON 
  {
    syntaxNode *ptmp,*pmethod;

    ptmp = create_node(TOK_MESSAGE,$3);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$4));

    pmethod = create_node(e_method_member);
      
    $$ = create_node(e_class_items_section); 
    
    $$->appendChild(pmethod);
    pmethod->appendChild($1);
    pmethod->appendChild($5);
    $5->appendChild(ptmp);
  }
  |
  property_definition 
  {
    $$ = create_node(e_class_items_section);
    $$->appendChild($1);
  }
  |
  identifier_list TOK_COLON type_spec TOK_SEMICOLON 
  {
    syntaxNode *pvar;
    $$ = create_node(e_class_items_section);
    pvar = create_node(e_variable_member);
    
    $$->appendChild(pvar);
    pvar->appendChild($1);
    pvar->appendChild($3);
  }
  |
  class_items_section class_method_heading TOK_SEMICOLON 
  {
    //hello3
    syntaxNode *pmethod;
    $$ = $1;
    pmethod = create_node(e_method_member);
    $$->appendChild(pmethod);
    pmethod->appendChild($2);
  }
  |
  class_items_section class_method_heading TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER TOK_SEMICOLON 
  {
    syntaxNode *ptmp,*pmethod;
    syntaxNode *pdirective = create_node(e_directive_list);

    $$ = $1;

    ptmp = create_node(TOK_MESSAGE,$4);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$5));
    pdirective->appendChild(ptmp);

    pmethod = create_node(e_method_member);

    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild(pdirective);
  }
  |
  class_items_section class_method_heading directive_list TOK_SEMICOLON 
  {
    // hello6
    syntaxNode *pmethod;
    $$ = $1;

    pmethod = create_node(e_method_member);
    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($3);
  }
  |
  class_items_section class_method_heading TOK_SEMICOLON TOK_MESSAGE TOK_IDENTIFIER directive_list TOK_SEMICOLON 
  {
    syntaxNode *ptmp;
    syntaxNode *pmethod = create_node(e_method_member);

    $$ = $1;
    
    ptmp = create_node(TOK_MESSAGE,$4);
    ptmp->appendChild(create_node(TOK_IDENTIFIER,$5));
    
    $$->appendChild(pmethod);
    pmethod->appendChild($2);
    pmethod->appendChild($6);
    $6->appendChild(ptmp);
  }
  |
  class_items_section property_definition 
  {
    $$ = $1;
    $$->appendChild($2);
  }
  |
  class_items_section identifier_list TOK_COLON type_spec TOK_SEMICOLON 
  {
    syntaxNode *pvar;

    $$ = $1;

    pvar = create_node(e_variable_member);
    $$->appendChild(pvar);

    pvar->appendChild($2);
    pvar->appendChild($4);
  }
  ;
  
class_method_heading:
  function_header 
  {
    $$ = $1;
  }
  |
  TOK_CLASS function_header 
  {
    $$ = $2;
    $$->static_ = 1;
  }
  |
  procedure_header 
  {
    $$ = $1;
  }
  |
  TOK_CLASS procedure_header 
  {
    $$ = $2;
    $$->static_ = 1;
  }
  |
  constructor_header 
  {
    $$ = $1;
  }
  |
  destructor_header 
  {
    $$ = $1;
  }
  ;
  
class_visibility:
  TOK_PUBLIC 
  {
    $$ = create_node(e_public_section)
  }
  |
  TOK_PROTECTED 
  {
    $$ = create_node(e_protected_section)
  }
  |
  TOK_PUBLISHED 
  {
    $$ = create_node(e_published_section)
  }
  |
  TOK_PRIVATE 
  {
    $$ = create_node(e_private_section)
  }
  ;
  
property_definition:
  TOK_PROPERTY TOK_IDENTIFIER opt_property_interface property_specifiers TOK_SEMICOLON 
  {
    $$ = create_node(e_property_definition);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
    $$->appendChild($4);
  }
  |
  TOK_PROPERTY TOK_IDENTIFIER opt_property_interface property_specifiers TOK_SEMICOLON TOK_DEFAULT TOK_SEMICOLON 
  {
    $$ = create_node(e_property_definition);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($3);
    $$->appendChild($4);
    $$->appendChild(create_node(TOK_DEFAULT,$6));
  }
  ;
  
opt_property_interface:
  {
    $$ = 0;
  }
  |
  TOK_COLON TOK_IDENTIFIER 
  {
    $$ = create_node(e_opt_property_interface);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_LBRACKET parameter_list TOK_RBRACKET TOK_COLON TOK_IDENTIFIER 
  {
    $$ = create_node(e_opt_property_interface);
    $$->appendChild(create_node(TOK_IDENTIFIER,$5));
    $$->appendChild($2);
  }
  ;
  
property_specifiers:
  opt_index opt_read opt_write opt_stored opt_readonly opt_writeonly opt_dispid opt_default opt_implements 
  {
    $$ = create_node(e_property_specifiers);
    $$->appendChild($1);
    $$->appendChild($2);
    $$->appendChild($3);
    $$->appendChild($4);
    $$->appendChild($5);
    $$->appendChild($6);
  }
  ;

identifier_rec_list:
  TOK_IDENTIFIER
  {
    $$ = create_node(e_identifier_list);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  identifier_rec_list TOK_DOT TOK_IDENTIFIER
  {
    $$=$1;
    $$->appendChild(create_node(TOK_IDENTIFIER,$3));
  }
  ;
  
  
opt_readonly:
  {
   $$ = 0;
  }
  |
  TOK_READONLY
  {
    $$ = create_node(TOK_READONLY,$1);
  }
  ;
  
opt_writeonly:
  {
   $$ = 0;
  }
  |
  TOK_WRITEONLY
  {
    $$ = create_node(TOK_READONLY,$1);
  }
  ;  
opt_dispid:
  {
    $$ = 0;
  }
  |
  TOK_DISPID TOK_INTEGER
  {
    $$ = create_node(TOK_DISPID,$1);
    $$->appendChild(create_node(TOK_INTEGER,$2));
    $$->children[0]->negate_ = 0;
  }
  |
  TOK_DISPID TOK_MINUS TOK_INTEGER
  {
    $$ = create_node(TOK_DISPID,$1);
    $$->appendChild(create_node(TOK_INTEGER,$2));
    $$->children[0]->negate_ = 1;
  }
  ;  
  
opt_read:
  {
      $$ = 0;
  }
  |
  TOK_READ identifier_rec_list
  {
    $$ = create_node(e_opt_read);
    $$->appendChild($2);
  }
  ;
  
opt_write:
  {
    $$ = 0;
  }
  |
  TOK_WRITE identifier_rec_list
  {
    $$ = create_node(e_opt_write);
    $$->appendChild($2);
  }
  ;
  
opt_stored:
  {
    $$ = 0;
  }
  |
  TOK_STORED TOK_IDENTIFIER 
  {
    $$ = create_node(e_opt_stored);
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_STORED simple_const 
  {
    $$ = create_node(e_opt_stored);
    $$->appendChild($2);
  }
  ;
  
opt_index:
  {
    $$ = 0;
  }
  |
  TOK_IDENTIFIER expression 
  {
    $$ = create_node(e_opt_index);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  ;
  
opt_default:
  {
    $$ = 0;
  }
  |
  TOK_DEFAULT expression 
  {
    $$ = create_node(e_opt_default);
    $$->appendChild(create_node(TOK_DEFAULT,$1));
    $$->appendChild($2);
  }
  |
  TOK_NODEFAULT 
  {
    $$ = create_node(e_opt_default);
    $$->appendChild(create_node(TOK_NODEFAULT,$1));
  }
  ;
  
opt_implements:
  {
    $$ = 0;
  }
  |
  TOK_IMPLEMENTS TOK_IDENTIFIER 
  {
    $$ = create_node(e_opt_implements);
    $$->appendChild(create_node(TOK_IMPLEMENTS,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  ;
  
interface_type:
  TOK_INTERFACE class_heritage interface_body 
  {
    $$ = create_node(e_interface_type);
    $$->appendChild($2);
    $$->appendChild($3);
  }
  |
  TOK_INTERFACE
  {
    $$ = create_node(e_interface_forward_decl);
  }
  |
  TOK_DISPINTERFACE
  {
    $$ = create_node(e_disp_forward_decl);
  }
  |
  TOK_INTERFACE interface_body 
  {
    $$ = create_node(e_interface_type);
    $$->appendChild($2);
  }
  |
  TOK_DISPINTERFACE interface_body 
  {
    $$ = create_node(e_dispinterface_type);
    $$->appendChild($2);
  }
  ;
  
opt_guid:
  TOK_LBRACKET TOK_STRING TOK_RBRACKET 
  {
    $$ = create_node(e_opt_guid);
    $$->appendChild(create_node(TOK_STRING,$2));
  }
  ;
  
interface_body:
  TOK_END 
  {
    $$ = create_node(e_interface_body);
  }
  |
  opt_guid class_items_list TOK_END 
  {
    $$ = create_node(e_interface_body);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  opt_guid TOK_END 
  {
    $$ = create_node(e_interface_body);
    $$->appendChild($1);
  }
  ;
  
as_expression:
  simple_expression 
  {
    $$ = $1;
  }
  |
  as_expression TOK_AS simple_expression 
  {
    if($1->node_type == e_as_expression)
    {
      $$ = $1;
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_as_expression);
      $$->appendChild($1);
      $$->appendChild($3); 
    }
  }
  ;
  
shift_expression:
  as_expression 
  {
    $$ = $1;
  }
  |
  shift_expression TOK_SHL as_expression 
  {
    if($1->node_type == e_shift_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_SHL,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_shift_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_SHL,$2));
      $$->appendChild($3); 
    }
  }
  |
  shift_expression TOK_SHR as_expression 
  {
    if($1->node_type == e_shift_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_SHR,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_shift_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_SHR,$2));
      $$->appendChild($3); 
    }
  }
  ;
  
multiplicative_expression:
  shift_expression 
  {
    $$ = $1;
  }
  |
  multiplicative_expression TOK_STAR shift_expression 
  {
    if($1->node_type == e_multiplicative_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_STAR,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_multiplicative_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_STAR,$2));
      $$->appendChild($3); 
    }
  }
  |
  multiplicative_expression TOK_SLASH shift_expression 
  {
    if($1->node_type == e_multiplicative_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_SLASH,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_multiplicative_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_SLASH,$2));
      $$->appendChild($3); 
    }
  }
  |
  multiplicative_expression TOK_MOD shift_expression 
  {
    if($1->node_type == e_multiplicative_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_MOD,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_multiplicative_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_MOD,$2));
      $$->appendChild($3); 
    }
  }
  |
  multiplicative_expression TOK_DIV shift_expression 
  {
    if($1->node_type == e_multiplicative_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_DIV,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_multiplicative_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_DIV,$2));
      $$->appendChild($3); 
    }
  }
  ;
additive_expression:
  multiplicative_expression 
  {
    $$=$1;
  }
  |
  additive_expression TOK_PLUS multiplicative_expression 
  {
    if($1->node_type == e_additive_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_PLUS,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_additive_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_PLUS,$2));
      $$->appendChild($3); 
    }
  }
  |
  additive_expression TOK_MINUS multiplicative_expression 
  {
    if($1->node_type == e_additive_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_MINUS,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_additive_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_MINUS,$2));
      $$->appendChild($3); 
    }
  }
  ;
  
relational_expression:
  additive_expression 
  {
    $$ = $1;
  }
  |
  relational_expression TOK_GT additive_expression 
  {
    if($1->node_type == e_relational_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_GT,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_relational_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_GT,$2));
      $$->appendChild($3); 
    }
  }
  |
  relational_expression TOK_LT additive_expression 
  {
    if($1->node_type == e_relational_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_LT,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_relational_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_LT,$2));
      $$->appendChild($3); 
    }
  }
  |
  relational_expression TOK_LE additive_expression 
  {
    if($1->node_type == e_relational_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_LE,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_relational_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_LE,$2));
      $$->appendChild($3); 
    }
  }
  |
  relational_expression TOK_GE additive_expression 
  {
    if($1->node_type == e_relational_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_GE,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_relational_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_GE,$2));
      $$->appendChild($3); 
    }
  }
  ;
  
equality_expression:
  relational_expression 
  {
    $$ = $1;
  }
  |
  equality_expression TOK_EQ relational_expression 
  {
    if($1->node_type == e_equality_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_EQ,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_equality_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_EQ,$2));
      $$->appendChild($3); 
    }
  }
  |
  equality_expression TOK_NE relational_expression 
  {
    if($1->node_type == e_equality_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_NE,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_equality_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_NE,$2));
      $$->appendChild($3); 
    }
  }
  |
  equality_expression TOK_IS relational_expression 
  {
    if($1->node_type == e_equality_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_IS,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_equality_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_IS,$2));
      $$->appendChild($3); 
    }
  }
  |
  equality_expression TOK_IN relational_expression 
  {
    if($1->node_type == e_equality_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_IN,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_equality_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_IN,$2));
      $$->appendChild($3); 
    }
  }
  ;
  
and_expression:
  equality_expression 
  {
    $$ = $1;
  }
  |
  and_expression TOK_LAND equality_expression 
  {
    if($1->node_type == e_and_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_LAND,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_and_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_LAND,$2));
      $$->appendChild($3); 
    }
  }
  ;
exclusive_or_expression:
  and_expression 
  {
    $$ = $1;
  }
  |
  exclusive_or_expression TOK_LOR and_expression 
  {
    if($1->node_type == e_exclusive_or_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_LOR,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_exclusive_or_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_LOR,$2));
      $$->appendChild($3); 
    }
  }
  ;
inclusive_or_expression:
  exclusive_or_expression 
  {
    $$ = $1;
  }
  |
  inclusive_or_expression TOK_XOR exclusive_or_expression 
  {
    if($1->node_type == e_inclusive_or_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_XOR,$2));
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_inclusive_or_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_XOR,$2));
      $$->appendChild($3); 
    }
  }
  ;
logical_and_expression:
  inclusive_or_expression 
  {
    $$ = $1;
  }
  |
  TOK_NOT inclusive_or_expression 
  {
    $$ = create_node(e_logical_and_expression);
    $$->appendChild(create_node(TOK_NOT,$1));
    $$->appendChild($2);
  }
  |
  logical_and_expression TOK_AND inclusive_or_expression 
  {
    if($1->node_type == e_logical_and_expression)
    {
      $$ = $1;
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_logical_and_expression);
      $$->appendChild($1);
      $$->appendChild($3); 
    }
  }
  |
  logical_and_expression TOK_AND TOK_NOT inclusive_or_expression 
  {
    if($1->node_type == e_logical_and_expression)
    {
      $$ = $1;
      $$->appendChild(create_node(TOK_NOT,$3));
      $$->appendChild($4);
    }
    else
    {
      $$ = create_node(e_logical_and_expression);
      $$->appendChild($1);
      $$->appendChild(create_node(TOK_NOT,$3));
      $$->appendChild($4); 
    }
  }
  ;
  
logical_or_expression:
  logical_and_expression 
  {
    $$ = $1;
  }
  |
  logical_or_expression TOK_OR logical_and_expression 
  {
    if($1->node_type == e_logical_or_expression)
    {
      $$ = $1;
      $$->appendChild($3);
    }
    else
    {
      $$ = create_node(e_logical_or_expression);
      $$->appendChild($1);
      $$->appendChild($3);
    }
  }
  ;

range_list:
    expression TOK_DOTDOT expression  
    {
    }
    |
    expression 
    {
    }
    |
    range_list TOK_COMMA expression TOK_DOTDOT expression
    {
    }
    |
    range_list TOK_COMMA expression
    {
    }
    ;
  
simple_expression:
  TOK_IDENTIFIER 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_IDENTIFIER defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  |
  TOK_MESSAGE 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_DEFAULT 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_WRITE 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_MESSAGE defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  |
  TOK_DEFAULT defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  |
  TOK_WRITE defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  |
  TOK_READ defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($2);
  }
  |
  TOK_READ 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_MINUS TOK_IDENTIFIER %prec PREC_UNARYOP 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_MINUS,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_PLUS TOK_IDENTIFIER %prec PREC_UNARYOP 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_PLUS,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_ADDR simple_expression %prec PREC_UNARYOP 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_ADDR,$1));
    $$->appendChild($2);
  }
  |
  TOK_MINUS simple_const 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_MINUS,$1));
    $$->appendChild($2);
  }
  |
  TOK_PLUS simple_const 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild(create_node(TOK_PLUS,$1));
    $$->appendChild($2);
  }
  |
  simple_const 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
  }
  |
  TOK_LPAREN expression TOK_RPAREN 
  {
    if($2->node_type == e_sub_expression)
      $$ = $2;
    else
    {
      $$ = create_node(e_sub_expression);
      $$->appendChild($2);
    }
  }
  |
  TOK_LPAREN expression TOK_RPAREN defptr_list 
  {
    if($2->node_type == e_sub_expression)
      $$ = $2;
    else
    {
      $$ = create_node(e_sub_expression);
      $$->appendChild($2);
    }
  }
  |
  proc_func_expression 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
  }
  |
  proc_func_expression defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  array_expression 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
  }
  |
  array_elt_expression 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
  }
  |
  array_elt_expression defptr_list 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
    $$->appendChild($2);
  }
  |
  record_expression 
  {
    $$ = create_node(e_simple_expression);
    $$->appendChild($1);
  }
  ;
  
expression:
  logical_or_expression 
  {
    $$ = $1;
  }
  ;
  
proc_func_expression:
  TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_WRITE TOK_LPAREN TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  TOK_READ TOK_LPAREN TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
  }
  |
  array_elt_expression TOK_LPAREN TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild($1);
  }
  |
  TOK_IDENTIFIER TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  TOK_WRITE TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  TOK_READ TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_IDENTIFIER,$1));
    $$->appendChild($3);
  }
  |
  array_elt_expression TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  |
  TOK_INHERITED TOK_IDENTIFIER 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_INHERITED,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_INHERITED TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_INHERITED,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
  }
  |
  TOK_INHERITED TOK_IDENTIFIER TOK_LPAREN expression_list TOK_RPAREN 
  {
    $$ = create_node(e_proc_func_expression);
    $$->appendChild(create_node(TOK_INHERITED,$1));
    $$->appendChild(create_node(TOK_IDENTIFIER,$2));
    $$->appendChild($4);
  }
  ;
  
expression_list:
  expression_list TOK_COMMA expression 
  {
    $$ = $1;
    $$->appendChild($3);
  }
  |
  expression 
  {
    $$ = create_node(e_expression_list);
    $$->appendChild($1);
  }
  ;
  
  
record_expression:
  simple_expression TOK_DOT simple_expression 
  {
    syntaxNode *pnode = $3->children[0];

    if(pnode->node_type==e_record_expression)
    {
      pnode->prependChild($1);
      $$=pnode;
      //$$=$3;
    }
    else
    {
      $$ = create_node(e_record_expression);
      $$->appendChild($1);
      $$->appendChild($3);
    }
    //$$ = create_node(e_record_expression);
    //$$->appendChild($1);
    //$$->appendChild($3);
  }
  ;
  
  
array_elt_expression:
  simple_expression TOK_LBRACKET expression_list TOK_RBRACKET 
  {
    $$ = create_node(e_array_elt_expression);
    $$->appendChild($1);
    $$->appendChild($3);
  }
  ;
  
array_expression:
  TOK_LBRACKET range_list TOK_RBRACKET 
  {
    $$ = create_node(e_array_expression);
    $$->appendChild($2);
  }
  |
  TOK_LBRACKET TOK_RBRACKET 
  {
    $$ = create_node(e_array_expression);
  }
  ;
  
simple_const:
  TOK_HEX
  {
    $$ = create_node(TOK_HEX,$1);
  }
  |
  TOK_INTEGER
  {
    $$ = create_node(TOK_INTEGER,$1);
  }
  |
  TOK_REAL
  {
    $$ = create_node(TOK_REAL,$1);
  }
  |
  TOK_STRING 
  {
    $$ = create_node(TOK_STRING,$1);
  }
  |
  TOK_TRUE 
  {
    $$ = create_node(TOK_TRUE,$1);
  }
  |
  TOK_FALSE 
  {
    $$ = create_node(TOK_FALSE,$1);
  }
  |
  TOK_NIL 
  {
    $$ = create_node(TOK_NIL,$1);
  }
  ;


%%

