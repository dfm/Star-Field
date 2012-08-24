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

subplot(2,1,1)
loglog(x, 1-F, 'b')
xlabel('x')
ylabel('P(>x)')
xlim([1, 100])

subplot(2,1,2)
plot(x, f, 'b')
xlabel('x')
ylabel('Probability Density')
xlim([0, 20])

show()


