# Help
usage()
{
  echo "ERROR - Usage:"
  echo
  echo "      sh $(basename $0) STUDYNAME"
  echo
  exit
}

if [ -z ${1} ]; then usage; fi
STUDY=${1}

YEARS="2016 2017 2018 Run2"

SAMPLES="TTJets_DiLept \
TTJets_SingLept_FromT \
TTJets_SingLept_FromTbar \
TTWJetsToLNu \
TTZToLLNuNu \
WZTo3LNu \
WpWpJJ_EWK-QCD \
ttHTobb"

rm .jobs.txt

for YEAR in ${YEARS}; do

    HADDDIR=hadds/${STUDY}_${YEAR}

    if [[ ${YEAR} == *"Run2"* ]]; then
        HISTDIR="hists/${STUDY}_*"
    else
        HISTDIR=hists/${STUDY}_${YEAR}
    fi

    mkdir -p ${HADDDIR}
    rm -rf ${HADDDIR}/*.root

    echo "hadd -f ${HADDDIR}/ssww.root ${HISTDIR}/WpWp*.root > ${HADDDIR}/ssww.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/tt1l.root ${HISTDIR}/TTJets_Sing*.root > ${HADDDIR}/tt1l.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/tt2l.root ${HISTDIR}/TTJets_Di*.root > ${HADDDIR}/tt2l.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/tth.root ${HISTDIR}/ttHTobb*.root > ${HADDDIR}/tth.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/ttw.root ${HISTDIR}/TTW*.root > ${HADDDIR}/ttw.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/ttz.root ${HISTDIR}/TTZ*.root > ${HADDDIR}/ttz.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/wz.root ${HISTDIR}/WZ*.root > ${HADDDIR}/wz.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/vbshww.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_TuneCP5*.root > ${HADDDIR}/vbshww.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_6.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_6.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_3.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_3.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_4p5.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_4p5.hadd.log 2>&1" >> .jobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_m2.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_m2.hadd.log 2>&1" >> .jobs.txt

done

echo "Launching hadd jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
