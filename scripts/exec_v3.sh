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
TTZToLLNuNu_M-10 \
TTZToLL_M-1to10 \
WZTo3LNu \
ZZTo4L \
GluGluHToZZTo4L \
DYJetsToLL_M-50 \
DYJetsToLL_M-10to50 \
WpWpJJ_EWK-QCD \
ttHToNonbb \
ttHTobb \
WJetsToLNu \
tZq_ll_4f_ckm_NLO \
ST_tWll_5f_LO \
TTTT \
TTWH \
TTWW \
TTWZ \
TTZH \
TTZZ \
WWW_4F \
WWZ \
WZG \
WZZ \
ZZZ \
WWToLNuQQ \
ST_s-channel_4f \
ST_t-channel_antitop_4f \
ST_t-channel_top_4f \
ST_tW_antitop_5f \
ST_tW_top_5f \
VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5 \
VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5 \
VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5 \
VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5 \
VBSWmpWmpHToLNuLNu_TuneCP5 \
DoubleEG_Run2016 \
DoubleMuon_Run2016 \
MuonEG_Run2016 \
SingleElectron_Run2016 \
SingleMuon_Run2016 \
DoubleEG_Run2017 \
DoubleMuon_Run2017 \
MuonEG_Run2017 \
SingleElectron_Run2017 \
SingleMuon_Run2017 \
EGamma_Run2018 \
DoubleMuon_Run2018 \
MuonEG_Run2018 \
SingleMuon_Run2018"

# NANOSKIMDIR=/hadoop/cms/store/user/phchang/VBSHWWNanoSkim/v12/
NANOSKIMDIR=/nfs-7/userdata/phchang/VBSHWWNanoSkim_v12/

rm -f .jobs.txt

