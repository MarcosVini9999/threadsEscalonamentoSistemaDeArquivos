#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

int nPrimos = 0;

// Função que diz se é primo
int ifPrimo(int n)
{
	int counter = 0;

	for (int i = 1; i <= n; i++)
	{
		if (n % i == 0)
		{
			counter++;
		}
	}

	if (counter == 2)
	{
		printf("[%d] ", n);
		return 1;
	}
	else
	{
		return 0;
	}
}

// Função da thread 1 que conta os primos de 0 até N/2
void *counter1(void *num)
{

	int *n = (int *)num;
	int counterPrimo;
	printf("\n======NUMEROS_PRIMOS======\n");
	for (int i = 2; i <= (*n / 2); i++)
	{
		if (ifPrimo(i) != 0)
		{
			counterPrimo++;
		}
	}
	nPrimos = nPrimos + counterPrimo;
}

// Função da thread 2 que conta os primos de (N/2)+1 até N
void *counter2(void *num)
{

	int *n = (int *)num;
	int counterPrimo;
	printf("\n");
	for (int i = (*n / 2) + 1; i <= *n; i++)
	{
		if (ifPrimo(i) != 0)
		{
			counterPrimo++;
		}
	}
	printf("\n==========================\n");
	nPrimos = nPrimos + counterPrimo;
}

void main()
{

	int n;
	pthread_t thread1, thread2;
	printf("Digite o numero N:");
	scanf("%d", &n);

	/*Criando as threads*/
	if (pthread_create(&thread1, NULL, counter1, &n) != 0)
	{
		printf("\nErro na criadação da thread 1\n");
	}
	else
	{
		pthread_join(thread1, NULL);
	}
	/*Criação da thread subtratora*/
	if (pthread_create(&thread2, NULL, counter2, &n) != 0)
	{
		printf("\nErro na criadação da thread 2\n");
	}
	else
	{
		pthread_join(thread2, NULL);
	}

	printf("\nQuantidade total de numeros primos = %d\n", nPrimos);
	return;
}