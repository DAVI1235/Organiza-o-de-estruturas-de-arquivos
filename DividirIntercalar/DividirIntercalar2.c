#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
} Endereco;


int compara(const void *a, const void *b) {
    return strcmp(((Endereco*)a)->cep, ((Endereco*)b)->cep);
}


void merge(int a, int b, int out) {
    char nome1[50], nome2[50], nome_out[50];

    sprintf(nome1, "bloco%d.dat", a);
    sprintf(nome2, "bloco%d.dat", b);
    sprintf(nome_out, "bloco%d.dat", out);

    FILE *f1 = fopen(nome1, "rb");
    FILE *f2 = fopen(nome2, "rb");
    FILE *fo = fopen(nome_out, "wb");

    Endereco e1, e2;
    int tem1 = fread(&e1, sizeof(Endereco), 1, f1);
    int tem2 = fread(&e2, sizeof(Endereco), 1, f2);

    while (tem1 && tem2) {
        if (strcmp(e1.cep, e2.cep) <= 0) {
            fwrite(&e1, sizeof(Endereco), 1, fo);
            tem1 = fread(&e1, sizeof(Endereco), 1, f1);
        } else {
            fwrite(&e2, sizeof(Endereco), 1, fo);
            tem2 = fread(&e2, sizeof(Endereco), 1, f2);
        }
    }

    while (tem1) {
        fwrite(&e1, sizeof(Endereco), 1, fo);
        tem1 = fread(&e1, sizeof(Endereco), 1, f1);
    }

    while (tem2) {
        fwrite(&e2, sizeof(Endereco), 1, fo);
        tem2 = fread(&e2, sizeof(Endereco), 1, f2);
    }

    fclose(f1);
    fclose(f2);
    fclose(fo);
}

int main() {
    FILE *f = fopen("cep.dat", "rb");
    if (!f) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    int k;
    printf("Numero de blocos: ");
    scanf("%d", &k);

    
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);

    long total = tam / sizeof(Endereco);
    long por_bloco = total / k;

    Endereco *buffer = malloc(sizeof(Endereco) * por_bloco);


    for (int i = 0; i < k; i++) {
        fread(buffer, sizeof(Endereco), por_bloco, f);

        qsort(buffer, por_bloco, sizeof(Endereco), compara);

        char nome[50];
        sprintf(nome, "bloco%d.dat", i);

        FILE *out = fopen(nome, "wb");
        fwrite(buffer, sizeof(Endereco), por_bloco, out);
        fclose(out);
    }

    fclose(f);
    free(buffer);

   
    int inicio = 0;
    int fim = k;
    int atual = k; 

    while ((fim - inicio) > 1) {
        int novo_inicio = atual;

        for (int i = inicio; i < fim; i += 2) {
            merge(i, i + 1, atual);
            atual++;
        }

        inicio = novo_inicio;
        fim = atual;
    }

    printf("Arquivo final ordenado: bloco%d.dat\n", atual - 1);

    return 0;
}