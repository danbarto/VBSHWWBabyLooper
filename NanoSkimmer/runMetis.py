import os

from metis.Sample import DBSSample, DirectorySample
from metis.CondorTask import CondorTask

if __name__ == "__main__":

    # Specify a dataset name and a short name for the output root file on nfs
    samples = [

            DBSSample(dataset="/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16NanoAODv7-PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8_ext2-v1/NANOAODSIM"),
    ]


    # submission tag
    tag = "ksalyer_fcnc_test1"

    for sample in samples:
        task = CondorTask(
                sample = sample,
                files_per_output = 1,
                output_name = "output.root",
                tag = tag,
                condor_submit_params = {"sites": "T2_US_UCSD", "use_xrootd":True},
                cmssw_version = "CMSSW_10_0_0",
                input_executable = "condor_executable_metis.sh", # your condor executable here
                tarfile = "/hadoop/cms/store/user/ksalyer/FCNC_NanoSkimmer_v1.package.tar.gz", # your tarfile with assorted goodies here
                #tarfile = "/nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v1.package.tar.gz",
                special_dir = "FCNC_NanoSkim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
                #special_dir = "VBSHWWNanoSkim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        # Straightforward logic
        if not task.complete():
            task.process()
