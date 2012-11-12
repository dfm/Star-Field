import plot_utils
import matplotlib.pyplot as pl
from matplotlib.ticker import MaxNLocator
import numpy as np
import time, os

maxNumStars = 1200
numHyperparams = 6  # Number of parameters before catalog begins
                    # maxNumStars, staleness, hyperparameters

sample = np.atleast_2d(np.loadtxt('posterior_sample.txt'))
data = np.loadtxt('../../c++_dnest/SimulatedData/crowded.txt')

#sample = sample[::2, :]

fig = pl.figure(figsize=(22, 5))

ax = fig.add_axes([0.1, 0.1, 0.2, 0.85])
n, b, p = plot_utils.hist(sample[:, 0], 20)
ax.set_ylim(0, n.max() * 1.1)
ax.axvline(1000, color='k', linewidth=2)
ax.set_xlabel('Number of Stars ($N$)')
ax.set_ylabel('Posterior Probability')
ax.set_yticklabels([])
ax.xaxis.set_major_locator(MaxNLocator(4))

ax = fig.add_axes([0.35, 0.1, 0.2, 0.85])
ax.plot(sample[:, 2], sample[:, 3], 'k.', markersize=3)
ax.plot(0.3, 0.6, "rs", markersize=8)
ax.set_xlabel('$h_1$')
ax.set_ylabel('$h_2$')
ax.set_xlim([0.19, 0.51])
ax.xaxis.set_major_locator(MaxNLocator(4))
ax.yaxis.set_major_locator(MaxNLocator(4))

ax = fig.add_axes([0.6, 0.1, 0.2, 0.85])
ax.plot(sample[:, 4], sample[:, 5], 'k.', markersize=3)
ax.plot(1.1, 2, "rs", markersize=6)
ax.set_xlabel(r'$\alpha_1$')
ax.set_ylabel(r'$\alpha_2$')
ax.set_xlim([-0.01, 5.01])
ax.xaxis.set_major_locator(MaxNLocator(4))
ax.yaxis.set_major_locator(MaxNLocator(4))

pl.savefig('inference.eps', bbox_inches='tight')
assert 0

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

k = 1
figure(figsize=(12, 12))
subplots_adjust(hspace=0.02, wspace=0.02)
for i in range(0, 9):
	mock = images[i,:].reshape(data.shape[0], data.shape[1])
	ax = subplot(3,3,k)
	which = nonzero(fStars[i, :] > 0)[0]
	plot(xStars[i,which], yStars[i,which], 'k.', markersize=1)
	axis('equal')
	xlim([-1., 1.])
	ylim([-1., 1.])
	gca().set_xticks([])
	gca().set_yticks([])
	if k==2:
		title('Catalogs', fontsize=14)
	k += 1

	mock_data_mean += mock/sample.shape[0]

	xCatalog = hstack([xCatalog, xStars[i,which]])
	yCatalog = hstack([yCatalog, yStars[i,which]])
	fCatalog = hstack([fCatalog, fStars[i,which]])

	print(i+1, ((((data - mock)/30.)**2).sum()/10000. - 1.)/(sqrt(2.)/100.))

##savefig('catalogs.eps', bbox_inches='tight')
show()


#subplot(1,3,1)
#img = (data - data.min())/(data.max() - data.min())
#imshow(img**0.5, extent=(-1, 1, -1, 1), cmap='jet')
#title('Data: A Crowded Image')
#axis([-1, 1, -1, 1])
#gca().set_xticks([-1, 0, 1])
#gca().set_yticks([-1, 0, 1])
#xlabel('$x$')
#ylabel('$y$')

figure(figsize=(22, 5))
subplot(1,3,1)

which1 = logical_and(xCatalog > -1., xCatalog < 1.)
which2 = logical_and(yCatalog > -1., yCatalog < 1.)
which = logical_and(which1, which2)

img = histogram2d(xCatalog[which], yCatalog[which], bins=200, weights=fCatalog[which])[0].T[::-1]
img = (img - img.min())/(img.max() - img.min())
imshow(img**0.25, extent=(-1, 1, -1, 1), cmap='jet')
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
title('Expected True Scene', fontsize=16)
#axis([-1, 1, -1, 1])
#gca().set_xticks([-1, 0, 1])
#gca().set_yticks([-1, 0, 1])
xlabel('$x$', fontsize=20)
ylabel('$y$', fontsize=20)

subplot(1,3,2)
img = (mock_data_mean - mock_data_mean.min())/(mock_data_mean.max() - mock_data_mean.min())
imshow(img**0.5, extent=(-1, 1, -1, 1), cmap='jet')
title('Expected Noise-Free Scene', fontsize=16)
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
xlabel('$x$', fontsize=20)
ylabel('$y$', fontsize=20)

subplot(1,3,3)
img = mock_data_mean - data
img = (img - img.min())/(img.max() - img.min())
imshow(img, extent=(-1, 1, -1, 1), cmap='jet')
title('Residuals', fontsize=16)
axis([-1, 1, -1, 1])
gca().set_xticks([-1, 0, 1])
gca().set_yticks([-1, 0, 1])
xlabel('$x$', fontsize=20)
ylabel('$y$', fontsize=20)
#savefig('summaries.eps', bbox_inches='tight')

show()

