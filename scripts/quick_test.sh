ANALYSIS=threeLeptons
ANALYSIS=mainAnalysis
 ./studies/${ANALYSIS}/doAnalysis -t Events --scale1fb .00026767592550483000 -i /nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_13TeV-madgraph-pythia8_PRIVATE_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1_NANOAODSIM_v12/merged/output.root, -d
 ./studies/${ANALYSIS}/doAnalysis -t Events --scale1fb .00305584814692857000 -i /nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//TTJets_SingleLeptFromTbar_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18NanoAODv6-Nano25Oct2019_102X_upgrade2018_realistic_v20-v1_NANOAODSIM_v12/merged/output.root, -d
 ./studies/${ANALYSIS}/doAnalysis -t Events --scale1fb 1 -i /nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//DoubleMuon_Run2017B-Nano25Oct2019-v1_NANOAOD_v12/merged/output.root,/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//DoubleMuon_Run2017C-Nano25Oct2019-v1_NANOAOD_v12/merged/output.root,/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//DoubleMuon_Run2017D-Nano25Oct2019-v1_NANOAOD_v12/merged/output.root,/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//DoubleMuon_Run2017E-Nano25Oct2019-v1_NANOAOD_v12/merged/output.root,/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12//DoubleMuon_Run2017F-Nano25Oct2019-v1_NANOAOD_v12/merged/output.root, -d
 rm debug.root
 rm debug.*.txt
