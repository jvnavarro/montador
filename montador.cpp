#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

/*
    programa assembler => montador => mv

    tabela simbolos (associar nome de variavel com endereço) -> struct com nome e endereço;

    $montador soma.txt soma

        argc = 3;
        argv[0] = montador;
        argv[1] = soma.txt;
        argv[2] = soma;
        int main(argc, char argv[])

        Erros:
            1ª passagem:
                -redeclarar variavel
                -local indevido variavel
                -variavel nao declarada
            2ª passagem:
                -codigo de operacao inexistente

*/
//Structs
typedef struct tabelaSimbolos
{
    int endereco;
    string nome;
}tipoTabelaSimbolos;
typedef struct tabelaInstrucoes
{
    int codigo;
    string nome;
}tipoTabelaInstrucoes;

//Variaveis globais
    tipoTabelaInstrucoes tabelaInst[13];
    tipoTabelaSimbolos tabelaSimbolos[50];
    int contadorAreaDados = 0;
    int contadorIndiceTabSimbolos = 0; 
    int contadorNumVariaveis = 0;
    int contadorEndAtual = 0;
    bool leuVariavelOuLabel = false;
    bool leuInst = false;
    bool numeroIndevidoTokens = false;
// Outras
void carrega_tabela_inst();
void escrever_arq(int codigo, int end);
void print_tabela_simbolos();
// F
bool verifica_se_eh_inst(string string);
void insercao_tabela_simbolos(int contadorTokens, string token1, string token2, string token3);
bool checa_decl_var_lugar_errado(string token2);
bool verifica_var_ja_existe(string string);
void atribuicao_flags(int contadorTokens);
void construir_tabela_simbolos();
void primeira_passagem();
// Funções usadas na segunda passagem
int retorna_end_var(string string);
int retorna_codigo_inst(string string);
void retorna_codigo_end_e_inst(int* codigoInst, int* endVar, string token1, string token2, string token3);
void gerar_codigo();
void segunda_passagem();

