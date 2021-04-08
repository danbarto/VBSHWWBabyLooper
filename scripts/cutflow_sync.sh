#!/bin/bash

mkdir -p cutflow_sync/

# rm cutflow_sync/c2v_4p5.root
# ./studies/mainAnalysis/doAnalysis -t Events -o cutflow_sync/c2v_4p5.root --scale1fb .00001404685235699000 -i /nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1_ext1_NANOAODSIM_v12/merged/output.root
# rm cutflow_sync/ttw.root
# ./studies/mainAnalysis/doAnalysis -t Events -o cutflow_sync/ttw.root --scale1fb .00007605836725804000 -i /nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1_NANOAODSIM_v12/merged/output.root

python - <<EOF

import plottery_wrapper as p
import sys

bkgs = [
        "cutflow_sync/c2v_4p5.root",
        "cutflow_sync/ttw.root",
        ]
bkg_labels = [
        "c2v_4p5",
        "ttw",
        ]

# cutflow
p.dump_plot(fnames=bkgs,
    dirname="cutflow_sync/",
    filter_pattern="LooseVRChannel_cutflow,LooseVRChannel_rawcutflow",
    extraoptions={
        "print_yield": True,
        "yield_prec": 3,
        "human_format": False,
        },
    )

EOF
