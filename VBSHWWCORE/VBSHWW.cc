#include "VBSHWW.h"

VBSHWW::VBSHWW(int argc, char** argv) :
    tx(
       (parseCLI(argc, argv), "variable"), // This weird syntax guarantees that parseCLI runs prior to initializing "tx"
       "variable"
      )
{

//=============================
//
// Setting Up Analysis
//
//=============================

    // Create the TChain that holds the TTree's of the baby ntuples
    events_tchain = RooUtil::FileUtil::createTChain(input_tree_name, input_file_list_tstring);

    // Set up a looper
    looper.init(events_tchain, &nt, n_events);

    // Set the cutflow object output file
    cutflow.setTFile(output_tfile);

    // Set up the NanoCORE's common configuration service tool
    gconf.GetConfigs(nt.year());

//=============================
//
// Analysis Data Structure
//
//=============================

    // Create event level branches
    tx.createBranch<int>("run");
    tx.createBranch<int>("lumi");
    tx.createBranch<unsigned long long>("evt");

    // Create met branches
    tx.createBranch<LV>("met_p4");

    // Create lepton branches
    tx.createBranch<vector<LV>>("good_leptons_p4");
    tx.createBranch<vector<int>>("good_leptons_pdgid");
    tx.createBranch<vector<int>>("good_leptons_tight");
    tx.createBranch<vector<int>>("good_leptons_jetIdx");
    tx.createBranch<vector<float>>("good_leptons_pfRelIso03_all");
    tx.createBranch<vector<float>>("good_leptons_pfRelIso03_chg");
    tx.createBranch<vector<float>>("good_leptons_jetPtRelv2");
    tx.createBranch<vector<float>>("good_leptons_jetRelIso");
    tx.createBranch<vector<float>>("good_leptons_miniPFRelIso_all");

    // Create jet branches (NOTE: > 20 GeV jets are saved)
    tx.createBranch<vector<LV>>("good_jets_p4");
    tx.createBranch<vector<int>>("good_jets_loose_btagged");
    tx.createBranch<vector<int>>("good_jets_medium_btagged");
    tx.createBranch<vector<int>>("good_jets_tight_btagged");
    tx.createBranch<vector<float>>("good_jets_btag_score");
    tx.createBranch<vector<float>>("good_jets_qg_disc");
    
    // Create higgs tagged jet branches
    tx.createBranch<vector<LV>>("higgs_jets_p4");
    tx.createBranch<vector<int>>("higgs_jets_loose_btagged");
    tx.createBranch<vector<int>>("higgs_jets_medium_btagged");
    tx.createBranch<vector<int>>("higgs_jets_tight_btagged");
    tx.createBranch<vector<float>>("higgs_jets_btag_score");
    tx.createBranch<vector<int>>("higgs_jets_good_jets_idx");

    // Create vbs tagged jet branches
    tx.createBranch<vector<LV>>("vbs_jets_p4");
    tx.createBranch<vector<int>>("vbs_jets_good_jets_idx");

    // Create number of b-tagged jets branches
    tx.createBranch<int>("nbloose");
    tx.createBranch<int>("nbmedium");
    tx.createBranch<int>("nbtight");

    // Create jet multiplicity branches
    tx.createBranch<int>("ncenjet30");
    tx.createBranch<int>("njet30");

//=============================
//
// Common Selections
//
//=============================

    //*****************************
    // Weighting each MC event
    //*****************************
    // Description: Weighting each event to 137/fb
    // TODO TODO TODO TODO TODO: MAKE LUMINOSITY AN CLI ARGUMENT THAT DEPENDS ON YEAR
    cutflow.addCut("Weight",
        [&]()
        {
            // Set event level variables that would be processed for the given event regardless
            tx.setBranch<int>("run", nt.run());
            tx.setBranch<int>("lumi", nt.luminosityBlock());
            tx.setBranch<unsigned long long>("evt", nt.event());
            tx.setBranch<LV>("met_p4", RooUtil::Calc::getLV(nt.MET_pt(), 0, nt.MET_phi(), 0));
            return 1/*set your cut here*/;
        },
        [&]()
        {
            float wgt = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0)) * scale1fb;
            return wgt * gconf.lumi;
        });

    //*****************************
    // - NanoAOD level preselection
    //*****************************
    // Description: Select at least two leptons in NanoAOD with 20 GeV and above
    cutflow.addCutToLastActiveCut("Preselection",
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
}

