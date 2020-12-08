#!/bin/bash

echo '===BUILDING==='
make

echo '===GETTING IMAGES==='

./sequential tests/darth-vader.pgm darth_seq
./sequential tests/macro.pnm macro_seq

for i in 1 2 4
do
	./pthreads tests/darth-vader.pgm darth_p$i $i
	./openmp tests/darth-vader.pgm darth_o$i $i
	./openmp2 tests/darth-vader.pgm darth_o2$i $i
	./pthreads tests/macro.pnm macro_p$i $i
	./openmp tests/macro.pnm macro_o$i $i
	./openmp2 tests/macro.pnm macro_o2$i $i
done

echo '===COMPARING RESULTS==='
for i in 1 2 4
do
	echo 'Comparing darth_seq with dart parallel with $i threads'
	./compare darth_seq darth_p$i
	echo 'Comparing darth_seq with dart OMP1 with $i threads'
	./compare darth_seq darth_o$i
	echo 'Comparing darth_seq with dart OMP2 with $i threads'
	./compare darth_seq darth_o2$i


	echo 'Comparing macro_seq with macro parallel with $i threads'
	./compare macro_seq macro_p$i
	echo 'Comparing macro_seq with macro OMP1 with $i threads'
	./compare macro_seq macro_o$i
	echo 'Comparing macro_seq with macro OMP2 with $i threads'
	./compare macro_seq macro_o2$i
done

echo '===CLEANING==='

make clean
rm macro*
rm darth*
