#!/bin/env python

import ROOT as r
import plottery_wrapper as p

r.gROOT.SetBatch(True)

histxaxislabeloptions = {
        "Mbb"      : {"xaxis_label": "m_{bb} [GeV]", "xaxis_ndivisions" : 505, "nbins": 20},
        "MbbInOut" : {"xaxis_label": ""            , "xaxis_ndivisions" : 505, "nbins": 20, "bin_labels": ["Out", "In"]},
        }

def compare(output_name, sample1, sample2, histname1, histname2, legend1, legend2):
    ref_file = r.TFile("hadds/mainAnalysis_Run2/{}.root".format(sample1))
    mbb_ref = ref_file.Get("{}".format(histname1)).Clone()
    tar_file = r.TFile("hadds/mainAnalysis_Run2/{}.root".format(sample2))
    mbb_tar = tar_file.Get("{}".format(histname2)).Clone()
    mbb_ref.Scale(1. / mbb_ref.Integral())
    mbb_tar.Scale(1. / mbb_tar.Integral())

    colors = {
            "ttw"  : 4021,
            "ttz"  : 4024,
            "tt1l" : 4020,
            "tt2l" : 4023,
            "raretop" : 2001,
            "alltop" : r.kGray,
            }

    options={
            "legend_datalabel": legend2,
            "legend_ncolumns": 2,
            "legend_scalex": 2.0,
            "legend_scaley": 0.8,
            "nbins": 20,
            "output_name": "mbb_study/{}.pdf".format(output_name),
            "ratio_range": [0.5, 1.5] if "__MbbInOut" in histname1 else [0., 2.],
            "remove_underflow": True,
            "yaxis_label": "Normalized",
            "xaxis_label": "" if "__MbbInOut" in histname1 else "m_{bb} [GeV]",
            "lumi_value": 137,
            }
    if "__MbbInOut" in histname1:
        options["bin_labels"] = ["Out", "In"]

    p.plot_hist(
            bgs = [mbb_ref.Clone()],
            data = mbb_tar.Clone(),
            legend_labels=[legend1],
            colors=[colors[sample1]],
            options=options,
            )

