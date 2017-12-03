/*

 This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define MAX 3000001
#define N 3000000
#define C 100000
int max(int a, int b) {
    return a > b ? a : b;
}


int weight[MAX], value[MAX];
struct timeval tstart, tend;
double t1, t2;
int thread_count = 10;
int ans = 0;
int max_weights = 10;
int dp[N+1][C+1];
/*
 * Creates a new instance of the knapsack problem.
 *
 * Arguments:
 * `N`: Number of items.
 * `max_`: The maximum weight and value an item can have.
 * `w`: Array of weights. Output
 * `v`: Array of values. Output
 */
void create_problem(int max_, int w[N], int v[N]) {
    int i;
   // for (i=0 ; i<N ; i++) {
     //   w[i] = (rand() % max_) + 1;
     //   v[i] = (rand() % max_) + 1;
   // }
   //
   
   FILE *fpRead2=fopen("data/weight_3000000.txt","r");  
   FILE *fpRead=fopen("data/value_3000000.txt","r");  
    if(fpRead==NULL){  
	return;  
    }  
    for(i=0;i<N;i++)  {  
	fscanf(fpRead,"%d ",&v[i]);  
    }  

    if(fpRead2==NULL){  
	return;  
    }  
    for(i=0;i<N;i++)  {  
	fscanf(fpRead2,"%d ",&w[i]);  
    }  
}

/*
 * Implementation of parallel knapsack using dynamic programming.
 * It uses openMP to parallelize the code using a parallel for
 * directive. The main improvement over the original version is
 * that it only uses an array of two rows because for every element
 * to be computed, we only need two elements from the previous row.
 *
 * Arguments:
 * `N`: The number of items to be considered.
 * `C`: The capacity of the Knapsack.
 * `w`: Weights of the items.
 * `v`: Value of the items.
 */
int solve(int *w, int *v) {
   int i, j;
    dp[0][0] = 0;
    for (j=1 ; j<=C ; j++) {
	dp[0][j] = -1;
    }
    // Start on the second row.
    for (i=1 ; i<=N ; i++) {
#       pragma omp parallel for num_threads(thread_count)
        for (j=0 ; j<=C ; j++) {
	    if (i == 0 || j == 0) {
		dp[i][j] = 0;
	    } else {
		dp[i][j] = dp[i-1][j];
		if (j-w[i-1] >= 0 && dp[i-1][j-w[i-1]]>=0) {
		    dp[i][j] = max(dp[i-1][j], v[i-1] + dp[i-1][j-w[i-1]]);
		}
	    }
	 }
    }
    int res = -1;
    for (i = 0; i <= C; i++) {
	if (res < dp[N][i]) {
	    res = dp[N][i];
	}
    }
    return res;
}

/*
 * Implementation of the professor of the Knapsack problem
 * Same arguments as the previous function
 */
int solve2(int n, int c, int weight[MAX], int profit[MAX]) {
    int i, j;
    int total[2][MAX];
    int curr = 0;
    /* Max profit using thing 0, if it fits */
    for (j = 0; j < c; j++) {
        if (weight[0] > j) {
            total[curr][j] = 0;
        } else {
            total[curr][j] = profit[0];
        }
    }

    for (i = 1; i < n; i++) {
        for (j = 0; j < c; j++) {
            if ( (j<weight[i]) ||
                (total[!curr][j] >= total[!curr][j-weight[i]] + profit[i])) {
                total[curr][j] = total[!curr][j];
            } else {
                total[curr][j] = total[!curr][j-weight[i]] + profit[i];
            }
        }
        curr = !curr;
    }

    return total[!curr][c-1];
}

/*
 * Returns the amount of seconds passed from a time `tstart` to 
 * `tend`.
 *
 * Arguments:
 * `tstart`: Start time.
 * `tend`: End time.
 */
double check_time(struct timeval tstart, struct timeval tend) {
    unsigned long long usec;
    if (tend.tv_usec > tstart.tv_usec) {
        usec = (tend.tv_sec - tstart.tv_sec) * 1000000
                    + tend.tv_usec - tstart.tv_usec;
    } else {
        usec = (tend.tv_sec - (tstart.tv_sec + 1)) * 1000000
                + (1000000 + tend.tv_usec - tstart.tv_usec);
    }
    return (double)usec;
}

/*
 * Runs the main program
 */
int main(int argc, char *argv[]) {
    create_problem(max_weights, weight, value);

    gettimeofday(&tstart, NULL);
    ans = solve(weight, value);
    gettimeofday(&tend, NULL);


    t1 = check_time(tstart, tend);
    printf("Solver finished in %lf seconds.\n", t1/1000000.0);

    gettimeofday(&tstart, NULL);
    //solve2(N, C, weight, value);
    gettimeofday(&tend, NULL);

   // t2 = check_time(tstart, tend);

   // double s_scaling = t2 / (thread_count * t1);

  //  printf("Solver finished in %lf seconds.\n", t2/1000000.0);
    printf("Answer: %d \n", ans);
    //printf("Strong Scaling: %lf\n", s_scaling);

    return 0;
}
