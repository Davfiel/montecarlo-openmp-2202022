/* ERROR INTENCIONAL: acumulador compartido sin reduction */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main(int argc,char*argv[]){long N=atol(argv[1]);int p=atoi(argv[2]);omp_set_num_threads(p);
 long dentro=0;
 #pragma omp parallel
 { unsigned int st=2202022u+977u*omp_get_thread_num();
   #pragma omp for
   for(long i=0;i<N;i++){double x=rand_r(&st)/(double)RAND_MAX,y=rand_r(&st)/(double)RAND_MAX;
     if(x*x+y*y<=1.0) dentro++;} }
 printf("p=%d pi=%.6f\n",p,4.0*dentro/(double)N);return 0;}
