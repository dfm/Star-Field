from pylab import *

x = linspace(0., 1000., 1000001)
F = zeros(x.shape)
logf = zeros(x.shape)

x1 = 0.3
x2 = 0.6

a1 = 1.1
a2 = 2.0

Z1 = (x1**(-a1) - x2**(-a1))/a1 + x2**(-a1)/a2
Z2 = Z1*x2**(a1 - a2)

which = nonzero(x <= x1)[0]
F[which] = 0.
logf[which] = -1E300

which = nonzero(logical_and(x > x1, x < x2))[0]
F[which] = (x1**(-a1) - x[which]**(-a1))/(Z1*a1)
logf[which] = -log(Z1) -(a1 + 1)*log(x[which])

which = nonzero(x >= x2)[0]
F[which] = 1. - x[which]**(-a2)/(Z2*a2)
logf[which] = -log(Z2) -(a2 + 1)*log(x[which])

f = exp(logf)
f = f/trapz(f, x=x)
print(trapz(f*(x < x2), x=x))

plot(x, f, 'b', linewidth=2, label='Probability Density')
xlabel('$x$', fontsize=14)
ylabel('Probability Density', fontsize=14)
xlim([0, 2])
ylim([0, 1.5*f.max()])
plot([0.3, 0.3], [0., 1.5*f.max()], 'r--', linewidth=2, label='Lower Limit $h_1$')
plot([0.6, 0.6], [0., 1.5*f.max()], 'g--', linewidth=2, label='Break Point $h_2$')
legend()
gca().tick_params(labelsize=14)
#savefig('broken.eps')
show()


