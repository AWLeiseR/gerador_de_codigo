#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sintatico.tab.h"
#include <string.h>
#include "funcoes.h"

int labelComandos = 0;
int countVar = 0;
int countReg = 0;

struct parametros {
    int tipo;
    Expressao* id;
    Parametros *prox;
};

struct variaveis{
    int tipo;
    Expressao *id;   
    Variaveis *prox;
};

struct expressao{
    int tipo; //+ - * / % ++ += -- -= ...
    int value;
    float valueFloat;
    char * str;
    int dimensaoArray;
    Expressao *filho_esquerdo;
    Expressao *filho_direito;
};

struct registrador{
    int tipo;
    char *id;
};
struct cmd_expressao{
    Expressao *exp;
    Cmd_expressao *prox;
};

struct comandos{
    int tipo; // expr ou tipo comando (if, while, for...)
    //if, while,
    Expressao *expr_comandos;
    Comandos *cmd_if;
    Comandos *cmd_else;
    Expressao *cmd_printf;
    Expressao *cmd_scanf;
    Comandos *cmd_return;
    Comandos *prox;
};

struct function_struct{
    Expressao *nome;
    int tipo_retorno;
    Expressao *exp_retorno;
    Parametros *parametros;
    Variaveis *variaveis;
    Comandos *function_comandos;
    Function_struct * prox;
};

struct regAlloc{
    char tipo;
    int num;
};

Registrador regs[8];
int regT[10] = {0,0,0,0,0,0,0,0,0,0};

int acharVar(char *id){
    int i;
    for(i=0;i<8;i++){
        if(strcmp(id,regs[i].id) == 0){
            return i;
        }
    }
    return -1;
}

int regTdisponivel(){
    int i;
    for(i=0;i<10;i++){
        if(regT[i] == 0){
            regT[i] = 1;
            return i;
        }
    }
    return -1;
}

void ocuparRegT(int i){
    regT[i] = 1;
}

