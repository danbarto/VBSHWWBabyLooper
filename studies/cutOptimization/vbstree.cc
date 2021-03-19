#include "vbstree.h"
vbstree vbs;

void vbstree::Init(TTree *tree) {
  met_p4_branch = tree->GetBranch("met_p4");
  if (met_p4_branch) met_p4_branch->SetAddress(&met_p4_);
  good_leptons_p4_branch = tree->GetBranch("good_leptons_p4");
  if (good_leptons_p4_branch) good_leptons_p4_branch->SetAddress(&good_leptons_p4_);
  good_taus_p4_branch = tree->GetBranch("good_taus_p4");
  if (good_taus_p4_branch) good_taus_p4_branch->SetAddress(&good_taus_p4_);
  good_jets_p4_branch = tree->GetBranch("good_jets_p4");
  if (good_jets_p4_branch) good_jets_p4_branch->SetAddress(&good_jets_p4_);
  higgs_jets_p4_branch = tree->GetBranch("higgs_jets_p4");
  if (higgs_jets_p4_branch) higgs_jets_p4_branch->SetAddress(&higgs_jets_p4_);
  vbs_jets_p4_branch = tree->GetBranch("vbs_jets_p4");
  if (vbs_jets_p4_branch) vbs_jets_p4_branch->SetAddress(&vbs_jets_p4_);

  tree->SetMakeClass(1);

  run_branch = tree->GetBranch("run");
  if (run_branch) run_branch->SetAddress(&run_);
  lumi_branch = tree->GetBranch("lumi");
  if (lumi_branch) lumi_branch->SetAddress(&lumi_);
  evt_branch = tree->GetBranch("evt");
  if (evt_branch) evt_branch->SetAddress(&evt_);
  wgt_branch = tree->GetBranch("wgt");
  if (wgt_branch) wgt_branch->SetAddress(&wgt_);
  good_leptons_pdgid_branch = tree->GetBranch("good_leptons_pdgid");
  if (good_leptons_pdgid_branch) good_leptons_pdgid_branch->SetAddress(&good_leptons_pdgid_);
  good_leptons_tight_branch = tree->GetBranch("good_leptons_tight");
  if (good_leptons_tight_branch) good_leptons_tight_branch->SetAddress(&good_leptons_tight_);
  good_leptons_jetIdx_branch = tree->GetBranch("good_leptons_jetIdx");
  if (good_leptons_jetIdx_branch) good_leptons_jetIdx_branch->SetAddress(&good_leptons_jetIdx_);
  good_leptons_pfRelIso03_all_branch = tree->GetBranch("good_leptons_pfRelIso03_all");
  if (good_leptons_pfRelIso03_all_branch) good_leptons_pfRelIso03_all_branch->SetAddress(&good_leptons_pfRelIso03_all_);
  good_leptons_pfRelIso03_chg_branch = tree->GetBranch("good_leptons_pfRelIso03_chg");
  if (good_leptons_pfRelIso03_chg_branch) good_leptons_pfRelIso03_chg_branch->SetAddress(&good_leptons_pfRelIso03_chg_);
  good_leptons_jetPtRelv2_branch = tree->GetBranch("good_leptons_jetPtRelv2");
  if (good_leptons_jetPtRelv2_branch) good_leptons_jetPtRelv2_branch->SetAddress(&good_leptons_jetPtRelv2_);
  good_leptons_jetRelIso_branch = tree->GetBranch("good_leptons_jetRelIso");
  if (good_leptons_jetRelIso_branch) good_leptons_jetRelIso_branch->SetAddress(&good_leptons_jetRelIso_);
  good_leptons_miniPFRelIso_all_branch = tree->GetBranch("good_leptons_miniPFRelIso_all");
  if (good_leptons_miniPFRelIso_all_branch) good_leptons_miniPFRelIso_all_branch->SetAddress(&good_leptons_miniPFRelIso_all_);
  good_taus_pdgid_branch = tree->GetBranch("good_taus_pdgid");
  if (good_taus_pdgid_branch) good_taus_pdgid_branch->SetAddress(&good_taus_pdgid_);
  good_taus_tight_branch = tree->GetBranch("good_taus_tight");
  if (good_taus_tight_branch) good_taus_tight_branch->SetAddress(&good_taus_tight_);
  good_taus_jetIdx_branch = tree->GetBranch("good_taus_jetIdx");
  if (good_taus_jetIdx_branch) good_taus_jetIdx_branch->SetAddress(&good_taus_jetIdx_);
  good_taus_genPartFlav_branch = tree->GetBranch("good_taus_genPartFlav");
  if (good_taus_genPartFlav_branch) good_taus_genPartFlav_branch->SetAddress(&good_taus_genPartFlav_);
  good_jets_loose_btagged_branch = tree->GetBranch("good_jets_loose_btagged");
  if (good_jets_loose_btagged_branch) good_jets_loose_btagged_branch->SetAddress(&good_jets_loose_btagged_);
  good_jets_medium_btagged_branch = tree->GetBranch("good_jets_medium_btagged");
  if (good_jets_medium_btagged_branch) good_jets_medium_btagged_branch->SetAddress(&good_jets_medium_btagged_);
  good_jets_tight_btagged_branch = tree->GetBranch("good_jets_tight_btagged");
  if (good_jets_tight_btagged_branch) good_jets_tight_btagged_branch->SetAddress(&good_jets_tight_btagged_);
  good_jets_btag_score_branch = tree->GetBranch("good_jets_btag_score");
  if (good_jets_btag_score_branch) good_jets_btag_score_branch->SetAddress(&good_jets_btag_score_);
  good_jets_qg_disc_branch = tree->GetBranch("good_jets_qg_disc");
  if (good_jets_qg_disc_branch) good_jets_qg_disc_branch->SetAddress(&good_jets_qg_disc_);
  higgs_jets_loose_btagged_branch = tree->GetBranch("higgs_jets_loose_btagged");
  if (higgs_jets_loose_btagged_branch) higgs_jets_loose_btagged_branch->SetAddress(&higgs_jets_loose_btagged_);
  higgs_jets_medium_btagged_branch = tree->GetBranch("higgs_jets_medium_btagged");
  if (higgs_jets_medium_btagged_branch) higgs_jets_medium_btagged_branch->SetAddress(&higgs_jets_medium_btagged_);
  higgs_jets_tight_btagged_branch = tree->GetBranch("higgs_jets_tight_btagged");
  if (higgs_jets_tight_btagged_branch) higgs_jets_tight_btagged_branch->SetAddress(&higgs_jets_tight_btagged_);
  higgs_jets_btag_score_branch = tree->GetBranch("higgs_jets_btag_score");
  if (higgs_jets_btag_score_branch) higgs_jets_btag_score_branch->SetAddress(&higgs_jets_btag_score_);
  higgs_jets_good_jets_idx_branch = tree->GetBranch("higgs_jets_good_jets_idx");
  if (higgs_jets_good_jets_idx_branch) higgs_jets_good_jets_idx_branch->SetAddress(&higgs_jets_good_jets_idx_);
  vbs_jets_good_jets_idx_branch = tree->GetBranch("vbs_jets_good_jets_idx");
  if (vbs_jets_good_jets_idx_branch) vbs_jets_good_jets_idx_branch->SetAddress(&vbs_jets_good_jets_idx_);
  nbloose_branch = tree->GetBranch("nbloose");
  if (nbloose_branch) nbloose_branch->SetAddress(&nbloose_);
  nbmedium_branch = tree->GetBranch("nbmedium");
  if (nbmedium_branch) nbmedium_branch->SetAddress(&nbmedium_);
  nbtight_branch = tree->GetBranch("nbtight");
  if (nbtight_branch) nbtight_branch->SetAddress(&nbtight_);
  ncenjet30_branch = tree->GetBranch("ncenjet30");
  if (ncenjet30_branch) ncenjet30_branch->SetAddress(&ncenjet30_);
  njet30_branch = tree->GetBranch("njet30");
  if (njet30_branch) njet30_branch->SetAddress(&njet30_);
  leppt0_branch = tree->GetBranch("leppt0");
  if (leppt0_branch) leppt0_branch->SetAddress(&leppt0_);
  leppt1_branch = tree->GetBranch("leppt1");
  if (leppt1_branch) leppt1_branch->SetAddress(&leppt1_);
  taupt_branch = tree->GetBranch("taupt");
  if (taupt_branch) taupt_branch->SetAddress(&taupt_);
  channel_branch = tree->GetBranch("channel");
  if (channel_branch) channel_branch->SetAddress(&channel_);
  mjj_branch = tree->GetBranch("mjj");
  if (mjj_branch) mjj_branch->SetAddress(&mjj_);
  ptjj_branch = tree->GetBranch("ptjj");
  if (ptjj_branch) ptjj_branch->SetAddress(&ptjj_);
  detajj_branch = tree->GetBranch("detajj");
  if (detajj_branch) detajj_branch->SetAddress(&detajj_);
  dphijj_branch = tree->GetBranch("dphijj");
  if (dphijj_branch) dphijj_branch->SetAddress(&dphijj_);
  mbb_branch = tree->GetBranch("mbb");
  if (mbb_branch) mbb_branch->SetAddress(&mbb_);
  ptbb_branch = tree->GetBranch("ptbb");
  if (ptbb_branch) ptbb_branch->SetAddress(&ptbb_);
  dphibb_branch = tree->GetBranch("dphibb");
  if (dphibb_branch) dphibb_branch->SetAddress(&dphibb_);
  drbb_branch = tree->GetBranch("drbb");
  if (drbb_branch) drbb_branch->SetAddress(&drbb_);
  mll_branch = tree->GetBranch("mll");
  if (mll_branch) mll_branch->SetAddress(&mll_);
  ptll_branch = tree->GetBranch("ptll");
  if (ptll_branch) ptll_branch->SetAddress(&ptll_);
  dphill_branch = tree->GetBranch("dphill");
  if (dphill_branch) dphill_branch->SetAddress(&dphill_);
  drll_branch = tree->GetBranch("drll");
  if (drll_branch) drll_branch->SetAddress(&drll_);
  st_branch = tree->GetBranch("st");
  if (st_branch) st_branch->SetAddress(&st_);
  mtllbbmet_branch = tree->GetBranch("mtllbbmet");
  if (mtllbbmet_branch) mtllbbmet_branch->SetAddress(&mtllbbmet_);
  mllbbmet_branch = tree->GetBranch("mllbbmet");
  if (mllbbmet_branch) mllbbmet_branch->SetAddress(&mllbbmet_);

  tree->SetMakeClass(0);
}

