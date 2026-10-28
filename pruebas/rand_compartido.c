/* ERROR INTENCIONAL: todos los hilos usan rand() con estado global */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main(int argc,char*argv[]){long N=atol(argv[1]);int p=atoi(argv[2]);omp_set_num_threads(p);
 srand(2202022u);long dentro=0;double t0=omp_get_wtime();
 #pragma omp parallel for reduction(+:dentro)
 for(long i=0;i<N;i++){double x=rand()/(double)RAND_MAX,y=rand()/(double)RAND_MAX;
   if(x*x+y*y<=1.0) dentro++;}
 printf("p=%d pi=%.6f t=%.3fs\n",p,4.0*dentro/(double)N,omp_get_wtime()-t0);return 0;}
