#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

class AnalysisConfig {

public:

    // TString that holds the input file list (comma separated)
    TString input_file_list_tstring;

    // TString that holds the name of the TTree to open for each input files
    TString input_tree_name;

    // Output TFile
    TFile* output_tfile;

    // Number of events to loop over
    int n_events;

    // Jobs to split (if this number is positive, then we will skip certain number of events)
    // If there are N events, and was asked to split 2 ways, then depending on job_index, it will run over first half or latter half
    int nsplit_jobs;

    // Job index (assuming nsplit_jobs is set, the job_index determine where to loop over)
    int job_index;

    // scale1fb of the sample
    float scale1fb;

    // Debug boolean
    bool debug;

    // TChain that holds the input TTree's
    TChain* events_tchain;

    // Custom Looper object to facilitate looping over many files
    RooUtil::Looper<Nano> looper;

    // Custom Cutflow framework
    RooUtil::Cutflow cutflow;

    // Custom Histograms object compatible with RooUtil::Cutflow framework
    RooUtil::Histograms histograms;

};

AnalysisConfig ana;

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

//********************************************************************************
//
// 1. Parsing options
//
//********************************************************************************

    // cxxopts is just a tool to parse argc, and argv easily

    // Grand option setting
    cxxopts::Options options("\n  $ doAnalysis",  "\n         **********************\n         *                    *\n         *       Looper       *\n         *                    *\n         **********************\n");

    // Read the options
    options.add_options()
        ("i,input"       , "Comma separated input file list OR if just a directory is provided it will glob all in the directory BUT must end with '/' for the path", cxxopts::value<std::string>())
        ("t,tree"        , "Name of the tree in the root file to open and loop over"                                             , cxxopts::value<std::string>())
        ("o,output"      , "Output file name"                                                                                    , cxxopts::value<std::string>())
        ("n,nevents"     , "N events to loop over"                                                                               , cxxopts::value<int>()->default_value("-1"))
        ("j,nsplit_jobs" , "Enable splitting jobs by N blocks (--job_index must be set)"                                         , cxxopts::value<int>())
        ("I,job_index"   , "job_index of split jobs (--nsplit_jobs must be set. index starts from 0. i.e. 0, 1, 2, 3, etc...)"   , cxxopts::value<int>())
        ("s,scale1fb"    , "pass scale1fb of the sample"                                                                         , cxxopts::value<float>())
        ("d,debug"       , "Run debug job. i.e. overrides output option to 'debug.root' and 'recreate's the file.")
        ("h,help"        , "Print help")
        ;

    auto result = options.parse(argc, argv);

    // NOTE: When an option was provided (e.g. -i or --input), then the result.count("<option name>") is more than 0
    // Therefore, the option can be parsed easily by asking the condition if (result.count("<option name>");
    // That's how the several options are parsed below

    //_______________________________________________________________________________
    // --help
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --input
    if (result.count("input"))
    {
        ana.input_file_list_tstring = result["input"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input list is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --tree
    if (result.count("tree"))
    {
        ana.input_tree_name = result["tree"].as<std::string>();
    }
    else
    {
        std::cout << options.help() << std::endl;
        std::cout << "ERROR: Input tree name is not provided! Check your arguments" << std::endl;
        exit(1);
    }

    //_______________________________________________________________________________
    // --scale1fb
    if (result.count("scale1fb"))
    {
        ana.scale1fb = result["scale1fb"].as<float>();
    }

    //_______________________________________________________________________________
    // --debug
    if (result.count("debug"))
    {
        ana.output_tfile = new TFile("debug.root", "recreate");
    }
    else
    {
        //_______________________________________________________________________________
        // --output
        if (result.count("output"))
        {
            ana.output_tfile = new TFile(result["output"].as<std::string>().c_str(), "create");
            if (not ana.output_tfile->IsOpen())
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: output already exists! provide new output name or delete old file. OUTPUTFILE=" << result["output"].as<std::string>() << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: Output file name is not provided! Check your arguments" << std::endl;
            exit(1);
        }
    }

    //_______________________________________________________________________________
    // --nevents
    ana.n_events = result["nevents"].as<int>();

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("nsplit_jobs"))
    {
        ana.nsplit_jobs = result["nsplit_jobs"].as<int>();
        if (ana.nsplit_jobs <= 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --nsplit_jobs" << ana.nsplit_jobs << " has zero or negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.nsplit_jobs = -1;
    }

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("job_index"))
    {
        ana.job_index = result["job_index"].as<int>();
        if (ana.job_index < 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index" << ana.job_index << " has negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        ana.job_index = -1;
    }


    // Sanity check for split jobs (if one is set the other must be set too)
    if (result.count("job_index") or result.count("nsplit_jobs"))
    {
        // If one is not provided then throw error
        if ( not (result.count("job_index") and result.count("nsplit_jobs")))
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index and --nsplit_jobs must be set at the same time!" << std::endl;
            exit(1);
        }
        // If it is set then check for sanity
        else
        {
            if (ana.job_index >= ana.nsplit_jobs)
            {
                std::cout << options.help() << std::endl;
                std::cout << "ERROR: --job_index >= --nsplit_jobs ! This does not make sense..." << std::endl;
                exit(1);
            }
        }
    }

    //
    // Printing out the option settings overview
    //
    std::cout <<  "=========================================================" << std::endl;
    std::cout <<  " Setting of the analysis job based on provided arguments " << std::endl;
    std::cout <<  "---------------------------------------------------------" << std::endl;
    std::cout <<  " ana.input_file_list_tstring: " << ana.input_file_list_tstring <<  std::endl;
    std::cout <<  " ana.output_tfile: " << ana.output_tfile->GetName() <<  std::endl;
    std::cout <<  " ana.n_events: " << ana.n_events <<  std::endl;
    std::cout <<  " ana.nsplit_jobs: " << ana.nsplit_jobs <<  std::endl;
    std::cout <<  " ana.job_index: " << ana.job_index <<  std::endl;
    std::cout <<  "=========================================================" << std::endl;

//********************************************************************************
//
// 2. Opening input baby files
//
//********************************************************************************

    // Create the TChain that holds the TTree's of the baby ntuples
    ana.events_tchain = RooUtil::FileUtil::createTChain(ana.input_tree_name, ana.input_file_list_tstring);

    // Create a Looper object to loop over input files
    // the "www" object is defined in the wwwtree.h/cc
    // This is an instance which helps read variables in the WWW baby TTree
    // It is a giant wrapper that facilitates reading TBranch values.
    // e.g. if there is a TBranch named "lep_pt" which is a std::vector<float> then, one can access the branch via
    //
    //    std::vector<float> lep_pt = www.lep_pt();
    //
    // and no need for "SetBranchAddress" and declaring variable shenanigans necessary
    // This is a standard thing SNT does pretty much every looper we use
    nt.SetYear(2017);
    ana.looper.init(ana.events_tchain, &nt, ana.n_events);

    // Set the cutflow object output file
    ana.cutflow.setTFile(ana.output_tfile);

    // Intermediate variables
    RooUtil::TTreeX tx("variable", "variable"); // new TTree
    tx.createBranch<vector<LV>>("good_leptons_p4");
    tx.createBranch<vector<int>>("good_leptons_pdgid");
    tx.createBranch<vector<int>>("good_leptons_tight");
    tx.createBranch<vector<float>>("good_leptons_pfRelIso03_all");
    tx.createBranch<vector<float>>("good_leptons_pfRelIso03_chg");
    tx.createBranch<vector<float>>("good_leptons_jetPtRelv2");
    tx.createBranch<vector<float>>("good_leptons_jetRelIso");
    tx.createBranch<vector<float>>("good_leptons_miniPFRelIso_all");
    tx.createBranch<vector<LV>>("good_jets_p4");
    tx.createBranch<vector<int>>("good_jets_loose_btagged");
    tx.createBranch<vector<int>>("good_jets_medium_btagged");
    tx.createBranch<vector<int>>("good_jets_tight_btagged");
    tx.createBranch<vector<float>>("good_jets_btag_score");
    tx.createBranch<vector<LV>>("higgs_jets_p4");
    tx.createBranch<vector<int>>("higgs_jets_loose_btagged");
    tx.createBranch<vector<int>>("higgs_jets_medium_btagged");
    tx.createBranch<vector<int>>("higgs_jets_tight_btagged");
    tx.createBranch<vector<float>>("higgs_jets_btag_score");
    tx.createBranch<vector<int>>("higgs_jets_good_jets_idx");
    tx.createBranch<vector<LV>>("vbs_jets_p4");
    tx.createBranch<int>("nbloose");
    tx.createBranch<int>("nbmedium");
    tx.createBranch<int>("nbtight");
    tx.createBranch<int>("ncenjet30");
    tx.createBranch<int>("njet30");

    // ======================================
    /* TODO: Factor this out to NanoCORE */
    // Load the b-tagging working points
    float loose_working_point;  
    float medium_working_point;
    float tight_working_point;
    if (nt.year() == 2016)
    {
        loose_working_point = 0.0614;
        medium_working_point = 0.3093;
        tight_working_point = 0.7221;
    }
    else if (nt.year() == 2017)
    {
        loose_working_point = 0.0521;
        medium_working_point = 0.3033;
        tight_working_point = 0.7489;
    }
    else if (nt.year() == 2018)
    {
        loose_working_point = 0.0494;
        medium_working_point = 0.2770;
        tight_working_point = 0.7264;
    }
    // ======================================

    // Weight
    ana.cutflow.addCut("Weight",
            [&]() { return 1/*set your cut here*/; },
            [&]()
            {
                float wgt = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0)) * ana.scale1fb;
                return wgt * 137.f;
            } );

    ana.cutflow.addCutToLastActiveCut("Preselection",
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
                return nel20 + nmu20 >= 2;
            },
            UNITY);

    ana.cutflow.addCutToLastActiveCut("SelectLeptons",
            [&]()
            {

                // Select muons
                for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
                {
                    if (SS::muonID(imu, SS::IDfakable, nt.year()))
                    {
                        tx.pushbackToBranch<LV>("good_leptons_p4", nt.Muon_p4()[imu]);
                        tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Muon_charge()[imu]) * 13);
                        tx.pushbackToBranch<int>("good_leptons_tight", SS::muonID(imu, SS::IDtight, nt.year()));
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Muon_pfRelIso03_all()[imu]);
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", -999);
                        tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Muon_jetPtRelv2()[imu]);
                        tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Muon_jetRelIso()[imu]);
                        tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Muon_miniPFRelIso_all()[imu]);
                    }
                }

                // Select electrons
                for (unsigned int iel = 0; iel < nt.Electron_pt().size(); ++iel)
                {
                    if (SS::electronID(iel, SS::IDfakable, nt.year()))
                    {
                        tx.pushbackToBranch<LV>("good_leptons_p4", nt.Electron_p4()[iel]);
                        tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Electron_charge()[iel]) * 11);
                        tx.pushbackToBranch<int>("good_leptons_tight", SS::electronID(iel, SS::IDtight, nt.year()) * (nt.Electron_pfRelIso03_all()[iel] < 0.05));
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Electron_pfRelIso03_all()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", nt.Electron_pfRelIso03_chg()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Electron_jetPtRelv2()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Electron_jetRelIso()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Electron_miniPFRelIso_all()[iel]);
                    }
                }

                tx.sortVecBranchesByPt(
                        /* name of the 4vector branch to use to pt sort by*/               "good_leptons_p4",
                        /* names of any associated vector<float> branches to sort along */ {"good_leptons_pfRelIso03_all", "good_leptons_pfRelIso03_chg", "good_leptons_jetPtRelv2", "good_leptons_jetRelIso", "good_leptons_miniPFRelIso_all"},
                        /* names of any associated vector<int>   branches to sort along */ {"good_leptons_pdgid", "good_leptons_tight"},
                        /* names of any associated vector<bool>  branches to sort along */ {}
                        );

                return true;

            },
            UNITY);

    ana.cutflow.getCut("SelectLeptons");
    ana.cutflow.addCutToLastActiveCut("SSPreselection",
            [&]()
            {

                // Select only two loose leptons
                if (not (tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() == 2))
                    return false;

                int ntight = 0;
                for (auto& istight : tx.getBranch<vector<int>>("good_leptons_tight"))
                {
                    if (istight)
                        ntight++;
                }

                // Select only two tight leptons
                if (not (ntight == 2))
                    return false;

                // Therefore I will only have two leptons in the good_leptons container
                const int& pdgid0 = tx.getBranch<vector<int>>("good_leptons_pdgid")[0];
                const int& pdgid1 = tx.getBranch<vector<int>>("good_leptons_pdgid")[1];

                // Require same sign
                if (not (pdgid0 * pdgid1 > 0))
                    return false;

                const float& pt0 = tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt();
                const float& pt1 = tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt();

                // Apply Pt selections
                if (abs(pdgid0) == 11 and abs(pdgid1) == 11)
                {
                    return ((pt0 > 25.) and (pt1 > 25.));
                }
                else if (abs(pdgid0) == 11 and abs(pdgid1) == 13)
                {
                    return ((pt0 > 25.) and (pt1 > 20.));
                }
                else if (abs(pdgid0) == 13 and abs(pdgid1) == 11)
                {
                    return ((pt0 > 25.) and (pt1 > 25.));
                }
                else if (abs(pdgid0) == 13 and abs(pdgid1) == 13)
                {
                    return ((pt0 > 25.) and (pt1 > 20.));
                }
                else
                {
                    // I should not be here
                    std::cout << "I should not be here!!!! " << std::endl;
                    return false;
                }
            },
            UNITY);

    ana.cutflow.addCutToLastActiveCut("SelectJets",
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
                    for (auto& lep_p4 : tx.getBranch<vector<LV>>("good_leptons_p4"))
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

                    // B-tagging is done down to 20 GeV
                    if (not (jet_p4.pt() > 20.))
                        continue;

                    bool is_loose_btagged = false;
                    bool is_medium_btagged = false;
                    bool is_tight_btagged = false;

                    // B-tagging is also done up to 2.5 in eta only
                    if (abs(jet_p4.eta()) < 2.5)
                    {
                        // Check if it passes btagging
                        is_loose_btagged = nt.Jet_btagDeepFlavB()[ijet] > loose_working_point;
                        is_medium_btagged = nt.Jet_btagDeepFlavB()[ijet] > medium_working_point;
                        is_tight_btagged = nt.Jet_btagDeepFlavB()[ijet] > tight_working_point;

                        // Count up the btagging
                        if (is_loose_btagged) nbloose++;
                        if (is_medium_btagged) nbmedium++;
                        if (is_tight_btagged) nbtight++;
                    }

                    tx.pushbackToBranch<LV>("good_jets_p4", jet_p4);
                    tx.pushbackToBranch<int>("good_jets_loose_btagged", is_loose_btagged);
                    tx.pushbackToBranch<int>("good_jets_medium_btagged", is_medium_btagged);
                    tx.pushbackToBranch<int>("good_jets_tight_btagged", is_tight_btagged);
                    tx.pushbackToBranch<float>("good_jets_btag_score", nt.Jet_btagDeepFlavB()[ijet]);

                    if (abs(jet_p4.eta()) < 3.0 and jet_p4.pt() > 30.)
                    {
                        ncenjet30 ++;
                    }
                    if (jet_p4.pt() > 30.)
                    {
                        njet30 ++;
                    }

                }

                tx.setBranch<int>("nbloose", nbloose);
                tx.setBranch<int>("nbmedium", nbmedium);
                tx.setBranch<int>("nbtight", nbtight);
                tx.setBranch<int>("ncenjet30", ncenjet30);
                tx.setBranch<int>("njet30", njet30);

                tx.sortVecBranchesByPt(
                        /* name of the 4vector branch to use to pt sort by*/               "good_jets_p4",
                        /* names of any associated vector<float> branches to sort along */ {"good_jets_btag_score"},
                        /* names of any associated vector<int>   branches to sort along */ {"good_jets_loose_btagged", "good_jets_medium_btagged", "good_jets_tight_btagged"},
                        /* names of any associated vector<bool>  branches to sort along */ {}
                        );

                return true;
            },
            UNITY);

    ana.cutflow.addCutToLastActiveCut("GeqTwoTightBtag", [&]() { return tx.getBranch<int>("nbtight") >= 2; }, /* TODO btag scale factor */ UNITY);

    ana.cutflow.addCutToLastActiveCut("SelectHiggsJets",
            [&]()
            {
                // get scores and indices pairs
                std::vector<std::pair<float, int>> btag_jets;
                for (unsigned int i = 0; i < tx.getBranch<vector<LV>>("good_jets_p4").size(); i++)
                {
                    const float& btag_score = tx.getBranch<vector<float>>("good_jets_btag_score")[i];
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

                tx.pushbackToBranch<LV>("higgs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[higgs_jet_0]);
                tx.pushbackToBranch<int>("higgs_jets_loose_btagged", tx.getBranch<vector<int>>("good_jets_loose_btagged")[higgs_jet_0]);
                tx.pushbackToBranch<int>("higgs_jets_medium_btagged", tx.getBranch<vector<int>>("good_jets_medium_btagged")[higgs_jet_0]);
                tx.pushbackToBranch<int>("higgs_jets_tight_btagged", tx.getBranch<vector<int>>("good_jets_tight_btagged")[higgs_jet_0]);
                tx.pushbackToBranch<float>("higgs_jets_btag_score", tx.getBranch<vector<float>>("good_jets_btag_score")[higgs_jet_0]);
                tx.pushbackToBranch<int>("higgs_jets_good_jets_idx", higgs_jet_0);

                tx.pushbackToBranch<LV>("higgs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[higgs_jet_1]);
                tx.pushbackToBranch<int>("higgs_jets_loose_btagged", tx.getBranch<vector<int>>("good_jets_loose_btagged")[higgs_jet_1]);
                tx.pushbackToBranch<int>("higgs_jets_medium_btagged", tx.getBranch<vector<int>>("good_jets_medium_btagged")[higgs_jet_1]);
                tx.pushbackToBranch<int>("higgs_jets_tight_btagged", tx.getBranch<vector<int>>("good_jets_tight_btagged")[higgs_jet_1]);
                tx.pushbackToBranch<float>("higgs_jets_btag_score", tx.getBranch<vector<float>>("good_jets_btag_score")[higgs_jet_1]);
                tx.pushbackToBranch<int>("higgs_jets_good_jets_idx", higgs_jet_1);
                return true;

            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("SelectVBSJets",
            [&]()
            {

                // higgs jet indices
                const int& higgs_jet_0 = tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[0];
                const int& higgs_jet_1 = tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[1];

                // Select VBS candidates
                std::vector<int> vbs_jet_cands_idxs;
                for (unsigned int i = 0; i < tx.getBranch<vector<LV>>("good_jets_p4").size(); i++)
                {
                    if ((int) i != higgs_jet_0 and (int) i != higgs_jet_1)
                    {
                        if (tx.getBranch<vector<LV>>("good_jets_p4")[i].pt() > 30.)
                        {
                            vbs_jet_cands_idxs.push_back(i);
                        }
                    }
                }

                if (vbs_jet_cands_idxs.size() < 2)
                    return false;

                if (vbs_jet_cands_idxs.size() == 2)
                {
                    tx.pushbackToBranch<LV>("vbs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_cands_idxs[0]]);
                    tx.pushbackToBranch<LV>("vbs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_cands_idxs[1]]);
                    return true;
                }

                // Otherwise, I have 3 or more vbs candidate jets
                std::vector<std::pair<float, int>> vbs_pos_eta_jets;
                std::vector<std::pair<float, int>> vbs_neg_eta_jets;
                for (unsigned int ijet = 0; ijet < vbs_jet_cands_idxs.size(); ijet++)
                {
                    const LV& jet = tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_cands_idxs[ijet]];
                    const float& P = tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_cands_idxs[ijet]].P();
                    if (jet.eta() >= 0)
                    {
                        vbs_pos_eta_jets.push_back(std::make_pair(P, ijet));
                    }
                    if (jet.eta() < 0)
                    {
                        vbs_neg_eta_jets.push_back(std::make_pair(P, ijet));
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

                tx.pushbackToBranch<LV>("vbs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_idx_0]);
                tx.pushbackToBranch<LV>("vbs_jets_p4", tx.getBranch<vector<LV>>("good_jets_p4")[vbs_jet_idx_1]);

                return true;

            },
            UNITY);

    ana.cutflow.addCutToLastActiveCut("SignalRegion", UNITY, UNITY);

    ana.cutflow.getCut("Preselection");
    ana.cutflow.addCutToLastActiveCut("OS", [&]() { return 1; }, UNITY);

    ana.cutflow.printCuts();

    ana.histograms.addHistogram("NCenJet30", 9, 0, 9, [&]() { return tx.getBranchLazy<int>("ncenjet30"); } );
    ana.histograms.addHistogram("NJet30", 9, 0, 9, [&]() { return tx.getBranchLazy<int>("njet30"); } );

    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "SelectVBSJets");
    ana.cutflow.bookCutflows();

    // Looping input file
    while (ana.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        tx.clear();

        //Do what you need to do in for each event here
        //To save use the following function
        ana.cutflow.fill();

        // tx.fill();
    }

    // Writing output file
    ana.cutflow.saveOutput();

    tx.write();

    // The below can be sometimes crucial
    delete ana.output_tfile;
}
