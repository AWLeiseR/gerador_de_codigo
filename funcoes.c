#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"

int contadorRotulos = 0;
int contadorLabelIf = 0;
int contadorVariaveis = 0;


Item_struct* iniciaData(){
  char *inicio = malloc(sizeof(char)*10);
  Item_struct *item = malloc(sizeof(Item_struct));
  strcpy(inicio,".data\n");
  item->prox=NULL;
  item->comandoImpressao = inicio;
  return item;
}

Item_struct *iniciaText(){
  char *inicio = malloc(sizeof(char)*10);
  Item_struct *item = malloc(sizeof(Item_struct));
  strcpy(inicio,".text\n");
  item->prox=NULL;
  item->comandoImpressao = inicio;
  return item;
}

ProgramaMips *iniciaProgramaStruct(){
  ProgramaMips *i = malloc(sizeof(ProgramaMips));
  i->data = iniciaData();
  i->text = iniciaText();
  return i;
}

Item_struct *inseriRotulo(Item_struct *lista,char* str){
  Item_struct *aux = lista;
  char strAux[]="\t printf";
  char strAux2[]=":\t .asciiz ";
  char* strConcatenada = malloc(sizeof(char)*(strlen(strAux)+strlen(strAux2)+2));
  sprintf(strConcatenada,"%s%d%s",strAux,contadorRotulos,strAux2);
  while(aux->prox != NULL){
    aux = aux->prox;
  }
  char *inicio = malloc(sizeof(char)*(strlen(strConcatenada)));
  Item_struct *item = malloc(sizeof(Item_struct));
  strcpy(inicio,strConcatenada);
  item->prox=NULL;
  item->comandoImpressao = inicio;
  aux->prox = item;
  return lista;
}

Item_struct* inseriLista(Item_struct *lista, Item_struct *x){
  Item_struct *aux = lista;
  while (aux->prox != NULL){
    aux = aux->prox; 
  }
  aux->prox = x;
  return lista;
}

void imprimirPrograma(ProgramaMips *p){
  Item_struct *data = p->data;
  Item_struct *text = p->text;
  Item_struct *aux = data;
  while(aux!=NULL){
    printf("%s",aux->comandoImpressao);
    aux =aux->prox;
  }
  aux = text;
  while(aux!=NULL){
    printf("%s",aux->comandoImpressao);
    aux =aux->prox;
  }
}

void inseriAd(Item_struct *lista, char *destino,char *registrador2,char *registrador3){
  Item_struct *i1 = malloc(sizeof(Item_struct));
  // printf("add $t%d, %d, %d\n", destino, registrador2, registrador3);
  char *c = malloc(sizeof(char)*25);
  sprintf(c,"\tadd %s, %s, %s\n", destino, registrador2, registrador3);
  i1->comandoImpressao = c;
  inseriLista(lista,i1);
}

void inseriSub(int destino, int registrador2, int registrador3){
  printf("sub %d, %d, %d\n", destino, registrador2, registrador3);
  // char *c = malloc(sizeof(char)+(strlen(destino)+strlen(registrador2)+strlen(registrador3)+10));
  // sprintf(c,"sub %s, %s,%s\n", destino, registrador2, registrador3);
  //return c;
}

void inseriMul(int destino,int registrador2,int registrador3){
  printf("mul %d, %d, %d\n", destino, registrador2, registrador3);
  // char *c = malloc(sizeof(char)*18);
  // sprintf(c,"add %d, %d, %d\n", destino, registrador2, registrador3);
  // return c;
}

void inseriDiv(int destino,int registrador2,int registrador3){
  printf("div %d, %d, %d\n", destino, registrador2, registrador3);
  // char *c = malloc(sizeof(char)*18);
  // sprintf(c,"add %d, %d, %d\n", destino, registrador2, registrador3);
  // return c;
}

void inseriLabel(Item_struct *lista,char *str,int labelNum){
  Item_struct *i1 = malloc(sizeof(Item_struct));
  //printf("%s%d:\n",str,labelNum);
  char *c = malloc(sizeof(char)*(strlen(str)+15));
  sprintf(c,"%s%d:\n",str,labelNum);
  i1->comandoImpressao = c;
  inseriLista(lista,i1);
}

