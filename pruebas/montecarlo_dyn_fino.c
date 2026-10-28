/* montecarlo.c -- estimacion de pi por Monte Carlo (acierto y fallo) con OpenMP.
 *
 * Uso:   ./montecarlo <numero de muestras> [numero de hilos] [semilla base]
 * Compilar: gcc -O2 -fopenmp montecarlo.c -o montecarlo -lm
 *
 * Diseno (resumen):
 *  - Generador: LCG de 64 bits (constantes MMIX de Knuth) con salto hacia
 *    adelante en O(log k). Permite ubicar a cualquier hilo en cualquier punto
 *    de una UNICA secuencia global sin generar los valores intermedios.
 *  - El dominio [0, N) se divide en NBLOQUES bloques fijos (no dependen de p).
 *    El bloque b usa exactamente los numeros 2*inicio_b ... 2*fin_b - 1 de la
 *    secuencia global (2 numeros por muestra: x e y).
 *  - Consecuencias: (1) los flujos de los bloques nunca se solapan; (2) con el
 *    reparto estatico, cada hilo recibe ~NBLOQUES/p bloques contiguos, es decir
 *    un tramo de ~2N/p valores (>= N/p, como pide el enunciado); (3) la muestra
 *    i siempre usa los mismos numeros, sin importar p ni la planificacion, por
 *    lo que la estimacion es identica para cualquier numero de hilos.
 *  - El unico dato que se acumula es 'dentro', con reduction(+:dentro).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>

/* TODO: reemplazar por la semilla asignada por el docente. */
#define SEMILLA_BASE 20261018ULL

#define LCG_A 6364136223846793005ULL
#define LCG_C 1442695040888963407ULL
#define NBLOQUES 100000000L

/* Siguiente valor del LCG: s <- a*s + c (mod 2^64). */
static inline uint64_t lcg_siguiente(uint64_t *s) {
    *s = LCG_A * (*s) + LCG_C;
    return *s;
}

/* Uniforme en [0,1): usa los 53 bits altos (los bits bajos de un LCG con
 * modulo 2^k tienen periodo corto y no deben usarse). */
static inline double lcg_uniforme(uint64_t *s) {
    return (double)(lcg_siguiente(s) >> 11) * 0x1.0p-53;
}

/* Salta k pasos: devuelve el estado tras aplicar k veces s <- a*s + c.
 * Composicion de transformaciones afines por cuadrados sucesivos (Brown, 1994). */
static uint64_t lcg_saltar(uint64_t s, uint64_t k) {
    uint64_t a_acum = 1, c_acum = 0;   /* transformacion acumulada */
    uint64_t a = LCG_A, c = LCG_C;     /* transformacion de 2^j pasos */
    while (k > 0) {
        if (k & 1ULL) {
            a_acum = a_acum * a;
            c_acum = c_acum * a + c;
        }
        c = (a + 1) * c;
        a = a * a;
        k >>= 1;
    }
    return a_acum * s + c_acum;
}

int main(int argc, char *argv[]) {
    long N = (argc > 1) ? atol(argv[1]) : 100000000L;
    int p = (argc > 2) ? atoi(argv[2]) : omp_get_max_threads();
    uint64_t semilla = (argc > 3) ? strtoull(argv[3], NULL, 10) : SEMILLA_BASE;

    if (N <= 0 || p <= 0) {
        fprintf(stderr, "Uso: %s <N> [hilos] [semilla]\n", argv[0]);
        return 1;
    }
    omp_set_num_threads(p);

    long nbloques = (N < NBLOQUES) ? N : NBLOQUES;
    long dentro = 0;

    double t0 = omp_get_wtime();

    #pragma omp parallel for reduction(+:dentro) schedule(dynamic,1) \
            default(none) shared(N, nbloques, semilla)
    for (long b = 0; b < nbloques; b++) {
        long inicio = (long)((__int128)N * b / nbloques);
        long fin    = (long)((__int128)N * (b + 1) / nbloques);
        uint64_t estado = lcg_saltar(semilla, 2ULL * (uint64_t)inicio);
        long locales = 0;
        for (long i = inicio; i < fin; i++) {
            double x = lcg_uniforme(&estado);
            double y = lcg_uniforme(&estado);
            if (x * x + y * y <= 1.0)
                locales++;
        }
        dentro += locales;
    }

    double t1 = omp_get_wtime();

    double pi_est = 4.0 * dentro / (double)N;
    double frac = dentro / (double)N;
    double sigma = 4.0 * sqrt(frac * (1.0 - frac) / (double)N); /* error estandar */

    printf("Estimacion de pi: %.6f\n", pi_est);
    printf("Error |pi_est - pi|: %.2e   (error estandar esperado: %.2e)\n",
           fabs(pi_est - M_PI), sigma);
    printf("Aciertos: %ld de %ld\n", dentro, N);
    printf("Hilos: %d   Tiempo: %.6f s\n", p, t1 - t0);
    return 0;
}
