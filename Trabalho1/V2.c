#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVParser.h"

#define READ_BUF_SIZE 8192

typedef struct {
    int linhas;
    double casos;
    double mortes;
} Dados;


void callback(char **cols, int ncols, void *userData)
{
    Dados *d = (Dados*)userData;


    if(ncols > 8)
    {
        
        if(strcmp(cols[1], "South America") == 0)
        {
    
            d->casos += atof(cols[5]);

            
            d->mortes += atof(cols[8]);
        }
    }

    
    d->linhas++;
}

int main(int argc, char *argv[]){
    int qt;
    unsigned int total = 0;
    Dados d = {0, 0, 0};
    char *buf = (char *)malloc(READ_BUF_SIZE);

    if(buf == NULL)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
    
    CSVParser csv;
    CSVParser_init(&csv);
    FILE *f = fopen("./owid-covid-data.csv", "rb");
    if(f == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        free(buf);
        return 1;
    }
    qt = fread(buf, 1, READ_BUF_SIZE, f);
    while(qt > 0)
    {
        total += qt;

        CSVParser_processLines( &csv, buf, qt, callback, &d );
        qt = fread(buf, 1, READ_BUF_SIZE, f);
    }
    CSVParser_processLines(
        &csv,
        "\n",
        1,
        callback,
        &d
    );

    fclose(f);

    printf("Total rows: %d\n", d.linhas);
    printf("Total cases in South America: %.2f\n", d.casos);
    printf("Total deaths in South America: %.2f\n", d.mortes);
    printf("Total bytes read: %u\n", total);

    free(buf);

    return 0;
}