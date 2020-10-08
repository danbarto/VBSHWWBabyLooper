rm .jobs.txt
echo "hadd -f hadds/ssww.root hists/output_ssww.root" >> .jobs.txt
echo "hadd -f hadds/tt1l.root hists/output_tt1l_*.root" >> .jobs.txt
echo "hadd -f hadds/tt2l.root hists/output_tt2l_*.root" >> .jobs.txt
echo "hadd -f hadds/tth.root hists/output_tth.root" >> .jobs.txt
echo "hadd -f hadds/ttw.root hists/output_ttw.root" >> .jobs.txt
echo "hadd -f hadds/ttz.root hists/output_ttz.root" >> .jobs.txt
echo "hadd -f hadds/vbshww.root hists/output_vbshww.root" >> .jobs.txt
echo "hadd -f hadds/wz.root hists/output_wz.root" >> .jobs.txt
echo "hadd -f hadds/lambda20_vbshww.root hists/output_lambda20_vbshww.root" >> .jobs.txt
echo "hadd -f hadds/lambdam20_vbshww.root hists/output_lambdam20_vbshww.root" >> .jobs.txt
xargs.sh .jobs.txt
