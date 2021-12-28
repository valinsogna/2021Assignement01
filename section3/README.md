# Section 2

In this folder there are:

* The filder *'csv-files/'* with all the thin/gpu data and the performance model.
* The script used for the thin node is 'jacobi_thin.sh'. (For gpu simply change queue *dssc_gpu*, *ppn* requested submitting the job and remove by node mapping.)'
* The *'mydata/dati_orfeo/'* folder where there are the raw data obtained from my measuremnts used in the report/csv.
* Inside *'mydata/'* there also are some csv files used to sum up the results from measuremets.
* Inside *'others/'* folder there is the program *'visualize.py'* and script *'parse.sh'* used to get from the raw data placed inside *'mydata/dati_orfeo/'* folder the mean performance in MLUP/s between the output results from *'jacobi3D.x'* program.
* *'jacobi3D.x'* is the executable used.

## CSV FILES
The data along with the expected performance is divided in thin and gpu folders.
Moreover, *Performance.xlsx* reports the same results in excel tables.

