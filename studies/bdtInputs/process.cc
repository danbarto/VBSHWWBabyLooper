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

    // Additional branches
    vbs.tx.createBranch<float>("event_weight");
    vbs.tx.createBranch<int>("ncenjet25");
    vbs.tx.createBranch<int>("njet25");
    vbs.tx.createBranch<vector<int>>("extra_jets_good_jets_idx");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4");
    vbs.tx.createBranch<vector<float>>("extra_jets_qg_disc");
    // BDT bookkeeping
    vbs.tx.createBranch<int>("BDT_run");
    vbs.tx.createBranch<int>("BDT_event");
    vbs.tx.createBranch<int>("BDT_lumi");
    // BDT MET
    vbs.tx.createBranch<float>("BDT_MET_pt");
    vbs.tx.createBranch<float>("BDT_MET_phi");
    // BDT leptons
    vbs.tx.createBranch<int>("BDT_ld_lep_flav");
    vbs.tx.createBranch<float>("BDT_ld_lep_pt");
    vbs.tx.createBranch<float>("BDT_ld_lep_eta");
    vbs.tx.createBranch<float>("BDT_ld_lep_phi");
    vbs.tx.createBranch<float>("BDT_ld_lep_reliso");
    vbs.tx.createBranch<int>("BDT_tr_lep_flav");
    vbs.tx.createBranch<float>("BDT_tr_lep_pt");
    vbs.tx.createBranch<float>("BDT_tr_lep_eta");
    vbs.tx.createBranch<float>("BDT_tr_lep_phi");
    vbs.tx.createBranch<float>("BDT_tr_lep_reliso");
    // BDT VBS jets
    vbs.tx.createBranch<float>("BDT_ld_vbsjet_P");
    vbs.tx.createBranch<float>("BDT_tr_vbsjet_P");
    vbs.tx.createBranch<float>("BDT_vbsjet_pair_M");
    vbs.tx.createBranch<float>("BDT_vbsjet_pair_deta");
    // BDT b-jets
    vbs.tx.createBranch<float>("BDT_ld_bjet_score");
    vbs.tx.createBranch<float>("BDT_tr_bjet_score");
    vbs.tx.createBranch<float>("BDT_bjet_pair_pt");
    vbs.tx.createBranch<float>("BDT_bjet_pair_dR");
    // BDT extra jets
    vbs.tx.createBranch<int>("BDT_n_extrajets");
    // BDT W hypothesis
    vbs.tx.createBranch<float>("BDT_W_hyp_pt");
    vbs.tx.createBranch<float>("BDT_W_hyp_M");
    vbs.tx.createBranch<float>("BDT_W_hyp_dR");
    vbs.tx.createBranch<float>("BDT_W_hyp_dphi");
    // BDT t->W+b hypothesis
    vbs.tx.createBranch<float>("BDT_t_to_W_b_hyp_pt");
    vbs.tx.createBranch<float>("BDT_t_to_W_b_hyp_M");
    vbs.tx.createBranch<float>("BDT_t_to_W_b_hyp_dR");
    vbs.tx.createBranch<float>("BDT_t_to_W_b_hyp_dphi");
    // BDT t->l(+nu)+b hypothesis
    vbs.tx.createBranch<float>("BDT_t_to_lep_b_hyp_pt");
    vbs.tx.createBranch<float>("BDT_t_to_lep_b_hyp_M");
    vbs.tx.createBranch<float>("BDT_t_to_lep_b_hyp_dR");
    vbs.tx.createBranch<float>("BDT_t_to_lep_b_hyp_dphi");
    // BDT l+b pairs (# extra jets < 2)
    vbs.tx.createBranch<float>("BDT_ld_lep_ld_b_pt");   // ld/ld
    vbs.tx.createBranch<float>("BDT_ld_lep_ld_b_M");    // "
    vbs.tx.createBranch<float>("BDT_ld_lep_ld_b_dR");   // "
    vbs.tx.createBranch<float>("BDT_ld_lep_ld_b_dphi"); // "
    vbs.tx.createBranch<float>("BDT_tr_lep_ld_b_pt");   // tr/ld
    vbs.tx.createBranch<float>("BDT_tr_lep_ld_b_M");    // "
    vbs.tx.createBranch<float>("BDT_tr_lep_ld_b_dR");   // "
    vbs.tx.createBranch<float>("BDT_tr_lep_ld_b_dphi"); // "
    vbs.tx.createBranch<float>("BDT_ld_lep_tr_b_pt");   // ld/tr
    vbs.tx.createBranch<float>("BDT_ld_lep_tr_b_M");    // "
    vbs.tx.createBranch<float>("BDT_ld_lep_tr_b_dR");   // "
    vbs.tx.createBranch<float>("BDT_ld_lep_tr_b_dphi"); // "
    vbs.tx.createBranch<float>("BDT_tr_lep_tr_b_pt");   // tr/tr
    vbs.tx.createBranch<float>("BDT_tr_lep_tr_b_M");    // "
    vbs.tx.createBranch<float>("BDT_tr_lep_tr_b_dR");   // "
    vbs.tx.createBranch<float>("BDT_tr_lep_tr_b_dphi"); // "

    vbs.cutflow.addCutToLastActiveCut("SaveWeight",
        [&]()
        {
            float weight = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0))*vbs.scale1fb;
            vbs.tx.setBranch<float>("event_weight", weight*gconf.lumi);
            return true;
        },
        UNITY
    );

    // Looser BDT preselection
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
                {"good_leptons_pdgid", "good_leptons_tight", "good_leptons_jetIdx"},
                /* names of any associated vector<bool>  branches to sort along */ 
                {}
            );

            return true;

        },
        UNITY
    );

    //*****************************
    // - Same Sign Preselection
    //*****************************
    // Description: Pass events only when we have:
    //              - two loose
    //              - two tight
    //              - they are same sign
    vbs.cutflow.addCutToLastActiveCut("SSPreselection",
        [&]()
        {

            // Select only two loose leptons
            if (not (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() == 2))
                return false;

            int ntight = 0;
            for (auto& istight : vbs.tx.getBranch<vector<int>>("good_leptons_tight"))
            {
                if (istight)
                    ntight++;
            }

            // Select only two tight leptons
            if (not (ntight == 2))
                return false;

            // Therefore I will only have two leptons in the good_leptons container
            const int& pdgid0 = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[0];
            const int& pdgid1 = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[1];

            // Require same sign
            if (not (pdgid0 * pdgid1 > 0))
                return false;

            return true;
        },
        UNITY
    );

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

            int ncenjet25 = 0;
            int njet25 = 0;

            // Loop over the jets
            for (unsigned int ijet = 0; ijet < nt.Jet_pt().size(); ++ijet)
            {
                // Read jet p4
                const LV& jet_p4 = nt.Jet_p4()[ijet];

                // Overlap check against good leptons
                bool isOverlap = false;
                for (unsigned int ilep = 0; ilep < vbs.tx.getBranch<vector<LV>>("good_leptons_p4").size(); ++ilep)
                {
                    if (vbs.tx.getBranch<vector<int>>("good_leptons_jetIdx").at(ilep) == (int) ijet)
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

                if (abs(jet_p4.eta()) < 3.0 and jet_p4.pt() > 25.)
                {
                    ncenjet25 ++;
                }
                if (jet_p4.pt() > 25.)
                {
                    njet25 ++;
                }

            }

            vbs.tx.setBranch<int>("nbloose", nbloose);
            vbs.tx.setBranch<int>("nbmedium", nbmedium);
            vbs.tx.setBranch<int>("nbtight", nbtight);
            vbs.tx.setBranch<int>("ncenjet25", ncenjet25);
            vbs.tx.setBranch<int>("njet25", njet25);

            vbs.tx.sortVecBranchesByPt(
                /* name of the 4vector branch to use to pt sort by*/               
                "good_jets_p4",
                /* names of any associated vector<float> branches to sort along */ 
                {"good_jets_btag_score", "good_jets_qg_disc"},
                /* names of any associated vector<int>   branches to sort along */ 
                {"good_jets_loose_btagged", "good_jets_medium_btagged", "good_jets_tight_btagged"},
                /* names of any associated vector<bool>  branches to sort along */ 
                {}
            );

            return true;
        },
        UNITY
    );

    //*****************************
    // - Require Two Loose Btag
    //*****************************
    // Description: Select two loose b-tag /* TODO TODO TODO TODO btag scale factor */
    vbs.cutflow.addCutToLastActiveCut("GeqTwoLooseBtag", [&]() { return vbs.tx.getBranch<int>("nbloose") >= 2; }, UNITY);

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

        }, 
        UNITY
    );

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

            return true;

        }, 
        UNITY
    );

    vbs.cutflow.addCutToLastActiveCut("VBSJetPreselection",
        [&]()
        {
            LV vbs_jet_0 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(0);
            LV vbs_jet_1 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4").at(1);
            float M_jj = (vbs_jet_0 + vbs_jet_1).M();
            float deta_jj = vbs_jet_0.eta() - vbs_jet_1.eta();
            return (M_jj >= 300 && fabs(deta_jj) >= 2.5);
        }, 
        UNITY);

    //*****************************
    // - Tag two extra jets
    //*****************************
    // Description: save kinematics of extra jets (good jets != (vbs or higgs jets))
    vbs.cutflow.addCutToLastActiveCut("SelectExtraJets",
        [&]()
        {
            // Higgs and VBS jet idxs in good_jets_*
            const int& i_higgs_jet_0 = vbs.tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[0];
            const int& i_higgs_jet_1 = vbs.tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[1];
            const int& i_vbs_jet_0 = vbs.tx.getBranch<vector<int>>("vbs_jets_good_jets_idx")[0];
            const int& i_vbs_jet_1 = vbs.tx.getBranch<vector<int>>("vbs_jets_good_jets_idx")[1];
            // Good jet p4
            const vector<LV>& good_jets_p4 = vbs.tx.getBranch<vector<LV>>("good_jets_p4");
            const vector<float>& good_jets_qg_disc = vbs.tx.getBranch<vector<float>>("good_jets_qg_disc");
            // Find extra jets
            vector<int> extra_jets_good_jets_idx;
            vector<LV> extra_jets_p4;
            vector<float> extra_jets_qg_disc;
            // Good jets include b-jets -- they only have a 20 GeV pt requirement
            for (int i = 0; i < int(good_jets_p4.size()); i++)
            {
                // Reject any jets tagged as b-jets from Higgs
                if (i == i_higgs_jet_0) { continue; }
                if (i == i_higgs_jet_1) { continue; }
                // Reject any jets tagged as VBS jets
                if (i == i_vbs_jet_0) { continue; }
                if (i == i_vbs_jet_1) { continue; }
                // Further requirements
                if (good_jets_p4[i].pt() < 30) { continue; }
                // These are "extra" jets!
                extra_jets_good_jets_idx.push_back(i);
                extra_jets_p4.push_back(good_jets_p4.at(i));
                extra_jets_qg_disc.push_back(good_jets_qg_disc.at(i));
            }
            vbs.tx.setBranch<vector<int>>("extra_jets_good_jets_idx", extra_jets_good_jets_idx);
            vbs.tx.setBranch<vector<LV>>("extra_jets_p4", extra_jets_p4);
            vbs.tx.setBranch<vector<float>>("extra_jets_qg_disc", extra_jets_qg_disc);
            // Sort by pt
            vbs.tx.sortVecBranchesByPt(
                // name of the vector<LV> branch to use to pt sort by
                "extra_jets_p4",
                // names of any associated vector<float> branches to sort along 
                {"extra_jets_qg_disc"},
                // names of any associated vector<int> branches to sort along 
                {"extra_jets_good_jets_idx"},
                // names of any associated vector<bool> branches to sort along
                {}
            );
            return true;
        },
        UNITY);

    //*****************************
    // - SR Preselection Dummy Cut Node
    //*****************************
    // Description: This is a dummy cut node to indicate that this node represents where all the SR preselection is applied
    vbs.cutflow.addCutToLastActiveCut("BDTPreselection", UNITY, UNITY);

    vbs.cutflow.addCutToLastActiveCut("ComputeBDTInputs",
        [&]()
        {
            // Event
            vbs.tx.setBranch<int>("BDT_run", nt.run());
            vbs.tx.setBranch<int>("BDT_event", nt.event());
            vbs.tx.setBranch<int>("BDT_lumi", nt.luminosityBlock());
            // MET
            const LV& MET_p4 = vbs.tx.getBranch<LV>("met_p4");
            vbs.tx.setBranch<float>("BDT_MET_pt", MET_p4.pt());
            vbs.tx.setBranch<float>("BDT_MET_phi", MET_p4.phi());
            // b-jets
            const LV& ld_bjet_p4 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4")[0];
            const LV& tr_bjet_p4 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4")[1];
            const float& ld_bjet_score = vbs.tx.getBranch<vector<float>>("higgs_jets_btag_score")[0];
            const float& tr_bjet_score = vbs.tx.getBranch<vector<float>>("higgs_jets_btag_score")[1];
            vbs.tx.setBranch<float>("BDT_ld_bjet_score", ld_bjet_score);
            vbs.tx.setBranch<float>("BDT_tr_bjet_score", tr_bjet_score);
            vbs.tx.setBranch<float>("BDT_bjet_pair_pt", (ld_bjet_p4 + tr_bjet_p4).pt());
            vbs.tx.setBranch<float>("BDT_bjet_pair_dR", RooUtil::Calc::DeltaR(ld_bjet_p4, tr_bjet_p4));
            // Leptons
            const LV& ld_lep_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0];
            const LV& tr_lep_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1];
            const int& ld_lep_flav = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[0];
            const int& tr_lep_flav = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[1];
            const float& ld_lep_reliso = vbs.tx.getBranch<vector<float>>("good_leptons_miniPFRelIso_all")[0];
            const float& tr_lep_reliso = vbs.tx.getBranch<vector<float>>("good_leptons_miniPFRelIso_all")[1];
            vbs.tx.setBranch<int>("BDT_ld_lep_flav", ld_lep_flav);
            vbs.tx.setBranch<float>("BDT_ld_lep_pt", ld_lep_p4.pt());
            vbs.tx.setBranch<float>("BDT_ld_lep_eta", ld_lep_p4.eta());
            vbs.tx.setBranch<float>("BDT_ld_lep_phi", ld_lep_p4.phi());
            vbs.tx.setBranch<float>("BDT_ld_lep_reliso", ld_lep_reliso);
            vbs.tx.setBranch<int>("BDT_tr_lep_flav", tr_lep_flav);
            vbs.tx.setBranch<float>("BDT_tr_lep_pt", tr_lep_p4.pt());
            vbs.tx.setBranch<float>("BDT_tr_lep_eta", tr_lep_p4.eta());
            vbs.tx.setBranch<float>("BDT_tr_lep_phi", tr_lep_p4.phi());
            vbs.tx.setBranch<float>("BDT_tr_lep_reliso", tr_lep_reliso);
            // VBS jets
            const LV& ld_vbs_p4 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4")[0];
            const LV& tr_vbs_p4 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4")[1];
            vbs.tx.setBranch<float>("BDT_ld_vbsjet_P", ld_vbs_p4.P());
            vbs.tx.setBranch<float>("BDT_tr_vbsjet_P", tr_vbs_p4.P());
            vbs.tx.setBranch<float>("BDT_vbsjet_pair_M", (ld_vbs_p4 + tr_vbs_p4).M());
            vbs.tx.setBranch<float>("BDT_vbsjet_pair_deta", fabs(ld_vbs_p4.eta() - tr_vbs_p4.eta()));
            // Extra jets
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            vbs.tx.setBranch<int>("BDT_n_extrajets", extra_jets_p4.size());
            if (extra_jets_p4.size() < 2) 
            { 
                // Store all l+b pairs
                LV ld_lep_ld_b_p4 = ld_lep_p4 + ld_bjet_p4;
                vbs.tx.setBranch<float>("BDT_ld_lep_ld_b_pt", ld_lep_ld_b_p4.pt());
                vbs.tx.setBranch<float>("BDT_ld_lep_ld_b_M", ld_lep_ld_b_p4.M());
                vbs.tx.setBranch<float>("BDT_ld_lep_ld_b_dR", RooUtil::Calc::DeltaR(ld_lep_p4, ld_bjet_p4));
                vbs.tx.setBranch<float>("BDT_ld_lep_ld_b_dphi", fabs(ld_lep_p4.phi() - ld_bjet_p4.phi()));
                LV tr_lep_ld_b_p4 = tr_lep_p4 + ld_bjet_p4;
                vbs.tx.setBranch<float>("BDT_tr_lep_ld_b_pt", tr_lep_ld_b_p4.pt());
                vbs.tx.setBranch<float>("BDT_tr_lep_ld_b_M", tr_lep_ld_b_p4.M());
                vbs.tx.setBranch<float>("BDT_tr_lep_ld_b_dR", RooUtil::Calc::DeltaR(tr_lep_p4, ld_bjet_p4));
                vbs.tx.setBranch<float>("BDT_tr_lep_ld_b_dphi", fabs(tr_lep_p4.phi() - ld_bjet_p4.phi()));
                LV ld_lep_tr_b_p4 = ld_lep_p4 + tr_bjet_p4;
                vbs.tx.setBranch<float>("BDT_ld_lep_tr_b_pt", ld_lep_tr_b_p4.pt());
                vbs.tx.setBranch<float>("BDT_ld_lep_tr_b_M", ld_lep_tr_b_p4.M());
                vbs.tx.setBranch<float>("BDT_ld_lep_tr_b_dR", RooUtil::Calc::DeltaR(ld_lep_p4, tr_bjet_p4));
                vbs.tx.setBranch<float>("BDT_ld_lep_tr_b_dphi", fabs(ld_lep_p4.phi() - tr_bjet_p4.phi()));
                LV tr_lep_tr_b_p4 = tr_lep_p4 + tr_bjet_p4;
                vbs.tx.setBranch<float>("BDT_tr_lep_tr_b_pt", tr_lep_tr_b_p4.pt());
                vbs.tx.setBranch<float>("BDT_tr_lep_tr_b_M", tr_lep_tr_b_p4.M());
                vbs.tx.setBranch<float>("BDT_tr_lep_tr_b_dR", RooUtil::Calc::DeltaR(tr_lep_p4, tr_bjet_p4));
                vbs.tx.setBranch<float>("BDT_tr_lep_tr_b_dphi", fabs(tr_lep_p4.phi() - tr_bjet_p4.phi()));
            }
            else
            {
                // Find W hypothesis
                float min_dM = 9999.;
                pair<LV, LV> best_W_hyp;
                for (unsigned int i = 0; i < extra_jets_p4.size(); i++)
                {   
                    for (unsigned int j = i+1; j < extra_jets_p4.size(); j++)
                    {
                        // Sort into leading (ld) and trailing (tr)
                        LV ld_extrajet_p4 = extra_jets_p4.at(i);
                        LV tr_extrajet_p4 = extra_jets_p4.at(j);
                        // Check distance to W mass
                        LV extrajet_pair_p4 = (ld_extrajet_p4 + tr_extrajet_p4);
                        float this_dM = fabs(80.379 - (extrajet_pair_p4).M());
                        if (this_dM < min_dM)
                        {
                            best_W_hyp = make_pair(ld_extrajet_p4, tr_extrajet_p4);
                            min_dM = this_dM;
                        }
                    }
                }
                LV best_W_hyp_p4 = best_W_hyp.first + best_W_hyp.second;
                // Find t->W+b hypothesis and t->l+b(+nu) hypothesis
                pair<LV, LV> best_t_to_W_b_hyp;
                pair<LV, LV> best_t_to_l_b_hyp;
                float t_to_W_b_hyp_1 = (ld_bjet_p4 + best_W_hyp_p4).M();
                float t_to_W_b_hyp_2 = (tr_bjet_p4 + best_W_hyp_p4).M();
                if (fabs(172.76 - t_to_W_b_hyp_1) < fabs(172.76 - t_to_W_b_hyp_2))
                {
                    // t -> W + leading bjet (hypothesis 1) is better
                    best_t_to_W_b_hyp = make_pair(best_W_hyp_p4, ld_bjet_p4);
                    // --> use trailing bjet for t -> l(+nu)+b hypothesis
                    float t_to_l_b_hyp_1 = (ld_lep_p4 + tr_bjet_p4).M();
                    float t_to_l_b_hyp_2 = (tr_lep_p4 + tr_bjet_p4).M();
                    if (fabs(172.76 - t_to_l_b_hyp_1) < fabs(172.76 - t_to_l_b_hyp_2))
                    {
                        // t -> leading lep + trailing bjet (hypothesis 1) is better
                        best_t_to_l_b_hyp = make_pair(ld_lep_p4, tr_bjet_p4);
                    }
                    else
                    {
                        // t -> trailing lep + trailing bjet (hypothesis 2) is better
                        best_t_to_l_b_hyp = make_pair(tr_lep_p4, tr_bjet_p4);
                    }
                }
                else
                {
                    // t -> W + trailing bjet (hypothesis 2) is better
                    best_t_to_W_b_hyp = make_pair(best_W_hyp_p4, tr_bjet_p4);
                    // --> use leading bjet for t -> l(+nu)+b hypothesis
                    float t_to_l_b_hyp_1 = (ld_lep_p4 + ld_bjet_p4).M();
                    float t_to_l_b_hyp_2 = (tr_lep_p4 + ld_bjet_p4).M();
                    if (fabs(172.76 - t_to_l_b_hyp_1) < fabs(172.76 - t_to_l_b_hyp_2))
                    {
                        // t -> leading lep + leading bjet (hypothesis 1) is better
                        best_t_to_l_b_hyp = make_pair(ld_lep_p4, ld_bjet_p4);
                    }
                    else
                    {
                        // t -> trailing lep + leading bjet (hypothesis 2) is better
                        best_t_to_l_b_hyp = make_pair(tr_lep_p4, ld_bjet_p4);
                    }
                }
                LV best_t_to_W_b_hyp_p4 = best_t_to_W_b_hyp.first + best_t_to_W_b_hyp.second;
                LV best_t_to_l_b_hyp_p4 = best_t_to_l_b_hyp.first + best_t_to_l_b_hyp.second;
                // W->jj hypothesis
                vbs.tx.setBranch<float>("BDT_W_hyp_pt", best_W_hyp_p4.pt());
                vbs.tx.setBranch<float>("BDT_W_hyp_M", best_W_hyp_p4.M());
                vbs.tx.setBranch<float>("BDT_W_hyp_dR", RooUtil::Calc::DeltaR(best_W_hyp.first, best_W_hyp.second));
                vbs.tx.setBranch<float>("BDT_W_hyp_dphi", fabs(RooUtil::Calc::DeltaPhi(best_W_hyp.first, best_W_hyp.second)));
                // t->W+b hypothesis
                vbs.tx.setBranch<float>("BDT_t_to_W_b_hyp_pt", best_t_to_W_b_hyp_p4.pt());
                vbs.tx.setBranch<float>("BDT_t_to_W_b_hyp_M", best_t_to_W_b_hyp_p4.M());
                vbs.tx.setBranch<float>("BDT_t_to_W_b_hyp_dR", RooUtil::Calc::DeltaR(best_t_to_W_b_hyp.first, best_t_to_W_b_hyp.second));
                vbs.tx.setBranch<float>("BDT_t_to_W_b_hyp_dphi", fabs(RooUtil::Calc::DeltaPhi(best_t_to_W_b_hyp.first, best_t_to_W_b_hyp.second)));
                // t->l+b(+nu) hypothesis
                vbs.tx.setBranch<float>("BDT_t_to_lep_b_hyp_pt", best_t_to_l_b_hyp_p4.pt());
                vbs.tx.setBranch<float>("BDT_t_to_lep_b_hyp_M", best_t_to_l_b_hyp_p4.M());
                vbs.tx.setBranch<float>("BDT_t_to_lep_b_hyp_dR", RooUtil::Calc::DeltaR(best_t_to_l_b_hyp.first, best_t_to_l_b_hyp.second));
                vbs.tx.setBranch<float>("BDT_t_to_lep_b_hyp_dphi", fabs(RooUtil::Calc::DeltaPhi(best_t_to_l_b_hyp.first, best_t_to_l_b_hyp.second)));
            }
            return true;
        },
        UNITY
    );

    vbs.cutflow.printCuts();

    vbs.cutflow.bookCutflows();

    // Looping input file
    while (vbs.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (vbs.job_index != -1 and vbs.nsplit_jobs > 0)
        {
            if (vbs.looper.getNEventsProcessed() % vbs.nsplit_jobs != (unsigned int) vbs.job_index)
                continue;
        }

        vbs.process("ComputeBDTInputs");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
