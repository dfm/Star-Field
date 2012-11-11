import plot_utils
import numpy as np
import matplotlib.pyplot as pl


x = np.linspace(0., 2., 1001)
F = np.zeros(x.shape)
logf = np.zeros(x.shape)

x1 = 0.3
x2 = 0.6

a1 = 1.1
a2 = 2.0

Z1 = (x1 ** (-a1) - x2 ** (-a1)) / a1 + x2 ** (-a1) / a2
Z2 = Z1 * x2 ** (a1 - a2)

which = np.nonzero(x <= x1)[0]
F[which] = 0.
logf[which] = -1E300

which = np.nonzero(np.logical_and(x > x1, x < x2))[0]
F[which] = (x1 ** (-a1) - x[which] ** (-a1)) / (Z1 * a1)
logf[which] = -np.log(Z1) - (a1 + 1) * np.log(x[which])

which = np.nonzero(x >= x2)[0]
F[which] = 1. - x[which] ** (-a2) / (Z2 * a2)
logf[which] = -np.log(Z2) - (a2 + 1) * np.log(x[which])

f = np.exp(logf)
f = f / np.trapz(f, x=x)
print(np.trapz(f * (x < x2), x=x))

fig = pl.figure(figsize=(12, 4))

fig.add_axes([0.12, 0.1, 0.35, 0.85])
pl.plot(x, f, 'k', linewidth=2, label='Probability Density')

pl.xlabel(r'$x$')
pl.ylabel(r'$p(x)$')

bp = np.exp(-np.log(Z2) - (a2 + 1) * np.log(x2))
pl.plot([x2, x2], [0, bp], "--k")
pl.gca().annotate(r"$h_2$",
                  xy=(x2, bp), xycoords="data",
                  xytext=(70, 20), textcoords="offset points",
                  arrowprops=dict(arrowstyle="->", connectionstyle="arc3"),
                 )

mx = f.max()
pl.gca().axvline(x1, color="k", ls="dashed")
pl.gca().annotate(r"$h_1$",
                  xy=(x1, mx + 0.5), xycoords="data",
                  xytext=(70, 0), textcoords="offset points", va="center",
                  arrowprops=dict(arrowstyle="->", connectionstyle="arc3"),
                 )

pl.xlim([0, 2])
pl.ylim([0, 1.3 * f.max()])


#
# LOG-LOG VERSION.
#

lx, lf = np.log10(x[f > 0]), np.log10(f[f > 0])
lx = np.append([np.log10(x1), np.log10(x1)], lx)
lf = np.append([-1.5, np.log10(mx)], lf)

fig.add_axes([0.6, 0.1, 0.35, 0.85])
pl.plot(lx, lf, 'k', linewidth=2, label='Probability Density')
pl.xlabel(r'$\log_{10} \, x$')
pl.ylabel(r'$\log_{10}\,p(x)$')

pl.plot(np.log10([x2, x2]), [-1.5, np.log10(bp)], "--k")
pl.gca().annotate(r"$h_2$",
                  xy=np.log10([x2, bp]), xycoords="data",
                  xytext=(70, 20), textcoords="offset points",
                  arrowprops=dict(arrowstyle="->", connectionstyle="arc3"),
                 )

pl.gca().axvline(np.log10(x1), color="k", ls="dashed")
pl.gca().annotate(r"$h_1$",
                  xy=(np.log10(x1), np.log10(mx) + 0.1), xycoords="data",
                  xytext=(70, 0), textcoords="offset points", va="center",
                  arrowprops=dict(arrowstyle="->", connectionstyle="arc3"),
                 )

pl.ylim(-1.3, 1)

pl.savefig('broken.eps', bbox_inches='tight')
