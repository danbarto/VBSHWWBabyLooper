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
    vbs.tx.createBranch<vector<int>>("good_jets_is_higgs_jet");
    vbs.tx.createBranch<vector<int>>("good_jets_is_vbs_jet");
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

    vbs.cutflow.printCuts();

    vbs.cutflow.getCut("Preselection");

    //*****************************
    // - Fill generator branches
    //*****************************
    // Description: 'benign' cut that only serves to fill some generator branches
    vbs.cutflow.addCutToLastActiveCut("FillGenBranches",
            [&]()
            {
                int quark_A_idx = -1;
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
                            else if (quark_B_idx < 0) { quark_B_idx = idx; }
                            else 
                            {
                                std::cout << "wtf?? more than two quarks matched to a W" << std::endl;
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

    // vbs.histograms.addHistogram("NCenJet30", 9, 0, 9, [&]() { return vbs.tx.getBranchLazy<int>("ncenjet30"); } );
    // vbs.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return vbs.tx.getBranchLazy<int>("njet30"); } );

    // vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "TagVBSJets");
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

        vbs.process("FillGenBranches");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    vbs.writeEventList("FillGenBranches");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
