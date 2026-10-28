#!/usr/bin/env bash
# medir.sh -- protocolo experimental de la Parte III.
# Uso: ./medir.sh [N] [repeticiones] [lista de hilos]
# Ejemplo: ./medir.sh 100000000 5 "1 2 4 8"
# Salida: tiempos_crudos.csv (todas las corridas) y tabla.csv (medianas, S, E)
set -euo pipefail

N=${1:-100000000}
REP=${2:-5}
HILOS=${3:-"1 2 4 8"}

gcc -O2 -fopenmp -Wall -Wextra montecarlo.c -o montecarlo -lm
lscpu > lscpu.txt

echo "p,corrida,tiempo_s,estimacion" > tiempos_crudos.csv
for p in $HILOS; do
  ./montecarlo "$N" "$p" > /dev/null   # corrida de calentamiento (no se registra)
  for r in $(seq 1 "$REP"); do
    salida=$(./montecarlo "$N" "$p")
    t=$(echo "$salida" | sed -n 's/.*Tiempo: \([0-9.]*\) s/\1/p')
    pi=$(echo "$salida" | sed -n 's/Estimacion de pi: \([0-9.]*\)/\1/p')
    echo "$p,$r,$t,$pi" >> tiempos_crudos.csv
    echo "p=$p corrida=$r t=$t pi=$pi"
  done
done

python3 - <<'EOF'
import csv, statistics
filas = list(csv.DictReader(open("tiempos_crudos.csv")))
ps = sorted({int(f["p"]) for f in filas})
med = {p: statistics.median(float(f["tiempo_s"]) for f in filas if int(f["p"]) == p) for p in ps}
est = {f["estimacion"] for f in filas}
T1 = med[ps[0]]
with open("tabla.csv", "w") as out:
    out.write("p,mediana_s,speedup,eficiencia\n")
    print(f"\n{'p':>3} {'mediana[s]':>11} {'S(p)':>7} {'E(p)':>7}")
    for p in ps:
        S = T1 / med[p]; E = S / p
        out.write(f"{p},{med[p]:.6f},{S:.4f},{E:.4f}\n")
        print(f"{p:>3} {med[p]:>11.4f} {S:>7.3f} {E:>7.3f}")
print("\nEstimaciones distintas observadas:", est, "(debe ser una sola)")
EOF