void vbstree::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  run_isLoaded = false;
  lumi_isLoaded = false;
  evt_isLoaded = false;
  wgt_isLoaded = false;
  met_p4_isLoaded = false;
  good_leptons_p4_isLoaded = false;
  good_leptons_pdgid_isLoaded = false;
  good_leptons_tight_isLoaded = false;
  good_leptons_jetIdx_isLoaded = false;
  good_leptons_pfRelIso03_all_isLoaded = false;
  good_leptons_pfRelIso03_chg_isLoaded = false;
  good_leptons_jetPtRelv2_isLoaded = false;
  good_leptons_jetRelIso_isLoaded = false;
  good_leptons_miniPFRelIso_all_isLoaded = false;
  good_taus_p4_isLoaded = false;
  good_taus_pdgid_isLoaded = false;
  good_taus_tight_isLoaded = false;
  good_taus_jetIdx_isLoaded = false;
  good_taus_genPartFlav_isLoaded = false;
  good_jets_p4_isLoaded = false;
  good_jets_loose_btagged_isLoaded = false;
  good_jets_medium_btagged_isLoaded = false;
  good_jets_tight_btagged_isLoaded = false;
  good_jets_btag_score_isLoaded = false;
  good_jets_qg_disc_isLoaded = false;
  higgs_jets_p4_isLoaded = false;
  higgs_jets_loose_btagged_isLoaded = false;
  higgs_jets_medium_btagged_isLoaded = false;
  higgs_jets_tight_btagged_isLoaded = false;
  higgs_jets_btag_score_isLoaded = false;
  higgs_jets_good_jets_idx_isLoaded = false;
  vbs_jets_p4_isLoaded = false;
  vbs_jets_good_jets_idx_isLoaded = false;
  nbloose_isLoaded = false;
  nbmedium_isLoaded = false;
  nbtight_isLoaded = false;
  ncenjet30_isLoaded = false;
  njet30_isLoaded = false;
  leppt0_isLoaded = false;
  leppt1_isLoaded = false;
  taupt_isLoaded = false;
  channel_isLoaded = false;
  mjj_isLoaded = false;
  ptjj_isLoaded = false;
  detajj_isLoaded = false;
  dphijj_isLoaded = false;
  mbb_isLoaded = false;
  ptbb_isLoaded = false;
  dphibb_isLoaded = false;
  drbb_isLoaded = false;
  mll_isLoaded = false;
  ptll_isLoaded = false;
  dphill_isLoaded = false;
  drll_isLoaded = false;
  st_isLoaded = false;
  mtllbbmet_isLoaded = false;
  mllbbmet_isLoaded = false;
}

