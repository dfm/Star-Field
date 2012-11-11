import plot_utils
import numpy as np
import matplotlib.pyplot as pl


x = np.linspace(0., 1000., 1000001)
F = np.zeros(x.shape)
logf = np.zeros(x.shape)

x1 = 0.3
x2 = 0.6

a1 = 1.1
a2 = 2.0

Z1 = (x1**(-a1) - x2**(-a1))/a1 + x2**(-a1)/a2
Z2 = Z1*x2**(a1 - a2)

which = np.nonzero(x <= x1)[0]
F[which] = 0.
logf[which] = -1E300

which = np.nonzero(np.logical_and(x > x1, x < x2))[0]
F[which] = (x1**(-a1) - x[which]**(-a1))/(Z1*a1)
logf[which] = -np.log(Z1) -(a1 + 1)*np.log(x[which])

which = np.nonzero(x >= x2)[0]
F[which] = 1. - x[which]**(-a2)/(Z2*a2)
logf[which] = -np.log(Z2) -(a2 + 1)*np.log(x[which])

f = np.exp(logf)
f = f / np.trapz(f, x=x)
print(np.trapz(f*(x < x2), x=x))

pl.plot(x, f, 'k', linewidth=2, label='Probability Density')
pl.xlabel('$x$')
pl.ylabel('Probability Density')
pl.xlim([0, 2])
pl.ylim([0, 1.5*f.max()])
pl.plot([0.3, 0.3], [0., 1.5*f.max()], 'r--', linewidth=2, label='Lower Limit $h_1$')
pl.plot([0.6, 0.6], [0., 1.5*f.max()], 'g:', linewidth=2, label='Break Point $h_2$')
pl.legend()
pl.savefig('broken.eps', bbox_inches='tight')
