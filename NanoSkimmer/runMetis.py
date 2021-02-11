import os

from metis.Sample import DBSSample, DirectorySample
from metis.CondorTask import CondorTask

if __name__ == "__main__":

    # Specify a dataset name and a short name for the output root file on nfs
    samples = [

            DBSSample(dataset="/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_ext_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17NanoAODv6-PU2017_12Apr2018_Nano25Oct2019_102X_mc2017_realistic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_EXT_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17NanoAODv7-PU2017_12Apr2018_Nano02Apr2020_new_pmx_102X_mc2017_realistic_v8-v1/NANOAODSIM"),

            DBSSample(dataset="/TTJets_DiLept_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20_ext1-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCP5_13TeV-madgraph-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1/NANOAODSIM"),

            DBSSample(dataset="/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTWJetsToLNu_TuneCP5_PSweights_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/TTZToLLNuNu_M-10_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/ttHTobb_ttToSemiLep_M125_TuneCP5_13TeV-powheg-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),
            DBSSample(dataset="/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16NanoAODv6-PUMoriond17_Nano25Oct2019_102X_mcRun2_asymptotic_v7-v1/NANOAODSIM"),

            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
                dataset = "/VBSWmpWmpHToLNuLNu_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_dKLambda_m20_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
                dataset = "/VBSWmpWmpHToLNuLNu_dKLambda_m20_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
                ),
            DirectorySample(
                location = "/hadoop/cms/store/user/phchang/nanoaod/VBSWmpWmpHToLNuLNu_dKLambda_20_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_NANOAODSIM",
                dataset = "/VBSWmpWmpHToLNuLNu_dKLambda_20_TuneCP5_13TeV-madgraph-pythia8/PRIVATE_RunIIFall17NanoAODv7-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/NANOAODSIM",
                ),

            ]

    # submission tag
    tag = "v6"

    for sample in samples:
        task = CondorTask(
                sample = sample,
                files_per_output = 1,
                output_name = "output.root",
                tag = tag,
                condor_submit_params = {"sites": "T2_US_UCSD", "use_xrootd":True},
                cmssw_version = "CMSSW_10_5_0",
                input_executable = "condor_executable_metis.sh", # your condor executable here
                tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v1.package.tar.gz", # your tarfile with assorted goodies here
                special_dir = "VBSHWWNanoSkim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        # Straightforward logic
        if not task.complete():
            task.process()
