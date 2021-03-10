#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

#include "VBSHWW.h"

#include "bdt.icc"
#include "kitchen_sink_bdt.icc"

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    VBSHWW vbs(argc, argv);

    // Additional branches
    vbs.tx.createBranch<vector<int>>("extra_jets_good_jets_idx");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4");
    vbs.tx.createBranch<vector<float>>("extra_jets_qg_disc");
    vbs.tx.createBranch<vector<LV>>("q_from_W_p4");
    vbs.tx.createBranch<vector<int>>("q_from_W_id");
    vbs.tx.createBranch<vector<LV>>("extra_jet_pair_ld_p4");
    vbs.tx.createBranch<vector<LV>>("extra_jet_pair_tr_p4");
    vbs.tx.createBranch<vector<LV>>("MaxBDT_extra_jets_p4");
    vbs.tx.createBranch<float>("event_weight");
    vbs.tx.createBranch<float>("MaxBDT_extra_jets_score");
    vbs.tx.createBranch<float>("MaxKitchenSink_extra_jets_score");
    vbs.tx.createBranch<float>("MinDeltaM_extra_jets_deltaM");
    vbs.tx.createBranch<float>("MinDeltaR_extra_jets_deltaM");
    // (BDT) Extra jets
    vbs.tx.createBranch<vector<bool>>("MVA_extra_jet_pair_is_match");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_dR");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_M");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_eta");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_phi");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_ld_qg_disc");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_pt");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_eta");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_phi");
    vbs.tx.createBranch<vector<float>>("MVA_extra_jet_pair_tr_qg_disc");
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

    vbs.cutflow.addCutToLastActiveCut("SaveWeight",
        [&]()
        {
            float weight = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0))*vbs.scale1fb;
            vbs.tx.setBranch<float>("event_weight", weight*gconf.lumi);
            return true;
        },
        UNITY
    );

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
    // - Tag two extra jets
    //*****************************
    // Description: save kinematics of extra jets (good jets != (vbs or higgs jets))
    vbs.cutflow.addCutToLastActiveCut("MakeExtraJetPairs",
        [&]()
        {
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
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
                    if (extra_jets_p4[i].pt() > extra_jets_p4[j].pt()) 
                    {
                        ld_extrajet_p4 = extra_jets_p4.at(i);
                        tr_extrajet_p4 = extra_jets_p4.at(j);
                        ld_qg_disc = extra_jets_qg_disc.at(i);
                        tr_qg_disc = extra_jets_qg_disc.at(j);
                    }
                    else 
                    {
                        ld_extrajet_p4 = extra_jets_p4.at(j);
                        tr_extrajet_p4 = extra_jets_p4.at(i);
                        ld_qg_disc = extra_jets_qg_disc.at(j);
                        tr_qg_disc = extra_jets_qg_disc.at(i);
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
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_pt", tr_extrajet_p4.pt());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_eta", tr_extrajet_p4.eta());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_phi", tr_extrajet_p4.phi());
                    vbs.tx.pushbackToBranch<float>("MVA_extra_jet_pair_tr_qg_disc", tr_qg_disc);
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

    vbs.cutflow.addCutToLastActiveCut("MakeAdditionalVariables",
        [&]()
        {
            const vector<float>& extra_jet_pair_M = vbs.tx.getBranchLazy<vector<float>>("MVA_extra_jet_pair_M");
            if (extra_jet_pair_M.size() == 0) { return true; }
            vector<float> extra_jet_pair_dM;
            float W_mass = 80.379;
            for (auto& Mjj : extra_jet_pair_M)
            {
                extra_jet_pair_dM.push_back(fabs(Mjj - W_mass));
            }
            const vector<float>& extra_jet_pair_dR = vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_dR");
            int dM_argmin = std::distance(
                extra_jet_pair_dM.begin(), 
                std::min_element(extra_jet_pair_dM.begin(), extra_jet_pair_dM.end())
            );
            int dR_argmin = std::distance(
                extra_jet_pair_dR.begin(), 
                std::min_element(extra_jet_pair_dR.begin(), extra_jet_pair_dR.end())
            );
            vbs.tx.setBranch<float>("MinDeltaM_extra_jets_deltaM", extra_jet_pair_dM.at(dM_argmin));
            vbs.tx.setBranch<float>("MinDeltaR_extra_jets_deltaM", extra_jet_pair_dM.at(dR_argmin));

            return true;
        },
        UNITY
    );

    vbs.cutflow.addCutToLastActiveCut("ApplyExtraJetsBDT",
        [&]()
        {
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            if (extra_jets_p4.size() < 2) { return true; }
            int n_extra_jet_pairs = vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_M").size();
            // Run BDT on all possible extra jet pairs
            vector<float> bdt_discs;
            vector<float> kitchen_sink_discs;
            vector<int> extra_jet_pair_idx;
            for (int i = 0; i < n_extra_jet_pairs; i++)
            {
                extra_jet_pair_idx.push_back(i);
                // Apply BDT
                float bdt_disc = extraJetBDT(
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_eta").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_phi").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_qg_disc").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_eta").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_phi").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_qg_disc").at(i)
                );
                bdt_discs.push_back(bdt_disc);
                // Apply Kitchen Sink BDT
                float kitchen_sink_disc = extraJetKitchenSinkBDT(
                    vbs.tx.getBranch<float>("MVA_MET_phi"),
                    vbs.tx.getBranch<float>("MVA_MET_pt"),
                    vbs.tx.getBranch<float>("MVA_bjet_pair_ld_pt"),
                    vbs.tx.getBranch<float>("MVA_bjet_pair_tr_pt"),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_bjet_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_eta").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_lep_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_phi").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_ld_qg_disc").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_bjet_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_eta").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_M").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_dR").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_lep_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_phi").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_pt").at(i),
                    vbs.tx.getBranch<vector<float>>("MVA_extra_jet_pair_tr_qg_disc").at(i),
                    vbs.tx.getBranch<float>("MVA_lep_pair_ld_phi"),
                    vbs.tx.getBranch<float>("MVA_lep_pair_ld_pt"),
                    vbs.tx.getBranch<float>("MVA_lep_pair_tr_phi"),
                    vbs.tx.getBranch<float>("MVA_lep_pair_tr_pt"),
                    vbs.tx.getBranch<float>("MVA_vbs_pair_ld_phi"),
                    vbs.tx.getBranch<float>("MVA_vbs_pair_ld_pt"),
                    vbs.tx.getBranch<float>("MVA_vbs_pair_tr_phi"),
                    vbs.tx.getBranch<float>("MVA_vbs_pair_tr_pt")
                );
                kitchen_sink_discs.push_back(kitchen_sink_disc);
            }
            if (bdt_discs.empty()) { return true; }
            const vector<LV>& extra_jet_pair_ld_p4 = vbs.tx.getBranch<vector<LV>>("extra_jet_pair_ld_p4");
            const vector<LV>& extra_jet_pair_tr_p4 = vbs.tx.getBranch<vector<LV>>("extra_jet_pair_tr_p4");

            int argmax = std::distance(
                bdt_discs.begin(), 
                std::max_element(bdt_discs.begin(), bdt_discs.end())
            );
            int idx = extra_jet_pair_idx.at(argmax);
            vbs.tx.setBranch<vector<LV>>(
                "MaxBDT_extra_jets_p4",
                {extra_jet_pair_ld_p4.at(idx), extra_jet_pair_tr_p4.at(idx)}
            );
            vbs.tx.setBranch<float>("MaxBDT_extra_jets_score", bdt_discs.at(argmax));
            if (kitchen_sink_discs.empty()) { return true; }
            int ks_argmax = std::distance(
                kitchen_sink_discs.begin(), 
                std::max_element(kitchen_sink_discs.begin(), kitchen_sink_discs.end())
            );
            vbs.tx.setBranch<float>("MaxKitchenSink_extra_jets_score", kitchen_sink_discs.at(ks_argmax));
            return true;
        },
        UNITY
    );

    vbs.cutflow.addCutToLastActiveCut("SelectPairsEnteringBDT",
        [&]()
        {
            return (vbs.tx.getBranchLazy<vector<LV>>("MaxBDT_extra_jets_p4").size() > 0);
        },
        UNITY
    );

    vbs.cutflow.addCutToLastActiveCut("FailCutOnExtraJetsBDT",
        [&]()
        {
            const float& max_bdt_score = vbs.tx.getBranch<float>("MaxBDT_extra_jets_score");
            return (max_bdt_score >= 0.6);
        },
        UNITY
    );

    vbs.cutflow.getCut("SelectPairsEnteringBDT");
    vbs.cutflow.addCutToLastActiveCut("PassCutOnExtraJetsBDT",
        [&]()
        {
            const float& max_bdt_score = vbs.tx.getBranch<float>("MaxBDT_extra_jets_score");
            return (max_bdt_score < 0.6);
        },
        UNITY
    );

    vbs.cutflow.getCut("ApplyExtraJetsBDT");
    vbs.cutflow.addCutToLastActiveCut("CutOnExtraJetsBDT",
        [&]()
        {
            if (vbs.tx.getBranchLazy<vector<LV>>("MaxBDT_extra_jets_p4").size() == 0) { return true; }
            const float& max_bdt_score = vbs.tx.getBranch<float>("MaxBDT_extra_jets_score");
            return (max_bdt_score < 0.6);
        },
        UNITY
    );

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
            bool q0_in_calo = (fabs(q_from_W_0.eta()) < 5.);
            bool q1_in_calo = (fabs(q_from_W_1.eta()) < 5.);

            return (q0_in_calo && q1_in_calo && q_from_W_1.pt() > 30);
        },
        UNITY);

    vbs.cutflow.addCutToLastActiveCut("MatchQQtoMaxBDTPair",
        [&]()
        {
            if (vbs.tx.getBranchLazy<vector<LV>>("MaxBDT_extra_jets_p4").size() == 0) { return false; }
            const LV& q_from_W_0 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[0];
            const LV& q_from_W_1 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[1];
            const LV& extra_jet_0 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[0];
            const LV& extra_jet_1 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[1];
            float dR_00 = RooUtil::Calc::DeltaR(q_from_W_0, extra_jet_0);
            float dR_10 = RooUtil::Calc::DeltaR(q_from_W_1, extra_jet_0);
            float dR_01 = RooUtil::Calc::DeltaR(q_from_W_0, extra_jet_1);
            float dR_11 = RooUtil::Calc::DeltaR(q_from_W_1, extra_jet_1);
            return ((dR_00 < 0.4 && dR_11 < 0.4) || (dR_10 < 0.4 && dR_01 < 0.4));
        },
        UNITY);

    // vbs.cutflow.getCut("SelectGenWtoQQ");
    // vbs.cutflow.addCutToLastActiveCut("MatchQQtoOnePair",
    //     [&]()
    //     {
    //         const LV& q_from_W_0 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[0];
    //         const LV& q_from_W_1 = vbs.tx.getBranch<vector<LV>>("q_from_W_p4")[1];
    //         const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
    //         int n_matches = 0;
    //         for (unsigned int i = 0; i < extra_jets_p4.size(); i++)
    //         {   
    //             LV p4_i = extra_jets_p4.at(i);
    //             for (unsigned int j = i+1; j < extra_jets_p4.size(); j++)
    //             {
    //                 LV p4_j = extra_jets_p4.at(j);
    //                 float dR_i0 = RooUtil::Calc::DeltaR(p4_i, q_from_W_0);
    //                 float dR_i1 = RooUtil::Calc::DeltaR(p4_i, q_from_W_1);
    //                 float dR_j0 = RooUtil::Calc::DeltaR(p4_j, q_from_W_0);
    //                 float dR_j1 = RooUtil::Calc::DeltaR(p4_j, q_from_W_1);
    //                 bool matched = (dR_i0 < 0.4 && dR_j1 < 0.4) || (dR_i1 < 0.4 && dR_j0 < 0.4);
    //                 vbs.tx.pushbackToBranch<bool>("MVA_extra_jet_pair_is_match", matched);
    //                 if (matched) { n_matches++; }
    //             }
    //         }
    //         return (n_matches == 1);
    //     },
    //     UNITY);

    RooUtil::Histograms bdt_hists;
    bdt_hists.addHistogram("All_extraJetPair_Mjj", 6, 50, 110, 
        [&]() 
        { 
            const LV& extra_jet_0 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[0];
            const LV& extra_jet_1 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[1];
            return (extra_jet_0 + extra_jet_1).M();
        } 
    );
    bdt_hists.addHistogram("MaxBDT_extraJetPair_Mjj", 6, 50, 110, 
        [&]() 
        { 
            const LV& extra_jet_0 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[0];
            const LV& extra_jet_1 = vbs.tx.getBranch<vector<LV>>("MaxBDT_extra_jets_p4")[1];
            return (extra_jet_0 + extra_jet_1).M();
        } 
    );
    vbs.cutflow.bookHistogramsForCut(bdt_hists, "SelectPairsEnteringBDT");
    vbs.cutflow.bookHistogramsForCut(bdt_hists, "PassCutOnExtraJetsBDT");
    vbs.cutflow.bookHistogramsForCut(bdt_hists, "FailCutOnExtraJetsBDT");

    RooUtil::Histograms hists;
    hists.addHistogram("N_extraJets", 20, 0, 20,
        [&]()
        {
            return vbs.tx.getBranchLazy<vector<LV>>("extra_jets_p4").size();
        }
    );
    vbs.cutflow.bookHistogramsForCut(hists, "SelectExtraJets");

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

        vbs.process("ApplyExtraJetsBDT");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
