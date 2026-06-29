#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Etapa 1: Geracao dos arquivos de dados.
 *
 * Para cada endereco (e) no arquivo de entrada (arquivo mestre de CEPs):
 *     le o endereco e.
 *     sorteia um numero entre 0 e 1.
 *     se o sorteio for < 0.8 (80%), grava o registro no arquivo de saida.
 *
 * Use este programa DUAS vezes, com sementes diferentes, para gerar
 * cep1.dat e cep2.dat a partir do mesmo arquivo mestre. Cada saida tera
 * ~80% dos registros, mas conjuntos diferentes (alguns CEPs vao cair
 * nos dois, outros so em um) -- e exatamente isso que a Etapa 3 (Juncao)
 * vai explorar.
 */

int main(int argc, char **argv)
{
    FILE *fin, *fout;
    Endereco e;
    unsigned int semente;
    long totalLidos = 0, totalGravados = 0;

    if (argc != 4)
    {
        fprintf(stderr, "USO: %s [arquivo_entrada] [arquivo_saida] [semente]\n", argv[0]);
        fprintf(stderr, "Ex.:  %s cep.dat cep1.dat 1\n", argv[0]);
        fprintf(stderr, "      %s cep.dat cep2.dat 2\n", argv[0]);
        return 1;
    }

    fin = fopen(argv[1], "rb");
    if (fin == NULL)
    {
        printf("Erro ao abrir %s\n", argv[1]);
        return 1;
    }

    fout = fopen(argv[2], "wb");
    if (fout == NULL)
    {
        printf("Erro ao criar %s\n", argv[2]);
        fclose(fin);
        return 1;
    }

    semente = (unsigned int) atoi(argv[3]);
    srand(semente);

    while (fread(&e, sizeof(Endereco), 1, fin) == 1)
    {
        totalLidos++;

        double sorteio = (double) rand() / (double) RAND_MAX; /* numero entre 0 e 1 */

        if (sorteio < 0.8) /* 80% dos registros vao para o arquivo de saida */
        {
            fwrite(&e, sizeof(Endereco), 1, fout);
            totalGravados++;
        }
    }

    fclose(fin);
    fclose(fout);

    printf("Lidos do arquivo de entrada: %ld\n", totalLidos);
    printf("Gravados em %s: %ld (%.1f%%)\n", argv[2], totalGravados,
           100.0 * totalGravados / (totalLidos > 0 ? totalLidos : 1));

    return 0;
}
