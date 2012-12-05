import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

# CDF of p(flux|hyperparameters)
def CDF(flux, params=[0.3, 0.6, 1.1, 2.]):
	h1, h2, a1, a2 = params[0], params[1], params[2], params[3]
	# Normalisation constants (from appendix)
	Z1 = (h1**(-a1) - h2**(-a1))/a1 + h2**(-a1)/a2
	Z2 = Z1*h2**(a1 - a2)
	F = np.zeros(flux.shape)
	subset = np.logical_and(flux > h1, flux <= h2)
	F[subset] = (Z1*a1)**(-1)*(h1**(-a1) - flux[subset]**(-a1))
	subset = flux > h2
	F[subset] = 1. - (Z2*a2)**(-1)*flux[subset]**(-a2)
	return F

# Compute CDF
_flux = np.linspace(0., 20., 10001)
_F = CDF(_flux)

catalog = np.loadtxt('../../dolphot/crowded.txt')

flux = np.sort(catalog[:,2]*0.0004)
dm = 1./flux.size

mass = np.linspace(0.5*dm, 1. - 0.5*dm, flux.size)

plt.loglog(flux, (1. - mass)*flux.size, 'b.')
plt.loglog(_flux, (1. - _F)*820, 'r')
plt.show()


