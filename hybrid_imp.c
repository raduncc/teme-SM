#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define min(a, b) ((a) + ((b) < (a) ? (b) - (a) : 0))

void read_file(char *file_name, int *width, int *height, int *maxval, int *type,
			   unsigned char ***red_buffer, unsigned char ***green_buffer, unsigned char ***blue_buffer)
{
	*width = 0;
	*height = 0;
	*maxval = 0;
	int i, j;
	unsigned char c;
	FILE *input_file = fopen(file_name, "rb");
	fseek(input_file, 1, SEEK_CUR);
	fread(&c, sizeof(char), 1, input_file);
	if (c == '5')
		*type = 0;
	else
		*type = 1;
	fseek(input_file, 1, SEEK_CUR);
	fread(&c, sizeof(char), 1, input_file);
	while (c != '\n')
	{
		fread(&c, sizeof(char), 1, input_file);
	}
	fread(&c, sizeof(char), 1, input_file);
	while (c != ' ')
	{
		*width = *width * 10 + (c - '0');
		fread(&c, sizeof(char), 1, input_file);
	}
	fread(&c, sizeof(char), 1, input_file);
	while (c != '\n')
	{
		*height = *height * 10 + (c - '0');
		fread(&c, sizeof(char), 1, input_file);
	}
	fread(&c, sizeof(char), 1, input_file);
	while (c != '\n')
	{
		*maxval = *maxval * 10 + (c - '0');
		fread(&c, sizeof(char), 1, input_file);
	}
	*red_buffer = malloc((*height + 2) * sizeof(char *));
	*green_buffer = malloc((*height + 2) * sizeof(char *));
	*blue_buffer = malloc((*height + 2) * sizeof(char *));
	for (i = 0; i < *height + 2; i++)
	{
		(*red_buffer)[i] = malloc((*width + 2) * sizeof(char));
		(*green_buffer)[i] = malloc((*width + 2) * sizeof(char));
		(*blue_buffer)[i] = malloc((*width + 2) * sizeof(char));
	}
	for (i = 0; i < *width + 2; i++)
	{
		(*red_buffer)[0][i] = 0;
		(*green_buffer)[0][i] = 0;
		(*blue_buffer)[0][i] = 0;
		(*red_buffer)[*height + 1][i] = 0;
		(*green_buffer)[*height + 1][i] = 0;
		(*blue_buffer)[*height + 1][i] = 0;
	}
	for (i = 0; i < *height + 2; i++)
	{
		(*red_buffer)[i][0] = 0;
		(*green_buffer)[i][0] = 0;
		(*blue_buffer)[i][0] = 0;
		(*red_buffer)[i][*width + 1] = 0;
		(*green_buffer)[i][*width + 1] = 0;
		(*blue_buffer)[i][*width + 1] = 0;
	}
	if (*type == 0)
	{
		for (i = 1; i <= *height; i++)
		{
			for (j = 1; j <= *width; j++)
			{
				fread(&((*red_buffer)[i][j]), sizeof(char), 1, input_file);
			}
		}
	}
	else if (*type == 1)
	{
		for (i = 1; i <= *height; i++)
		{
			for (j = 1; j <= *width; j++)
			{
				fread(&((*red_buffer)[i][j]), sizeof(char), 1, input_file);
				fread(&((*green_buffer)[i][j]), sizeof(char), 1, input_file);
				fread(&((*blue_buffer)[i][j]), sizeof(char), 1, input_file);
			}
		}
	}
}

int num_digits(int n)
{
	int res = 1;
	while (n > 0)
	{
		res *= 10;
		n /= 10;
	}
	return res / 10;
}

void write_file(char *file_name, int width, int height, int maxval, int type, unsigned char **red_buffer,
				unsigned char **green_buffer, unsigned char **blue_buffer)
{
	int i, j;
	FILE *output_file = fopen(file_name, "wb");
	unsigned char c;
	c = 'P';
	fwrite(&c, sizeof(char), 1, output_file);
	if (type == 0)
		c = '5';
	else
		c = '6';
	fwrite(&c, sizeof(char), 1, output_file);
	c = '\n';
	fwrite(&c, sizeof(char), 1, output_file);
	int n = num_digits(width);
	while (n > 0)
	{
		c = '0' + width / n % 10;
		fwrite(&c, sizeof(char), 1, output_file);
		n /= 10;
	}
	c = ' ';
	fwrite(&c, sizeof(char), 1, output_file);
	n = num_digits(height);
	while (n > 0)
	{
		c = '0' + height / n % 10;
		fwrite(&c, sizeof(char), 1, output_file);
		n /= 10;
	}
	c = '\n';
	fwrite(&c, sizeof(char), 1, output_file);
	n = num_digits(maxval);
	while (n > 0)
	{
		c = '0' + maxval / n % 10;
		fwrite(&c, sizeof(char), 1, output_file);
		n /= 10;
	}
	c = '\n';
	fwrite(&c, sizeof(char), 1, output_file);
	if (type == 0)
	{
		for (i = 1; i <= height; i++)
		{
			for (j = 1; j <= width; j++)
			{
				fwrite(&red_buffer[i][j], sizeof(char), 1, output_file);
			}
		}
	}
	else if (type == 1)
	{
		for (i = 1; i <= height; i++)
		{
			for (j = 1; j <= width; j++)
			{
				fwrite(&red_buffer[i][j], sizeof(char), 1, output_file);
				fwrite(&green_buffer[i][j], sizeof(char), 1, output_file);
				fwrite(&blue_buffer[i][j], sizeof(char), 1, output_file);
			}
		}
	}
}

