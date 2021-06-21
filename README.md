# April 15th: Producing preliminary loose validation region plots

    git clone --recursive git@github.com:sgnoohc/VBSHWWBabyLooper.git
    cd VBSHWWBabyLooper
    source setup.sh
    make clean
    make -j
    sh scripts/run_v3.sh -s mainAnalysis # Runs all the jobs
    python scripts/cutflow.py mainAnalysis_Run2 # Produces every cutflow table possible
    python scripts/summary_table.py  > data.csv # Creates a .csv file of grand table
    cp data.csv table/ # If you want some pretty printing on your webpage
    python scripts/plot.py mainAnalysis_Run2 LooseVRChannel__ChannelsDetails

# Producing mbb extrapolation factor study plots

    python scripts/mbb_study.py

# Quick Instructions

First, fork this or the original (cmstas/VBSHWWBabyLooper) repo to user's area.   
Then, clone the repository to local area.

    git clone --recursive git@github.com:<yourgitusername>/VBSHWWBabyLooper.git
    cd VBSHWWBabyLooper
    git remote add upstream git@github.com:cmstas/VBSHWWBabyLooper.git
    git pull upstream main # to pull changes upstream repository in cmstas to local repo
    git pull origin main # to pull changes forked repository in cmstas to local repo
    source setup.sh
    make clean
    make -j
    sh scripts/run_v2.sh -s mainAnalysis # for more info, $ sh scripts/run.sh -h
    python scripts/cutflow.py mainAnalysis_Run2 SRLLDEtajj # To make cutflow for SignalRegionPreselection for full Run2
    python scripts/plot.py mainAnalysis_Run2 SRLLDEtajj # To make plots at SignalRegionPreselection cut selection level full Run2

# Running BDT input preparation code

Assuming you have compiled the code following the previous step instructions:

    sh scripts/run.sh -s bdtInputs

The outputs are located in

    ls hadds/bdtInputs_2016
    ls hadds/bdtInputs_2017 # For now, this is the only usable ones
    ls hadds/bdtInputs_2018
    ls hadds/bdtInputs_Run2

# Making changes to the code and making pull requests

    git pull origin main # Make sure you're at the latest code
    # vim <yourcode>
    git commit -a -m "Write a commit message"
    git pull upstream main # Make sure to merge any changes that is in the main repository
    # vim <conflictingcode> # Make any necessary merging and changes
    git push origin main # to push the local changes to the forked repository
    # Then go to https://github.com/<yourgitusername>/VBSHWWBabyLooper/pulls and click "New pull request" button

# Using the NanoSkimmer
Set your CMS environment:
    
    cmsrel CMSSW_10_0_0
    cd CMSSW_10_0_0/src/
    cmsenv

Check that the skim is appropriate for your purposes.
For this analysis, the skim is saved in a tarred directory stored on the uaf's hadoop:

    /hadoop/cms/store/user/ksalyer/FCNC_NanoSkimmer_v1.package.tar.gz

To find the skim, untar this directory by running

    tar xf /hadoop/cms/store/user/ksalyer/FCNC_NanoSkimmer_v1.package.tar.gz

This will produce a directory titled "PhysicsTools". The skim module is

    ./PhysicsTools/NanoAODTools/python/postprocessing/examples/vbsHwwSkimModule.py

If you edit the skim, be sure to re-tar the PhysicsTools directory in the CMSSW_10_0_0 environment to match the framework of the condor executable.

Local tests of the skimmer can be done by running:

    cd PhysicsTools/NanoAODTools
    scram b
    python scripts/nano_postproc.py \
        ./ \
        /MY/NANO/AOD/FILE.root \
        -b python/postprocessing/examples/keep_and_drop.txt \
        -I PhysicsTools.NanoAODTools.postprocessing.examples.vbsHwwSkimModule vbsHwwSkimModuleConstr
    
