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

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;
    long posicao;

    if(argc != 2)
    {
        fprintf(stderr,"USO: %s [CEP]\n",argv[0]);
        return 1;
    }

    f = fopen("cep.dat","rb");

    if(f == NULL)
    {
        printf("Erro ao abrir cep.dat\n");
        return 1;
    }

    ArvoreB *indice = ArvoreB_Abre("indice.dat");

    posicao = ArvoreB_Busca(indice, argv[1]);

    if(posicao == -1)
    {
        printf("CEP nao encontrado.\n");
    }
    else
    {
        fseek(f, posicao, SEEK_SET);

        fread(&e, sizeof(Endereco), 1, f);

        printf("%.72s\n", e.logradouro);
        printf("%.72s\n", e.bairro);
        printf("%.72s\n", e.cidade);
        printf("%.72s\n", e.uf);
        printf("%.2s\n", e.sigla);
        printf("%.8s\n", e.cep);
    }

    fclose(f);
    ArvoreB_Fecha(indice);

    return 0;
}