def different_composition_test(histname):
    samples = ["ttw", "ttz", "tt1l", "tt2l", "raretop"]
    files = []
    hists = []
    for sample in samples:
        ref_file = r.TFile("hadds/mainAnalysis_Run2/{}.root".format(sample))
        files.append(ref_file)
        hists.append(ref_file.Get(histname))

    # Nominal
    nominal_totalbkg = hists[0].Clone("alltop")
    nominal_totalbkg.Add(hists[1].Clone())
    nominal_totalbkg.Add(hists[2].Clone())
    nominal_totalbkg.Add(hists[3].Clone())
    nominal_totalbkg.Add(hists[4].Clone())

    # Vary each i-th ones by factor 2 to get up-down variation
    hists_up = []
    hists_dn = []
    hists_up_indiv = []
    hists_dn_indiv = []
    for i in range(len(samples)):

        up_totalbkg = hists[0].Clone("alltop")
        up_totalbkg.Reset()
        uplist = []
        for j in range(len(samples)):
            temp = hists[j].Clone()
            if i == j:
                temp.Scale(3.)
            up_totalbkg.Add(temp)
            uplist.append(temp.Clone())
        sf = 1./up_totalbkg.Integral()           
        up_totalbkg.Scale(sf)
        hists_up.append(up_totalbkg)
        for h in uplist:
            h.Scale(sf)
        hists_up_indiv.append(uplist)

        dn_totalbkg = hists[0].Clone("alltop")
        dn_totalbkg.Reset()
        dnlist = []
        for j in range(len(samples)):
            temp = hists[j].Clone()
            if i == j:
                temp.Scale(1/3.)
            dn_totalbkg.Add(temp)
            dnlist.append(temp.Clone())
        sf = 1./dn_totalbkg.Integral()           
        dn_totalbkg.Scale(sf)
        hists_dn.append(dn_totalbkg)
        for h in dnlist:
            h.Scale(sf)
        hists_dn_indiv.append(dnlist)

    sf = 1./nominal_totalbkg.Integral()
    nominal_totalbkg.Scale(sf)
    for h in hists:
        h.Scale(sf)

    colors = [
            4021,
            4024,
            4020,
            4023,
            2001,
            ]

    for i, (uphist, dnhist, uphist_list, dnhist_list) in enumerate(zip(hists_up, hists_dn, hists_up_indiv, hists_dn_indiv)):

        options={
            "legend_ncolumns": 3,
            "legend_scalex": 2.0,
            "legend_scaley": 0.8,
            "nbins": 20,
            "output_name": "mbb_study/{}_{}_upvaried_detail.pdf".format(histname, samples[i]),
            "ratio_range": [0.5, 1.5] if "__MbbInOut" in histname else [0., 2.],
            "remove_underflow": True,
            "yaxis_label": "Normalized",
            "xaxis_label": "" if "__MbbInOut" in histname else "m_{bb} [GeV]",
            "lumi_value": 137,
            }
        if "__MbbInOut" in histname:
            options["bin_labels"] = ["Out", "In"]

        options["output_name"] = "mbb_study/{}_{}_upvaried_detail.pdf".format(histname, samples[i])
        p.plot_hist(
                bgs = [ h.Clone() for h in uphist_list ],
                data = nominal_totalbkg.Clone(),
                legend_labels=[ "{}x2".format(samples[j]) if j == i else samples[j] for j in range(len(samples)) ],
                colors=colors,
                options=options,
                )

        options["output_name"] = "mbb_study/{}_{}_dnvaried_detail.pdf".format(histname, samples[i])
        p.plot_hist(
                bgs = [ h.Clone() for h in dnhist_list ],
                data = nominal_totalbkg.Clone(),
                legend_labels=[ "{}x1/2".format(samples[j]) if j == i else samples[j] for j in range(len(samples)) ],
                colors=colors,
                options=options,
                )

        options["output_name"] = "mbb_study/{}_{}_varied.pdf".format(histname, samples[i])
        p.plot_hist(
                bgs = [nominal_totalbkg.Clone()],
                sigs = [uphist.Clone(), dnhist.Clone()],
                legend_labels=["All Top Bkg."],
                sig_labels=["{}x2".format(samples[i]), "{}x1/2".format(samples[i])],
                colors=[r.kGray],
                options=options,
                )

        options["output_name"] = "mbb_study/{}_{}_up_varied.pdf".format(histname, samples[i])
        p.plot_hist(
                bgs = [nominal_totalbkg.Clone()],
                data = uphist.Clone(),
                legend_labels=["All Top Bkg."],
                # sig_labels=["{}x2".format(samples[i]), "{}x1/2".format(samples[i])],
                colors=[r.kGray],
                options=options,
                )

        options["output_name"] = "mbb_study/{}_{}_dn_varied.pdf".format(histname, samples[i])
        p.plot_hist(
                bgs = [nominal_totalbkg.Clone()],
                data = dnhist.Clone(),
                legend_labels=["All Top Bkg."],
                # sig_labels=["{}x2".format(samples[i]), "{}x1/2".format(samples[i])],
                colors=[r.kGray],
                options=options,
                )

