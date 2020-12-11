#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

int N;
int P;
int width, height;
int maxval;
int size;
unsigned char *img;
unsigned char *img_new, *img_new_aux, *img_result;

void edit_image()
{
    img_new = malloc(size);
    if (!img_new)
    {
        return;
    }
    if (P == 5)
    {
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
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 3; j < 3 * width - 1; j++)
            {
                unsigned char min_val = 255;
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
}

void write_to_final(unsigned char *image, int wdth, int hgt, int id)
{
    int i, j;
    for (i = 0; i < hgt; ++i)
    {
        for (j = 0; j < wdth; ++j)
        {
            if (P == 5)
            {
                img_new[i * width + id * wdth + j] = image[i * wdth + j];
            }
            else if (P == 6)
            {
                img_new[i * 3 * width + id * wdth + j] = image[i * wdth + j];
            }
        }
    }
}

void edit_remaining()
{
    int i, j;
    if (P == 5)
    {
        for (i = 1; i < height; ++i)
        {
            for (j = 1; j < width / N; ++j)
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
        for (i = 1; i < height - 1; ++i)
        {
            for (j = width / N; j < width; j += width / N)
            {
                unsigned char min_val;
                j--;
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
                j++;
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
                j++;
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
                j--;
            }
        }
    }
    else if (P == 6)
    {
        for (i = 1; i < height; ++i)
        {
            for (j = 3; j < 3 * width / N; j++)
            {
                unsigned char min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img[(i + a) * 3 * width + j + b] < min_val)
                            min_val = img[(i + a) * 3 * width + j + b];
                    }
                }
                img_new[i * 3 * width + j] = min_val;
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
        for (i = 1; i < height - 1; ++i)
        {
            for (j = 3 * width / N; j <= 3 * width; j += 3 * width / N)
            {
                unsigned char min_val;
                if (j == 3 * width / N)
                {
                    for (int t = 0; t < 6; t++)
                    {
                        min_val = 255;
                        for (int a = -1; a <= 1; ++a)
                        {
                            for (int b = -3; b <= 3; b += 3)
                            {
                                if (img[(i + a) * 3 * width + j + t + b] < min_val)
                                    min_val = img[(i + a) * 3 * width + j + t + b];
                            }
                        }
                        img_new[i * 3 * width + j + t] = min_val;
                    }
                    continue;
                }
                for (int t = -3; t < 3; t++)
                {
                    min_val = 255;
                    for (int a = -1; a <= 1; ++a)
                    {
                        for (int b = -3; b <= 3; b += 3)
                        {
                            if (img[(i + a) * 3 * width + j + t + b] < min_val)
                                min_val = img[(i + a) * 3 * width + j + t + b];
                        }
                    }
                    img_new[i * 3 * width + j + t] = min_val;
                }
            }
        }
        for (i = 0; i < height; ++i)
        {
            for (j = 0; j < 3; ++j)
            {
                img_new[i * 3 * width + j] = 0;
            }
        }
    }
}

void filter_function(int hgt, int wdth)
{
    int i, j;
    if (P == 5)
    {
        for (i = 1; i < hgt; ++i)
        {
            for (j = 1; j < wdth; ++j)
            {
                unsigned char min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -1; b <= 1; ++b)
                    {
                        if (img_new_aux[(i + a) * wdth + j + b] < min_val)
                            min_val = img_new_aux[(i + a) * wdth + j + b];
                    }
                }
                img_result[i * wdth + j] = min_val;
            }
        }
    }
    else if (P == 6)
    {
        for (i = 1; i < hgt; ++i)
        {
            for (j = 3; j < wdth; ++j)
            {
                unsigned char min_val = 255;
                //red channel
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img_new_aux[(i + a) * wdth + j + b] < min_val)
                            min_val = img_new_aux[(i + a) * wdth + j + b];
                    }
                }
                img_result[i * wdth + j] = min_val;
                // green channel
                j++;
                min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img_new_aux[(i + a) * wdth + j + b] < min_val)
                            min_val = img_new_aux[(i + a) * wdth + j + b];
                    }
                }
                img_result[i * wdth + j] = min_val;
                //blue channel
                j++;
                min_val = 255;
                for (int a = -1; a <= 1; ++a)
                {
                    for (int b = -3; b <= 3; b += 3)
                    {
                        if (img_new_aux[(i + a) * wdth + j + b] < min_val)
                            min_val = img_new_aux[(i + a) * wdth + j + b];
                    }
                }
                img_result[i * wdth + j] = min_val;
            }
        }
    }
}

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

