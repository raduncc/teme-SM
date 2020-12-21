#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compare_images(char *name_src, char *name_tgt)
{
    int flag = 0;
    FILE *src = fopen(name_src, "r");
    FILE *tgt = fopen(name_tgt, "r");
    FILE *diff = fopen("difference", "w");

    int P_src, P_tgt;
    int maxval_src, maxval_tgt;
    int width_src, height_src, width_tgt, height_tgt;
    char *aux = malloc(2);

    if (!aux)
        return;

    fscanf(src, "%s\n", aux);
    P_src = aux[1] - 48;
    fscanf(tgt, "%s\n", aux);
    P_tgt = aux[1] - 48;

    if (P_src != P_tgt)
    {
        flag = 1;
    }

    fscanf(src, "%d %d\n", &width_src, &height_src);
    fscanf(tgt, "%d %d\n", &width_tgt, &height_tgt);
    if (width_src != width_tgt || height_src != height_tgt)
    {
        flag = 1;
    }

    fscanf(src, "%d\n", &maxval_src);
    fscanf(tgt, "%d\n", &maxval_tgt);
    if (maxval_src != maxval_tgt)
    {
        flag = 1;
    }
    free(aux);
    char val_src, val_tgt;
    fprintf(diff, "P%d\n", P_src);
    fprintf(diff, "%d %d\n", width_src, height_src);
    fprintf(diff, "%d\n", maxval_src);
    
    for (int i = 0; i < height_src; ++i)
    {
        for (int j = 0; j < width_src * 3; ++j)
        {
            fscanf(src, "%c", &val_src);
            fscanf(tgt, "%c", &val_tgt);
            if (val_src != val_tgt)
            {
                printf("Pixels differ! IMAGES ARE DIFFERENT at %d %d! %d %d\n", i, j, val_src, val_tgt);
                fprintf(diff, "%c", 100);
                flag = 1;
            }
            else
            {
                fprintf(diff, "%c", val_src);
            }
        }
    }
    if (flag == 0)
    {
        printf("IMAGES ARE EQUAL! Congrats!\n");
    }
    else
    {
        printf("Images differ!\n");
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Not enough arguments! Usage: ./compare file1 file2\n");
        return -1;
    }
    char *name1 = strdup(argv[1]);
    char *name2 = strdup(argv[2]);
    compare_images(name1, name2);
    return 0;
}
