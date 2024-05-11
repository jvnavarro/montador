/*
    Olá professor, bem vindo a minha humilde máquina virtual, ela faz tudo
    que o senhor pediu na especificação. Peço desculpas de antemão pelos 
    ponteiros por parâmetro, ia fazer com variável global, mas já tinha feito assim
    e fiquei com preguiça de mudar, onde tem "*ptrPC" na CPU é simplesmente o valor 
    que está na variável PC da main. 

    Fiz um programa que testa todas instruções da especificação, fiquei em dúvida
    quanto a PW (print word), fiz minha máquina imprimir o operador e operando do 
    endereço especificado no operador de PW, foi como entendi.

    Vou deixar neste comentário como o programa está organizado após carregado
    na  memória, o operador coloquei na forma mnemonica para facilitar a leitura. 
    Para lhe poupar esforço mental de interpretar o assembly, é um programa que 
    le dois valores do teclado e realiza uma operação, coloquei um branch de cada tipo para demonstrar o funcionamento da instrução.

    Passo a passo do programa:

        1º. Então, começando do começo, coloque dois valores e eles serão adicionados, se a soma
            der 0 ele pede valores novamente. 
        2º. Se não, segue o rumo, pedindo dessa vez dois 
            valores para multiplicar, se der maior que 0, pede valores novamente.
        3º  Se não, segue o rumo, pedindo dessa vez dois valores para dividir, aqui com
            um jump incondicional, que pula um HLT.
        4º  Por fim, o programa pede dois valores e subtrai-os, se der negativo ele pede 
            dois valores novamente, se não, o programa chega ao fim.
        ___________________________________________
        |   Addr.   |    Operador   |   Operando    |   
        |-----------|---------------|---------------|
        |     00    |      SPACE    |       00      |
        |     01    |      SPACE    |       00      |      
        |     02    |      SPACE    |       00      |
        |     03    |      RW       |       00      |
        |     04    |      RW       |       01      |
        |     05    |      LA       |       00      |
        |     06    |      ADD      |       02      |
        |     07    |      SA       |       02      |
        |     09    |      JEQ      |       00      |
        |     08    |      PW       |       03      |
        |     10    |      RW       |       00      |
        |     11    |      RW       |       01      |
        |     12    |      LA       |       00      |
        |     13    |      MUL      |       01      | 
        |     14    |      SA       |       02      |
        |     15    |      PW       |       02      |
        |     16    |      JGT      |       10      |
        |     17    |      RW       |       00      |
        |     18    |      RW       |       01      |
        |     19    |      LA       |       00      |
        |     20    |      DIV      |       01      |
        |     21    |      SA       |       02      |
        |     22    |      PW       |       02      |
        |     23    |      JMP      |       25      |
        |     24    |      HLT      |       00      |
        |     25    |      RW       |       00      |
        |     26    |      RW       |       01      |
        |     27    |      LA       |       00      |
        |     28    |      SUB      |       01      |
        |     29    |      SA       |       02      |
        |     30    |      PW       |       02      |
        |     31    |      JLT      |       25      |
        |     32    |      HLT      |       00      |
        |     ...   |      ...      |       ...     |
        |_____127___|_______________|_______________|
*/  
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <string.h>
#include <fstream>
#include <sstream>

#define STOP 12
using namespace std;
typedef struct mem
{
    int operador;
    int operando;
} mem;

typedef struct tabelaInstrucoes
{
    int codigo;
    string nome;
}tipoTabelaInstrucoes;

tipoTabelaInstrucoes tabelaInst[13];

int R1, tamAreaDados;

// rocketseat

void loader(mem memoria[], int* ptrPC)
{
    FILE *f;
    int i = 0, j = 0;
    int operador, operando;

    f = fopen("programa_decimal.txt", "r");

    fscanf(f, "%d %d\n", &i, &j);
    *ptrPC = i;
    tamAreaDados = i;

    while (!feof(f))
    {
        fscanf(f, "%d %d\n", &memoria[i].operador, &memoria[i].operando);
        i++;
    }
    R1 = i - 1;
}

