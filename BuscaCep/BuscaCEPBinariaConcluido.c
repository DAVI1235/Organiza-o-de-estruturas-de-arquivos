#include <stdio.h>
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
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};
















int main(int argc, char**argv)
{ printf("programa funcionando\n");
	FILE *f;
	Endereco e;
	int qt;
	int c=0;

	if(argc != 2)
	{	printf("Numero de argumentos invalido\n");
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}
	else
	{
		printf("CEP a ser buscado: %s\n", argv[1]);
	}

	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	
	if(f = fopen("cep_ordenado.dat","rb"))
	{
		printf("Arquivo aberto com sucesso\n");
	}
	else
	{
		printf("Erro ao abrir arquivo\n");
		return 1;
	}
	
	//qt = fread(&e,sizeof(Endereco),1,f);// ele vai empurrar a cabeça de leitura em 300 bytes ??
	fseek(f,0,SEEK_END);// joga pro fim
	long fim = (ftell(f)/sizeof(Endereco))-1;// divide pelo tamanho da estrutura para saber quantos registros tem no arquivo
	printf("Tamanho do arquivo: %ld\n", fim);
	 fseek(f,0,SEEK_SET);// joga pro inicio
	long inicio = ftell(f)/sizeof(Endereco);
	//busca binaria
if(inicio < fim)
{	printf("Inicio: %ld\nFim: %ld\n", inicio, fim);
	
}


	while (inicio <= fim)
	{	long meio = (inicio + fim)/2;
		printf("meio: %ld\n", meio);
	c++;	
	fseek(f, meio*sizeof(Endereco),SEEK_SET);
	fread(&e,sizeof(Endereco),1,f);
	printf("rodada %d\n", c);


	if(strncmp(argv[1],e.cep,8)==0){
		printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
		break;
	}
	else{
		if(strncmp(argv[1],e.cep,8)>0)
		{printf("CEP buscado e maior que o CEP lido\n");
			printf("%.8s\n oi teste1",e.cep);
			inicio = meio + 1;
		}
		else{printf("CEP buscado e menor que o CEP lido\n");
			fim = meio - 1;
		}

		}
	
}

	fclose(f);



