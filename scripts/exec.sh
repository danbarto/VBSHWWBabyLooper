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

rm .jobs.txt

YEARS="2016 2017 2018"

EXECUTABLE=./studies/${STUDY}/doAnalysis

SAMPLES="TTJets_DiLept \
TTJets_SingLept_FromT \
TTJets_SingLept_FromTbar \
TTWJetsToLNu \
TTZToLLNuNu \
WZTo3LNu \
WpWpJJ_EWK-QCD \
ttHTobb \
VBSHWW"

BABYDIR=/nfs-7/userdata/phchang/HWW_babies/Baby_v3

rm .jobs.txt

for SAMPLE in ${SAMPLES}; do

    for YEAR in ${YEARS}; do

        HISTDIR=hists/${STUDY}_${YEAR}
        mkdir -p ${HISTDIR}
        rm -rf ${HISTDIR}/*.root

        if [[ ${YEAR} == *"2016"* ]]; then NANOTAG=RunIISummer16NanoAOD*; fi
        if [[ ${YEAR} == *"2017"* ]]; then NANOTAG=RunIIFall17NanoAOD*; fi
        if [[ ${YEAR} == *"2018"* ]]; then NANOTAG=RunIIAutumn18NanoAOD*; fi

        if [[ ${SAMPLE}_${YEAR} == *"ttHTobb_2016"* ]]; then SCALE1FB=1.35959e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"ttHTobb_2017"* ]]; then SCALE1FB=1.41572e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"ttHTobb_2018"* ]]; then SCALE1FB=1.37127e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2016"* ]]; then SCALE1FB=7.48404e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2017"* ]]; then SCALE1FB=7.77749e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2018"* ]]; then SCALE1FB=8.8951e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_2016"* ]]; then SCALE1FB=3.95777e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_2017"* ]]; then SCALE1FB=6.75246e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_2018"* ]]; then SCALE1FB=4.03089e-05; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_DiLept_2016"* ]]; then SCALE1FB=0.0089522; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_DiLept_2017"* ]]; then SCALE1FB=0.00321153; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_DiLept_2018"* ]]; then SCALE1FB=0.00304636; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromT_2016"* ]]; then SCALE1FB=0.0160886; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromT_2017"* ]]; then SCALE1FB=0.0030306; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromT_2018"* ]]; then SCALE1FB=0.00319832; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromTbar_2016"* ]]; then SCALE1FB=0.0152786; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromTbar_2017"* ]]; then SCALE1FB=0.0032409; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTJets_SingLept_FromTbar_2018"* ]]; then SCALE1FB=0.00305585; fi
        if [[ ${SAMPLE}_${YEAR} == *"WpWpJJ_EWK-QCD_2016"* ]]; then SCALE1FB=0.000360099; fi
        if [[ ${SAMPLE}_${YEAR} == *"WpWpJJ_EWK-QCD_2017"* ]]; then SCALE1FB=0.000363222; fi
        if [[ ${SAMPLE}_${YEAR} == *"WpWpJJ_EWK-QCD_2018"* ]]; then SCALE1FB=0.000360854; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2016"* ]]; then SCALE1FB=0.00222241; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2017"* ]]; then SCALE1FB=0.000643159; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2018"* ]]; then SCALE1FB=0.00065728; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSHWW_2016"* ]]; then continue; fi # TODO
        if [[ ${SAMPLE}_${YEAR} == *"VBSHWW_2017"* ]]; then SCALE1FB=0.00000017131; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSHWW_2018"* ]]; then continue; fi # TODO

        echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_${SAMPLE}.root --scale1fb ${SCALE1FB} -i ${BABYDIR}/${YEAR}/${SAMPLE}_${NANOTAG}.root > ${HISTDIR}/log_${SAMPLE}.txt " >> .jobs.txt

    done

done

echo "Launching jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
