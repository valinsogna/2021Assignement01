# Section 1

To compile ring.c and matrix.c on the login node just type:
- `module load openmpi-4.1.1+gnu-9.3.0`
- `make all`

Oss. In folder 'mydata/' there are the data obtained from my measuremnts on the ring and matrix that have been used in the report section.

## Exercise 1.1 Ring

To run ring.c submitting a job on a thin node using up to 24 processes (with 3 different mappings: by core, socket, node) simply run the script:
- `qsub ring.sh`

This script will run the program with different number of processes for each mappings.
The results and output files csv/txt are available in the subfolders 'out/' inside each 'by_core/', 'by_node/', 'by_socket/', 'default/' folder.

To clean all the measures of the ring.c program just type:
- `clean_ring`

## Excercise 1.2 Matrix 3D

The code is in folder *matrix/* with a dedicated Makefile.

Available rules:

- `make default` provide a program that satisfy the assigment requests, only message of correct result is provided from executable. -O3 flag is specified.
- `make timeit` as previous but each process provide some time output.
- `make print` resulting code provide some output from each processor that explain how many elements they get and if the result is correct.
- `make format` code formatter provided by clang.
- `make clean` remove executable produced The code is tested with openMPI 4.03.

How to run it: 
- `mpirun -np #nprocs matrix.x #nx #ny #nz`. Total element number must be greater or equalt than processors number. The element are equally distribuited to worker and reminder is assigned in round robin way.

## Others folder

There are the supported programs used for the ring exercise, like 'formatter.cpp'.
It has been used in order to create a csv file list selecting only the time measurements from the slower process among all of a specific run.

In order to compile it, type:
- `g++ -std=c++11 formatter.cpp -o formatter.x`

Then move 'formatter.x' and 'formatter.sh' in the folder 'out/' where there are all the files 'npP.csv/txt' with the results from a run and type:
- `chmod +x formatter.sh`
- `./formatter.sh`

It will produce a 'results.csv' table, that can be used in the report file.


