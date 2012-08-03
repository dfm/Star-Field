"""
This file is part of the Star-Field project

It's purpose is to run SExtractor (tm) on the same data on which we are doing inference.

### Requires:
- `pyfits`
- `SExtractor`

### Bugs:
- Not yet written!
"""
import os
import numpy as np
import pyfits

def txt2fits(txtfn, fitsfn):
    """
    Convert an ascii text file containing an image (who would do
    that?) into a FITS file.  Brittle; makes assumptions.
    """
    print "txt2fits: reading {0}".format(txtfn)
    image = np.loadtxt(txtfn)
    print "txt2fits: got image of shape {0}".format(image.shape)
    print "txt2fits: writing {0}".format(fitsfn)
    hdu = pyfits.PrimaryHDU(image)
    hdu.writeto(fitsfn)
    return None

if __name__ == "__main__":
    fns = [("../c++_dnest/SimulatedData/cluster.txt", "cluster.fits"),
           ("../c++_dnest/SimulatedData/crowded.txt", "crowded.fits"),
           ("../c++_dnest/SimulatedData/sparse.txt", "sparse.fits"), ]
    for txtfn, fitsfn in fns:
        if not os.path.exists(fitsfn):
            txt2fits(txtfn, fitsfn)
