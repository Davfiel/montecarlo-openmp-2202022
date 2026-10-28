/* montecarlo_secuencial.c -- version secuencial de referencia (II.1),
 * identica a la entregada por el docente salvo la medicion de tiempo.
 * Compilar: gcc -O2 -fopenmp montecarlo_secuencial.c -o montecarlo_sec -lm */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    long N = (argc > 1) ? atol(argv[1]) : 100000000L;
    unsigned int estado = 20261018u; /* semilla del generador */
    long dentro = 0;

    double t0 = omp_get_wtime();
    for (long i = 0; i < N; i++) {
        double x = rand_r(&estado) / (double)RAND_MAX;
        double y = rand_r(&estado) / (double)RAND_MAX;
        if (x * x + y * y <= 1.0)
            dentro++;
    }
    double t1 = omp_get_wtime();

    double pi_est = 4.0 * dentro / (double)N;
    printf("Estimacion de pi: %.6f\n", pi_est);
    printf("Error |pi_est - pi|: %.2e\n", fabs(pi_est - M_PI));
    printf("Tiempo T(1): %.6f s\n", t1 - t0);
    return 0;
}
