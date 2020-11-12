

FILEPATH=/hadoop/cms/store/group/snt/nanoaod/mc/RunIIFall17NanoAODv7/TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/NANOAODSIM/PU2017_12Apr2018_Nano02Apr2020_102X_mc2017_realistic_v8-v1/20000/91DA3343-F0A0-4345-A7E5-CE8CF5A7889D.root

mkdir -p outputs/

rm outputs/output_ttw_*.root
rm outputs/signal.root

echo "" > .jobs.txt
for i in $(seq 0 15); do
    echo "./doAnalysis -i ${FILEPATH} -t Events -o outputs/output_ttw_${i}.root -s 0.000076266 -j 16 -I ${i}" >> .jobs.txt
done

echo " ./doAnalysis -i /hadoop/cms/store/user/yxiang/Baby/Baby_v2/sig_results.root -t newTree -o outputs/signal.root -s 0.00000017131" >> .jobs.txt

# ttW (W->lv)    0.2043    2.67878e+06    0.000076266

xargs.sh .jobs.txt

hadd -f outputs/output_ttw.root outputs/output_ttw_*.root
