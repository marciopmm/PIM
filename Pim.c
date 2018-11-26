#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

int opc;
int posicao = 0;
int contadorPC = 1;
int contadorpne = 1;
int contadorParts = 1;
int quant_assento;
int posicaoInicialPalestrantesConvidados;
// int posicaoInicialpne;
int posicaoInicialParticipantes;
int maxPalestrantesConvidados;
int maxPne;
int maxParticipantes;

typedef struct{
	char nome[60];
	int categoria;
	char cpf[11];
	int pne;
	int convidadoPalestrante;
} participante;

typedef struct{
	participante part;
	int n_Assento;
	time_t hora;
} reserva;

participante cadastrarParticipante();
void reservaAssento(reserva listaReservas[]);
void contadoresAssento(int quant_assento);
int verificarPosicao(reserva listaReservas[], int posicaoDesejada);
//void ticketHora();
//void sortearBrinde();
void limpaBuffer();
void inicializarLista(reserva lista[]);

int main(void)
{
	quant_assento = 100;

	// printf("Digite a quantidade de assentos do auditorio: ");
	// scanf("%i", &quant_assento);
	reserva listaReservas[quant_assento];
	contadoresAssento(quant_assento);
	inicializarLista(listaReservas);

	setlocale(LC_ALL,"portuguese");

	do
	{
		//clrscr();
		system("cls");
		printf("=== SISTEMA DE RESERVA PARA EVENTOS ===\n\n");
		printf("1) Cadastrar o participante e reservar o assento\n2) Sorteio dos brindes\n0) Sair\nDigite a opcao desejada: ");
		scanf("%i", &opc);

		switch(opc)
		{
			case 1: reservaAssento(listaReservas); break;
			//case 2: sorteioBrinde(); break;
		}
	}
	while(opc != 0);

	return (0);
}

participante cadastrarParticipante()
{
	participante part;

	limpaBuffer();

	printf("\nDigite o nome do participante: ");
	gets(part.nome);

	printf("Digite a categoria do participante: \n1) Convidado\n2) Professor\n3) Estudante\n");
	scanf("%i", &part.categoria);

	printf("Digite o CPF do participante: ");
	scanf("%s", &part.cpf);

	char pne[1];
	printf("\nO participante possui alguma necessidade especial? (S/N): ");
	scanf("%s", &pne);
	pne[0] = toupper(pne[0]);

	char convPalestrante[1];
	printf("\nO participante eh convidado de algum palestrante? (S/N): ");
	scanf("%s", &convPalestrante);
	convPalestrante[0] = toupper(convPalestrante[0]);

	system("cls");
	printf("\nVerifique as informacoes inseridas e confira se os dados estao corretos.\n");
	printf("\n Nome: %s\n Cpf: %s\n Portador de Necessidades Especiais: %s\n Convidado de Palestrante: %s\n", 
			part.nome, 
			part.cpf, 
			pne[0] == 'S' ? "SIM" : "NAO", 
			convPalestrante[0] == 'S' ? "SIM" : "NAO");

	if(part.categoria == 1)
	{
		printf(" O participante eh um Convidado!\n");
	}
	else if(part.categoria == 2)
	{
		printf(" O participante eh um Professor!\n");
	}
	else
	{
		printf(" O participante eh um Estudante!\n");
	}

	if(*pne == 'S')
	{
		printf(" O participante possui deficiencia\n");
		part.pne = 1;
	}
	else
	{
		part.pne = 0;
	}

	if(*convPalestrante == 'S')
	{
		printf(" O participante eh convidado de um palestrante\n");
		part.convidadoPalestrante = 1;
	}
	else
	{
		part.convidadoPalestrante = 0;
	}

	system("pause");

	return part;
}

void reservaAssento(reserva listaReservas[])
{
	participante part = cadastrarParticipante();

	int n_assento;

	// Se o participante for PNE
	if(part.pne == 1)
	{
		// Solicitar a posição desejada
		int posicaoDesejada = 0;
		while (posicaoDesejada == 0)
		{
			printf("Selecione a posição desejada (entre %d e %d): ", posicaoInicialPalestrantesConvidados, quant_assento);
			scanf("%d", &posicaoDesejada);

			// Verificar se a posição está ocupada
			posicaoDesejada = verificarPosicao(listaReservas, posicaoDesejada);
		}

		// Caso livre, reservar.
		n_assento = posicaoDesejada;

	} // Se o participante for PALESTRANTE ou CONVIDADO do PALESTRANTE
	else if(part.convidadoPalestrante == 1)
	{
		if(contadorPC <= maxPalestrantesConvidados)
		{
			// Verificar se a posição está ocupada
			int posicaoDesejada;
			do
			{
				posicaoDesejada = contadorPC;
				posicaoDesejada = verificarPosicao(listaReservas, posicaoDesejada);

				if (posicaoDesejada > 0)
				{
					n_assento = posicaoDesejada;
				}
				contadorPC++;
			} while (posicaoDesejada == 0);
		} 
		else if (contadorParts <= quant_assento)
		{
			// Verificar se a posição está ocupada
			int posicaoDesejada;
			do
			{
				posicaoDesejada = contadorParts;
				posicaoDesejada = verificarPosicao(listaReservas, posicaoDesejada);

				if (posicaoDesejada > 0)
				{
					n_assento = posicaoDesejada;
				}
				contadorParts++;
			} while (posicaoDesejada == 0);			
		}
		else 
		{
			printf("O EVENTO ESTÁ LOTADO.\nNão eh possivel realizar mais reservas.");
			return;
		}
	} // Se o participante for apenas CONVIDADO
	else
	{
		// Verificar se a posição está ocupada
			int posicaoDesejada;
			do
			{
				posicaoDesejada = contadorParts;
				posicaoDesejada = verificarPosicao(listaReservas, posicaoDesejada);

				if (posicaoDesejada > 0)
				{
					n_assento = contadorParts;
				}
				contadorParts++;
			} while (posicaoDesejada == 0);	
	}

	listaReservas[posicao].n_Assento = n_assento;
	listaReservas[posicao].part = part;
	listaReservas[posicao].hora = time(NULL);
	
	system("cls");
	printf("TICKET DE RESERVA CRIADO: \n");
	printf("Data/Hora: %s", ctime(&listaReservas[posicao].hora));
	printf("Assento no.: %d \n", n_assento);

	posicao++;

	system("pause");

}

void contadoresAssento(int quant_assento)
{
	maxPalestrantesConvidados = quant_assento * 0.2;
	maxPne = quant_assento * 0.05;
	maxParticipantes = quant_assento - maxPalestrantesConvidados - maxPne;

	posicaoInicialPalestrantesConvidados = 1;
	contadorPC = 1;
	// posicaoInicialPne = maxPalestrantesConvidados + 1;
	posicaoInicialParticipantes = maxPalestrantesConvidados + 1;
	contadorParts = posicaoInicialParticipantes;
}

int verificarPosicao(reserva listaReservas[], int posicaoDesejada)
{
	for (int i = 0; i < quant_assento; i++)
	{
		reserva r = listaReservas[i];
		if (r.n_Assento == posicaoDesejada)
		{
			printf("A posicao %d ja esta ocupada! Por favor, selecione outra.\n");
			system("pause");
			posicaoDesejada = 0;
			break;
		}
	}

	return (posicaoDesejada);
}

void inicializarLista(reserva lista[]) 
{
	for (int i = 0; i < quant_assento; i++)
	{
		lista[i].n_Assento = 0;
	}
}

void limpaBuffer(){
	char ch;
	while ((ch=getchar())!='\n' && ch!=EOF);
}
