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

YEARS="2016 2017 2018"

EXECUTABLE=./studies/${STUDY}/doAnalysis

SAMPLES="TTJets_DiLept \
TTJets_SingleLeptFromT \
TTJets_SingleLeptFromTbar \
TTWJetsToLNu \
TTZToLLNuNu \
WZTo3LNu \
WpWpJJ_EWK-QCD \
ttHTobb \
VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5 \
VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5 \
VBSWmpWmpHToLNuLNu_TuneCP5"

NANOSKIMDIR=/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v9/

rm -f .jobs.txt

for SAMPLE in ${SAMPLES}; do

    for YEAR in ${YEARS}; do

        HISTDIR=hists/${STUDY}_${YEAR}
        mkdir -p ${HISTDIR}
        rm -rf ${HISTDIR}/*.root

        if [[ ${YEAR} == *"2016"* ]]; then NANOTAG=RunIISummer16NanoAOD*; fi
        if [[ ${YEAR} == *"2017"* ]]; then NANOTAG=RunIIFall17NanoAOD*; fi
        if [[ ${YEAR} == *"2018"* ]]; then NANOTAG=RunIIAutumn18NanoAOD*; fi

        if [[ ${SAMPLE} == *"ttHTobb"* ]]; then XSEC=0.1279; fi
        if [[ ${SAMPLE} == *"TTWJetsToLNu"* ]]; then XSEC=0.2043; fi
        if [[ ${SAMPLE} == *"TTZToLLNuNu"* ]]; then XSEC=0.2529; fi
        if [[ ${SAMPLE} == *"TTJets_DiLept"* ]]; then XSEC=91.044; fi
        if [[ ${SAMPLE} == *"TTJets_SingleLeptFromT"* ]]; then XSEC=182.96; fi
        if [[ ${SAMPLE} == *"TTJets_SingleLeptFromTbar"* ]]; then XSEC=182.96; fi
        if [[ ${SAMPLE} == *"WpWpJJ_EWK"* ]]; then XSEC=0.0539; fi
        if [[ ${SAMPLE} == *"WZTo3LNu"* ]]; then XSEC=4.4297; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_TuneCP5"* ]]; then XSEC=0.00001708; fi
        # if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5"* ]]; then XSEC=0.00201128180449; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5"* ]]; then XSEC=0.00116*137/59.97; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5"* ]]; then XSEC=0.000203237376*137/59.97; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_TuneCP5_2018"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_2017"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5_2017"* ]]; then continue; fi

        NTOTALEVENTS=$(head -n1 ${NANOSKIMDIR}/${SAMPLE}_*${NANOTAG}*/nevents.txt)
        NEFFEVENTS=$(tail -n1 ${NANOSKIMDIR}/${SAMPLE}_*${NANOTAG}*/nevents.txt)
        SCALE1FB=$(echo "${XSEC} / ${NEFFEVENTS} * 1000" | bc -l)

        echo ""
        echo "=========================================================================================="
        echo "Preparing command lines to process ..."
        echo "Sample                            : "${SAMPLE}
        echo "Year                              : "${YEAR}
        echo "Nano tag                          : "${NANOTAG}
        echo "N events information file         :" ${NANOSKIMDIR}/${SAMPLE}_*${NANOTAG}*/nevents.txt
        echo "N total events                    :" ${NTOTALEVENTS}
        echo "N eff total events (i.e. pos-neg) :" ${NEFFEVENTS}
        echo "Cross section (pb)                :" ${XSEC}
        echo "Scale1fb                          :" ${SCALE1FB}
        echo ""

        for FILE in $(ls ${NANOSKIMDIR}/${SAMPLE}_*${NANOTAG}*/*.root); do
            BASENAME=$(basename ${FILE})
            FILENAME=${BASENAME%.*}
            echo " ${EXECUTABLE} -t Events -o ${HISTDIR}/${SAMPLE}_${FILENAME}.root --scale1fb ${SCALE1FB} -i ${FILE} > ${HISTDIR}/${SAMPLE}_${FILENAME}.log 2>&1" >> .jobs.txt
        done

    done

done

echo "Launching jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
