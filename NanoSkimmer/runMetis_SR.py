import os
import time
from metis.StatsParser import StatsParser
from metis.Sample import DBSSample, DirectorySample
from metis.CondorTask import CondorTask

if __name__ == "__main__":

    # specify which samples to run over
    all_2016 = False
    all_2017 = False
    all_2018 = False
    all_mc = False
    all_data = False
    all_mc_data = True

    # Specify a dataset name and a short name for the output root file on nfs
    samples2016BG = [
        # 2016 BG MC
        DBSSample(
            dataset="/TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="831.762",
        ),
        DBSSample(
            dataset="/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="364.35",
        ),
        DBSSample(
            dataset="/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="87.31",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="18610",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM",
            xsec="6020.85",
        ),
        DBSSample(
            dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.01181",
        ),
        DBSSample(
            dataset="/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.01123",
        ),
        DBSSample(
            dataset="/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="2.967",
        ),
        DBSSample(
            dataset="/TTGamma_Dilept_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.632",
        ),
        DBSSample(
            dataset="/TTGamma_SingleLept_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.77",
        ),
        DBSSample(
            dataset="/TTHH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.000757",
        ),
        DBSSample(
            dataset="/TTTJ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.000474",
        ),
        DBSSample(
            dataset="/TTTT_TuneCUETP8M2T4_PSweights_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.01197",
        ),
        DBSSample(
            dataset="/TTTW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.000788",
        ),
        DBSSample(
            dataset="/TTWH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.001582",
        ),
        DBSSample(
            dataset="/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM",
            xsec="0.2043",
        ),
        DBSSample(
            dataset="/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.01150",
        ),
        DBSSample(
            dataset="/TTWZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.003884",
        ),
        DBSSample(
            dataset="/TTZH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.001535",
        ),
        DBSSample(
            dataset="/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext3-v1/NANOAODSIM",
            xsec="0.2529",
        ),
        DBSSample(
            dataset="/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.0493",
        ),
        DBSSample(
            dataset="/TTZZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.001982",
        ),
        DBSSample(
            dataset="/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="2.1360",
        ),
        DBSSample(
            dataset="/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext3-v1/NANOAODSIM",
            xsec="405.271",
        ),
        DBSSample(
            dataset="/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM",
            xsec="61334.9",
        ),
        DBSSample(
            dataset="/WWG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.2147",
        ),
        DBSSample(
            dataset="/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.16975",
        ),
        DBSSample(
            dataset="/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.2086",
        ),
        DBSSample(
            dataset="/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.1651",
        ),
        DBSSample(
            dataset="/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.04123",
        ),
        DBSSample(
            dataset="/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="4.4297",
        ),
        DBSSample(
            dataset="/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.05565",
        ),
        DBSSample(
            dataset="/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.05390",
        ),
        DBSSample(
            dataset="/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="123.9",
        ),
        DBSSample(
            dataset="/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="1.256",
        ),
        DBSSample(
            dataset="/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.01398",
        ),
        DBSSample(
            dataset="/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM",
            xsec="0.0758",
        ),
        DBSSample(
            dataset="/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.2710",
        ),
        # DBSSample(dataset="/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM", xsec = "1345"),
        # DBSSample(dataset="/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM", xsec = "359.7"),
        # DBSSample(dataset="/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM", xsec = "48.91"),
        # DBSSample(dataset="/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM", xsec = "12.05"),
        # DBSSample(dataset="/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext1-v1/NANOAODSIM", xsec = "5.501"),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="0.002703",
        ),
    ]

    samples2016Signal = [
        # 2016 Signal MC
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hut-MadGraph5-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hut-MadGraph5-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hct-MadGraph5-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hct-MadGraph5-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
    ]

    samples2017BG = [
        # 2017 BG MC
        DBSSample(
            dataset="/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.16975",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="18610",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017RECOSIMstep_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="6020.85",
        ),
        DBSSample(
            dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext3-v1/NANOAODSIM",
            xsec="0.01181",
        ),
        DBSSample(
            dataset="/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.01123",
        ),
        DBSSample(
            dataset="/TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="2.967",
        ),
        DBSSample(
            dataset="/TTGamma_Dilept_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.632",
        ),
        DBSSample(
            dataset="/TTGamma_SingleLept_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.77",
        ),
        DBSSample(
            dataset="/TTHH_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v2/NANOAODSIM",
            xsec="0.000757",
        ),
        DBSSample(
            dataset="/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="831.762",
        ),
        DBSSample(
            dataset="/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="364.35",
        ),
        DBSSample(
            dataset="/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="87.31",
        ),
        DBSSample(
            dataset="/TTTJ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.000474",
        ),
        DBSSample(
            dataset="/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.01197",
        ),
        DBSSample(
            dataset="/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.000788",
        ),
        DBSSample(
            dataset="/TTWH_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.001582",
        ),
        DBSSample(
            dataset="/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.2043",
        ),
        DBSSample(
            dataset="/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.01150",
        ),
        DBSSample(
            dataset="/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.003884",
        ),
        DBSSample(
            dataset="/TTZH_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.001535",
        ),
        DBSSample(
            dataset="/TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.2529",
        ),
        DBSSample(
            dataset="/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.0493",
        ),
        DBSSample(
            dataset="/TTZZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.001982",
        ),
        DBSSample(
            dataset="/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="2.1360",
        ),
        DBSSample(
            dataset="/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="61334.9",
        ),
        DBSSample(
            dataset="/WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="405.271",
        ),
        DBSSample(
            dataset="/WWG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.2147",
        ),
        DBSSample(
            dataset="/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.2086",
        ),
        DBSSample(
            dataset="/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.1651",
        ),
        DBSSample(
            dataset="/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.04123",
        ),
        DBSSample(
            dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="4.4297",
        ),
        DBSSample(
            dataset="/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.05565",
        ),
        DBSSample(
            dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.05390",
        ),
        DBSSample(
            dataset="/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="405.271",
        ),
        DBSSample(
            dataset="/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="1.256",
        ),
        DBSSample(
            dataset="/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.01398",
        ),
        DBSSample(
            dataset="/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.0758",
        ),
        DBSSample(
            dataset="/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="0.2710",
        ),
        # DBSSample(dataset="/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM", xsec = "1345"),
        # DBSSample(dataset="/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM", xsec = "359.7"),
        # DBSSample(dataset="/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM", xsec = "48.91"),
        # DBSSample(dataset="/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM", xsec = "12.05"),
        # DBSSample(dataset="/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM", xsec = "5.501"),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext2-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext2-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8_ext1-v1/NANOAODSIM",
            xsec="0.002703",
        ),
    ]

    samples2017Signal = [
        # 2017 Signal MC
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hut-MadGraph5-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hut-MadGraph5-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hct-MadGraph5-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hct-MadGraph5-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM",
            xsec="83.88144",
        ),
    ]

    samples2018BG = [
        # 2018 BG MC
        DBSSample(
            dataset="/WWTo2L2Nu_DoubleScattering_13TeV-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.16975",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="18610",
        ),
        DBSSample(
            dataset="/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="6020.85",
        ),
        DBSSample(
            dataset="/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM",
            xsec="831.762",
        ),
        DBSSample(
            dataset="/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="364.35",
        ),
        DBSSample(
            dataset="/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="87.31",
        ),
        DBSSample(
            dataset="/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.0493",
        ),
        DBSSample(
            dataset="/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.2710",
        ),
        DBSSample(
            dataset="/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM",
            xsec="0.01197",
        ),
        DBSSample(
            dataset="/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.2043",
        ),
        DBSSample(
            dataset="/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.2529",
        ),
        DBSSample(
            dataset="/VHToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="2.1360",
        ),
        DBSSample(
            dataset="/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.1651",
        ),
        DBSSample(
            dataset="/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.04123",
        ),
        DBSSample(
            dataset="/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.05565",
        ),
        DBSSample(
            dataset="/ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM",
            xsec="405.271",
        ),
        DBSSample(
            dataset="/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.01398",
        ),
        DBSSample(
            dataset="/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.01123",
        ),
        DBSSample(
            dataset="/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.0758",
        ),
        DBSSample(
            dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.01181",
        ),
        DBSSample(
            dataset="/TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="2.967",
        ),
        DBSSample(
            dataset="/TTGamma_Dilept_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.632",
        ),
        DBSSample(
            dataset="/TTGamma_SingleLept_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.77",
        ),
        DBSSample(
            dataset="/WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="405.271",
        ),
        DBSSample(
            dataset="/WWG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.2147",
        ),
        DBSSample(
            dataset="/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.2086",
        ),
        DBSSample(
            dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="4.4297",
        ),
        DBSSample(
            dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.05390",
        ),
        DBSSample(
            dataset="/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM",
            xsec="1.256",
        ),
        DBSSample(
            dataset="/TTHH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.000757",
        ),
        DBSSample(
            dataset="/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.003884",
        ),
        DBSSample(
            dataset="/TTZZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.001982",
        ),
        DBSSample(
            dataset="/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.000788",
        ),
        DBSSample(
            dataset="/TTTJ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.000474",
        ),
        DBSSample(
            dataset="/TTWH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.001582",
        ),
        DBSSample(
            dataset="/TTZH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.001535",
        ),
        DBSSample(
            dataset="/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM",
            xsec="0.01150",
        ),
        DBSSample(
            dataset="/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="61334.9",
        ),
        # DBSSample(dataset="/WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM", xsec = "1345"),
        # DBSSample(dataset="/WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM", xsec = "359.7"),
        # DBSSample(dataset="/WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM", xsec = "48.91"),
        # DBSSample(dataset="/WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM", xsec = "12.05"),
        # DBSSample(dataset="/WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM", xsec = "5.501"),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2mu_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2e2tau_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo2mu2tau_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.005423",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4e_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4mu_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext1-v1/NANOAODSIM",
            xsec="0.002703",
        ),
        DBSSample(
            dataset="/GluGluToContinToZZTo4tau_13TeV_MCFM701_pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="0.002703",
        ),
    ]

    samples2018Signal = [
        # 2018 Signal MC
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hut_TuneCP5-MadGraph5-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_tauDecays_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hut_TuneCP5-MadGraph5-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_tauDecays_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-TtoHJ_aTleptonic_HToWWZZtautau_eta_hct_TuneCP5-MadGraph5-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_tauDecays_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="83.88144",
        ),
        DBSSample(
            dataset="/TT_FCNC-aTtoHJ_Tleptonic_HToWWZZtautau_eta_hct_TuneCP5-MadGraph5-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_tauDecays_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            xsec="83.88144",
        ),
    ]

    samples2016Data = [
        # 2016Data
        DBSSample(dataset="/DoubleEG/Run2016B-02Apr2020_ver2-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016E-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016F-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016G-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2016H-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016B-02Apr2020_ver2-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016F-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016H-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016E-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2016G-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016B-02Apr2020_ver2-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016E-02Apr2020-v2/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016F-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016G-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2016H-02Apr2020-v1/NANOAOD"),
    ]

    samples2017Data = [
        # 2017 Data
        DBSSample(dataset="/DoubleEG/Run2017B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2017C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2017D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2017E-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleEG/Run2017F-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2017B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2017C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2017D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2017E-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2017F-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2017B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2017C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2017D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2017E-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2017F-02Apr2020-v1/NANOAOD"),
    ]

    samples2018Data = [
        # 2018 Data
        DBSSample(dataset="/EGamma/Run2018A-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/EGamma/Run2018B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/EGamma/Run2018C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/EGamma/Run2018D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2018A-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2018B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2018C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/DoubleMuon/Run2018D-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2018A-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2018B-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2018C-02Apr2020-v1/NANOAOD"),
        DBSSample(dataset="/MuonEG/Run2018D-02Apr2020-v1/NANOAOD"),
    ]

    # Aliases
    if all_2016:
        samples = samples2016BG + samples2016Signal + samples2016Data
    if all_2017:
        samples = samples2017BG + samples2017Signal + samples2017Data
    if all_2018:
        samples = samples2018BG + samples2018Signal + samples2018Data
    if all_mc:
        samples = (
            samples2016BG
            + samples2017BG
            + samples2018BG
            + samples2016Signal
            + samples2017Signal
            + samples2018Signal
        )
    if all_data:
        samples = samples2016Data + samples2017Data + samples2018Data
    if all_mc_data:
        samples = (
            samples2016BG
            + samples2016Signal
            + samples2016Data
            + samples2017BG
            + samples2017Signal
            + samples2017Data
            + samples2018BG
            + samples2018BG
            + samples2018Signal
            + samples2018Data
        )

    #samples = samples2017BG[5:6] + samples2017Signal[:1] + samples2017Data[:1]

    # submission tag
    tag = "fcnc_v10_SRonly_11june2021"

    task_list = []
    for sample in samples:
        task = CondorTask(
            sample=sample,
            files_per_output=1,
            output_name="output.root",
            tag=tag,
            condor_submit_params={"sites": "T2_US_UCSD", "use_xrootd": True},
            cmssw_version="CMSSW_10_2_9",
            input_executable="condor_executable_metis.sh",  # your condor executable here
            # tarfile = "/hadoop/cms/store/user/ksalyer/FCNC_NanoSkimmer_v2.package.tar.gz", # your tarfile with assorted goodies here
            tarfile="/hadoop/cms/store/user/dspitzba/FCNC/FCNC_NanoSkimmer_v10_SROnly.package.tar.gz",
            # tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v1.package.tar.gz",
            special_dir="FCNC_NanoSkim/{}".format(
                tag
            ),  # output files into /hadoop/cms/store/<user>/<special_dir>
            # special_dir = "VBSHWWNanoSkim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        task_list.append(task)
        # Straightforward logic
        # if not task.complete():
        # task.process()
    for i in range(100):
        total_summary = {}
        for task in task_list:
            task.process()
            total_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

        StatsParser(
            data=total_summary, webdir="~/public_html/dump/metis_FCNCBabies/"
        ).do()
        # power nap
        time.sleep(60.0 * 60)
