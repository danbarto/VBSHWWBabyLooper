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
        # "{}/lambda20_vbshww.root".format(hadd_dir),
        # "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    # data_fname="{}/vbshww.root".format(hadd_dir),
    dirname="plots/cutflow/{}".format(study_name),
    filter_pattern="{}_cutflow".format(cutname_to_plot), # TODO this is not generalized yet
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
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
        # "{}/lambda20_vbshww.root".format(hadd_dir),
        # "{}/lambdam20_vbshww.root".format(hadd_dir),
        ],
    # data_fname="{}/vbshww.root".format(hadd_dir),
    dirname="plots/cutflow/{}".format(study_name),
    filter_pattern="{}_rawcutflow".format(cutname_to_plot), # TODO this is not generalized yet
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
        },
    )
