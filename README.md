# Integración Monte Carlo con OpenMP

Daniel Felipe Vidal · código 2202022
Introducción a la computación paralela (28661), UIS · Docente: Sergio Augusto Gélvez Cortés

Estimación de π por el método de acierto y fallo, paralelizada con OpenMP. La estimación
es idéntica para cualquier número de hilos: un generador congruencial de 64 bits con salto
hacia adelante hace que la muestra i use siempre los mismos números, sin importar qué hilo
la procese.

## Contenido

| Archivo | Qué es |
| --- | --- |
| `montecarlo.c` | Versión paralela (entregable principal) |
| `montecarlo_secuencial.c` | Versión secuencial del docente, con medición de tiempo |
| `medir.sh` | Protocolo de la Parte III: 5 corridas por p, mediana, speedup y eficiencia |
| `graficar.py` | Gráfica de S(p) frente a p con la recta ideal y la curva de Amdahl |
| `tabla.csv` | Medianas, speedup y eficiencia |
| `tiempos_crudos.csv` | Las 20 corridas individuales |
| `lscpu.txt` | Procesador donde se midió |
| `speedup.png` | Gráfica de speedup |
| `pruebas/test_salto.c` | Verifica el salto del generador contra la generación paso a paso |
| `pruebas/rand_compartido.c` | Error intencional: `rand()` compartido entre hilos (II.3) |
| `pruebas/misma_semilla.c` | Error intencional: misma semilla en todos los hilos (II.3) |
| `pruebas/carrera_acumulador.c` | Error intencional: acumulador sin `reduction` (II.4) |
| `pruebas/montecarlo_dynamic.c` | Variante con `schedule(dynamic)` (II.5) |

## Compilar y ejecutar

    gcc -O2 -fopenmp montecarlo.c -o montecarlo -lm
    ./montecarlo <N> [hilos] [semilla]
    ./montecarlo 100000000 4

Sin argumentos usa N = 10^8, todos los hilos disponibles y la semilla 2202022.

## Reproducir las mediciones

    ./medir.sh 100000000 5 "1 2 4 8"
    python3 graficar.py

## Resultados

Intel Core i5-8300H @ 2,30 GHz, 4 núcleos físicos y 8 hilos lógicos, Ubuntu en WSL 2, N = 10^8.

| p | Mediana de T(p) [s] | S(p) | E(p) |
| --- | --- | --- | --- |
| 1 | 0,2315 | 1,000 | 1,000 |
| 2 | 0,1286 | 1,800 | 0,900 |
| 4 | 0,0985 | 2,350 | 0,588 |
| 8 | 0,0730 | 3,171 | 0,396 |

Estimación: π ≈ 3,141612 (78 540 300 aciertos) con 1, 2, 4 y 8 hilos.

## Uso de inteligencia artificial

Se usó Claude (Anthropic) como asistente. El detalle de cada consulta, lo aceptado y lo
descartado está en la bitácora anexa al informe.