void inseriLabelSaida(Item_struct *lista,char *str,int labelNum){
  Item_struct *i1 = malloc(sizeof(Item_struct));
  //printf("%s%d:\n",str,labelNum);
  char *c = malloc(sizeof(char)*(strlen(str)+15));
  sprintf(c,"%s%d\n",str,labelNum);
  i1->comandoImpressao = c;
  inseriLista(lista,i1);
}

//0 para int e 1 para string
void imprimePrintf(ProgramaMips *p,char *label, char* formatPrint,char *variavel, int labelNum){
  int i,tipo=0;
  char c;
  if(strlen(variavel) > 0){
    for(i=0;i<strlen(formatPrint);i++){
    if(formatPrint[i]=='%' && formatPrint[i+1]){
      tipo=1;
      break;
    }
  }
  }else{
    Item_struct *i1 = malloc(sizeof(Item_struct));
    char * linha1;
    linha1 = malloc(sizeof(char)*(strlen(formatPrint)+30));
    sprintf(linha1,"\tprintf%d:\t .asciiz %s\n",labelNum,formatPrint);
    i1->comandoImpressao = linha1;
    inseriLista(p->data,i1);
  }
  
  if(!tipo){
    Item_struct *i2 = malloc(sizeof(Item_struct));
    Item_struct *i3 = malloc(sizeof(Item_struct));
    char *linha2, *linha3;
    linha2 = malloc(sizeof(char)*30);
    sprintf(linha2, "\taddi $v0, $zero, 4\n");
    i2->comandoImpressao = linha2;
    linha3 = malloc(sizeof(char)*(strlen(label)+30));
    sprintf(linha3, "\tla $a0, %s%d\n",label,contadorRotulos);
    i3->comandoImpressao = linha3;
    inseriLista(p->text,i2);
    inseriLista(p->text,i3);
  }else{
    // precisa checar os comandos mips
    Item_struct *i4 = malloc(sizeof(Item_struct));
    Item_struct *i5 = malloc(sizeof(Item_struct));
    char *linha4, *linha5;
    linha4 = malloc(sizeof(char)*(strlen(label)+30));
    sprintf(linha4, "\tli $v0, %s%d\n",label,contadorRotulos);
    i4->comandoImpressao = linha4;
    linha5 = malloc(sizeof(char)*30);
    sprintf(linha5, "\tmove $a0, 4, $zero\n");
    i5->comandoImpressao = linha5;
    inseriLista(p->text,i4);
    inseriLista(p->text,i5);
  }
  contadorRotulos++;
  Item_struct *i6 = malloc(sizeof(Item_struct));
  char *linha6;
  linha6 = malloc(sizeof(char)*15);
  sprintf(linha6, "\tsyscall\n");
  i6->comandoImpressao = linha6;
  inseriLista(p->text,i6);
}

void imprimeScanf(Item_struct *lista,int reg){
  Item_struct *i1 = malloc(sizeof(Item_struct));
  Item_struct *i2 = malloc(sizeof(Item_struct));
  Item_struct *i3 = malloc(sizeof(Item_struct));
  char *linha1, *linha2, *linha3;
  linha1 = malloc(sizeof(char)*25);
  sprintf(linha1,"\tli $v0, 5\n");
  i1->comandoImpressao = linha1;
  linha2 = malloc(sizeof(char)*25);
  sprintf(linha2,"\tsyscall\n");
  i2->comandoImpressao = linha2;
  linha3 = malloc(sizeof(char)*25);
  sprintf(linha3,"\tadd $s%d, $zero, $v0\n",reg);
  i3->comandoImpressao = linha3;
  inseriLista(lista,i1);
  inseriLista(lista,i2);
  inseriLista(lista,i3);
}

void imprimeFunction(Item_struct *lista, char *nome) {
  Item_struct *i1 = malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*strlen(nome)+10);
  sprintf(linha1, "%s:\n", nome);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimeExit(Item_struct *lista){
  Item_struct *i1= malloc(sizeof(Item_struct));
  Item_struct *i2= malloc(sizeof(Item_struct));
  char *linha1, *linha2;
  linha1 = malloc(sizeof(char)*30);
  linha2 = malloc(sizeof(char)*25);
  sprintf(linha1,"\taddi $v0, $zero, 10\n");
  i1->comandoImpressao = linha1;
  sprintf(linha2,"\tsyscall\n");
  i2->comandoImpressao = linha2;
  inseriLista(lista,i1);
  inseriLista(lista,i2);
}

