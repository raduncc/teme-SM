#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

int N;
int P;
int width, height;
int maxval;
int size;
unsigned char *img;
unsigned char *img_new;

void *filter_function(void *data)
{
    int thread_id = *(int *)data;
    int start, stop, i, j, offset_start = 0, offset_end = 0;
    unsigned char min_val;

    float aux = ((float)width) / N;
    start = ceil(aux) * thread_id;
    stop = fmin(ceil(aux) * (thread_id + 1), width);
    if (P == 5)
    {
        if (N == 1)
        {
            offset_start = 1;
            offset_end = -1;
        }
        else
        {
            if (thread_id == 0)
            {
                offset_start = 1;
            }
            if (thread_id == N - 1)
            {
                offset_end = -1;
            }
        }
    }
    else if (P == 6)
    {
        if (N == 1)
        {
            offset_start = 3;
            offset_end = -1;
        }
        else
        {
            if (thread_id == 0)
            {
                offset_start = 3;
            }
            if (thread_id == N - 1)
            {
                offset_end = -1;
            }
        }
    }

    if (P == 5)
    {
        for (i = 1; i < height - 1; ++i)
        {
            for (j = start + offset_start; j < stop + offset_end; ++j)
            {
                min_val = 255;
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
        for (i = 1; i < height - 1; ++i)
        {
            for (j = 3 * start + offset_start; j < 3 * stop + offset_end; ++j)
            {
                min_val = 255;
                //red channel
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img[(i + a) * 3 * width + j + b] < min_val)
                            min_val = img[(i + a) * 3 * width + j + b];
                    }
                }
                img_new[i * 3 * width + j] = min_val;
                //green channel
                j++;
                min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img[(i + a) * 3 * width + j + b] < min_val)
                            min_val = img[(i + a) * 3 * width + j + b];
                    }
                }
                img_new[i * 3 * width + j] = min_val;
                //blue channel
                j++;
                min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img[(i + a) * 3 * width + j + b] < min_val)
                            min_val = img[(i + a) * 3 * width + j + b];
                    }
                }
                img_new[i * 3 * width + j] = min_val;
            }
        }
    }

    return NULL;
}

void read_image(char *name)
{
    FILE *f = fopen(name, "r");
    char *aux = malloc(2);
    if (!aux)
        return;
    fscanf(f, "%s\n", aux);
    P = aux[1] - 48;
    // printf("%d\n", P);
    aux = realloc(aux, 500);
    fgets(aux, 500, f);
    fscanf(f, "%d %d\n", &width, &height);
    printf("%d %d\n", width, height);
    fscanf(f, "%d\n", &maxval);
    // printf("%d\n", maxval);
    free(aux);
    if (P == 5)
    {
        size = width * height;
    }
    else if (P == 6)
    {
        size = width * height * 3;
    }
    // printf("%d\n", size);
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

void write_image(char *name)
{
    int i;
    FILE *f = fopen(name, "w");
    fprintf(f, "P%d\n", P);
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "%d\n", maxval);
    for (i = 0; i < size; ++i)
    {
        fprintf(f, "%c", img_new[i]);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        printf("Not enough arguments! Usage: ./sequential file_in_name file_out_name no_of_threads\n");
        return -1;
    }
    int i;
    N = atoi(argv[3]);
    pthread_t threads[N];
    int thread_id[N];
    char *name_in = strdup(argv[1]);
    char *name_out = strdup(argv[2]);
    read_image(name_in);

    img_new = malloc(size);
    if (!img_new)
    {
        return -1;
    }

    for (i = 0; i < N; ++i)
    {
        thread_id[i] = i;
    }

    for (i = 0; i < N; ++i)
    {
        pthread_create(&threads[i], NULL, filter_function, &thread_id[i]);
    }

    for (i = 0; i < N; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    write_image(name_out);
    return 0;
}
