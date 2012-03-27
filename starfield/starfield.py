"""
Just generating some fake data.

"""

__all__ = ["PSF", "StarField"]

import os

import numpy as np
try:
    import pyfits
except ImportError:
    pyfits = None
try:
    import h5py
except ImportError:
    h5py = None

class Catalog(object):
    """
    A catalog of stars

    ```
    for star in catalog:
        print star
    ```

    ## Keyword Arguments

    * `x` (numpy.ndarray): The x positions of the stars.
    * `y` (numpy.ndarray): The y positions of the stars.
    * `flux` (numpy.ndarray): The fluxes of the stars.

    """
    def __init__(self, **kwargs):
        self.x = kwargs.pop("x", np.array([]))
        self.y = kwargs.pop("y", np.array([]))
        self.S = kwargs.pop("flux", np.array([]))

        assert len(self.x) == len(self.y) == len(self.S)

    def __getitem__(self, i):
        return (self.x[i], self.y[i], self.S[i])

    @property
    def flux(self):
        return self.S

    def add_star(self, x, y, S):
        """
        Add a star to the catalog.

        ## Arguments

        * `x`, `y` (float, float): The position of the star.
        * `S` (float): The flux of the star.

        """
        self.x = np.append(self.x, x)
        self.y = np.append(self.y, y)
        self.S = np.append(self.S, S)

class PSF(object):
    """
    A mixture-of-Gaussians approximation of a pixel-convolved point-spread
    function.

    ## Arguments

    * `amplitudes` (list or float): The relative amplitudes of the
      components.
    * `variances` (list or float): The list of variances for the components.

    """
    def __init__(self, amplitudes, variances):
        self.variances = np.atleast_1d(variances)
        self.amplitudes = np.atleast_1d(amplitudes)
        self.amplitudes /= np.sum(self.amplitudes)

        # Precalculate the normalization.
        N = 10*np.sqrt(np.max(variances))
        self._norm = 1.0
        self._norm = 1./np.sum(self.image(0.5*N, 0.5*N, nx=N, ny=N))

    def image(self, x, y, flux=1., nx=200, ny=200, img=None, wfactor=5):
        """
        Get the image generated by a point source at a given position.

        ## Arguments

        * `x` (float): The x-position of the source.
        * `y` (float): The y-position of the source.

        ## Keyword Arguments

        * `flux` (float): The flux of the source.
        * `nx` (int): The width of the image.
        * `ny` (int): The height of the image.
        * `img` (numpy.ndarray): A preallocated image object to add to. If
          this is not `None` it will override the value of `nx` and `ny`.
        * `wfactor` (float): The factor to multiply the maximum width by to
          set the maximum distance threshold.

        ## Returns

        * `img` (numpy.ndarray): The image where the shape is given by
          `(nx, ny)`.

        """
        # Find the dimensions of the image if it's provided and generate an
        # "image" of zeros if not.
        if img is not None:
            nx, ny = img.shape
        else:
            img = np.zeros((nx, ny))

        # Generate a pixel grid.
        x0, y0 = np.arange(nx), np.arange(ny)

        # Then, calculate the distances (on this grid) from the central point.
        negr2 = - ((x - x0)**2)[:, None] - ((y - y0)**2)[None, :]

        # For speed, just consider the points where the distance is less than
        # some factor times the maximum width.
        mask = negr2 > -wfactor**2 * np.max(self.variances)

        # Calculate the full image as a mixture of Gaussians on the pixel
        # grid.
        t =  np.exp(0.5 * negr2[mask][:,None] / self.variances[None, :])
        t /= np.sqrt(2 * np.pi * self.variances[None, :])
        img[mask] += self._norm * flux \
                * np.sum(self.amplitudes[None, :] * t, axis=-1)

        return img