void vbstree::LoadAllBranches() {
  // load all branches
  if (run_branch != 0) run();
  if (lumi_branch != 0) lumi();
  if (evt_branch != 0) evt();
  if (wgt_branch != 0) wgt();
  if (met_p4_branch != 0) met_p4();
  if (good_leptons_p4_branch != 0) good_leptons_p4();
  if (good_leptons_pdgid_branch != 0) good_leptons_pdgid();
  if (good_leptons_tight_branch != 0) good_leptons_tight();
  if (good_leptons_jetIdx_branch != 0) good_leptons_jetIdx();
  if (good_leptons_pfRelIso03_all_branch != 0) good_leptons_pfRelIso03_all();
  if (good_leptons_pfRelIso03_chg_branch != 0) good_leptons_pfRelIso03_chg();
  if (good_leptons_jetPtRelv2_branch != 0) good_leptons_jetPtRelv2();
  if (good_leptons_jetRelIso_branch != 0) good_leptons_jetRelIso();
  if (good_leptons_miniPFRelIso_all_branch != 0) good_leptons_miniPFRelIso_all();
  if (good_taus_p4_branch != 0) good_taus_p4();
  if (good_taus_pdgid_branch != 0) good_taus_pdgid();
  if (good_taus_tight_branch != 0) good_taus_tight();
  if (good_taus_jetIdx_branch != 0) good_taus_jetIdx();
  if (good_taus_genPartFlav_branch != 0) good_taus_genPartFlav();
  if (good_jets_p4_branch != 0) good_jets_p4();
  if (good_jets_loose_btagged_branch != 0) good_jets_loose_btagged();
  if (good_jets_medium_btagged_branch != 0) good_jets_medium_btagged();
  if (good_jets_tight_btagged_branch != 0) good_jets_tight_btagged();
  if (good_jets_btag_score_branch != 0) good_jets_btag_score();
  if (good_jets_qg_disc_branch != 0) good_jets_qg_disc();
  if (higgs_jets_p4_branch != 0) higgs_jets_p4();
  if (higgs_jets_loose_btagged_branch != 0) higgs_jets_loose_btagged();
  if (higgs_jets_medium_btagged_branch != 0) higgs_jets_medium_btagged();
  if (higgs_jets_tight_btagged_branch != 0) higgs_jets_tight_btagged();
  if (higgs_jets_btag_score_branch != 0) higgs_jets_btag_score();
  if (higgs_jets_good_jets_idx_branch != 0) higgs_jets_good_jets_idx();
  if (vbs_jets_p4_branch != 0) vbs_jets_p4();
  if (vbs_jets_good_jets_idx_branch != 0) vbs_jets_good_jets_idx();
  if (nbloose_branch != 0) nbloose();
  if (nbmedium_branch != 0) nbmedium();
  if (nbtight_branch != 0) nbtight();
  if (ncenjet30_branch != 0) ncenjet30();
  if (njet30_branch != 0) njet30();
  if (leppt0_branch != 0) leppt0();
  if (leppt1_branch != 0) leppt1();
  if (taupt_branch != 0) taupt();
  if (channel_branch != 0) channel();
  if (mjj_branch != 0) mjj();
  if (ptjj_branch != 0) ptjj();
  if (detajj_branch != 0) detajj();
  if (dphijj_branch != 0) dphijj();
  if (mbb_branch != 0) mbb();
  if (ptbb_branch != 0) ptbb();
  if (dphibb_branch != 0) dphibb();
  if (drbb_branch != 0) drbb();
  if (mll_branch != 0) mll();
  if (ptll_branch != 0) ptll();
  if (dphill_branch != 0) dphill();
  if (drll_branch != 0) drll();
  if (st_branch != 0) st();
  if (mtllbbmet_branch != 0) mtllbbmet();
  if (mllbbmet_branch != 0) mllbbmet();
}

