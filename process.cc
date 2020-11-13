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
    vbshww.cutflow.bookEventLists();

    // Looping input file
    while (vbshww.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (vbshww.job_index != -1 and vbshww.nsplit_jobs > 0)
        {
            if (vbshww.looper.getNEventsProcessed() % vbshww.nsplit_jobs != (unsigned int) vbshww.job_index)
                continue;
        }

        vbshww.process();

    }

    // Writing output file
    vbshww.cutflow.saveOutput();

    // Write the data structure to the root file
    vbshww.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    vbshww.writeEventList("SignalRegionPreselection");

    // The below can be sometimes crucial
    delete vbshww.output_tfile;
}
