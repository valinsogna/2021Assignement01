#!/bin/bash
#PBS -l nodes=2:ppn=24
#PBS -l walltime=00:30:00
#PBS -q dssc

module load openmpi-4.1.1+gnu-9.3.0
cd $PBS_O_WORKDIR
mkdir -p by_node
mkdir -p by_core
mkdir -p by_socket
mkdir -p default

#mpicc ring.c -o ring.x -lm
#qsub -l nodes=1:ppn=24 -I -l walltime=1:00:00 -q dssc


cd by_node
mkdir out
for i in {1..24}; do 
    mpirun --map-by node -np $i --report-bindings ./../../../ring.x   
done


cd by_core
mkdir out
for i in {1..24}; do 
    mpirun --map-by core -np $i --report-bindings ./../../ring.x  >> std_out$i.txt   
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
cd by_socket
mkdir out
for i in {1..24}; do 
    mpirun --map-by socket -np $i --report-bindings ./../../ring.x >> std_out$i.txt
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
cd default
mkdir out
for i in {1..24}; do 
    mpirun -np $i --report-bindings ./../../ring.x >> std_out$i.txt
done
cat $PBS_NODEFILE >> thin_resources_used1.out

exit