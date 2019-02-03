# Copyright

This repository is (c) 2012, 2013 Brendon J. Brewer, Daniel 
Foreman-Mackey, David W. Hogg. Its contents are released under the
MIT License. See the LICENSE file for details.

Probabilistic Catalogs for Stellar Fields
-----

If you're looking for a more recent and somewhat more user-friendly implementation of this idea,
see [StarStudded](https://github.com/eggplantbren/StarStudded).

<!--# Synthetic Star Field Generator-->

<!--Generate some sweet fake crowded stellar fields for fun & profit.-->

<!--## Example-->

<!--Run this...-->

<!--```python-->
<!--from starfield import PSF, StarField-->
<!--import matplotlib.pyplot as pl-->

<!--psf = PSF([1.0, 0.6], [1.0, 4.0])-->

<!--sf = StarField(psf, 200, 200)-->
<!--sf.generate_bg_stars(100)-->
<!--sf.generate_gaussian_stars(50, 95, 83.5, [[8.**2, 0], [0, 8.**2]])-->
<!--img = sf.image()-->

<!--fig = pl.figure(figsize=(8,8))-->
<!--ax = fig.add_subplot(111, aspect="equal")-->
<!--ax.pcolor(-img, cmap="gray")-->
<!--pl.savefig("example.png")-->
<!--```-->

<!--...to get this...-->

<!--!["Just some stars"](https://github.com/dfm/Star-Field/raw/master/example.png)-->

<!--...and you can save it to a file...-->

<!--```python-->
<!--sf.save("starfield.h5")-->
<!--sf.save("starfield.fits")-->
<!--```-->

