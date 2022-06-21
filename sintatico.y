%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    extern int yylex();
    extern int yyparse();
    extern char* yytext;
    void yyerror(char *s);
    Function_struct *AST = NULL;
%}

%union {
    char *str;
    int num;
    float real;
    Parametros *parametros;
    Variaveis *variaveis;
    Cmd_expressao *expressao;
    Comandos *comandos;
    Expressao *expr;
    Function_struct *lista_funcoes;
}

%token FUNCTION
%token CHARACTER
%token VOID
%token INT
%token NUM_INTEGER
%token CHAR
%token RETURN
%token BREAK
%token SWITCH
%token CASE
%token DEFAULT
%token DO_WHILE
%token WHILE
%token FOR
%token IF
%token ELSE
%token TYPEDEF
%token STRUCT
%token PLUS
%token MINUS
%token MULTIPLY
%token DIV
%token REMAINDER
%token INC
%token DEC
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_NOT
%token BITWISE_XOR
%token NOT
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUAL
%token NOT_EQUAL
%token LESS_THAN
%token GREATER_THAN
%token GREATER_EQUAL
%token LESS_EQUAL
%token R_SHIFT
%token L_SHIFT
%token ASSIGN
%token ADD_ASSIGN
%token MINUS_ASSIGN
%token SEMICOLON
%token COMMA
%token COLON
%token L_PARENTESE
%token R_PARENTESE
%token L_CURLY_BRACKET
%token R_CURLY_BRACKET
%token L_SQUARE_BRACKET
%token R_SQUARE_BRACKET
%token TERNARY_CONDITIONAL
%token NUMBER_SIGN
%token POINTER
%token PRINTF
%token SCANF
%token DEFINE
%token EXIT
%token IDENTIFIER
%token RETURN_TYPE
%token VARIABLE
%token TYPE
%token PARAMETER
%token END_FUNCTION
%token GLOBAL
%token STRING
%token VALUE
%token CONSTANT

%type <lista_funcoes> inicio
%type <lista_funcoes> funcao
%type <num> tipos funcao_tipo_retorno NUM_INTEGER
%type <parametros> funcao_parametros
%type <variaveis> funcao_variaveis
%type <expr> expressao return_fun
%type <expr> expressao_primaria
%type <comandos> comandos
%type <comandos> comandos_if

%type <comandos> if
%type <comandos> printf
%type <comandos> scanf

%start inicio

%%

inicio: funcao {AST = $1; imprimiAST(AST);};

funcao: {$$ = NULL;}
    | funcao FUNCTION COLON expressao_primaria funcao_tipo_retorno funcao_parametros funcao_variaveis comandos return_fun END_FUNCTION {$$ = novaFunction($4, $5, $9, $6, $7, $8, $1);}
;

return_fun: RETURN L_PARENTESE expressao R_PARENTESE {$$ = novaExpressao(RETURN,0,NULL,0,$3,NULL,0);};

funcao_tipo_retorno: RETURN_TYPE COLON tipos{$$ = $3;};

funcao_variaveis: {$$ = NULL;}
    | funcao_variaveis VARIABLE COLON expressao_primaria TYPE COLON tipos {$$ = novaVariavel($4, $7, $1);}
;

funcao_parametros: { $$ = NULL;}
    | funcao_parametros PARAMETER COLON expressao_primaria TYPE COLON tipos {$$ = novoParametro($4, $7, $1);}
;


comandos: {$$ = NULL;}
    | if ponto_virgula comandos {$$ = setProxGenerico($1,$3 );}
    | printf ponto_virgula comandos {$$ = setProxGenerico($1,$3 );}
    | scanf ponto_virgula comandos {$$ = setProxGenerico($1,$3 );}
;

comandos_if: {$$ = NULL;}
    | if {$$ = $1;}
    | printf {$$ = $1;}
    | scanf {$$ = $1;}
;

scanf: SCANF L_PARENTESE expressao_primaria COMMA BITWISE_AND L_PARENTESE expressao_primaria R_PARENTESE R_PARENTESE ponto_virgula {$$ = cmd_generico(SCANF, $3,NULL, NULL,$7);}
;

printf: PRINTF L_PARENTESE expressao_primaria COMMA expressao R_PARENTESE {$$ = cmd_generico(PRINTF, $3,NULL, NULL,$5);}
    | PRINTF L_PARENTESE expressao_primaria R_PARENTESE {$$ = cmd_generico(PRINTF, $3, NULL, NULL,NULL);}
;

if: IF L_PARENTESE expressao COMMA comandos_if COMMA comandos_if R_PARENTESE {$$ = cmd_generico(IF, $3, $5, $7,NULL);}
    | IF L_PARENTESE expressao COMMA comandos_if R_PARENTESE {$$ = cmd_generico(IF, $3, $5, NULL, NULL);}
;

ponto_virgula: { }
    | SEMICOLON { }
    | COMMA {}
;

expressao: {$$ = NULL; }
    | expressao_primaria {$$ = $1;}
    | PLUS L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(PLUS,0,NULL,0,$3,$5,0);}
    | MINUS L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(MINUS,0,NULL,0,$3,$5,0);}
    | GREATER_THAN L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(GREATER_THAN,0,NULL,0,$3,$5,0);}
    | GREATER_EQUAL L_PARENTESE expressao COMMA  expressao R_PARENTESE {$$ = novaExpressao(GREATER_EQUAL,0,NULL,0,$3,$5,0);}
    | LESS_THAN L_PARENTESE expressao COMMA  expressao R_PARENTESE {$$ = novaExpressao(LESS_THAN,0,NULL,0,$3,$5,0);}
    | LESS_EQUAL L_PARENTESE expressao COMMA  expressao R_PARENTESE {$$ = novaExpressao(LESS_EQUAL,0,NULL,0,$3,$5,0);}
    | EQUAL L_PARENTESE expressao COMMA  expressao R_PARENTESE {$$ = novaExpressao(EQUAL,0,NULL,0,$3,$5,0);}
    | NOT_EQUAL L_PARENTESE expressao COMMA  expressao R_PARENTESE {$$ = novaExpressao(NOT_EQUAL,0,NULL,0,$3,$5,0);}
    | LOGICAL_AND L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(LOGICAL_AND,0,NULL,0,$3,$5,0);}
    | LOGICAL_OR L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(LOGICAL_OR,0,NULL,0,$3,$5,0);}
    | MULTIPLY L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(MULTIPLY,0,NULL,0,$3,$5,0);}
    | DIV L_PARENTESE expressao COMMA expressao R_PARENTESE {$$ = novaExpressao(DIV,0,NULL,0,$3,$5,0);}
;

expressao_primaria: IDENTIFIER { $$ = novaExpressao(IDENTIFIER,0,yytext,0,NULL,NULL,0);}
    | NUM_INTEGER {$$ = novaExpressao(NUM_INTEGER,0,NULL,0,NULL,NULL,$1);}
    | CHARACTER { printf("entrou aqui\n");}
    | L_PARENTESE expressao R_PARENTESE {}
    | STRING {$$ = novaExpressao(STRING,0,yytext,0,NULL,NULL,0);}
;

tipos: INT {$$ = INT; }
    | CHAR {$$ = CHAR; }
    | VOID {$$ = VOID; }
;
%%

void yyerror(char *s) {
    printf("Erro %s",yytext);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}