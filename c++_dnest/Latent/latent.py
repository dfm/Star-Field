import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

def transform(u, frac_on=0.5, mu=1.0):
	"""
	Turn a uniform distribution of latent variables
	into an actual distribution of fluxes
	"""
	f = np.zeros(u.shape)
	which = np.nonzero(u > frac_on)[0]
	f[which] = -mu*np.log((u[which] - frac_on)/(1. - frac_on))
	return f

if __name__ == '__main__':
	"""
	Simple main for testing
	"""
	N = 10
	u = rng.rand(N)
	f = transform(u)

	# Make some uniform positions
	x = rng.rand(N)
	y = rng.rand(N)
	img = np.histogram2d(x, y, weights=f, bins=100)[0]
	plt.imshow(img)
	plt.show()


