import os
import numpy as np
import matplotlib.pyplot as pl
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
flux = np.linspace(0., 20., 10001)
F = CDF(flux)

# Directories
directories = ['../../sextractor/100', 'crowded']
counts = [63, 640]

# Catalog filenames
sigmas = ['0.5', '1.5', '2.5', '3.5', '4.5', '5.5', '6.5']
files = ['stars.' + sigma + '.cat' for sigma in sigmas]

pl.figure(figsize=(11, 4))
loc = [3, 5]

pl.subplots_adjust(wspace=0.3)
for i in xrange(0, 2):
    # Load catalogs
    catalogs = []
    for f in files:
        catalogs.append(np.loadtxt(os.path.join(directories[i], f)))

    # Convert sigmas to floats
    sigma = np.array([float(sig) for sig in sigmas])

    # Number of stars in each catalog
    N = np.array([cat.shape[0] for cat in catalogs])

    pl.subplot(1, 2, i + 1)
    pl.plot(sigma, N, 'ko-', mfc="#888888", mew=1.5, lw=2)
    pl.axhline(counts[i], color='k', linestyle='--', linewidth=2)
#    pl.plot(13*flux, 82*10**i*(1. - F), color='r', linewidth=2)
    pl.xlabel(r'Detection Threshold ($\sigma$)')
    pl.ylabel('Number of Stars $N$')
#    pl.xlim([0, 10])
    pl.ylim(0)
    pl.gca().xaxis.set_major_locator(MaxNLocator(4))
    pl.gca().yaxis.set_major_locator(MaxNLocator(4))
    pl.title('Test Case ' + str(i + 1))

pl.savefig('sex_N.eps', bbox_inches='tight')
pl.show()

