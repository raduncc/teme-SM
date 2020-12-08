#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compare_images(char *name_src, char *name_tgt)
{
    FILE *src = fopen(name_src, "r");
    FILE *tgt = fopen(name_tgt, "r");
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
        printf("P's differ! IMAGES ARE DIFFERENT!\n");
        return;
    }

    fscanf(src, "%d %d\n", &width_src, &height_src);
    fscanf(tgt, "%d %d\n", &width_tgt, &height_tgt);
    if (width_src != width_tgt || height_src != height_tgt)
    {
        printf("Heights or widths differ! IMAGES ARE DIFFERENT!\n");
        return;
    }

    fscanf(src, "%d\n", &maxval_src);
    fscanf(tgt, "%d\n", &maxval_tgt);
    if (maxval_src != maxval_tgt)
    {
        printf("Maxvals differ! IMAGES ARE DIFFERENT!\n");
        return;
    }
    free(aux);
    int size;
    if (P_src == 5)
    {
        size = width_src * height_src;
    }
    else if (P_src == 6)
    {
        size = width_src * height_src * 3;
    }
    char val_src, val_tgt;
    for (int i = 0; i < size; ++i) {
        fscanf(src, "%c", &val_src);
        fscanf(tgt, "%c", &val_tgt);
        if (val_src != val_tgt) {
            printf("Pixels differ! IMAGES ARE DIFFERENT at %d!\n", i);
            // return;
        }
    }
    printf("IMAGES ARE EQUAL! Congrats!\n");
}

int main(int argc, char const *argv[])
{
    char *name1 = strdup(argv[1]);
    char *name2 = strdup(argv[2]);
    compare_images(name1, name2);
    return 0;
}