class StarField(object):
    """
    A field of stars with known PSF, dimensions and a given set of _pixel_
    positions and fluxes.

    ## Arguments

    * `psf` (PSF): The pixel-convolved point-spread function.
    * `nx` (int): The width of the image.
    * `ny` (int): The height of the image.

    ## Keyword Arguments

    * `catalog` (Catalog): The initial catalog of stars.
    * `noisevar` (float): A constant variance for the noise.
    * `relnoise` (float): A relative component of the noise variance.

    """
    def __init__(self, psf, nx, ny, catalog=None, noisevar=1., relnoise=0.01):
        self.psf  = psf
        self.nx   = nx
        self.ny   = ny
        if catalog is None:
            catalog = Catalog()
        self.catalog = catalog

        self._image = None

        self._bgs = []
        self._clusters = []

        self.noisevar = noisevar
        self.relnoise = relnoise

    def _gen_fluxes(self, N, p, f0):
        return f0*10.**(np.random.rand(N)/p)

    def generate_bg_stars(self, N, p=-2, f0=100.):
        """
        Generate a set of uniformly distributed stars drawn from a given
        luminosity function.

        ## Arguments

        * `N` (int): The number of background stars.

        ## Keyword Arguments

        * `p` (float): The log-slope of the luminosity function.
        * `f0` (float): The normalization of the luminosity function.

        """
        self._bgs.append((N, p, f0))

        flux = self._gen_fluxes(N, p, f0)
        x, y = self.nx * np.random.rand(N), self.ny * np.random.rand(N)

        self.catalog.add_star(x, y, flux)

    def generate_gaussian_stars(self, N, x0, y0, cov, p=-2, f0=100.):
        """
        Generate a set of stars drawn from a 2D Gaussian with given mean and
        covariance.

        ## Arguments

        * `N` (int): The number of stars.
        * `x0`, `y0` (float, float): The central position of the cluster.
        * `cov` (numpy.ndarray): The covariance matrix giving the cluster
          shape.

        ## Keyword Arguments

        * `p` (float): The log-slope of the luminosity function.
        * `f0` (float): The normalization of the luminosity function.

        """
        cov = np.atleast_2d(cov)
        self._clusters.append((N,x0,y0,cov[0,0],cov[0,1],cov[1,1],p,f0))

        flux = self._gen_fluxes(N, p, f0)
        xy = np.random.multivariate_normal([x0, y0], cov, N)

        self.catalog.add_star(xy[:,0], xy[:,1], flux)

    def image(self, noise=True):
        """
        Get the image generated by the ensemble of all the point sources
        convolved with the point-spread function.

        ## Keyword Arguments

        * `noise` (bool): Include noise in the image?

        ## Returns

        * `img` (numpy.ndarray): The image where the shape is given by
          `(nx, ny)`.

        """
        if self._image is not None:
            return self._image

        # First, initialize the image as a set of zeros.
        self._image = np.zeros((self.nx, self.ny))

        # Then, loop over the stars and sum up the contributions.
        for x, y, S in self.catalog:
            self._image = self.psf.image(x, y, flux=S, img=self._image)

        if noise:
            self._image +=\
                    np.sqrt(self.noisevar + self._image**2*self.relnoise) \
                    * np.random.randn(*self._image.shape)

        return self._image

    def save(self, fn):
        """
        Save the image and metadata to a file.

        ## Arguments

        * `fn` (str): The filename to save to. This should have either
          `.fits` or `.h5` extension.

        """
        root, ext = os.path.splitext(fn)
        if ext.lower() not in [".fits", ".h5"]:
            root += ext
            ext = ".fits"
        fn = root + ext

        assert (ext.lower() == ".fits" and pyfits is not None) \
                or (ext.lower() == ".h5" and h5py is not None)


        if ext.lower() == ".fits":
            hdu0 = pyfits.PrimaryHDU(self.image())

            hdu0.header.update("noisevar", self.noisevar)
            hdu0.header.update("relnoise", self.relnoise)

            sources = pyfits.new_table(pyfits.ColDefs([
                pyfits.Column(name="x", format="E", array=self.catalog.x),
                pyfits.Column(name="y", format="E", array=self.catalog.y),
                pyfits.Column(name="S", format="E", array=self.catalog.flux),
                ]))

            hdulist = pyfits.HDUList([hdu0, sources])
            hdulist.writeto(fn, clobber=True)

        if ext.lower() == ".h5":
            sources = np.array([s for s in self.catalog],
                dtype=[("x", float), ("y", float), ("flux", float)])
            bgs = np.array(self._bgs, dtype=[("N", int), ("p", float),
                                            ("f0", float)])
            clusters = np.array(self._clusters, dtype=[("N", int),
                ("x0", float), ("y0", float), ("c00", float), ("c01", float),
                ("c11", float), ("p", float), ("f0", float)])

            f = h5py.File(fn, "w")
            ds = f.create_dataset("data", data=self.image())

            ds.attrs["noisevar"] = self.noisevar
            ds.attrs["relnoise"] = self.relnoise

            f.create_dataset("sources", data=sources)
            f.create_dataset("bgs", data=bgs)
            f.create_dataset("clusters", data=clusters)
            f.close()

if __name__ == "__main__":
    import time
    import matplotlib.pyplot as pl

    psf = PSF([1.0, 0.6], [1.0, 4.0])

    sf = StarField(psf, 200, 200)
    sf.generate_bg_stars(100)
    sf.generate_gaussian_stars(50, 95, 83.5, [[8.**2, 0], [0, 8.**2]])

    s = time.time()
    img = sf.image()
    print "Generating an image w/ %d stars took: %.4f seconds"\
            %(len(sf.catalog), time.time()-s)

    fig = pl.figure(figsize=(8,8))
    ax = fig.add_subplot(111, aspect="equal")
    ax.pcolor(-img, cmap="gray")
    pl.savefig("starfield.png")

    sf.save("starfield.h5")
    sf.save("starfield.fits")

