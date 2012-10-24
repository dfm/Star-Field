#!/usr/bin/env python
"""
Run the IDL version of DAOPHOT on some FITS images and show the results.

Usage: dao.py <infile>... [--server <server>] [--plot_only]

Options:
  -h --help         Show this information.
  <infile>          The path to the input FITS image.
  --server <sever>  The SSH path to the server to run on. [default: sculptor]
  --plot_only       Only generate the plots.

"""

from __future__ import print_function

import os
import hashlib
import subprocess

from docopt import docopt
import pyfits
import numpy as np
import matplotlib
import matplotlib.pyplot as pl

from utils import estimate_sigma, asinh


# Nicer colors off the bat from:
#  https://github.com/mbostock/d3/wiki/Ordinal-Scales
matplotlib.rcParams["axes.color_cycle"] = ["#1f77b4", "#ff7f0e", "#2ca02c",
                                           "#d62728", "#9467bd", "#8c564b",
                                           "#e377c2", "#7f7f7f", "#bcbd22",
                                           "#17becf"]


def run_dao(fitfn, outdir, server, threshes=[1]):
    try:
        os.makedirs(outdir)
    except os.error:
        pass

    # Temporary filenames.
    id_ = hashlib.md5(fitfn).hexdigest()
    tmpfn = id_ + ".fits"
    localscript = "idl.pro"
    scriptfn = id_ + ".pro"
    catfns = [os.path.join(outdir, "daophot.{0}.cat".format(t))
                                    for t in threshes]

    # Generate an IDL script.
    script = "img = mrdfits('{0}')\n".format(tmpfn)
    for t in threshes:
        script += "find, img, x, y, f, sharp, round, {0}, 2,".format(t)
        script += "[0.2,1.0], [-1.0,1.0], PRINT='{0}.{1}.cat'\n".format(id_, t)
    script += "exit\n"
    with open(localscript, "w") as f:
        f.write(script)

    try:
        # Copy the image.
        subprocess.check_call(["scp", fitfn, "{0}:{1}".format(server, tmpfn)])

        # Copy the script.
        subprocess.check_call(["scp", localscript,
                            "{0}:{1}".format(server, scriptfn)])

        # Run DAOPHOT.
        cmd = "ssh " + server + " 'source .bash_profile;setup idlutils;" \
                + "idl {0};'".format(id_)
        subprocess.check_call(cmd, shell=True)

        # Copy back the catalogs.
        [subprocess.check_call(["scp", "{0}:{1}".format(server,
                "{0}.{1}.cat".format(id_, t)), c]) for c in catfns]
    finally:
        # Clean up.
        os.remove(localscript)
        subprocess.check_call(["ssh", server, "rm -rf {0}.*".format(id_)])

    return catfns


def plot_dao(imgfn, catfn, hist_ax, thresh):
    hdus = pyfits.open(imgfn)
    img = np.array(hdus[0].data, dtype=float)
    hdus.close()

    # Estimate sigma.
    sigma = estimate_sigma(img)

    # Asinh stretch.
    img = asinh(img, np.median(img), sigma, 0.2)

    # Load catalog.
    with open(catfn) as f:
        lines = f.readlines()[12:]
        headers = lines[0].split()
        catalog = [tuple(l.split()) for l in lines[1:]]

    catalog = np.array(catalog, dtype=np.dtype([(n, float) for n in headers]))

    fig = pl.figure(figsize=(8, 8))
    ax = fig.add_subplot(111)

    ax.imshow(img, cmap="gray", interpolation="nearest", vmin=0, vmax=1)
    ax.plot(catalog["X"] - 1, catalog["Y"] - 1, "+r")

    ax.set_xlim([0, img.shape[0]])
    ax.set_ylim([0, img.shape[1]])

    fig.savefig(os.path.splitext(catfn)[0] + ".dao.pdf")
    fig.savefig(os.path.splitext(catfn)[0] + ".dao.png")

    # Plot the histogram of fluxes.
    fluxes = catalog["FLUX"]
    n, b = np.histogram(fluxes, max(2, int(0.3 * len(fluxes))))
    inds = np.array([np.all(n[:i] > 0) for i in range(len(n))], dtype=bool)
    hist_ax.plot(np.log(0.5 * (b[:-1] + b[1:]))[inds], np.log(n[inds]), lw=2,
                label=r"{0:.1f}-$\sigma$ ({1})".format(thresh, len(fluxes)))


if __name__ == "__main__":
    args = docopt(__doc__)
    fig = pl.figure(figsize=(8, 8,))
    threshes = np.arange(0.5, 6, 0.5)
    for i, fn in enumerate(args["<infile>"]):
        ax = fig.add_subplot(111)
        outdir = os.path.splitext(os.path.split(fn)[1])[0]
        if not args["--plot_only"]:
            catfns = run_dao(fn, outdir, args["--server"], threshes=threshes)
        else:
            catfns = [os.path.join(outdir, "daophot.{0}.cat".format(t))
                                            for t in threshes]
        [plot_dao(fn, catfns[i], ax, t) for i, t in enumerate(threshes)]

        # Layout/save the histograms plot.
        ax.legend(prop={"size": 13}, loc="lower left")
        ax.set_xlabel(r"$\ln \, f$")
        ax.set_ylabel(r"$\ln \, N$")
        hfn = os.path.join(outdir, os.path.splitext(fn)[0]) + ".hist"
        ax.figure.savefig(hfn + ".dao.png")
        ax.figure.savefig(hfn + ".dao.pdf")
        fig.clear()
