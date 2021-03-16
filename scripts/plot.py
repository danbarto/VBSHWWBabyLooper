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

bkg_labels = ["WZ", "t#bar{t}(1l)", "t#bar{t}(2l)", "t#bar{t}W", "t#bar{t}Z", "t#bar{t}H", "W^{#pm}W^{#pm}jj"]
signal_labels = ["SM", "C2V=6", "C2V=4.5", "C2V=3", "C2V=-2"]
colors = [4022, 4020, 4023, 4021, 4024, 4101, 4201]

# bkg_labels = ["WZ", "tt1l", "ttw", "ttz", "ttH", "ssww"]
# signal_labels = ["SM", "lam20", "lam-20"]

#
lumi = -1
if "2016" in hadd_dir: lumi = 35.9
if "2017" in hadd_dir: lumi = 41.5
if "2018" in hadd_dir: lumi = 59.97

##########################

# cutflow
p.dump_plot(fnames=[
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/vbshww.root".format(hadd_dir),
        "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        "{}/vbshww_c2v_3.root".format(hadd_dir),
        "{}/vbshww_c2v_m2.root".format(hadd_dir),
        # "{}/lambda20_vbshww.root".format(hadd_dir),
        # "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots/sig_1x/{}".format(study_name),
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    usercolors=colors,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        "signal_scale": 1,
        # "signal_scale": "auto",
        "legend_ncolumns": 3,
        "legend_scalex": 1.8,
        "lumi_value": lumi,
        # "yaxis_log": True,
        },
    )

# cutflow
p.dump_plot(fnames=[
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/vbshww.root".format(hadd_dir),
        "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        "{}/vbshww_c2v_3.root".format(hadd_dir),
        "{}/vbshww_c2v_m2.root".format(hadd_dir),
        # "{}/lambda20_vbshww.root".format(hadd_dir),
        # "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots/sig_auto/{}".format(study_name),
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    usercolors=colors,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        # "signal_scale": 150,
        "signal_scale": "auto",
        "legend_ncolumns": 3,
        "legend_scalex": 1.8,
        "lumi_value": lumi,
        },
    )

# cutflow
p.dump_plot(fnames=[
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/tth.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        ],
    sig_fnames=[
        "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        "{}/vbshww_c2v_3.root".format(hadd_dir),
        "{}/vbshww_c2v_m2.root".format(hadd_dir),
        # "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    dirname="plots/cut_opt_scan/{}".format(study_name),
    filter_pattern="{}__".format(cutname_to_plot),
    dogrep=True,
    usercolors=colors,
    extraoptions={
        "print_yield": True,
        # "nbins": 60,
        "signal_scale": 1,
        # "yaxis_range": [0., 5.],
        },
    _plotter=p.plot_cut_scan,
    )