void CPU(mem memoria[], int* ptrPC)
{
    int ACC = 0;
    int R0 = *ptrPC;
    while(memoria[*ptrPC].operador != STOP)
    {
        printf("PC: %d\nACC: %d\nOpdr.: %d\t|\tOpnd.: %d\n", *ptrPC, ACC, memoria[*ptrPC].operador, memoria[*ptrPC].operando);
         // Procurando o nome da instrução na tabela de instruções
        if(memoria[*ptrPC].operador >= 0 && memoria[*ptrPC].operador <= 12)
        {
            cout << "Opdr.: " << tabelaInst[memoria[*ptrPC].operador].nome << "\t|\tOpnd.: " << memoria[*ptrPC].operando << "\n";
        }
        else
        {
            cout << "Código de instrução inválido\n";
        }
        printf("Area de Dados:\n");
        for(int i = 0; i < tamAreaDados; i++)
        {
            printf("|\taddr. %.3d \t|\toptr: %d \t|\topnd: %d \t|\n", i, memoria[i].operador, memoria[i].operando);
        }
        //printf("addrs 0: var1 = %d\naddrs 1: var2 = %d\naddrs 2: var3 = %d\n\n", memoria[0].operando, memoria[1].operando, memoria[2].operando);
        switch(memoria[*ptrPC].operador)
        {
            case 0:
                //LA op, cod. 00
                ACC = memoria[memoria[*ptrPC].operando].operando;
                break;
            case 1:
                    //SA op, cod. 01
                memoria[memoria[*ptrPC].operando].operando = ACC;
                break;
            case 2:
                    //AA op, cod 02
                ACC += memoria[memoria[*ptrPC].operando].operando;
                break;
            case 3:
                    //MUL op, cod 03
                ACC *= memoria[memoria[*ptrPC].operando].operando;
                break;
            case 4:
                ACC /= memoria[memoria[*ptrPC].operando].operando;
                break;
            case 5:
                ACC -= memoria[memoria[*ptrPC].operando].operando;
                break;
            case 6:
                *ptrPC = memoria[*ptrPC].operando;
                printf("jump incondicional feito para o endereco: %d\n\n", *ptrPC);
                *ptrPC -= 1;
                break;
            case 7:
                if (ACC == 0)
                {
                    *ptrPC = memoria[*ptrPC].operando;
                    printf("ACC = 0, branch feito para o endereco: %d\n\n", *ptrPC);
                    *ptrPC -= 1;
                }
                break;
            case 8:
                if (ACC > 0)
                {
                    *ptrPC = memoria[*ptrPC].operando;
                    printf("ACC > 0, branch feito para o endereco: %d\n\n", *ptrPC);
                    *ptrPC -= 1;
                }
                break;
            case 9:
                if (ACC < 0)
                {
                    *ptrPC = memoria[*ptrPC].operando;
                    printf("ACC < 0, branch feito para o endereco: %d\n\n", *ptrPC);
                    *ptrPC -= 1;
                }
                break;
            case 10:
                printf("Addrs -> %d Operando -> %d\n", memoria[*ptrPC].operando ,memoria[memoria[*ptrPC].operando].operando);
                break;
            case 11:
                printf("Digite um valor: ");
                scanf("%d", &memoria[memoria[*ptrPC].operando].operando);
                break;
            case 12: 
                printf("Programa finalizado!\n");
                break;
            }
            
        *ptrPC += 1;
        if(memoria[*ptrPC].operador == 12)
        {
            printf("Programa finalizado!\n");
        }
    }
}
void print_mem(mem mem[])
{
    printf("|\taddr:\t|\tOpdr:\t|\tOpnd:\t|\n");
    for(int i = 0; i < 128; i++)
    {
        printf("|\t%d\t|\t%d\t|\t%d\t\n", i, mem[i].operador, mem[i].operando);
    }
}
int main()
{
    //ptrPC pra usar nas duas funções
    mem memoria[128];
    int memDados;
    int PC = 0;

    tabelaInst[0].codigo = 0;
    tabelaInst[0].nome = "LDR";
    tabelaInst[1].codigo = 1;
    tabelaInst[1].nome = "STR";
    tabelaInst[2].codigo = 2;
    tabelaInst[2].nome = "ADD";
    tabelaInst[3].codigo = 3;
    tabelaInst[3].nome = "MUL";
    tabelaInst[4].codigo = 4;
    tabelaInst[4].nome = "DIV";
    tabelaInst[5].codigo = 5;
    tabelaInst[5].nome = "SUB";
    tabelaInst[6].codigo = 6;
    tabelaInst[6].nome = "JMP";
    tabelaInst[7].codigo = 7;
    tabelaInst[7].nome = "JEQ";
    tabelaInst[8].codigo = 8;
    tabelaInst[8].nome = "JGT";
    tabelaInst[9].codigo = 9;
    tabelaInst[9].nome = "JLT";
    tabelaInst[10].codigo = 10;
    tabelaInst[10].nome = "PW";
    tabelaInst[11].codigo = 11;
    tabelaInst[11].nome = "RW";
    tabelaInst[12].codigo = 12;
    tabelaInst[12].nome = "STOP";

    loader(memoria, &PC);
    for(int i = 0; i < tamAreaDados; i++)
    {
        memoria[i].operador = 0;
        memoria[i].operando = 0;
    }
    print_mem(memoria);
    CPU(memoria, &PC);
    //printf("O valor armazenado na var = %d", memoria[0].operando);
    }