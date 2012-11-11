from matplotlib import rc
rc("font", size=20, family="serif", serif="Computer Sans")
rc("text", usetex=True)
import matplotlib.pyplot as pl


def hist(*args, **kwargs):
    ax = kwargs.pop("ax", pl.gca())

    kwargs["histtype"] = kwargs.pop("histtype", "stepfilled")
    kwargs["color"] = kwargs.pop("histtype", "#A6BDDB")

    ax.hist(*args, **kwargs)
