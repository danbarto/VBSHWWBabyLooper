#!/bin/env python

import plottery_wrapper as p

p.dump_plot(
        fnames=[
            "outputs/output_ttw.root",
            ],
        sig_fnames=[
            "outputs/signal.root",
            ],
        extraoptions={
            "signal_scale":"auto",
            }
        )
