build:
	gcc compare.c -o compare
	gcc sequential_imp.c -o sequential
	gcc pthreads_imp.c -lm -lpthread -o pthreads
	gcc omp_imp.c -lm -fopenmp -o openmp

clean:
	rm compare
	rm sequential
	rm pthreads
	rm openmp