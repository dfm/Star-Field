from pylab import *
import time

maxNumStars = 200
numHyperparams = 3 # Number of hyperparameters + 1 (staleness)

sample = atleast_2d(loadtxt('sample.txt'))
data = loadtxt('SimulatedData/sparse.txt')

params =   sample[:, 0:numHyperparams]
catalogs = sample[:, numHyperparams:numHyperparams+3*maxNumStars]
xStars = catalogs[:, 0:maxNumStars]
yStars = catalogs[:, maxNumStars:2*maxNumStars]
fStars = catalogs[:, 2*maxNumStars:]
images =   sample[:, numHyperparams+3*maxNumStars:]

ion()
hold(False)
for i in xrange(0, sample.shape[0]):
	mock = images[i,:].reshape(data.shape[0], data.shape[1])
	subplot(2,2,1)
	imshow(data)
	title('Data')
	gca().set_xticks([])
	gca().set_yticks([])
	ax = subplot(2,2,2)
	which = nonzero(fStars[i, :] > 0)[0]
	plot(xStars[i,which], yStars[i,which], 'k.', markersize=3)
	xlim([-1., 1.])
	ylim([-1., 1.])
	ax.set_aspect('equal')
	gca().set_xticks([])
	gca().set_yticks([])
	title('Catalog ' + str(i+1))
	subplot(2,2,3)
	imshow(mock)
	gca().set_xticks([])
	gca().set_yticks([])
	title('Mock Data ' + str(i+1))
	subplot(2,2,4)
	imshow(mock - data)
	gca().set_xticks([])
	gca().set_yticks([])
	title('Residuals')
	draw()
ioff()
show()