def main(selection):

    different_composition_test("TightLLMbbAll{}__Mbb".format(selection))
    different_composition_test("TightLLMbbAll{}__MbbInOut".format(selection))
    different_composition_test("LooseLLMbbAll{}__Mbb".format(selection))
    different_composition_test("LooseLLMbbAll{}__MbbInOut".format(selection))
    different_composition_test("TightLTMbbAll{}__Mbb".format(selection))
    different_composition_test("TightLTMbbAll{}__MbbInOut".format(selection))

    samples       = [ "ttw", "ttz", "tt1l", "tt2l", "raretop", "alltop" ]
    sample_labels = [ "t#bar{t}W", "t#bar{t}Z", "t#bar{t}#rightarrow1l", "t#bar{t}#rightarrow2l", "t#bar{t}+X(Y)", "All t#bar{t}" ]

    for sample, sample_label in zip(samples, sample_labels):
        for var in ["Mbb", "MbbInOut"]:
            ## can combine per lepton channels
            compare("TightLL_{}_ll_v_ee_{}".format(sample, var), sample, sample, "TightLLMbbAll{}__{}".format(selection, var), "TightEEMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (ee)".format(sample_label))
            compare("TightLL_{}_ll_v_em_{}".format(sample, var), sample, sample, "TightLLMbbAll{}__{}".format(selection, var), "TightEMMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (e#mu)".format(sample_label))
            compare("TightLL_{}_ll_v_mm_{}".format(sample, var), sample, sample, "TightLLMbbAll{}__{}".format(selection, var), "TightMMMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (#mu#mu)".format(sample_label))
            compare("LooseLL_{}_ll_v_ee_{}".format(sample, var), sample, sample, "LooseLLMbbAll{}__{}".format(selection, var), "LooseEEMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (ee)".format(sample_label))
            compare("LooseLL_{}_ll_v_em_{}".format(sample, var), sample, sample, "LooseLLMbbAll{}__{}".format(selection, var), "LooseEMMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (e#mu)".format(sample_label))
            compare("LooseLL_{}_ll_v_mm_{}".format(sample, var), sample, sample, "LooseLLMbbAll{}__{}".format(selection, var), "LooseMMMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (#mu#mu)".format(sample_label))
            compare("TightLT_{}_lt_v_et_{}".format(sample, var), sample, sample, "TightLTMbbAll{}__{}".format(selection, var), "TightETMbbAll{}__{}".format(selection, var), "{} (l#tau)".format(sample_label), "{} (e#tau)".format(sample_label))
            compare("TightLT_{}_lt_v_mt_{}".format(sample, var), sample, sample, "TightLTMbbAll{}__{}".format(selection, var), "TightMTMbbAll{}__{}".format(selection, var), "{} (l#tau)".format(sample_label), "{} (#mu#tau)".format(sample_label))
            compare("TightLX_{}_ll_v_lt_{}".format(sample, var), sample, sample, "TightLLMbbAll{}__{}".format(selection, var), "TightLTMbbAll{}__{}".format(selection, var), "{} (ll)".format(sample_label), "{} (l#tau)".format(sample_label))

    for sample, sample_label in zip(samples, sample_labels):
        for var in ["Mbb", "MbbInOut"]: #, "MbbBothGenMatched"]:
            # cannot combine Tight v. Loose
            compare("LL_{}_T_v_L_{}".format(sample, var), sample, sample, "LooseLLMbbAll{}__{}".format(selection, var), "TightLLMbbAll{}__{}".format(selection, var), "{} (Loose)".format(sample_label), "{} (Tight)".format(sample_label))

    for i in range(len(samples)):
        for j in range(i + 1, len(samples)):

            isample = samples[i]
            isample_label = sample_labels[i]
            jsample = samples[j]
            jsample_label = sample_labels[j]

            for var in ["Mbb", "MbbInOut"]:
                # can combine each individual to alltop
                compare("TightLL_{}_v_{}_{}".format(isample, jsample, var), isample, jsample, "TightLLMbbAll{}__{}".format(selection, var), "TightLLMbbAll{}__{}".format(selection, var), "{} (ll)".format(isample_label), "{} (ll)".format(jsample_label))
                compare("LooseLL_{}_v_{}_{}".format(isample, jsample, var), isample, jsample, "LooseLLMbbAll{}__{}".format(selection, var), "LooseLLMbbAll{}__{}".format(selection, var), "{} (ll)".format(isample_label), "{} (ll)".format(jsample_label))
                compare("TightLT_{}_v_{}_{}".format(isample, jsample, var), isample, jsample, "TightLTMbbAll{}__{}".format(selection, var), "TightLTMbbAll{}__{}".format(selection, var), "{} (ll)".format(isample_label), "{} (ll)".format(jsample_label))

if __name__ == "__main__":

    # main("")
    main("VBF")
