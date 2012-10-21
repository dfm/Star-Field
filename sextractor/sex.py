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
    config = replace_settings(config, ["CATALOG_NAME", "FILTER_NAME",
                                       "PARAMETERS_NAME"],
                                      [os.path.join(outdir, "stars.cat"),
                                       os.path.join(configdir, convname),
                                       parsfn])
    configfn = os.path.join(outdir, "config.sex")
    with open(configfn, "w") as f:
        f.write(config)

    # Run SExtractor.
    ret = subprocess.call(["sex", fitfn, "-c", configfn])
    print(ret)


def replace_settings(config, names, values):
    for n, v in zip(names, values):
        config = re.sub("^{0} (.*?)$".format(n),
                        "{0} {1}".format(n, v),
                        config, flags=re.S | re.M)
    return config


if __name__ == "__main__":
    args = docopt(__doc__)
    print(args)

    for i, fn in enumerate(args["<infile>"]):
        outdir = os.path.splitext(os.path.split(fn)[1])[0]
        run_sex(fn, outdir, convname=args["--conv"])
