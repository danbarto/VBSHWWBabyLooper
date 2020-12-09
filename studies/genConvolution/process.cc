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

    VBSHWW vbs(argc, argv);

    vbs.initSRCutflow();

    vbs.cutflow.printCuts();

    vbs.histograms.addHistogram("Mjj", 60, 0, 6000, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(0) + vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(1)).M(); } );
    vbs.histograms.addHistogram("Detajj", 20, 0, 10, [&]() { return fabs(vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(0).eta() - vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(1).eta()); } );
    vbs.histograms.addHistogram("Mll", 50, 0, 1000, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").at(0) + vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").at(1)).M(); } );
    vbs.histograms.addHistogram("Mbb", 50, 0, 1000, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").at(0) + vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").at(1)).M(); } );
    vbs.histograms.addHistogram("LepPt0", 50, 0, 1000, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").at(0).pt(); } );
    vbs.histograms.addHistogram("LepPt1", 40, 0, 400, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").at(1).pt(); } );
    vbs.histograms.addHistogram("bjetPt0", 50, 0, 1000, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").at(0).pt(); } );
    vbs.histograms.addHistogram("bjetPt1", 40, 0, 400, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").at(1).pt(); } );
    vbs.histograms.addHistogram("vbsjetPt0", 50, 0, 1000, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(0).pt(); } );
    vbs.histograms.addHistogram("vbsjetPt1", 40, 0, 400, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(1).pt(); } );

    vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "TagVBSJets");
    vbs.cutflow.bookCutflows();
    vbs.cutflow.bookEventLists();

    // Looping input file
    while (vbs.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (vbs.job_index != -1 and vbs.nsplit_jobs > 0)
        {
            if (vbs.looper.getNEventsProcessed() % vbs.nsplit_jobs != (unsigned int) vbs.job_index)
                continue;
        }

        vbs.process("SignalRegionPreselection");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    vbs.writeEventList("SignalRegionPreselection");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
