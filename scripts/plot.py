#!/bin/env python

import plottery_wrapper as p

cutname_to_plot = "Mjj1000"

##########################

# cutflow
p.dump_plot(fnames=[
        "hadds/wz.root",
        "hadds/tt1l.root",
        "hadds/tt2l.root",
        "hadds/ttw.root",
        "hadds/ttz.root",
        "hadds/tth.root",
        "hadds/ssww.root",
        ],
    sig_fnames=[
        "hadds/vbshww.root",
        "hadds/lambda20_vbshww.root",
        "hadds/lambdam20_vbshww.root",
        ],
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        # "signal_scale": 20,
        "signal_scale": "auto",
        },
    )

# cutflow
p.dump_plot(fnames=[
        "hadds/wz.root",
        "hadds/tt1l.root",
        "hadds/tt2l.root",
        "hadds/ttw.root",
        "hadds/ttz.root",
        "hadds/tth.root",
        "hadds/ssww.root",
        ],
    sig_fnames=[
        "hadds/lambdam20_vbshww.root",
        ],
    dirname="plots_cut_opt_scan",
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    extraoptions={
        "print_yield": True,
        # "nbins": 60,
        "signal_scale": "auto",
        "yaxis_range": [0., 5.],
        },
    _plotter=p.plot_soverb_scan,
    )
