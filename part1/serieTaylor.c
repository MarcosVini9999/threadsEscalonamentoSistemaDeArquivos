#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define N 20

double somResult = 0, subResult = 0;

// Fatorial
double factorial(int n){
    double fat;
    for(fat =1; n>1; n=n-1){
        fat = fat *n;
    }
    return fat;
}

// Termos positivos da série taylor
void* somador(void *y){ 
    int* x = (int*) y; 
 
    double term, numerator, denominator;

    for(int i = 0; i < N; i+=2){
        numerator = pow(-1, i) * pow((*x), (2*i+1));
        denominator = factorial( 2*i + 1 );
        term = numerator/denominator;

        somResult += term; 
    }
    return NULL;
}

// Termos negativos da série taylor
void* subtrator(void *y){ 
    int* x = (int*) y;  

    double term, numerator, denominator;

    for(int i = 1; i < N; i+=2){
        numerator = pow(-1, i) * pow((*x), (2*i+1));
        denominator = factorial( 2*i + 1 );
        term = numerator/denominator;

        subResult += term; 
    }
    return NULL;
}

void main(){

    int x;
    pthread_t somadora, subtratora; // thread somadora e thread subtratora

    printf("Digite o valor de x: ");
    scanf("%d", &x);
    /*Criação da thread somadora*/
    if (pthread_create(&somadora, NULL, &somador, &x) != 0){
        printf("\nErro na criadação da thread somadora\n");
    }else{
        pthread_join(somadora,NULL);
    }
    /*Criação da thread subtratora*/
    if (pthread_create(&subtratora, NULL, &subtrator, &x) != 0){
        printf("\nErro na criadação da thread subtratora\n");
    }else{
        pthread_join(subtratora,NULL);
    }

    printf("Serie Taylor: %f\n", (somResult + subResult)); 
}