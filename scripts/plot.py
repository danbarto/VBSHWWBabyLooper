#!/bin/env python

import plottery_wrapper as p
import sys

def usage():
    print("Usage:")
    print("")
    print("   python {} STUDYNAME CUTNAME".format(sys.argv[0]))
    print("")
    print("")
    sys.exit()


try:
    study_name = sys.argv[1]
    cutname_to_plot = sys.argv[2]
except:
    usage()
    sys.exit()

hadd_dir = "hadds/{}".format(study_name)

bkg_labels = ["WZ", "tt1l", "tt2l", "ttw", "ttz", "ttH", "ssww"]
bkg_labels = ["WZ", "tt1l", "ttw", "ttz", "ttH", "ssww"]
signal_labels = ["SM", "lam20", "lam-20"]

##########################

# cutflow
p.dump_plot(fnames=[
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        # "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/vbshww.root".format(hadd_dir),
        "{}/lambda20_vbshww.root".format(hadd_dir),
        "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots/sig_150x/{}".format(study_name),
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
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        # "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/vbshww.root".format(hadd_dir),
        "{}/lambda20_vbshww.root".format(hadd_dir),
        "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots/sig_auto/{}".format(study_name),
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
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        # "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    dirname="plots/cut_opt_scan/{}".format(study_name),
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
