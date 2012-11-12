import plot_utils
import matplotlib.pyplot as pl
from matplotlib.ticker import MaxNLocator
import numpy as np

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
ax.plot(sample[:, 2], sample[:, 3], '.', color="#888888", mec="#888888",
        markersize=3)
ax.plot(0.3, 0.6, "rs", markersize=8)
ax.set_xlabel('$h_1$')
ax.set_ylabel('$h_2$')
ax.set_xlim([0.19, 0.51])
ax.xaxis.set_major_locator(MaxNLocator(4))
ax.yaxis.set_major_locator(MaxNLocator(4))

ax = fig.add_axes([0.6, 0.1, 0.2, 0.85])
ax.plot(sample[:, 4], sample[:, 5], '.', color="#888888", mec="#888888",
        markersize=3)
ax.plot(1.1, 2, "rs", markersize=8)
ax.set_xlabel(r'$\alpha_1$')
ax.set_ylabel(r'$\alpha_2$')
ax.set_xlim([-0.01, 5.01])
ax.xaxis.set_major_locator(MaxNLocator(4))
ax.yaxis.set_major_locator(MaxNLocator(4))

pl.savefig('inference.eps', bbox_inches='tight')


#
# CATALOG SAMPLES.
#

params = sample[:, 0:numHyperparams]
catalogs = sample[:, numHyperparams:numHyperparams + 3 * maxNumStars]
xStars = catalogs[:, 0:maxNumStars]
yStars = catalogs[:, maxNumStars:2 * maxNumStars]
fStars = catalogs[:, 2 * maxNumStars:]
images = sample[:, numHyperparams + 3 * maxNumStars:]

xCatalog = np.array([])
yCatalog = np.array([])
fCatalog = np.array([])

mock_data_mean = np.zeros(data.shape)

k = 1

pl.figure(figsize=(12, 12))
pl.subplots_adjust(hspace=0.02, wspace=0.02)

for i in range(0, 9):
    mock = images[i, :].reshape(data.shape[0], data.shape[1])
    ax = pl.subplot(3, 3, k)
    which = np.nonzero(fStars[i, :] > 0)[0]

    s = 50 * np.log(1 + fStars[i, which] - fStars[i, which].min())
    c = [[1 - c0] * 3 for c0 in
                        np.sqrt(fStars[i, which] / fStars[i, which].max())]
    pl.scatter(xStars[i, which], yStars[i, which], c=c, s=s)

    pl.axis('equal')
    pl.xlim([-1., 1.])
    pl.ylim([-1., 1.])
    ax.set_xticks([])
    ax.set_yticks([])
    if k == 2:
        pl.title('Catalogs')
    k += 1

    mock_data_mean += mock / 9

    xCatalog = np.hstack([xCatalog, xStars[i, which]])
    yCatalog = np.hstack([yCatalog, yStars[i, which]])
    fCatalog = np.hstack([fCatalog, fStars[i, which]])

    print(i + 1, ((((data - mock) / 30.) ** 2).sum() / 10000. - 1.)
            / (np.sqrt(2.) / 100.))

pl.savefig('catalogs.eps', bbox_inches='tight')


#
# SUMMARY IMAGES
#

scale = lambda i: (i.max() - i.min()) ** 0.4 - (i - i.min()) ** 0.4

fig = pl.figure(figsize=(10, 10))
ax = fig.add_subplot(2, 2, 1)

which1 = np.logical_and(xCatalog > -1., xCatalog < 1.)
which2 = np.logical_and(yCatalog > -1., yCatalog < 1.)
which = np.logical_and(which1, which2)

img = np.histogram2d(xCatalog[which], yCatalog[which],
                     bins=200, weights=fCatalog[which])[0].T[::-1]
pl.imshow(scale(img), extent=(-1, 1, -1, 1), interpolation="nearest",
                                             cmap='gray')
pl.axis([-1, 1, -1, 1])
ax.set_xticks([-1, 0, 1])
ax.set_yticks([-1, 0, 1])
ax.set_title('Expected True Scene')
ax.set_xlabel('$x$')
ax.set_ylabel('$y$')

ax = fig.add_subplot(2, 2, 2)
ax.imshow(scale(mock_data_mean), extent=(-1, 1, -1, 1), cmap='gray',
                                                interpolation="nearest")
ax.set_title('Expected Noise-Free Scene')
ax.axis([-1, 1, -1, 1])
ax.set_xticks([-1, 0, 1])
ax.set_yticks([-1, 0, 1])
ax.set_xlabel('$x$')
ax.set_ylabel('$y$')

ax = fig.add_subplot(2, 2, 3)
ax.imshow(mock_data_mean - data, extent=(-1, 1, -1, 1), cmap='gray',
                                                interpolation="nearest")
ax.set_title('Residuals')
ax.axis([-1, 1, -1, 1])
ax.set_xticks([-1, 0, 1])
ax.set_yticks([-1, 0, 1])
ax.set_xlabel('$x$')
ax.set_ylabel('$y$')

ax = fig.add_subplot(2, 2, 4)
plot_utils.hist((mock_data_mean - data).flatten(), 100)
ax.set_yticklabels([])
ax.set_xlim(-200, 200)
ax.xaxis.set_major_locator(MaxNLocator(4))
ax.set_xlabel('Residuals')

pl.savefig('summaries.eps', bbox_inches='tight')
