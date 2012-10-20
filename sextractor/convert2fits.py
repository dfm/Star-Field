#!/usr/bin/env python
"""
Convert an ASCII representation of an image (gasp!) to a FITS file.

Usage: convert2fits.py <infile>... [-o <outfile>...] [--clobber]

Options:
  -h --help     Show this information.
  <infile>      The path to the input text file.
  -o <outfile>  The path to the output FITS file.
  --clobber     Overwrite existing output. [default: False]

"""

from __future__ import print_function

import os
import numpy as np

from docopt import docopt
import pyfits


def txt2fits(txtfn, fitsfn):
    """
    Convert an ascii text file containing an image (who would do
    that?) into a FITS file.  Brittle; makes assumptions.
    """
    print("Converting {0} to {1}...".format(txtfn, fitsfn))
    try:
        img = np.loadtxt(txtfn)
    except ValueError:
        print("    {0} contains invalid values. Aborting.".format(txtfn))
        return
    hdu = pyfits.PrimaryHDU(img)
    hdu.writeto(fitsfn)

if __name__ == "__main__":
    args = docopt(__doc__)

    if args["-o"] is not None:
        assert len(args["<infile>"]) == len(args["-o"]), \
                "You need to provide the same number of output file names " \
                "as inputs."

    for i, fn in enumerate(args["<infile>"]):
        if args["-o"] is None:
            ofn = os.path.splitext(os.path.split(fn)[1])[0] + ".fits"
        else:
            ofn = args["-o"][i]
        txt2fits(fn, ofn)
