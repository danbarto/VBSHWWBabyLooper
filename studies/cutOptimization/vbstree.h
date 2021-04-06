// -*- C++ -*-
// This is a header file generated with the command:
// makeCMS3ClassFiles("/home/users/phchang/analysis/hwh/VBSHWWBabyLooper/studies/cutOptimization/vbshww_c2v_6.root", "variable", "vbstree", "tas", "vbs")

#ifndef vbstree_H
#define vbstree_H

#include "Math/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class vbstree {
 private: 
 protected: 
  unsigned int index;
  int      run_;
  TBranch *run_branch;
  bool     run_isLoaded;
  int      lumi_;
  TBranch *lumi_branch;
  bool     lumi_isLoaded;
  unsigned long long evt_;
  TBranch *evt_branch;
  bool     evt_isLoaded;
  float    wgt_;
  TBranch *wgt_branch;
  bool     wgt_isLoaded;
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > *met_p4_;
  TBranch *met_p4_branch;
  bool     met_p4_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > *good_leptons_p4_;
  TBranch *good_leptons_p4_branch;
  bool     good_leptons_p4_isLoaded;
  vector<int> *good_leptons_pdgid_;
  TBranch *good_leptons_pdgid_branch;
  bool     good_leptons_pdgid_isLoaded;
  vector<int> *good_leptons_tight_;
  TBranch *good_leptons_tight_branch;
  bool     good_leptons_tight_isLoaded;
  vector<int> *good_leptons_jetIdx_;
  TBranch *good_leptons_jetIdx_branch;
  bool     good_leptons_jetIdx_isLoaded;
  vector<float> *good_leptons_pfRelIso03_all_;
  TBranch *good_leptons_pfRelIso03_all_branch;
  bool     good_leptons_pfRelIso03_all_isLoaded;
  vector<float> *good_leptons_pfRelIso03_chg_;
  TBranch *good_leptons_pfRelIso03_chg_branch;
  bool     good_leptons_pfRelIso03_chg_isLoaded;
  vector<float> *good_leptons_jetPtRelv2_;
  TBranch *good_leptons_jetPtRelv2_branch;
  bool     good_leptons_jetPtRelv2_isLoaded;
  vector<float> *good_leptons_jetRelIso_;
  TBranch *good_leptons_jetRelIso_branch;
  bool     good_leptons_jetRelIso_isLoaded;
  vector<float> *good_leptons_miniPFRelIso_all_;
  TBranch *good_leptons_miniPFRelIso_all_branch;
  bool     good_leptons_miniPFRelIso_all_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > *good_taus_p4_;
  TBranch *good_taus_p4_branch;
  bool     good_taus_p4_isLoaded;
  vector<int> *good_taus_pdgid_;
  TBranch *good_taus_pdgid_branch;
  bool     good_taus_pdgid_isLoaded;
  vector<int> *good_taus_tight_;
  TBranch *good_taus_tight_branch;
  bool     good_taus_tight_isLoaded;
  vector<int> *good_taus_jetIdx_;
  TBranch *good_taus_jetIdx_branch;
  bool     good_taus_jetIdx_isLoaded;
  vector<int> *good_taus_genPartFlav_;
  TBranch *good_taus_genPartFlav_branch;
  bool     good_taus_genPartFlav_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > *good_jets_p4_;
  TBranch *good_jets_p4_branch;
  bool     good_jets_p4_isLoaded;
  vector<int> *good_jets_loose_btagged_;
  TBranch *good_jets_loose_btagged_branch;
  bool     good_jets_loose_btagged_isLoaded;
  vector<int> *good_jets_medium_btagged_;
  TBranch *good_jets_medium_btagged_branch;
  bool     good_jets_medium_btagged_isLoaded;
  vector<int> *good_jets_tight_btagged_;
  TBranch *good_jets_tight_btagged_branch;
  bool     good_jets_tight_btagged_isLoaded;
  vector<float> *good_jets_btag_score_;
  TBranch *good_jets_btag_score_branch;
  bool     good_jets_btag_score_isLoaded;
  vector<float> *good_jets_qg_disc_;
  TBranch *good_jets_qg_disc_branch;
  bool     good_jets_qg_disc_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > *higgs_jets_p4_;
  TBranch *higgs_jets_p4_branch;
  bool     higgs_jets_p4_isLoaded;
  vector<int> *higgs_jets_loose_btagged_;
  TBranch *higgs_jets_loose_btagged_branch;
  bool     higgs_jets_loose_btagged_isLoaded;
  vector<int> *higgs_jets_medium_btagged_;
  TBranch *higgs_jets_medium_btagged_branch;
  bool     higgs_jets_medium_btagged_isLoaded;
  vector<int> *higgs_jets_tight_btagged_;
  TBranch *higgs_jets_tight_btagged_branch;
  bool     higgs_jets_tight_btagged_isLoaded;
  vector<float> *higgs_jets_btag_score_;
  TBranch *higgs_jets_btag_score_branch;
  bool     higgs_jets_btag_score_isLoaded;
  vector<int> *higgs_jets_good_jets_idx_;
  TBranch *higgs_jets_good_jets_idx_branch;
  bool     higgs_jets_good_jets_idx_isLoaded;
  vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > *vbs_jets_p4_;
  TBranch *vbs_jets_p4_branch;
  bool     vbs_jets_p4_isLoaded;
  vector<int> *vbs_jets_good_jets_idx_;
  TBranch *vbs_jets_good_jets_idx_branch;
  bool     vbs_jets_good_jets_idx_isLoaded;
  int      nbloose_;
  TBranch *nbloose_branch;
  bool     nbloose_isLoaded;
  int      nbmedium_;
  TBranch *nbmedium_branch;
  bool     nbmedium_isLoaded;
  int      nbtight_;
  TBranch *nbtight_branch;
  bool     nbtight_isLoaded;
  int      ncenjet30_;
  TBranch *ncenjet30_branch;
  bool     ncenjet30_isLoaded;
  int      njet30_;
  TBranch *njet30_branch;
  bool     njet30_isLoaded;
  float    leppt0_;
  TBranch *leppt0_branch;
  bool     leppt0_isLoaded;
  float    leppt1_;
  TBranch *leppt1_branch;
  bool     leppt1_isLoaded;
  float    taupt_;
  TBranch *taupt_branch;
  bool     taupt_isLoaded;
  int      channel_;
  TBranch *channel_branch;
  bool     channel_isLoaded;
  float    mjj_;
  TBranch *mjj_branch;
  bool     mjj_isLoaded;
  float    ptjj_;
  TBranch *ptjj_branch;
  bool     ptjj_isLoaded;
  float    detajj_;
  TBranch *detajj_branch;
  bool     detajj_isLoaded;
  float    dphijj_;
  TBranch *dphijj_branch;
  bool     dphijj_isLoaded;
  float    mbb_;
  TBranch *mbb_branch;
  bool     mbb_isLoaded;
  float    ptbb_;
  TBranch *ptbb_branch;
  bool     ptbb_isLoaded;
  float    dphibb_;
  TBranch *dphibb_branch;
  bool     dphibb_isLoaded;
  float    drbb_;
  TBranch *drbb_branch;
  bool     drbb_isLoaded;
  float    mll_;
  TBranch *mll_branch;
  bool     mll_isLoaded;
  float    ptll_;
  TBranch *ptll_branch;
  bool     ptll_isLoaded;
  float    dphill_;
  TBranch *dphill_branch;
  bool     dphill_isLoaded;
  float    drll_;
  TBranch *drll_branch;
  bool     drll_isLoaded;
  float    st_;
  TBranch *st_branch;
  bool     st_isLoaded;
  float    mtllbbmet_;
  TBranch *mtllbbmet_branch;
  bool     mtllbbmet_isLoaded;
  float    mllbbmet_;
  TBranch *mllbbmet_branch;
  bool     mllbbmet_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const float &wgt();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &met_p4();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_leptons_p4();
  const vector<int> &good_leptons_pdgid();
  const vector<int> &good_leptons_tight();
  const vector<int> &good_leptons_jetIdx();
  const vector<float> &good_leptons_pfRelIso03_all();
  const vector<float> &good_leptons_pfRelIso03_chg();
  const vector<float> &good_leptons_jetPtRelv2();
  const vector<float> &good_leptons_jetRelIso();
  const vector<float> &good_leptons_miniPFRelIso_all();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_taus_p4();
  const vector<int> &good_taus_pdgid();
  const vector<int> &good_taus_tight();
  const vector<int> &good_taus_jetIdx();
  const vector<int> &good_taus_genPartFlav();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_jets_p4();
  const vector<int> &good_jets_loose_btagged();
  const vector<int> &good_jets_medium_btagged();
  const vector<int> &good_jets_tight_btagged();
  const vector<float> &good_jets_btag_score();
  const vector<float> &good_jets_qg_disc();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &higgs_jets_p4();
  const vector<int> &higgs_jets_loose_btagged();
  const vector<int> &higgs_jets_medium_btagged();
  const vector<int> &higgs_jets_tight_btagged();
  const vector<float> &higgs_jets_btag_score();
  const vector<int> &higgs_jets_good_jets_idx();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbs_jets_p4();
  const vector<int> &vbs_jets_good_jets_idx();
  const int &nbloose();
  const int &nbmedium();
  const int &nbtight();
  const int &ncenjet30();
  const int &njet30();
  const float &leppt0();
  const float &leppt1();
  const float &taupt();
  const int &channel();
  const float &mjj();
  const float &ptjj();
  const float &detajj();
  const float &dphijj();
  const float &mbb();
  const float &ptbb();
  const float &dphibb();
  const float &drbb();
  const float &mll();
  const float &ptll();
  const float &dphill();
  const float &drll();
  const float &st();
  const float &mtllbbmet();
  const float &mllbbmet();

  static void progress(int nEventsTotal, int nEventsChain);
};

