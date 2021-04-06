#!/bin/env python

import glob
import os

# This script starts from the stdout of the exec_v3.sh and parse them into a table.

exec_v3_script_log = open("log.txt")

lines = [ line.replace("\n", ",") for line in exec_v3_script_log.readlines() if ("===========" not in line and "..." not in line and len(line.strip()) > 0 and "Done!" not in line) ]


# Order of content being printed
# Sample                            :
# Year                              :
# Nano tag                          :
# N events information file         :
# N total events                    :
# N eff total events (i.e. pos-neg) :
# Cross section (pb)                :
# Scale1fb                          :

lines_iter = iter(lines)

samples = []
for items in zip(lines_iter, lines_iter, lines_iter, lines_iter, lines_iter, lines_iter, lines_iter, lines_iter):
    item = ",".join(items)
    d = dict()
    for pair in item.split(","):
        if len(pair.strip()) == 0:
            continue
        vals = [ x.strip() for x in pair.split(":") ]
        d[vals[0]] = vals[1]
    samples.append(d)

# {'N total events': '1', 'N events information file': '/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//MuonEG_Run2018**/nevents.txt', 'Scale1fb': '1', 'Sample': 'MuonEG_Run2018', 'Nano tag': '', 'Year': '2018', 'N eff total events (i.e. pos-neg)': '1', 'Cross section (pb)': '1'}

def print_samples_table_twiki_style(year):
    ofile_mc_bkg = open("sample_list_" + str(year) + "_mc_bkg.txt", "w")
    ofile_mc_sig = open("sample_list_" + str(year) + "_mc_sig.txt", "w")
    ofile_data   = open("sample_list_" + str(year) + "_data.txt", "w")

    ofile_mc_bkg.write("---+++ {} MC background samples\n".format(year))
    ofile_mc_bkg.write("<br />%EDITTABLE{}% \n")
    ofile_mc_bkg.write("| *" + "* | *".join([ "Sample Name", "Path", "# of MC Events", "# of MC Eff. Events", "Cross section (pb)" ]) + "* |\n")

    ofile_mc_sig.write("---+++ {} MC signal samples\n".format(year))
    ofile_mc_sig.write("<br />%EDITTABLE{}% \n")
    ofile_mc_sig.write("| *" + "* | *".join([ "Sample Name", "Path", "# of MC Events", "# of MC Eff. Events", "Cross section (pb)" ]) + "* |\n")

    ofile_data.write("---+++ {} Data samples\n".format(year))
    ofile_data.write("<br />%EDITTABLE{}% \n")
    ofile_data.write("| *" + "* | *".join([ "Sample Name", "Path", "# of MC Events", "# of MC Eff. Events", "Cross section (pb)" ]) + "* |\n")

    motherdir = samples[0]["N events information file"].split("VBSHWWNanoSkim_")[0] + "VBSHWWNanoSkim_"
    version = samples[0]["N events information file"].split("VBSHWWNanoSkim_")[1].split("/")[0]
    basedir = motherdir + version

    for sample in samples:
        if int(year) != int(sample["Year"]):
            continue
        sample_name = sample["Sample"]
        nano_tag = sample["Nano tag"]
        path = os.path.join(basedir, sample_name + "*" + nano_tag + "*")
        nevents_path = glob.glob(sample["N events information file"])
        is_data = True if "Run201" in sample_name else False
        is_signal = True if "VBSWmpWmpH" in sample_name else False
        n_events = sample["N total events"]
        neff_events = sample["N eff total events (i.e. pos-neg)"]
        xsec = sample["Cross section (pb)"]
        dirpaths = glob.glob(path + "/merged/output.root")
        if is_data:
            for dirpath in dirpaths:
                sample_path = dirpath
                ofile_data.write("| " + " | ".join([ "!"+sample_name, sample_path, "-1", "-1", "-1" ]) + " |\n")
        else:
            if is_signal:
                sample_path = dirpaths[0]
                ofile_mc_sig.write("| " + " | ".join([ "!"+sample_name, sample_path, n_events, neff_events, xsec ]) + " |\n")
            else:
                sample_path = dirpaths[0]
                ofile_mc_bkg.write("| " + " | ".join([ "!"+sample_name, sample_path, n_events, neff_events, xsec ]) + " |\n")
    ofile_mc_bkg.write("\n")
    ofile_mc_sig.write("\n")
    ofile_data.write("\n")

print_samples_table_twiki_style(2016)
print_samples_table_twiki_style(2017)
print_samples_table_twiki_style(2018)
