# Section 2

In this folder there are:

* The filder *'csv-files/'* with all the graphs and picture taken from my data.
* The scripts used for the data in the report/csv.
* The *'mydata/'* folder where there are the data obtained from my measuremnts used in the report/csv.
* Inside *'mydata/'* there also are some R scripts used to generate plots from my data.
* Inside *'others/'* folder there is the program *'formatter2.py'* and script *'formatter2.sh'* used to get from the data placed inside *'out/'* folder the mean time t between the 10 running results for each message size X, along with some statistics (ex. maximum error on time e_t = t_max - t_min/2).
* *'plot.Rmd'* is an R script that I used in order to create csv files and plots inside directory *'csv-files/'*.

## CSV FILES
I have added a few column is each csv.

Your requirements were:

- #header_line 1: command line used 
- #header_line 2: list of nodes involved 
- #header_line 3: lamba, bandwith computed by fitting data 
- #header: #bytes #repetitions      t[usec]   Mbytes/sec    t[usec] computed   Mbytes/sec (computed )

Instead in my files there is no mention of the header_line 1, header_line 2 and header_line 3, but:
- command line used: it can be easly inferred from the report.html.
- list of nodes involved: they are mentioned in the report.html file:
    * ct1pt-tnode007, thin node for results across cores, sockets and nodes.
    * ct1pt-tnode008, thin node for results across nodes.
    * ct1pg-gnode001, gpu node for results across cores, sockets and nodes.
    * ct1pg-gnode003, gpu node for results across nodes.
- lamba, bandwith computed by fitting data: they are present as columns named respectively 'l_fit' and 'b_fit' along with their standard errors 'l_std' and 'b_std'.
- the #header line has several columns more: 'err_speed','err_lat' which are the maximum errors of throughput and time taken from PingPong benchmark on 10 measurements overall.
- Columns 't_est' and 'b_est' are the 't[usec] computed' and 'Mbytes/sec (computed )' respectively.
