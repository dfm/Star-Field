import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

numHyperparams = 11

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
_flux = 10.**(np.linspace(-2., 2., 5001))
F = np.empty(_flux.shape)

names = ['100', '1000']
titles = ['Test Case 1', 'Test Case 2']
sex_files = ['3.5']
counts = [63, 640]

plt.figure(figsize=(14, 6))

## Plot true cumulative LF
#for i in xrange(0, 1):
#	plt.subplot(1,2,i+1)
#	plt.loglog(_flux, (1. - _F)*counts[i], 'g-', linewidth=1, label='Smooth')







# Plot inference empirical cumulative LF
for i in xrange(0, 2):
	sample = np.loadtxt('../../Runs/' + names[i] + '/posterior_sample.txt')

	# Dumb hack because the data had 2000 maxNumStars
	# but inference was run with 1200
	if i==0:
		maxNumStars = 200
	if i==1:
		maxNumStars = 1200

	plt.subplot(1,2,i+1)
	plt.hold(True)
	theLabel = 'Inference'
	for k in xrange(0, 20):
		if k > 0:
			theLabel = None
		params =   sample[k, 0:numHyperparams]
		catalogs = sample[k, numHyperparams:numHyperparams+3*maxNumStars]
		xStars = catalogs[0:maxNumStars]
		yStars = catalogs[maxNumStars:2*maxNumStars]
		fStars = catalogs[2*maxNumStars:]
		fStars = fStars[np.logical_and(np.abs(xStars) < 1., np.abs(yStars) < 1.)]

		flux = fStars[fStars > 0.]
		for j in xrange(0, _flux.size):
			F[j] = np.sum(flux >= _flux[j])

		plt.loglog(_flux, F, 'r', alpha=0.25, label=theLabel)

	plt.xlabel('$x$', fontsize=15)
	plt.ylabel('Number (Flux > $x$)', fontsize=15)
	plt.title(titles[i], fontsize=16)
	plt.legend(numpoints=1, loc='lower left')
	#plt.xlim([0.01, 10.])

	if i==0:
		plt.ylim([0.6667, 150.])
	elif i==1:
		plt.ylim([0.6667, 1500.])

# Plot SExtractor empirical cumulative LF
for i in xrange(0, 2):
	for j in xrange(0, len(sex_files)):
		catalog = np.loadtxt('../../sextractor/' + names[i] + '/stars.' + sex_files[j] + '.cat')

		flux = catalog[:,2]*0.0004
		for j in xrange(0, _flux.size):
			F[j] = np.sum(flux >= _flux[j])

		plt.subplot(1,2,i+1)
		plt.loglog(_flux, F, 'b--', linewidth=3, label='SExtractor')
#	plt.xlim([0.01, 10.])

# Plot data empirical cumulative LF
for i in xrange(0, 2):
	sample = np.loadtxt('../../c++_dnest/SimulatedData/' + names[i] + '_ground_truth.txt')

	# Dumb hack because the data had 2000 maxNumStars
	# but inference was run with 1200
	if i==0:
		maxNumStars = 200
	if i==1:
		maxNumStars = 2000

	params =   sample[0:numHyperparams]
	catalogs = sample[numHyperparams:numHyperparams+3*maxNumStars]
	xStars = catalogs[0:maxNumStars]
	yStars = catalogs[maxNumStars:2*maxNumStars]
	fStars = catalogs[2*maxNumStars:]
	fStars = fStars[np.logical_and(np.abs(xStars) < 1., np.abs(yStars) < 1.)]

	flux = fStars[fStars > 0.]
	for j in xrange(0, _flux.size):
		F[j] = np.sum(flux >= _flux[j])

	plt.subplot(1,2,i+1)
	plt.loglog(_flux, F, 'k', linewidth=3, label='True')

	plt.xlabel('$x$', fontsize=15)
	plt.ylabel('Number (Flux > $x$)', fontsize=15)
	plt.title(titles[i], fontsize=16)
	plt.legend(numpoints=1, loc='lower left')
	if i==0:
		plt.ylim([0.6667, 150.])
	elif i==1:
		plt.ylim([0.6667, 1500.])
	if i==0:
		plt.xlim([0.1, 3.])
	elif i==1:
		plt.xlim([0.1, 25.])	

plt.savefig('luminosity_function.eps', bbox_inches='tight', dpi=600)
plt.savefig('luminosity_function.png', bbox_inches='tight', dpi=600)
plt.show()

