#!/bin/env python

import os

jobs = [
        ("SR"     , ","       , "Presel,"             ) ,
        ("VBF"    , "VBF,"    , "Only VBF cuts,"      ) ,
        ("LepPts" , "LepPts," , "Only Lepton PT cuts,"),
        ("SR"     , "SR,"     , "All Nominal Cuts,"   ) ,
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


cmd = "python rooutil/yield_for_keynote.py cutflow/mainAnalysis_Run2/TightLLMbbOnSR_cutflow.txt | grep \"tt\" | sed 's/,tt(1l)/,,,,tt(1l)/'"
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
