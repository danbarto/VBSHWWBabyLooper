#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

#include "VBSHWW.h"

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    VBSHWW vbshww(argc, argv);

    vbshww.cutflow.printCuts();

    vbshww.histograms.addHistogram("NCenJet30", 9, 0, 9, [&]() { return vbshww.tx.getBranchLazy<int>("ncenjet30"); } );
    vbshww.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return vbshww.tx.getBranchLazy<int>("njet30"); } );

    vbshww.cutflow.bookHistogramsForCutAndBelow(vbshww.histograms, "TagVBSJets");
    vbshww.cutflow.bookCutflows();

    // Looping input file
    // while (ana.looper.nextEvent())
    while (vbshww.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (vbshww.job_index != -1 and vbshww.nsplit_jobs > 0)
        {
            if (vbshww.looper.getNEventsProcessed() % vbshww.nsplit_jobs != (unsigned int) vbshww.job_index)
                continue;
        }

        vbshww.tx.clear();

        //Do what you need to do in for each event here
        //To save use the following function
        vbshww.cutflow.fill();

        // tx.fill();
    }

    // Writing output file
    vbshww.cutflow.saveOutput();

    vbshww.tx.write();

    // The below can be sometimes crucial
    delete vbshww.output_tfile;
}
