#!/bin/bash 
 
#PBS -q dssc 
#PBS -l nodes=2:ppn=24
#PBS -l walltime=0:30:00 
 
cd /u/dssc/valinsogna/mpi-benchmarks/src_c
make clean 
module load openmpi-4.1.1+gnu-9.3.0 
make
EXE="/u/dssc/valinsogna/mpi-benchmarks/src_c/IMB-MPI1"

cd $PBS_O_WORKDIR 
mkdir -p openmpi_lib
cd openmpi_lib
mkdir -p thin
cd thin
mkdir -p out
cd out
OUT='/fast/dssc/valinsogna/2021Assignement01/section2/openmpi_lib/thin/out'

for i in {1..10}; do 
    mpirun --map-by node -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' > ${OUT}/node_ib.out 
    #mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=ib0 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_ib0.out 
    #mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=br0 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_br0.out 
    #mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=mlx5_0:1 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_mlx5.out 
    mpirun --map-by node --mca pml ob1 --mca btl self,tcp -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ob1_selftcp.out 
done 
 
for i in {1..10}; do 
    mpirun --map-by socket -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/socket_ib.out 
    mpirun --map-by socket --mca pml ob1 --mca btl self,vader -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/socket_ob1_selfvader.out 
    mpirun --map-by socket --mca pml ob1 --mca btl self,tcp -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/socket_ob1_selftcp.out 
done 
 
for i in {1..10}; do 
    mpirun --map-by core -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/core_ib.out 
    mpirun --map-by core --mca pml ob1 --mca btl self,vader -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/core_ob1_selfvader.out 
    mpirun --map-by core --mca pml ob1 --mca btl self,tcp -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/core_ob1_selftcp.out 
 
done 
cd ..
cat $PBS_NODEFILE >> nodes_used.out

cd /u/dssc/valinsogna/mpi-benchmarks/src_c
make clean 
module load openmpi/4.0.3/gnu/9
make
cd $PBS_O_WORKDIR

for i in {1..10}; do 
    mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=ib0 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_ib0.out 
    mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=br0 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_br0.out 
    mpirun --map-by node --mca pml ucx -mca btl ^uct -x UCX_NET_DEVICES=mlx5_0:1 -np 2 ${EXE} PingPong -msglog 28 | grep -v ^\# | grep -v '^$' | sed -r 's/^\s+//;s/\s+/,/g' >> ${OUT}/node_ucx_mlx5.out
done
cd..
cat $PBS_NODEFILE >> nodes_used.out

exit