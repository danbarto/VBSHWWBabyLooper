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
    nt.SetYear(2017); // Hardcoded since we're running on 2017 MC only
    ana.looper.init(ana.events_tchain, &nt, ana.n_events);

//********************************************************************************
//
// Interlude... notes on RooUtil framework
//
//********************************************************************************

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Cutflow object cut tree formation
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Cutflow object facilitates creating a tree structure of cuts
    //
    // To add cuts to each node of the tree with cuts defined, use "addCut" or "addCutToLastActiveCut"
    // The "addCut" or "addCutToLastActiveCut" accepts three argument, <name>, and two lambda's that define the cut selection, and the weight to apply to that cut stage
    //
    // e.g. To create following cut-tree structure one does
    //
    //             (Root) <--- Always exists as soon as RooUtil::Cutflow object is created. But this is basically hidden underneath and users do not have to care
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    //
    //   code:
    //
    //      // Create the object (Root node is created as soon as the instance is created)
    //      RooUtil::Cutflow cutflow;
    //
    //      cutflow.addCut("CutWeight"                 , <lambda> , <lambda>); // CutWeight is added below "Root"-node
    //      cutflow.addCutToLastActiveCut("CutPresel1" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I just added that. So "CutPresel1" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel1"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel1" since I just added that. So "CutSel1" is added below "CutPresel1"
    //
    //      cutflow.getCut("CutWeight"); // By "getting" the cut object, this makes the "CutWeight" the last "active" cut.
    //      cutflow.addCutToLastActiveCut("CutPresel2" , <lambda> , <lambda>); // The last "active" cut is "CutWeight" since I "getCut" on it. So "CutPresel2" is added below "CutWeight"
    //      cutflow.addCutToLastActiveCut("CutSel2"    , <lambda> , <lambda>); // The last "active" cut is "CutPresel2" since I just added that. So "CutSel2" is added below "CutPresel1"
    //
    // (Side note: "UNITY" lambda is defined in the framework to just return 1. This so that use don't have to type [&]() {return 1;} so many times.)
    //
    // Once the cutflow is formed, create cutflow histograms can be created by calling RooUtil::Cutflow::bookCutflows())
    // This function looks through the terminating nodes of the tree structured cut tree. and creates a histogram that will fill the yields
    // For the example above, there are two terminationg nodes, "CutSel1", and "CutSel2"
    // So in this case Root::Cutflow::bookCutflows() will create two histograms. (Actually four histograms.)
    //
    //  - TH1F* type object :  CutSel1_cutflow (4 bins, with first bin labeled "Root", second bin labeled "CutWeight", third bin labeled "CutPreSel1", fourth bin labeled "CutSel1")
    //  - TH1F* type object :  CutSel2_cutflow (...)
    //  - TH1F* type object :  CutSel1_rawcutflow (...)
    //  - TH1F* type object :  CutSel2_rawcutflow (...)
    //                                ^
    //                                |
    // NOTE: There is only one underscore "_" between <CutName>_cutflow or <CutName>_rawcutflow
    //
    // And later in the loop when RooUtil::Cutflow::fill() function is called, the tree structure will be traversed through and the appropriate yields will be filled into the histograms
    //
    // After running the loop check for the histograms in the output root file

    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    // Quick tutorial on RooUtil::Histograms object
    // ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
    //
    // The RooUtil::Histograms object facilitates book keeping histogram definitions
    // And in conjunction with RooUtil::Cutflow object, one can book same histograms across different cut stages easily without copy pasting codes many times by hand.
    //
    // The histogram addition happens in two steps.
    // 1. Defining histograms
    // 2. Booking histograms to cuts
    //
    // Histograms are defined via following functions
    //
    //      RooUtil::Histograms::addHistogram       : Typical 1D histogram (TH1F*) "Fill()" called once per event
    //      RooUtil::Histograms::addVecHistogram    : Typical 1D histogram (TH1F*) "Fill()" called multiple times per event
    //      RooUtil::Histograms::add2DHistogram     : Typical 2D histogram (TH2F*) "Fill()" called once per event
    //      RooUtil::Histograms::add2DVecHistogram  : Typical 2D histogram (TH2F*) "Fill()" called multiple times per event
    // e.g.
    //
    //    RooUtil::Histograms histograms;
    //    histograms.addHistogram   ("MllSS"    , 180 , 0. , 300. , [&]() { return www.MllSS()  ; }); // The lambda returns float
    //    histograms.addVecHistogram("AllLepPt" , 180 , 0. , 300. , [&]() { return www.lep_pt() ; }); // The lambda returns vector<float>
    //
    // The addVecHistogram will have lambda to return vector<float> and it will loop over the values and call TH1F::Fill() for each item
    //
    // To book histograms to cuts one uses
    //
    //      RooUtil::Cutflow::bookHistogramsForCut()
    //      RooUtil::Cutflow::bookHistogramsForCutAndBelow()
    //      RooUtil::Cutflow::bookHistogramsForCutAndAbove()
    //      RooUtil::Cutflow::bookHistogramsForEndCuts()
    //
    // e.g. Given a tree like the following, we can book histograms to various cuts as we want
    //
    //              Root
    //                |
    //            CutWeight
    //            |       |
    //     CutPreSel1    CutPreSel2
    //       |                  |
    //     CutSel1           CutSel2
    //
    // For example,
    //
    //    1. book a set of histograms to one cut:
    //
    //       cutflow.bookHistogramsForCut(histograms, "CutPreSel2")
    //
    //    2. book a set of histograms to a cut and below
    //
    //       cutflow.bookHistogramsForCutAndBelow(histograms, "CutWeight") // will book a set of histograms to CutWeight, CutPreSel1, CutPreSel2, CutSel1, and CutSel2
    //
    //    3. book a set of histograms to a cut and above (... useless...?)
    //
    //       cutflow.bookHistogramsForCutAndAbove(histograms, "CutPreSel2") // will book a set of histograms to CutPreSel2, CutWeight (nothing happens to Root node)
    //
    //    4. book a set of histograms to a terminating nodes
    //
    //       cutflow.bookHistogramsForEndCuts(histograms) // will book a set of histograms to CutSel1 and CutSel2
    //
    // The naming convention of the booked histograms are as follows
    //
    //   cutflow.bookHistogramsForCut(histograms, "CutSel1");
    //
    //  - TH1F* type object : CutSel1__MllSS;
    //  - TH1F* type object : CutSel1__AllLepPt;
    //                               ^^
    //                               ||
    // NOTE: There are two underscores "__" between <CutName>__<HistogramName>
    //
    // And later in the loop when RooUtil::CutName::fill() function is called, the tree structure will be traversed through and the appropriate histograms will be filled with appropriate variables
    // After running the loop check for the histograms in the output root file

    // Set the cutflow object output file
    ana.cutflow.setTFile(ana.output_tfile);

    ana.cutflow.addCut("Weight", [&]() { return 1/*set your cut here*/; },
            [&]()
            {
                float wgt = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0)) * ana.scale1fb;
                return wgt * 137.f;
            } );

    // Variables
    RooUtil::TTreeX tx("variable", "variable");
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
    tx.createBranch<int>("nbloose");
    tx.createBranch<int>("nbmedium");
    tx.createBranch<int>("nbtight");
    tx.createBranch<float>("maxmjj");
    tx.createBranch<LV>("vbf_jets_maxmjj_0");
    tx.createBranch<LV>("vbf_jets_maxmjj_1");
    tx.createBranch<int>("vbf_jets_maxmjj_0_idx");
    tx.createBranch<int>("vbf_jets_maxmjj_1_idx");
    tx.createBranch<float>("mbb_maxmjj");
    tx.createBranch<float>("drbb_maxmjj");
    tx.createBranch<LV>("hbb_jets_maxmjj_0");
    tx.createBranch<LV>("hbb_jets_maxmjj_1");
    tx.createBranch<float>("mbb_mindr");
    tx.createBranch<float>("drbb_mindr");
    tx.createBranch<LV>("hbb_mindr_jets_maxmjj_0");
    tx.createBranch<LV>("hbb_mindr_jets_maxmjj_1");
    tx.createBranch<float>("minmlb");
    tx.createBranch<float>("summlb");
    tx.createBranch<float>("mindrlb");
    tx.createBranch<float>("sumdrlb");
    tx.createBranch<float>("ht");
    tx.createBranch<vector<LV>>("extra_jets_p4");
    tx.createBranch<vector<LV>>("central_jets_p4");
    tx.createBranch<float>("cjv_pt0");
    tx.createBranch<float>("cjv_pt1");
    tx.createBranch<float>("cjv_pt2");
    ana.cutflow.addCutToLastActiveCut("SelectObjects",
            [&]()
            {
                // Initialize
                tx.clear();

                // Select muons
                for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
                {
                    if (SS::muonID(imu, SS::IDfakable, 2017))
                    {
                        tx.pushbackToBranch<LV>("good_leptons_p4", nt.Muon_p4()[imu]);
                        tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Muon_charge()[imu]) * 13);
                        tx.pushbackToBranch<int>("good_leptons_tight", SS::muonID(imu, SS::IDtight, 2017));
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
                    if (SS::electronID(iel, SS::IDfakable, 2017))
                    {
                        tx.pushbackToBranch<LV>("good_leptons_p4", nt.Electron_p4()[iel]);
                        tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Electron_charge()[iel]) * 11);
                        tx.pushbackToBranch<int>("good_leptons_tight", SS::electronID(iel, SS::IDtight, 2017) * (nt.Electron_pfRelIso03_all()[iel] < 0.05));
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Electron_pfRelIso03_all()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", nt.Electron_pfRelIso03_chg()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Electron_jetPtRelv2()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Electron_jetRelIso()[iel]);
                        tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Electron_miniPFRelIso_all()[iel]);
                    }
                }

                tx.sortVecBranchesByPt(
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

                    if (not (jet_p4.pt() > 30.))
                        continue;

                    // if (not (fabs(jet_p4.eta()) < 5.0))
                    //     continue;

                    tx.pushbackToBranch<LV>("good_jets_p4", jet_p4);
                    bool is_loose_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.0521;
                    bool is_medium_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.3033;
                    bool is_tight_btagged = nt.Jet_btagDeepFlavB()[ijet] > 0.7489;

                    if (is_loose_btagged) nbloose++;
                    if (is_medium_btagged) nbmedium++;
                    if (is_tight_btagged) nbtight++;

                    tx.pushbackToBranch<int>("good_jets_loose_btagged", is_loose_btagged);
                    tx.pushbackToBranch<int>("good_jets_medium_btagged", is_medium_btagged);
                    tx.pushbackToBranch<int>("good_jets_tight_btagged", is_tight_btagged);

                    ht += jet_p4.pt();

                }

                tx.setBranch<int>("nbloose", nbloose);
                tx.setBranch<int>("nbmedium", nbmedium);
                tx.setBranch<int>("nbtight", nbtight);
                tx.setBranch<float>("ht", ht);

                tx.sortVecBranchesByPt(
                        /* name of the 4vector branch to use to pt sort by*/               "good_jets_p4",
                        /* names of any associated vector<float> branches to sort along */ {},
                        /* names of any associated vector<int>   branches to sort along */ {"good_jets_loose_btagged", "good_jets_medium_btagged", "good_jets_tight_btagged"},
                        /* names of any associated vector<bool>  branches to sort along */ {}
                        );

                float max_mjj = -999;
                for (unsigned int ijet = 0; ijet < tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++ijet)
                {
                    for (unsigned int jjet = ijet + 1; jjet < tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++jjet)
                    {
                        if (ijet == jjet)
                            continue;

                        const LV& ijet_p4 = tx.getBranch<vector<LV>>("good_jets_p4")[ijet];
                        const LV& jjet_p4 = tx.getBranch<vector<LV>>("good_jets_p4")[jjet];

                        float tmp_mjj = (ijet_p4 + jjet_p4).mass();

                        if (max_mjj < tmp_mjj)
                        {
                            max_mjj = tmp_mjj;
                            tx.setBranch<float>("maxmjj", max_mjj);
                            tx.setBranch<LV>("vbf_jets_maxmjj_0", ijet_p4);
                            tx.setBranch<LV>("vbf_jets_maxmjj_1", jjet_p4);
                            tx.setBranch<int>("vbf_jets_maxmjj_0_idx", ijet);
                            tx.setBranch<int>("vbf_jets_maxmjj_1_idx", jjet);
                        }
                    }
                }

                tx.fill();

                if (not (tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt() > 25. and tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt() > 20.))
                    return false;

                return true;
            },
            UNITY);

    ana.cutflow.addCutToLastActiveCut("GeqFourJets",
            [&]()
            {
                return tx.getBranchLazy<vector<LV>>("good_jets_p4").size() >= 4;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("SelectHiggs",
            [&]()
            {
                float min_dr = 999;
                for (unsigned int ijet = 0; ijet < tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++ijet)
                {
                    if (ijet == (unsigned int) tx.getBranch<int>("vbf_jets_maxmjj_0_idx")) continue;
                    if (ijet == (unsigned int) tx.getBranch<int>("vbf_jets_maxmjj_1_idx")) continue;
                    if (not tx.isBranchSet<LV>("hbb_jets_maxmjj_0"))
                        tx.setBranch<LV>("hbb_jets_maxmjj_0", tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    else if (not tx.isBranchSet<LV>("hbb_jets_maxmjj_1"))
                        tx.setBranch<LV>("hbb_jets_maxmjj_1", tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    else
                        tx.pushbackToBranch<LV>("extra_jets_p4", tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                    for (unsigned int jjet = ijet + 1; jjet < tx.getBranchLazy<vector<LV>>("good_jets_p4").size(); ++jjet)
                    {
                        if (jjet == (unsigned int) tx.getBranch<int>("vbf_jets_maxmjj_0_idx")) continue;
                        if (jjet == (unsigned int) tx.getBranch<int>("vbf_jets_maxmjj_1_idx")) continue;
                        float tmp_dr = RooUtil::Calc::DeltaR(tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet], tx.getBranchLazy<vector<LV>>("good_jets_p4")[jjet]);
                        if (tmp_dr < min_dr)
                        {
                            tmp_dr = min_dr;
                            tx.setBranch<LV>("hbb_mindr_jets_maxmjj_0", tx.getBranchLazy<vector<LV>>("good_jets_p4")[ijet]);
                            tx.setBranch<LV>("hbb_mindr_jets_maxmjj_1", tx.getBranchLazy<vector<LV>>("good_jets_p4")[jjet]);
                        }
                    }
                }
                tx.setBranch<float>("mbb_maxmjj", (tx.getBranchLazy<LV>("hbb_jets_maxmjj_0") + tx.getBranchLazy<LV>("hbb_jets_maxmjj_1")).mass());
                tx.setBranch<float>("drbb_maxmjj", RooUtil::Calc::DeltaR(tx.getBranchLazy<LV>("hbb_jets_maxmjj_0"), tx.getBranchLazy<LV>("hbb_jets_maxmjj_1")));
                tx.setBranch<float>("mbb_mindr", (tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_0") + tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_1")).mass());
                tx.setBranch<float>("drbb_mindr", RooUtil::Calc::DeltaR(tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_0"), tx.getBranchLazy<LV>("hbb_mindr_jets_maxmjj_1")));

                float mlb00 = (tx.getBranch<vector<LV>>("good_leptons_p4")[0] + tx.getBranch<LV>("hbb_jets_maxmjj_0")).mass();
                float mlb10 = (tx.getBranch<vector<LV>>("good_leptons_p4")[1] + tx.getBranch<LV>("hbb_jets_maxmjj_0")).mass();
                float mlb01 = (tx.getBranch<vector<LV>>("good_leptons_p4")[0] + tx.getBranch<LV>("hbb_jets_maxmjj_1")).mass();
                float mlb11 = (tx.getBranch<vector<LV>>("good_leptons_p4")[1] + tx.getBranch<LV>("hbb_jets_maxmjj_1")).mass();
                float summlb = mlb00 + mlb10 + mlb01 + mlb11;
                float minmlb = min(mlb00, min(mlb10, min(mlb01, mlb11)));
                tx.setBranch<float>("minmlb", minmlb);
                tx.setBranch<float>("summlb", summlb);
                float drlb00 = RooUtil::Calc::DeltaR(tx.getBranch<vector<LV>>("good_leptons_p4")[0], tx.getBranch<LV>("hbb_jets_maxmjj_0"));
                float drlb10 = RooUtil::Calc::DeltaR(tx.getBranch<vector<LV>>("good_leptons_p4")[1], tx.getBranch<LV>("hbb_jets_maxmjj_0"));
                float drlb01 = RooUtil::Calc::DeltaR(tx.getBranch<vector<LV>>("good_leptons_p4")[0], tx.getBranch<LV>("hbb_jets_maxmjj_1"));
                float drlb11 = RooUtil::Calc::DeltaR(tx.getBranch<vector<LV>>("good_leptons_p4")[1], tx.getBranch<LV>("hbb_jets_maxmjj_1"));
                float sumdrlb = drlb00 + drlb10 + drlb01 + drlb11;
                float mindrlb = min(drlb00, min(drlb10, min(drlb01, drlb11)));
                tx.setBranch<float>("mindrlb", mindrlb);
                tx.setBranch<float>("sumdrlb", sumdrlb);

                // CJV
                float vbfeta0 = tx.getBranch<LV>("vbf_jets_maxmjj_0").eta();
                float vbfeta1 = tx.getBranch<LV>("vbf_jets_maxmjj_1").eta();
                for (auto& extra_jet_p4 : tx.getBranchLazy<vector<LV>>("extra_jets_p4"))
                {
                    float extraeta = extra_jet_p4.eta();
                    if ((vbfeta0 > extraeta and extraeta > vbfeta1) or (vbfeta1 > extraeta and extraeta > vbfeta0))
                    {
                        tx.pushbackToBranch<LV>("central_jets_p4", extra_jet_p4);
                    }
                }
                if (tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 0)
                    tx.setBranch<float>("cjv_pt0", tx.getBranch<vector<LV>>("central_jets_p4")[0].pt());
                if (tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 1)
                    tx.setBranch<float>("cjv_pt1", tx.getBranch<vector<LV>>("central_jets_p4")[1].pt());
                if (tx.getBranchLazy<vector<LV>>("central_jets_p4").size() > 2)
                    tx.setBranch<float>("cjv_pt2", tx.getBranch<vector<LV>>("central_jets_p4")[2].pt());
                return true;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("VBFJetsEtaCut",
            [&]()
            {
                return fabs(tx.getBranchLazy<LV>("vbf_jets_maxmjj_0").eta()) > 2.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("TwoTightLeptons",
            [&]()
            {
                int ntight = 0;
                for (auto& is_tight : tx.getBranch<vector<int>>("good_leptons_tight"))
                    if (is_tight)
                        ntight++;
                return ntight == 2;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("LeptonPt",
            [&]()
            {
                if (not (tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt() > 50. and tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt() > 25.))
                    return false;
                return true;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("NbMed",
            [&]()
            {
                return tx.getBranch<int>("nbmedium") > 1.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("NjetCuts",
            [&]()
            {
                return tx.getBranch<vector<LV>>("good_jets_p4").size() < 6.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("HiggsCuts",
            [&]()
            {
                return tx.getBranch<float>("mbb_maxmjj") < 150.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("Mjj500",
            [&]()
            {
                return tx.getBranch<float>("maxmjj") > 500.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("Mjj1000",
            [&]()
            {
                return tx.getBranch<float>("maxmjj") > 1000.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("Mjj1500",
            [&]()
            {
                return tx.getBranch<float>("maxmjj") > 1500.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("SignalCuts",
            [&]()
            {
                return tx.getBranch<float>("maxmjj") > 2000.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("CJVCuts",
            [&]()
            {
                return tx.getBranchLazy<float>("cjv_pt0") < 30.;
            }, UNITY);

    ana.cutflow.addCutToLastActiveCut("SignalRegion", UNITY, UNITY);


    // Histogram utility object that is used to define the histograms
    ana.histograms.addHistogram("Nlep", 6, 0, 6, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4").size(); } );
    ana.histograms.addHistogram("lepPt0", 180, 0, 450, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[0].pt(); } );
    ana.histograms.addHistogram("lepPt1", 180, 0, 250, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[1].pt(); } );
    ana.histograms.addHistogram("lepEta0", 180, -5, 5, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[0].eta(); } );
    ana.histograms.addHistogram("lepEta1", 180, -5, 5, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[1].eta(); } );
    ana.histograms.addHistogram("lepPhi0", 180, -3.1416, 3.1416, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[0].phi(); } );
    ana.histograms.addHistogram("lepPhi1", 180, -3.1416, 3.1416, [&]() { return tx.getBranch<vector<LV>>("good_leptons_p4")[1].phi(); } );
    ana.histograms.addHistogram("leppfIsoRel03All0", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_pfRelIso03_all")[0]; } );
    ana.histograms.addHistogram("leppfIsoRel03All1", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_pfRelIso03_all")[1]; } );
    ana.histograms.addHistogram("leppfIsoRel03Chg0", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_pfRelIso03_chg")[0]; } );
    ana.histograms.addHistogram("leppfIsoRel03Chg1", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_pfRelIso03_chg")[1]; } );
    ana.histograms.addHistogram("leppfIsoRelminiAll0", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_miniPFRelIso_all")[0]; } );
    ana.histograms.addHistogram("leppfIsoRelminiAll1", 180, -0.02, 0.2, [&]() { return tx.getBranch<vector<float>>("good_leptons_miniPFRelIso_all")[1]; } );
    ana.histograms.addHistogram("cjv_pt0", 180, 0, 100, [&]() { return tx.getBranchLazy<float>("cjv_pt0"); } );
    ana.histograms.addHistogram("cjv_pt1", 180, 0, 100, [&]() { return tx.getBranchLazy<float>("cjv_pt1"); } );
    ana.histograms.addHistogram("cjv_pt2", 180, 0, 100, [&]() { return tx.getBranchLazy<float>("cjv_pt2"); } );
    ana.histograms.addHistogram("ht", 180, 0, 1500, [&]() { return tx.getBranch<float>("ht"); } );
    ana.histograms.addHistogram("mll", 180, 0, 650, [&]() { return (tx.getBranch<vector<LV>>("good_leptons_p4")[0]+tx.getBranch<vector<LV>>("good_leptons_p4")[1]).mass(); } );
    ana.histograms.addHistogram("dphill", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(tx.getBranch<vector<LV>>("good_leptons_p4")[0], tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("detall", 180, 0, 5, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<vector<LV>>("good_leptons_p4")[0], tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("mindrlb", 180, 0, 10, [&]() { return tx.getBranch<float>("mindrlb"); } );
    ana.histograms.addHistogram("minmlb", 180, 0, 400, [&]() { return tx.getBranch<float>("minmlb"); } );
    ana.histograms.addHistogram("sumdrlb", 180, 0, 40, [&]() { return tx.getBranch<float>("sumdrlb"); } );
    ana.histograms.addHistogram("summlb", 180, 0, 1600, [&]() { return tx.getBranch<float>("summlb"); } );
    ana.histograms.addHistogram("channel", 3, 0, 3, [&]() { int pdgidprod = tx.getBranch<vector<int>>("good_leptons_pdgid")[0] * tx.getBranch<vector<int>>("good_leptons_pdgid")[1]; if (pdgidprod == 121) return 0; else if (pdgidprod == 143) return 1; else if (pdgidprod == 169) return 2; else return 3; } );
    ana.histograms.addHistogram("nbloose", 5, 0, 5, [&]() { return tx.getBranch<int>("nbloose"); } );
    ana.histograms.addHistogram("nbmedium", 5, 0, 5, [&]() { return tx.getBranch<int>("nbmedium"); } );
    ana.histograms.addHistogram("nbtight", 5, 0, 5, [&]() { return tx.getBranch<int>("nbtight"); } );
    ana.histograms.addHistogram("maxmjj", 180, 0, 3000, [&]() { return tx.getBranch<float>("maxmjj"); } );
    ana.histograms.addHistogram("detajj_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<LV>("vbf_jets_maxmjj_1"))); } );
    ana.histograms.addHistogram("mbb_maxmjj", 180, 0, 300, [&]() { return tx.getBranch<float>("mbb_maxmjj"); } );
    ana.histograms.addHistogram("drbb_maxmjj", 180, 0, 10, [&]() { return tx.getBranch<float>("drbb_maxmjj"); } );
    ana.histograms.addHistogram("mbb_mindr", 180, 0, 300, [&]() { return tx.getBranch<float>("mbb_mindr"); } );
    ana.histograms.addHistogram("drbb_mindr", 180, 0, 10, [&]() { return tx.getBranch<float>("drbb_mindr"); } );
    ana.histograms.addHistogram("drjl00_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaR(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("drjl01_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaR(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("drjl10_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaR(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("drjl11_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaR(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("detajl00_maxmjj", 180, -10, 10, [&]() { return RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[0]); } );
    ana.histograms.addHistogram("detajl01_maxmjj", 180, -10, 10, [&]() { return RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[1]); } );
    ana.histograms.addHistogram("detajl10_maxmjj", 180, -10, 10, [&]() { return RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[0]); } );
    ana.histograms.addHistogram("detajl11_maxmjj", 180, -10, 10, [&]() { return RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[1]); } );
    ana.histograms.addHistogram("dphijl00_maxmjj", 180, -3.1416, 3.1416, [&]() { return RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[0]); } );
    ana.histograms.addHistogram("dphijl01_maxmjj", 180, -3.1416, 3.1416, [&]() { return RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[1]); } );
    ana.histograms.addHistogram("dphijl10_maxmjj", 180, -3.1416, 3.1416, [&]() { return RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[0]); } );
    ana.histograms.addHistogram("dphijl11_maxmjj", 180, -3.1416, 3.1416, [&]() { return RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[1]); } );
    ana.histograms.addHistogram("absdetajl00_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("absdetajl01_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("absdetajl10_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("absdetajl11_maxmjj", 180, 0, 10, [&]() { return fabs(RooUtil::Calc::DeltaEta(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("absdphijl00_maxmjj", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("absdphijl01_maxmjj", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_0"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("absdphijl10_maxmjj", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[0])); } );
    ana.histograms.addHistogram("absdphijl11_maxmjj", 180, 0, 3.1416, [&]() { return fabs(RooUtil::Calc::DeltaPhi(tx.getBranch<LV>("vbf_jets_maxmjj_1"), tx.getBranch<vector<LV>>("good_leptons_p4")[1])); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_0_pt", 180, 0, 1000, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_0").pt(); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_1_pt", 180, 0, 500, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_1").pt(); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_0_eta", 180, -5, 5, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_0").eta(); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_1_eta", 180, -5, 5, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_1").eta(); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_0_abseta", 180, 0, 5, [&]() { return fabs(tx.getBranch<LV>("vbf_jets_maxmjj_0").eta()); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_1_abseta", 180, 0, 5, [&]() { return fabs(tx.getBranch<LV>("vbf_jets_maxmjj_1").eta()); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_0_phi", 180, -3.1416, 3.1416, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_0").phi(); } );
    ana.histograms.addHistogram("vbf_jets_maxmjj_1_phi", 180, -3.1416, 3.1416, [&]() { return tx.getBranch<LV>("vbf_jets_maxmjj_1").phi(); } );
    ana.histograms.addHistogram("njets", 8, 0, 8, [&]() { return tx.getBranch<vector<LV>>("good_jets_p4").size(); } );
    ana.histograms.addHistogram("met", 180, 0, 400, [&]() { return nt.MET_pt(); } );
    ana.histograms.addHistogram("maxmjj_binned", 3, 0, 3,
            [&]()
            {
                if (tx.getBranch<float>("maxmjj") < 1000.)
                {
                    return 0.;
                }
                else if (tx.getBranch<float>("maxmjj") < 2000.)
                {
                    return 1.;
                }
                else if (tx.getBranch<float>("maxmjj") < 2000.)
                {
                    return 2.;
                }
                else
                {
                    return -1.;
                }
            } );

    // Book cutflows
    ana.cutflow.bookCutflows();

    // Book Histograms
    ana.cutflow.bookHistogramsForCutAndBelow(ana.histograms, "TwoTightLeptons");

    ana.cutflow.printCuts();

    // Looping input file
    while (ana.looper.nextEvent())
    {

        // If splitting jobs are requested then determine whether to process the event or not based on remainder
        if (result.count("job_index") and result.count("nsplit_jobs"))
        {
            if (ana.looper.getNEventsProcessed() % ana.nsplit_jobs != (unsigned int) ana.job_index)
                continue;
        }

        //Do what you need to do in for each event here
        //To save use the following function
        ana.cutflow.fill();
    }

    // Writing output file
    ana.cutflow.saveOutput();

    tx.write();

    // The below can be sometimes crucial
    delete ana.output_tfile;
}
