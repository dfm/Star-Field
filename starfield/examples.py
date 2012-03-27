#!/usr/bin/env python
"""


"""

import time

import numpy as np
import scipy.optimize as op
import matplotlib.pyplot as pl
import h5py

from starfield import Catalog, PSF, StarField

def gaussian_and_bg():
    psf = PSF([1.0, 0.6], [1.0, 4.0])

    sf = StarField(psf, 200, 200)
    sf.generate_bg_stars(100)
    sf.generate_gaussian_stars(50, 95, 83.5, [[8.**2, 0], [0, 8.**2]])

    s = time.time()
    img = sf.image()
    print sf.lnlike(img)

    print "Generating an image w/ %d stars took: %.4f seconds"\
            %(len(sf.catalog), time.time()-s)

    fig = pl.figure(figsize=(8,8))
    ax = fig.add_subplot(111, aspect="equal")
    ax.pcolor(-img, cmap="gray")
    pl.savefig("cluster.png")

    sf.save("cluster.h5")
    sf.save("cluster.fits")

def bg_only():
    psf = PSF([1.0, 0.6], [1.0, 4.0])

    sf = StarField(psf, 200, 200)
    sf.generate_bg_stars(100, p=0, f0=1)

    sf.save("bg.h5")

def fit_bg():
    psf = PSF([0.5, 0.5], [1.0, 25.0])
    nx = 100
    Nstars = 10
    f0 = 1.0
    nwalkers = 50

    sf = StarField(psf, nx, nx)
    sf.generate_bg_stars(Nstars, p=1e20, f0=f0)

    truth = sf.image(noise=False)
    data = sf.image()

    # sf = StarField(psf, nx, nx)
    sampler, r = sf.sample(Nstars, data, nwalkers=nwalkers)

    fig = pl.figure(figsize=(10,10))

    ax = fig.add_subplot(221, aspect="equal")
    ax.imshow(-data, cmap="gray")

    ax = fig.add_subplot(222, aspect="equal")
    ax.imshow(-truth, cmap="gray")

    ax1 = fig.add_subplot(223, aspect="equal")
    ax2 = fig.add_subplot(224, aspect="equal")

    for i, (pos,lnprob,state) in enumerate(sampler.sample(*r,
                                          iterations=500)):
        if i%10 == 0:
            w = np.random.randint(nwalkers)
            catalog = Catalog(x=pos[w,:Nstars], y=pos[w,Nstars:],
                    flux=f0*np.ones(Nstars))
            sf = StarField(psf, nx, nx, catalog=catalog)

            ax1.cla()
            ax2.cla()
            ax1.imshow(-sf.image(noise=False), cmap="gray")
            ax2.imshow(data-sf.image(noise=False), cmap="gray")

            pl.savefig("fit/%d-%d.png"%(i, w))

if __name__ == '__main__':
    import sys

    if "--cluster" in sys.argv:
        gaussian_and_bg()

    if "--bg" in sys.argv:
        bg_only()

    if "--fitbg" in sys.argv:
        fit_bg()

