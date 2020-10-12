#!/bin/env python

import plottery_wrapper as p

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
    data_fname="hadds/vbshww.root",
    filter_pattern="SignalRegion_cutflow",
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
        },
    )
