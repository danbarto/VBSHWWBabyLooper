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

mkdir -p hists/${STUDY}
rm -rf hists/${STUDY}/*.root

rm .jobs.txt

BABYVERSION=Baby_v2
EXECUTABLE=./studies/${STUDY}/doAnalysis
HISTDIR=hists/${STUDY}

echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tth.root              --scale1fb 0.000013717    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017tth_results.root         > ${HISTDIR}/log_tth.txt              " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_ttw.root              --scale1fb 0.000076266    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017ttw_results.root         > ${HISTDIR}/log_ttw.txt              " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_ttz.root              --scale1fb 0.000067525    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017ttz_results.root         > ${HISTDIR}/log_ttz.txt              " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_wz.root               --scale1fb 0.00064316     -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017wz_results.root          > ${HISTDIR}/log_wz.txt               " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_ssww.root             --scale1fb 0.00036322     -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/WWSS_results.root            > ${HISTDIR}/log_ssww.txt             " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_lambda20_vbshww.root  --scale1fb 0.00000077834  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_lambda_20_results.root   > ${HISTDIR}/log_lambda20_vbshww.txt  " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_lambdam20_vbshww.root --scale1fb 0.00000071875  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_lambda_m20_results.root  > ${HISTDIR}/log_lambdam20_vbshww.txt " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_vbshww.root           --scale1fb 0.00000017131  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_results.root             > ${HISTDIR}/log_vbshww.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_1.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep1_results.root     > ${HISTDIR}/log_tt2l_1.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_2.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep2_results.root     > ${HISTDIR}/log_tt2l_2.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_3.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep3_results.root     > ${HISTDIR}/log_tt2l_3.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_4.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep4_results.root     > ${HISTDIR}/log_tt2l_4.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_5.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep5_results.root     > ${HISTDIR}/log_tt2l_5.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_6.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep6_results.root     > ${HISTDIR}/log_tt2l_6.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_7.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep7_results.root     > ${HISTDIR}/log_tt2l_7.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_8.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep8_results.root     > ${HISTDIR}/log_tt2l_8.txt           " >> .jobs.txt
# echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt2l_9.root           --scale1fb 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep9_results.root     > ${HISTDIR}/log_tt2l_9.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_1.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt10_results.root    > ${HISTDIR}/log_tt1l_1.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_2.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt1_results.root     > ${HISTDIR}/log_tt1l_2.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_3.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt2_results.root     > ${HISTDIR}/log_tt1l_3.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_4.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt3_results.root     > ${HISTDIR}/log_tt1l_4.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_5.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt4_results.root     > ${HISTDIR}/log_tt1l_5.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_6.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt5_results.root     > ${HISTDIR}/log_tt1l_6.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_7.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt7_results.root     > ${HISTDIR}/log_tt1l_7.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_8.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt8_results.root     > ${HISTDIR}/log_tt1l_8.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_9.root           --scale1fb 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt9_results.root     > ${HISTDIR}/log_tt1l_9.txt           " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_10.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar10_results.root > ${HISTDIR}/log_tt1l_10.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_11.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar1_results.root  > ${HISTDIR}/log_tt1l_11.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_12.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar2_results.root  > ${HISTDIR}/log_tt1l_12.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_13.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar3_results.root  > ${HISTDIR}/log_tt1l_13.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_14.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar4_results.root  > ${HISTDIR}/log_tt1l_14.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_15.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar5_results.root  > ${HISTDIR}/log_tt1l_15.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_16.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar6_results.root  > ${HISTDIR}/log_tt1l_16.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_17.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar7_results.root  > ${HISTDIR}/log_tt1l_17.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_18.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar8_results.root  > ${HISTDIR}/log_tt1l_18.txt          " >> .jobs.txt
echo " ${EXECUTABLE} -t newTree -o ${HISTDIR}/output_tt1l_19.root          --scale1fb 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar9_results.root  > ${HISTDIR}/log_tt1l_19.txt          " >> .jobs.txt

echo "Launching jobs in parallel...."
xargs.sh .jobs.txt
echo "Done!"