int main(int argc, char *argv[])
{
    MPI_Status Stat;
    int vec_size;
    int i, j;
    int rank;
    char *name_in, *name_out;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &N);
    if (argc != 3)
    {
        printf("Not enough arguments! Usage: mpirun -np no_of_processes ./mpi file_in_name  file_out_name\n");
        return -1;
    }
    if (N == 1)
    {
        name_in = strdup(argv[1]);
        name_out = strdup(argv[2]);
        read_image(name_in);
        edit_image();
        write_image(name_out);
        return 0;
    }
    if (rank == 0)
    {
        name_in = strdup(argv[1]);
        name_out = strdup(argv[2]);
        read_image(name_in);
        vec_size = width / N;
        img_new_aux = malloc(vec_size);
        if (!img_new_aux)
        {
            printf("Error trying to allocate img_new_aux by rank %d\n", rank);
            return -1;
        }
        for (i = 1; i < N; ++i)
        {
            MPI_Send(&vec_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&P, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        unsigned char *vec;
        if (P == 5)
        {
            vec = malloc(height * vec_size);
        }
        else if (P == 6)
        {
            vec = malloc(height * vec_size * 3);
        }
        for (j = 0; j < N; ++j)
        {
            for (i = 0; i < height; ++i)
            {
                if (P == 5)
                {
                    memcpy(vec + i * vec_size, img + i * width + j * vec_size, vec_size);
                }
                else if (P == 6)
                {
                    memcpy(vec + i * 3 * vec_size, img + i * 3 * width + j * 3 * vec_size, vec_size * 3);
                }
            }
            if (j != 0)
            {
                if (P == 5)
                {
                    MPI_Send(vec, height * vec_size, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);
                }
                else if (P == 6)
                {
                    MPI_Send(vec, height * vec_size * 3, MPI_UNSIGNED_CHAR, j, 0, MPI_COMM_WORLD);
                }
            }
        }
    }
    else
    {
        int my_size;
        MPI_Recv(&my_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &Stat);
        MPI_Recv(&P, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &Stat);
        MPI_Recv(&height, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &Stat);
        if (P == 5)
        {
            img_new_aux = malloc(my_size * height);
            img_result = malloc(my_size * height);
        }
        else if (P == 6)
        {
            img_new_aux = malloc(my_size * height * 3);
            img_result = malloc(my_size * height * 3);
        }
        if (!img_new_aux || !img_result)
        {
            printf("Error trying to allocate img_new_aux by rank %d\n", rank);
            return -1;
        }
        if (P == 5)
        {
            MPI_Recv(img_new_aux, my_size * height, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &Stat);
        }
        else if (P == 6)
        {
            MPI_Recv(img_new_aux, 3 * my_size * height, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &Stat);
        }
        if (P == 5)
        {
            filter_function(height, my_size);
            MPI_Send(img_result, my_size * height, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        }
        else if (P == 6)
        {
            filter_function(height, 3 * my_size);
            MPI_Send(img_result, 3 * my_size * height, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0)
    {

        img_new = malloc(size);
        if (!img_new)
        {
            printf("Error trying to allocate img_new by rank %d\n", rank);
            return -1;
        }
        unsigned char *final_image;
        if (P == 5)
        {
            final_image = malloc(vec_size * height);
        }
        else if (P == 6)
        {
            final_image = malloc(3 * vec_size * height);
        }
        for (i = 1; i < N; ++i)
        {
            if (P == 5)
            {
                MPI_Recv(final_image, vec_size * height, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &Stat);
                write_to_final(final_image, vec_size, height, i);
            }
            else if (P == 6)
            {
                MPI_Recv(final_image, 3 * vec_size * height, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &Stat);
                write_to_final(final_image, 3 * vec_size, height, i);
            }
        }
        edit_remaining();
        write_image(name_out);
    }
    MPI_Finalize();
    return 0;
}
