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

/*
 * Etapa 2: Indexacao dos arquivos de saida.
 *
 * Abre a arvore B (a)
 * Abre o arquivo f
 * posicao = 0
 * para cada endereco (e) em (f)
 *     le e
 *     ArvoreB_Insere(a, e.cep, posicao);
 *     posicao++
 * Fecha arquivos
 *
 * "posicao" aqui e um CONTADOR DE REGISTRO (0, 1, 2, 3...), exatamente
 * como no pseudocodigo. Nao e um deslocamento em bytes. Por isso, quem
 * for ler o registro de volta a partir dessa posicao (ex.: BuscaCEP.c)
 * precisa multiplicar por sizeof(Endereco) antes do fseek.
 *
 * Corrigido para receber os nomes dos arquivos por linha de comando,
 * em vez de usar "cep.dat"/"indice.dat" fixos. Assim o mesmo programa
 * serve para indexar tanto cep1.dat quanto cep2.dat.
 */

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;
    long posicao = 0;

    if (argc != 3)
    {
        fprintf(stderr, "USO: %s [arquivo_dados] [arquivo_indice]\n", argv[0]);
        fprintf(stderr, "Ex.:  %s cep1.dat indice1.dat\n", argv[0]);
        fprintf(stderr, "      %s cep2.dat indice2.dat\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        printf("Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    ArvoreB *indice = ArvoreB_Abre(argv[2]);

    while (fread(&e, sizeof(Endereco), 1, f) == 1)
    {
        ArvoreB_Insere(indice, e.cep, posicao);
        posicao++;
    }

    fclose(f);
    ArvoreB_Fecha(indice);

    printf("Indice '%s' criado a partir de '%s' (%ld registros).\n",
           argv[2], argv[1], posicao);

    return 0;
}
