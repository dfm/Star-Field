# Synthetic Star Field Generator

```python
from starfield import PSF, StarField
import matplotlib.pyplot as pl

psf = PSF([1.0, 0.6], [1.0, 4.0])

sf = StarField(psf, 200, 200)
sf.generate_bg_stars(100)
sf.generate_gaussian_stars(50, 95, 83.5, [[8.**2, 0], [0, 8.**2]])
img = sf.image()

fig = pl.figure(figsize=(8,8))
ax = fig.add_subplot(111, aspect="equal")
ax.pcolor(-img, cmap="gray")
pl.savefig("example.png")
```

!["Just some stars"](https://github.com/dfm/Star-Field/raw/master/example.png)

