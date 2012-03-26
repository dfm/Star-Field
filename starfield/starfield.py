#!/usr/bin/env python
# encoding: utf-8
"""


"""

__all__ = [""]


import numpy as np

class PSF(object):
    def __init__(self, amplitudes, variances):
        self.variances = np.atleast_1d(variances)
        self.amplitudes = np.atleast_1d(amplitudes)

    def image(self, a, x, y, nx, ny, wfactor=3, img=None):
        # First, generate a pixel grid.
        x0, y0 = np.arange(nx), np.arange(ny)

        # Then, calculate the distances (on this grid) from the central point.
        r2  = - ((x - x0)**2)[:, None] - ((y - y0)**2)[None, :]

        # For speed, just consider the points where the distance is less than
        # some factor times the maximum width.
        mask = r2 > -wfactor * np.max(self.variances)

        # Calculate the full image as a mixture of Gaussians on the pixel
        # grid.
        if img is None:
            img = np.zeros_like(r2)
        t = np.sum(self.amplitudes[None, :] \
                * np.exp(r2[mask][:,None] / self.variances[None, :]), axis=-1)
        img[mask] += a * t / np.sum(t)

        return img

class Starfield(object):
    def __init__(self, psf, nx, ny, x, y, flux):
        self.psf  = psf
        self.nx   = nx
        self.ny   = ny
        self.x    = np.atleast_1d(x)
        self.y    = np.atleast_1d(y)
        self.flux = np.atleast_1d(flux)

        assert np.all(np.array([len(self.x), len(self.y)]) == len(self.flux))

    def image(self):
        img = np.zeros((self.nx, self.ny))
        for k in range(len(self.x)):
            img =  self.psf.image(self.flux[k], self.x[k], self.y[k],
                    self.nx, self.ny, img=img)
        return img.T

if __name__ == "__main__":
    import time
    import matplotlib.pyplot as pl

    psf = PSF([1.2, 0.5], [1.0, 4.0])
    N = 800
    x, y = 100 + 10*np.random.randn(N), 100 + 10*np.random.randn(N)
    flux = np.random.rand(N)
    sf = Starfield(psf, 200, 200, x, y, flux)

    s = time.time()
    img = sf.image()
    print time.time()-s

    ax = pl.subplot(111, aspect="equal")
    ax.pcolor(-img, cmap="gray")
    # pl.plot(x, y, ".k")
    pl.show()

