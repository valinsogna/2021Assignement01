# 2021Assignement01

This repository contains **MPI programs exploring parallel computing techniques**. It implements a message-passing ring topology, benchmarks point-to-point MPI performance, and studies various computational tasks like the Jacobi solver and 3D matrix operations. The repo evaluates performance across different topologies, networks, and nodes, using OpenMPI and IntelMPI on the ORFEO platform.


## Exercise 1: MPI Ring Topology
### Description:
This repository hosts an MPI-based C program that simulates message passing among P processes in a ring topology with periodic boundaries. The program facilitates the transmission of a stream of 4B messages, both clockwise and counter-clockwise. The detailed information about the implementation, the methodology used, and the performance results can be found below.

### Key Features:
- Ring Topology: The program sets up a ring topology and leverages periodic boundaries.
- Message Passing: Messages of type int (the rank) are sent/received with a tag proportional to the rank (tag = rank * 10).
- Output Files: Outputs are stored in the ./out folder with filenames formatted as 'npP.txt'.
- MPI Routines: The MPI routines used include MPI_Isend, MPI_Irecv, and MPI_Wait.
- Performance Analysis: Performance is analyzed up to P = 24 processes, and runtime data, captured using MPI_Wtime(), is stored in ./out in the 'npP.csv' file.

Running the Program:
Ensure that you have the latest version of OpenMPI (tested on openmpi-4.1.1+gnu-9.3.0).

```bash
mpicc ring.c -o ring
mpirun -np [NUMBER_OF_PROCESSES] ./ring
# Check the ./out folder for the output files.
```

## Exercise 1.2: Measure MPI Point-to-Point Performance
### Description:
In this exercise, we use the Intel MPI IMB-MPI1 benchmark PingPong to estimate latency and bandwidth for various combinations of topologies and networks on ORFEO computational nodes.

### Key Features:
- Network Protocols Tested: Includes 100 Gbit InfiniBand and 25 Gbit Ethernet.
- Devices Tested: With OpenMPI, tested across nodes including ib0, br0, and mlx5_0:1.
- Benchmarking: Detailed analysis using the IMB-MPI1 PingPong benchmark for different configurations.
- Running the Benchmark: Detailed instructions and scripts for running the benchmark will be provided.

## Exercise 2: Jacobi Solver
### Description:
This exercise entails the 3D implementation of the Jacobi solver, a stencil-based iterative method, used for solving partial differential equations.

### Key Features:
- Parallel Computation: Uses parallel processing to calculate the convergence criterion and update boundary layer values.
- Performance Modeling: Provides a model to predict the performance of the 3D Jacobi solver.
- Domain Decomposition: Tests for optimal domain decompositions for performance enhancement.
- Running the Jacobi Solver:Detailed instructions and scripts for executing the Jacobi solver will be provided.

## Exercise 3: 3D Matrix Sum
### Description:
The exercise focuses on parallel summing of a 3D matrix. Given its representation as a linear array in memory, the most effective method for parallel addition is using collective operation.

### Key Features:
- Efficient Summation: The program disregards the 3D shape for simplicity and efficiency in summation.
- MPI Routines: Uses MPI_Scatterv and other MPI functions for distributing data and collecting results.
- Running the 3D Matrix Sum:
- Detailed instructions and scripts for computing the 3D matrix sum will be provided.


## Report
In this folder there are the results from the 1 assignement shown in the *'report.html'* file.

I strongly suggest to read the report via pdf file, since with the html version the latex formula are disturbed.

In folder *'section1/'*, *'section2/'* and *'section3/'* there are the data results from each section along with the programs used.

In folder *'img/'* there are the pictures used in the report file.

**Instructions are inside the README in each folder.**
