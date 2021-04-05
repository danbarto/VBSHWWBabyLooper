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

    const float lep_pt0_threshold = 140;
    const float lep_pt1_threshold = 60;

    VBSHWW vbs(argc, argv);

    vbs.initSRCutflow();

    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SREEChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 0 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SREMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 1; }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SRMMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 2; }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SRETChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 3; }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SRMTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 4; }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SRLLChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 0 and vbs.tx.getBranch<int>("channel") <= 2 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("SRLTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 3 and vbs.tx.getBranch<int>("channel") <= 4; }, UNITY);

    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSREEChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 0 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSREMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 1; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSRMMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 2; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSRETChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 3; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSRMTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 4; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSRLLChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 0 and vbs.tx.getBranch<int>("channel") <= 2 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LooseSRLTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 3 and vbs.tx.getBranch<int>("channel") <= 4; }, UNITY);

    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSREEChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 0 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSREMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 1; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSRMMChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 2; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSRETChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 3; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSRMTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") == 4; }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSRLLChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 0 and vbs.tx.getBranch<int>("channel") <= 2 and vbs.tx.getBranch<int>("mee_noZ"); }, UNITY);
    vbs.cutflow.getCut("AK4CategLooseButNotTightObjectPreselection"); vbs.cutflow.addCutToLastActiveCut("LbntSRLTChannel", [&]() { return (vbs.tx.getBranch<int>("pass_blind")) and vbs.tx.getBranch<int>("channel") >= 3 and vbs.tx.getBranch<int>("channel") <= 4; }, UNITY);

    std::vector<TString> categories = {"", "Loose", "Lbnt"};
    std::vector<TString> channels = {"LL"};
    for (auto& category : categories)
    {
        for (auto& channel : channels)
        {
            vbs.cutflow.getCut(TString::Format("%sSR%sChannel", category.Data(), channel.Data()));
            vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR%sMbb", category.Data(), channel.Data()), [&]() { return (vbs.tx.getBranch<LV>("b0")+vbs.tx.getBranch<LV>("b1")).mass() < 150.; }, UNITY );
            vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR%sLepPt0", category.Data(), channel.Data()), [&, lep_pt0_threshold]() { return vbs.tx.getBranch<LV>("leadlep").pt() > lep_pt0_threshold; }, UNITY );
            vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR%sLepPt1", category.Data(), channel.Data()), [&, lep_pt1_threshold]() { return vbs.tx.getBranch<LV>("subllep").pt() > lep_pt1_threshold; }, UNITY );
            vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR%sMjj", category.Data(), channel.Data()), [&]() { return (vbs.tx.getBranch<LV>("j0")+vbs.tx.getBranch<LV>("j1")).mass() > 500.; }, UNITY );
            vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR%s", category.Data(), channel.Data()), UNITY, UNITY); // Dummy cut to have a clean name for final SR
        }
    }

    std::vector<TString> categories_cr = {"", "Loose", "Lbnt"};
    std::vector<TString> channels_cr = {"LL", "EE", "EM", "MM"};
    for (auto& category : categories_cr)
    {
        for (auto& channel : channels_cr)
        {
        vbs.cutflow.getCut( TString::Format("%sSR%sChannel" , category.Data(), channel.Data()));
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sChannel" , category.Data(), channel.Data()), UNITY, UNITY);
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sMbb" , category.Data(), channel.Data()), [&]() { return (vbs.tx.getBranch<LV>("b0")+vbs.tx.getBranch<LV>("b1")).mass() > 150.; }, UNITY);
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sLepPt0" , category.Data(), channel.Data()), [&, lep_pt0_threshold]() { return vbs.tx.getBranch<LV>("leadlep").pt() > lep_pt0_threshold; }, UNITY);
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sLepPt1" , category.Data(), channel.Data()), [&, lep_pt1_threshold]() { return vbs.tx.getBranch<LV>("subllep").pt() > lep_pt1_threshold; }, UNITY);
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sMjj" , category.Data(), channel.Data()), [&]() { return (vbs.tx.getBranch<LV>("j0")+vbs.tx.getBranch<LV>("j1")).mass() > 500.; }, UNITY);
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%s" , category.Data(), channel.Data()), UNITY, UNITY );
        vbs.cutflow.getCut( TString::Format("%sCR%sMbb" , category.Data(), channel.Data()));
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR%sLowLep" , category.Data(), channel.Data()), [&, lep_pt0_threshold, lep_pt1_threshold]() { return (vbs.tx.getBranch<LV>("leadlep").pt() < lep_pt0_threshold or vbs.tx.getBranch<LV>("subllep").pt() < lep_pt1_threshold) and ((vbs.tx.getBranch<LV>("j0")+vbs.tx.getBranch<LV>("j1")).mass() > 500.); }, UNITY);
        }
    }

    vbs.cutflow.printCuts();

    vbs.histograms.addHistogram("NCenJet30", 9, 0, 9, [&]() { return vbs.tx.getBranch<int>("ncenjet30"); } );
    vbs.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return vbs.tx.getBranch<int>("njet30"); } );
    vbs.histograms.addHistogram("NBLoose", 5, 0, 5, [&]() { return vbs.tx.getBranch<int>("nbloose"); } );
    vbs.histograms.addHistogram("NBMedium", 5, 0, 5, [&]() { return vbs.tx.getBranch<int>("nbmedium"); } );
    vbs.histograms.addHistogram("NBTight", 5, 0, 5, [&]() { return vbs.tx.getBranch<int>("nbtight"); } );
    vbs.histograms.addHistogram("LeptonPt0", 180, 0, 600, [&]() { return vbs.tx.getBranch<LV>("leadlep").pt(); } );
    vbs.histograms.addHistogram("LeptonPt1", 180, 0, 600, [&]() { return vbs.tx.getBranch<LV>("subllep").pt(); } );
    vbs.histograms.addHistogram("LeptonPt0Zoom", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("leadlep").pt(); } );
    vbs.histograms.addHistogram("LeptonPt1Zoom", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("subllep").pt(); } );
    vbs.histograms.addHistogram("LeptonEta0", 180, -3, 3, [&]() { return vbs.tx.getBranch<LV>("leadlep").eta(); } );
    vbs.histograms.addHistogram("LeptonEta1", 180, -3, 3, [&]() { return vbs.tx.getBranch<LV>("subllep").eta(); } );
    vbs.histograms.addHistogram("LeptonPhi0", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("leadlep").phi(); } );
    vbs.histograms.addHistogram("LeptonPhi1", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("subllep").phi(); } );
    vbs.histograms.addHistogram("JetPt0", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("j0").pt(); } );
    vbs.histograms.addHistogram("JetPt1", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("j1").pt(); } );
    vbs.histograms.addHistogram("JetEta0", 180, -5, 5, [&]() { return vbs.tx.getBranch<LV>("j0").eta(); } );
    vbs.histograms.addHistogram("JetEta1", 180, -5, 5, [&]() { return vbs.tx.getBranch<LV>("j1").eta(); } );
    vbs.histograms.addHistogram("JetPhi0", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("j0").phi(); } );
    vbs.histograms.addHistogram("JetPhi1", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("j1").phi(); } );
    vbs.histograms.addHistogram("BJetPt0", 180, 0, 450, [&]() { return vbs.tx.getBranch<LV>("b0").pt(); } );
    vbs.histograms.addHistogram("BJetPt1", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("b1").pt(); } );
    vbs.histograms.addHistogram("BJetEta0", 180, -5, 5, [&]() { return vbs.tx.getBranch<LV>("b0").eta(); } );
    vbs.histograms.addHistogram("BJetEta1", 180, -5, 5, [&]() { return vbs.tx.getBranch<LV>("b1").eta(); } );
    vbs.histograms.addHistogram("BJetPhi0", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("b0").phi(); } );
    vbs.histograms.addHistogram("BJetPhi1", 180, -3.1416, 3.1416, [&]() { return vbs.tx.getBranch<LV>("b1").phi(); } );
    vbs.histograms.addHistogram("MJJ", 180, 0, 4500, [&]() { return (vbs.tx.getBranch<LV>("j0")+vbs.tx.getBranch<LV>("j1")).mass(); } );
    vbs.histograms.addHistogram("MJJLow", 180, 0, 350, [&]() { return (vbs.tx.getBranch<LV>("j0")+vbs.tx.getBranch<LV>("j1")).mass(); } );
    vbs.histograms.addHistogram("DEtaJJ", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(vbs.tx.getBranch<LV>("j0"), vbs.tx.getBranch<LV>("j1"))); } );
    vbs.histograms.addHistogram("DPhiJJ", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(vbs.tx.getBranch<LV>("j0"), vbs.tx.getBranch<LV>("j1"))); } );
    vbs.histograms.addHistogram("HiggsPt", 180, 0, 1000, [&]() { return (vbs.tx.getBranch<LV>("b0")+vbs.tx.getBranch<LV>("b1")).pt(); } );
    vbs.histograms.addHistogram("HiggsPtLow", 180, 0, 350, [&]() { return (vbs.tx.getBranch<LV>("b0")+vbs.tx.getBranch<LV>("b1")).pt(); } );
    vbs.histograms.addHistogram("DPhill", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(vbs.tx.getBranch<LV>("leadlep"), vbs.tx.getBranch<LV>("subllep"))); } );
    vbs.histograms.addHistogram("DEtall", 180, 0, 7, [&]() { return fabs(RooUtil::Calc::DeltaEta(vbs.tx.getBranch<LV>("leadlep"), vbs.tx.getBranch<LV>("subllep"))); } );
    vbs.histograms.addHistogram("DRll", 180, 0, 7, [&]() { return fabs(RooUtil::Calc::DeltaR(vbs.tx.getBranch<LV>("leadlep"), vbs.tx.getBranch<LV>("subllep"))); } );
    vbs.histograms.addHistogram("Mll", 180, 0, 2000, [&]() { return (vbs.tx.getBranch<LV>("leadlep")+vbs.tx.getBranch<LV>("subllep")).mass(); } );
    vbs.histograms.addHistogram("MllLow", 180, 0, 250, [&]() { return (vbs.tx.getBranch<LV>("leadlep")+vbs.tx.getBranch<LV>("subllep")).mass(); } );
    vbs.histograms.addHistogram("Mbb", 180, 0, 350, [&]() { return (vbs.tx.getBranch<LV>("b0")+vbs.tx.getBranch<LV>("b1")).mass(); } );
    vbs.histograms.addHistogram("DRbb", 180, 0, 10, [&]() { return RooUtil::Calc::DeltaR(vbs.tx.getBranch<LV>("b0"), vbs.tx.getBranch<LV>("b1")); } );
    vbs.histograms.addHistogram("MET", 180, 0, 1500, [&]() { return vbs.tx.getBranch<LV>("met_p4").pt(); } );
    vbs.histograms.addHistogram("METLow", 180, 0, 250, [&]() { return vbs.tx.getBranch<LV>("met_p4").pt(); } );

    RooUtil::Histograms genhistograms;
    genhistograms.addHistogram("genMJJ", 180, 0, 4500, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmjj"); return -999.f; } );
    genhistograms.addHistogram("genDEtaJJ", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendetajj"); return -999.f; } );
    genhistograms.addHistogram("genDPhiJJ", 180, 0, 3.1416, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendphijj"); return -999.f; } );
    genhistograms.addHistogram("genHiggsPt", 180, 0, 1000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genptbb"); return -999.f; } );
    genhistograms.addHistogram("genHiggsPtMax", 180, 0, 1500, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genptbb"); return -999.f; } );
    genhistograms.addHistogram("genHiggsPtVar", {0., 100., 200., 300., 400., 600., 800., 1000., 1200., 1500.}, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genptbb"); return -999.f; } );
    genhistograms.addHistogram("genLeptonPt0", 180, 0, 600, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genleppt0"); return -999.f; } );
    genhistograms.addHistogram("genLeptonPt1", 180, 0, 600, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genleppt1"); return -999.f; } );
    genhistograms.addHistogram("genLeptonEta0", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_lep0_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genLeptonEta1", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_lep1_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genMll", 180, 0, 2000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmll"); return -999.f; } );
    genhistograms.addHistogram("genMbb", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmbb"); return -999.f; } );
    genhistograms.addHistogram("genDRbb", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendrbb"); return -999.f; } );
    genhistograms.addHistogram("genDRbbZoom", 180, 0, 4, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("gendrbb"); return -999.f; } );
    genhistograms.addHistogram("genMWWH", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmllbbmet"); return -999.f; } );
    genhistograms.addHistogram("genMTWWH", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genmtllbbmet"); return -999.f; } );
    genhistograms.addHistogram("genST", 180, 0, 5000, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<float>("genst"); return -999.f; } );
    genhistograms.addHistogram("genJetPt0", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet0_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genJetPt1", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet1_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genJetEta0", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet0_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genJetEta1", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_jet1_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genBPt0", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_b0_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genBPt1", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_b1_p4").pt(); return -999.f; } );
    genhistograms.addHistogram("genBEta0", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_b0_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genBEta1", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<LV>("gen_b1_p4").eta(); return -999.f; } );
    genhistograms.addHistogram("genchannel", 6, 0, 6, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<int>("genchannel"); return -999; } );
    genhistograms.addHistogram("genMbbtag", 180, 0, 250, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jetIdx") >= 0 and vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jetIdx") >= 0) return (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jetIdx")]+vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jetIdx")]).mass() ; return -999.f; });
    genhistograms.addHistogram("genDRbbtag", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jetIdx") >= 0 and vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jetIdx") >= 0) return RooUtil::Calc::DeltaR(vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jetIdx")],vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jetIdx")]) ; return -999.f; });
    genhistograms.addHistogram("genMjjtag", 180, 0, 4500,  [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return (vbs.tx.getBranchLazy<LV>("gen_j0_bestmatch_jet")+vbs.tx.getBranchLazy<LV>("gen_j1_bestmatch_jet")).mass(); return -999.f; });
    genhistograms.addHistogram("genDEtajjtag", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return fabs(RooUtil::Calc::DeltaEta(vbs.tx.getBranchLazy<LV>("gen_j0_bestmatch_jet"), vbs.tx.getBranchLazy<LV>("gen_j1_bestmatch_jet"))); return -999.f; });
    genhistograms.addHistogram("genJet0tagDR", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr"); return -999.f; });
    genhistograms.addHistogram("genJet1tagDR", 180, 0, 10, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr"); return -999.f; });
    genhistograms.addHistogram("genJet0tagDPtFrac", 180, -1, 1, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dptfrac"); return -999.f; });
    genhistograms.addHistogram("genJet1tagDPtFrac", 180, -1, 1, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh") and vbs.tx.getBranchLazy<float>("gen_j0_bestmatch_jet_dr") < 10 and vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dr") < 10) return vbs.tx.getBranchLazy<float>("gen_j1_bestmatch_jet_dptfrac"); return -999.f; });
    genhistograms.add2DHistogram("genB0tag", 4, 0, 4, "genB1tag", 4, 0, 4, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jet_is_btag_tight") ? 3 : (vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jet_is_btag_medium") ? 2 : (vbs.tx.getBranchLazy<int>("gen_b0_bestmatch_jet_is_btag_loose") ? 1 : 0)); return -999; }, [&]() { if (vbs.tx.getBranchLazy<int>("isvbswwh")) return vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jet_is_btag_tight") ? 3 : (vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jet_is_btag_medium") ? 2 : (vbs.tx.getBranchLazy<int>("gen_b1_bestmatch_jet_is_btag_loose") ? 1 : 0)); return -999; } );

    vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "AK4CategObjectPreselection");
    vbs.cutflow.bookCutflows();
    // vbs.cutflow.bookEventLists();

    // Looping input file
    while (vbs.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (vbs.job_index != -1 and vbs.nsplit_jobs > 0)
        {
            if (vbs.looper.getNEventsProcessed() % vbs.nsplit_jobs != (unsigned int) vbs.job_index)
                continue;
        }

        vbs.process("AK4CategObjectPreselection");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    // vbs.writeEventList("AK4CategObjectPreselection");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
