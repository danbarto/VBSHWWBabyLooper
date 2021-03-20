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

    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SREEChannel", [&]() { return vbs.tx.getBranch<int>("channel") == 0; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SREMChannel", [&]() { return vbs.tx.getBranch<int>("channel") == 1; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRMMChannel", [&]() { return vbs.tx.getBranch<int>("channel") == 2; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRETChannel", [&]() { return vbs.tx.getBranch<int>("channel") == 3; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRMTChannel", [&]() { return vbs.tx.getBranch<int>("channel") == 4; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRLLChannel", [&]() { return vbs.tx.getBranch<int>("channel") >= 0 and vbs.tx.getBranch<int>("channel") <= 2; }, UNITY);
    vbs.cutflow.getCut("KinematicVariables"); vbs.cutflow.addCutToLastActiveCut("SRLTChannel", [&]() { return vbs.tx.getBranch<int>("channel") >= 3 and vbs.tx.getBranch<int>("channel") <= 4; }, UNITY);

    std::vector<TString> channels = {"EE", "EM", "MM", "ET", "MT", "LL", "LT"};
    for (auto& channel : channels)
    {
        vbs.cutflow.getCut(TString::Format("SR%sChannel", channel.Data()));
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sNB", channel.Data()), [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 2.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sLepPt0", channel.Data()), [&]() { return vbs.tx.getBranchLazy<float>("leppt0") > 160.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sMbb", channel.Data()), [&]() { return vbs.tx.getBranchLazy<float>("mbb") < 160.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sMll", channel.Data()), [&]() { return vbs.tx.getBranchLazy<float>("mll") > 200.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sHPt", channel.Data()), [&]() { return vbs.tx.getBranchLazy<float>("ptbb") > 200.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%sDEtajj", channel.Data()), [&]() { return vbs.tx.getBranchLazy<float>("detajj") > 4.8; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("SR%s", channel.Data()), UNITY, UNITY); // Dummy cut to have a clean name for final SR
    }

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

    RooUtil::Histograms genhistograms;
    genhistograms.addHistogram("genMJJ", 180, 0, 4500, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmjj"); return -999.f; } );
    genhistograms.addHistogram("genDEtaJJ", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendetajj"); return -999.f; } );
    genhistograms.addHistogram("genDPhiJJ", 180, 0, 3.1416, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendphijj"); return -999.f; } );
    genhistograms.addHistogram("genHiggsPt", 180, 0, 1000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genptbb"); return -999.f; } );
    genhistograms.addHistogram("genLeptonPt0", 180, 0, 600, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genleppt0"); return -999.f; } );
    genhistograms.addHistogram("genLeptonPt1", 180, 0, 600, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genleppt1"); return -999.f; } );
    genhistograms.addHistogram("genMll", 180, 0, 2000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmll"); return -999.f; } );
    genhistograms.addHistogram("genMbb", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmbb"); return -999.f; } );
    genhistograms.addHistogram("genDRbb", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendrbb"); return -999.f; } );
    genhistograms.addHistogram("genMWWH", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmllbbmet"); return -999.f; } );
    genhistograms.addHistogram("genMTWWH", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmtllbbmet"); return -999.f; } );
    genhistograms.addHistogram("genST", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genst"); return -999.f; } );
    genhistograms.addHistogram("genJetPt0", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet0_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genJetPt1", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet1_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genJetEta0", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet0_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genJetEta1", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet1_p4").eta(); return -999.f; } );

    vbs.cutflow.bookHistogramsForCut(genhistograms, "SelectGenPart");

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
