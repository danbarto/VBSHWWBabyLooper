#!/bin/env python

import plottery_wrapper as p

# # cutflow
# p.dump_plot(fnames=[
#         "hadds/wz.root",
#         "hadds/tt1l.root",
#         "hadds/tt2l.root",
#         "hadds/ttw.root",
#         "hadds/ttz.root",
#         "hadds/tth.root",
#         "hadds/ssww.root",
#         ],
#     sig_fnames=[
#         "hadds/vbshww.root",
#         "hadds/lambda20_vbshww.root",
#         "hadds/lambdam20_vbshww.root",
#         ],
#     filter_pattern="TwoTightLeptons__",
#     dogrep=True,
#     extraoptions={
#         "print_yield": True,
#         "nbins": 20,
#         # "signal_scale": 20,
#         "signal_scale": "auto",
#         },
#     # _plotter=p.plot_cut_scan,
#     )

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
    filter_pattern="TwoTightLeptons__nb",
    dogrep=True,
    extraoptions={
        "print_yield": True,
        # "nbins": 60,
        "signal_scale": "auto",
        "yaxis_range": [0., 5.],
        },
    _plotter=p.plot_cut_scan,
    )
