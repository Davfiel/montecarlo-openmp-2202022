"""graficar.py -- grafica S(p) vs p con la recta ideal y la curva de Amdahl.
Uso: python3 graficar.py            (lee tabla.csv, escribe speedup.png)
"""
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

filas = list(csv.DictReader(open("tabla.csv")))
p = [int(f["p"]) for f in filas]
S = [float(f["speedup"]) for f in filas]

# f estimada a partir del mayor p medido que no exceda los nucleos fisicos;
# ajuste P_REF al valor que use en III.2 (el enunciado pide p = 4).
P_REF = 4 if 4 in p else p[-1]
S_ref = S[p.index(P_REF)]
f = (1 - 1 / S_ref) / (1 - 1 / P_REF)

pp = list(range(1, max(p) + 1))
amdahl = [1 / ((1 - f) + f / x) for x in pp]

AZUL, NARANJA, GRIS, TEXTO = "#2a78d6", "#eb6834", "#8a8a85", "#3d3d3a"
fig, ax = plt.subplots(figsize=(6.4, 4.4), dpi=150)
ax.plot(pp, pp, color=GRIS, lw=1.5, ls="--", label="Ideal lineal S(p) = p")
ax.plot(pp, amdahl, color=NARANJA, lw=2, label=f"Amdahl con f = {f:.3f}")
ax.plot(p, S, color=AZUL, lw=2, marker="o", ms=7,
        markeredgecolor="white", markeredgewidth=1.5, label="Medido (mediana de 5)")
for x, y in zip(p, S):
    ax.annotate(f"{y:.2f}", (x, y), textcoords="offset points", xytext=(6, -12),
                fontsize=9, color=TEXTO)
ax.set_xlabel("Número de hilos p", color=TEXTO)
ax.set_ylabel("Speedup S(p) = T(1)/T(p)", color=TEXTO)
ax.set_title("Speedup de la integración Monte Carlo (N = 10^8)", color=TEXTO, fontsize=11)
ax.set_xticks(p)
ax.set_xlim(0.5, max(p) + 0.5)
ax.set_ylim(0, max(p) + 0.5)
ax.grid(True, color="#e4e3dd", lw=0.8)
for lado in ("top", "right"):
    ax.spines[lado].set_visible(False)
ax.legend(frameon=False, fontsize=9, loc="upper left")
fig.tight_layout()
fig.savefig("speedup.png")
print(f"f estimada con p={P_REF}: {f:.4f}  ->  speedup.png")
