#ifndef _AST_H_
#define _AST_H_
    typedef struct registrador Registrador;
    typedef struct No Tree;
    typedef void * expression;
    typedef struct parametros Parametros;
    typedef struct variaveis Variaveis;
    typedef struct expressao Expressao;
    typedef struct cmd_expressao Cmd_expressao;
    typedef struct comandos Comandos;
    typedef struct no No;
    typedef struct function_struct Function_struct;
    typedef struct regAlloc RegAlloc;
    
    
    Cmd_expressao *novoCmdExpressao(Expressao *exp, Cmd_expressao *prox);
    
    Expressao *novaExpressao(int tipo, int value, char *str, int dimensaoArray, Expressao *filho_esquerdo, Expressao *filho_direito, float valueFloat);
    
    Function_struct *novaFunction(Expressao *nome, int tipo_retorno, Expressao *exp_retorno, Parametros *parametros, Variaveis *variaveis, Comandos *function_comandos, Function_struct *proximo);
    
    Parametros *novoParametro(Expressao *id, int tipo, Parametros *prox);
    
    Variaveis *novaVariavel(Expressao *id, int tipo, Variaveis *prox);
    
    Comandos *cmd_generico(int tipo, Expressao *exp, Comandos *cmd1, Comandos *cmd2, Expressao *exp2);
    
    Comandos* setProxGenerico(Comandos *atual, Comandos *prox) ;

    void imprimiAST(Function_struct *f);
#endif