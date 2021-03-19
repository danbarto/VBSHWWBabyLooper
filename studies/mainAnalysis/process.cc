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

    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SREE", [&]() { return vbs.tx.getBranch<int>("channel") == 0; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SREM", [&]() { return vbs.tx.getBranch<int>("channel") == 1; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRMM", [&]() { return vbs.tx.getBranch<int>("channel") == 2; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRET", [&]() { return vbs.tx.getBranch<int>("channel") == 3; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRMT", [&]() { return vbs.tx.getBranch<int>("channel") == 4; }, UNITY);


    // vbs.cutflow.addCutToLastActiveCut("SRLepPt0", [&]()
    //                                   {
    //                                       if (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() < 2)
    //                                           return false;
    //                                       return vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0).pt() > 160.;
    //                                   }, UNITY);
    // vbs.cutflow.addCutToLastActiveCut("SRMbb150", [&]()
    //                                   {
    //                                       if (vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").size() < 2)
    //                                           return false;
    //                                       LV higgs_0 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4").at(0);
    //                                       LV higgs_1 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4").at(1);
    //                                       return (higgs_0+higgs_1).mass() < 150.;
    //                                   }, UNITY);
    // vbs.cutflow.addCutToLastActiveCut("SRMll200", [&]()
    //                                   {
    //                                       if (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() < 2)
    //                                           return false;
    //                                       LV lepton_0 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0);
    //                                       LV lepton_1 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(1);
    //                                       return (lepton_0+lepton_1).mass() > 200.;
    //                                   }, UNITY);
    // vbs.cutflow.addCutToLastActiveCut("SRHiggsPt200", [&]()
    //                                   {
    //                                       if (vbs.tx.getBranchLazy<vector<LV>>("higgs_jets_p4").size() < 2)
    //                                           return false;
    //                                       LV higgs_0 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4").at(0);
    //                                       LV higgs_1 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4").at(1);
    //                                       return (higgs_0+higgs_1).pt() > 200.;
    //                                   }, UNITY);
    // vbs.cutflow.addCutToLastActiveCut("SRDEtajj4p8", [&]()
    //                                   {
    //                                       if (vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").size() < 2)
    //                                           return false;
    //                                       LV vbs_jet_0 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(0);
    //                                       LV vbs_jet_1 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(1);
    //                                       float deta_jj = fabs(vbs_jet_0.eta() - vbs_jet_1.eta());
    //                                       return deta_jj > 4.8;
    //                                   }, UNITY);

    vbs.cutflow.printCuts();

    vbs.histograms.addHistogram("NCenJet30", 9, 0, 9, [&]() { return vbs.tx.getBranchLazy<int>("ncenjet30"); } );
    vbs.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return vbs.tx.getBranchLazy<int>("njet30"); } );

    vbs.histograms.addHistogram("MJJ", 180, 0, 4500, [&]() { return vbs.tx.getBranch<float>("mjj"); } );
    vbs.histograms.addHistogram("DEtaJJ", 180, 0, 10, [&]() { return vbs.tx.getBranch<float>("detajj"); } );
    vbs.histograms.addHistogram("DPhiJJ", 180, 0, 3.1416, [&]() { return vbs.tx.getBranch<float>("dphijj"); } );
    vbs.histograms.addHistogram("HiggsPt", 180, 0, 1000, [&]() { return vbs.tx.getBranch<float>("ptbb"); } );
    vbs.histograms.addHistogram("LeptonPt0", 180, 0, 600, [&]() { return vbs.tx.getBranch<float>("leppt0"); } );
    vbs.histograms.addHistogram("LeptonPt1", 180, 0, 600, [&]() { return vbs.tx.getBranch<float>("leppt1"); } );
    vbs.histograms.addHistogram("Mll", 180, 0, 2000, [&]() { return vbs.tx.getBranch<float>("mll"); } );
    vbs.histograms.addHistogram("Mbb", 180, 0, 250, [&]() { return vbs.tx.getBranch<float>("mbb"); } );
    vbs.histograms.addHistogram("DRbb", 180, 0, 10, [&]() { return vbs.tx.getBranch<float>("drbb"); } );
    vbs.histograms.addHistogram("MWWH", 180, 0, 5000, [&]() { return vbs.tx.getBranch<float>("mllbbmet"); } );
    vbs.histograms.addHistogram("MTWWH", 180, 0, 5000, [&]() { return vbs.tx.getBranch<float>("mtllbbmet"); } );
    vbs.histograms.addHistogram("ST", 180, 0, 5000, [&]() { return vbs.tx.getBranch<float>("st"); } );
    vbs.histograms.addHistogram("MET", 180, 0, 1500, [&]() { return vbs.tx.getBranchLazy<LorentzVector>("met_p4").pt(); } );
    vbs.histograms.addHistogram("HT", 180, 0, 2000,
                                [&]()
                                {
                                    float ht = 0;
                                    for (auto& lv : vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4"))
                                    {
                                        ht += lv.pt();
                                    }
                                    return ht;
                                } );
    vbs.histograms.addHistogram("JetPt0", 180, 0, 250, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(0).pt(); } );
    vbs.histograms.addHistogram("JetPt1", 180, 0, 250, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(1).pt(); } );
    vbs.histograms.addHistogram("JetEta0", 180, -5, 5, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(0).eta(); } );
    vbs.histograms.addHistogram("JetEta1", 180, -5, 5, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("vbs_jets_p4").at(1).eta(); } );

    vbs.histograms.addHistogram("NBLoose", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbloose"); } );
    vbs.histograms.addHistogram("NBMedium", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbmedium"); } );
    vbs.histograms.addHistogram("NBTight", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbtight"); } );

    vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "KinematicVariables");
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

        vbs.process("KinematicVariables");

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
