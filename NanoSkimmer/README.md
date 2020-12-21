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
