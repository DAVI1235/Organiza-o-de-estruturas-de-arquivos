#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2) // retorna inteiro  ponteiro void (ponteiro para qualquer tipo) e1 e e2 -
{	
	
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char**argv)
{
	FILE *f, *saidaA, *saidaB;

	Endereco *a, *b;
	long posicao, qtd, metadeA, metadeB;

	if(!(f = fopen("cep.dat","rb")))
	{
		printf("Erro ao abrir arquivo\n");
		return 1;
	}

	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd = posicao/sizeof(Endereco);
	metadeA = qtd/2;
	metadeB = qtd - metadeA;
	a = (Endereco*) malloc(metadeA*sizeof(Endereco));
	b = (Endereco*) malloc (metadeB*sizeof(Endereco));
	rewind(f);
	
//inicio de uma a partir de 0 e outra na metade
// metade dos elementos da matriz
//mesmo tamahno

// lê e ordena primeira metade
if(fread(a,sizeof(Endereco),metadeA,f) == metadeA)
	{printf("Lido Metade 1=OK\n");}
qsort(a,metadeA,sizeof(Endereco),compara);
printf("Ordenado metade 1 = OK\n");
saidaA = fopen("cep-metadeordenada1.dat","wb");
fwrite(a,sizeof(Endereco),metadeA,saidaA);
fclose(saidaA);
free(a);



//lê e ordena segunda metade
if(fread(b,sizeof(Endereco),metadeB,f) == metadeB)
	{printf("Lido Metade 2=OK\n");}
qsort(b,metadeB,sizeof(Endereco),compara);
printf("Ordenado metade 2 = OK\n");
saidaB = fopen("cep-metadeordenada2.dat","wb");
fwrite(b,sizeof(Endereco),metadeB,saidaB);
fclose(saidaB);
free(b);
fclose(f);

return 0;
}





