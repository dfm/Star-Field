from pylab import *
import time, os

saveFrames = True # For making movies
if saveFrames:
	os.system('rm Frames/*.png')

maxNumStars = 200
numHyperparams = 6	# Number of parameters before catalog begins
			# maxNumStars, staleness, hyperparameters

sample = atleast_2d(loadtxt('posterior_sample.txt'))
data = loadtxt('SimulatedData/sparse.txt')

params =   sample[:, 0:numHyperparams]
catalogs = sample[:, numHyperparams:numHyperparams+3*maxNumStars]
xStars = catalogs[:, 0:maxNumStars]
yStars = catalogs[:, maxNumStars:2*maxNumStars]
fStars = catalogs[:, 2*maxNumStars:]
images =   sample[:, numHyperparams+3*maxNumStars:]

xCatalog = np.array([])
yCatalog = np.array([])
fCatalog = np.array([])

mock_data_mean = np.zeros(data.shape)

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
	plot(xStars[i,which], yStars[i,which], 'k.', markersize=1)
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

	mock_data_mean += mock/sample.shape[0]

	xCatalog = hstack([xCatalog, xStars[i,which]])
	yCatalog = hstack([yCatalog, yStars[i,which]])
	fCatalog = hstack([fCatalog, fStars[i,which]])

	print(i+1, ((((data - mock)/30.)**2).sum()/10000. - 1.)/(sqrt(2.)/100.))

	if saveFrames:
		savefig('Frames/' + '%0.4d'%(i+1) + '.png', bbox_inches='tight')
		print('Frames/' + '%0.4d'%(i+1) + '.png')
ioff()
show()

subplot(1,3,1)
imshow(data, extent=(-1, 1, -1, 1), cmap='gray')
title('Data')
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
xlabel('$x$')
ylabel('$y$')

subplot(1,3,2)
imshow(mock_data_mean, extent=(-1, 1, -1, 1), cmap='gray')
title('Denoised')
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
xlabel('$x$')
ylabel('$y$')


subplot(1,3,3)
img = histogram2d(xCatalog, yCatalog, bins=200, weights=fCatalog)[0].T[::-1]
img = img/img.max()
imshow(img**0.25, extent=(-1.1, 1.1, -1.1, 1.1), cmap='gray')
title('Denoised and Deblurred')
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
xlabel('$x$')
ylabel('$y$')

show()