void liberaRegT(int i){
    regT[i] = 0;
}

 RegAlloc imprimeExpressao(ProgramaMips *p,Expressao *exp){
    Expressao *aux = exp;
    RegAlloc auxReg, auxReg2, c, regString;
    int tempReg;
    int freee = 0;
    if(aux){
        //ver o q fazer com as strings e os interios
        switch (aux->tipo){
            // case NUM_INTEGER:
            //     //printf("--aux %f\n",aux->valueFloat);
            //     break;
            // case STRING:
            //     // printf("%s\n",aux->str);
            //     break;
            case IDENTIFIER:
                tempReg = acharVar(aux->str);
                if(tempReg != -1){
                    c.num = tempReg;
                    c.tipo = 's';
                }else{
                    freee = regTdisponivel();
                    if(freee){
                        ocuparRegT(freee);
                    }
                    c.num = freee;
                    c.tipo = 't';
                }
                break;
            case EQUAL:
                auxReg = imprimeExpressao(p,aux->filho_direito);
                auxReg2 = imprimeExpressao(p,aux->filho_esquerdo);
                // auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
                // auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
                if(auxReg2.tipo =='s' && auxReg.tipo == 's') {
                    freee = regTdisponivel();
                    if(freee != -1){
                        c.num = freee;                    
                        c.tipo = 't';
                    }else{
                        c.tipo = 's';
                        c.num = countVar+1;
                    }
                }else if(auxReg2.tipo =='t' && auxReg.tipo == 't') { 
                    c.num = auxReg2.num;                    
                    c.tipo = auxReg2.tipo;
                    liberaRegT(auxReg.num);
                }else if(auxReg.tipo == 't'){
                    c.num = auxReg.num;                    
                    c.tipo = auxReg.tipo;
                }
                imprimirEqual(p->text,auxReg.tipo,auxReg.num,auxReg2.tipo,auxReg2.num,c.tipo,c.num);
                break;
            // case NOT_EQUAL:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     imprimirNotEqual(p->text,auxReg, auxReg2, reg);
            //     break;
            // case GREATER_EQUAL:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     imprimirGreater(p->text,auxReg,auxReg2,reg);
            //     break;
            // case GREATER_THAN:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     imprimirGreater(p->text,auxReg,auxReg2,reg);
            //     break;
            // case LESS_EQUAL:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     imprimirLessEqual(p->text,auxReg,auxReg2,reg);
            //     break;
            case LESS_THAN:
                auxReg = imprimeExpressao(p,aux->filho_direito);
                auxReg2 = imprimeExpressao(p,aux->filho_esquerdo);
                // auxReg = imprimeExpressao(p,aux->filho_direito);
                // auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
                if(auxReg2.tipo =='s' && auxReg.tipo == 's') {
                    freee = regTdisponivel();
                    if(freee != -1){
                        c.num = freee;                    
                        c.tipo = 't';
                    }else{
                        c.tipo = 's';
                        c.num = countVar+1;
                    }
                }else if(auxReg2.tipo =='t' && auxReg.tipo == 't') { 
                    c.num = auxReg2.num;                    
                    c.tipo = auxReg2.tipo;
                    liberaRegT(auxReg.num);
                }else if(auxReg.tipo == 't'){
                    c.num = auxReg.num;                    
                    c.tipo = auxReg.tipo;
                }
                imprimirLess(p->text,auxReg.tipo,auxReg.num,auxReg2.tipo,auxReg2.num,c.tipo,c.num);
                break;
            // case MULTIPLY:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     inseriMul(reg,auxReg,auxReg2);
            //     break;
            // case DIV:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     inseriDiv(reg,auxReg,auxReg2);
            //     break;
            case PLUS:
                auxReg = imprimeExpressao(p,aux->filho_direito);
                auxReg2 = imprimeExpressao(p,aux->filho_esquerdo);
                // auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
                // auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
                  if(auxReg2.tipo =='s' && auxReg.tipo == 's') {
                    freee = regTdisponivel();
                    if(freee != -1){
                        c.num = freee;                    
                        c.tipo = 't';
                    }else{
                        c.tipo = 's';
                        c.num = countVar+1;
                    }
                }else if(auxReg2.tipo =='t' && auxReg.tipo == 't') { 
                    c.num = auxReg2.num;                    
                    c.tipo = auxReg2.tipo;
                    liberaRegT(auxReg.num);
                }else if(auxReg.tipo == 't'){
                    c.num = auxReg.num;                    
                    c.tipo = auxReg.tipo;
                }
                inseriAd(p->text,auxReg.tipo,auxReg.num,auxReg2.tipo,auxReg2.num,c.tipo,c.num);
                break;
            // case MINUS:
            //     auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
            //     auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
            //     inseriSub(reg, auxReg, auxReg2);
            //     break;
            case LOGICAL_AND:
                auxReg = imprimeExpressao(p,aux->filho_direito);
                auxReg2 = imprimeExpressao(p,aux->filho_esquerdo);
                // auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
                // auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
                if(auxReg2.tipo =='s' && auxReg.tipo == 's') {
                    freee = regTdisponivel();
                    if(freee != -1){
                        c.num = freee;                    
                        c.tipo = 't';
                    }else{
                        c.tipo = 's';
                        c.num = countVar+1;
                    }
                }else if(auxReg2.tipo =='t' && auxReg.tipo == 't') { 
                    c.num = auxReg2.num;                    
                    c.tipo = auxReg2.tipo;
                    liberaRegT(auxReg.num);
                }else if(auxReg.tipo == 't'){
                    c.num = auxReg.num;                    
                    c.tipo = auxReg.tipo;
                }
                imprimiAND(p->text,auxReg.tipo,auxReg.num,auxReg2.tipo,auxReg2.num,c.tipo,c.num);
                break;
            case LOGICAL_OR:
                auxReg = imprimeExpressao(p,aux->filho_direito);
                auxReg2 = imprimeExpressao(p,aux->filho_esquerdo);
                // auxReg = imprimeExpressao(p,aux->filho_direito,reg+1);
                // auxReg2 = imprimeExpressao(p,aux->filho_esquerdo,auxReg+1);
                if(auxReg2.tipo =='s' && auxReg.tipo == 's') {
                    freee = regTdisponivel();
                    if(freee != -1){
                        c.num = freee;                    
                        c.tipo = 't';
                    }else{
                        c.tipo = 's';
                        c.num = countVar+1;
                    }
                }else if(auxReg2.tipo =='t' && auxReg.tipo == 't') { 
                    c.num = auxReg2.num;                    
                    c.tipo = auxReg2.tipo;
                    liberaRegT(auxReg.num);
                }else if(auxReg.tipo == 't'){
                    c.num = auxReg.num;                    
                    c.tipo = auxReg.tipo;
                }
                imprimiOR(p->text,auxReg.tipo,auxReg.num,auxReg2.tipo,auxReg2.num,c.tipo,c.num);
                break;
            default:
                freee = regTdisponivel();
                if(freee){
                    ocuparRegT(freee);
                }
                c.num = freee;
                c.tipo = 't';
                //printf("--aux %d\n",aux->tipo);
                break;
        }
    }
    return c;
}

