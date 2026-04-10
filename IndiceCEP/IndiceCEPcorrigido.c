#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char CEP[9];
    unsigned int posicao;
} IE;

typedef struct
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;

int compara(const void *a, const void *b)
{
    IE *x = (IE *)a;
    IE *y = (IE *)b;
    return strncmp(x->CEP, y->CEP, 8);
}

int main()
{
    FILE *f = fopen("cep.dat", "rb");
    if (!f)
    {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long qtd = ftell(f) / sizeof(Endereco);
    rewind(f);

    IE *vetor = malloc(qtd * sizeof(IE));
    Endereco e;

    for (int i = 0; i < qtd; i++)
    {
        fread(&e, sizeof(Endereco), 1, f);
        strncpy(vetor[i].CEP, e.cep, 8);
        vetor[i].posicao = i;
    }

    qsort(vetor, qtd, sizeof(IE), compara);

    FILE *out = fopen("indice.dat", "wb");
    fwrite(vetor, sizeof(IE), qtd, out);

    fclose(f);
    fclose(out);
    free(vetor);

    return 0;
}