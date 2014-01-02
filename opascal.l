%x ASMBLOCK

%{
#include "stdafx.h"
#include "opascalParser.h"
#include "utils.h"

#define YY_SKIP_YYWRAP
#undef  YY_DECL
#define YY_DECL int CopascalLexxer::yylex(YYSTYPE *pascalval)
#define SETTOK(x) do { pascalval->name.node_type=x;pascalval->name.line=yylineno; pascalval->name.name =str_to_lower(_m_syntaxTree->duplicateString(yytext)); return x;} while(0)

#pragma warning(disable : 4244)

/* **** PASCAL KEYWORDS ****
threadvar 
assembler 
abstract  
finalization 
initialization 
as 
in 
with
on 
try 
except 
finally
while 
is
implementation 
unit 
object    
interface 
read 
write 
default 
stored 
nodefault 
implements
property  
protected 
public    
private   
published 
class     
goto      
inherited 
set         
packed      
file        
out         
forward     
const       
type        
var         
array       
of          
record      
end         
repeat      
until       
begin       
if          
then        
else        
for         
case        
do          
to          
downto      
shl 
shr 
at  
div 
raise
mod  
and         
or          
xor         
program     
function    
procedure   
uses        
cdecl       
register    
dynamic     
virtual     
export      
external    
far         
override    
overload    
pascal      
message     
reintroduce 
safecall    
stdcall     
true        
false       
nil         
not         
constructor 
destructor  
readonly
writeonly
*/
%}