const int &vbstree::run() {
  if (not run_isLoaded) {
    if (run_branch != 0) {
      run_branch->GetEntry(index);
    } else {
      printf("branch run_branch does not exist!\n");
      exit(1);
    }
    run_isLoaded = true;
  }
  return run_;
}

const int &vbstree::lumi() {
  if (not lumi_isLoaded) {
    if (lumi_branch != 0) {
      lumi_branch->GetEntry(index);
    } else {
      printf("branch lumi_branch does not exist!\n");
      exit(1);
    }
    lumi_isLoaded = true;
  }
  return lumi_;
}

const unsigned long long &vbstree::evt() {
  if (not evt_isLoaded) {
    if (evt_branch != 0) {
      evt_branch->GetEntry(index);
    } else {
      printf("branch evt_branch does not exist!\n");
      exit(1);
    }
    evt_isLoaded = true;
  }
  return evt_;
}

const float &vbstree::wgt() {
  if (not wgt_isLoaded) {
    if (wgt_branch != 0) {
      wgt_branch->GetEntry(index);
    } else {
      printf("branch wgt_branch does not exist!\n");
      exit(1);
    }
    wgt_isLoaded = true;
  }
  return wgt_;
}

const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &vbstree::met_p4() {
  if (not met_p4_isLoaded) {
    if (met_p4_branch != 0) {
      met_p4_branch->GetEntry(index);
    } else {
      printf("branch met_p4_branch does not exist!\n");
      exit(1);
    }
    met_p4_isLoaded = true;
  }
  return *met_p4_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbstree::good_leptons_p4() {
  if (not good_leptons_p4_isLoaded) {
    if (good_leptons_p4_branch != 0) {
      good_leptons_p4_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_p4_branch does not exist!\n");
      exit(1);
    }
    good_leptons_p4_isLoaded = true;
  }
  return *good_leptons_p4_;
}

const vector<int> &vbstree::good_leptons_pdgid() {
  if (not good_leptons_pdgid_isLoaded) {
    if (good_leptons_pdgid_branch != 0) {
      good_leptons_pdgid_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_pdgid_branch does not exist!\n");
      exit(1);
    }
    good_leptons_pdgid_isLoaded = true;
  }
  return *good_leptons_pdgid_;
}

const vector<int> &vbstree::good_leptons_tight() {
  if (not good_leptons_tight_isLoaded) {
    if (good_leptons_tight_branch != 0) {
      good_leptons_tight_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_tight_branch does not exist!\n");
      exit(1);
    }
    good_leptons_tight_isLoaded = true;
  }
  return *good_leptons_tight_;
}

const vector<int> &vbstree::good_leptons_jetIdx() {
  if (not good_leptons_jetIdx_isLoaded) {
    if (good_leptons_jetIdx_branch != 0) {
      good_leptons_jetIdx_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_jetIdx_branch does not exist!\n");
      exit(1);
    }
    good_leptons_jetIdx_isLoaded = true;
  }
  return *good_leptons_jetIdx_;
}

const vector<float> &vbstree::good_leptons_pfRelIso03_all() {
  if (not good_leptons_pfRelIso03_all_isLoaded) {
    if (good_leptons_pfRelIso03_all_branch != 0) {
      good_leptons_pfRelIso03_all_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_pfRelIso03_all_branch does not exist!\n");
      exit(1);
    }
    good_leptons_pfRelIso03_all_isLoaded = true;
  }
  return *good_leptons_pfRelIso03_all_;
}

const vector<float> &vbstree::good_leptons_pfRelIso03_chg() {
  if (not good_leptons_pfRelIso03_chg_isLoaded) {
    if (good_leptons_pfRelIso03_chg_branch != 0) {
      good_leptons_pfRelIso03_chg_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_pfRelIso03_chg_branch does not exist!\n");
      exit(1);
    }
    good_leptons_pfRelIso03_chg_isLoaded = true;
  }
  return *good_leptons_pfRelIso03_chg_;
}

const vector<float> &vbstree::good_leptons_jetPtRelv2() {
  if (not good_leptons_jetPtRelv2_isLoaded) {
    if (good_leptons_jetPtRelv2_branch != 0) {
      good_leptons_jetPtRelv2_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_jetPtRelv2_branch does not exist!\n");
      exit(1);
    }
    good_leptons_jetPtRelv2_isLoaded = true;
  }
  return *good_leptons_jetPtRelv2_;
}

const vector<float> &vbstree::good_leptons_jetRelIso() {
  if (not good_leptons_jetRelIso_isLoaded) {
    if (good_leptons_jetRelIso_branch != 0) {
      good_leptons_jetRelIso_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_jetRelIso_branch does not exist!\n");
      exit(1);
    }
    good_leptons_jetRelIso_isLoaded = true;
  }
  return *good_leptons_jetRelIso_;
}

const vector<float> &vbstree::good_leptons_miniPFRelIso_all() {
  if (not good_leptons_miniPFRelIso_all_isLoaded) {
    if (good_leptons_miniPFRelIso_all_branch != 0) {
      good_leptons_miniPFRelIso_all_branch->GetEntry(index);
    } else {
      printf("branch good_leptons_miniPFRelIso_all_branch does not exist!\n");
      exit(1);
    }
    good_leptons_miniPFRelIso_all_isLoaded = true;
  }
  return *good_leptons_miniPFRelIso_all_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbstree::good_taus_p4() {
  if (not good_taus_p4_isLoaded) {
    if (good_taus_p4_branch != 0) {
      good_taus_p4_branch->GetEntry(index);
    } else {
      printf("branch good_taus_p4_branch does not exist!\n");
      exit(1);
    }
    good_taus_p4_isLoaded = true;
  }
  return *good_taus_p4_;
}

const vector<int> &vbstree::good_taus_pdgid() {
  if (not good_taus_pdgid_isLoaded) {
    if (good_taus_pdgid_branch != 0) {
      good_taus_pdgid_branch->GetEntry(index);
    } else {
      printf("branch good_taus_pdgid_branch does not exist!\n");
      exit(1);
    }
    good_taus_pdgid_isLoaded = true;
  }
  return *good_taus_pdgid_;
}

const vector<int> &vbstree::good_taus_tight() {
  if (not good_taus_tight_isLoaded) {
    if (good_taus_tight_branch != 0) {
      good_taus_tight_branch->GetEntry(index);
    } else {
      printf("branch good_taus_tight_branch does not exist!\n");
      exit(1);
    }
    good_taus_tight_isLoaded = true;
  }
  return *good_taus_tight_;
}

const vector<int> &vbstree::good_taus_jetIdx() {
  if (not good_taus_jetIdx_isLoaded) {
    if (good_taus_jetIdx_branch != 0) {
      good_taus_jetIdx_branch->GetEntry(index);
    } else {
      printf("branch good_taus_jetIdx_branch does not exist!\n");
      exit(1);
    }
    good_taus_jetIdx_isLoaded = true;
  }
  return *good_taus_jetIdx_;
}

const vector<int> &vbstree::good_taus_genPartFlav() {
  if (not good_taus_genPartFlav_isLoaded) {
    if (good_taus_genPartFlav_branch != 0) {
      good_taus_genPartFlav_branch->GetEntry(index);
    } else {
      printf("branch good_taus_genPartFlav_branch does not exist!\n");
      exit(1);
    }
    good_taus_genPartFlav_isLoaded = true;
  }
  return *good_taus_genPartFlav_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbstree::good_jets_p4() {
  if (not good_jets_p4_isLoaded) {
    if (good_jets_p4_branch != 0) {
      good_jets_p4_branch->GetEntry(index);
    } else {
      printf("branch good_jets_p4_branch does not exist!\n");
      exit(1);
    }
    good_jets_p4_isLoaded = true;
  }
  return *good_jets_p4_;
}

const vector<int> &vbstree::good_jets_loose_btagged() {
  if (not good_jets_loose_btagged_isLoaded) {
    if (good_jets_loose_btagged_branch != 0) {
      good_jets_loose_btagged_branch->GetEntry(index);
    } else {
      printf("branch good_jets_loose_btagged_branch does not exist!\n");
      exit(1);
    }
    good_jets_loose_btagged_isLoaded = true;
  }
  return *good_jets_loose_btagged_;
}

const vector<int> &vbstree::good_jets_medium_btagged() {
  if (not good_jets_medium_btagged_isLoaded) {
    if (good_jets_medium_btagged_branch != 0) {
      good_jets_medium_btagged_branch->GetEntry(index);
    } else {
      printf("branch good_jets_medium_btagged_branch does not exist!\n");
      exit(1);
    }
    good_jets_medium_btagged_isLoaded = true;
  }
  return *good_jets_medium_btagged_;
}

const vector<int> &vbstree::good_jets_tight_btagged() {
  if (not good_jets_tight_btagged_isLoaded) {
    if (good_jets_tight_btagged_branch != 0) {
      good_jets_tight_btagged_branch->GetEntry(index);
    } else {
      printf("branch good_jets_tight_btagged_branch does not exist!\n");
      exit(1);
    }
    good_jets_tight_btagged_isLoaded = true;
  }
  return *good_jets_tight_btagged_;
}

const vector<float> &vbstree::good_jets_btag_score() {
  if (not good_jets_btag_score_isLoaded) {
    if (good_jets_btag_score_branch != 0) {
      good_jets_btag_score_branch->GetEntry(index);
    } else {
      printf("branch good_jets_btag_score_branch does not exist!\n");
      exit(1);
    }
    good_jets_btag_score_isLoaded = true;
  }
  return *good_jets_btag_score_;
}

const vector<float> &vbstree::good_jets_qg_disc() {
  if (not good_jets_qg_disc_isLoaded) {
    if (good_jets_qg_disc_branch != 0) {
      good_jets_qg_disc_branch->GetEntry(index);
    } else {
      printf("branch good_jets_qg_disc_branch does not exist!\n");
      exit(1);
    }
    good_jets_qg_disc_isLoaded = true;
  }
  return *good_jets_qg_disc_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbstree::higgs_jets_p4() {
  if (not higgs_jets_p4_isLoaded) {
    if (higgs_jets_p4_branch != 0) {
      higgs_jets_p4_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_p4_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_p4_isLoaded = true;
  }
  return *higgs_jets_p4_;
}

const vector<int> &vbstree::higgs_jets_loose_btagged() {
  if (not higgs_jets_loose_btagged_isLoaded) {
    if (higgs_jets_loose_btagged_branch != 0) {
      higgs_jets_loose_btagged_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_loose_btagged_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_loose_btagged_isLoaded = true;
  }
  return *higgs_jets_loose_btagged_;
}

const vector<int> &vbstree::higgs_jets_medium_btagged() {
  if (not higgs_jets_medium_btagged_isLoaded) {
    if (higgs_jets_medium_btagged_branch != 0) {
      higgs_jets_medium_btagged_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_medium_btagged_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_medium_btagged_isLoaded = true;
  }
  return *higgs_jets_medium_btagged_;
}

const vector<int> &vbstree::higgs_jets_tight_btagged() {
  if (not higgs_jets_tight_btagged_isLoaded) {
    if (higgs_jets_tight_btagged_branch != 0) {
      higgs_jets_tight_btagged_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_tight_btagged_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_tight_btagged_isLoaded = true;
  }
  return *higgs_jets_tight_btagged_;
}

const vector<float> &vbstree::higgs_jets_btag_score() {
  if (not higgs_jets_btag_score_isLoaded) {
    if (higgs_jets_btag_score_branch != 0) {
      higgs_jets_btag_score_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_btag_score_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_btag_score_isLoaded = true;
  }
  return *higgs_jets_btag_score_;
}

const vector<int> &vbstree::higgs_jets_good_jets_idx() {
  if (not higgs_jets_good_jets_idx_isLoaded) {
    if (higgs_jets_good_jets_idx_branch != 0) {
      higgs_jets_good_jets_idx_branch->GetEntry(index);
    } else {
      printf("branch higgs_jets_good_jets_idx_branch does not exist!\n");
      exit(1);
    }
    higgs_jets_good_jets_idx_isLoaded = true;
  }
  return *higgs_jets_good_jets_idx_;
}

const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbstree::vbs_jets_p4() {
  if (not vbs_jets_p4_isLoaded) {
    if (vbs_jets_p4_branch != 0) {
      vbs_jets_p4_branch->GetEntry(index);
    } else {
      printf("branch vbs_jets_p4_branch does not exist!\n");
      exit(1);
    }
    vbs_jets_p4_isLoaded = true;
  }
  return *vbs_jets_p4_;
}

const vector<int> &vbstree::vbs_jets_good_jets_idx() {
  if (not vbs_jets_good_jets_idx_isLoaded) {
    if (vbs_jets_good_jets_idx_branch != 0) {
      vbs_jets_good_jets_idx_branch->GetEntry(index);
    } else {
      printf("branch vbs_jets_good_jets_idx_branch does not exist!\n");
      exit(1);
    }
    vbs_jets_good_jets_idx_isLoaded = true;
  }
  return *vbs_jets_good_jets_idx_;
}

const int &vbstree::nbloose() {
  if (not nbloose_isLoaded) {
    if (nbloose_branch != 0) {
      nbloose_branch->GetEntry(index);
    } else {
      printf("branch nbloose_branch does not exist!\n");
      exit(1);
    }
    nbloose_isLoaded = true;
  }
  return nbloose_;
}

const int &vbstree::nbmedium() {
  if (not nbmedium_isLoaded) {
    if (nbmedium_branch != 0) {
      nbmedium_branch->GetEntry(index);
    } else {
      printf("branch nbmedium_branch does not exist!\n");
      exit(1);
    }
    nbmedium_isLoaded = true;
  }
  return nbmedium_;
}

const int &vbstree::nbtight() {
  if (not nbtight_isLoaded) {
    if (nbtight_branch != 0) {
      nbtight_branch->GetEntry(index);
    } else {
      printf("branch nbtight_branch does not exist!\n");
      exit(1);
    }
    nbtight_isLoaded = true;
  }
  return nbtight_;
}

const int &vbstree::ncenjet30() {
  if (not ncenjet30_isLoaded) {
    if (ncenjet30_branch != 0) {
      ncenjet30_branch->GetEntry(index);
    } else {
      printf("branch ncenjet30_branch does not exist!\n");
      exit(1);
    }
    ncenjet30_isLoaded = true;
  }
  return ncenjet30_;
}

const int &vbstree::njet30() {
  if (not njet30_isLoaded) {
    if (njet30_branch != 0) {
      njet30_branch->GetEntry(index);
    } else {
      printf("branch njet30_branch does not exist!\n");
      exit(1);
    }
    njet30_isLoaded = true;
  }
  return njet30_;
}

const float &vbstree::leppt0() {
  if (not leppt0_isLoaded) {
    if (leppt0_branch != 0) {
      leppt0_branch->GetEntry(index);
    } else {
      printf("branch leppt0_branch does not exist!\n");
      exit(1);
    }
    leppt0_isLoaded = true;
  }
  return leppt0_;
}

const float &vbstree::leppt1() {
  if (not leppt1_isLoaded) {
    if (leppt1_branch != 0) {
      leppt1_branch->GetEntry(index);
    } else {
      printf("branch leppt1_branch does not exist!\n");
      exit(1);
    }
    leppt1_isLoaded = true;
  }
  return leppt1_;
}

const float &vbstree::taupt() {
  if (not taupt_isLoaded) {
    if (taupt_branch != 0) {
      taupt_branch->GetEntry(index);
    } else {
      printf("branch taupt_branch does not exist!\n");
      exit(1);
    }
    taupt_isLoaded = true;
  }
  return taupt_;
}

const int &vbstree::channel() {
  if (not channel_isLoaded) {
    if (channel_branch != 0) {
      channel_branch->GetEntry(index);
    } else {
      printf("branch channel_branch does not exist!\n");
      exit(1);
    }
    channel_isLoaded = true;
  }
  return channel_;
}

const float &vbstree::mjj() {
  if (not mjj_isLoaded) {
    if (mjj_branch != 0) {
      mjj_branch->GetEntry(index);
    } else {
      printf("branch mjj_branch does not exist!\n");
      exit(1);
    }
    mjj_isLoaded = true;
  }
  return mjj_;
}

const float &vbstree::ptjj() {
  if (not ptjj_isLoaded) {
    if (ptjj_branch != 0) {
      ptjj_branch->GetEntry(index);
    } else {
      printf("branch ptjj_branch does not exist!\n");
      exit(1);
    }
    ptjj_isLoaded = true;
  }
  return ptjj_;
}

const float &vbstree::detajj() {
  if (not detajj_isLoaded) {
    if (detajj_branch != 0) {
      detajj_branch->GetEntry(index);
    } else {
      printf("branch detajj_branch does not exist!\n");
      exit(1);
    }
    detajj_isLoaded = true;
  }
  return detajj_;
}

const float &vbstree::dphijj() {
  if (not dphijj_isLoaded) {
    if (dphijj_branch != 0) {
      dphijj_branch->GetEntry(index);
    } else {
      printf("branch dphijj_branch does not exist!\n");
      exit(1);
    }
    dphijj_isLoaded = true;
  }
  return dphijj_;
}

const float &vbstree::mbb() {
  if (not mbb_isLoaded) {
    if (mbb_branch != 0) {
      mbb_branch->GetEntry(index);
    } else {
      printf("branch mbb_branch does not exist!\n");
      exit(1);
    }
    mbb_isLoaded = true;
  }
  return mbb_;
}

const float &vbstree::ptbb() {
  if (not ptbb_isLoaded) {
    if (ptbb_branch != 0) {
      ptbb_branch->GetEntry(index);
    } else {
      printf("branch ptbb_branch does not exist!\n");
      exit(1);
    }
    ptbb_isLoaded = true;
  }
  return ptbb_;
}

const float &vbstree::dphibb() {
  if (not dphibb_isLoaded) {
    if (dphibb_branch != 0) {
      dphibb_branch->GetEntry(index);
    } else {
      printf("branch dphibb_branch does not exist!\n");
      exit(1);
    }
    dphibb_isLoaded = true;
  }
  return dphibb_;
}

const float &vbstree::drbb() {
  if (not drbb_isLoaded) {
    if (drbb_branch != 0) {
      drbb_branch->GetEntry(index);
    } else {
      printf("branch drbb_branch does not exist!\n");
      exit(1);
    }
    drbb_isLoaded = true;
  }
  return drbb_;
}

const float &vbstree::mll() {
  if (not mll_isLoaded) {
    if (mll_branch != 0) {
      mll_branch->GetEntry(index);
    } else {
      printf("branch mll_branch does not exist!\n");
      exit(1);
    }
    mll_isLoaded = true;
  }
  return mll_;
}

const float &vbstree::ptll() {
  if (not ptll_isLoaded) {
    if (ptll_branch != 0) {
      ptll_branch->GetEntry(index);
    } else {
      printf("branch ptll_branch does not exist!\n");
      exit(1);
    }
    ptll_isLoaded = true;
  }
  return ptll_;
}

const float &vbstree::dphill() {
  if (not dphill_isLoaded) {
    if (dphill_branch != 0) {
      dphill_branch->GetEntry(index);
    } else {
      printf("branch dphill_branch does not exist!\n");
      exit(1);
    }
    dphill_isLoaded = true;
  }
  return dphill_;
}

const float &vbstree::drll() {
  if (not drll_isLoaded) {
    if (drll_branch != 0) {
      drll_branch->GetEntry(index);
    } else {
      printf("branch drll_branch does not exist!\n");
      exit(1);
    }
    drll_isLoaded = true;
  }
  return drll_;
}

const float &vbstree::st() {
  if (not st_isLoaded) {
    if (st_branch != 0) {
      st_branch->GetEntry(index);
    } else {
      printf("branch st_branch does not exist!\n");
      exit(1);
    }
    st_isLoaded = true;
  }
  return st_;
}

const float &vbstree::mtllbbmet() {
  if (not mtllbbmet_isLoaded) {
    if (mtllbbmet_branch != 0) {
      mtllbbmet_branch->GetEntry(index);
    } else {
      printf("branch mtllbbmet_branch does not exist!\n");
      exit(1);
    }
    mtllbbmet_isLoaded = true;
  }
  return mtllbbmet_;
}

const float &vbstree::mllbbmet() {
  if (not mllbbmet_isLoaded) {
    if (mllbbmet_branch != 0) {
      mllbbmet_branch->GetEntry(index);
    } else {
      printf("branch mllbbmet_branch does not exist!\n");
      exit(1);
    }
    mllbbmet_isLoaded = true;
  }
  return mllbbmet_;
}


void vbstree::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace tas {

const int &run() { return vbs.run(); }
const int &lumi() { return vbs.lumi(); }
const unsigned long long &evt() { return vbs.evt(); }
const float &wgt() { return vbs.wgt(); }
const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &met_p4() { return vbs.met_p4(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_leptons_p4() { return vbs.good_leptons_p4(); }
const vector<int> &good_leptons_pdgid() { return vbs.good_leptons_pdgid(); }
const vector<int> &good_leptons_tight() { return vbs.good_leptons_tight(); }
const vector<int> &good_leptons_jetIdx() { return vbs.good_leptons_jetIdx(); }
const vector<float> &good_leptons_pfRelIso03_all() { return vbs.good_leptons_pfRelIso03_all(); }
const vector<float> &good_leptons_pfRelIso03_chg() { return vbs.good_leptons_pfRelIso03_chg(); }
const vector<float> &good_leptons_jetPtRelv2() { return vbs.good_leptons_jetPtRelv2(); }
const vector<float> &good_leptons_jetRelIso() { return vbs.good_leptons_jetRelIso(); }
const vector<float> &good_leptons_miniPFRelIso_all() { return vbs.good_leptons_miniPFRelIso_all(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_taus_p4() { return vbs.good_taus_p4(); }
const vector<int> &good_taus_pdgid() { return vbs.good_taus_pdgid(); }
const vector<int> &good_taus_tight() { return vbs.good_taus_tight(); }
const vector<int> &good_taus_jetIdx() { return vbs.good_taus_jetIdx(); }
const vector<int> &good_taus_genPartFlav() { return vbs.good_taus_genPartFlav(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_jets_p4() { return vbs.good_jets_p4(); }
const vector<int> &good_jets_loose_btagged() { return vbs.good_jets_loose_btagged(); }
const vector<int> &good_jets_medium_btagged() { return vbs.good_jets_medium_btagged(); }
const vector<int> &good_jets_tight_btagged() { return vbs.good_jets_tight_btagged(); }
const vector<float> &good_jets_btag_score() { return vbs.good_jets_btag_score(); }
const vector<float> &good_jets_qg_disc() { return vbs.good_jets_qg_disc(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &higgs_jets_p4() { return vbs.higgs_jets_p4(); }
const vector<int> &higgs_jets_loose_btagged() { return vbs.higgs_jets_loose_btagged(); }
const vector<int> &higgs_jets_medium_btagged() { return vbs.higgs_jets_medium_btagged(); }
const vector<int> &higgs_jets_tight_btagged() { return vbs.higgs_jets_tight_btagged(); }
const vector<float> &higgs_jets_btag_score() { return vbs.higgs_jets_btag_score(); }
const vector<int> &higgs_jets_good_jets_idx() { return vbs.higgs_jets_good_jets_idx(); }
const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbs_jets_p4() { return vbs.vbs_jets_p4(); }
const vector<int> &vbs_jets_good_jets_idx() { return vbs.vbs_jets_good_jets_idx(); }
const int &nbloose() { return vbs.nbloose(); }
const int &nbmedium() { return vbs.nbmedium(); }
const int &nbtight() { return vbs.nbtight(); }
const int &ncenjet30() { return vbs.ncenjet30(); }
const int &njet30() { return vbs.njet30(); }
const float &leppt0() { return vbs.leppt0(); }
const float &leppt1() { return vbs.leppt1(); }
const float &taupt() { return vbs.taupt(); }
const int &channel() { return vbs.channel(); }
const float &mjj() { return vbs.mjj(); }
const float &ptjj() { return vbs.ptjj(); }
const float &detajj() { return vbs.detajj(); }
const float &dphijj() { return vbs.dphijj(); }
const float &mbb() { return vbs.mbb(); }
const float &ptbb() { return vbs.ptbb(); }
const float &dphibb() { return vbs.dphibb(); }
const float &drbb() { return vbs.drbb(); }
const float &mll() { return vbs.mll(); }
const float &ptll() { return vbs.ptll(); }
const float &dphill() { return vbs.dphill(); }
const float &drll() { return vbs.drll(); }
const float &st() { return vbs.st(); }
const float &mtllbbmet() { return vbs.mtllbbmet(); }
const float &mllbbmet() { return vbs.mllbbmet(); }

}

#include "rooutil.cc"
