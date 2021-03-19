## Setting up local run of the NanoSkimmer

Following will set up a version of NanoSkimmer for VBSHWW analysis

    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH=slc7_amd64_gcc700
    cmsrel CMSSW_10_2_13
    cd CMSSW_10_2_13/src/
    cmsenv
    tar xvf /nfs-7/userdata/phchang/VBSHWWNanoSkimmer_v2.package.tar.gz



## NanoSkimmer

First git clone ProjectMetis

    git clone git@github.com:aminnj/ProjectMetis.git

Then run ProjectMetis setup script which adds the package to PYTHONPATH

    cd ProjectMetis
    source setup.sh
    cd ..

Then change the ```tag``` in the ```runMetis.py``` to your liking and submit

    python runMetis.py

The submission will create ```tasks/``` directory with each job sample job submissions.
The subdirectories will contain input files, submission, stdout logs, error logs, etc.

To re-submit only the failed jobs just run the script again with same ```tag``` setting.  
Then, the script will check the output and use predefined submit scripts under ```tasks/```

    python runMetis.py
