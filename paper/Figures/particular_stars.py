# Investigate the inference about a particular star
# (actually, the flux within a little circle)

import numpy as np
import matplotlib.pyplot as plt

numHyperparams = 11
maxNumStars = 200

# Load the truth and get catalog out
ground_truth = np.loadtxt('../../c++_dnest/SimulatedData/100_ground_truth.txt')
params =   ground_truth[0:numHyperparams]
catalogs = ground_truth[numHyperparams:numHyperparams+3*maxNumStars]
xStars = catalogs[0:maxNumStars]
yStars = catalogs[maxNumStars:2*maxNumStars]
fStars = catalogs[2*maxNumStars:]

# Get rid of zero-padded (nonexistent) stars
xStars = xStars[fStars > 0.]
yStars = yStars[fStars > 0.]
fStars = fStars[fStars > 0.]

# Only look at stars within image
good = np.logical_and(np.abs(xStars) < 1., np.abs(yStars) < 1.)
xStars = xStars[good]
yStars = yStars[good]
fStars = fStars[good]

# Pick a bright star
ranks = np.argsort(fStars)
which = np.nonzero(ranks == int(0.4*fStars.size))[0]

# Make a little circular aperture around it
xc = xStars[which]
yc = yStars[which]
radius = 0.04

# For plotting the circle
theta = np.linspace(0., 2.*np.pi, 1001)
xx = xc + radius*np.cos(theta)
yy = yc + radius*np.sin(theta)

# Plot the stars and the circular aperture
plt.plot(xStars, yStars, 'k.')
plt.plot(xx, yy, 'k', linewidth=1)
plt.axis('scaled')
#plt.show()

# Check number of stars within the circle
rsq = (xStars - xc)**2 + (yStars - yc)**2
within = rsq < radius**2
true_flux = np.sum(fStars[within])

# Load posterior samples
posterior_sample = np.loadtxt('../../Runs/100/posterior_sample.txt')

posterior_flux = np.empty(posterior_sample.shape[0])
for i in xrange(0, posterior_sample.shape[0]):
	params =   posterior_sample[i, 0:numHyperparams]
	catalogs = posterior_sample[i, numHyperparams:numHyperparams+3*maxNumStars]
	xStars = catalogs[0:maxNumStars]
	yStars = catalogs[maxNumStars:2*maxNumStars]
	fStars = catalogs[2*maxNumStars:]

	# Get rid of zero-padded (nonexistent) stars
	xStars = xStars[fStars > 0.]
	yStars = yStars[fStars > 0.]
	fStars = fStars[fStars > 0.]

	# Check number of stars within the circle
	rsq = (xStars - xc)**2 + (yStars - yc)**2
	within = rsq < radius**2
	posterior_flux[i] = np.sum(fStars[within])
	print((i, within.sum()))



# Load SExtractor catalog
sex_catalog = np.loadtxt('../../sextractor/100/stars.2.5.cat')
xSex =  sex_catalog[:,5]*0.02 - 1. - 0.01
ySex = -(sex_catalog[:,6]*0.02 - 1.) + 0.01
fSex = sex_catalog[:,1]*0.0004/0.69666
fErrSex = sex_catalog[:,2]*0.0004/0.69666
plt.plot(xSex, ySex, 'bo', alpha=0.5)
plt.axis('scaled')
plt.show()

rsq = (xSex - xc)**2 + (ySex - yc)**2
within = rsq < radius**2
sex_flux = fSex[within].sum()
sex_fErr = np.sqrt((fErrSex[within]**2).sum())

plt.hist(posterior_flux, 30, alpha=0.3, normed=True)
plt.axvline(true_flux, color='k', linewidth=3, label='Inference')
plt.axvline(sex_flux, color='r', linestyle='--', linewidth=3, label='SExtractor')
plt.axvline(sex_flux - fErrSex[within], color='r', linestyle='--', linewidth=1)
plt.axvline(sex_flux + fErrSex[within], color='r', linestyle='--', linewidth=1)
plt.xlabel('Flux', fontsize=16)
plt.ylabel('Posterior PDF', fontsize=16)
plt.legend()
plt.show()

