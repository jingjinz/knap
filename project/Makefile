CC=c99
FLAGS+=-Wall
FLAGS+=-g

all: omp

omp: knapsack_omp.c
	$(CC) $(FLAGS) -fopenmp -o run_omp_knapsack knapsack_omp.c

single: knap.c
	$(CC) $(FLAGS) -o run_single_knapsack knap.c
