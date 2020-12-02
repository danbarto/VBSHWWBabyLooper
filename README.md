# Quick Instructions

First fork the following repo to user's area.  
Then, clone the repository to local area.

    git clone --recursive git@github.com:<yourgitusername>/VBSHWWBabyLooper.git
    cd VBSHWWBabyLooper
    git remote add upstream git@github.com:cmstas/VBSHWWBabyLooper.git
    git pull upstream main # to pull changes upstream repository in cmstas to local repo
    git pull origin main # to pull changes forked repository in cmstas to local repo
    source setup.sh
    make clean; make -j
    sh scripts/run.sh

# Making changes to the code and making pull requests

    git pull origin main # Make sure you're at the latest code
    # vim <yourcode>
    git commit -a -m "Write a commit message"
    git pull upstream main # Make sure to merge any changes that is in the main repository
    # vim <conflictingcode> # Make any necessary merging and changes
    git push origin main # to push the local changes to the forked repository
    # Then go to https://github.com/<yourgitusername>/VBSHWWBabyLooper/pulls and click "New pull request" button

