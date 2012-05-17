import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

def transform(u, frac_on=0.5, mu=1.0):
	"""
	Turn a uniform distribution of latent variables
	into an actual distribution of fluxes
	"""
	f = np.zeros(u.shape)
	t = 1. - frac_on
	which = np.nonzero(u > t)[0]
	U = (u[which] - t)/frac_on
	f[which] = -mu*np.log(1. - U) # exponential
	#f[which] = 1.*(1. - U)**(-1./mu) # Pareto
	return f

if __name__ == '__main__':
	"""
	Simple main for testing
	"""
	u = np.linspace(0, 1, 1001)
	plt.plot(u, transform(u, frac_on = 0.75, mu=1.), linewidth=2)
	plt.title('Transformation', fontsize=14)
	plt.xlabel('Latent Uniform Variable', fontsize=14)
	plt.ylabel('Flux of Star', fontsize=14)
	plt.show()

	N = 1000
	u = rng.rand(N)
	f = transform(u, frac_on=0.75, mu=2.)

	# Make some uniform positions
	x = rng.rand(N)
	y = rng.rand(N)
	#img = np.histogram2d(x, y, weights=f, bins=100)[0]
	#plt.imshow(img)
	plt.hist(f[np.nonzero(f > 0.)[0]], 20)
	print(f[np.nonzero(f > 0.)[0]].mean())
	plt.show()


