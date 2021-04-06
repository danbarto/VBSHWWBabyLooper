#include "Nano.h"
#include "rooutil.h"
#include "cxxopts.h"
#include "Base.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"

#include "VBSHWW.h"

#define VBSLV(name) vbs.tx.getBranch<LV>(name)
#define VBSINT(name) vbs.tx.getBranch<int>(name)

// ./process INPUTFILEPATH OUTPUTFILE [NEVENTS]
int main(int argc, char** argv)
{

    const float lep_pt0_threshold = 140;
    const float lep_pt1_threshold = 60;

    VBSHWW vbs(argc, argv);

    vbs.initSRCutflow();

    // Splitting events by channels
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("AllChannel"   , UNITY, UNITY);

    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightEEChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") == 0;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightEMChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") == 1;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightMMChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") == 2;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightETChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") == 3;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightMTChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") == 4;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightLLChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") >= 0 and VBSINT("lepchannel") <= 2; }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("TightLTChannel", [&]() { return VBSINT("btagchannel") == 0 and VBSINT("lepchannel") >= 3 and VBSINT("lepchannel") <= 4; }, UNITY);

    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("LooseEEChannel", [&]() { return VBSINT("btagchannel") == 1 and VBSINT("lepchannel") == 0;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("LooseEMChannel", [&]() { return VBSINT("btagchannel") == 1 and VBSINT("lepchannel") == 1;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("LooseMMChannel", [&]() { return VBSINT("btagchannel") == 1 and VBSINT("lepchannel") == 2;                               }, UNITY);
    vbs.cutflow.getCut("AK4CategPresel"); vbs.cutflow.addCutToLastActiveCut("LooseLLChannel", [&]() { return VBSINT("btagchannel") == 1 and VBSINT("lepchannel") >= 0 and VBSINT("lepchannel") <= 2; }, UNITY);

    // Selecting channels of interest
    std::vector<TString> channels =
        {
            // All channels
            "All", // All channels included

            // Main channels
            "TightLL", // Tight-Tight b-tag + lightlep + lightlep
            "LooseLL", // Tight-Loose b-tag + lightlep + lightlep
            "TightLT", // Tight-Tight b-tag + lightlep + tau
            "TightET", // Tight-Tight b-tag + electron + tau
            "TightMT", // Tight-Tight b-tag + muon     + tau
                       // TODO: Low Pt VBS jets

            // Sub-channels
            "TightEE", // Tight-Tight b-tag + electron + electron
            "LooseEE", // Tight-Loose b-tag + electron + electron
            "TightEM", // Tight-Tight b-tag + electron + muon
            "LooseEM", // Tight-Loose b-tag + electron + muon
            "TightMM", // Tight-Tight b-tag + muon     + muon
            "LooseMM", // Tight-Loose b-tag + muon     + muon
        };

    // Looping over channels of interest and adding kinematic selections
    for (auto& channel : channels)
    {
        vbs.cutflow.getCut(TString::Format("%sChannel", channel.Data()));
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSRMjj", channel.Data()), [&]() { return (VBSLV("j0")+VBSLV("j1")).mass() > 500.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSRLepPt0", channel.Data()), [&, lep_pt0_threshold]() { return VBSLV("leadlep").pt() > lep_pt0_threshold; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSRLepPt1", channel.Data()), [&, lep_pt1_threshold]() { return VBSLV("subllep").pt() > lep_pt1_threshold; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSRFit", channel.Data()), UNITY, UNITY); // Dummy cut to have a clean name for "Mbb fit" distribution
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSRMbb", channel.Data()), [&]() { return (VBSLV("b0")+VBSLV("b1")).mass() < 140. and (VBSLV("b0")+VBSLV("b1")).mass() > 90.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sSR", channel.Data()), UNITY, UNITY); // Dummy cut to have a clean name for final SR
    }

    for (auto& channel : channels)
    {
        vbs.cutflow.getCut(TString::Format("%sChannel", channel.Data()));
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCRMbb", channel.Data()), [&]() { return not ((VBSLV("b0")+VBSLV("b1")).mass() < 140. and (VBSLV("b0")+VBSLV("b1")).mass() > 90.); }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCRMjj", channel.Data()), [&]() { return (VBSLV("j0")+VBSLV("j1")).mass() > 500.; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCRLepPt0", channel.Data()), [&, lep_pt0_threshold]() { return VBSLV("leadlep").pt() > lep_pt0_threshold; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCRLepPt1", channel.Data()), [&, lep_pt1_threshold]() { return VBSLV("subllep").pt() > lep_pt1_threshold; }, UNITY );
        vbs.cutflow.addCutToLastActiveCut(TString::Format("%sCR", channel.Data()), UNITY, UNITY); // Dummy cut to have a clean name for final SR
    }

    // Print the cuts
    vbs.cutflow.printCuts();

    // Defining histograms
    vbs.histograms.addHistogram("NCenJet30"           , 9   , 0       , 9      , [&]() { return VBSINT("ncenjet30");                                                               } );
    vbs.histograms.addHistogram("NJet30"              , 9   , 0       , 9      , [&]() { return VBSINT("njet30");                                                                  } );
    vbs.histograms.addHistogram("NBLoose"             , 5   , 0       , 5      , [&]() { return VBSINT("nbloose");                                                                 } );
    vbs.histograms.addHistogram("NBMedium"            , 5   , 0       , 5      , [&]() { return VBSINT("nbmedium");                                                                } );
    vbs.histograms.addHistogram("NBTight"             , 5   , 0       , 5      , [&]() { return VBSINT("nbtight");                                                                 } );
    vbs.histograms.addHistogram("LeptonPt0"           , 180 , 0       , 600    , [&]() { return VBSLV("leadlep").pt();                                                             } );
    vbs.histograms.addHistogram("LeptonPt1"           , 180 , 0       , 600    , [&]() { return VBSLV("subllep").pt();                                                             } );
    vbs.histograms.addHistogram("LeptonPt0Zoom"       , 180 , 0       , 250    , [&]() { return VBSLV("leadlep").pt();                                                             } );
    vbs.histograms.addHistogram("LeptonPt1Zoom"       , 180 , 0       , 250    , [&]() { return VBSLV("subllep").pt();                                                             } );
    vbs.histograms.addHistogram("LeptonEta0"          , 180 , -3      , 3      , [&]() { return VBSLV("leadlep").eta();                                                            } );
    vbs.histograms.addHistogram("LeptonEta1"          , 180 , -3      , 3      , [&]() { return VBSLV("subllep").eta();                                                            } );
    vbs.histograms.addHistogram("LeptonPhi0"          , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("leadlep").phi();                                                            } );
    vbs.histograms.addHistogram("LeptonPhi1"          , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("subllep").phi();                                                            } );
    vbs.histograms.addHistogram("LeptonPtFlavor0"     , 180 , 0       , 600    , [&]() { return VBSLV("lep0").pt();                                                                } );
    vbs.histograms.addHistogram("LeptonPtFlavor1"     , 180 , 0       , 600    , [&]() { return VBSLV("lep1").pt();                                                                } );
    vbs.histograms.addHistogram("LeptonPtFlavor0Zoom" , 180 , 0       , 250    , [&]() { return VBSLV("lep0").pt();                                                                } );
    vbs.histograms.addHistogram("LeptonPtFlavor1Zoom" , 180 , 0       , 250    , [&]() { return VBSLV("lep1").pt();                                                                } );
    vbs.histograms.addHistogram("LeptonEtaFlavor0"    , 180 , -3      , 3      , [&]() { return VBSLV("lep0").eta();                                                               } );
    vbs.histograms.addHistogram("LeptonEtaFlavor1"    , 180 , -3      , 3      , [&]() { return VBSLV("lep1").eta();                                                               } );
    vbs.histograms.addHistogram("LeptonPhiFlavor0"    , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("lep0").phi();                                                               } );
    vbs.histograms.addHistogram("LeptonPhiFlavor1"    , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("lep1").phi();                                                               } );
    vbs.histograms.addHistogram("JetPt0"              , 180 , 0       , 250    , [&]() { return VBSLV("j0").pt();                                                                  } );
    vbs.histograms.addHistogram("JetPt1"              , 180 , 0       , 250    , [&]() { return VBSLV("j1").pt();                                                                  } );
    vbs.histograms.addHistogram("JetEta0"             , 180 , -5      , 5      , [&]() { return VBSLV("j0").eta();                                                                 } );
    vbs.histograms.addHistogram("JetEta1"             , 180 , -5      , 5      , [&]() { return VBSLV("j1").eta();                                                                 } );
    vbs.histograms.addHistogram("JetPhi0"             , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("j0").phi();                                                                 } );
    vbs.histograms.addHistogram("JetPhi1"             , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("j1").phi();                                                                 } );
    vbs.histograms.addHistogram("BJetPt0"             , 180 , 0       , 450    , [&]() { return VBSLV("b0").pt();                                                                  } );
    vbs.histograms.addHistogram("BJetPt1"             , 180 , 0       , 250    , [&]() { return VBSLV("b1").pt();                                                                  } );
    vbs.histograms.addHistogram("BJetEta0"            , 180 , -5      , 5      , [&]() { return VBSLV("b0").eta();                                                                 } );
    vbs.histograms.addHistogram("BJetEta1"            , 180 , -5      , 5      , [&]() { return VBSLV("b1").eta();                                                                 } );
    vbs.histograms.addHistogram("BJetPhi0"            , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("b0").phi();                                                                 } );
    vbs.histograms.addHistogram("BJetPhi1"            , 180 , -3.1416 , 3.1416 , [&]() { return VBSLV("b1").phi();                                                                 } );
    vbs.histograms.addHistogram("MJJ"                 , 180 , 0       , 4500   , [&]() { return (VBSLV("j0")+VBSLV("j1")).mass();                                                  } );
    vbs.histograms.addHistogram("MJJLow"              , 180 , 0       , 350    , [&]() { return (VBSLV("j0")+VBSLV("j1")).mass();                                                  } );
    vbs.histograms.addHistogram("DEtaJJ"              , 180 , 0       , 10     , [&]() { return fabs(RooUtil::Calc::DeltaEta(VBSLV("j0"), VBSLV("j1")));                           } );
    vbs.histograms.addHistogram("DPhiJJ"              , 180 , 0       , 3.1416 , [&]() { return fabs(RooUtil::Calc::DeltaPhi(VBSLV("j0"), VBSLV("j1")));                           } );
    vbs.histograms.addHistogram("HiggsPt"             , 180 , 0       , 1000   , [&]() { return (VBSLV("b0")+VBSLV("b1")).pt();                                                    } );
    vbs.histograms.addHistogram("HiggsPtLow"          , 180 , 0       , 350    , [&]() { return (VBSLV("b0")+VBSLV("b1")).pt();                                                    } );
    vbs.histograms.addHistogram("DPhill"              , 180 , 0       , 3.1416 , [&]() { return fabs(RooUtil::Calc::DeltaPhi(VBSLV("leadlep"), VBSLV("subllep")));                 } );
    vbs.histograms.addHistogram("DEtall"              , 180 , 0       , 7      , [&]() { return fabs(RooUtil::Calc::DeltaEta(VBSLV("leadlep"), VBSLV("subllep")));                 } );
    vbs.histograms.addHistogram("DRll"                , 180 , 0       , 7      , [&]() { return fabs(RooUtil::Calc::DeltaR(VBSLV("leadlep"), VBSLV("subllep")));                   } );
    vbs.histograms.addHistogram("Mll"                 , 180 , 0       , 2000   , [&]() { return (VBSLV("leadlep")+VBSLV("subllep")).mass();                                        } );
    vbs.histograms.addHistogram("MllLow"              , 180 , 0       , 250    , [&]() { return (VBSLV("leadlep")+VBSLV("subllep")).mass();                                        } );
    vbs.histograms.addHistogram("Mbb"                 , 180 , -10     , 290    , [&]() { return (VBSLV("b0")+VBSLV("b1")).mass();                                                  } );
    vbs.histograms.addHistogram("MbbInOut"            , 2   , 0       , 2      , [&]() { return (VBSLV("b0")+VBSLV("b1")).mass() < 140 and (VBSLV("b0")+VBSLV("b1")).mass() > 90.; } );
    vbs.histograms.addHistogram("DRbb"                , 180 , 0       , 10     , [&]() { return RooUtil::Calc::DeltaR(VBSLV("b0"), VBSLV("b1"));                                   } );
    vbs.histograms.addHistogram("MET"                 , 180 , 0       , 1500   , [&]() { return VBSLV("met_p4").pt();                                                              } );
    vbs.histograms.addHistogram("METLow"              , 180 , 0       , 250    , [&]() { return VBSLV("met_p4").pt();                                                              } );
    vbs.histograms.addHistogram("Channels"            , 8   , 0       , 8      , [&]() { return VBSINT("channel");                                                                  } );

    // Book the histograms
    vbs.cutflow.bookHistogramsForCutAndBelow(vbs.histograms, "AK4CategPresel");
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

        vbs.process("AK4CategPresel");

    }

    // Writing output file
    vbs.cutflow.saveOutput();

    // Write the data structure to the root file
    vbs.tx.write();

    // Write out the "run:lumi:evt" of the events passing a certain cut into a text file
    // If the output.root is "output/path/dir/name.root"
    // then the text file will be named "output/path/dir/name_CutName.txt"
    vbs.writeEventList("AK4CategPresel");

    // The below can be sometimes crucial
    delete vbs.output_tfile;
}
