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

    vbs.tx.createBranch<float>("mllZ");
    vbs.tx.createBranch<int>("nsfos");

    vbs.initSRCutflow();

    vbs.cutflow.removeCut("SSPreselection");

    vbs.cutflow.getCut("ObjectsSelection");
 
    //*****************************
    // - Same Sign Preselection
    //*****************************
    // Description: Pass events only when we have:
    //              - at least one light lepton (no ditau-had)
    //              - two loose
    //              - two tight
    //              - they are same sign
    //              - all pass pt > 25
    vbs.cutflow.addCutToLastActiveCut("TLPreselection",
        [&]()
        {

            // Only three loose leptons
            if (not (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() == 3))
                return false;

            int ntight = 0;
            for (auto& istight : vbs.tx.getBranch<vector<int>>("good_leptons_tight"))
            {
                if (istight)
                    ntight++;
            }

            // Select only two tight leptons
            if (not (ntight == 3))
                return false;

            float pt0 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0].pt();
            float pt1 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1].pt();
            float pt2 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[2].pt();

            float mllZ = -999;
            for (unsigned int ilep = 0; ilep < vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size(); ++ilep)
            {
                for (unsigned int jlep = ilep + 1; jlep < vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size(); ++jlep)
                {
                    const int& pdgid0 = vbs.tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[ilep];
                    const int& pdgid1 = vbs.tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[jlep];
                    if (pdgid0 == -pdgid1)
                    {
                        const LV& lep0 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[ilep];
                        const LV& lep1 = vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[jlep];
                        float tmpmll = (lep0 + lep1).mass();
                        if (abs(mllZ - 91.1876) > abs(tmpmll - 91.1876))
                        {
                            mllZ = tmpmll;
                        }
                    }
                }
            }
            vbs.tx.setBranch<float>("mllZ", mllZ);
            int nsfos = 0;
            for (unsigned int ilep = 0; ilep < vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size(); ++ilep)
            {
                for (unsigned int jlep = ilep + 1; jlep < vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4").size(); ++jlep)
                {
                    const int& lep_i = vbs.tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[ilep];
                    const int& lep_j = vbs.tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[jlep];
                    if (lep_i == -lep_j)
                        nsfos++;
                }
            }
            vbs.tx.setBranch<int>("nsfos", nsfos);

            if (not (pt0 > 40. and pt1 > 40. and pt2 > 10.))
                return false;

            const int& trig_ee = vbs.tx.getBranch<int>("trig_ee");
            const int& trig_em = vbs.tx.getBranch<int>("trig_em");
            const int& trig_mm = vbs.tx.getBranch<int>("trig_mm");
            const int& pass_duplicate_ee_em_mm = vbs.tx.getBranch<int>("pass_duplicate_ee_em_mm");
            bool pass_trigger = trig_ee or trig_em or trig_mm;
            bool duplicate = nt.isData() ? pass_duplicate_ee_em_mm : 1.;
            return (pass_trigger and duplicate);

        },
        UNITY);

    //*****************************
    // - TLOnZ (ttZ)
    //*****************************
    // Description: Select Good Reco Jets
    vbs.cutflow.getCut("TLPreselection");
    vbs.cutflow.addCutToLastActiveCut("TLOnZ", [&]() { return vbs.tx.getBranch<int>("nsfos") > 0 and abs(vbs.tx.getBranchLazy<float>("mllZ") - 91.1876) < 15.; }, UNITY);
    vbs.cutflow.getCut("TLOnZ");
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbTightGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbTightGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 2; }, UNITY);
    vbs.cutflow.getCut("TLOnZ");
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbMediumGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbMediumGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 2; }, UNITY);
    vbs.cutflow.getCut("TLOnZ");
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbLooseGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOnZNbLooseGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 2; }, UNITY);

    //*****************************
    // - TLOffZ
    //*****************************
    // Description: Select Good Reco Jets
    vbs.cutflow.getCut("TLPreselection");
    vbs.cutflow.addCutToLastActiveCut("TLOffZ", [&]() { return vbs.tx.getBranch<int>("nsfos") > 0 and abs(vbs.tx.getBranchLazy<float>("mllZ") - 91.1876) > 15.; }, UNITY);
    vbs.cutflow.getCut("TLOffZ");
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbTightGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbTightGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 2; }, UNITY);
    vbs.cutflow.getCut("TLOffZ");
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbMediumGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbMediumGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 2; }, UNITY);
    vbs.cutflow.getCut("TLOffZ");
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbLooseGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TLOffZNbLooseGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 2; }, UNITY);

    //*****************************
    // - TL0SFOS
    //*****************************
    // Description: Select Good Reco Jets
    vbs.cutflow.getCut("TLPreselection");
    vbs.cutflow.addCutToLastActiveCut("TL0SFOS", [&]() { return vbs.tx.getBranch<int>("nsfos") == 0; }, UNITY);
    vbs.cutflow.getCut("TL0SFOS");
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbTightGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbTightGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbtight") >= 2; }, UNITY);
    vbs.cutflow.getCut("TL0SFOS");
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbMediumGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbMediumGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbmedium") >= 2; }, UNITY);
    vbs.cutflow.getCut("TL0SFOS");
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbLooseGeq1", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 1; }, UNITY);
    vbs.cutflow.addCutToLastActiveCut("TL0SFOSNbLooseGeq2", [&]() { return vbs.tx.getBranchLazy<int>("nbloose") >= 2; }, UNITY);

    vbs.cutflow.printCuts();

    RooUtil::Histograms tlhistograms;
    tlhistograms.addHistogram("TLLepPt0", 180, 0, 180, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0].pt(); } );
    tlhistograms.addHistogram("TLLepPt1", 180, 0, 180, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1].pt(); } );
    tlhistograms.addHistogram("TLLepPt2", 180, 0, 180, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[2].pt(); } );
    tlhistograms.addHistogram("TLLepEta0", 180, -3, 3, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0].eta(); } );
    tlhistograms.addHistogram("TLLepEta1", 180, -3, 3, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1].eta(); } );
    tlhistograms.addHistogram("TLLepEta2", 180, -3, 3, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[2].eta(); } );
    tlhistograms.addHistogram("TLMLL01", 180, 0, 450, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0]+vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1]).mass(); } );
    tlhistograms.addHistogram("TLMLL12", 180, 0, 350, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1]+vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[2]).mass(); } );
    tlhistograms.addHistogram("TLMLL02", 180, 0, 400, [&]() { return (vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0]+vbs.tx.getBranchLazy<vector<LV>>("good_leptons_p4")[2]).mass(); } );
    tlhistograms.addHistogram("TLNBLoose", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbloose"); } );
    tlhistograms.addHistogram("TLNBMedium", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbmedium"); } );
    tlhistograms.addHistogram("TLNBTight", 5, 0, 5, [&]() { return vbs.tx.getBranchLazy<int>("nbtight"); } );
    tlhistograms.addHistogram("TLNJets", 10, 0, 10, [&]() { return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); } );
    tlhistograms.addHistogram("TLJetPt0", 180, 0, 180, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 0) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[0].pt(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetPt1", 180, 0, 180, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 1) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[1].pt(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetPt2", 180, 0, 180, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 2) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[2].pt(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetPt3", 180, 0, 180, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 3) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[3].pt(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetPt4", 180, 0, 180, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 4) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[4].pt(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetEta0", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 0) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[0].eta(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetEta1", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 1) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[1].eta(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetEta2", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 2) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[2].eta(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetEta3", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 3) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[3].eta(); else return -999.f; } );
    tlhistograms.addHistogram("TLJetEta4", 180, -5, 5, [&]() { if (vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4").size() > 4) return vbs.tx.getBranchLazy<vector<LV>>("good_jets_p4")[4].eta(); else return -999.f; } );
    tlhistograms.addHistogram("TLMllZ", 180, 0, 180, [&]() { return vbs.tx.getBranch<float>("mllZ"); } );

    vbs.cutflow.bookHistogramsForCutAndBelow(tlhistograms, "TLPreselection");
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

        vbs.process("TLPreselection");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    vbs.writeEventList("TLPreselection");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
