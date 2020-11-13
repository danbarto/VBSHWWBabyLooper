#!/bin/env python

import plottery_wrapper as p

cutname_to_plot = "NbMed"

bkg_labels = ["WZ", "tt1l", "tt2l", "ttw", "ttz", "ttH", "ssww"]
bkg_labels = ["WZ", "tt1l", "ttw", "ttz", "ttH", "ssww"]
signal_labels = ["SM", "lam20", "lam-20"]

##########################

# cutflow
p.dump_plot(fnames=[
        "hadds/wz.root",
        "hadds/tt1l.root",
        # "hadds/tt2l.root",
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
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        "signal_scale": 150,
        # "signal_scale": "auto",
        "legend_ncolumns": 3,
        "legend_scalex": 1.8,
        "lumi_value": 137,
        },
    )

# cutflow
p.dump_plot(fnames=[
        "hadds/wz.root",
        "hadds/tt1l.root",
        # "hadds/tt2l.root",
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
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots_sig_auto",
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        # "signal_scale": 150,
        "signal_scale": "auto",
        "legend_ncolumns": 3,
        "legend_scalex": 1.8,
        "lumi_value": 137,
        },
    )

# cutflow
p.dump_plot(fnames=[
        "hadds/wz.root",
        "hadds/tt1l.root",
        # "hadds/tt2l.root",
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
