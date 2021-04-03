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

bkg_labels = [
        "WZ",
        "t#bar{t}(1l)",
        "t#bar{t}(2l)",
        "t#bar{t}W",
        "t#bar{t}Z",
        "W^{#pm}W^{#pm}jj",
        "V",
        "ZZ",
        "Rare t#bar{t}+X(Y)",
        "VVV"]
signal_labels = ["C2V=6", "C2V=4.5", "C2V=-2", "C2V=3"]
colors = [4022, 4020, 4023, 4021, 4024, 4101, 4201]

# cutflow
p.dump_plot(fnames=[
        "{}/wz.root".format(hadd_dir),
        "{}/tt1l.root".format(hadd_dir),
        "{}/tt2l.root".format(hadd_dir),
        "{}/ttw.root".format(hadd_dir),
        "{}/ttz.root".format(hadd_dir),
        "{}/ssww.root".format(hadd_dir),
        "{}/dy.root".format(hadd_dir),
        "{}/zz.root".format(hadd_dir),
        "{}/raretop.root".format(hadd_dir),
        "{}/vvv.root".format(hadd_dir),
        ],
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
    dirname="plots/cutflow/{}".format(study_name),
    filter_pattern="{}_cutflow".format(cutname_to_plot), # TODO this is not generalized yet
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
        "human_format": False,
        },
    )

# # cutflow
# p.dump_plot(fnames=[
#         "{}/wz.root".format(hadd_dir),
#         "{}/tt1l.root".format(hadd_dir),
#         "{}/tt2l.root".format(hadd_dir),
#         "{}/ttw.root".format(hadd_dir),
#         "{}/ttz.root".format(hadd_dir),
#         "{}/tth.root".format(hadd_dir),
#         "{}/ssww.root".format(hadd_dir),
#         "{}/wjet.root".format(hadd_dir),
#         ],
#     sig_fnames=[
#         "{}/vbshww_c2v_3.root".format(hadd_dir),
#         "{}/vbshww_c2v_m2.root".format(hadd_dir),
#         ],
#     legend_labels=bkg_labels,
#     signal_labels=signal_labels,
#     usercolors=colors,
#     dirname="plots/cutflow/{}".format(study_name),
#     filter_pattern="{}_rawcutflow".format(cutname_to_plot), # TODO this is not generalized yet
#     extraoptions={
#         "print_yield": True,
#         "yield_prec": 3,
#         },
#     )
