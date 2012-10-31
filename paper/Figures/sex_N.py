import numpy as np
import matplotlib.pyplot as plt

# Directories
directories = ['break2', 'crowded']

# Catalog filenames
sigmas = ['0.5', '1.5', '2.5', '3.5', '4.5', '5.5', '6.5']
files = ['stars.' + sigma + '.cat' for sigma in sigmas]

plt.figure(figsize=(16, 6))
loc=[3, 5]

for i in xrange(0, 2):
	# Load catalogs
	catalogs = []
	for f in files:
		catalogs.append(np.loadtxt(directories[i] + '/' + f))

	# Convert sigmas to floats
	sigma = np.array([float(sig) for sig in sigmas])

	# Number of stars in each catalog
	N = np.array([cat.shape[0] for cat in catalogs])

	plt.subplot(1,2,i+1)
	plt.plot(sigma, N, 'bo-', label='SExtractor Inference')
	plt.axhline(82*10**i, color='k', linestyle='--', linewidth=2, label='True Value')
	plt.xlabel('Cutoff (number of $\\sigma$s)', fontsize=15)
	plt.ylabel('Number of Stars $N$', fontsize=15)
	plt.ylim(0)
	plt.legend(loc=loc[i])
	plt.title('Test Case ' + str(i+1), fontsize=15)

plt.#savefig('sex_N.eps', bbox_inches='tight')
plt.show()

