#!/usr/bin/env python
"""
Run SExtractor on some FITS images and show the results.

Usage: sex.py <infile>... [--conv <convname>]

Options:
  -h --help         Show this information.
  <infile>          The path to the input FITS image.
  --conv <convname> The name of the convolution filter to use.
                    [default: gauss_2.0_5x5.conv]

"""

from __future__ import print_function

import os
import re
import shutil
import subprocess

from docopt import docopt
import pyfits
import numpy as np
import matplotlib.pyplot as pl


def run_sex(fitfn, outdir, convname="gauss_2.0_5x5.conv"):
    try:
        os.makedirs(outdir)
    except os.error:
        pass

    configdir = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                             "config")

    # Copy the parameters file.
    parsfn = os.path.join(outdir, "config.param")
    shutil.copy(os.path.join(configdir, "default.param"), parsfn)

    # Parse and update the configuration file.
    with open(os.path.join(configdir, "default.sex")) as f:
        config = f.read()

    catfn = os.path.join(outdir, "stars.cat")
    config = replace_settings(config, ["CATALOG_NAME", "FILTER_NAME",
                                       "PARAMETERS_NAME"],
                                      [catfn,
                                       os.path.join(configdir, convname),
                                       parsfn])
    configfn = os.path.join(outdir, "config.sex")
    with open(configfn, "w") as f:
        f.write(config)

    # Run SExtractor.
    ret = subprocess.call(["sex", fitfn, "-c", configfn])
    assert ret == 0, "SExtractor failed with status: {0}.".format(ret)

    return catfn


def replace_settings(config, names, values):
    for n, v in zip(names, values):
        config = re.sub("^{0} (.*?)$".format(n),
                        "{0} {1}".format(n, v),
                        config, flags=re.S | re.M)
    return config


def plot_sex(imgfn, catfn):
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
    for i, fn in enumerate(args["<infile>"]):
        outdir = os.path.splitext(os.path.split(fn)[1])[0]
        catfn = run_sex(fn, outdir, convname=args["--conv"])
        plot_sex(fn, catfn)
