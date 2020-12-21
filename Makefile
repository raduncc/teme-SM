build:
	gcc compare.c -o compare -Wall
	gcc sequential_imp.c -o sequential -Wall
	gcc pthreads_imp.c -lm -lpthread -o pthreads -Wall
	gcc omp_imp.c -lm -fopenmp -o openmp -Wall
	gcc omp_imp2.c -lm -fopenmp -o openmp2 -Wall
	mpicc mpi_imp.c -lm -o mpi -Wall
	mpicc hybrid_imp.c -lm -fopenmp -o hybrid -Wall

clean:
	rm compare
	rm sequential
	rm pthreads
	rm openmp
	rm openmp2
	rm mpi
	rm hybrid