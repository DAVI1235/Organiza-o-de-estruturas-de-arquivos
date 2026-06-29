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
 * Etapa 3: Juncao dos arquivos.
 *
 * Abre o arquivo de saida: ceps que estao nos dois arquivos.
 * Abre o Arquivo a1.
 * Abre a arvore do arquivo a2.
 * Para cada endereco e no arquivo a1
 *     Buscar o endereco e na arvore (do a2).
 *     Se encontrar (!= -1):
 *         o cep esta nos dois arquivos.
 *         copiar o endereco para o arquivo de saida.
 * Fecha os arquivos.
 *
 * O arquivo de saida tem os ceps que estao simultaneamente
 * nos arquivos a1 e a2.
 *
 * Uso:
 *   Juncao cep1.dat indice2.dat saida.dat
 *
 * (le os enderecos de cep1.dat e busca cada CEP no indice de cep2.dat)
 */

int main(int argc, char **argv)
{
    FILE *fa1, *fsaida;
    Endereco e;
    long posicao;
    long totalA1 = 0, totalComuns = 0;

    if (argc != 4)
    {
        fprintf(stderr, "USO: %s [arquivo_a1] [indice_a2] [arquivo_saida]\n", argv[0]);
        fprintf(stderr, "Ex.:  %s cep1.dat indice2.dat comuns.dat\n", argv[0]);
        return 1;
    }

    fa1 = fopen(argv[1], "rb");
    if (fa1 == NULL)
    {
        printf("Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    fsaida = fopen(argv[3], "wb");
    if (fsaida == NULL)
    {
        printf("Erro ao criar %s\n", argv[3]);
        fclose(fa1);
        return 1;
    }

    ArvoreB *indiceA2 = ArvoreB_Abre(argv[2]);

    while (fread(&e, sizeof(Endereco), 1, fa1) == 1)
    {
        totalA1++;

        posicao = ArvoreB_Busca(indiceA2, e.cep);

        if (posicao != -1)
        {
            /* o cep esta nos dois arquivos: copia o endereco de a1 */
            fwrite(&e, sizeof(Endereco), 1, fsaida);
            totalComuns++;
        }
    }

    fclose(fa1);
    fclose(fsaida);
    ArvoreB_Fecha(indiceA2);

    printf("Enderecos lidos de %s: %ld\n", argv[1], totalA1);
    printf("CEPs em comum (gravados em %s): %ld\n", argv[3], totalComuns);

    return 0;
}
