#define _XOPEN_SOURCE 700
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef USE_FDS
#define USE_FDS 15
#endif

int print_entrada(const char *filepath, const struct stat *info,
                  const int typeflag, struct FTW *pathinfo)
{

    const double bytes = (double)info->st_size;

    if (bytes >= 1099511627776.0)
        printf(" %9.3f TiB", bytes / 1099511627776.0);
    else if (bytes >= 1073741824.0)
        printf(" %9.3f GiB", bytes / 1073741824.0);
    else if (bytes >= 1048576.0)
        printf(" %9.3f MiB", bytes / 1048576.0);
    else if (bytes >= 1024.0)
        printf(" %9.3f KiB", bytes / 1024.0);
    else
        printf(" %9.0f B  ", bytes);

    if (typeflag == FTW_SL)
    {
        char *target;
        size_t maxlen = 1023;
        ssize_t len;

        while (1)
        {

            target = malloc(maxlen + 1);
            if (target == NULL)
                return ENOMEM;

            len = readlink(filepath, target, maxlen);
            if (len == (ssize_t)-1)
            {
                const int saved_errno = errno;
                free(target);
                return saved_errno;
            }
            if (len >= (ssize_t)maxlen)
            {
                free(target);
                maxlen += 1024;
                continue;
            }

            target[len] = '\0';
            break;
        }

        printf(" %s -> %s\n", filepath, target);
        free(target);
    }
    else if (typeflag == FTW_SLN)
        printf(" %s (dangling symlink)\n", filepath);
    else if (typeflag == FTW_F)
        printf(" %s\n", filepath);
    else if (typeflag == FTW_D || typeflag == FTW_DP)
        printf(" %s/\n", filepath);
    else if (typeflag == FTW_DNR)
        printf(" %s/ (unreadable)\n", filepath);
    else
        printf(" %s (unknown)\n", filepath);

    return 0;
}

int print_arvore_diretorio(const char *const dirpath)
{
    int result;

    if (dirpath == NULL || *dirpath == '\0')
        return errno = EINVAL;

    result = nftw(dirpath, print_entrada, USE_FDS, FTW_PHYS);
    if (result >= 0)
        errno = result;

    return errno;
}

int main(int argc, char *argv[])
{
    int arg;

    if (argc < 2)
    {

        if (print_arvore_diretorio("."))
        {
            fprintf(stderr, "%s.\n", strerror(errno));
            return EXIT_FAILURE;
        }
    }
    else
    {

        for (arg = 1; arg < argc; arg++)
        {
            if (print_arvore_diretorio(argv[arg]))
            {
                fprintf(stderr, "%s.\n", strerror(errno));
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}