#!/usr/bin/env python
"""
Run the IDL version of DAOPHOT on some FITS images and show the results.

Usage: dao.py <infile>... [--server <server>]

Options:
  -h --help         Show this information.
  <infile>          The path to the input FITS image.
  --server <sever>  The SSH path to the server to run on. [default: sculptor]

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


# Nicer colors off the bat from:
#  https://github.com/mbostock/d3/wiki/Ordinal-Scales
matplotlib.rcParams["axes.color_cycle"] = ["#1f77b4", "#ff7f0e", "#2ca02c",
                                           "#d62728", "#9467bd", "#8c564b",
                                           "#e377c2", "#7f7f7f", "#bcbd22",
                                           "#17becf"]


def run_dao(fitfn, outdir, server):
    try:
        os.makedirs(outdir)
    except os.error:
        pass

    # Temporary filenames.
    id_ = hashlib.md5(fitfn).hexdigest()
    tmpfn = id_ + ".fits"
    localscript = "idl.pro"
    scriptfn = id_ + ".pro"
    catfn = id_ + ".cat"

    # Generate an IDL script.
    with open(localscript, "w") as f:
        f.write("""img = mrdfits('{0}')
find, img, x, y, f, sharp, round, 4, 2, [0.2,1.0], [-1.0,1.0], PRINT='{1}'
exit
""".format(tmpfn, catfn))

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

        # Copy back the catalog.
        subprocess.check_call(["scp", "{0}:{1}".format(server, catfn),
                            os.path.join(outdir, "daophot.cat")])
    finally:
        # Clean up.
        os.remove(localscript)
        subprocess.check_call(["ssh", server,
                    "rm -rf {0}".format(" ".join([tmpfn, scriptfn, catfn]))])


def plot_sex(imgfn, catfn, hist_ax):
    hdus = pyfits.open(imgfn)
    img = np.array(hdus[0].data, dtype=float)
    hdus.close()

    # Estimate sigma.
    sigma = estimate_sigma(img)

    # Asinh stretch.
    img = asinh(img, np.median(img), sigma, 0.2)

    # Load catalog.
    with open(catfn) as f:
        headers = []
        catalog = []
        for line in f:
            if line[0] == "#":
                headers.append(line.split()[2])
            else:
                catalog.append(tuple(line.split()))

    catalog = np.array(catalog, dtype=np.dtype([(n, float) for n in headers]))

    fig = pl.figure(figsize=(8, 8))
    ax = fig.add_subplot(111)

    ax.imshow(img, cmap="gray", interpolation="nearest", vmin=0, vmax=1)
    ax.plot(catalog["X_IMAGE"] - 1, catalog["Y_IMAGE"] - 1, "+r")

    ax.set_xlim([0, img.shape[0]])
    ax.set_ylim([0, img.shape[1]])

    fig.savefig(os.path.splitext(catfn)[0] + ".pdf")
    fig.savefig(os.path.splitext(catfn)[0] + ".png")

    # Plot the histogram of fluxes.
    fluxes = catalog["FLUX_AUTO"]
    n, b = np.histogram(fluxes, max(2, int(0.3 * len(fluxes))))
    inds = np.array([np.all(n[:i] > 0) for i in range(len(n))], dtype=bool)
    hist_ax.plot(np.log(0.5 * (b[:-1] + b[1:]))[inds], np.log(n[inds]), lw=2,
                label=r"{0:.1f}-$\sigma$ ({1})".format(thresh, len(fluxes)))


def estimate_sigma(scene, nsigma=3.5, tol=0.0):
    img = scene.flatten()
    mask = np.ones(len(img), dtype=bool)
    ms_old = 0.0
    for i in range(500):
        m = np.median(img[mask])
        ms = np.mean((img[mask] - m) ** 2)
        mask = (img - m) ** 2 < nsigma ** 2 * ms
        if i > 1 and np.abs(ms - ms_old) < tol:
            break
        ms_old = ms
    return np.sqrt(ms)


def asinh(img, mu, sigma, f):
    return f * np.arcsinh((img - mu) / sigma) + 0.2


if __name__ == "__main__":
    args = docopt(__doc__)
    # fig = pl.figure(figsize=(8, 8,))
    for i, fn in enumerate(args["<infile>"]):
        # ax = fig.add_subplot(111)
        outdir = os.path.splitext(os.path.split(fn)[1])[0]
        catfn = run_dao(fn, outdir, args["--server"])

        # # Layout/save the histograms plot.
        # ax.legend(prop={"size": 13}, loc="lower left")
        # ax.set_xlabel(r"$\ln \, f$")
        # ax.set_ylabel(r"$\ln \, N$")
        # hfn = os.path.join(outdir, os.path.splitext(fn)[0]) + ".hist"
        # ax.figure.savefig(hfn + ".png")
        # ax.figure.savefig(hfn + ".pdf")
        # fig.clear()
