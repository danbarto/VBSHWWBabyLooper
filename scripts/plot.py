#!/bin/env python

import plottery_wrapper as p
import sys
import ROOT as r
r.gROOT.SetBatch(True)

def usage():
    print("Usage:")
    print("")
    print("   python {} [STUDYNAME=mainAnalysis_Run2] [PLOTNAMES]".format(sys.argv[0]))
    print("")
    print("")
    sys.exit()


try:
    study_name = sys.argv[1]
except:
    study_name = "mainAnalysis_Run2"

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
colors = [
        4020,
        4023,
        4021,
        4024,
        2001,
        2003,
        ]

sigs = [
        # "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        # "{}/vbshww_c2v_3.root".format(hadd_dir),
        # "{}/vbshww_c2v_m2.root".format(hadd_dir),
        ]

signal_labels = [
        # "C_{2V}=6",
        "C_{2V}=4.5",
        # "C_{2V}=3",
        # "C_{2V}=-2",
        ]

data_fname = "{}/data.root".format(hadd_dir)

lumi = -1
if "2016" in hadd_dir: lumi = 35.9
if "2017" in hadd_dir: lumi = 41.5
if "2018" in hadd_dir: lumi = 59.97
if "Run2" in hadd_dir: lumi = 137

ChannelsDetails_bin_labels = [
"T-ee-in",
"T-ee-out",
"T-e#mu-in",
"T-e#mu-out",
"T-#mu#mu-in",
"T-#mu#mu-out",
"L-ee-in",
"L-ee-out",
"L-e#mu-in",
"L-e#mu-out",
"L-#mu#mu-in",
"L-#mu#mu-out",
"T-e#tau-in",
"T-e#tau-out",
"T-#mu#tau-in",
"T-#mu#tau-out",
]

histxaxislabeloptions = {
        "LL__Mb0b1MET"             : {"xaxis_label"      : "m_{bbMET} [GeV]"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "remove_overflow":True},
        "TightLLChannel__MJJ"      : {"xaxis_label"      : "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto" ,"blind":True},
        "TightLLChannel__LeptonPt0": {"xaxis_label"      : "p_{T,lead-lep} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto" ,"blind":True},
        "TightLLChannel__LeptonPt1": {"xaxis_label"      : "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto" ,"blind":True},
        "Mbb"                      : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 18, "signal_scale":   1                },
        "TightLLChannel__MJJ"      : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 18, "signal_scale":   1                },
        "TightLLCRMjj__Mbb"        : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 18, "signal_scale":   1                },
        "TightLLCRLepPt0__Mbb"     : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 12, "signal_scale":   1                },
        "TightLLCRLepPt1__Mbb"     : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 12, "signal_scale":   1                },
        "LeptonPt0"                : {"xaxis_label"      : "p_{T,lead-lep} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto"            },
        "LeptonPt1"                : {"xaxis_label"      : "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto"            },
        "LeptonPtFlavor0"          : {"xaxis_label"      : "p_{T,type0} [GeV]"       , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto"            },
        "LeptonPtFlavor1"          : {"xaxis_label"      : "p_{T,type1} [GeV]"       , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  "auto"            },
        "HiggsPt"                  : {"xaxis_label"      : "p_{T,bb} [GeV]"          , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "DRbb"                     : {"xaxis_label"      : "#DeltaR_{bb}"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "MET"                      : {"xaxis_label"      : "MET [GeV]"               , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "METLow"                   : {"xaxis_label"      : "MET [GeV]"               , "xaxis_ndivisions" : 505, "nbins": 180, "signal_scale":   1                },
        "Mll"                      : {"xaxis_label"      : "m_{ll} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "Mll"                      : {"xaxis_label"      : "m_{ll} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "MJJ"                      : {"xaxis_label"      : "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "DEtaJJ"                   : {"xaxis_label"      : "#Delta#eta_{jj}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "DPhill"                   : {"xaxis_label"      : "#Delta#phi_{ll}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "DRll"                     : {"xaxis_label"      : "#DeltaR_{ll}"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "DEtall"                   : {"xaxis_label"      : "#Delta#eta_{ll}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "JetPt0"                   : {"xaxis_label"      : "p_{T,lead-jet} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "JetPt1"                   : {"xaxis_label"      : "p_{T,sublead-jet} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "BJetPt0"                  : {"xaxis_label"      : "p_{T,lead-b} [GeV]"      , "xaxis_ndivisions" : 505, "nbins": 45, "signal_scale":   1                },
        "BJetPt1"                  : {"xaxis_label"      : "p_{T,sublead-b} [GeV]"   , "xaxis_ndivisions" : 505, "nbins": 45, "signal_scale":   1                },
        "JetEta0"                  : {"xaxis_label"      : "#eta_{lead-jet}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "JetEta1"                  : {"xaxis_label"      : "#eta_{sublead-jet}"      , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "BJetEta0"                 : {"xaxis_label"      : "#eta_{lead-b}"           , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "BJetEta1"                 : {"xaxis_label"      : "#eta_{sublead-b}"        , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "JetPhi0"                  : {"xaxis_label"      : "#phi_{lead-jet}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "JetPhi1"                  : {"xaxis_label"      : "#phi_{sublead-jet}"      , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "BJetPhi0"                 : {"xaxis_label"      : "#phi_{lead-b}"           , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "BJetPhi1"                 : {"xaxis_label"      : "#phi_{sublead-b}"        , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "Channels"                 : {"xaxis_label"      : "Channels"                , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                },
        "ChannelsDetails"          : {"xaxis_label"      : ""                        , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1                , "bin_labels": ChannelsDetails_bin_labels},
        }

dogrep = False
if len(cutname_to_plot) == 0:
    filter_pattern = ",".join(histxaxislabeloptions.keys())
    dogrep = False
else:
    filter_pattern = ""
    if "," in cutname_to_plot:
        filter_patterns = []
        for cn in cutname_to_plot.split(","):
            if "__" in cn:
                filter_patterns.append("{}".format(cn))
            else:
                filter_patterns.append("{}__".format(cn))
        filter_pattern = ",".join(filter_patterns)
    else:
        if "__" in cutname_to_plot:
            filter_pattern = "{}".format(cutname_to_plot)
        else:
            filter_pattern = "{}__".format(cutname_to_plot)

if "*" in filter_pattern:
    dogrep = True

# cutflow
p.dump_plot(fnames=bkgs,
    sig_fnames=sigs,
    data_fname=data_fname,
    legend_labels=bkg_labels,
    signal_labels=signal_labels,
    dirname="plots/{}".format(study_name),
    filter_pattern=filter_pattern,
    dogrep=dogrep,
    usercolors=colors,
    extraoptions={
        "print_yield": True,
        "nbins": 20,
        "signal_scale": 1,
        "legend_ncolumns": 3,
        "legend_scalex": 2.0,
        "lumi_value": lumi,
        "ratio_range": [0., 2.],
        },
    histxaxislabeloptions=histxaxislabeloptions,
    skip2d=True,
    )

