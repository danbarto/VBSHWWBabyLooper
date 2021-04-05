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
        ]
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
        "VVV",
        ]
colors = [
        4022,
        4020,
        4023,
        4021,
        4024,
        4101,
        4201,
        2005,
        2001,
        2003,
        ]

sigs = [
        "{}/vbshww_c2v_6.root".format(hadd_dir),
        "{}/vbshww_c2v_4p5.root".format(hadd_dir),
        "{}/vbshww_c2v_3.root".format(hadd_dir),
        "{}/vbshww_c2v_m2.root".format(hadd_dir),
        ]

signal_labels = [
        "C_{2V}=6",
        "C_{2V}=4.5",
        "C_{2V}=3",
        "C_{2V}=-2",
        ]

data_fname = "{}/data.root".format(hadd_dir)

lumi = -1
if "2016" in hadd_dir: lumi = 35.9
if "2017" in hadd_dir: lumi = 41.5
if "2018" in hadd_dir: lumi = 59.97
if "Run2" in hadd_dir: lumi = 137

histxaxislabeloptions = {
        "SRLLLepPt0__LeptonPt1"    : {"xaxis_label"      : "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":   5, "blind":  True, "extra_lines":[( 60., 0.,  60.,  17.)], "extra_text" : ["p_{T,sublead-lep} > 60 GeV"],                                 "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "SRLLLepPt1__MJJ"          : {"xaxis_label"      : "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":   5, "blind":  True, "extra_lines":[(500., 0., 500.,  11.)], "extra_text" : ["m_{jj} > 500 GeV"],                                           "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},

        "SRLLMbb__Mbb"             : {"xaxis_label"      : "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":  20, "blind":  True, "extra_lines":[(150., 0., 150.,  55.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "SRLLMbb__LeptonPt0"       : {"xaxis_label"      : "p_{T,lead-lep} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True, "extra_lines":[(140., 0., 140.,  70.)], "extra_text" : ["p_{T,lead-lep} > 140 GeV"],                                   "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "SRLLMbb__LeptonPt1"       : {"xaxis_label"      : "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True, "extra_lines":[( 60., 0.,  60., 150.)], "extra_text" : ["p_{T,sublead-lep} > 60 GeV"],                                 "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "SRLLMbb__HiggsPt"         : {"xaxis_label"      : "p_{T,bb} [GeV]"          , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__DRbb"            : {"xaxis_label"      : "#DeltaR_{bb}"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__MET"             : {"xaxis_label"      : "MET [GeV]"               , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__Mll"             : {"xaxis_label"      : "m_{ll} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__Mll"             : {"xaxis_label"      : "m_{ll} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__MJJ"             : {"xaxis_label"      : "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__DEtaJJ"          : {"xaxis_label"      : "#Delta#eta_{jj}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__DPhill"          : {"xaxis_label"      : "#Delta#phi_{ll}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__DRll"            : {"xaxis_label"      : "#DeltaR_{ll}"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__DEtall"          : {"xaxis_label"      : "#Delta#eta_{ll}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetPt0"          : {"xaxis_label"      : "p_{T,lead-jet} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetPt1"          : {"xaxis_label"      : "p_{T,sublead-jet} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetPt0"         : {"xaxis_label"      : "p_{T,lead-b} [GeV]"      , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetPt1"         : {"xaxis_label"      : "p_{T,sublead-b} [GeV]"   , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetEta0"         : {"xaxis_label"      : "#eta_{lead-jet}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetEta1"         : {"xaxis_label"      : "#eta_{sublead-jet}"      , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetEta0"        : {"xaxis_label"      : "#eta_{lead-b}"           , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetEta1"        : {"xaxis_label"      : "#eta_{sublead-b}"        , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetPhi0"         : {"xaxis_label"      : "#phi_{lead-jet}"         , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__JetPhi1"         : {"xaxis_label"      : "#phi_{sublead-jet}"      , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetPhi0"        : {"xaxis_label"      : "#phi_{lead-b}"           , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },
        "SRLLMbb__BJetPhi1"        : {"xaxis_label"      : "#phi_{sublead-b}"        , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":  20, "blind":  True                                                                                                                                                                                                  },

        "CRLLMbb__Mbb"             : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  55.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLLMbb__LeptonPt0"       : {"ratio_xaxis_title": "p_{T,lead-lep} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False, "extra_lines":[(140., 0., 140.,  70.)], "extra_text" : ["p_{T,lead-lep} > 140 GeV"],                                   "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "CRLLMbb__LeptonPt1"       : {"ratio_xaxis_title": "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False, "extra_lines":[( 60., 0.,  60., 150.)], "extra_text" : ["p_{T,sublead-lep} > 60 GeV"],                                 "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "CRLLMbb__HiggsPt"         : {"ratio_xaxis_title": "p_{T,bb} [GeV]"          , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False                                                                                                                                                                                                  },
        "CRLLMbb__DRbb"            : {"ratio_xaxis_title": "#DeltaR_{bb}"            , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False                                                                                                                                                                                                  },
        "CRLLMbb__MET"             : {"ratio_xaxis_title": "MET [GeV]"               , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False                                                                                                                                                                                                  },

        "CRLLChannel__Mbb"         : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":  20, "blind": False, "extra_lines":[(150., 0., 150.,  80.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLLLepPt0__Mbb"          : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 10, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  20.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLLLepPt1__Mbb"          : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins":  6, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  17.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLLMjj__Mbb"             : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins":  5, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  6.5)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLL__Mbb"                : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins":  5, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  6.5)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},
        "CRLLLowLep__Mbb"          : {"ratio_xaxis_title": "m_{bb} [GeV]"            , "xaxis_ndivisions" : 505, "nbins":  6, "signal_scale":   1, "blind": False, "extra_lines":[(150., 0., 150.,  30.)], "extra_text" : ["m_{bb} < 150 GeV (SR)                m_{bb} > 150 GeV (CR)"], "extra_text_xpos": 0.25, "extra_text_ypos": 0.63, "extra_text_size": 0.03},

        "CRLLLepPt0__LeptonPt1"    : {"ratio_xaxis_title": "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 20, "signal_scale":   1, "blind": False, "extra_lines":[( 60., 0.,  60.,  70.)], "extra_text" : ["p_{T,sublead-lep} > 60 GeV"],                                 "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "CRLLLepPt1__MJJ"          : {"ratio_xaxis_title": "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 45, "signal_scale":   1, "blind": False, "extra_lines":[(500., 0., 500.,  70.)], "extra_text" : ["m_{jj} > 500 GeV"],                                           "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "CRLLLowLep__MJJ"          : {"ratio_xaxis_title": "m_{jj} [GeV]"            , "xaxis_ndivisions" : 505, "nbins": 30, "signal_scale":   1, "blind": False, "extra_lines":[(500., 0., 500.,  11.)], "extra_text" : ["m_{jj} > 500 GeV"],                                           "extra_text_xpos": 0.45, "extra_text_ypos": 0.63, "extra_text_size": 0.04},
        "CRLLLowLep__LeptonPt0Zoom": {"ratio_xaxis_title": "p_{T,lead-lep} [GeV]"    , "xaxis_ndivisions" : 505, "nbins": 15, "signal_scale":   1, "blind": False                                                                                                                                                                                                  },
        "CRLLLowLep__LeptonPt1Zoom": {"ratio_xaxis_title": "p_{T,sublead-lep} [GeV]" , "xaxis_ndivisions" : 505, "nbins": 15, "signal_scale":   1, "blind": False                                                                                                                                                                                                  },
        }

dogrep = True
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
    )