void imprimiComandos(ProgramaMips *program,Comandos *auxC,int labelNum){
    RegAlloc reg;
    int scanfVar = 0;
    while (auxC != NULL){
        switch (auxC->tipo){
            case PRINTF:
                if(auxC->cmd_printf){
                    imprimePrintf(program,"printf", auxC->expr_comandos->str,auxC->cmd_printf->str, labelComandos);
                }
                imprimePrintf(program,"printf", auxC->expr_comandos->str,"",labelComandos);
                labelComandos++;
                break;
            case SCANF:
                imprimeScanf(program->text,scanfVar);
                //printf("%s\n",auxC->cmd_scanf->str);
                scanfVar++;
                break;
            case IF:
                //verificar se os if estao indo certinho
                //imprimir comparacoes
                reg = imprimeExpressao(program,auxC->expr_comandos);
                imprimiIF(program->text,reg.tipo,reg.num,labelNum);
                //label
                inseriLabel(program->text,"IF",labelNum);
                //comandos if
                imprimiComandos(program,auxC->cmd_if,labelNum+1);
                //label j pra sair do if
                inseriLabelSaida(program->text,"j EXIT_IF",labelNum);
                //label if_else
                if(auxC->cmd_else){
                    inseriLabel(program->text,"ELSE",labelNum);
                    //comandos else
                    imprimiComandos(program,auxC->cmd_else,labelNum+1);
                }
                //label sair if
                inseriLabel(program->text,"EXIT_IF",labelNum);
                break;
            default:
                break;
        }
       //imprimeExpressao(program,auxC->expr_comandos,reg++);
        auxC = auxC->prox;
    }
}

void imprimiAST(Function_struct *f){
    Expressao *n =(Expressao*) f->nome;
    Expressao *r = f->exp_retorno;
    Variaveis *aux = f->variaveis;
    Comandos *auxC = f->function_comandos;
    ProgramaMips *program = iniciaProgramaStruct();
    do {
        imprimeFunction(program->text, n->str);
        //reservar variaveis e trocar os registradores temporarios por elas no codigo
        while (aux !=NULL){
            Expressao *nomeVar= aux->id;
            //printf("variveis = %s tipo=%d\n contador=%d\n",nomeVar->str,aux->tipo, aux->contador);
            aux = aux->prox;
        }
        imprimiComandos(program,auxC,0);
    } while(f->prox != NULL);     
    imprimeExit(program->text);
    imprimirPrograma(program);
}

Cmd_expressao *novoCmdExpressao(Expressao *exp, Cmd_expressao *prox) {
    Cmd_expressao *novo;
    novo = (Cmd_expressao *) malloc(sizeof(Cmd_expressao));
    novo->exp = exp;
    novo->prox = prox;
    return novo;
}

Expressao *novaExpressao(int tipo, int value, char *str, int dimensaoArray, Expressao *filho_esquerdo, Expressao *filho_direito, float valueFloat){
    Expressao *novo;
    novo = (Expressao *) malloc(sizeof(Expressao));
    novo->tipo = tipo;
    novo->filho_direito = filho_direito;
    novo->filho_esquerdo = filho_esquerdo;
    if(tipo == IDENTIFIER || tipo == STRING){
        char *aux = (char*) malloc(sizeof(char)*(strlen(str)+4));
        strcpy(aux,str);
        aux[strlen(aux)] ='\0'; 
        novo->str = aux;
    }else if(tipo == NUM_INTEGER){
        novo->valueFloat = valueFloat;
    }
    
    // novo->value = value;
    // novo->dimensaoArray = dimensaoArray;
    return novo;
}

Comandos *cmd_generico(int tipo, Expressao *exp, Comandos *cmd1, Comandos *cmd2, Expressao *exp2){
    Comandos *novo = (Comandos *)malloc(sizeof(Comandos));
    novo->tipo = tipo;
    switch(tipo){
        case IF: 
            novo->expr_comandos = exp;
            novo->cmd_if = cmd1;
            novo->cmd_else = cmd2;
            break;
        case PRINTF:
            novo->expr_comandos = exp;
            novo->cmd_printf = exp2;
            break;
        case SCANF:
            novo->expr_comandos = exp;
            novo->cmd_scanf= exp2;
            break;
    }
    return novo;
}

Comandos* setProxGenerico(Comandos *atual, Comandos *prox) {
    atual->prox = prox;
    return atual;
}

Function_struct *novaFunction(Expressao *nome, int tipo_retorno, Expressao *exp_retorno, Parametros *parametros, Variaveis *variaveis, Comandos *function_comandos, Function_struct *proximo) {
    Function_struct *novo = (Function_struct *) malloc(sizeof(Function_struct));
    novo->nome = nome;
    novo->tipo_retorno = tipo_retorno;
    novo->parametros = parametros;
    novo->variaveis = variaveis;
    novo->exp_retorno = exp_retorno;
    novo->function_comandos = function_comandos;
    novo->prox = proximo;
    return novo;
}

Parametros *novoParametro(Expressao *id, int tipo, Parametros *prox) {
    Parametros *novo = (Parametros *) malloc(sizeof(Parametros));
    novo->id = id;
    novo->tipo = tipo;
    novo->prox = prox;
    return novo;
}

Variaveis *novaVariavel(Expressao *id, int tipo, Variaveis *prox) {
    Variaveis *novo = (Variaveis *) malloc(sizeof(Variaveis));
    Registrador novoReg;
    if(id->tipo == IDENTIFIER){
       novoReg.id = id->str;
       novoReg.tipo = tipo;
       regs[countVar] = novoReg; 
       countVar++;
    }
    novo->id = id;
    novo->tipo = tipo;
    novo->prox = prox;
    return novo;
}