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

rm .haddjobs.txt

for YEAR in ${YEARS}; do

    HADDDIR=hadds/${STUDY}_${YEAR}

    if [[ ${YEAR} == *"Run2"* ]]; then
        HISTDIR="hists/${STUDY}_*"
    else
        HISTDIR=hists/${STUDY}_${YEAR}
    fi

    mkdir -p ${HADDDIR}
    rm -rf ${HADDDIR}/*.root

    echo "hadd -f ${HADDDIR}/data.root ${HISTDIR}/*Run2*.root > ${HADDDIR}/data.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/ssww.root ${HISTDIR}/WpWp*.root > ${HADDDIR}/ssww.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/wjet.root ${HISTDIR}/WJetsTo*.root > ${HADDDIR}/wjet.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/dy.root ${HISTDIR}/DY*.root > ${HADDDIR}/dy.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vjet.root ${HISTDIR}/WJetsTo*.root ${HISTDIR}/DY*.root > ${HADDDIR}/vjet.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/tt1l.root ${HISTDIR}/TTJets_Sing*.root > ${HADDDIR}/tt1l.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/tt2l.root ${HISTDIR}/TTJets_Di*.root > ${HADDDIR}/tt2l.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/ttw.root ${HISTDIR}/TTW*.root > ${HADDDIR}/ttw.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/ttz.root ${HISTDIR}/TTZ*.root > ${HADDDIR}/ttz.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/tth.root ${HISTDIR}/ttHToNonbb*.root ${HISTDIR}/ttHTobb*.root > ${HADDDIR}/tth.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/twz.root ${HISTDIR}/ST_tWll*.root > ${HADDDIR}/twz.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/tzq.root ${HISTDIR}/tZq_ll_4f_ckm_NLO*.root > ${HADDDIR}/tzq.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/raretop.root ${HISTDIR}/ttHToNonbb*.root ${HISTDIR}/ttHTobb*.root ${HISTDIR}/ST_tWll*.root ${HISTDIR}/tZq_ll_4f_ckm_NLO*.root ${HISTDIR}/TTTT*.root ${HISTDIR}/TTWH*.root ${HISTDIR}/TTZH*.root ${HISTDIR}/TTWW*.root ${HISTDIR}/TTWZ*.root ${HISTDIR}/TTZZ*.root > ${HADDDIR}/raretop.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vvv.root ${HISTDIR}/WWW_4F*.root ${HISTDIR}/WWZ*.root ${HISTDIR}/WZZ*.root ${HISTDIR}/ZZZ*.root > ${HADDDIR}/vvv.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/wz.root ${HISTDIR}/WZTo3LNu*.root > ${HADDDIR}/wz.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/wzg.root ${HISTDIR}/WZG*.root > ${HADDDIR}/wzg.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/zz.root ${HISTDIR}/ZZTo4L*.root ${HISTDIR}/GluGluHToZZTo4L*.root > ${HADDDIR}/zz.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/bosons.root ${HISTDIR}/WJetsTo*.root ${HISTDIR}/DY*.root ${HISTDIR}/WpWp*.root ${HISTDIR}/WZTo3LNu*.root ${HISTDIR}/ZZTo4L*.root ${HISTDIR}/GluGluHToZZTo4L*.root ${HISTDIR}/WWW_4F*.root ${HISTDIR}/WWZ*.root ${HISTDIR}/WZZ*.root ${HISTDIR}/ZZZ*.root > ${HADDDIR}/wz.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vbshww.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_TuneCP5*.root > ${HADDDIR}/vbshww.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_6.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_6.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_3.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_3.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_4p5.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_4p5.hadd.log 2>&1" >> .haddjobs.txt
    echo "hadd -f ${HADDDIR}/vbshww_c2v_m2.root ${HISTDIR}/VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5*.root > ${HADDDIR}/vbshww_c2v_m2.hadd.log 2>&1" >> .haddjobs.txt

done

echo "Launching hadd jobs in parallel...."
xargs.sh .haddjobs.txt
echo "Done!"
