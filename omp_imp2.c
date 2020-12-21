#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>

int N;
int P;
int width, height;
int maxval;
int size;
unsigned char *img;
unsigned char *img_new;

void read_image(char *name)
{
    FILE *f = fopen(name, "r");
    char *aux = malloc(2);
    if (!aux)
        return;
    fscanf(f, "%s\n", aux);
    P = aux[1] - 48;
    aux = realloc(aux, 500);
    fgets(aux, 500, f);
    fscanf(f, "%d %d\n", &width, &height);
    fscanf(f, "%d\n", &maxval);
    free(aux);
    if (P == 5)
    {
        size = width * height;
    }
    else if (P == 6)
    {
        size = width * height * 3;
    }
    img = malloc(size);
    if (!img)
    {
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        fscanf(f, "%c", &img[i]);
    }
}

void edit_image()
{
    if (P == 5)
    {
#pragma omp parallel for
        for (int i = 1; i < height - 1; ++i)
        {
            for (int j = 1; j < width - 1; ++j)
            {
                unsigned char min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -1; b <= 1; ++b)
                    {
                        if (img[(i + a) * width + j + b] < min_val)
                            min_val = img[(i + a) * width + j + b];
                    }
                }
                img_new[i * width + j] = min_val;
            }
        }
    }
    else if (P == 6)
    {
#pragma omp parallel for
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 3; j < 3 * width - 3; j += 3)
            {
                for (int t = 0; t < 3; ++t)
                {
                    unsigned char min_val = 255;
                    for (int a = -1; a <= 1; ++a)
                    {
                        for (int b = -3; b <= 3; b += 3)
                        {
                            if (img[(i + a) * 3 * width + j + b + t] < min_val)
                                min_val = img[(i + a) * 3 * width + j + b + t];
                        }
                    }
                    img_new[i * 3 * width + j + t] = min_val;
                }
            }
        }
    }
}

void write_image(char *name)
{
    FILE *f = fopen(name, "w");
    fprintf(f, "P%d\n", P);
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "%d\n", maxval);
    for (int i = 0; i < size; ++i)
    {
        fprintf(f, "%c", img_new[i]);
    }
}

int main(int argc, char const *argv[])
{
    struct timeval start_time, end_time;
    double elapsed;
    if (argc != 4)
    {
        printf("Not enough arguments! Usage: ./openmp2 file_in_name file_out_name num_threads\n");
        return -1;
    }
    char *name_in = strdup(argv[1]);
    char *name_out = strdup(argv[2]);
    N = atoi(argv[3]);
    read_image(name_in);
    img_new = malloc(size);
    if (!img_new)
    {
        return -1;
    }
    gettimeofday(&start_time, 0);
    omp_set_num_threads(N);
    edit_image();
    gettimeofday(&end_time, 0);
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long microseconds = end_time.tv_usec - start_time.tv_usec;
    elapsed = seconds + microseconds * 1e-6;
    printf("\nOMP2 took %f seconds with %d threads\n", elapsed, N);
    write_image(name_out);
    return 0;
}
