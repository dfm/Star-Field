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
_flux = np.linspace(0., 20., 10001)
_F = CDF(_flux)

names = ['100', '1000']
titles = ['Test Case 1', 'Test Case 2']
counts = [63, 640]

plt.figure(figsize=(14, 6))

## Plot true cumulative LF
#for i in xrange(0, 1):
#	plt.subplot(1,2,i+1)
#	plt.loglog(_flux, (1. - _F)*counts[i], 'g-', linewidth=1, label='Smooth')


# Plot SExtractor empirical cumulative LF
for i in xrange(0, 2):
	catalog = np.loadtxt('../../sextractor/' + names[i] + '/stars.4.5.cat')

	flux = np.sort(catalog[:,2]*0.0004)
	dm = 1./flux.size

	mass = np.linspace(0.5*dm, 1. - 0.5*dm, flux.size)

	plt.subplot(1,2,i+1)
	plt.loglog(flux, (1. - mass)*flux.size, 'k*', markersize=5, alpha=0.3, label='SExtractor')
#	plt.xlim([0.01, 10.])
	if i==0:
		plt.ylim([0.6667, 150.])
	elif i==1:
		plt.ylim([0.6667, 1500.])

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

	flux = np.sort(fStars[fStars > 0.])
	dm = 1./flux.size

	mass = np.linspace(0.5*dm, 1. - 0.5*dm, flux.size)
	print(flux.size)

	plt.subplot(1,2,i+1)
	plt.loglog(flux, (1. - mass)*flux.size, 'bo', markersize=5, alpha=0.3, label='True')
	plt.xlabel('$x$', fontsize=15)
	plt.ylabel('Number (Flux > $x$)', fontsize=15)
	plt.title(titles[i], fontsize=16)
	plt.legend(numpoints=1, loc='lower left')
	if i==0:
		plt.ylim([0.6667, 150.])
	elif i==1:
		plt.ylim([0.6667, 1500.])

# Plot inference empirical cumulative LF
for i in xrange(0, 2):
	sample = np.loadtxt('../../Runs/' + names[i] + '/posterior_sample.txt')

	# Dumb hack because the data had 2000 maxNumStars
	# but inference was run with 1200
	if i==0:
		maxNumStars = 200
	if i==1:
		maxNumStars = 1200

	params =   sample[0, 0:numHyperparams]
	catalogs = sample[0, numHyperparams:numHyperparams+3*maxNumStars]
	xStars = catalogs[0:maxNumStars]
	yStars = catalogs[maxNumStars:2*maxNumStars]
	fStars = catalogs[2*maxNumStars:]
	fStars = fStars[np.logical_and(np.abs(xStars) < 1., np.abs(yStars) < 1.)]

	flux = np.sort(fStars[fStars > 0.])
	dm = 1./flux.size

	mass = np.linspace(0.5*dm, 1. - 0.5*dm, flux.size)
	print(flux.size)

	plt.subplot(1,2,i+1)
	plt.loglog(flux, (1. - mass)*flux.size, 'ro', markersize=5, alpha=0.3, label='Inference')
	plt.xlabel('$x$', fontsize=15)
	plt.ylabel('Number (Flux > $x$)', fontsize=15)
	plt.title(titles[i], fontsize=16)
	plt.legend(numpoints=1, loc='lower left')
	plt.xlim([0.01, 10.])

	if i==0:
		plt.ylim([0.6667, 150.])
	elif i==1:
		plt.ylim([0.6667, 1500.])		

plt.savefig('luminosity_function.eps', bbox_inches='tight', dpi=600)
plt.show()