int main(int argc, char *argv[])
{
    carrega_tabela_inst();
    primeira_passagem();
    segunda_passagem();

    return 0;
}
// Outras
void carrega_tabela_inst()
{
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
}
void escrever_arq(int codigo, int end)
{
    ofstream escrita;

    escrita.open("programa_decimal.txt", ofstream::app);
    if(!escrita.is_open( )) 
    { 
        cout << "Não foi possível abrir arquivo! Programa será terminado.\n"; 
        escrita.clear( ); //reseta o objeto leitura, para limpar memória do sistema 
        return; 
    }
    if(codigo == 12)
        escrita << codigo << " " << "0\n";
    else
    { 
        escrita << codigo << " " << end << "\n";
    }
    escrita.close();
}
void print_tabela_simbolos()
{
    for(int i = 0; i < 50; i++)
    {
        cout << "nome: " << tabelaSimbolos[i].nome << "\tendereco: " << tabelaSimbolos[i].endereco << "\n"; 
    }
}
// Funções usadas na primeira passagem
bool verifica_se_eh_inst(string string)
{
    for(int i = 0; i < 13; i++)
    {
        if(string == tabelaInst[i].nome)
        {
            return true;
        }
    }
    return false;
}
void insercao_tabela_simbolos(int contadorTokens, string token1, string token2, string token3)
{
    switch (contadorTokens)
    {
        case 1:
            contadorEndAtual++;
            break;
        case 2:
            contadorEndAtual++;
            break;
        case 3:
            //se ler uma instrucao e vier pro case 3, quer dizer
            //que uma variavel foi declarada no meio da parte de codigo
            // ou entao uma label foi adicionada a uma inst.
            if(verifica_se_eh_inst(token2) == true)
            {
                tabelaSimbolos[contadorIndiceTabSimbolos].endereco = contadorEndAtual;
                tabelaSimbolos[contadorIndiceTabSimbolos].nome = token1;
                contadorIndiceTabSimbolos++;
                contadorEndAtual++;
            }
            else
            {
                int espacoVar = stoi(token3);
                contadorNumVariaveis++;
                for(int i = 0; i < espacoVar; i++)
                {
                    tabelaSimbolos[contadorAreaDados].endereco = contadorAreaDados;
                    tabelaSimbolos[contadorAreaDados].nome = token1;
                    contadorAreaDados++;
                    contadorIndiceTabSimbolos++;
                    contadorEndAtual++;
                }
            }      
            break;
        default:
            break;
    }
}
bool checa_decl_var_lugar_errado(string token2)
{
    string espaco = "SPACE";
    if(token2 == espaco)
        return true;
    else
        return false;
}
bool verifica_var_ja_existe(string string)
{
    int i = 0;
    for(i; i < 50; i++)
    {
        if(string == tabelaSimbolos[i].nome)
        {
            cout << "Erro: a variavel"<< string << " ja foi declarada anteriormente\n";
            return true;
        }
    }
    return false;
}
void atribuicao_flags(int contadorTokens)
{
    switch(contadorTokens)
    {
        case 1: leuInst = true;
            break;
        case 2: leuInst = true;
            break;
        case 3: leuVariavelOuLabel = true;
            break;
        default: numeroIndevidoTokens = true;
            break;
    }
}
void construir_tabela_simbolos()
{
    char linha[300];
    string token1, token2, token3, token;

    ifstream leitura;

    leitura.open("programa_assembly.txt");
    if (!leitura.is_open())
    {
        cout << "Não foi possível abrir arquivo! Programa será terminado!\n";
        leitura.clear(); //reseta o objeto leitura, para limpar memória do sistema
    }
    while (leitura.getline(linha, 300))
    {
        stringstream linha_lida_stream(linha), linha_lida_stream_count(linha);
        int contadorTokens = 0;

        linha_lida_stream.str(string(linha)); // Define o conteúdo do stringstream como a nova linha
        linha_lida_stream_count.str(string(linha));

        linha_lida_stream >> token1 >> token2 >> token3;
        while (linha_lida_stream_count >> token) // Enquanto houver tokens para serem lidos
            contadorTokens++;
        atribuicao_flags(contadorTokens);
        //Erro variavel redeclarada
        if((leuInst == false) && (verifica_var_ja_existe(token1) == true))
        {
            cout << "Uma variavel com este nome: " << token1 << " Jah existe.\n";
            return;
        }
        //Erro variavel declarada no local errado
        if((leuInst == true) && (checa_decl_var_lugar_errado(token2) == true))
        {   
            cout << "Erro: Variavel declarada em local indevido.\n";
            return;
        }
        //Erro linha + de 3 tokens
        if(numeroIndevidoTokens == true)
        {
            cout << "O montador nao permite linhas com mais de 3 tokens.\n";
            return;
        }
        insercao_tabela_simbolos(contadorTokens, token1, token2, token3);
    }
    leitura.close();
}
void primeira_passagem()
{
    construir_tabela_simbolos();
    escrever_arq(contadorAreaDados, 00);
    print_tabela_simbolos();
}
// Funções usadas na segunda passagem
int retorna_end_var(string string)
{
    for(int i = 0; i < 50; i++)
    {
        if(tabelaSimbolos[i].nome == string)
            return tabelaSimbolos[i].endereco;
    }
    if(stoi(string) > 0)
        return stoi(string);
    cout << "Erro: variavel nao existe na tabela de simbolos.\n";
    return -1;
}
int retorna_codigo_inst(string string)
{
    for(int i = 0; i < 13; i++)
    {
        
        if(tabelaInst[i].nome == string)
            return tabelaInst[i].codigo;
    }
    return -1;
}
void retorna_codigo_end_e_inst(int* codigoInst, int* endVar, string token1, string token2, string token3)
{
    //Tem label
    if(verifica_se_eh_inst(token1) == false)
    {
        *codigoInst = retorna_codigo_inst(token2);
        *endVar = retorna_end_var(token3);
    }
    //Não tem label
    else
    {
        *codigoInst = retorna_codigo_inst(token1);
        *endVar = retorna_end_var(token2);
    }
}
void gerar_codigo()
{
    char linha[300];
    string token1, token2, token3, token;
    int codigoInst, endVar, i=0, contadorTokens = 0;
    bool leuInst = false;

    ifstream leitura;

    leitura.open("programa_assembly.txt");
    if (!leitura.is_open())
    {
        cout << "Não foi possível abrir arquivo! Programa será terminado!\n";
        leitura.clear(); //reseta o objeto leitura, para limpar memória do sistema
    }
    while (leitura.getline(linha, 300))
    {
        stringstream linha_lida_stream(linha), linha_lida_stream_count(linha);

        cout << linha << "\n";
        linha_lida_stream.str(string(linha)); // Define o conteúdo do stringstream como a nova linha

        if (i < contadorNumVariaveis) 
        {
            i++;
            continue; // Isso pulará para a próxima iteração do loop
        }
        
        linha_lida_stream >> token1 >> token2 >> token3;
        
        retorna_codigo_end_e_inst(&codigoInst, &endVar, token1, token2, token3);
        if(codigoInst == -1)
        {
            cout << "Erro: a instrucao inserida nao existe na tabela de instrucoes.\n";
            return;
        }
        if(endVar == -1)
        {
            cout << "Erro: o endereco inserido eh negativo ou nao existe na tabela de simbolos.\n";
            return;
        }    
        escrever_arq(codigoInst, endVar);
    }
    leitura.close();
}
void segunda_passagem()
{
    gerar_codigo();
}