integer    ([0-9]+)
real       ([0-9]+\.[0-9]+([eE][-+]?[0-9]+)?)
identifier    ([a-zA-Z_][a-zA-Z0-9_]*)
hex_number  ([$][a-fA-F0-9][a-fA-F0-9]*)
l_number  ([#][0-9][0-9]*)

%option yyclass = "CopascalLexxer"

%%

[{] {
      int c;
      
      do
      {
        c = yyinput();
        if(c == '\n')
          yylineno++;         
      }
      while(c!='}');      
    }

"(*"  {
        int  c;
        bool cont=true;

        c = yyinput();
        
        do
        {
          switch(c)
          {
            case '\n':
              yylineno++;
              c=yyinput();
            break;
            case '*':
              c=yyinput();
              if(c==')')
                cont = false;
            break;
            default:
              c=yyinput();
          }
        }
        while(cont);
      }

[/][/](.)*[\n] {
                 yylineno++;
               }


['] {
       std::string s;
     bool cont = true;
     int c;

     c = yyinput();

     do
     {
       switch(c)
     {
       case '\n':
         YY_FATAL_ERROR("invalid string");
       break;
       case '\'':
         c = yyinput();

       if(c!='\'')
         cont=false;
       else
       {
         s += c;
         c = yyinput();
       }
       break;
       default:
         s += c;
       c = yyinput();
     }
     }
     while(cont);

     unput(c);

       pascalval->name.node_type=TOK_STRING;
     pascalval->name.line=yylineno; 
     pascalval->name.name =_m_syntaxTree->duplicateString(s.c_str());
     return TOK_STRING;
    }

"asm"            BEGIN ASMBLOCK;
<ASMBLOCK>[eE][Nn][Dd][;] BEGIN INITIAL;
<ASMBLOCK>[\n]   { yylineno++;}
<ASMBLOCK>.      {}

[dD][iI][sS][pP][iI][nN][tT][eE][rR][fF][aA][cC][eE]     { SETTOK( TOK_DISPINTERFACE);}
[lL][aA][bB][eE][lL]                                      { SETTOK( TOK_LABEL);}
[tT][hH][rR][eE][aA][dD][vV][aA][rR]                      { SETTOK( TOK_THREADVAR);}
[aA][sS][sS][eE][mM][bB][lL][eE][rR]                      { SETTOK( TOK_ASSEMBLER);}
[aA][bB][sS][tT][rR][aA][cC][tT]                          { SETTOK( TOK_ABSTRACT);}
[fF][iI][nN][aA][lL][iI][zZ][aA][tT][iI][oO][nN]          { SETTOK( TOK_FINALIZATION);}
[iI][nN][iI][tT][iI][aA][lL][iI][zZ][aA][tT][iI][oO][nN]  { SETTOK( TOK_INITIALIZATION);}
[aA][sS]                                                  { SETTOK( TOK_AS);}
[iI][nN]                                                  { SETTOK( TOK_IN);}
[wW][iI][tT][hH]                                          { SETTOK( TOK_WITH);}
[oO][nN]                                                  { SETTOK( TOK_ON);}
[tT][rR][yY]                                              { SETTOK( TOK_TRY);}
[eE][xX][cC][eE][pP][tT]                                  { SETTOK( TOK_EXCEPT);}
[fF][iI][nN][aA][lL][lL][yY]                              { SETTOK( TOK_FINALLY);}
[wW][hH][iI][lL][eE]                                      { SETTOK( TOK_WHILE);}
[iI][sS]                                                  { SETTOK( TOK_IS);}
[iI][mM][pP][lL][eE][mM][eE][nN][tT][aA][tT][iI][oO][nN]  { SETTOK( TOK_IMPLEMENTATION);}
[uU][nN][iI][tT]                                          { SETTOK( TOK_UNIT);}
[oO][bB][jJ][eE][cC][tT]                                  { SETTOK( TOK_OBJECT);}
[iI][nN][tT][eE][rR][fF][aA][cC][eE]                      { SETTOK( TOK_INTERFACE);}
[rR][eE][aA][dD]                                          { SETTOK( TOK_READ );}
[wW][rR][iI][tT][eE]                                      { SETTOK( TOK_WRITE );}
[dD][eE][fF][aA][uU][lL][tT]                              { SETTOK( TOK_DEFAULT );}
[sS][tT][oO][rR][eE][dD]                                  { SETTOK( TOK_STORED );}
[nN][oO][dD][eE][fF][aA][uU][lL][tT]                      { SETTOK( TOK_NODEFAULT );}
[iI][mM][pP][lL][eE][mM][eE][nN][tT][sS]                  { SETTOK( TOK_IMPLEMENTS);}
[pP][rR][oO][pP][eE][rR][tT][yY]                          { SETTOK( TOK_PROPERTY);}
[pP][rR][oO][tT][eE][cC][tT][eE][dD]                      { SETTOK( TOK_PROTECTED);}
[pP][uU][bB][lL][iI][cC]                                  { SETTOK( TOK_PUBLIC);}
[pP][rR][iI][vV][aA][tT][eE]                              { SETTOK( TOK_PRIVATE);}
[pP][uU][bB][lL][iI][sS][hH][eE][dD]                      { SETTOK( TOK_PUBLISHED);}
[cC][lL][aA][sS][sS]                                      { SETTOK( TOK_CLASS);}
[gG][oO][tT][oO]                                          { SETTOK( TOK_GOTO);}
[iI][nN][hH][eE][rR][iI][tT][eE][dD]                      { SETTOK( TOK_INHERITED);}
[sS][eE][tT]                                              { SETTOK( TOK_SET);}
[dD][iI][sS][pP][iI][dD]                                  { SETTOK( TOK_DISPID);}
[pP][aA][cC][kK][eE][dD]                                  { SETTOK( TOK_PACKED);}
[fF][iI][lL][eE]                                          { SETTOK( TOK_FILE);}
[oO][uU][tT]                                              { SETTOK( TOK_OUT);}
[fF][oO][rR][wW][aA][rR][dD]                              { SETTOK( TOK_FORWARD );}
[cC][oO][nN][sS][tT]                                      { SETTOK( TOK_CONST);}
[tT][yY][pP][eE]                                          { SETTOK( TOK_TYPE);}
[vV][aA][rR]                                              { SETTOK( TOK_VAR);}
[aA][rR][rR][aA][yY]                                      { SETTOK( TOK_ARRAY);}
[oO][fF]                                                  { SETTOK( TOK_OF);}
[rR][eE][cC][oO][rR][dD]                                  { SETTOK( TOK_RECORD);}
[eE][nN][dD]                                              { SETTOK( TOK_END);}
[rR][eE][pP][eE][aA][tT]                                  { SETTOK( TOK_REPEAT);}
[uU][nN][tT][iI][lL]                                      { SETTOK( TOK_UNTIL);}
[bB][eE][gG][iI][nN]                                      { SETTOK( TOK_BEGIN);}
[iI][fF]                                                  { SETTOK( TOK_IF);}
[tT][hH][eE][nN]                                          { SETTOK( TOK_THEN);}
[eE][lL][sS][eE]                                          { SETTOK( TOK_ELSE);}
[fF][oO][rR]                                              { SETTOK( TOK_FOR);}
[cC][aA][sS][eE]                                          { SETTOK( TOK_CASE);}
[dD][oO]                                                  { SETTOK( TOK_DO);}
[tT][oO]                                                  { SETTOK( TOK_TO);}
[dD][oO][wW][nN][tT][oO]                                  { SETTOK( TOK_DOWNTO);}
[sS][hH][lL]                                              { SETTOK( TOK_SHL);}
[sS][hH][rR]                                              { SETTOK( TOK_SHR);}
[aA][tT]                                                  { SETTOK( TOK_AT);}
[dD][iI][vV]                                              { SETTOK( TOK_DIV);}
[rR][aA][iI][sS][eE]                                      { SETTOK( TOK_RAISE);}
[mM][oO][dD]                                              { SETTOK( TOK_MOD);}
[aA][nN][dD]                                              { SETTOK( TOK_AND);}
[oO][rR]                                                  { SETTOK( TOK_OR);}
[xX][oO][rR]                                              { SETTOK( TOK_XOR);}
[pP][rR][oO][gG][rR][aA][mM]                              { SETTOK( TOK_PROGRAM);}
[fF][uU][nN][cC][tT][iI][oO][nN]                          { SETTOK( TOK_FUNCTION);}
[pP][rR][oO][cC][eE][dD][uU][rR][eE]                      { SETTOK( TOK_PROCEDURE);}
[uU][sS][eE][sS]                                          { SETTOK( TOK_USES);}
[cC][dD][eE][cC][lL]                                      { SETTOK( TOK_CDECL );}
[dD][yY][nN][aA][mM][iI][cC]                              { SETTOK( TOK_DYNAMIC );}
[vV][iI][rR][tT][uU][aA][lL]                              { SETTOK( TOK_VIRTUAL);}
[eE][xX][pP][oO][rR][tT]                                  { SETTOK( TOK_EXPORT );}
[eE][xX][tT][eE][rR][nN][aA][lL]                          { SETTOK( TOK_EXTERNAL );}
[fF][aA][rR]                                              { SETTOK( TOK_FAR );}
[oO][vV][eE][rR][rR][iI][dD][eE]                          { SETTOK( TOK_OVERRIDE);}
[oO][vV][eE][rR][lL][oO][aA][dD]                          { SETTOK( TOK_OVERLOAD );}
[pP][aA][sS][cC][aA][lL]                                  { SETTOK( TOK_PASCAL );}
[mM][eE][sS][sS][aA][gG][eE]                              { SETTOK( TOK_MESSAGE );}
[rR][eE][iI][nN][tT][rR][oO][dD][uU][cC][eE]              { SETTOK( TOK_REINTRODUCE );}
[sS][aA][fF][eE][cC][aA][lL][lL]                          { SETTOK( TOK_SAFECALL );}
[sS][tT][dD][cC][aA][lL][lL]                              { SETTOK( TOK_STDCALL);}
[tT][rR][uU][eE]                                          { SETTOK( TOK_TRUE);}
[fF][aA][lL][sS][eE]                                      { SETTOK( TOK_FALSE);}
[nN][iI][lL]                                              { SETTOK( TOK_NIL);}
[nN][oO][tT]                                              { SETTOK( TOK_NOT);}
[cC][oO][nN][sS][tT][rR][uU][cC][tT][oO][rR]              { SETTOK( TOK_CONSTRUCTOR);}
[dD][eE][sS][tT][rR][uU][cC][tT][oO][rR]                  { SETTOK( TOK_DESTRUCTOR);}
[rR][eE][aA][dD][o0][nN][lL][yY]                          { SETTOK(TOK_READONLY);}
[wW][rR][iI][tT][eE][oO][nN][lL][yY]                      { SETTOK(TOK_WRITEONLY);}

";"   { SETTOK( TOK_SEMICOLON);}
":"   { SETTOK( TOK_COLON);}
","   { SETTOK( TOK_COMMA);}
"."   { SETTOK( TOK_DOT);}

".."  { SETTOK( TOK_DOTDOT);}
":="  { SETTOK( TOK_ASSIGN);}

"["   { SETTOK( TOK_LBRACKET);}
"]"   { SETTOK( TOK_RBRACKET);}
"("   { SETTOK( TOK_LPAREN);}
")"   { SETTOK( TOK_RPAREN);}

"="   { SETTOK( TOK_EQ);}
"<>"  { SETTOK( TOK_NE);}
"<"   { SETTOK( TOK_LT);}
"<="  { SETTOK( TOK_LE);}
">"   { SETTOK( TOK_GT);}
">="  { SETTOK( TOK_GE);}

"+"   { SETTOK( TOK_PLUS);}
"-"   { SETTOK( TOK_MINUS);}
"*"   { SETTOK( TOK_STAR);}
"/"   { SETTOK( TOK_SLASH);}
"&"     { SETTOK( TOK_LAND);}
"|"     { SETTOK( TOK_LOR);}
"@"     { SETTOK( TOK_ADDR);}
"^"     { SETTOK( TOK_DEREF);}
   

{l_number}+     { seq_to_str(yytext,pascalval);return TOK_STRING;}
{identifier}  { SETTOK(TOK_IDENTIFIER);}
{integer}     { SETTOK(TOK_INTEGER);}
{real}          { SETTOK(TOK_REAL);}
{hex_number}    { SETTOK( TOK_HEX);}

[\t ]+  { ; }
[\n] { yylineno++; }

.   { }
%%

/*

'[^'\n]*'' {
               yyless(yyleng-1);
               yymore();
           }


'[^'\n]*' {
        yytext[yyleng-1]=0;
              SETTOK2(TOK_STRING);
              yytext[yyleng-1]=0x27;
              return TOK_STRING;
          }
*/