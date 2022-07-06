#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

int numProcessGlobal;

/*Estrutura de um processo */
struct process
{
    int id;    /* ID do processo */
    int time;  /* tempo de execucao*/
    int order; /* ordem do processo */
    struct process *next;
};

typedef struct process Process;

/*FUNÇÃO QUE INICIALIZA O PROCESSO*/
Process *newProcess(void)
{
    return NULL;
}
/*FUNÇÃO QUE LIBERA O ESPAÇO ALOCADO PELO PROCESSO*/
void freeProcess(Process *process)
{
    Process *processAux = process;
    while (processAux != 0)
    {
        Process *temp = processAux->next;
        free(processAux);
        processAux = temp;
    }
}
/*FUNÇÃO QUE INSERE O PROCESSO NA LISTA*/
Process *insertProcess(Process *process, int id, int time, int order)
{

    Process *processPrev = NULL;
    Process *processAux = process;

    while (processAux != NULL && processAux->order < order)
    {

        processPrev = processAux;
        processAux = processAux->next;
    }

    /*Aloca um tamanho de espaço de memória para um novo processo*/
    Process *processNew = (Process *)malloc(sizeof(Process));
    if (processNew == NULL)
    {
        printf("ERRO!!\n");
        exit(1);
    }

    processNew->id = id;
    processNew->time = time;
    processNew->order = order;

    if (processPrev == NULL)
    {
        processNew->next = process;
        process = processNew;
    }
    else
    {
        processNew->next = processPrev->next;
        processPrev->next = processNew;
    }

    return process;
}
/*INSERE O PROCESSO EM ORDEM*/
Process *insertShortestProcessFirst(Process *process, int id, int time, int order)
{

    Process *processPrev = NULL;
    Process *processAux = process;

    while (processAux != NULL && processAux->time < time)
    {

        processPrev = processAux;
        processAux = processAux->next;
    }

    /*Aloca um tamanho de espaço de memória para um novo processo*/
    Process *processNew = (Process *)malloc(sizeof(Process));
    if (processNew == NULL)
    {
        printf("Erro na alocacao de memória.\n");
        exit(1);
    }

    processNew->id = id;
    processNew->time = time;
    processNew->order = order;

    if (processPrev == NULL)
    {
        processNew->next = process;
        process = processNew;
    }
    else
    {
        processNew->next = processPrev->next;
        processPrev->next = processNew;
    }

    return process;
}
/*FUNÇÃO QUE TORNA A LISTA CIRCULAR*/
Process *turnRound(Process *process)
{

    Process *processAux = process;
    Process *processAux2 = process;

    while (processAux2->next != NULL)
        processAux2 = processAux2->next;

    processAux2->next = processAux;

    return processAux;
}
/*FUNÇÃO QUE FAZ O ESCALONAMENTO FIFO*/
void FIFO(Process *process)
{

    Process *processAux = process;

    printf("\n===FIRST_IN_FIRST_OUT(FIFO)===\n");
    while (processAux != NULL)
    {
        printf("Processo %d executando [%d ut]\n", processAux->id, processAux->time);
        sleep(processAux->time);
        processAux->time = 0;

        processAux = processAux->next;
    }
}
/*FUNÇÃO QUE FAZ O ESCALONAMENTO SJF*/
void SJF(Process *process)
{

    Process *processAux = process;     /*CRIA UM PROCESSO AUXILIAR A PARTIR DO PARAMETRO RECEBIDO*/
    Process *shortests = newProcess(); /*CRIA UM PROCESSO VAZIO*/

    while (processAux != NULL)
    {

        shortests = insertShortestProcessFirst(
            shortests,
            processAux->id,
            processAux->time,
            processAux->order);

        processAux = processAux->next;
    }

    printf("===SHORTEST_JOB_FIRST(SJF)===\n");

    for (Process *temp = shortests; temp != NULL; temp = temp->next)
    {

        printf("Processo %d executando... [%d ut]\n", temp->id, temp->time);
        sleep(temp->time);
    }
    freeProcess(shortests);
    freeProcess(process);
}
/*FUNÇÃO QUE FAZ O ESCALONAMENTO RR*/
void RR(Process *process, int quantum)
{
    int cont = 0;

    Process *processAux = turnRound(process);

    printf("\n===ROUND_ROBIN(RR)===\n");
    while (processAux != NULL)
    {

        if ((processAux->time != 0) && (processAux->time <= quantum))
        {
            cont = 0;

            printf("Processo %d executando [%d ut] final\n", processAux->id, processAux->time);
            sleep(processAux->time);
            processAux->time = 0;
        }
        else if (processAux->time > quantum)
        {
            cont = 0;

            printf("Processo %d executando %dut. Tamanho: [%d ut]\n", processAux->id, quantum, processAux->time);
            sleep(quantum);
            processAux->time -= quantum;
        }
        else
        {

            for (Process *i = process; process->time == 0; process = process->next)
            {
                cont++;
                if (cont >= numProcessGlobal)
                    exit(1);
            }
        }

        processAux = processAux->next;
    }
}
/*FUNÇÃO QUE IMPRIME OS PROCESSOS*/
void printProcess(Process *process)
{
    printf("Processos digitados:\n");
    for (Process *temp = process; temp != NULL; temp = temp->next)
    {
        printf("=============================\n");
        printf("ID:%d", temp->id);
        printf("\n");
        printf("TEMPO DE EXECUAO:%d", temp->time);
        printf("\n");
        printf("ORDEM:%d", temp->order);
        printf("\n");
        printf("=============================");
        printf("\n");
    }
}
/*FUNÇÃO QUE ESCOLHE QUAL ESCALONAMENTO EXECUTAR*/
int switchOption(int option, Process *process)
{
    int quantum;
    switch (option)
    {
    case 1:
        FIFO(process);
        break;

    case 2:
        SJF(process);
        break;

    case 3:
        printf("Digite o quantum: ");
        scanf("%d", &quantum);
        RR(process, quantum);
        break;
    case 4:
        printf("APLICAÇÃO CANCELADA");
        break;

    default:
        printf("ERRO, OPÇÃO NÃO ENCONTRADA, DIGITE NOVAMENTE!!!!!!\n");
        printf("FIFO.................1\n");
        printf("SJF..................2\n");
        printf("RR...................3\n");
        printf("Cancelar_aplicação...4\n");
        printf("Digite o número da opção desejada: ");
        scanf("%d", &option);
        break;
    }
    return option;
}
/*FUNÇÃO PRINCIPAL*/
void main()
{
    setlocale(LC_ALL, "Portuguese");
    Process *process = newProcess(); // cria um novo processo
    int option;
    int numProcess, option2, time, order;
    printf("Quantidade de processos na fila: ");
    scanf("%d", &numProcess); // armazena a quantidade de procesos na fila
    printf("\n");
    numProcessGlobal = numProcess;
    for (int id = 1; id <= numProcess; id++)
    {
        printf("Ordem de chegada do processo %d: ", id);
        scanf("%d", &order);
        printf("Digite o tempo de execucao do processo %d: ", id);
        scanf("%d", &time);
        process = insertProcess(process, id, time, order);
        printf("\n");
    }
    printProcess(process); // imprime os processos informados pelo usuário
    printf("\nFIFO.................1\n");
    printf("SJF..................2\n");
    printf("RR...................3\n");
    printf("Cancelar_aplicação...4\n");
    printf("Digite o número da opção desejada: ");
    scanf("%d", &option);
    if (option == 4)
    {
        return;
    }
    else
    {
        option = switchOption(option, process);
    }
    /*laço de repetição para o fluxo do programa*/
    option2 = 1;
    while (option2 == 1)
    {
        printf("\nSIM...1\n");
        printf("NÃO...0\n");
        printf("Deseja repetir a aplicação ?(1/0)\n");
        scanf("%d", &option2);
        /*Repetição do código da função main*/
        if (option2 == 1)
        {
            printf("Quantidade de processos na fila: ");
            scanf("%d", &numProcess); // armazena a quantidade de procesos na fila
            printf("\n");
            numProcessGlobal = numProcess;
            for (int id = 1; id <= numProcess; id++)
            {
                printf("Ordem de chegada do processo %d: ", id);
                scanf("%d", &order);
                printf("Digite o tempo de execucao do processo %d: ", id);
                scanf("%d", &time);
                process = insertProcess(process, id, time, order);
                printf("\n");
            }
            printProcess(process);
            printf("\nFIFO.................1\n");
            printf("SJF..................2\n");
            printf("RR...................3\n");
            printf("Cancelar_aplicação...4\n");
            printf("Digite o número da opção desejada: ");
            scanf("%d", &option);
            if (option == 4)
            {
                return;
            }
            else
            {
                option = switchOption(option, process);
            }
        }
        else
        {
            option2 = 0;
        }
    }
    freeProcess(process); // limpa os processos da memória
    return;
}