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
 * Utilitario de busca de um unico CEP num arquivo de dados + indice.
 * Corrigido para receber os nomes dos arquivos por linha de comando,
 * em vez de usar "cep.dat"/"indice.dat" fixos -- assim serve para
 * testar tanto (cep1.dat, indice1.dat) quanto (cep2.dat, indice2.dat).
 */

int main(int argc, char **argv)
{
    FILE *f;
    Endereco e;
    long posicao;
    char chave[TAM_CHAVE];

    if (argc != 4)
    {
        fprintf(stderr, "USO: %s [arquivo_dados] [arquivo_indice] [CEP]\n", argv[0]);
        fprintf(stderr, "Ex.:  %s cep1.dat indice1.dat 12345678\n", argv[0]);
        return 1;
    }

    if (strlen(argv[3]) > TAM_CHAVE)
    {
        fprintf(stderr, "CEP invalido: '%s' tem mais de %d caracteres.\n", argv[3], TAM_CHAVE);
        return 1;
    }

    /* O cep no arquivo ocupa exatamente TAM_CHAVE bytes, sem terminador e
     * sem separadores. Completa com zeros a esquerda para nao perder a
     * busca quando o usuario digitar o CEP sem os zeros iniciais
     * (ex.: "1310000" em vez de "01310000"). */
    memset(chave, '0', TAM_CHAVE);
    memcpy(chave + (TAM_CHAVE - strlen(argv[3])), argv[3], strlen(argv[3]));

    f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        printf("Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    ArvoreB *indice = ArvoreB_Abre(argv[2]);

    posicao = ArvoreB_Busca(indice, chave);

    if (posicao == -1)
    {
        printf("CEP nao encontrado.\n");
    }
    else
    {
        /* posicao agora e um INDICE DE REGISTRO (0, 1, 2...), nao um
         * deslocamento em bytes -- por isso multiplica por sizeof(Endereco)
         * antes do fseek. */
        fseek(f, posicao * (long) sizeof(Endereco), SEEK_SET);

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
