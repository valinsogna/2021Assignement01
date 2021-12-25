#!/bin/bash
#PBS -l nodes=2:ppn=24
#PBS -l walltime=00:30:00
#PBS -q dssc

module load openmpi-4.1.1+gnu-9.3.0
cd $PBS_O_WORKDIR
EXE="/fast/dssc/valinsogna/2021Assignement01/section1/ring.x"
clean_ring

mkdir -p by_node
mkdir -p by_core
mkdir -p by_socket
mkdir -p default

cd by_node
mkdir out
for i in {1..24}; do 
    mpirun --map-by node --mca btl ^openib -np $i ${EXE}  >> stdout_node$i.txt   
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
cd by_core
mkdir out
for i in {1..24}; do 
    mpirun --map-by core --mca btl ^openib -np $i ${EXE}  >> stdout_core$i.txt   
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
cd by_socket
mkdir out
for i in {1..24}; do 
    mpirun --map-by socket --mca btl ^openib -np $i ${EXE} >> stdout_socket$i.txt
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
cd default
mkdir out
for i in {1..24}; do 
    mpirun --mca btl ^openib -np $i ${EXE} >> stdout_default$i.txt
done
cat $PBS_NODEFILE >> thin_resources_used.out

cd ..
exit