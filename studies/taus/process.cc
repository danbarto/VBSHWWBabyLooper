#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "TauSelections.h"

#include "VBSHWW.h"

bool isPromptLepton(int lep_reco_flav, int lep_gen_part_flav_code)
{
    if (abs(lep_reco_flav) == 11 || abs(lep_reco_flav) == 13)
    {
        return (lep_gen_part_flav_code == 1);
    }
    else if (abs(lep_reco_flav) == 15)
    {
        return (lep_gen_part_flav_code >= 3);
    }
    else
    {
        return false;
    }
}

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    VBSHWW vbs(argc, argv);

    // // Real lepton branches
    // vbs.tx.createBranch<int>("real_lep_id");
    // vbs.tx.createBranch<float>("real_lep_pt");
    // vbs.tx.createBranch<float>("real_lep_eta");
    // vbs.tx.createBranch<float>("real_lep_phi");
    // vbs.tx.createBranch<int>("real_lep_origin");
    // vbs.tx.createBranch<bool>("real_lep_is_loose");
    // vbs.tx.createBranch<bool>("real_lep_is_tight");
    // // Fake lepton branches
    // vbs.tx.createBranch<int>("fake_lep_id");
    // vbs.tx.createBranch<float>("fake_lep_pt");
    // vbs.tx.createBranch<float>("fake_lep_eta");
    // vbs.tx.createBranch<float>("fake_lep_phi");
    // vbs.tx.createBranch<int>("fake_lep_origin");
    // vbs.tx.createBranch<bool>("fake_lep_is_loose");
    // vbs.tx.createBranch<bool>("fake_lep_is_tight");
    // // b-jets
    // vbs.tx.createBranch<int>("n_gen_bquarks");
    // vbs.tx.createBranch<int>("n_loose_bjets");
    // vbs.tx.createBranch<int>("n_medium_bjets");
    // vbs.tx.createBranch<int>("n_tight_bjets");
    // Other
    vbs.tx.createBranch<float>("event_weight");
    vbs.tx.createBranch<bool>("is_mu_tau");
    vbs.tx.createBranch<vector<int>>("good_leptons_genPartFlav");
    vbs.tx.createBranch<vector<LV>>("good_taus_p4");
    vbs.tx.createBranch<vector<int>>("good_taus_pdgid");
    vbs.tx.createBranch<vector<int>>("good_taus_tight");
    vbs.tx.createBranch<vector<int>>("good_taus_jetIdx");
    vbs.tx.createBranch<vector<int>>("good_taus_genPartFlav");

    //*****************************
    // - NanoAOD level preselection
    //*****************************
    // Description: Select at least two leptons in NanoAOD with 20 GeV and above
    vbs.cutflow.getCut("Weight");
    vbs.cutflow.addCutToLastActiveCut("ADifferentPreselection",
        [&]()
        {

            int nel20 = 0;
            int nmu20 = 0;
            std::vector<int> el20_idx;
            std::vector<int> mu20_idx;

            // loop over electrons to count electrons above 20 GeV, and store indices
            for (unsigned int idx = 0; idx < nt.Electron_pt().size(); ++idx)
            {
                float el_pt = nt.Electron_pt()[idx];
                if (el_pt > 20)
                {
                    nel20++;
                    el20_idx.push_back(idx);
                }
            }

            // loop over muons to count muons above 20 GeV, and store indices
            for (unsigned int idx = 0; idx < nt.Muon_pt().size(); ++idx)
            {
                float mu_pt = nt.Muon_pt()[idx];
                if (mu_pt > 20)
                {
                    nmu20++;
                    mu20_idx.push_back(idx);
                }
            }

            // return true if more than or equals to 2 leptons
            return nel20 + nmu20 >= 1;
        },
        UNITY);

    vbs.cutflow.addCutToLastActiveCut(
        "SaveWeight",
        [&]()
        {
            float weight = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0))*vbs.scale1fb;
            vbs.tx.setBranch<float>("event_weight", weight*gconf.lumi);
            return true;
        },
        UNITY
    );

    //*****************************
    // - Selecting Analysis Leptons
    //*****************************
    // Description: Select leptons used for the analysis
    //              The electrons / muons definitions are in NanoCORE/Electron(Muon)Selections.cc
    vbs.cutflow.addCutToLastActiveCut("SelectLeptons",
        [&]()
        {

            // Select muons
            for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
            {
                if (SS::muonID(imu, SS::IDfakable, nt.year()))
                {
                    vbs.tx.pushbackToBranch<LV>("good_leptons_p4", nt.Muon_p4()[imu]);
                    vbs.tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Muon_charge()[imu]) * 13);
                    vbs.tx.pushbackToBranch<int>("good_leptons_tight", SS::muonID(imu, SS::IDtight, nt.year()));
                    vbs.tx.pushbackToBranch<int>("good_leptons_jetIdx", nt.Muon_jetIdx()[imu]);
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
                if (SS::electronID(iel, SS::IDfakable, nt.year()))
                {
                    vbs.tx.pushbackToBranch<LV>("good_leptons_p4", nt.Electron_p4()[iel]);
                    vbs.tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Electron_charge()[iel]) * 11);
                    vbs.tx.pushbackToBranch<int>("good_leptons_tight", SS::electronID(iel, SS::IDtight, nt.year()) * (nt.Electron_pfRelIso03_all()[iel] < 0.05));
                    vbs.tx.pushbackToBranch<int>("good_leptons_jetIdx", nt.Electron_jetIdx()[iel]);
                    vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Electron_pfRelIso03_all()[iel]);
                    vbs.tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", nt.Electron_pfRelIso03_chg()[iel]);
                    vbs.tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Electron_jetPtRelv2()[iel]);
                    vbs.tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Electron_jetRelIso()[iel]);
                    vbs.tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Electron_miniPFRelIso_all()[iel]);
                }
            }

            // Select taus
            vector<LV> good_leptons_p4 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4");
            vector<int> good_leptons_pdgid = vbs.tx.getBranchLazy<vector<int>>("good_leptons_pdgid");
            for (unsigned int itau = 0; itau < nt.nTau(); ++itau)
            {
                if (SS::tauID(itau, SS::IDfakable, nt.year()))
                {
                    // tau-(non-tau lep) overlap removal
                    bool save_this_tau = true;
                    for (unsigned int ilep = 0; ilep < good_leptons_p4.size(); ++ilep)
                    {
                        if (RooUtil::Calc::DeltaR(nt.Tau_p4().at(itau), good_leptons_p4.at(ilep)) < 0.4)
                        {
                            save_this_tau = false;
                            break;
                        }
                    }
                    if (!save_this_tau) { continue; }
                    // Only save >= loose taus that do not overlap w/ a 'good' lepton
                    vbs.tx.pushbackToBranch<LV>("good_taus_p4", nt.Tau_p4()[itau]);
                    vbs.tx.pushbackToBranch<int>("good_taus_genPartFlav", nt.Tau_genPartFlav()[itau]);
                    vbs.tx.pushbackToBranch<int>("good_taus_pdgid", (-nt.Tau_charge()[itau]) * 15);
                    vbs.tx.pushbackToBranch<int>("good_taus_tight", SS::tauID(itau, SS::IDtight, nt.year()));
                    vbs.tx.pushbackToBranch<int>("good_taus_jetIdx", nt.Tau_jetIdx()[itau]);
                }
            }

            vbs.tx.sortVecBranchesByPt(
                /* name of the 4vector branch to use to pt sort by*/
                "good_leptons_p4",
                /* names of any associated vector<float> branches to sort along */ 
                {
                    "good_leptons_pfRelIso03_all", 
                    "good_leptons_pfRelIso03_chg", 
                    "good_leptons_jetPtRelv2", 
                    "good_leptons_jetRelIso", 
                    "good_leptons_miniPFRelIso_all"
                },
                /* names of any associated vector<int>   branches to sort along */ 
                {
                    "good_leptons_pdgid", 
                    "good_leptons_tight", 
                    "good_leptons_jetIdx", 
                    "good_leptons_genPartFlav"
                },
                /* names of any associated vector<bool>  branches to sort along */ 
                {}
            );

            vbs.tx.sortVecBranchesByPt(
                /* name of the 4vector branch to use to pt sort by*/
                "good_taus_p4",
                /* names of any associated vector<float> branches to sort along */ 
                {},
                /* names of any associated vector<int>   branches to sort along */ 
                {
                    "good_taus_pdgid", 
                    "good_taus_tight", 
                    "good_taus_jetIdx",
                    "good_taus_genPartFlav"
                },
                /* names of any associated vector<bool>  branches to sort along */ 
                {}
            );

            return true;

        },
        UNITY);

    //*****************************
    // - Same Sign Preselection
    //*****************************
    // Description: Pass events only when we have:
    //              - two loose
    //              - two tight
    //              - they are same sign
    //              - pt > 25 or 20 depending on flavor (see below for detail)
    vbs.cutflow.addCutToLastActiveCut("SSPreselection",
        [&]()
        {

            // Select only one loose lepton, one loose tau
            if (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() != 1)
                return false;
            if (vbs.tx.getBranchLazy<vector<LV>>("good_taus_p4").size() != 1)
                return false;

            int ntight_leps = 0;
            for (auto& istight_lep : vbs.tx.getBranch<vector<int>>("good_leptons_tight"))
            {
                if (istight_lep)
                    ntight_leps++;
            }
            int ntight_taus = 0;
            for (auto& istight_tau : vbs.tx.getBranch<vector<int>>("good_taus_tight"))
            {
                if (istight_tau)
                    ntight_taus++;
            }

            // Select only one tight lepton, one tight tau
            if (ntight_leps != 1)
                return false;
            if (ntight_taus != 1)
                return false;

            // Therefore I will only have one leptons in the good_leptons container
            const int& pdgid0 = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[0];
            const int& pdgid1 = vbs.tx.getBranch<vector<int>>("good_taus_pdgid")[0];

            // Require same sign
            if (pdgid0 * pdgid1 < 0)
                return false;

            const float& pt0 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt();
            const float& pt1 = vbs.tx.getBranch<vector<LV>>("good_taus_p4")[0].pt();

            // Apply Pt selections
            return ((pt0 > 25.) and (pt1 > 25.));
        },
        UNITY);

    //*****************************
    // - Select Good Reco Jets
    //*****************************
    // Description: Select Good Reco Jets
    //              - (TODO?) CURRENTLY NO ID APPLIED TO THE JETS
    //              - TODO TODO TODO TODO : Check pileup jet ID for year 2017
    //              - Perform overlap removal against loose leptons
    //              - Accept jets above 20 GeV for the container
    //              - Count N btagged jets (nbloose, nbmedium, nbtight)
    //              - Count central and all jets with pt > 30 GeV
    //              - Save the jets into good_jets containers
    vbs.cutflow.addCutToLastActiveCut("SelectJets",
        [&]()
        {

            // b tagging counters
            int nbloose = 0;
            int nbmedium = 0;
            int nbtight = 0;

            int ncenjet30 = 0;
            int njet30 = 0;

            // Loop over the jets
            for (unsigned int ijet = 0; ijet < nt.Jet_pt().size(); ++ijet)
            {
                // Read jet p4
                const LV& jet_p4 = nt.Jet_p4()[ijet];

                // Overlap check against good leptons
                bool isOverlap = false;
                vector<LV> good_leptons_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4");
                vector<int> good_leptons_jetIdx = vbs.tx.getBranch<vector<int>>("good_leptons_jetIdx");
                for (unsigned int ilep = 0; ilep < good_leptons_p4.size(); ++ilep)
                {
                    if (good_leptons_jetIdx.at(ilep) == (int) ijet)
                    {
                        isOverlap = true;
                        break;
                    }
                }
                vector<LV> good_taus_p4 = vbs.tx.getBranch<vector<LV>>("good_taus_p4");
                for (unsigned int itau = 0; itau < good_taus_p4.size(); ++itau)
                {
                    if (RooUtil::Calc::DeltaR(good_taus_p4.at(itau), jet_p4) < 0.4)
                    {
                        isOverlap = true;
                        break;
                    }
                }

                // Then skip
                if (isOverlap)
                    continue;

                // B-tagging is done down to 20 GeV
                if (not (jet_p4.pt() > 20.))
                    continue;

                bool is_loose_btagged = false;
                bool is_medium_btagged = false;
                bool is_tight_btagged = false;

                // B-tagging is also done up to 2.5 in eta only
                if (abs(jet_p4.eta()) < 2.4)
                {
                    // Check if it passes btagging
                    is_loose_btagged = nt.Jet_btagDeepFlavB()[ijet] > gconf.WP_DeepFlav_loose;
                    is_medium_btagged = nt.Jet_btagDeepFlavB()[ijet] > gconf.WP_DeepFlav_medium;
                    is_tight_btagged = nt.Jet_btagDeepFlavB()[ijet] > gconf.WP_DeepFlav_tight;

                    // Count up the btagging
                    if (is_loose_btagged) nbloose++;
                    if (is_medium_btagged) nbmedium++;
                    if (is_tight_btagged) nbtight++;
                }

                vbs.tx.pushbackToBranch<LV>("good_jets_p4", jet_p4);
                vbs.tx.pushbackToBranch<int>("good_jets_loose_btagged", is_loose_btagged);
                vbs.tx.pushbackToBranch<int>("good_jets_medium_btagged", is_medium_btagged);
                vbs.tx.pushbackToBranch<int>("good_jets_tight_btagged", is_tight_btagged);
                vbs.tx.pushbackToBranch<float>("good_jets_btag_score", nt.Jet_btagDeepFlavB()[ijet]);
                vbs.tx.pushbackToBranch<float>("good_jets_qg_disc", nt.Jet_qgl()[ijet]);

                if (abs(jet_p4.eta()) < 3.0 and jet_p4.pt() > 30.)
                {
                    ncenjet30 ++;
                }
                if (jet_p4.pt() > 30.)
                {
                    njet30 ++;
                }

            }

            vbs.tx.setBranch<int>("nbloose", nbloose);
            vbs.tx.setBranch<int>("nbmedium", nbmedium);
            vbs.tx.setBranch<int>("nbtight", nbtight);
            vbs.tx.setBranch<int>("ncenjet30", ncenjet30);
            vbs.tx.setBranch<int>("njet30", njet30);

            vbs.tx.sortVecBranchesByPt(
                    /* name of the 4vector branch to use to pt sort by*/               "good_jets_p4",
                    /* names of any associated vector<float> branches to sort along */ {"good_jets_btag_score", "good_jets_qg_disc"},
                    /* names of any associated vector<int>   branches to sort along */ {"good_jets_loose_btagged", "good_jets_medium_btagged", "good_jets_tight_btagged"},
                    /* names of any associated vector<bool>  branches to sort along */ {}
                    );

            return true;
        },
        UNITY);

    //*****************************
    // - Require Two Medium Btag
    //*****************************
    // Description: Select two medium b-tag /* TODO TODO TODO TODO btag scale factor */
    vbs.cutflow.addCutToLastActiveCut("GeqTwoLooseBtag", [&]() { return vbs.tx.getBranch<int>("nbloose") >= 2; }, UNITY);

    // //*****************************
    // // - Require Two Medium Btag
    // //*****************************
    // // Description: Select two medium b-tag /* TODO TODO TODO TODO btag scale factor */
    // vbs.cutflow.addCutToLastActiveCut("GeqTwoMedBtag", [&]() { return vbs.tx.getBranch<int>("nbmedium") >= 2; }, UNITY);

    // //*****************************
    // // - Require Two Tight Btag
    // //*****************************
    // // Description: Select two tight b-tag /* TODO TODO TODO TODO btag scale factor */
    // vbs.cutflow.addCutToLastActiveCut("GeqTwoTightBtag", [&]() { return vbs.tx.getBranch<int>("nbtight") >= 2; }, UNITY);

    //*****************************
    // - Tag Hbb jets
    //*****************************
    // Description: Select the two b-tagged jets with highest btagging score as Hbb jets
    vbs.cutflow.addCutToLastActiveCut("TagHiggsJets",
        [&]()
        {
            // get scores and indices pairs
            std::vector<std::pair<float, int>> btag_jets;
            for (unsigned int i = 0; i < vbs.tx.getBranch<vector<LV>>("good_jets_p4").size(); i++)
            {
                const float& btag_score = vbs.tx.getBranch<vector<float>>("good_jets_btag_score")[i];
                btag_jets.push_back(std::make_pair(btag_score, i));
            }

            // Sort the pairs
            std::sort(btag_jets.begin(), btag_jets.end(),
                    [](const std::pair<float, int> & a, const std::pair<float, int> & b) -> bool
                    { 
                        return a.first > b.first;
                    });

            int higgs_jet_0 = btag_jets[0].second < btag_jets[1].second ? btag_jets[0].second : btag_jets[1].second;
            int higgs_jet_1 = btag_jets[0].second < btag_jets[1].second ? btag_jets[1].second : btag_jets[0].second;

            vbs.tx.pushbackToBranch<LV>("higgs_jets_p4", vbs.tx.getBranch<vector<LV>>("good_jets_p4")[higgs_jet_0]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_loose_btagged", vbs.tx.getBranch<vector<int>>("good_jets_loose_btagged")[higgs_jet_0]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_medium_btagged", vbs.tx.getBranch<vector<int>>("good_jets_medium_btagged")[higgs_jet_0]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_tight_btagged", vbs.tx.getBranch<vector<int>>("good_jets_tight_btagged")[higgs_jet_0]);
            vbs.tx.pushbackToBranch<float>("higgs_jets_btag_score", vbs.tx.getBranch<vector<float>>("good_jets_btag_score")[higgs_jet_0]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_good_jets_idx", higgs_jet_0);

            vbs.tx.pushbackToBranch<LV>("higgs_jets_p4", vbs.tx.getBranch<vector<LV>>("good_jets_p4")[higgs_jet_1]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_loose_btagged", vbs.tx.getBranch<vector<int>>("good_jets_loose_btagged")[higgs_jet_1]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_medium_btagged", vbs.tx.getBranch<vector<int>>("good_jets_medium_btagged")[higgs_jet_1]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_tight_btagged", vbs.tx.getBranch<vector<int>>("good_jets_tight_btagged")[higgs_jet_1]);
            vbs.tx.pushbackToBranch<float>("higgs_jets_btag_score", vbs.tx.getBranch<vector<float>>("good_jets_btag_score")[higgs_jet_1]);
            vbs.tx.pushbackToBranch<int>("higgs_jets_good_jets_idx", higgs_jet_1);
            return true;

        }, UNITY);

    //*****************************
    // - Tag VBS jets
    //*****************************
    // Description: Select two jets that are not part of the Hbb jets to be VBS jets with following algorithm
    //              - If all jets are in same hemisphere (eta > 0 or eta < 0) then choose the two leading jets in P (N.B. not pt!)
    //              - If not choose the leading jet in each hemisphere leading in P (N.B. not pt!)
    vbs.cutflow.addCutToLastActiveCut("TagVBSJets",
        [&]()
        {

            // higgs jet indices
            const int& higgs_jet_0 = vbs.tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[0];
            const int& higgs_jet_1 = vbs.tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[1];
            // good jets p4
            const vector<LV>& good_jets_p4 = vbs.tx.getBranch<vector<LV>>("good_jets_p4");

            // Select VBS candidates
            std::vector<int> vbs_jet_cands_idxs;
            for (unsigned int i = 0; i < good_jets_p4.size(); i++)
            {
                if ((int) i != higgs_jet_0 and (int) i != higgs_jet_1)
                {
                    if (good_jets_p4[i].pt() >= 30.)
                    {
                        vbs_jet_cands_idxs.push_back(i);
                    }
                }
            }

            if (vbs_jet_cands_idxs.size() < 2)
                return false;

            if (vbs_jet_cands_idxs.size() == 2)
            {
                vbs.tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_cands_idxs[0]]);
                vbs.tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_cands_idxs[1]]);
                vbs.tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_cands_idxs[0]);
                vbs.tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_cands_idxs[1]);
                vbs.tx.setBranch<float>(
                    "vbs_Mjj", 
                    (good_jets_p4[vbs_jet_cands_idxs[0]] + good_jets_p4[vbs_jet_cands_idxs[1]]).M()
                );
                return true;
            }

            // Otherwise, I have 3 or more vbs candidate jets
            std::vector<std::pair<float, int>> vbs_pos_eta_jets;
            std::vector<std::pair<float, int>> vbs_neg_eta_jets;
            for (unsigned int ijet = 0; ijet < vbs_jet_cands_idxs.size(); ijet++)
            {
                const LV& jet = good_jets_p4[vbs_jet_cands_idxs[ijet]];
                const float& P = good_jets_p4[vbs_jet_cands_idxs[ijet]].P();
                if (jet.eta() >= 0)
                {
                    vbs_pos_eta_jets.push_back(std::make_pair(P, vbs_jet_cands_idxs[ijet]));
                }
                if (jet.eta() < 0)
                {
                    vbs_neg_eta_jets.push_back(std::make_pair(P, vbs_jet_cands_idxs[ijet]));
                }
            }

            // Sort the pairs
            std::sort(vbs_pos_eta_jets.begin(), vbs_pos_eta_jets.end(),
                    [](const std::pair<float, int> & a, const std::pair<float, int> & b) -> bool
                    { 
                    return a.first > b.first;
                    });

            // Sort the pairs
            std::sort(vbs_neg_eta_jets.begin(), vbs_neg_eta_jets.end(),
                    [](const std::pair<float, int> & a, const std::pair<float, int> & b) -> bool
                    { 
                    return a.first > b.first;
                    });

            int vbs_jet_idx_A = -999;
            int vbs_jet_idx_B = -999;
            if (vbs_pos_eta_jets.size() == 0)
            {
                vbs_jet_idx_A = vbs_neg_eta_jets[0].second;
                vbs_jet_idx_B = vbs_neg_eta_jets[1].second;
            }
            else if (vbs_neg_eta_jets.size() == 0)
            {
                vbs_jet_idx_A = vbs_pos_eta_jets[0].second;
                vbs_jet_idx_B = vbs_pos_eta_jets[1].second;
            }
            else
            {
                vbs_jet_idx_A = vbs_pos_eta_jets[0].second;
                vbs_jet_idx_B = vbs_neg_eta_jets[0].second;
            }

            int vbs_jet_idx_0 = vbs_jet_idx_A < vbs_jet_idx_B ? vbs_jet_idx_A : vbs_jet_idx_B;
            int vbs_jet_idx_1 = vbs_jet_idx_A < vbs_jet_idx_B ? vbs_jet_idx_B : vbs_jet_idx_A;

            vbs.tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_idx_0]);
            vbs.tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_idx_1]);
            vbs.tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_idx_0);
            vbs.tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_idx_1);
            vbs.tx.setBranch<float>("vbs_Mjj", (good_jets_p4[vbs_jet_idx_0] + good_jets_p4[vbs_jet_idx_1]).M());

            return true;

        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("VBSJetPreselection",
        [&]()
        {
            LV vbs_jet_0 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(0);
            LV vbs_jet_1 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(1);
            float M_jj = vbs.tx.getBranch<float>("vbs_Mjj");
            float deta_jj = vbs_jet_0.eta() - vbs_jet_1.eta();
            return (M_jj >= 500 && fabs(deta_jj) >= 3);
        }, 
        UNITY);

    //*****************************
    // - SR Preselection Dummy Cut Node
    //*****************************
    // Description: This is a dummy cut node to indicate that this node represents where all the SR preselection is applied
    vbs.cutflow.addCutToLastActiveCut("SignalRegionPreselection", UNITY, UNITY);

    vbs.cutflow.addCutToLastActiveCut("RequireMuonTau",
        [&]()
        {
            // Only one lepton, one tau in event (see SS preselection)
            int lep_pdgid = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid").at(0);
            vbs.tx.setBranch<bool>("is_mu_tau", abs(lep_pdgid) == 13);
            return (abs(lep_pdgid) == 13);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptTauGt50",
        [&]()
        {
            // Only one tau in event (see SS preselection)
            float tau_pt = vbs.tx.getBranch<vector<LV>>("good_taus_p4").at(0).pt();
            return (tau_pt > 50);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptTauGt100",
        [&]()
        {
            // Only one tau in event (see SS preselection)
            float tau_pt = vbs.tx.getBranch<vector<LV>>("good_taus_p4").at(0).pt();
            return (tau_pt > 100);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptTauGt150",
        [&]()
        {
            // Only one tau in event (see SS preselection)
            float tau_pt = vbs.tx.getBranch<vector<LV>>("good_taus_p4").at(0).pt();
            return (tau_pt > 150);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("MjjGeq1TeV",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float M_jj = vbs.tx.getBranch<float>("vbs_Mjj");
            return (M_jj > 1000);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("MjjGeq1p5TeV",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float M_jj = vbs.tx.getBranch<float>("vbs_Mjj");
            return (M_jj > 1500);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptMuonGt50",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float mu_pt = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0).pt();
            return (mu_pt > 50);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptMuonGt100",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float mu_pt = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0).pt();
            return (mu_pt > 100);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("ptMuonGt150",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float mu_pt = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0).pt();
            return (mu_pt > 150);
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("TauViabilityPreselection", UNITY, UNITY);

    vbs.cutflow.getCut("SignalRegionPreselection");

    vbs.cutflow.addCutToLastActiveCut("LepTauPtCuts",
        [&]()
        {
            // Only one lepton, one tau in event (see SS preselection)
            LV lep_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4").at(0);
            LV tau_p4 = vbs.tx.getBranch<vector<LV>>("good_taus_p4").at(0);
            if (lep_p4.pt() > tau_p4.pt())
            {
                return (lep_p4.pt() > 150 && tau_p4.pt() > 75);
            }
            else
            {
                return (tau_p4.pt() > 150 && lep_p4.pt() > 75);
            }
        }, 
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("AnotherMjjGeq1p5TeV",
        [&]()
        {
            // Only one lepton in event (see SS preselection)
            float M_jj = vbs.tx.getBranch<float>("vbs_Mjj");
            return (M_jj > 1500);
        }, 
        UNITY);
    
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

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