void imprimiIF(Item_struct *lista, char *reg1,int labelnum){
  //printf("beqz $t%d, ELSE%d:\n",reg1);
   Item_struct *i = malloc(sizeof(Item_struct));
  char *linha1, *linha2, *linha3;
  linha1 = malloc(sizeof(char)*35);
  // inserir label aqui
  sprintf(linha1,"\tbeqz %s, IF%d\n",reg1,labelnum);
  //sgt maior q
  i->comandoImpressao = linha1;
  inseriLista(lista,i);
  
}

void imprimiAND(Item_struct *lista, char *reg1, char *reg2, char *reg3){
  // printf("sne $t%d, $zero, $t%d\n",reg1,reg1);
  // printf("sne $t%d, $zero, $t%d\n",reg2,reg2);
  // printf("and $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  Item_struct *i2= malloc(sizeof(Item_struct));
  Item_struct *i3= malloc(sizeof(Item_struct));
  char *linha1, *linha2, *linha3;
  linha1 = malloc(sizeof(char)*30);
  sprintf(linha1,"\tsne %s, $zero, %s\n",reg1,reg1);
  i1->comandoImpressao = linha1;
  linha2 = malloc(sizeof(char)*30);
  sprintf(linha2,"\tsne %s, $zero, %s\n",reg2,reg2);
  i2->comandoImpressao = linha2;
  linha3 = malloc(sizeof(char)*30);
  sprintf(linha3,"\tand %s, %s, %s\n",reg3,reg1,reg2);
  i3->comandoImpressao = linha3;
  inseriLista(lista,i1);
  inseriLista(lista,i2);
  inseriLista(lista,i3);
}

void imprimiOR(Item_struct *lista, char *reg1, char *reg2, char *reg3){
  // printf("sne $t%d, $zero, $t%d\n",reg1,reg1);
  // printf("sne $t%d, $zero, $t%d\n",reg2,reg2);
  // printf("or $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  Item_struct *i2= malloc(sizeof(Item_struct));
  Item_struct *i3= malloc(sizeof(Item_struct));
  char *linha1, *linha2, *linha3;
  linha1 = malloc(sizeof(char)*25);
  sprintf(linha1,"\tsne %s, $zero, %s\n",reg1,reg1);
  i1->comandoImpressao = linha1;
  linha2 = malloc(sizeof(char)*25);
  sprintf(linha2,"\tsne %s, $zero, %s\n",reg2,reg2);
  i2->comandoImpressao = linha2;
  linha3 = malloc(sizeof(char)*25);
  sprintf(linha3,"\tor %s, %s, %s\n",reg3,reg1,reg2);
  i3->comandoImpressao = linha3;
  inseriLista(lista,i1);
  inseriLista(lista,i2);
  inseriLista(lista,i3);
}

void imprimirEqual(Item_struct *lista,char *reg1, char *reg2, char *reg3){
  //printf("seq $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*30);
  sprintf(linha1,"\tseq %s, %s, %s\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimirNotEqual(Item_struct *lista,int reg1, int reg2, int reg3){
  //printf("sne $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*22);
  sprintf(linha1,"\tsne $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimirGreaterEqual(Item_struct *lista,int reg1, int reg2, int reg3){
  //printf("sge $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*22);
  sprintf(linha1,"\tsge $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimirGreater(Item_struct *lista,int reg1, int reg2, int reg3){
  //printf("sgt $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*22);
  sprintf(linha1,"\tsgt $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimirLessEqual(Item_struct *lista,int reg1, int reg2, int reg3){
  //printf("sle $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*22);
  sprintf(linha1,"\tsle $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

void imprimirLess(Item_struct *lista, char *reg1, char *reg2, char *reg3){
  //printf("slt $t%d, $t%d, $t%d\n",reg3,reg1,reg2);
  Item_struct *i1= malloc(sizeof(Item_struct));
  char *linha1;
  linha1 = malloc(sizeof(char)*30);
  sprintf(linha1,"\tslt %s, %s, %s\n",reg3,reg1,reg2);
  i1->comandoImpressao = linha1;
  inseriLista(lista,i1);
}

