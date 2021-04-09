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

    // Setting up good runs list
    // TString vbsHwwNanoLooperDirPath = gSystem->Getenv("VBSHWWNANOLOOPERDIR");
    // const char* json_file = TString::Format("%s/NanoTools/NanoCORE/Tools/data/Cert_271036-325175_13TeV_Combined161718_JSON_snt.txt", vbsHwwNanoLooperDirPath.Data()).Data();
    // cout << "Setting grl: " << json_file << endl;
    set_goodrun_file("/nfs-7/userdata/phchang/analysis_data/grl/Cert_271036-325175_13TeV_Combined161718_JSON_snt.txt");

//=============================
//
// Analysis Data Structure
//
//=============================

    // Create event level branches
    tx.createBranch<int>("run");
    tx.createBranch<int>("lumi");
    tx.createBranch<unsigned long long>("evt");
    tx.createBranch<float>("wgt");

    // Create met branches
    tx.createBranch<LV>("met_p4");

    // Create gen particle branches
    tx.createBranch<int>("isvbswwh");
    tx.createBranch<LV>("gen_jet0_p4");
    tx.createBranch<LV>("gen_jet1_p4");
    tx.createBranch<LV>("gen_w0_p4");
    tx.createBranch<LV>("gen_w1_p4");
    tx.createBranch<LV>("gen_h_p4");
    tx.createBranch<LV>("gen_lep0_p4");
    tx.createBranch<LV>("gen_lep1_p4");
    tx.createBranch<LV>("gen_nu0_p4");
    tx.createBranch<LV>("gen_nu1_p4");
    tx.createBranch<LV>("gen_b0_p4");
    tx.createBranch<LV>("gen_b1_p4");
    tx.createBranch<float>("genleppt0");
    tx.createBranch<float>("genleppt1");
    tx.createBranch<float>("gentaupt");
    tx.createBranch<int>("genchannel");
    tx.createBranch<float>("genmjj");
    tx.createBranch<float>("genptjj");
    tx.createBranch<float>("gendetajj");
    tx.createBranch<float>("gendphijj");
    tx.createBranch<float>("genmbb");
    tx.createBranch<float>("genptbb");
    tx.createBranch<float>("gendphibb");
    tx.createBranch<float>("gendrbb");
    tx.createBranch<float>("genmll");
    tx.createBranch<float>("genptll");
    tx.createBranch<float>("gendphill");
    tx.createBranch<float>("gendrll");
    tx.createBranch<float>("genst");
    tx.createBranch<float>("genmtllbbmet");
    tx.createBranch<float>("genmllbbmet");

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

    // Create tau branches
    tx.createBranch<vector<LV>>("good_taus_p4");
    tx.createBranch<vector<int>>("good_taus_pdgid");
    tx.createBranch<vector<int>>("good_taus_tight");
    tx.createBranch<vector<int>>("good_taus_jetIdx");
    tx.createBranch<vector<int>>("good_taus_genPartFlav");

    // Create jet branches (NOTE: > 20 GeV jets are saved)
    tx.createBranch<vector<LV>>("good_jets_p4");
    tx.createBranch<vector<int>>("good_jets_loose_btagged");
    tx.createBranch<vector<int>>("good_jets_medium_btagged");
    tx.createBranch<vector<int>>("good_jets_tight_btagged");
    tx.createBranch<vector<float>>("good_jets_btag_score");
    tx.createBranch<vector<float>>("good_jets_qg_disc");

    // Create fatjet branches (NOTE: > 20 GeV jets are saved)
    tx.createBranch<vector<LV>>("good_fatjets_p4");
    tx.createBranch<vector<int>>("good_fatjets_loose");
    tx.createBranch<vector<int>>("good_fatjets_medium");
    tx.createBranch<vector<int>>("good_fatjets_tight");
    tx.createBranch<vector<float>>("good_fatjets_msd");

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

    // Create 6 fermion vectors
    tx.createBranch<LV>("lep0");
    tx.createBranch<LV>("lep1");
    tx.createBranch<LV>("leadlep");
    tx.createBranch<LV>("subllep");
    tx.createBranch<LV>("b0");
    tx.createBranch<LV>("b1");
    tx.createBranch<LV>("j0");
    tx.createBranch<LV>("j1");
    tx.createBranch<int>("channel");
    tx.createBranch<int>("channeldetail");
    tx.createBranch<int>("lepchannel");
    tx.createBranch<int>("btagchannel");
    tx.createBranch<int>("mee_noZ");
    tx.createBranch<int>("mbbIn");
    tx.createBranch<int>("pass_blind");

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
            if (nt.isData())
            {
                tx.setBranch<float>("wgt", 1.);
            }
            else
            {
                float wgt = ((nt.Generator_weight() > 0) - (nt.Generator_weight() < 0)) * scale1fb;
                tx.setBranch<float>("wgt", wgt * gconf.lumi);
            }
            return tx.getBranch<float>("wgt");
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
            int ntau20 = 0;
            std::vector<int> el20_idx;
            std::vector<int> mu20_idx;
            std::vector<int> tau20_idx;

            // loop over electrons to count electrons above 20 GeV
            for (unsigned int idx = 0; idx < nt.Electron_pt().size(); ++idx)
            {
                float el_pt = nt.Electron_pt()[idx];
                if (el_pt > 20)
                {
                    nel20++;
                    el20_idx.push_back(idx);
                }
            }

            // loop over muons to count muons above 20 GeV
            for (unsigned int idx = 0; idx < nt.Muon_pt().size(); ++idx)
            {
                float mu_pt = nt.Muon_pt()[idx];
                if (mu_pt > 20)
                {
                    nmu20++;
                    mu20_idx.push_back(idx);
                }
            }

            if (do_tau)
            {
                // loop over taus to count taus above 20 GeV
                for (unsigned int idx = 0; idx < nt.Tau_pt().size(); ++idx)
                {
                    float tau_pt = nt.Tau_pt()[idx];
                    if (tau_pt > 20)
                    {
                        ntau20++;
                        tau20_idx.push_back(idx);
                    }
                }
            }

            // return true if more than or equals to 2 leptons
            return nel20 + nmu20 + ntau20 >= 2;
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

    do_tau = true;

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

    // // Set the run lumi and event for this event
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

    //********************************
    // - Selecting Gen Level Particles
    //********************************
    // Description: If signal sample, select the gen level particles
    cutflow.addCutToLastActiveCut("SelectGenPart",
        [&]()
        {
            if (looper.getCurrentFileName().Contains("VBSWmpWmpHToLNuLNu_C2V"))
            {
                bool isvbswwh = nt.GenPart_status()[2] == 23;
                tx.setBranch<int>("isvbswwh", isvbswwh);
                if (isvbswwh)
                {
                    const LV& ijet = nt.GenPart_p4()[2];
                    const LV& jjet = nt.GenPart_p4()[3];
                    const LV& jet0 = ijet.pt() > jjet.pt() ? ijet : jjet;
                    const LV& jet1 = ijet.pt() > jjet.pt() ? jjet : ijet;
                    tx.setBranch<LV>("gen_jet0_p4", jet0);
                    tx.setBranch<LV>("gen_jet1_p4", jet1);
                    const LV& iW = nt.GenPart_p4()[4];
                    const LV& jW = nt.GenPart_p4()[5];
                    const LV& W0 = iW.pt() > jW.pt() ? iW : jW;
                    const LV& W1 = iW.pt() > jW.pt() ? jW : iW;
                    tx.setBranch<LV>("gen_w0_p4", W0);
                    tx.setBranch<LV>("gen_w1_p4", W1);
                    const LV& h = nt.GenPart_p4()[6];
                    tx.setBranch<LV>("gen_h_p4", h);

                    std::vector<LV> leptons;
                    std::vector<int> lepton_pdgids;
                    for (unsigned int igen = 0; igen < nt.GenPart_p4().size(); ++igen)
                    {
                        int midx = nt.GenPart_genPartIdxMother()[igen];
                        if (midx > 1)
                        {
                            if (abs(nt.GenPart_pdgId()[midx]) == 24 and abs(nt.GenPart_status()[midx]) == 62
                                and (
                                    ((nt.GenPart_status()[igen] == 1 or nt.GenPart_status()[igen] == 23) and (abs(nt.GenPart_pdgId()[igen]) == 11 or abs(nt.GenPart_pdgId()[igen]) == 13))
                                    or
                                    ((nt.GenPart_status()[igen] == 2 or nt.GenPart_status()[igen] == 23) and (abs(nt.GenPart_pdgId()[igen]) == 15))
                                    )
                               )
                            {
                                leptons.push_back(nt.GenPart_p4()[igen]);
                                lepton_pdgids.push_back(nt.GenPart_pdgId()[igen]);
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }

                    if (leptons.size() != 2)
                    {
                        std::cout <<  " leptons.size(): " << leptons.size() <<  std::endl;
                        for (unsigned int igen = 0; igen < nt.GenPart_p4().size(); ++igen)
                        {
                            int pdgid = nt.GenPart_pdgId()[igen];
                            int status = nt.GenPart_status()[igen];
                            int midx = nt.GenPart_genPartIdxMother()[igen];
                            int mid = midx > -1 ? nt.GenPart_pdgId()[midx] : -1;
                            int mstatus = midx > -1 ? nt.GenPart_status()[midx] : -1;
                            std::cout <<  " pdgid: " << pdgid <<  " status: " << status <<  " midx: " << midx <<  " mid: " << mid <<  " mstatus: " << mstatus <<  std::endl;
                        }
                    }
                    const LV& lep0 = leptons[0].pt() > leptons[1].pt() ? leptons[0] : leptons[1];
                    const LV& lep1 = leptons[0].pt() > leptons[1].pt() ? leptons[1] : leptons[0];

                    std::vector<LV> nus;
                    for (unsigned int igen = 0; igen < nt.GenPart_p4().size(); ++igen)
                    {
                        int midx = nt.GenPart_genPartIdxMother()[igen];
                        if (midx > 1)
                        {
                            if (abs(nt.GenPart_pdgId()[midx]) == 24 and abs(nt.GenPart_status()[midx]) == 62
                                and (
                                    ((nt.GenPart_status()[igen] == 1 or nt.GenPart_status()[igen] == 23) and (abs(nt.GenPart_pdgId()[igen]) == 12 or abs(nt.GenPart_pdgId()[igen]) == 14))
                                    or
                                    ((nt.GenPart_status()[igen] == 1 or nt.GenPart_status()[igen] == 23) and (abs(nt.GenPart_pdgId()[igen]) == 16))
                                    )
                               )
                            {
                                nus.push_back(nt.GenPart_p4()[igen]);
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }

                    if (nus.size() != 2)
                    {
                        std::cout <<  " nus.size(): " << nus.size() <<  std::endl;
                    }
                    const LV& nu0 = nus[0].pt() > nus[1].pt() ? nus[0] : nus[1];
                    const LV& nu1 = nus[0].pt() > nus[1].pt() ? nus[1] : nus[0];

                    std::vector<LV> bs;
                    for (unsigned int igen = 0; igen < nt.GenPart_p4().size(); ++igen)
                    {
                        if (abs(nt.GenPart_pdgId()[igen]) == 5 and nt.GenPart_status()[igen] == 23)
                        {
                            bs.push_back(nt.GenPart_p4()[igen]);
                        }
                    }

                    if (bs.size() != 2)
                    {
                        std::cout <<  " bs.size(): " << bs.size() <<  std::endl;
                    }
                    const LV& b0 = bs[0].pt() > bs[1].pt() ? bs[0] : bs[1];
                    const LV& b1 = bs[0].pt() > bs[1].pt() ? bs[1] : bs[0];

                    tx.setBranch<LV>("gen_lep0_p4", lep0);
                    tx.setBranch<LV>("gen_lep1_p4", lep1);
                    tx.setBranch<LV>("gen_nu0_p4", nu0);
                    tx.setBranch<LV>("gen_nu1_p4", nu1);
                    tx.setBranch<LV>("gen_b0_p4", b0);
                    tx.setBranch<LV>("gen_b1_p4", b1);

                    int channel = -1;
                    if (lepton_pdgids[0] * lepton_pdgids[1] == 121)
                    {
                        channel = 0;
                    }
                    else if (lepton_pdgids[0] * lepton_pdgids[1] == 143)
                    {
                        channel = 1;
                    }
                    else if (lepton_pdgids[0] * lepton_pdgids[1] == 169)
                    {
                        channel = 2;
                    }
                    else if (lepton_pdgids[0] * lepton_pdgids[1] == 165)
                    {
                        channel = 3;
                    }
                    else if (lepton_pdgids[0] * lepton_pdgids[1] == 195)
                    {
                        channel = 4;
                    }
                    else if (lepton_pdgids[0] * lepton_pdgids[1] == 225)
                    {
                        channel = 5;
                    }

                    tx.setBranch<float>("genleppt0", lep0.pt());
                    tx.setBranch<float>("genleppt1", lep1.pt());
                    tx.setBranch<float>("gentaupt", -999.);
                    tx.setBranch<int>("genchannel", channel);
                    tx.setBranch<float>("genmjj", (jet0+jet1).mass());
                    tx.setBranch<float>("genptjj", (jet0+jet1).pt());
                    tx.setBranch<float>("gendetajj", abs(RooUtil::Calc::DeltaEta(jet0, jet1)));
                    tx.setBranch<float>("gendphijj", abs(RooUtil::Calc::DeltaPhi(jet0, jet1)));
                    tx.setBranch<float>("genmbb", (b0+b1).mass());
                    tx.setBranch<float>("genptbb", (b0+b1).pt());
                    tx.setBranch<float>("gendphibb", abs(RooUtil::Calc::DeltaPhi(b0, b1)));
                    tx.setBranch<float>("gendrbb", RooUtil::Calc::DeltaR(b0, b1));
                    tx.setBranch<float>("genmll", (lep0+lep1).mass());
                    tx.setBranch<float>("genptll", (lep0+lep1).pt());
                    tx.setBranch<float>("gendphill", abs(RooUtil::Calc::DeltaPhi(lep0, lep1)));
                    tx.setBranch<float>("gendrll", RooUtil::Calc::DeltaR(lep0, lep1));
                    tx.setBranch<float>("genst", (lep0.pt() + lep1.pt() + b0.pt() + b1.pt() + (nu0 + nu1).pt()));
                    tx.setBranch<float>("genmtllbbmet", (lep0 + lep1 + b0 + b1 + nu0 + nu1).mt());
                    tx.setBranch<float>("genmllbbmet", (lep0 + lep1 + b0 + b1 + nu0 + nu1).mass());

                    return true;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }, UNITY);

    //************************************
    // - Selecting Good Runs List for data
    //************************************
    cutflow.addCutToLastActiveCut("GoodRunsList",
        [&]()
        {
            if (nt.isData())
            {
                if (goodrun(nt.run(), nt.luminosityBlock()))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return true;
            }
        }, UNITY);

    //*****************************
    // - Applying MET event filters
    //*****************************
    cutflow.addCutToLastActiveCut("EventFilters",
        [&]()
        {

            bool filterflag   = false;
            bool filterflagMC = false;
            switch (nt.year())
            {
                case 2016:
                    filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter();
                    filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter();
                    break;
                case 2017:
                    filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilterV2();
                    filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilterV2();
                    break;
                case 2018:
                    filterflag   = nt.Flag_goodVertices() and nt.Flag_globalSuperTightHalo2016Filter() and nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilterV2();
                    filterflagMC = nt.Flag_goodVertices() and                                              nt.Flag_HBHENoiseFilter() and nt.Flag_HBHENoiseIsoFilter() and nt.Flag_EcalDeadCellTriggerPrimitiveFilter() and nt.Flag_BadPFMuonFilter() and nt.Flag_ecalBadCalibFilterV2();
                    break;
            }
            if (nt.isData())
            {
                return filterflag;
            }
            else
            {
                return filterflagMC;
            }
        }, UNITY);

    //*****************************
    // - Selecting Analysis Leptons
    //*****************************
    // Description: Select leptons used for the analysis
    //              The electrons / muons / taus definitions are in NanoCORE/Electron(Muon/Tau)Selections.cc
    cutflow.addCutToLastActiveCut("SelectLeptons",
        [&]()
        {

            // Select muons
            for (unsigned int imu = 0; imu < nt.Muon_pt().size(); ++imu)
            {
                if (SS::muonID(imu, SS::IDveto, nt.year()))
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
                if (SS::electronID(iel, SS::IDveto, nt.year()))
                {
                    tx.pushbackToBranch<LV>("good_leptons_p4", nt.Electron_p4()[iel]);
                    tx.pushbackToBranch<int>("good_leptons_pdgid", (-nt.Electron_charge()[iel]) * 11);
                    tx.pushbackToBranch<int>("good_leptons_tight", SS::electronID(iel, SS::IDtight, nt.year()));
                    tx.pushbackToBranch<int>("good_leptons_jetIdx", nt.Electron_jetIdx()[iel]);
                    tx.pushbackToBranch<float>("good_leptons_pfRelIso03_all", nt.Electron_pfRelIso03_all()[iel]);
                    tx.pushbackToBranch<float>("good_leptons_pfRelIso03_chg", nt.Electron_pfRelIso03_chg()[iel]);
                    tx.pushbackToBranch<float>("good_leptons_jetPtRelv2", nt.Electron_jetPtRelv2()[iel]);
                    tx.pushbackToBranch<float>("good_leptons_jetRelIso", nt.Electron_jetRelIso()[iel]);
                    tx.pushbackToBranch<float>("good_leptons_miniPFRelIso_all", nt.Electron_miniPFRelIso_all()[iel]);
                }
            }

            tx.sortVecBranchesByPt(
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
                },
                /* names of any associated vector<bool>  branches to sort along */
                {}
            );

            if (do_tau)
            {
                // Select taus
                const vector<LV>& good_leptons_p4 = tx.getBranchLazy<vector<LV>>("good_leptons_p4");
                const vector<int>& good_leptons_pdgid = tx.getBranchLazy<vector<int>>("good_leptons_pdgid");
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

                        if (!save_this_tau)
                        {
                            continue;
                        }

                        // Only save >= loose taus that do not overlap w/ a 'good' lepton
                        tx.pushbackToBranch<LV>("good_taus_p4", nt.Tau_p4()[itau]);
                        tx.pushbackToBranch<int>("good_taus_genPartFlav", nt.isData() ? -999 : nt.Tau_genPartFlav()[itau]);
                        tx.pushbackToBranch<int>("good_taus_pdgid", (-nt.Tau_charge()[itau]) * 15);
                        tx.pushbackToBranch<int>("good_taus_tight", SS::tauID(itau, SS::IDtight, nt.year()));
                        tx.pushbackToBranch<int>("good_taus_jetIdx", nt.Tau_jetIdx()[itau]);

                    }
                }

                tx.sortVecBranchesByPt(
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
            }

            return true;

        },
        UNITY);

    //*******************************
    // - Select Two Same-Sign Leptons
    //*******************************
    cutflow.addCutToLastActiveCut("SSPreselection",
        [&]()
        {

            // At least one loose light lepton
            if (not (tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() >= 1))
                return false;

            // Only two loose leptons
            if (not (tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() + tx.getBranchLazy<vector<LV>>("good_taus_p4").size() == 2))
                return false;

            int ntight = 0;
            for (auto& istight : tx.getBranch<vector<int>>("good_leptons_tight"))
            {
                if (istight)
                    ntight++;
            }

            for (auto& istight : tx.getBranchLazy<vector<int>>("good_taus_tight"))
            {
                if (istight)
                    ntight++;
            }

            // Select only two tight leptons
            if (not (ntight == 2))
                return false;

            // Therefore I will only have two leptons in the good_leptons container
            const int& pdgid0 = tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[0];
            const int& pdgid1 = tx.getBranchLazy<vector<int>>("good_leptons_pdgid").size() == 2 ? tx.getBranchLazy<vector<int>>("good_leptons_pdgid")[1] : tx.getBranchLazy<vector<int>>("good_taus_pdgid")[0];

            // Require same sign
            if (not (pdgid0 * pdgid1 > 0))
                return false;

            const float& pt0 = tx.getBranchLazy<vector<LV>>("good_leptons_p4")[0].pt();
            const float& pt1 = tx.getBranchLazy<vector<LV>>("good_leptons_p4").size() == 2 ? tx.getBranchLazy<vector<LV>>("good_leptons_p4")[1].pt() : tx.getBranchLazy<vector<LV>>("good_taus_p4")[0].pt();

            // Set the channel
            int lepchannel = -1;
            if (tx.getBranch<vector<LV>>("good_leptons_p4").size() == 2)
            {
                if (tx.getBranch<vector<int>>("good_leptons_pdgid")[0] * tx.getBranch<vector<int>>("good_leptons_pdgid")[1] == 121)
                    lepchannel = 0;
                else if (tx.getBranch<vector<int>>("good_leptons_pdgid")[0] * tx.getBranch<vector<int>>("good_leptons_pdgid")[1] == 143)
                    lepchannel = 1;
                else if (tx.getBranch<vector<int>>("good_leptons_pdgid")[0] * tx.getBranch<vector<int>>("good_leptons_pdgid")[1] == 169)
                    lepchannel = 2;
            }
            else // It has to be good_leptons_p4.size() == 1 by now
            {
                if (abs(tx.getBranch<vector<int>>("good_leptons_pdgid")[0]) == 11)
                    lepchannel = 3;
                else if (abs(tx.getBranch<vector<int>>("good_leptons_pdgid")[0]) == 13)
                    lepchannel = 4;
            }

            tx.setBranch<int>("lepchannel", lepchannel);

            // lep0 is lead if same flavor
            // lep1 is sublead if same flavor
            // lep0 is electron if emu
            // lep1 is muon if emu
            // lep0 is e or mu if lep-tau
            // lep1 is tau if lep-tau
            switch (lepchannel)
            {
                case 0:
                    tx.setBranch<LV>("lep0", tx.getBranch<vector<LV>>("good_leptons_p4")[0]);
                    tx.setBranch<LV>("lep1", tx.getBranch<vector<LV>>("good_leptons_p4")[1]);
                    break;
                case 1:
                    tx.setBranch<LV>("lep0", abs(tx.getBranch<vector<int>>("good_leptons_pdgid")[0]) == 11 ? tx.getBranch<vector<LV>>("good_leptons_p4")[0] : tx.getBranch<vector<LV>>("good_leptons_p4")[1]);
                    tx.setBranch<LV>("lep1", abs(tx.getBranch<vector<int>>("good_leptons_pdgid")[0]) == 11 ? tx.getBranch<vector<LV>>("good_leptons_p4")[1] : tx.getBranch<vector<LV>>("good_leptons_p4")[0]);
                    break;
                case 2:
                    tx.setBranch<LV>("lep0", tx.getBranch<vector<LV>>("good_leptons_p4")[0]);
                    tx.setBranch<LV>("lep1", tx.getBranch<vector<LV>>("good_leptons_p4")[1]);
                    break;
                case 3:
                    tx.setBranch<LV>("lep0", tx.getBranch<vector<LV>>("good_leptons_p4")[0]);
                    tx.setBranch<LV>("lep1", tx.getBranch<vector<LV>>("good_taus_p4")[0]);
                    break;
                case 4:
                    tx.setBranch<LV>("lep0", tx.getBranch<vector<LV>>("good_leptons_p4")[0]);
                    tx.setBranch<LV>("lep1", tx.getBranch<vector<LV>>("good_taus_p4")[0]);
                    break;
            }
            tx.setBranch<LV>("leadlep", tx.getBranch<LV>("lep0").pt() > tx.getBranch<LV>("lep1").pt() ? tx.getBranch<LV>("lep0") : tx.getBranch<LV>("lep1"));
            tx.setBranch<LV>("subllep", tx.getBranch<LV>("lep0").pt() > tx.getBranch<LV>("lep1").pt() ? tx.getBranch<LV>("lep1") : tx.getBranch<LV>("lep0"));

            // To veto same-sign dielectron on-Z (charge flip)
            int mee_noZ = (not (lepchannel == 0 and abs((tx.getBranch<LV>("lep0")+tx.getBranch<LV>("lep1")).mass() - 91.1876) < 15.)); // if ee channel and mll is on-Z
            tx.setBranch<int>("mee_noZ", mee_noZ);

            if (not mee_noZ) // reject events with dielectrons being on-Z (charge flip from likely Z+jets)
                return false;

            return pt0 > 40. and pt1 > 40.;

        },
        UNITY);

    //**************************************************
    // - Selecting Trigger and removing duplicate events
    //**************************************************
    cutflow.addCutToLastActiveCut("Trigger",
        [&]()
        {
            if (tx.getBranch<int>("lepchannel") <= 2)
            {
                bool is_pd_ee = looper.getCurrentFileName().Contains("DoubleEG") or looper.getCurrentFileName().Contains("EGamma");
                bool is_pd_em = looper.getCurrentFileName().Contains("MuonEG");
                bool is_pd_mm = looper.getCurrentFileName().Contains("DoubleMuon");
                bool pass_duplicate_ee_em_mm = false;
                bool pass_duplicate_mm_em_ee = false;

                bool trig_ee = false;
                bool trig_em = false;
                bool trig_mm = false;

                bool Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = false;
                bool Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL = false;
                bool Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 = false;
                bool Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = false;
                bool Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL = false;
                bool Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = false;
                bool Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = false;
                bool Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = false;
                bool Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = false;

                try { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ                 = nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ();                } catch (std::runtime_error) { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ                 = false; } // Lowest unprescaled
                try { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL                    = nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL();                   } catch (std::runtime_error) { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL                    = false; } // Lowest unprescaled
                try { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8         = nt.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8();        } catch (std::runtime_error) { Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8         = false; } // Lowest unprescaled for >= 2017C
                try { Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ           = nt.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ();          } catch (std::runtime_error) { Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ           = false; }
                try { Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL              = nt.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL();             } catch (std::runtime_error) { Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL              = false; } // Lowest unprescaled
                try { Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ  = nt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ(); } catch (std::runtime_error) { Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ  = false; } // Lowest unprescaled
                try { Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL     = nt.HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL();    } catch (std::runtime_error) { Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL     = false; }
                try { Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ   = nt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ();  } catch (std::runtime_error) { Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ   = false; } // Lowest unprescaled
                try { Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL      = nt.HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL();     } catch (std::runtime_error) { Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL      = false; }

                switch (nt.year())
                {
                    case 2016:
                        trig_ee = Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
                        trig_em = Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL or Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL or
                            Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ or Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
                        trig_mm = Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ or
                            Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL;
                        break;
                    case 2017:
                        trig_ee = Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
                        trig_em = Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ or Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
                        trig_mm = Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8;
                        break;
                    case 2018:
                        trig_ee = Common_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
                        trig_em = Common_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ or Common_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
                        trig_mm = Common_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8;
                        break;
                }

                if (is_pd_ee)
                {
                    if (trig_ee)
                        pass_duplicate_ee_em_mm = true;
                    if (not trig_mm and not trig_em and trig_ee)
                        pass_duplicate_mm_em_ee = true;
                }
                else if (is_pd_em)
                {
                    if (not trig_ee and trig_em)
                        pass_duplicate_ee_em_mm = true;
                    if (not trig_mm and trig_em)
                        pass_duplicate_mm_em_ee = true;
                }
                else if (is_pd_mm)
                {
                    if (not trig_ee and not trig_em and trig_mm)
                        pass_duplicate_ee_em_mm = true;
                    if (trig_mm)
                        pass_duplicate_mm_em_ee = true;
                }


                bool pass_trigger = trig_ee or trig_em or trig_mm;
                bool duplicate = nt.isData() ? pass_duplicate_ee_em_mm : 1.;
                return (pass_trigger and duplicate);
            }
            else if (tx.getBranch<int>("lepchannel") == 3)
            {
                bool is_pd = looper.getCurrentFileName().Contains("SingleElectron") or looper.getCurrentFileName().Contains("EGamma");
                bool Common_HLT_Ele27_WPTight_Gsf        = false;
                bool Common_HLT_Ele25_eta2p1_WPTight_Gsf = false;
                bool Common_HLT_Ele35_WPTight_Gsf        = false;
                bool Common_HLT_Ele32_WPTight_Gsf        = false;
                try { Common_HLT_Ele27_WPTight_Gsf        = nt.HLT_Ele27_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele27_WPTight_Gsf        = false; }
                try { Common_HLT_Ele25_eta2p1_WPTight_Gsf = nt.HLT_Ele25_eta2p1_WPTight_Gsf(); } catch (std::runtime_error) { Common_HLT_Ele25_eta2p1_WPTight_Gsf = false; }
                try { Common_HLT_Ele35_WPTight_Gsf        = nt.HLT_Ele35_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele35_WPTight_Gsf        = false; }
                try { Common_HLT_Ele32_WPTight_Gsf        = nt.HLT_Ele32_WPTight_Gsf();        } catch (std::runtime_error) { Common_HLT_Ele32_WPTight_Gsf        = false; }
                bool trig_se = false;
                switch (nt.year())
                {
                    case 2016:
                        trig_se = Common_HLT_Ele27_WPTight_Gsf or Common_HLT_Ele25_eta2p1_WPTight_Gsf;
                        break;
                    case 2017:
                        trig_se = Common_HLT_Ele35_WPTight_Gsf;
                        break;
                    case 2018:
                        trig_se = Common_HLT_Ele32_WPTight_Gsf;
                        break;
                }
                return nt.isData() ? (is_pd and trig_se) : trig_se;
            }
            else if (tx.getBranch<int>("lepchannel") == 4)
            {
                bool is_pd = looper.getCurrentFileName().Contains("SingleMuon");
                bool Common_HLT_IsoMu24   = false;
                bool Common_HLT_IsoTkMu24 = false;
                bool Common_HLT_IsoMu27   = false;
                try { Common_HLT_IsoMu24   = nt.HLT_IsoMu24();   } catch (std::runtime_error) { Common_HLT_IsoMu24   = false; }
                try { Common_HLT_IsoTkMu24 = nt.HLT_IsoTkMu24(); } catch (std::runtime_error) { Common_HLT_IsoTkMu24 = false; }
                try { Common_HLT_IsoMu27   = nt.HLT_IsoMu27();   } catch (std::runtime_error) { Common_HLT_IsoMu27   = false; }
                bool trig_sm = false;
                switch (nt.year())
                {
                    case 2016:
                        trig_sm = Common_HLT_IsoMu24 or Common_HLT_IsoTkMu24;
                        break;
                    case 2017:
                        trig_sm = Common_HLT_IsoMu27;
                        break;
                    case 2018:
                        trig_sm = Common_HLT_IsoMu24;
                        break;
                }
                return nt.isData() ? (is_pd and trig_sm) : trig_sm;
            }
            else
            {
                return false;
            }

        }, UNITY);

    //*****************************
    // - Select Good Reco Jets
    //*****************************
    // Description: Select Good Reco Jets
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

                if (nt.Jet_jetId()[ijet] < 2) // "Tight" ID requirement
                    continue;

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

                for (unsigned int itau = 0; itau < tx.getBranchLazy<vector<LV>>("good_taus_p4").size(); ++itau)
                {
                    if (RooUtil::Calc::DeltaR(tx.getBranchLazy<vector<LV>>("good_taus_p4").at(itau), jet_p4) < 0.4)
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

                // B-tagging is also done up to 2.4 in eta only
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

            // Loop over the jets
            for (unsigned int ifatjet = 0; ifatjet < nt.FatJet_pt().size(); ++ifatjet)
            {

                // Read jet p4
                const LV& jet_p4 = nt.FatJet_p4()[ifatjet];

                // Overlap check against good leptons
                bool isOverlap = false;
                for (unsigned int ilep = 0; ilep < tx.getBranch<vector<LV>>("good_leptons_p4").size(); ++ilep)
                {
                    if (RooUtil::Calc::DeltaR(tx.getBranchLazy<vector<LV>>("good_leptons_p4")[ilep], jet_p4) < 0.8)
                    {
                        isOverlap = true;
                        break;
                    }
                }

                for (unsigned int itau = 0; itau < tx.getBranchLazy<vector<LV>>("good_taus_p4").size(); ++itau)
                {
                    if (RooUtil::Calc::DeltaR(tx.getBranchLazy<vector<LV>>("good_taus_p4").at(itau), jet_p4) < 0.4)
                    {
                        isOverlap = true;
                        break;
                    }
                }

                // Then skip
                if (isOverlap)
                    continue;

                // FatJet pt preselection is 250 GeV as Hbb tagger SF starts at 250 GeV.
                if (not (jet_p4.pt() > 250.))
                    continue;

                bool is_loose_btagged = false;
                bool is_medium_btagged = false;
                bool is_tight_btagged = false;

                // B-tagging is also done up to 2.4 in eta only
                if (abs(jet_p4.eta()) < 2.4)
                {
                    // Check if it passes btagging
                    if (nt.year() == 2018)
                    {
                        is_loose_btagged = nt.FatJet_btagDDBvL()[ifatjet] > 0.7; // 85% after SF
                        is_medium_btagged = nt.FatJet_btagDDBvL()[ifatjet] > 0.86; // 55% after SF
                        is_tight_btagged = nt.FatJet_btagDDBvL()[ifatjet] > 0.91; // 30% after SF
                    }
                }

                tx.pushbackToBranch<LV>("good_fatjets_p4", jet_p4);
                tx.pushbackToBranch<int>("good_fatjets_loose", is_loose_btagged);
                tx.pushbackToBranch<int>("good_fatjets_medium", is_medium_btagged);
                tx.pushbackToBranch<int>("good_fatjets_tight", is_tight_btagged);
                tx.pushbackToBranch<float>("good_fatjets_msd", nt.FatJet_msoftdrop()[ifatjet]);


            }

            tx.sortVecBranchesByPt(
                    /* name of the 4vector branch to use to pt sort by*/               "good_fatjets_p4",
                    /* names of any associated vector<float> branches to sort along */ {"good_fatjets_msd"},
                    /* names of any associated vector<int>   branches to sort along */ {"good_fatjets_loose", "good_fatjets_medium", "good_fatjets_tight"},
                    /* names of any associated vector<bool>  branches to sort along */ {}
                    );

            return true;
        },
        UNITY);

    //*****************************
    // - Tag Hbb jets
    //*****************************
    // Description: Select the two b-tagged jets with highest btagging score as Hbb jets and require two highest bjets pass tight
    cutflow.addCutToLastActiveCut("AK4CategTagHiggsJets",
        [&]()
        {
           // If less than two jets skip entirely
           if (tx.getBranchLazy<vector<LV>>("good_jets_p4").size() < 2)
               return false;

            // get scores and indices pairs
            std::vector<std::pair<float, int>> btag_jets;
            for (unsigned int i = 0; i < tx.getBranch<vector<LV>>("good_jets_p4").size(); i++)
            {
                const LV& bp4 = tx.getBranch<vector<LV>>("good_jets_p4")[i];
                float btag_score = tx.getBranch<vector<float>>("good_jets_btag_score")[i];
                if (abs(bp4.eta()) > 2.4) // If the jet is outside of 2.4 set the btag score to -999 so that we don't choose them
                {
                    btag_score = -999.f;
                }
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

            // Set the tagged higgs jets
            tx.setBranch<LV>("b0", tx.getBranch<vector<LV>>("higgs_jets_p4")[0]);
            tx.setBranch<LV>("b1", tx.getBranch<vector<LV>>("higgs_jets_p4")[1]);
            float mbb = (tx.getBranch<LV>("b0")+tx.getBranch<LV>("b1")).mass();
            int mbbIn = mbb > 90. and mbb < 140.;
            int mbbOut = (not mbbIn);
            tx.setBranch<int>("pass_blind", nt.isData() ? mbbOut: 1.);
            tx.setBranch<int>("mbbIn", mbbIn);

            // Require at least one of them pass a tight and another pass loose
            const int& btight0 = tx.getBranch<vector<int>>("higgs_jets_tight_btagged")[0];
            const int& btight1 = tx.getBranch<vector<int>>("higgs_jets_tight_btagged")[1];
            const int& bloose0 = tx.getBranch<vector<int>>("higgs_jets_loose_btagged")[0];
            const int& bloose1 = tx.getBranch<vector<int>>("higgs_jets_loose_btagged")[1];
            int btagchannel = -999;
            if (btight0 and btight1)
                btagchannel = 0;
            else if ((btight0 and bloose1) or (bloose0 and btight1))
                btagchannel = 1;
            else
                btagchannel = -1;
            tx.setBranch<int>("btagchannel", btagchannel);

            if (bloose0 and bloose1)
                return true;
            else
                return false;
            // if (btagchannel == 0 or btagchannel == 1)
            //     return true;
            // else
            //     return false;

        }, UNITY);

    //*****************************
    // - Tag VBS jets
    //*****************************
    // Description: Select two jets that are not part of the Hbb jets to be VBS jets with following algorithm
    //              - If all jets are in same hemisphere (eta > 0 or eta < 0) then choose the two leading jets in P (N.B. not pt!)
    //              - If not choose the leading jet in each hemisphere leading in P (N.B. not pt!)
    cutflow.addCutToLastActiveCut("AK4CategTagVBSJets",
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
                // Set the vbs jets
                tx.setBranch<LV>("j0", good_jets_p4[vbs_jet_cands_idxs[0]]);
                tx.setBranch<LV>("j1", good_jets_p4[vbs_jet_cands_idxs[1]]);
                return true;
            }
            else
            {

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

                // Set the vbs jets
                tx.setBranch<LV>("j0", good_jets_p4[vbs_jet_idx_0]);
                tx.setBranch<LV>("j1", good_jets_p4[vbs_jet_idx_1]);
                return true;
            }

        },
        UNITY);

    cutflow.addCutToLastActiveCut("AK4CategPresel",
        [&]()
        {
            const int& pass_blind = tx.getBranch<int>("pass_blind");
            const int& btagchannel = tx.getBranch<int>("btagchannel");
            const int& lepchannel = tx.getBranch<int>("lepchannel");
            const int& mbbIn = tx.getBranch<int>("mbbIn");
            int channel = -1;
            if (pass_blind and btagchannel == 0 and lepchannel <= 2 and     mbbIn) channel = 0;
            if (pass_blind and btagchannel == 0 and lepchannel <= 2 and not mbbIn) channel = 1;
            if (pass_blind and btagchannel == 1 and lepchannel <= 2 and     mbbIn) channel = 2;
            if (pass_blind and btagchannel == 1 and lepchannel <= 2 and not mbbIn) channel = 3;
            if (pass_blind and btagchannel == 0 and lepchannel == 3 and     mbbIn) channel = 4;
            if (pass_blind and btagchannel == 0 and lepchannel == 3 and not mbbIn) channel = 5;
            if (pass_blind and btagchannel == 0 and lepchannel == 4 and     mbbIn) channel = 6;
            if (pass_blind and btagchannel == 0 and lepchannel == 4 and not mbbIn) channel = 7;
            tx.setBranch<int>("channel", channel);
            int channeldetail = -1;
            if (pass_blind and btagchannel == 0 and lepchannel == 0 and     mbbIn) channeldetail = 0;
            if (pass_blind and btagchannel == 0 and lepchannel == 0 and not mbbIn) channeldetail = 1;
            if (pass_blind and btagchannel == 0 and lepchannel == 1 and     mbbIn) channeldetail = 2;
            if (pass_blind and btagchannel == 0 and lepchannel == 1 and not mbbIn) channeldetail = 3;
            if (pass_blind and btagchannel == 0 and lepchannel == 2 and     mbbIn) channeldetail = 4;
            if (pass_blind and btagchannel == 0 and lepchannel == 2 and not mbbIn) channeldetail = 5;
            if (pass_blind and btagchannel == 1 and lepchannel == 0 and     mbbIn) channeldetail = 6;
            if (pass_blind and btagchannel == 1 and lepchannel == 0 and not mbbIn) channeldetail = 7;
            if (pass_blind and btagchannel == 1 and lepchannel == 1 and     mbbIn) channeldetail = 8;
            if (pass_blind and btagchannel == 1 and lepchannel == 1 and not mbbIn) channeldetail = 9;
            if (pass_blind and btagchannel == 1 and lepchannel == 2 and     mbbIn) channeldetail = 10;
            if (pass_blind and btagchannel == 1 and lepchannel == 2 and not mbbIn) channeldetail = 11;
            if (pass_blind and btagchannel == 0 and lepchannel == 3 and     mbbIn) channeldetail = 12;
            if (pass_blind and btagchannel == 0 and lepchannel == 3 and not mbbIn) channeldetail = 13;
            if (pass_blind and btagchannel == 0 and lepchannel == 4 and     mbbIn) channeldetail = 14;
            if (pass_blind and btagchannel == 0 and lepchannel == 4 and not mbbIn) channeldetail = 15;
            tx.setBranch<int>("channeldetail", channeldetail);
            return true;
            // if (channel < 0)
            //     return false;
            // else
            //     return true;
        }, UNITY);

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
