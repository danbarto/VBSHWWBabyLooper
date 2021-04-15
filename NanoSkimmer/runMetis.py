import os

from metis.Sample import DBSSample, DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser


if __name__ == "__main__":

    # Specify a dataset name and a short name for the output root file on nfs
    samples = [

            DBSSample(dataset="/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext2-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext2-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLL_M-1to10_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUgenV6_pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tWll_5f_LO_13TeV-MadGraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZG_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWW_4F_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWW_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZZ_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWH_TuneCUETP8M2T4_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWToLNuQQ_13TeV-powheg/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),

            DBSSample(dataset="/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017RECOSIMstep_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_EXT_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_v6_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext3-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZH_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v2/NANOAODSIM"),
            DBSSample(dataset="/TTZZ_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWH_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_new_pmx_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),

            DBSSample(dataset="/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLL_M-1to10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext2-v1/NANOAODSIM"),
            DBSSample(dataset="/GluGluHToZZTo4L_M125_13TeV_powheg2_JHUGenV7011_pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tWll_5f_LO_TuneCP5_PSweights_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/WZG_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/WWZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/WZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ZZZ_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWW_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext2-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZZ_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWH_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTTT_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext2-v1/NANOAODSIM"),
            DBSSample(dataset="/WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/ST_s-channel_4f_leptonDecays_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),

            # Data 2016
            DBSSample(dataset="/DoubleEG/Run2016B_ver1-Nano25Oct2019_ver1-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016B_ver2-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016G-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2016H-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016B_ver1-Nano25Oct2019_ver1-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016B_ver2-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016G-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2016H-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016B_ver1-Nano25Oct2019_ver1-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016B_ver2-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016G-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2016H-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016B_ver1-Nano25Oct2019_ver1-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016B_ver2-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016G-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2016H-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016B_ver1-Nano25Oct2019_ver1-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016B_ver2-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016G-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2016H-Nano25Oct2019-v1/NANOAOD"),

            # Data 2017
            DBSSample(dataset="/DoubleEG/Run2017B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2017C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2017D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2017E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleEG/Run2017F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2017B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2017C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2017D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2017E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2017F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2017B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2017C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2017D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2017E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2017F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2017B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2017C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2017D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2017E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2017F-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2017B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2017C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2017D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2017E-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleElectron/Run2017F-Nano25Oct2019-v1/NANOAOD"),

            # Data 2018
            DBSSample(dataset="/EGamma/Run2018A-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/EGamma/Run2018B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/EGamma/Run2018C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/EGamma/Run2018D-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2018A-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2018B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2018C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/DoubleMuon/Run2018D-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2018A-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2018B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2018C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/MuonEG/Run2018D-Nano25Oct2019_ver2-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2018A-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2018B-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2018C-Nano25Oct2019-v1/NANOAOD"),
            DBSSample(dataset="/SingleMuon/Run2018D-Nano25Oct2019-v1/NANOAOD"),

            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
                dataset = "/VBSWmpWmpHToLNuLNu_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
                ),
            # DirectorySample(
            #     location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_dKLambda_m20_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
            #     dataset = "/VBSWmpWmpHToLNuLNu_dKLambda_m20_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
            #     ),
            # DirectorySample(
            #     location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_dKLambda_20_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
            #     dataset = "/VBSWmpWmpHToLNuLNu_dKLambda_20_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
            #     ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/VBSHWWSignalGeneration/VBSWWH_C2V_6_20210313_RunIIAutumn18NanoAOD_v2/merged",
                dataset = "/VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/VBSHWWSignalGeneration/VBSWWH_C2V_m2_RunIIAutumn18NanoAOD_VBSWWH_C2V_m2_v3/merged",
                dataset = "/VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/VBSHWWSignalGeneration/VBSWWH_C2V_3_20210314_RunIIAutumn18NanoAOD_VBSWWH_C2V_3_v3/merged",
                dataset = "/VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/VBSHWWSignalGeneration/VBSWWH_C2V_4p5_RunIIAutumn18NanoAOD_VBSWWH_C2V_4p5_v3/merged",
                dataset = "/VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/VBSHWWSignalGeneration/VBSWWH_C2V_4p5_RunIIAutumn18NanoAOD_VBSWWH_C2V_4p5_v3_ext1/merged/",
                dataset = "/VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1_ext1/NANOAODSIM",
                ),

            ]

    # submission tag
    tag = "v12"
    tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v5_CMSSW_10_2_13_slc7_amd64_gcc700.package.tar.gz" # your tarfile with assorted goodies here
    tag = "v13"
    tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v6_CMSSW_10_2_13_slc7_amd64_gcc700.package.tar.gz" # your tarfile with assorted goodies here
    tag = "v14"
    tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v7_CMSSW_10_2_13_slc7_amd64_gcc700.package.tar.gz" # your tarfile with assorted goodies here

    task_summary = {}

    for sample in samples:
        task = CondorTask(
                sample = sample,
                files_per_output = 1,
                output_name = "output.root",
                tag = tag,
                # condor_submit_params = {"sites": "T2_US_UCSD", "use_xrootd":True, "classads": [ ["metis_extraargs", "fetch_nano"] ]},
                condor_submit_params = {"sites": "T2_US_UCSD", "use_xrootd":True},
                cmssw_version = "CMSSW_10_2_13",
                scram_arch = "slc7_amd64_gcc700",
                input_executable = "condor_executable_metis.sh", # your condor executable here
                tarfile = tarfile, # your tarfile with assorted goodies here
                special_dir = "VBSHWWNanoSkim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        # Straightforward logic
        if not task.complete():
            task.process()

        # Set task summary
        task_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

    # Parse the summary and make a summary.txt that will be used to pretty status of the jobs
    os.system("rm -f web_summary.json")
    os.system("rm -f summary.json")
    webdir="~/public_html/VBSHWWNanoSkimmerDashboard"
    StatsParser(data=task_summary, webdir=webdir).do()
    os.system("chmod -R 755 {}".format(webdir))
    os.system("msummary -r -i {}/web_summary.json".format(webdir))
