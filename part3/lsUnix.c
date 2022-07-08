#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

void buscarDiretorio(const char *path)
{

    DIR *d = opendir(path);

    struct dirent *dir;

    char nomeDoArquivo[256];

    if (d == NULL)
    {
        printf("ERRO!!! Nao foi possivel abrir o(s) diretorio(s) escolhido(s) %s\n", path);
        return;
    }
    printf("Conteudo do Diretorio %s\n", path);

    while ((dir = readdir(d)) != NULL)
    {
        printf(">> %s\n", dir->d_name);
        if (nomeDoArquivo[0] == '.')
            continue;
        printf("%s\n", nomeDoArquivo);
    }
    closedir(d);
}

int main(int argc, char *argv[])
{

    int i;
    for (i = 1; i < argc; i++)
    {
        buscarDiretorio(argv[i]);
    }
    return 0;
}
