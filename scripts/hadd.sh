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

mkdir -p hadds/${STUDY}
rm -rf hadds/${STUDY}/*.root

HISTDIR=hists/${STUDY}
HADDDIR=hadds/${STUDY}

rm .jobs.txt
echo "hadd -f ${HADDDIR}/ssww.root ${HISTDIR}/output_ssww.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/tt1l.root ${HISTDIR}/output_tt1l_*.root" >> .jobs.txt
# echo "hadd -f ${HADDDIR}/tt2l.root ${HISTDIR}/output_tt2l_*.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/tth.root ${HISTDIR}/output_tth.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/ttw.root ${HISTDIR}/output_ttw.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/ttz.root ${HISTDIR}/output_ttz.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/vbshww.root ${HISTDIR}/output_vbshww.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/wz.root ${HISTDIR}/output_wz.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/lambda20_vbshww.root ${HISTDIR}/output_lambda20_vbshww.root" >> .jobs.txt
echo "hadd -f ${HADDDIR}/lambdam20_vbshww.root ${HISTDIR}/output_lambdam20_vbshww.root" >> .jobs.txt
echo "Launching hadd jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
