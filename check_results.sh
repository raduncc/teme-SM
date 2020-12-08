#!/bin/bash

make
./sequential tests/darth-vader.pgm darth_seq
./sequential tests/macro.pnm macro_seq
for i in 1 2 4
do
	./pthreads tests/darth-vader.pgm darth_p{i} i
	./omp_imp tests/darth-vader.pgm darth_o{i} i
	./omp_imp2 tests/darth-vader.pgm darth_o2{i} i
	./pthreads tests/macro.pnm macro_p{i} i
	./omp_imp tests/macro.pnm macro_o{i} i
	./omp_imp tests/macro.pnm macro_o2{i} i
done

for i in 1 2 4
do
	./compare darth_seq darth_p{i}
	./compare darth_seq darth_o{i}
	./compare darth_seq darth_o2{i}

	./compare macro_seq macro_p{i}
	./compare macro_seq macro_o{i}
	./compare macro_seq macro_o2{i}
done


make clean
rm macro*
rm darth*