VBSHWW::~VBSHWW() {}

void VBSHWW::parseCLI(int argc, char** argv)
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
        input_file_list_tstring = result["input"].as<std::string>();
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
        input_tree_name = result["tree"].as<std::string>();
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
        scale1fb = result["scale1fb"].as<float>();
    }

    //_______________________________________________________________________________
    // --debug
    if (result.count("debug"))
    {
        output_tfile = new TFile("debug.root", "recreate");
    }
    else
    {
        //_______________________________________________________________________________
        // --output
        if (result.count("output"))
        {
            output_tfile = new TFile(result["output"].as<std::string>().c_str(), "create");
            if (not output_tfile->IsOpen())
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
    n_events = result["nevents"].as<int>();

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("nsplit_jobs"))
    {
        nsplit_jobs = result["nsplit_jobs"].as<int>();
        if (nsplit_jobs <= 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --nsplit_jobs" << nsplit_jobs << " has zero or negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        nsplit_jobs = -1;
    }

    //_______________________________________________________________________________
    // --nsplit_jobs
    if (result.count("job_index"))
    {
        job_index = result["job_index"].as<int>();
        if (job_index < 0)
        {
            std::cout << options.help() << std::endl;
            std::cout << "ERROR: option string --job_index" << job_index << " has negative value!" << std::endl;
            std::cout << "I am not sure what this means..." << std::endl;
            exit(1);
        }
    }
    else
    {
        job_index = -1;
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
            if (job_index >= nsplit_jobs)
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
    std::cout <<  " input_file_list_tstring: " << input_file_list_tstring <<  std::endl;
    std::cout <<  " output_tfile: " << output_tfile->GetName() <<  std::endl;
    std::cout <<  " n_events: " << n_events <<  std::endl;
    std::cout <<  " nsplit_jobs: " << nsplit_jobs <<  std::endl;
    std::cout <<  " job_index: " << job_index <<  std::endl;
    std::cout <<  "=========================================================" << std::endl;

}

// For a given cutname it writes the run, lumi, evt into a textfile based on the output root name
void VBSHWW::writeEventList(TString cutname)
{
    TString eventlist_output_name = output_tfile->GetName(); // get the output file name
    TString suffix = TString::Format(".%s.txt", cutname.Data());
    eventlist_output_name.ReplaceAll(".root", suffix); // replace .root with suffix if .root exists
    if (not eventlist_output_name.Contains(suffix)) eventlist_output_name += suffix; // if no suffix exists, then append suffix
    cutflow.getCut(cutname).writeEventList(eventlist_output_name);
}

void VBSHWW::process(TString final_cutname)
{
    // Clear the data structure for the event
    tx.clear();

    // Set the run lumi and event for this event
    cutflow.setEventID(nt.run(), nt.luminosityBlock(), nt.event()); // Setting event ID in case we need to keep track of event id

    // Run all the cutflow selections and filling histograms
    cutflow.fill();

    // Writing skimmed event tree for the data structure we created
    if (cutflow.getCut(final_cutname).pass)
    {
        tx.fill();
    }
}

void VBSHWW::initSRCutflow() 
{

    //*****************************
    // - Selecting Analysis Leptons
    //*****************************
    // Description: Select leptons used for the analysis
    //              The electrons / muons definitions are in NanoCORE/Electron(Muon)Selections.cc
    cutflow.addCutToLastActiveCut("SelectLeptons",
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
                    tx.pushbackToBranch<int>("good_leptons_jetIdx", nt.Muon_jetIdx()[imu]);
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
                    tx.pushbackToBranch<int>("good_leptons_jetIdx", nt.Electron_jetIdx()[iel]);
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
                    /* names of any associated vector<int>   branches to sort along */ {"good_leptons_pdgid", "good_leptons_tight", "good_leptons_jetIdx"},
                    /* names of any associated vector<bool>  branches to sort along */ {}
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
    cutflow.addCutToLastActiveCut("SSPreselection",
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
    cutflow.addCutToLastActiveCut("SelectJets",
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
                for (unsigned int ilep = 0; ilep < tx.getBranch<vector<LV>>("good_leptons_p4").size(); ++ilep)
                {
                    if (tx.getBranch<vector<int>>("good_leptons_jetIdx").at(ilep) == (int) ijet)
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

                tx.pushbackToBranch<LV>("good_jets_p4", jet_p4);
                tx.pushbackToBranch<int>("good_jets_loose_btagged", is_loose_btagged);
                tx.pushbackToBranch<int>("good_jets_medium_btagged", is_medium_btagged);
                tx.pushbackToBranch<int>("good_jets_tight_btagged", is_tight_btagged);
                tx.pushbackToBranch<float>("good_jets_btag_score", nt.Jet_btagDeepFlavB()[ijet]);
                tx.pushbackToBranch<float>("good_jets_qg_disc", nt.Jet_qgl()[ijet]);

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
    cutflow.addCutToLastActiveCut("GeqTwoMedBtag", [&]() { return tx.getBranch<int>("nbmedium") >= 2; }, UNITY);

    //*****************************
    // - Require Two Tight Btag
    //*****************************
    // Description: Select two tight b-tag /* TODO TODO TODO TODO btag scale factor */
    cutflow.addCutToLastActiveCut("GeqTwoTightBtag", [&]() { return tx.getBranch<int>("nbtight") >= 2; }, UNITY);

    //*****************************
    // - Tag Hbb jets
    //*****************************
    // Description: Select the two b-tagged jets with highest btagging score as Hbb jets
    cutflow.addCutToLastActiveCut("TagHiggsJets",
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

    //*****************************
    // - Tag VBS jets
    //*****************************
    // Description: Select two jets that are not part of the Hbb jets to be VBS jets with following algorithm
    //              - If all jets are in same hemisphere (eta > 0 or eta < 0) then choose the two leading jets in P (N.B. not pt!)
    //              - If not choose the leading jet in each hemisphere leading in P (N.B. not pt!)
    cutflow.addCutToLastActiveCut("TagVBSJets",
        [&]()
        {

            // higgs jet indices
            const int& higgs_jet_0 = tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[0];
            const int& higgs_jet_1 = tx.getBranch<vector<int>>("higgs_jets_good_jets_idx")[1];
            // good jets p4
            const vector<LV>& good_jets_p4 = tx.getBranch<vector<LV>>("good_jets_p4");

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
                tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_cands_idxs[0]]);
                tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_cands_idxs[1]]);
                tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_cands_idxs[0]);
                tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_cands_idxs[1]);
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

            tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_idx_0]);
            tx.pushbackToBranch<LV>("vbs_jets_p4", good_jets_p4[vbs_jet_idx_1]);
            tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_idx_0);
            tx.pushbackToBranch<int>("vbs_jets_good_jets_idx", vbs_jet_idx_1);

            return true;

        }, 
        UNITY);

    cutflow.addCutToLastActiveCut("VBSJetPreselection",
        [&]()
        {
            LV vbs_jet_0 = tx.getBranch<vector<LV>>("vbs_jets_p4").at(0);
            LV vbs_jet_1 = tx.getBranch<vector<LV>>("vbs_jets_p4").at(1);
            float M_jj = (vbs_jet_0 + vbs_jet_1).M();
            float deta_jj = vbs_jet_0.eta() - vbs_jet_1.eta();
            return (M_jj >= 500 && fabs(deta_jj) >= 3);
        }, 
        UNITY);

    //*****************************
    // - SR Preselection Dummy Cut Node
    //*****************************
    // Description: This is a dummy cut node to indicate that this node represents where all the SR preselection is applied
    cutflow.addCutToLastActiveCut("SignalRegionPreselection", UNITY, UNITY);

    return;
}

