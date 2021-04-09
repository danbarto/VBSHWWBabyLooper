#!/bin/env python

import os

jobs = [
        ("LepPt1",    ","         , "Presel,"    ), # Presel
        ("LepPt1",    "Mjj,"      , "Only Mjj500,"   ), # Only Mjj
        ("231LepPt1", "231LepPt0,", "Only LeadLep140,"), # Only LepPt0
        ("321LepPt0", "321LepPt1,", "Only SubleadLep60,"), # Only LepPt1
        ("LepPt1",    "LepPt1,"   , "All Nominal Cuts,"), # All nominal cuts
    ]

categories = [
        "TightEE",
        "TightEM",
        "TightMM",
        "TightLL",
        "LooseEE",
        "LooseEM",
        "LooseMM",
        "LooseLL",
        "TightET",
        "TightMT",
        "TightLT",
    ]

mbbs = [
        "MbbOn",
        "MbbOff",
    ]


cmd = "python rooutil/yield_for_keynote.py cutflow/mainAnalysis_Run2/TightLLMbbOnLepPt1_cutflow.txt | grep \"tt\" | sed 's/,tt(1l)/,,,,tt(1l)/'"
os.system(cmd)

for job in jobs:
    for category in categories:
        for mbb in mbbs:
            cmd = "python rooutil/yield_for_keynote.py cutflow/mainAnalysis_Run2/{}{}{}_cutflow.txt | grep \"{}{}{}\" | sed 's/{}/{}/'".format(category, mbb, job[0], category, mbb, job[1], job[1], job[2])
            cmd += " | sed 's/Tight/Tight,/'"
            cmd += " | sed 's/Loose/Loose,/'"
            cmd += " | sed 's/MbbOn/,On Mbb-peak,/'"
            cmd += " | sed 's/MbbOff/,Off Mbb-peak,/'"
            os.system(cmd)
