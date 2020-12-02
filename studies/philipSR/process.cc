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

    vbs.tx.createBranch<float>("maxmjj");
    vbs.tx.createBranch<LV>("vbf_jets_maxmjj_0");
    vbs.tx.createBranch<LV>("vbf_jets_maxmjj_1");
    vbs.tx.createBranch<int>("vbf_jets_maxmjj_0_idx");
    vbs.tx.createBranch<int>("vbf_jets_maxmjj_1_idx");
    vbs.tx.createBranch<float>("mbb_maxmjj");
    vbs.tx.createBranch<float>("drbb_maxmjj");
    vbs.tx.createBranch<LV>("hbb_jets_maxmjj_0");
    vbs.tx.createBranch<LV>("hbb_jets_maxmjj_1");
    vbs.tx.createBranch<float>("mbb_mindr");
    vbs.tx.createBranch<float>("drbb_mindr");
    vbs.tx.createBranch<LV>("hbb_mindr_jets_maxmjj_0");
    vbs.tx.createBranch<LV>("hbb_mindr_jets_maxmjj_1");
    vbs.tx.createBranch<float>("minmlb");
    vbs.tx.createBranch<float>("summlb");
    vbs.tx.createBranch<float>("mindrlb");
    vbs.tx.createBranch<float>("sumdrlb");
    vbs.tx.createBranch<float>("ht");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4");
    vbs.tx.createBranch<vector<LV>>("central_jets_p4");
    vbs.tx.createBranch<float>("cjv_pt0");
    vbs.tx.createBranch<float>("cjv_pt1");
    vbs.tx.createBranch<float>("cjv_pt2");

    vbs.cutflow.addCutToLastActiveCut("SelectObjects",
            [&]()
            {
                // Initialize
                vbs.tx.clear();

                // Select muons
                for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
                {
                    if (SS::muonID(imu, SS::IDfakable, 2017))
                    {
                        vbs.tx.pushbackToBranch<LV>("good_leptons_p4", nt.Muon_p4()[imu]);
                        vbs.tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Muon_charge()[imu]) * 13);
                        vbs.tx.pushbackToBranch<int>("good_leptons_tight", SS::muonID(imu, SS::IDtight, 2017));
                        vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Muon_pfRelIso03_all()[imu]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", -999);
                        vbs.tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Muon_jetPtRelv2()[imu]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Muon_jetRelIso()[imu]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Muon_miniPFRelIso_all()[imu]);
                    }
                }

                // Select electrons
                for (unsigned int iel = 0; iel < nt.Electron_pt().size(); ++iel)
                {
                    if (SS::electronID(iel, SS::IDfakable, 2017))
                    {
                        vbs.tx.pushbackToBranch<LV>("good_leptons_p4", nt.Electron_p4()[iel]);
                        vbs.tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Electron_charge()[iel]) * 11);
                        vbs.tx.pushbackToBranch<int>("good_leptons_tight", SS::electronID(iel, SS::IDtight, 2017) * (nt.Electron_pfRelIso03_all()[iel] < 0.05));
                        vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Electron_pfRelIso03_all()[iel]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", nt.Electron_pfRelIso03_chg()[iel]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Electron_jetPtRelv2()[iel]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Electron_jetRelIso()[iel]);
                        vbs.tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Electron_miniPFRelIso_all()[iel]);
                    }
                }

                vbs.tx.sortVecBranchesByPt(
                        /* name of the 4vector branch to use to pt sort by*/               "good_leptons_p4",
                        /* names of any associated vector<float> branches to sort along */ {},
                        /* names of any associated vector<int>   branches to sort along */ {"good_leptons_pdgid", "good_leptons_tight"},
                        /* names of any associated vector<bool>  branches to sort along */ {}
                        );

                // Select jets
                int nbloose = 0;
                int nbmedium = 0;
                int nbtight = 0;
                float ht = 0;
                for (unsigned int ijet = 0; ijet < nt.Jet_pt().size(); ++ijet)
                {
                    // Read jet p4
                    const LV& jet_p4 = nt.Jet_p4()[ijet];

                    // Overlap check against good leptons
                    bool isOverlap = false;
                    for (auto& lep_p4 : vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4"))
                    {
                        if (RooUtil::Calc::DeltaR(jet_p4, lep_p4) < 0.4)
                        {
                            isOverlap = true;
                            break;
                        }
                    }

                    // Then skip
                    if (isOverlap)
                        continue;

                    if (not (jet_p4.pt() > 30.))
                        continue;

                    // if (not (fabs(jet_p4.eta()) < 5.0))
                    //     continue;

                    vbs.tx.pushbackToBranch<LV>("good_jets_p4", jet_p4);
                    bool is_loose_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.0521;
                    bool is_medium_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.3033;
                    bool is_tight_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.7489;

                    if (is_loose_btagged) nbloose++;
                    if (is_medium_btagged) nbmedium++;
                    if (is_tight_btagged) nbtight++;

                    vbs.tx.pushbackToBranch<int>("good_jets_loose_btagged", is_loose_btagged);
                    vbs.tx.pushbackToBranch<int>("good_jets_medium_btagged", is_medium_btagged);
                    vbs.tx.pushbackToBranch<int>("good_jets_tight_btagged", is_tight_btagged);

                    ht += jet_p4.pt();

                }

                vbs.tx.setBranch<int>("nbloose", nbloose);
                vbs.tx.setBranch<int>("nbmedium", nbmedium);
                vbs.tx.setBranch<int>("nbtight", nbtight);
                vbs.tx.setBranch<float>("ht", ht);

                vbs.tx.sortVecBranchesByPt(
                        /* name of the 4vector branch to use to pt sort by*/               "good_jets_p4",
                        /* names of any associated vector<float> branches to sort along */ {},
                        /* names of any associated vector<int>   branches to sort along */ {"good_jets_loose_btagged", "good_jets_medium_btagged", "good_jets_tight_btagged"},
                        /* names of any associated vector<bool>  branches to sort along */ {}
                        );

                float max_mjj = -999;
                for (unsigned int ijet = 0; ijet < vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++ijet)
                {
                    for (unsigned int jjet = ijet + 1; jjet < vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++jjet)
                    {
                        if (ijet == jjet)
                            continue;

                        const LV& ijet_p4 = vbs.tx.getBranch<vector<LV>>("good_jets_p4")[ijet];
                        const LV& jjet_p4 = vbs.tx.getBranch<vector<LV>>("good_jets_p4")[jjet];

                        float tmp_mjj = (ijet_p4 + jjet_p4).mass();

                        if (max_mjj < tmp_mjj)
                        {
                            max_mjj = tmp_mjj;
                            vbs.tx.setBranch<float>("maxmjj", max_mjj);
                            vbs.tx.setBranch<LV>("vbf_jets_maxmjj_0", ijet_p4);
                            vbs.tx.setBranch<LV>("vbf_jets_maxmjj_1", jjet_p4);
                            vbs.tx.setBranch<int>("vbf_jets_maxmjj_0_idx", ijet);
                            vbs.tx.setBranch<int>("vbf_jets_maxmjj_1_idx", jjet);
                        }
                    }
                }

                if (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() < 2) { return false; }

                vbs.tx.fill();

                if (not (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt() > 25. and vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt() > 20.))
                    return false;

                return true;
            },
            UNITY);

    vbs.cutflow.addCutToLastActiveCut("GeqFourJets",
            [&]()
            {
                return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() >= 4;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("SelectHiggs",
            [&]()
            {
                float min_dr = 999;
                for (unsigned int ijet = 0; ijet < vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++ijet)
                {
                    if (ijet == (unsigned int) vbs.tx.getBranch<int>("vbf_jets_maxmjj_0_idx")) continue;
                    if (ijet == (unsigned int) vbs.tx.getBranch<int>("vbf_jets_maxmjj_1_idx")) continue;
                    if (not vbs.tx.isBranchSet<LV>("hbb_jets_maxmjj_0"))
                        vbs.tx.setBranch<LV>("hbb_jets_maxmjj_0", vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    else if (not vbs.tx.isBranchSet<LV>("hbb_jets_maxmjj_1"))
                        vbs.tx.setBranch<LV>("hbb_jets_maxmjj_1", vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    else
                        vbs.tx.pushbackToBranch<LV>("extra_jets_p4", vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    for (unsigned int jjet = ijet + 1; jjet < vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++jjet)
                    {
                        if (jjet == (unsigned int) vbs.tx.getBranch<int>("vbf_jets_maxmjj_0_idx")) continue;
                        if (jjet == (unsigned int) vbs.tx.getBranch<int>("vbf_jets_maxmjj_1_idx")) continue;
                        float tmp_dr = RooUtil::Calc::DeltaR(vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet], vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[jjet]);
                        if (tmp_dr < min_dr)
                        {
                            min_dr = tmp_dr;
                            vbs.tx.setBranch<LV>("hbb_mindr_jets_maxmjj_0", vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                            vbs.tx.setBranch<LV>("hbb_mindr_jets_maxmjj_1", vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[jjet]);
                        }
                    }
                }
                vbs.tx.setBranch<float>("mbb_maxmjj", (vbs.tx.getBranchLazy<LV>("hbb_jets_maxmjj_0") + vbs.tx.getBranchLazy<LV>("hbb_jets_maxmjj_1")).mass());
                vbs.tx.setBranch<float>("drbb_maxmjj", RooUtil::Calc::DeltaR(vbs.tx.getBranchLazy<LV>("hbb_jets_maxmjj_0"), vbs.tx.getBranchLazy<LV>("hbb_jets_maxmjj_1")));
                vbs.tx.setBranch<float>("mbb_mindr", (vbs.tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_0") + vbs.tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_1")).mass());
                vbs.tx.setBranch<float>("drbb_mindr", RooUtil::Calc::DeltaR(vbs.tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_0"), vbs.tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_1")));

                float mlb00 = (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0] + vbs.tx.getBranch<LV>("hbb_jets_maxmjj_0")).mass();
                float mlb10 = (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1] + vbs.tx.getBranch<LV>("hbb_jets_maxmjj_0")).mass();
                float mlb01 = (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0] + vbs.tx.getBranch<LV>("hbb_jets_maxmjj_1")).mass();
                float mlb11 = (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1] + vbs.tx.getBranch<LV>("hbb_jets_maxmjj_1")).mass();
                float summlb = mlb00 + mlb10 + mlb01 + mlb11;
                float minmlb = min(mlb00, min(mlb10, min(mlb01, mlb11)));
                vbs.tx.setBranch<float>("minmlb", minmlb);
                vbs.tx.setBranch<float>("summlb", summlb);
                float drlb00 = RooUtil::Calc::DeltaR(vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0], vbs.tx.getBranch<LV>("hbb_jets_maxmjj_0"));
                float drlb10 = RooUtil::Calc::DeltaR(vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1], vbs.tx.getBranch<LV>("hbb_jets_maxmjj_0"));
                float drlb01 = RooUtil::Calc::DeltaR(vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0], vbs.tx.getBranch<LV>("hbb_jets_maxmjj_1"));
                float drlb11 = RooUtil::Calc::DeltaR(vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1], vbs.tx.getBranch<LV>("hbb_jets_maxmjj_1"));
                float sumdrlb = drlb00 + drlb10 + drlb01 + drlb11;
                float mindrlb = min(drlb00, min(drlb10, min(drlb01, drlb11)));
                vbs.tx.setBranch<float>("mindrlb", mindrlb);
                vbs.tx.setBranch<float>("sumdrlb", sumdrlb);

                // CJV
                float vbfeta0 = vbs.tx.getBranch<LV>("vbf_jets_maxmjj_0").eta();
                float vbfeta1 = vbs.tx.getBranch<LV>("vbf_jets_maxmjj_1").eta();
                for (auto& extra_jet_p4 : vbs.tx.getBranchLazy<vector<LV>>("extra_jets_p4"))
                {
                    float extraeta = extra_jet_p4.eta();
                    if ((vbfeta0 > extraeta and extraeta > vbfeta1) or (vbfeta1 > extraeta and extraeta > vbfeta0))
                    {
                        vbs.tx.pushbackToBranch<LV>("central_jets_p4", extra_jet_p4);
                    }
                }
                if (vbs.tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 0)
                    vbs.tx.setBranch<float>("cjv_pt0", vbs.tx.getBranch<vector<LV>>("central_jets_p4")[0].pt());
                if (vbs.tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 1)
                    vbs.tx.setBranch<float>("cjv_pt1", vbs.tx.getBranch<vector<LV>>("central_jets_p4")[1].pt());
                if (vbs.tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 2)
                    vbs.tx.setBranch<float>("cjv_pt2", vbs.tx.getBranch<vector<LV>>("central_jets_p4")[2].pt());
                return true;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("VBFJetsEtaCut",
            [&]()
            {
                return fabs(vbs.tx.getBranchLazy<LV>("vbf_jets_maxmjj_0").eta()) > 2.;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("TwoTightLeptons",
            [&]()
            {
                int ntight = 0;
                for (auto& is_tight : vbs.tx.getBranch<vector<int>>("good_leptons_tight"))
                    if (is_tight)
                        ntight++;
                return ntight == 2;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("LeptonPt",
            [&]()
            {
                if (not (vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt() > 50. and vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt() > 25.))
                    return false;
                return true;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("NbMed",
            [&]()
            {
                return vbs.tx.getBranch<int>("nbmedium") > 1.;
            }, UNITY);

    vbs.cutflow.addCutToLastActiveCut("PhilipSRPreselection", UNITY, UNITY);

    vbs.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); } );

    vbs.cutflow.bookHistogramsForCut(vbs.histograms, "PhilipSRPreselection");
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

        vbs.process("PhilipSRPreselection");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.vbs.txt"
    // vbs.writeEventList("SignalRegionPreselection");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