#ifndef __CINT__
extern vbstree vbs;
#endif

namespace tas {
  const int &run();
  const int &lumi();
  const unsigned long long &evt();
  const float &wgt();
  const ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > &met_p4();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_leptons_p4();
  const vector<int> &good_leptons_pdgid();
  const vector<int> &good_leptons_tight();
  const vector<int> &good_leptons_jetIdx();
  const vector<float> &good_leptons_pfRelIso03_all();
  const vector<float> &good_leptons_pfRelIso03_chg();
  const vector<float> &good_leptons_jetPtRelv2();
  const vector<float> &good_leptons_jetRelIso();
  const vector<float> &good_leptons_miniPFRelIso_all();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_taus_p4();
  const vector<int> &good_taus_pdgid();
  const vector<int> &good_taus_tight();
  const vector<int> &good_taus_jetIdx();
  const vector<int> &good_taus_genPartFlav();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &good_jets_p4();
  const vector<int> &good_jets_loose_btagged();
  const vector<int> &good_jets_medium_btagged();
  const vector<int> &good_jets_tight_btagged();
  const vector<float> &good_jets_btag_score();
  const vector<float> &good_jets_qg_disc();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &higgs_jets_p4();
  const vector<int> &higgs_jets_loose_btagged();
  const vector<int> &higgs_jets_medium_btagged();
  const vector<int> &higgs_jets_tight_btagged();
  const vector<float> &higgs_jets_btag_score();
  const vector<int> &higgs_jets_good_jets_idx();
  const vector<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > > &vbs_jets_p4();
  const vector<int> &vbs_jets_good_jets_idx();
  const int &nbloose();
  const int &nbmedium();
  const int &nbtight();
  const int &ncenjet30();
  const int &njet30();
  const float &leppt0();
  const float &leppt1();
  const float &taupt();
  const int &channel();
  const float &mjj();
  const float &ptjj();
  const float &detajj();
  const float &dphijj();
  const float &mbb();
  const float &ptbb();
  const float &dphibb();
  const float &drbb();
  const float &mll();
  const float &ptll();
  const float &dphill();
  const float &drll();
  const float &st();
  const float &mtllbbmet();
  const float &mllbbmet();
}
#include "rooutil.h"
#endif
