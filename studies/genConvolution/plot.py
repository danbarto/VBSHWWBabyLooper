#!/bin/env python

import plottery_wrapper as p

p.dump_plot(
        fnames=["hadds/genConvolution_2017/vbshww.root"],
        dirname="plots/genConvolution/",
        filter_pattern="TagVBSJets",
        dogrep=True,
        )
