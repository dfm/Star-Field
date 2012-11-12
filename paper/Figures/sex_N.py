import os
import numpy as np
import matplotlib.pyplot as pl
from matplotlib.ticker import MaxNLocator

# Directories
directories = ['break2', 'crowded']

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
    pl.axhline(82 * 10 ** i, color='k', linestyle='--', linewidth=2)
    pl.xlabel(r'Detection Threshold ($\sigma$)')
    pl.ylabel('Number of Stars $N$')
    pl.ylim(0)
    pl.gca().xaxis.set_major_locator(MaxNLocator(4))
    pl.gca().yaxis.set_major_locator(MaxNLocator(4))
    pl.title('Test Case ' + str(i + 1))

pl.savefig('sex_N.eps', bbox_inches='tight')