unsigned char compute_filter(unsigned char i11, unsigned char i12, unsigned char i13,
							 unsigned char i21, unsigned char i22, unsigned char i23, unsigned char i31,
							 unsigned char i32, unsigned char i33)
{
	unsigned char min_value;

	min_value = min(i11, i12);
	min_value = min(min_value, i13);
	min_value = min(min_value, i21);
	min_value = min(min_value, i22);
	min_value = min(min_value, i23);
	min_value = min(min_value, i31);
	min_value = min(min_value, i32);
	min_value = min(min_value, i33);

	return min_value;
}

int main(int argc, char *argv[])
{
	struct timeval start_time, end_time;
	double elapsed;
	int nThreads;
	int rank;
	int nProcesses;
	int i;
	int type, width, height, maxval, nr_filters = 1;
	if (argc != 4) {
        printf("Not enough arguments! Usage: mpirun -np no_of_processes ./hybrid file_in_name file_out_name no_of_threads\n");
        return -1;
	}
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	unsigned char **red_buffer, **green_buffer, **blue_buffer;
	if (rank == 0)
	{
		read_file(argv[1], &width, &height, &maxval, &type, &red_buffer, &green_buffer, &blue_buffer);
		for (i = 1; i < nProcesses; i++)
		{
			MPI_Send(&width, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&height, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&type, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		MPI_Recv(&width, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&height, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&type, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	nThreads = atoi(argv[3]);
	omp_set_num_threads(nThreads);
	float div = ((float)height) / nProcesses;
	int start = rank * ceil(div) + 1;
	int finish = fmin(height, (rank + 1) * (ceil(div)));
	int expected_lines = finish - start + 3;
	unsigned char **old_rb, **old_gb, **old_bb;
	unsigned char **new_rb, **new_gb, **new_bb;
	old_rb = malloc(expected_lines * sizeof(unsigned char *));
	old_gb = malloc(expected_lines * sizeof(unsigned char *));
	old_bb = malloc(expected_lines * sizeof(unsigned char *));
	new_rb = malloc(expected_lines * sizeof(unsigned char *));
	new_gb = malloc(expected_lines * sizeof(unsigned char *));
	new_bb = malloc(expected_lines * sizeof(unsigned char *));
	for (i = 0; i < expected_lines; i++)
	{
		old_rb[i] = malloc((width + 2) * sizeof(unsigned char));
		old_gb[i] = malloc((width + 2) * sizeof(unsigned char));
		old_bb[i] = malloc((width + 2) * sizeof(unsigned char));
		new_rb[i] = malloc((width + 2) * sizeof(unsigned char));
		new_gb[i] = malloc((width + 2) * sizeof(unsigned char));
		new_bb[i] = malloc((width + 2) * sizeof(unsigned char));
	}
	if (rank == 0)
	{
		int proc;
		gettimeofday(&start_time, 0);
		for (proc = 1; proc < nProcesses; proc++)
		{
			int first = proc * ceil(div) + 1;
			int last = fmin(height, (proc + 1) * (ceil(div)));
			int line;
			for (line = first - 1; line <= last + 1; line++)
			{
				MPI_Send(red_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD);
				if (type == 1)
				{
					MPI_Send(green_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD);
					MPI_Send(blue_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD);
				}
			}
		}
		int line;
		for (line = 0; line < expected_lines; line++)
		{
			memcpy(old_rb[line], red_buffer[line], width + 2);
			if (type == 1)
			{
				memcpy(old_gb[line], green_buffer[line], width + 2);
				memcpy(old_bb[line], blue_buffer[line], width + 2);
			}
		}
	}
	else
	{
		int line;
		for (line = 0; line < expected_lines; line++)
		{
			MPI_Recv(old_rb[line], width + 2, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (type == 1)
			{
				MPI_Recv(old_gb[line], width + 2, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(old_bb[line], width + 2, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}
	int index;
	for (index = 0; index < nr_filters; index++)
	{
		int line, collumn;
		for (line = 1; line <= expected_lines - 2; line++)
		{
			#pragma omp parallel for
			for (collumn = 1; collumn <= width; collumn++)
			{
				new_rb[line][collumn] = compute_filter(old_rb[line - 1][collumn - 1], old_rb[line - 1][collumn], old_rb[line - 1][collumn + 1],
													   old_rb[line][collumn - 1], old_rb[line][collumn], old_rb[line][collumn + 1], old_rb[line + 1][collumn - 1],
													   old_rb[line + 1][collumn], old_rb[line + 1][collumn + 1]);
				if (type == 1)
				{
					new_gb[line][collumn] = compute_filter(old_gb[line - 1][collumn - 1], old_gb[line - 1][collumn], old_gb[line - 1][collumn + 1],
														   old_gb[line][collumn - 1], old_gb[line][collumn], old_gb[line][collumn + 1], old_gb[line + 1][collumn - 1],
														   old_gb[line + 1][collumn], old_gb[line + 1][collumn + 1]);
					new_bb[line][collumn] = compute_filter(old_bb[line - 1][collumn - 1], old_bb[line - 1][collumn], old_bb[line - 1][collumn + 1],
														   old_bb[line][collumn - 1], old_bb[line][collumn], old_bb[line][collumn + 1], old_bb[line + 1][collumn - 1],
														   old_bb[line + 1][collumn], old_bb[line + 1][collumn + 1]);
				}
			}
		}
		if (nProcesses >= 2)
		{
			if (rank == 0)
			{
				MPI_Send(new_rb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);
				if (type == 1)
				{
					MPI_Send(new_gb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);
					MPI_Send(new_bb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD);
				}
				MPI_Recv(new_rb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (type == 1)
				{
					MPI_Recv(new_gb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(new_bb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
			else if (rank == nProcesses - 1)
			{
				MPI_Recv(new_rb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (type == 1)
				{
					MPI_Recv(new_gb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(new_bb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				MPI_Send(new_rb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
				if (type == 1)
				{
					MPI_Send(new_gb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
					MPI_Send(new_bb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
				}
			}
			else
			{
				MPI_Recv(new_rb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (type == 1)
				{
					MPI_Recv(new_gb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(new_bb[0], width + 2, MPI_UNSIGNED_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
				MPI_Send(new_rb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
				if (type == 1)
				{
					MPI_Send(new_gb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
					MPI_Send(new_bb[1], width + 2, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD);
				}
				MPI_Send(new_rb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
				if (type == 1)
				{
					MPI_Send(new_gb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
					MPI_Send(new_bb[expected_lines - 2], width + 2, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD);
				}
				MPI_Recv(new_rb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (type == 1)
				{
					MPI_Recv(new_gb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(new_bb[expected_lines - 1], width + 2, MPI_UNSIGNED_CHAR, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}
		unsigned char **aux;
		aux = old_rb;
		old_rb = new_rb;
		new_rb = aux;
		if (type == 1)
		{
			aux = old_gb;
			old_gb = new_gb;
			new_gb = aux;
			aux = old_bb;
			old_bb = new_bb;
			new_bb = aux;
		}
	}
	if (rank == 0)
	{
		int proc;
		for (proc = 1; proc < nProcesses; proc++)
		{
			int first = proc * ceil(div) + 1;
			int last = fmin(height, (proc + 1) * (ceil(div)));
			int line;
			for (line = first; line <= last; line++)
			{
				MPI_Recv(red_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if (type == 1)
				{
					MPI_Recv(green_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(blue_buffer[line], width + 2, MPI_UNSIGNED_CHAR, proc, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}
		int line;
		for (line = 1; line <= expected_lines - 2; line++)
		{
			memcpy(red_buffer[line], old_rb[line], width + 2);
			if (type == 1)
			{
				memcpy(green_buffer[line], old_gb[line], width + 2);
				memcpy(blue_buffer[line], old_bb[line], width + 2);
			}
		}
	}
	else
	{
		int line;
		for (line = 1; line <= expected_lines - 2; line++)
		{
			MPI_Send(old_rb[line], width + 2, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
			if (type == 1)
			{
				MPI_Send(old_gb[line], width + 2, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
				MPI_Send(old_bb[line], width + 2, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
			}
		}
	}
	if (rank == 0)
	{
		gettimeofday(&end_time, 0);
		long seconds = end_time.tv_sec - start_time.tv_sec;
		long microseconds = end_time.tv_usec - start_time.tv_usec;
		elapsed = seconds + microseconds * 1e-6;
		printf("\nHYBRID took %f seconds with %d processes and %d threads per process\n", elapsed, nProcesses, nThreads);
		write_file(argv[2], width, height, maxval, type, red_buffer, green_buffer, blue_buffer);
	}

	MPI_Finalize();
	return 0;
}