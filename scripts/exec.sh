
mkdir -p hists/
rm -rf hists/*.root

rm .jobs.txt

BABYVERSION=Baby_v2

echo " ./doAnalysis -t newTree -o hists/output_tth.root              -s 0.000013717    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017tth_results.root         > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_ttw.root              -s 0.000076266    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017ttw_results.root         > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_ttz.root              -s 0.000067525    -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017ttz_results.root         > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_wz.root               -s 0.00064316     -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/2017wz_results.root          > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_ssww.root             -s 0.00036322     -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/WWSS_results.root            > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_lambda20_vbshww.root  -s 0.00000077834  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_lambda_20_results.root   > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_lambdam20_vbshww.root -s 0.00000071875  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_lambda_m20_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_vbshww.root           -s 0.00000017131  -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/sig_results.root             > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_1.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep1_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_2.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep2_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_3.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep3_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_4.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep4_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_5.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep5_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_6.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep6_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_7.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep7_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_8.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep8_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt2l_9.root           -s 0.0016280      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbardilep9_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_1.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt10_results.root    > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_2.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt1_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_3.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt2_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_4.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt3_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_5.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt4_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_6.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt5_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_7.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt7_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_8.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt8_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_9.root           -s 0.0033379      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromt9_results.root     > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_10.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar10_results.root > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_11.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar1_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_12.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar2_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_13.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar3_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_14.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar4_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_15.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar5_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_16.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar6_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_17.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar7_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_18.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar8_results.root  > hists/" >> .jobs.txt
echo " ./doAnalysis -t newTree -o hists/output_tt1l_19.root          -s 0.0032409      -i /hadoop/cms/store/user/yxiang/Baby/${BABYVERSION}/ttbarfromtbar9_results.root  > hists/" >> .jobs.txt

xargs.sh .jobs.txt