void VBSHWW::initBDTInputComputation() 
{

    tx.createBranch<float>("lepton1pt");
    tx.createBranch<float>("lepton2pt");
    tx.createBranch<float>("lepton1reliso");
    tx.createBranch<float>("lepton2reliso");
    tx.createBranch<int>("bjetscoretype");
    tx.createBranch<float>("dibjetmass");
    tx.createBranch<float>("bjet1eta");
    tx.createBranch<float>("bjet2eta");
    tx.createBranch<float>("dibjetdeltaR");
    tx.createBranch<float>("dibjetdeta");
    tx.createBranch<float>("dibjetdphi");
    tx.createBranch<float>("leadingvbfjetp");
    tx.createBranch<float>("subleadvbfjetp");
    tx.createBranch<float>("divbfjetmass");
    tx.createBranch<float>("divbfjetdeta");
    tx.createBranch<float>("divbfjetdphi");
    tx.createBranch<float>("vbfjet1eta");
    tx.createBranch<float>("vbfjet2eta");
    tx.createBranch<float>("vbfjet1Pt");
    tx.createBranch<float>("vbfjet2Pt");
    tx.createBranch<float>("divbfjetPt");
    tx.createBranch<float>("divbfjetdeltaR");

    tx.createBranch<int>("lepflavorchannel");

    cutflow.getCut("VBSJetPreselection");
    cutflow.addCutToLastActiveCut("ComputeBDTInputs",
            [&]()
            {
                // lepton 4-vectors and extra info
                LV lepton_0 = tx.getBranch<vector<LV>>("good_leptons_p4").at(0);
                LV lepton_1 = tx.getBranch<vector<LV>>("good_leptons_p4").at(1);
                int lepton_0_pdgid = tx.getBranch<vector<int>>("good_leptons_pdgid").at(0);
                int lepton_1_pdgid = tx.getBranch<vector<int>>("good_leptons_pdgid").at(1);

                // b-jets 4-vectors and extra info
                LV higgs_jet_0 = tx.getBranch<vector<LV>>("higgs_jets_p4").at(0);
                LV higgs_jet_1 = tx.getBranch<vector<LV>>("higgs_jets_p4").at(1);
                float higgs_jet_0_btag_score = tx.getBranch<vector<float>>("higgs_jets_btag_score").at(0);
                float higgs_jet_1_btag_score = tx.getBranch<vector<float>>("higgs_jets_btag_score").at(1);

                // vbf tagged jets 4-vectors
                LV vbs_jet_0 = tx.getBranch<vector<LV>>("vbs_jets_p4").at(0);
                LV vbs_jet_1 = tx.getBranch<vector<LV>>("vbs_jets_p4").at(1);

                // Compute various input variables
                float M_jj = (vbs_jet_0 + vbs_jet_1).M();
                float deta_jj = vbs_jet_0.eta() - vbs_jet_1.eta();
                int lepflavorchannel = -1;
                if (abs(lepton_0_pdgid) == 11 and  abs(lepton_1_pdgid) == 11) lepflavorchannel = 0;
                if (abs(lepton_0_pdgid) == 13 and  abs(lepton_1_pdgid) == 11) lepflavorchannel = 1;
                if (abs(lepton_0_pdgid) == 11 and  abs(lepton_1_pdgid) == 13) lepflavorchannel = 2;
                if (abs(lepton_0_pdgid) == 13 and  abs(lepton_1_pdgid) == 13) lepflavorchannel = 3;
                // TODO: Compute other objects

                tx.setBranch<float>("lepton1pt", -999); // TODO
                tx.setBranch<float>("lepton2pt", -999); // TODO
                tx.setBranch<float>("lepton1reliso", -999); // TODO
                tx.setBranch<float>("lepton2reliso", -999); // TODO
                tx.setBranch<int>("bjetscoretype", -999); // TODO
                tx.setBranch<float>("dibjetmass", -999); // TODO
                tx.setBranch<float>("bjet1eta", -999); // TODO
                tx.setBranch<float>("bjet2eta", -999); // TODO
                tx.setBranch<float>("dibjetdeltaR", -999); // TODO
                tx.setBranch<float>("dibjetdeta", -999); // TODO
                tx.setBranch<float>("dibjetdphi", -999); // TODO
                tx.setBranch<float>("leadingvbfjetp", -999); // TODO
                tx.setBranch<float>("subleadvbfjetp", -999); // TODO
                tx.setBranch<float>("divbfjetmass", M_jj);
                tx.setBranch<float>("divbfjetdeta", deta_jj);
                tx.setBranch<float>("divbfjetdphi", -999); // TODO
                tx.setBranch<float>("vbfjet1eta", -999); // TODO
                tx.setBranch<float>("vbfjet2eta", -999); // TODO
                tx.setBranch<float>("vbfjet1Pt", -999); // TODO
                tx.setBranch<float>("vbfjet2Pt", -999); // TODO
                tx.setBranch<float>("divbfjetPt", -999); // TODO
                tx.setBranch<float>("divbfjetdeltaR", -999); // TODO

                tx.setBranch<int>("lepflavorchannel", lepflavorchannel);

                return true;
            },
            UNITY
            );
}
