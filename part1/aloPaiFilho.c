#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define N 100

pthread_t filho[N];

void *threadFilha(void *num)
{
    int i = *((int *)num);
    printf("Alo Filho[%d]\n", i);
}

void *threadPai()
{
    for (int i = 0; i < N; i++)
    {
        int *num = malloc(sizeof(int));
        *num = i;
        if (pthread_create(&(filho[i]), NULL, threadFilha, num) != 0)
        {
            printf("\nErro na criação da thread filha[%d]\n", i);
        }
        else
        {
            pthread_join(filho[i], NULL);
        }
        printf("Tchau filho[%d]\n", i);
    }
    printf("Tchau Pai\n");
}
void main()
{
    pthread_t pai;
    if (pthread_create(&pai, NULL, threadPai, NULL) != 0)
    {
        printf("\nErro na criadação da thread PAI\n");
    }
    else
    {
        printf("Alo pai\n");
        pthread_join(pai, NULL);
    }
}
