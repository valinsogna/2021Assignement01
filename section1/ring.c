#include <stdio.h>
#include <mpi.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define ITERS 10000

int main (int argc, char * argv[]){

    double start_time, end_time, time_us;
    double old_mean, mean = 0., s2, var = 0., var_mean, dev, s, N, mean_permsg;
    int old_rank, size, next_p, prev_p, recleft, sendleft, recright, sendright, init_tag, msg_count, my_rank;
    int ndim,reorder;
    int dims[1],periods[1];
    MPI_Status status[4];
    MPI_Request request[4];
    
    //FILE *fp;
    //fp = fopen("./out/ring.csv", "a");

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD,&old_rank );
    MPI_Comm_size( MPI_COMM_WORLD,&size );  
    ndim = 1;
    dims[1] = {0};
    MPI_Dims_create(size, ndim, dims);
    periods[1] = {TRUE};
    reorder = FALSE;
    MPI_Comm new_comm;
    MPI_Cart_create(MPI_COMM_WORLD, ndim, dims, periods, reorder, &new_comm);
    MPI_Comm_rank(new_comm, &my_rank);
    MPI_Cart_shift( new_comm, 0, 1, &prev_p, &next_p );

    for(int i=0;i<ITERS;i++) {
        sendright = - my_rank;
        sendleft = my_rank;
        init_tag = my_rank*10;
        msg_count = 1;

        start_time = MPI_Wtime();

        MPI_Isend( &sendleft, 1, MPI_INT, prev_p, init_tag, new_comm, &request[0]);
        MPI_Isend( &sendright, 1, MPI_INT, next_p, init_tag, new_comm, &request[1] );
        MPI_Irecv( &recleft, 1, MPI_INT, prev_p, MPI_ANY_TAG, new_comm, &request[2] );
        MPI_Irecv( &recright, 1, MPI_INT, next_p, MPI_ANY_TAG, new_comm, &request[3] );

        MPI_Wait(&request[0], &status[0]);
        MPI_Wait(&request[1], &status[1]);
        MPI_Wait(&request[2], &status[2]);
        MPI_Wait(&request[3], &status[3]);

        while(status[2].MPI_TAG != init_tag && status[3].MPI_TAG != init_tag){
            msg_count++;

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

        end_time = MPI_Wtime();
        time_us = (1e6)*end_time - (1e6)*start_time;
  
        old_mean = mean;
        mean = (double)(i)/(double)(i+1)*mean +
            1./(double)(i+1)*time_us;
        var = 1./(double)(i+1)*
            ((double)(i)* var + time_us*time_us +
            (double)(i)* old_mean*old_mean) -
            mean*mean;
    }

    // if(my_rank == 0){
    //     fprintf(fp, "rank,Np,mean,mean_permsg,var,s2,N,dev,s,dev_mean\n");
    // }
    N = (double)(ITERS);
    s2 = N/(N-1)*var;
    dev = sqrt(var);
    s = sqrt(s2);
    mean_permsg = mean/N;
    // fprintf(fp, "%i,%i,%f,%f,%f,%f,%i,%f,%f,%f,%f\n",my_rank,size,mean,mean_permsg,var,s2,ITERS*size,dev,s);
    // printf("I'm %d My time %f uSec, s %f and var %f\n",my_rank,mean,s,var);
    // //printf("%i,%f,%f,%f,%f,%i\n",size,mean,mean_permsg,var,s2,ITERS*size);
    char file_stat[100];
    snprintf(file_stat, sizeof(file_stat), "./out/np%i.csv", size);
    MPI_File stat_file;
    MPI_File_open( MPI_COMM_WORLD, file_stat, MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &stat_file );
    MPI_File_close(&stat_file);
    MPI_File_open( MPI_COMM_WORLD, file_stat, MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &stat_file );
    char buf[150];
    //buf[0] = "rank,Np,mean,mean_permsg,var,s2,N,dev,s\n"
    int num = sprintf(buf,"%i,%i,%f,%f,%f,%f,%i,%f,%f\n",my_rank,size,mean,mean_permsg,var,s2,ITERS*size,dev,s);
    MPI_File_write_ordered(stat_file,buf, num, MPI_CHARACTER, &status[0]);
    MPI_File_close(&stat_file);

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
    //fclose(fp);
}