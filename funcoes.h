#ifndef _FUNCOES_H_
#define _FUNCOES_H_
    typedef struct item Item_struct;
    typedef struct programaMips ProgramaMips;
    struct item{
        char * comandoImpressao;
        Item_struct *prox;
    };

    struct programaMips{
        Item_struct *data;
        Item_struct *text;
    };
    
    void imprimiprintf();
    ProgramaMips *iniciaProgramaStruct();
    void imprimirPrograma(ProgramaMips *p);
    void imprimiAND(Item_struct *lista,char reg1, int num1, char reg2, int num2,char regD, int numD);
    void imprimirEqual(Item_struct *lista,char reg1, int num1, char reg2, int num2, char regD, int numD);
    void imprimirNotEqual(Item_struct *lista,int reg1, int reg2, int reg3);
    void imprimirGreater(Item_struct *lista,int reg1, int reg2, int reg3);
    void imprimirLess(Item_struct *lista,char reg1, int num1, char reg2, int num2, char regD, int numD);
    void inseriAd(Item_struct *lista,char reg1, int num1, char reg2, int num2, char regD, int numD);
    void imprimiOR(Item_struct *lista,char reg1, int num1, char reg2, int num2, char regD, int numD);
    void inseriSub(int destino, int registrador2, int registrador3);
    void imprimirGreater(Item_struct *lista,int reg1, int reg2, int reg3);
    void imprimirLessEqual(Item_struct *lista,int reg1, int reg2, int reg3);
    void inseriMul(int destino,int registrador2,int registrador3);
    void inseriDiv(int destino,int registrador2,int registrador3);
    void inseriLabel(Item_struct *lista,char *str,int labelNum);
    void inseriLabelSaida(Item_struct *lista,char *str,int labelNum);
    void incrementaLabel();
    void imprimePrintf(ProgramaMips *p,char *label, char* formatPrint,char *variavel,int labelNum);
    void imprimeScanf(Item_struct *lista,int reg);
    void imprimeFunction(Item_struct *lista, char *nome);
    void imprimeExit(Item_struct *lista);
    void imprimiIF(Item_struct *lista, char reg1, int num1,int labelnum);
    
#endif