/*
Author: Valeria Insogna
Date: 30/12/2021

I have implemented an MPI program in C with 2P messages passing among $P$ processes on a ring topology with periodic boundaries.

The program implements a stream of messages of 4B each, both clockwise and anticlockwise: each process in fact send/receive messages 
of type int (the $rank$) with a tag proportional to the rank ( tag=rank*10 ).

When running on P process, the program prints out in folder ./out (ATT: folder must be present) a file 'npP.txt' with the following output ordered by rank (here $P=5$):

`I am process 0 and I have received 5 messages. My final messages have tag 0 and value msg-left -10, msg-right 10`
`I am process 1 and I have received 5 messages. My final messages have tag 10 and value msg-left -10, msg-right 10`
`I am process 2 and I have received 5 messages. My final messages have tag 20 and value msg-left -10, msg-right 10`
`I am process 3 and I have received 5 messages. My final messages have tag 30 and value msg-left -10, msg-right 10`
`I am process 4 and I have received 5 messages. My final messages have tag 40 and value msg-left -10, msg-right 10`

At each run with P processes, for each process I have taken the mean of the runtime in microseconds out of 10000 
iterations and print it out (along with some statistics) by ranking order on the file ‘npP.csv’ in folder ./out, as follows:

Results with P = 5
rk	P	t_mean	t_xmsg	var	s2	N	dev	s
0	5	7.073931	1.414786	1425.467	1425.496	10000	37.75536	37.75574
1	5	7.072714	1.414543	1423.095	1423.123	10000	37.72393	37.72430
2	5	7.073540	1.414708	1428.383	1428.412	10000	37.79395	37.79433
3	5	7.072797	1.414559	1419.479	1419.507	10000	37.67598	37.67635
4	5	7.074060	1.414812	1430.666	1430.694	10000	37.82414	37.82452

*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define ITERS 10000

int main (int argc, char * argv[]){

    //Variables initialization
    double start_time, end_time, time_us;
    double old_mean, mean = 0., s2, var = 0., var_mean, dev, s, N, mean_permsg;
    int old_rank, size, next_p, prev_p, recleft, sendleft, recright, sendright, init_tag, msg_count, my_rank;
    int ndim,reorder;
    int dims[1],periods[1];
    MPI_Status status[4];
    MPI_Request request[4];

    //Start MPI
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD,&old_rank );
    MPI_Comm_size( MPI_COMM_WORLD,&size );  

    //Create 1D ring topology with periodic boundaries
    ndim = 1;
    dims[1] = {0};
    MPI_Dims_create(size, ndim, dims);
    periods[1] = {TRUE};
    reorder = FALSE;
    MPI_Comm new_comm;
    MPI_Cart_create(MPI_COMM_WORLD, ndim, dims, periods, reorder, &new_comm);
    MPI_Comm_rank(new_comm, &my_rank);
    //Identify the previous and next process (by rank order) with respect to my_rank process
    MPI_Cart_shift( new_comm, 0, 1, &prev_p, &next_p );

    /*
    For each run with 'size' processes, 10.000 iterations are performed for each process in order to have some 
    statistics for the runtime (mean, variance).
    */
    for(int i=0;i<ITERS;i++) {
        sendright = - my_rank;
        sendleft = my_rank;
        //Both messages originating from a certain processor P should have a tag proportional to its rank:
        init_tag = my_rank*10;
        msg_count = 1;

        //Start taking the elapsed time for each process
        start_time = MPI_Wtime();

        /*
        As first step process my_rank sends a message ( sendleft = my_rank ) to its left neighbour (prev_p) and
        receives from its right neighbour (next_p) and send aother message ( sendright = - my_rank) to next_p and receive from prev_p.
        */
        MPI_Isend( &sendleft, 1, MPI_INT, prev_p, init_tag, new_comm, &request[0]);
        MPI_Isend( &sendright, 1, MPI_INT, next_p, init_tag, new_comm, &request[1] );
        MPI_Irecv( &recleft, 1, MPI_INT, prev_p, MPI_ANY_TAG, new_comm, &request[2] );
        MPI_Irecv( &recright, 1, MPI_INT, next_p, MPI_ANY_TAG, new_comm, &request[3] );

        //MPI_Wait routine waits for a specific non blocking operation to complete
        MPI_Wait(&request[0], &status[0]);
        MPI_Wait(&request[1], &status[1]);
        MPI_Wait(&request[2], &status[2]);
        MPI_Wait(&request[3], &status[3]);

        while(status[2].MPI_TAG != init_tag && status[3].MPI_TAG != init_tag){
            msg_count++;

            /*
            At each iteration each processor add its rank to the received message if it comes from left, 
            substracting if it comes from right
            */
            sendright = recleft - my_rank;
            sendleft = recright + my_rank;
            MPI_Isend( &sendleft, 1, MPI_INT, prev_p, status[3].MPI_TAG, new_comm, &request[0] );
            MPI_Isend( &sendright, 1, MPI_INT, next_p, status[2].MPI_TAG, new_comm, &request[1] );
            MPI_Irecv( &recleft, 1, MPI_INT, prev_p, MPI_ANY_TAG, new_comm, &request[2] );
            MPI_Irecv( &recright, 1, MPI_INT, next_p, MPI_ANY_TAG, new_comm, &request[3] );

            MPI_Wait(&request[0], &status[0]);
            MPI_Wait(&request[1], &status[1]);
            MPI_Wait(&request[2], &status[2]);
            MPI_Wait(&request[3], &status[3]);
        }
        //Stop runtime
        end_time = MPI_Wtime();
        //Take the difference and convert it to microseconds
        time_us = (1e6)*end_time - (1e6)*start_time;

        //Update the mean and the variance of the population
        old_mean = mean;
        mean = (double)(i)/(double)(i+1)*mean +
            1./(double)(i+1)*time_us;
        var = 1./(double)(i+1)*
            ((double)(i)* var + time_us*time_us +
            (double)(i)* old_mean*old_mean) -
            mean*mean;
    }


    N = (double)(ITERS);
    s2 = N/(N-1)*var; //variance of the sample
    dev = sqrt(var); //standard deviation
    s = sqrt(s2); //standard deviation of the sample
    mean_permsg = mean/(double)(size); //time for message passing among 2 processes

    //Write on an output file some statistics of the runtime per process
    char file_stat[100];
    snprintf(file_stat, sizeof(file_stat), "./out/np%i.csv", size);
    MPI_File stat_file;
    //Open file 'npP.csv' in folder './out' if it exist and delete it
    MPI_File_open( MPI_COMM_WORLD, file_stat, MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &stat_file );
    MPI_File_close(&stat_file);
    //Open it again
    MPI_File_open( MPI_COMM_WORLD, file_stat, MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &stat_file );
    char buf[150];
    int num = sprintf(buf,"%i,%i,%f,%f,%f,%f,%i,%f,%f\n",my_rank,size,mean,mean_permsg,var,s2,ITERS*size,dev,s);
    //Write with a collective routine using shared file pointer among processes.
    MPI_File_write_ordered(stat_file,buf, num, MPI_CHARACTER, &status[0]);
    MPI_File_close(&stat_file);

    //Write on an output file 'npP.txt' in folder './out' some info about message passing per process
    char file_out[100];
    snprintf(file_out, sizeof(file_out), "./out/np%i.txt", size);
    MPI_File out_file;
    MPI_File_open( MPI_COMM_WORLD, file_out, MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &out_file );
    MPI_File_close(&out_file);
    MPI_File_open( MPI_COMM_WORLD, file_out, MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &out_file );
    char buffer[150];
    int n = sprintf(buffer,"I am process %d and I have received %d messages. My final messages have tag %d and value msg-left %d, msg-right %d\n", my_rank, msg_count, status[2].MPI_TAG, recleft, recright);
    MPI_File_write_ordered(out_file,buffer, n, MPI_CHARACTER, &status[1]);
    MPI_File_close(&out_file);

    // done with MPI 
    MPI_Finalize();
}