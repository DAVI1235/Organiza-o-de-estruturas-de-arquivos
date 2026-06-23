#include <stdio.h>
#include <string.h>
#include "ArvoreB.h"

typedef struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

int main()
{
    FILE *f;
    Endereco e;
    long posicao = 0;

    f = fopen("cep.dat","rb");

    if(f == NULL)
    {
        printf("Erro ao abrir cep.dat\n");
        return 1;
    }

    ArvoreB *indice = ArvoreB_Abre("indice.dat");

    while(fread(&e, sizeof(Endereco), 1, f))
    {
        ArvoreB_Insere(indice, e.cep, posicao);
        posicao += sizeof(Endereco);
    }

    fclose(f);
    ArvoreB_Fecha(indice);

    printf("Indice criado com sucesso.\n");

    return 0;
}