for SAMPLE in ${SAMPLES}; do

    XSEC=""

    for YEAR in ${YEARS}; do

        HISTDIR=hists/${STUDY}_${YEAR}
        mkdir -p ${HISTDIR}

        if [[ ${YEAR} == *"2016"* ]]; then NANOTAG=RunIISummer16NanoAOD*; fi
        if [[ ${YEAR} == *"2017"* ]]; then NANOTAG=RunIIFall17NanoAOD*; fi
        if [[ ${YEAR} == *"2018"* ]]; then NANOTAG=RunIIAutumn18NanoAOD*; fi

        if [[ ${SAMPLE} == *"Run201"* ]]; then NANOTAG=""; fi

        if [[ ${SAMPLE} == *"tZq_ll_4f_ckm_NLO"* ]]; then XSEC=0.0758; fi
        if [[ ${SAMPLE} == *"ST_s-channel_4f"* ]]; then XSEC=3.74; fi
        if [[ ${SAMPLE} == *"ST_t-channel_antitop_4f"* ]]; then XSEC=80.95; fi
        if [[ ${SAMPLE} == *"ST_t-channel_top_4f"* ]]; then XSEC=136.02; fi
        if [[ ${SAMPLE} == *"ST_tW_antitop_5f"* ]]; then XSEC=19.559; fi
        if [[ ${SAMPLE} == *"ST_tW_top_5f"* ]]; then XSEC=19.559; fi
        if [[ ${SAMPLE} == *"ST_tWll_5f_LO"* ]]; then XSEC=0.01123; fi
        if [[ ${SAMPLE} == *"ZZTo4L"* ]]; then XSEC=1.256; fi
        if [[ ${SAMPLE} == *"GluGluHToZZTo4L"* ]]; then XSEC=0.0082323; fi
        if [[ ${SAMPLE} == *"DYJetsToLL_M-10to50"* ]]; then XSEC=20657.0; fi
        if [[ ${SAMPLE} == *"DYJetsToLL_M-50"* ]]; then XSEC=6198.0; fi
        if [[ ${SAMPLE} == *"WJetsToLNu"* ]]; then XSEC=61335.0; fi
        if [[ ${SAMPLE} == *"ttHTobb"* ]]; then XSEC=0.1279; fi
        if [[ ${SAMPLE} == *"TTWJetsToLNu"* ]]; then XSEC=0.2043; fi
        if [[ ${SAMPLE} == *"TTZToLLNuNu_M-10"* ]]; then XSEC=0.2529; fi
        if [[ ${SAMPLE} == *"TTZToLL_M-1to10"* ]]; then XSEC=0.2529; fi
        if [[ ${SAMPLE} == *"TTJets_DiLept"* ]]; then XSEC=91.044; fi
        if [[ ${SAMPLE} == *"TTJets_SingleLeptFromT"* ]]; then XSEC=182.96; fi
        if [[ ${SAMPLE} == *"TTJets_SingleLeptFromTbar"* ]]; then XSEC=182.96; fi
        if [[ ${SAMPLE} == *"WpWpJJ_EWK"* ]]; then XSEC=0.0539; fi
        if [[ ${SAMPLE} == *"WZTo3LNu"* ]]; then XSEC=4.4297; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_TuneCP5"* ]]; then XSEC=0.00001708; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5"* ]]; then XSEC=0.00116*137/59.97; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5"* ]]; then XSEC=0.000203237376*137/59.97; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5"* ]]; then XSEC=0.0005865984*137/59.97; fi
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5"* ]]; then XSEC=0.000418440192*137/59.97; fi
        if [[ ${SAMPLE} == *"ttHToNonbb"* ]]; then XSEC=0.215; fi
        if [[ ${SAMPLE} == *"TTTT"* ]]; then XSEC=0.009103; fi
        if [[ ${SAMPLE} == *"TTWW"* ]]; then XSEC=0.0115; fi
        if [[ ${SAMPLE} == *"TTWZ"* ]]; then XSEC=0.003884; fi
        if [[ ${SAMPLE} == *"TTZZ"* ]]; then XSEC=0.001982; fi
        if [[ ${SAMPLE} == *"TTWH"* ]]; then XSEC=0.001582; fi
        if [[ ${SAMPLE} == *"TTZH"* ]]; then XSEC=0.001535; fi
        if [[ ${SAMPLE} == *"WWW_4F"* ]]; then XSEC=0.2086; fi
        if [[ ${SAMPLE} == *"WWZ"* ]]; then XSEC=0.1651; fi
        if [[ ${SAMPLE} == *"WZG"* ]]; then XSEC=0.04123; fi
        if [[ ${SAMPLE} == *"WZZ"* ]]; then XSEC=0.05565; fi
        if [[ ${SAMPLE} == *"ZZZ"* ]]; then XSEC=0.01398; fi
        if [[ ${SAMPLE} == *"WWToLNuQQ"* ]]; then XSEC=49.997; fi
        if [[ ${SAMPLE} == *"Run201"* ]]; then XSEC=1; fi # data

        if [[ -z "${XSEC}" ]]; then
            echo ${SAMPLE} cross section missing!
            echo "here is my guess"
            grep ${SAMPLE} NanoTools/NanoCORE/datasetinfo/scale1fbs.txt | awk '{print $1, $5}'
            exit
        fi

        if [[ ${SAMPLE} == *"Run201"* ]]; then
            if [[ ${SAMPLE} == *"${YEAR}"* ]]; then
                :
            else
                continue
            fi
        fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_TuneCP5_2018"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_6_TuneCP5_2017"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_3_TuneCP5_2017"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5_2017"* ]]; then continue; fi

        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5_2016"* ]]; then continue; fi
        if [[ ${SAMPLE}_${YEAR} == *"VBSWmpWmpHToLNuLNu_C2V_m2_TuneCP5_2017"* ]]; then continue; fi

        EXTRATAG=""
        if [[ ${SAMPLE} == *"VBSWmpWmpHToLNuLNu_C2V_4p5_TuneCP5"* ]]; then EXTRATAG=ext1; fi

        # Last bit modification
        if [[ ${SAMPLE} == *"Run201"* ]]; then
            XSEC=1;
            SAMPLEWITHUNDERSCORE=${SAMPLE} # Data does not get the underscore
        else
            # If it is not data then the SAMPLE string gets a "_" subscript in order to loop over them
            SAMPLEWITHUNDERSCORE=${SAMPLE}_
        fi

        NEVENTSINFOFILE=${NANOSKIMDIR}/${SAMPLEWITHUNDERSCORE}*${NANOTAG}*${EXTRATAG}*/merged/nevents.txt
        if [[ ${SAMPLE} == *"Run201"* ]]; then
            NTOTALEVENTS=1
            NEFFEVENTS=1
            SCALE1FB=1
        else
            NTOTALEVENTS=$(head -n1 ${NEVENTSINFOFILE})
            NEFFEVENTS=$(tail -n1 ${NEVENTSINFOFILE})
            SCALE1FB=$(echo "${XSEC} / ${NEFFEVENTS} * 1000" | bc -l)
        fi

        echo ""
        echo "=========================================================================================="
        echo "Preparing command lines to process ..."
        echo "Sample                            :" ${SAMPLE}
        echo "Year                              :" ${YEAR}
        echo "Nano tag                          :" ${NANOTAG}
        echo "N events information file         :" ${NEVENTSINFOFILE}
        echo "N total events                    :" ${NTOTALEVENTS}
        echo "N eff total events (i.e. pos-neg) :" ${NEFFEVENTS}
        echo "Cross section (pb)                :" ${XSEC}
        echo "Scale1fb                          :" ${SCALE1FB}
        echo ""

        #
        # More than 1 jobs
        #
        NJOBS=1
        if [[ ${SAMPLE}_${YEAR} == *"ttHToNonbb_2016"* ]]; then NJOBS=2; fi
        if [[ ${SAMPLE}_${YEAR} == *"ttHToNonbb_2017"* ]]; then NJOBS=2; fi
        if [[ ${SAMPLE}_${YEAR} == *"ttHToNonbb_2018"* ]]; then NJOBS=2; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2016"* ]]; then NJOBS=6; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2017"* ]]; then NJOBS=6; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTWJetsToLNu_2018"* ]]; then NJOBS=6; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2016"* ]]; then NJOBS=1; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2017"* ]]; then NJOBS=6; fi
        if [[ ${SAMPLE}_${YEAR} == *"WZTo3LNu_2018"* ]]; then NJOBS=6; fi
        if [[ ${SAMPLE}_${YEAR} == *"tZq_ll_4f_ckm_NLO_2016"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"tZq_ll_4f_ckm_NLO_2017"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"tZq_ll_4f_ckm_NLO_2018"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_M-10_2016"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_M-10_2017"* ]]; then NJOBS=5; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTZToLLNuNu_M-10_2018"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTTT_2017"* ]]; then NJOBS=2; fi
        if [[ ${SAMPLE}_${YEAR} == *"TTTT_2018"* ]]; then NJOBS=7; fi
        if [[ ${SAMPLE}_${YEAR} == *"ZZTo4L_2016"* ]]; then NJOBS=2; fi
        if [[ ${SAMPLE}_${YEAR} == *"ZZTo4L_2017"* ]]; then NJOBS=20; fi
        if [[ ${SAMPLE}_${YEAR} == *"ZZTo4L_2018"* ]]; then NJOBS=20; fi
        NJOBSMAXIDX=$((NJOBS - 1))

        FILELIST=$(ls ${NANOSKIMDIR}/${SAMPLEWITHUNDERSCORE}*${NANOTAG}*${EXTRATAG}*/merged/output.root | tr '\n' ',')
        FILENAME=output
        for IJOB in $(seq 0 ${NJOBSMAXIDX}); do
            echo "rm -f ${HISTDIR}/${SAMPLE}_${FILENAME}_${IJOB}.root; ${EXECUTABLE} -t Events -o ${HISTDIR}/${SAMPLE}_${FILENAME}_${IJOB}.root --scale1fb ${SCALE1FB} -j ${NJOBS} -I ${IJOB} -i ${FILELIST} > ${HISTDIR}/${SAMPLE}_${FILENAME}_${IJOB}.log 2>&1" >> .jobs.txt
        done


    done

done

echo "Launching jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
