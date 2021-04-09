#!/bin/env python

import plottery_wrapper as p
import sys

def usage():
    print("Usage:")
    print("")
    print("   python {} STUDYNAME [CUTNAME]".format(sys.argv[0]))
    print("")
    print("")
    sys.exit()


try:
    study_name = sys.argv[1]
except:
    usage()
    sys.exit()

try:
    cutname_to_plot = sys.argv[2]
except:
    cutname_to_plot = ""

hadd_dir = "hadds/{}".format(study_name)

bkgs = [
        "{}/tt1l.root".format(hadd_dir),
        "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/raretop.root".format(hadd_dir),
        "{}/bosons.root".format(hadd_dir),
        ]
bkg_labels = [
        "t#bar{t}(1l)",
        "t#bar{t}(2l)",
        "t#bar{t}W",
        "t#bar{t}Z",
        "Rare t#bar{t}+X(Y)",
        "V/VV/VVV",
        ]
signal_labels = ["C2V=6", "C2V=4.5", "C2V=-2", "C2V=3"]
colors = [
        4020,
        4023,
        4021,
        4024,
        2001,
        2003,
        ]

histxaxislabeloptions = {
        "TightLLSR_cutflow" : {"print_yield_bin_indices" : range(13,21)},
        "LooseLLSR_cutflow" : {"print_yield_bin_indices" : range(13,21)},
        # "TightETSR_cutflow" : {"print_yield_bin_indices" : range(13,21)},
        # "TightMTSR_cutflow" : {"print_yield_bin_indices" : range(13,21)},
        }

# cutflow
p.dump_plot(fnames=bkgs,
    sig_fnames=[
        "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        "{}/vbshww_c2v_m2.root".format(hadd_dir),
        "{}/vbshww_c2v_3.root".format(hadd_dir),
        ],
    data_fname="{}/data.root".format(hadd_dir),
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    usercolors=colors,
    dirname="cutflow/{}".format(study_name),
    filter_pattern="{}_cutflow".format(cutname_to_plot),
    dogrep=True,
    histxaxislabeloptions=histxaxislabeloptions,
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
        "human_format": False,
        },
    )

