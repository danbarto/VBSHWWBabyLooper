#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

#include "VBSHWW.h"

#include "bdt.icc"

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    VBSHWW vbs(argc, argv);

    // Additional branches
    vbs.tx.createBranch<vector<int>>("extra_jets_good_jets_idx");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4");
    vbs.tx.createBranch<vector<float>>("extra_jets_qg_disc");
    vbs.tx.createBranch<vector<float>>("extra_jets_btag_score");
    vbs.tx.createBranch<vector<LV>>("q_from_W_p4");
    vbs.tx.createBranch<vector<int>>("q_from_W_id");
    vbs.tx.createBranch<vector<LV>>("extra_jet_pair_ld_p4");
    vbs.tx.createBranch<vector<LV>>("extra_jet_pair_tr_p4");
    // (BDT) Extra jets
    vbs.tx.createBranch<vector<bool>>("MVA_extra_jet_pair_is_match");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_dR");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_eta");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_phi");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_qg_disc");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_deepjet"); // PROPOSED
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_eta");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_phi");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_qg_disc");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_deepjet"); // PROPOSED
    // (BDT) Extra jets + Higgs b-jets
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_dR");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_dR");
    // (BDT) Extra jets + leptons
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_dR");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_dR");
    // (BDT) Event
    vbs.tx.createBranch<int>("MVA_run");
    vbs.tx.createBranch<int>("MVA_event");
    vbs.tx.createBranch<int>("MVA_lumi");
    vbs.tx.createBranch<float>("MVA_MET_pt");
    vbs.tx.createBranch<float>("MVA_MET_phi");
    // (BDT) Higgs b-jets
    vbs.tx.createBranch<float>("MVA_bjet_pair_ld_deepjet");
    vbs.tx.createBranch<float>("MVA_bjet_pair_tr_deepjet");
    vbs.tx.createBranch<float>("MVA_bjet_pair_ld_pt");
    vbs.tx.createBranch<float>("MVA_bjet_pair_tr_pt");
    vbs.tx.createBranch<float>("MVA_bjet_pair_ld_eta");
    vbs.tx.createBranch<float>("MVA_bjet_pair_tr_eta");
    vbs.tx.createBranch<float>("MVA_bjet_pair_ld_phi");
    vbs.tx.createBranch<float>("MVA_bjet_pair_tr_phi");
    // (BDT) Leptons
    vbs.tx.createBranch<int>("MVA_lep_pair_ld_id");
    vbs.tx.createBranch<int>("MVA_lep_pair_tr_id");
    vbs.tx.createBranch<float>("MVA_lep_pair_ld_pt");
    vbs.tx.createBranch<float>("MVA_lep_pair_tr_pt");
    vbs.tx.createBranch<float>("MVA_lep_pair_ld_eta");
    vbs.tx.createBranch<float>("MVA_lep_pair_tr_eta");
    vbs.tx.createBranch<float>("MVA_lep_pair_ld_phi");
    vbs.tx.createBranch<float>("MVA_lep_pair_tr_phi");
    // (BDT) VBS jets
    vbs.tx.createBranch<float>("MVA_vbs_pair_ld_pt");
    vbs.tx.createBranch<float>("MVA_vbs_pair_tr_pt");
    vbs.tx.createBranch<float>("MVA_vbs_pair_ld_eta");
    vbs.tx.createBranch<float>("MVA_vbs_pair_tr_eta");
    vbs.tx.createBranch<float>("MVA_vbs_pair_ld_phi");
    vbs.tx.createBranch<float>("MVA_vbs_pair_tr_phi");

    vbs.initSRCutflow();

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
            const vector<float>& good_jets_btag_score =  vbs.tx.getBranch<vector<float>>("good_jets_btag_score");
            // Find extra jets
            vector<int> extra_jets_good_jets_idx;
            vector<LV> extra_jets_p4;
            vector<float> extra_jets_qg_disc;
            vector<float> extra_jets_btag_score;
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
                extra_jets_btag_score.push_back(good_jets_btag_score.at(i));
            }
            vbs.tx.setBranch<vector<int>>("extra_jets_good_jets_idx", extra_jets_good_jets_idx);
            vbs.tx.setBranch<vector<LV>>("extra_jets_p4", extra_jets_p4);
            vbs.tx.setBranch<vector<float>>("extra_jets_qg_disc", extra_jets_qg_disc);
            vbs.tx.setBranch<vector<float>>("extra_jets_btag_score", extra_jets_btag_score);
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
    // - Tag two extra jets
    //*****************************
    // Description: save kinematics of extra jets (good jets != (vbs or higgs jets))
    vbs.cutflow.addCutToLastActiveCut("MakeExtraJetPairs",
        [&]()
        {
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            const vector<float>& extra_jets_btag_scores = vbs.tx.getBranch<vector<float>>("extra_jets_btag_score");
            if (extra_jets_p4.size() < 2) { return true; }
            const LV& MET_p4 = vbs.tx.getBranch<LV>("met_p4");
            const vector<float>& extra_jets_qg_disc = vbs.tx.getBranch<vector<float>>("extra_jets_qg_disc");
            const LV& ld_bjet_p4 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4")[0];
            const LV& tr_bjet_p4 = vbs.tx.getBranch<vector<LV>>("higgs_jets_p4")[1];
            const float& ld_bjet_score = vbs.tx.getBranch<vector<float>>("higgs_jets_btag_score")[0];
            const float& tr_bjet_score = vbs.tx.getBranch<vector<float>>("higgs_jets_btag_score")[1];
            const LV& ld_lep_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[0];
            const LV& tr_lep_p4 = vbs.tx.getBranch<vector<LV>>("good_leptons_p4")[1];
            const int& ld_lep_id = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[0];
            const int& tr_lep_id = vbs.tx.getBranch<vector<int>>("good_leptons_pdgid")[1];
            const LV& ld_vbs_p4 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4")[0];
            const LV& tr_vbs_p4 = vbs.tx.getBranch<vector<LV>>("vbs_jets_p4")[1];
            for (unsigned int i = 0; i < extra_jets_p4.size(); i++)
            {   
                for (unsigned int j = i+1; j < extra_jets_p4.size(); j++)
                {
                    // Sort into leading (ld) and trailing (tr)
                    LV ld_extrajet_p4;
                    LV tr_extrajet_p4;
                    float ld_qg_disc;
                    float tr_qg_disc;
                    float ld_deepjet; // PROPOSED
                    float tr_deepjet; // PROPOSED
                    if (extra_jets_p4[i].pt() > extra_jets_p4[j].pt()) 
                    {
                        ld_extrajet_p4 = extra_jets_p4.at(i);
                        tr_extrajet_p4 = extra_jets_p4.at(j);
                        ld_qg_disc = extra_jets_qg_disc.at(i); // PROPOSED
                        tr_qg_disc = extra_jets_qg_disc.at(j); // PROPOSED
                        ld_deepjet = extra_jets_btag_scores.at(i); // PROPOSED
                        tr_deepjet = extra_jets_btag_scores.at(j); // PROPOSED
                    }
                    else 
                    {
                        ld_extrajet_p4 = extra_jets_p4.at(j);
                        tr_extrajet_p4 = extra_jets_p4.at(i);
                        ld_qg_disc = extra_jets_qg_disc.at(j);
                        tr_qg_disc = extra_jets_qg_disc.at(i);
                        ld_deepjet = extra_jets_btag_scores.at(j); // PROPOSED
                        tr_deepjet = extra_jets_btag_scores.at(i); // PROPOSED
                    }
                    // Extra jets
                    LV extrajet_pair_p4 = (ld_extrajet_p4 + tr_extrajet_p4);
                    vbs.tx.pushbackToBranch<LV>("extra_jet_pair_ld_p4", ld_extrajet_p4);
                    vbs.tx.pushbackToBranch<LV>("extra_jet_pair_tr_p4", tr_extrajet_p4);
                    vbs.tx.pushbackToBranch<float>(
                        "MVA_extra_jet_pair_dR",
                        RooUtil::Calc::DeltaR(ld_extrajet_p4, tr_extrajet_p4)
                    );
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_pt", extrajet_pair_p4.pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_M", extrajet_pair_p4.M());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_pt", ld_extrajet_p4.pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_eta", ld_extrajet_p4.eta());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_phi", ld_extrajet_p4.phi());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_qg_disc", ld_qg_disc);
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_deepjet", ld_deepjet); // PROPOSED
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_pt", tr_extrajet_p4.pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_eta", tr_extrajet_p4.eta());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_phi", tr_extrajet_p4.phi());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_qg_disc", tr_qg_disc);
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_deepjet", tr_deepjet); // PROPOSED
                    // Extra jets + Higgs b-jet systems
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_bjet_pt", (extrajet_pair_p4 + ld_bjet_p4).pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_bjet_M", (extrajet_pair_p4 + ld_bjet_p4).M());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_bjet_dR", RooUtil::Calc::DeltaR(extrajet_pair_p4, ld_bjet_p4));
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_bjet_pt", (extrajet_pair_p4 + tr_bjet_p4).pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_bjet_M", (extrajet_pair_p4 + tr_bjet_p4).M());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_bjet_dR", RooUtil::Calc::DeltaR(extrajet_pair_p4, tr_bjet_p4));
                    // Extra jets + lepton systems
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_lep_pt", (extrajet_pair_p4 + ld_lep_p4).pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_lep_M", (extrajet_pair_p4 + ld_lep_p4).M());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_ld_lep_dR", RooUtil::Calc::DeltaR(extrajet_pair_p4, ld_lep_p4));
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_lep_pt", (extrajet_pair_p4 + tr_lep_p4).pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_lep_M", (extrajet_pair_p4 + tr_lep_p4).M());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_lep_dR", RooUtil::Calc::DeltaR(extrajet_pair_p4, tr_lep_p4));
                    // Event
                    vbs.tx.setBranch<int>("MVA_run", nt.run());
                    vbs.tx.setBranch<int>("MVA_event", nt.event());
                    vbs.tx.setBranch<int>("MVA_lumi", nt.luminosityBlock());
                    vbs.tx.setBranch<float>("MVA_MET_pt", MET_p4.pt());
                    vbs.tx.setBranch<float>("MVA_MET_phi", MET_p4.phi());
                    // Higgs b-jets
                    vbs.tx.setBranch<float>("MVA_bjet_pair_ld_deepjet", ld_bjet_score);
                    vbs.tx.setBranch<float>("MVA_bjet_pair_tr_deepjet", tr_bjet_score);
                    vbs.tx.setBranch<float>("MVA_bjet_pair_ld_pt", ld_bjet_p4.pt());
                    vbs.tx.setBranch<float>("MVA_bjet_pair_tr_pt", tr_bjet_p4.pt());
                    vbs.tx.setBranch<float>("MVA_bjet_pair_ld_eta", ld_bjet_p4.eta());
                    vbs.tx.setBranch<float>("MVA_bjet_pair_tr_eta", tr_bjet_p4.eta());
                    vbs.tx.setBranch<float>("MVA_bjet_pair_ld_phi", ld_bjet_p4.phi());
                    vbs.tx.setBranch<float>("MVA_bjet_pair_tr_phi", tr_bjet_p4.phi());
                    // Leptons
                    vbs.tx.setBranch<int>("MVA_lep_pair_ld_id", ld_lep_id);
                    vbs.tx.setBranch<int>("MVA_lep_pair_tr_id", tr_lep_id);
                    vbs.tx.setBranch<float>("MVA_lep_pair_ld_pt", ld_lep_p4.pt());
                    vbs.tx.setBranch<float>("MVA_lep_pair_tr_pt", tr_lep_p4.pt());
                    vbs.tx.setBranch<float>("MVA_lep_pair_ld_eta", ld_lep_p4.eta());
                    vbs.tx.setBranch<float>("MVA_lep_pair_tr_eta", tr_lep_p4.eta());
                    vbs.tx.setBranch<float>("MVA_lep_pair_ld_phi", ld_lep_p4.phi());
                    vbs.tx.setBranch<float>("MVA_lep_pair_tr_phi", tr_lep_p4.phi());
                    // VBS jets
                    vbs.tx.setBranch<float>("MVA_vbs_pair_ld_pt", ld_vbs_p4.pt());
                    vbs.tx.setBranch<float>("MVA_vbs_pair_tr_pt", tr_vbs_p4.pt());
                    vbs.tx.setBranch<float>("MVA_vbs_pair_ld_eta", ld_vbs_p4.eta());
                    vbs.tx.setBranch<float>("MVA_vbs_pair_tr_eta", tr_vbs_p4.eta());
                    vbs.tx.setBranch<float>("MVA_vbs_pair_ld_phi", ld_vbs_p4.phi());
                    vbs.tx.setBranch<float>("MVA_vbs_pair_tr_phi", tr_vbs_p4.phi());
                }
            }
            return true;
        },
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("GeqTwoExtraJets",
        [&]()
        {
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            return (extra_jets_p4.size() >= 2);
        },
        UNITY
    );

    //*****************************
    // - Select W->qq events
    //*****************************
    // Description: Select events with a W->qq decay, save quark kinematics
    vbs.cutflow.addCutToLastActiveCut("SelectGenWtoQQ",
        [&]()
        {
            int quark_A_idx = -1;
            int quark_A_mother_idx = -1;
            int quark_B_idx = -1;
            for (unsigned int idx = 0; idx < nt.nGenPart(); ++idx)
            {
                if (abs(nt.GenPart_pdgId()[idx]) >= 6)  { continue; }
                if (nt.GenPart_status()[idx] != 23) { continue; }
                // Try to find mother
                int mother_idx = nt.GenPart_genPartIdxMother()[idx];
                if (mother_idx < 0) { continue; }
                // Check if mother is a W
                if (abs(nt.GenPart_pdgId()[mother_idx]) == 24) 
                {
                    if (quark_A_idx < 0) 
                    { 
                        quark_A_idx = idx; 
                        quark_A_mother_idx = mother_idx;
                    }
                    else if (mother_idx == quark_A_mother_idx) 
                    { 
                        quark_B_idx = idx; 
                        break;
                    }
                }
            }
            if (quark_A_idx < 0 || quark_B_idx < 0) { return false; }
            // Sort into leading and trailing
            int quark_idx_0;
            int quark_idx_1;
            if (nt.GenPart_pt()[quark_A_idx] >= nt.GenPart_pt()[quark_B_idx]) 
            {
                quark_idx_0 = quark_A_idx;
                quark_idx_1 = quark_B_idx;
            }
            else 
            {
                quark_idx_0 = quark_B_idx;
                quark_idx_1 = quark_A_idx;
            }
            // Fill branches
            vbs.tx.pushbackToBranch<LV>("q_from_W_p4", nt.GenPart_p4()[quark_idx_0]);
            vbs.tx.pushbackToBranch<LV>("q_from_W_p4", nt.GenPart_p4()[quark_idx_1]);
            vbs.tx.pushbackToBranch<int>("q_from_W_id", nt.GenPart_pdgId()[quark_idx_0]);
            vbs.tx.pushbackToBranch<int>("q_from_W_id", nt.GenPart_pdgId()[quark_idx_1]);
            return true;
        },
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("TaggableQQ",
        [&]()
        {
            const LV& q_from_W_0 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[0];
            const LV& q_from_W_1 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[1];
            // const vector<LV>& good_jets_p4 = vbs.tx.getBranch<vector<LV>>("good_jets_p4");
            // Require trailing pt > 30, forcing both quarks to have pt > 30
            // if (q_from_W_1.pt() < 30) { return false; }
            // // Select quarks that are within dR < 0.4 of any reco jet
            // bool matched_0 = false;
            // bool matched_1 = false;
            // for (auto& jet : good_jets_p4)
            // {
            //     if (!matched_0 && RooUtil::Calc::DeltaR(q_from_W_0, jet) < 0.4) { matched_0 = true; }
            //     else if (!matched_1 && RooUtil::Calc::DeltaR(q_from_W_1, jet) < 0.4) { matched_1 = true; }
            //     if (matched_0 && matched_1) { break; }
            // }
            // return (matched_0 && matched_1);
            bool q0_in_calo = (fabs(q_from_W_0.eta()) < 5.);
            bool q1_in_calo = (fabs(q_from_W_1.eta()) < 5.);

            return (q0_in_calo && q1_in_calo && q_from_W_1.pt() > 30);
        },
        UNITY);

    vbs.cutflow.getCut("SelectGenWtoQQ");
    vbs.cutflow.addCutToLastActiveCut("MatchQQtoOnePair",
        [&]()
        {
            const LV& q_from_W_0 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[0];
            const LV& q_from_W_1 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[1];
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            int n_matches = 0;
            for (unsigned int i = 0; i < extra_jets_p4.size(); i++)
            {   
                LV p4_i = extra_jets_p4.at(i);
                for (unsigned int j = i+1; j < extra_jets_p4.size(); j++)
                {
                    LV p4_j = extra_jets_p4.at(j);
                    float dR_i0 = RooUtil::Calc::DeltaR(p4_i, q_from_W_0);
                    float dR_i1 = RooUtil::Calc::DeltaR(p4_i, q_from_W_1);
                    float dR_j0 = RooUtil::Calc::DeltaR(p4_j, q_from_W_0);
                    float dR_j1 = RooUtil::Calc::DeltaR(p4_j, q_from_W_1);
                    bool matched = (dR_i0 < 0.4 && dR_j1 < 0.4) || (dR_i1 < 0.4 && dR_j0 < 0.4);
                    vbs.tx.pushbackToBranch<bool>("MVA_extra_jet_pair_is_match", matched);
                    if (matched) { n_matches++; }
                }
            }
            return (n_matches == 1);
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

        vbs.process("MatchQQtoOnePair");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
