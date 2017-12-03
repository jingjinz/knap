# La Mochila

La Mochila is my 4 project of the Parallel Programming class. "La mochila"
directly translates from spanish into "the knapsack", or "the satchel".

The code uses the openMP library to parallelize the code; to compile
just run `make`. After this is done, you can run the program.

The executable is called `run_omp_knapsack`, command line arguments specify
the different parameters for a run. To run the program, simply do:

    ./run_omp_knapsack 30000 30000 50

This will generate a problem with 30000 items, a knapsack with capacity
if 30000 and maximum weight/value of 50. By default, the code is written
to execute the code using 6 threads. In order to change this, the variable
`thread_count` should be modified inside the main function.
