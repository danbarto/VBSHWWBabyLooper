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
    vbs.tx.createBranch<vector<int>>("extra_jets_good_jets_idx");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4");
    vbs.tx.createBranch<vector<LV>>("extra_jets_p4_min_dR_pair");
    vbs.tx.createBranch<int>("leading_q_from_W_id");
    vbs.tx.createBranch<float>("leading_q_from_W_pt");
    vbs.tx.createBranch<float>("leading_q_from_W_eta");
    vbs.tx.createBranch<float>("leading_q_from_W_phi");
    vbs.tx.createBranch<bool>("leading_q_from_W_has_match"); // i.e. matched to extra jet
    vbs.tx.createBranch<int>("trailing_q_from_W_id");
    vbs.tx.createBranch<float>("trailing_q_from_W_pt");
    vbs.tx.createBranch<float>("trailing_q_from_W_eta");
    vbs.tx.createBranch<float>("trailing_q_from_W_phi");
    vbs.tx.createBranch<bool>("trailing_q_from_W_has_match"); // i.e. matched to extra jet

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
            // Find extra jets
            vector<int> extra_jets_good_jets_idx;
            vector<LV> extra_jets_p4;
            // Good jets include b-jets -- they only have a 20 GeV pt requirement
            for (int i = 0; i < int(good_jets_p4.size()); i++)
            {
                if (i == i_higgs_jet_0) { continue; }
                if (i == i_higgs_jet_1) { continue; }
                if (i == i_vbs_jet_0) { continue; }
                if (i == i_vbs_jet_1) { continue; }
                if (good_jets_p4[i].pt() < 30) { continue; }
                extra_jets_good_jets_idx.push_back(i);
                extra_jets_p4.push_back(good_jets_p4.at(i));
            }
            vbs.tx.setBranch<vector<int>>("extra_jets_good_jets_idx", extra_jets_good_jets_idx);
            vbs.tx.setBranch<vector<LV>>("extra_jets_p4", extra_jets_p4);
            // Sort by pt
            vbs.tx.sortVecBranchesByPt(
                // name of the vector<LV> branch to use to pt sort by
                "extra_jets_p4",
                // names of any associated vector<float> branches to sort along 
                {},
                // names of any associated vector<int> branches to sort along 
                {"extra_jets_good_jets_idx"},
                // names of any associated vector<bool> branches to sort along
                {}
            );
            return true;
        },
        UNITY);

    //*****************************
    // - Require two extra jets
    //*****************************
    // Description: select events with exactly 2 extra jets in addition to 2 VBS and 2 Higgs (b-jets)
    vbs.cutflow.addCutToLastActiveCut("TagTwoExtraJets",
        [&]()
        {
            const vector<LV>& extra_jets_p4 = vbs.tx.getBranch<vector<LV>>("extra_jets_p4");
            // In order to tag two jets, must have at least two
            if (extra_jets_p4.size() < 2) { return false; }
            // Select min dR(j1,j2) extra jet pair
            float min_dR = 9999.;
            vector<LV> min_dR_pair_p4;
            for (unsigned int i = 0; i < extra_jets_p4.size(); i++)
            {
                LV extra_jet_i = extra_jets_p4.at(i);
                for (unsigned int j = i+1; j < extra_jets_p4.size(); j++)
                {
                    LV extra_jet_j = extra_jets_p4.at(j);
                    float this_dR = RooUtil::Calc::DeltaR(extra_jet_i, extra_jet_j);
                    if (this_dR < min_dR) 
                    { 
                        min_dR = this_dR; 
                        // pt(i) > pt(j) because j > i and extra jets are sorted by pt
                        min_dR_pair_p4 = { extra_jet_i, extra_jet_j };
                    }
                }
            }
            vbs.tx.setBranch<vector<LV>>("extra_jets_p4_min_dR_pair", min_dR_pair_p4);
            return (min_dR != 9999.);
        },
        UNITY);

    //*****************************
    // - Fill generator branches
    //*****************************
    // Description: 'benign' cut that only serves to fill some generator branches
    vbs.cutflow.addCutToLastActiveCut("FillGenBranches",
        [&]()
        {
            int quark_A_idx = -1;
            int quark_A_mother_idx = -1;
            int quark_B_idx = -1;
            for (unsigned int idx = 0; idx < nt.nGenPart(); ++idx)
            {
                if (abs(nt.GenPart_pdgId()[idx]) < 6 && nt.GenPart_status()[idx] == 23) 
                {
                    // Try to find mother
                    int mother_idx = nt.GenPart_genPartIdxMother()[idx];
                    if (mother_idx < 0) { continue; }
                    // Check if mother is a W
                    if (abs(nt.GenPart_pdgId()[mother_idx]) == 24) 
                    {
                        if (quark_A_idx < 0) { quark_A_idx = idx; }
                        else if (mother_idx == quark_A_mother_idx) 
                        { 
                            quark_B_idx = idx; 
                            break;
                        }
                    }
                }
            }
            if (quark_A_idx < 0 || quark_B_idx < 0) { return true; }
            // Sort into leading and trailing
            int leading_quark_idx;
            int trailing_quark_idx;
            if (nt.GenPart_pt()[quark_A_idx] >= nt.GenPart_pt()[quark_B_idx]) 
            {
                leading_quark_idx = quark_A_idx;
                trailing_quark_idx = quark_B_idx;
            }
            else {
            
                leading_quark_idx = quark_B_idx;
                trailing_quark_idx = quark_A_idx;
            }
            // Fill branches
            vbs.tx.setBranch<int>("leading_q_from_W_id", nt.GenPart_pdgId()[leading_quark_idx]);
            vbs.tx.setBranch<float>("leading_q_from_W_pt", nt.GenPart_pt()[leading_quark_idx]);
            vbs.tx.setBranch<float>("leading_q_from_W_eta", nt.GenPart_eta()[leading_quark_idx]);
            vbs.tx.setBranch<float>("leading_q_from_W_phi", nt.GenPart_phi()[leading_quark_idx]);
            vbs.tx.setBranch<int>("trailing_q_from_W_id", nt.GenPart_pdgId()[trailing_quark_idx]);
            vbs.tx.setBranch<float>("trailing_q_from_W_pt", nt.GenPart_pt()[trailing_quark_idx]);
            vbs.tx.setBranch<float>("trailing_q_from_W_eta", nt.GenPart_eta()[trailing_quark_idx]);
            vbs.tx.setBranch<float>("trailing_q_from_W_phi", nt.GenPart_phi()[trailing_quark_idx]);
            return true;
        },
        UNITY);

    //*****************************
    // - Matching gen quarks to reco jets
    //*****************************
    // Description: Match generator quarks to extra jets
    // vbs.cutflow.addCutToLastActiveCut("MatchToExtraJets",
    //         [&]()
    //         {
    //             return true;
    //         },
    //         UNITY);

    vbs.histograms.addHistogram("Mextrajj", 20, 0, 200, 
        [&]() 
        { 
            LV extra_jet_0 = vbs.tx.getBranchLazy<vector<LV>>("extra_jets_p4_min_dR_pair")[0];
            LV extra_jet_1 = vbs.tx.getBranchLazy<vector<LV>>("extra_jets_p4_min_dR_pair")[1];
            return (extra_jet_0 + extra_jet_1).M();
        } 
    );

    vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "TagTwoExtraJets");
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

        vbs.process("TagTwoExtraJets");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
