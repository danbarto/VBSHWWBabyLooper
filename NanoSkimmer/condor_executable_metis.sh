#!/bin/bash

# Parse the arguments
OUTPUTDIR=$1
OUTPUTNAME=$2
INPUTFILENAMES=$3
IFILE=$4
# CMSSWVERSION=$5 # We will be overriding by hand with the following
# SCRAMARCH=$6 # We will be overriding by hand with the following
CMSSWVERSION=CMSSW_10_0_0
SCRAMARCH=slc6_amd64_gcc700

function getjobad {
    grep -i "^$1" "$_CONDOR_JOB_AD" | cut -d= -f2- | xargs echo
}
function setup_chirp {
    if [ -e ./condor_chirp ]; then
    # Note, in the home directory
        mkdir chirpdir
        mv condor_chirp chirpdir/
        export PATH="$PATH:$(pwd)/chirpdir"
        echo "[chirp] Found and put condor_chirp into $(pwd)/chirpdir"
    elif [ -e /usr/libexec/condor/condor_chirp ]; then
        export PATH="$PATH:/usr/libexec/condor"
        echo "[chirp] Found condor_chirp in /usr/libexec/condor"
    else
        echo "[chirp] No condor_chirp :("
    fi
}
function chirp {
    # Note, $1 (the classad name) must start with Chirp
    condor_chirp set_job_attr_delayed $1 $2
    ret=$?
    echo "[chirp] Chirped $1 => $2 with exit code $ret"
}

# Make sure OUTPUTNAME doesn't have .root since we add it manually
OUTPUTNAME=$(echo $OUTPUTNAME | sed 's/\.root//')

setup_chirp

echo -e "\n--- begin header output ---\n" #                     <----- section division
echo "OUTPUTDIR: $OUTPUTDIR"
echo "OUTPUTNAME: $OUTPUTNAME"
echo "INPUTFILENAMES: $INPUTFILENAMES"
echo "IFILE: $IFILE"
echo "CMSSWVERSION: $CMSSWVERSION"
echo "SCRAMARCH: $SCRAMARCH"

echo "GLIDEIN_CMSSite: $GLIDEIN_CMSSite"
echo "hostname: $(hostname)"
echo "uname -a: $(uname -a)"
echo "time: $(date +%s)"
echo "args: $@"

echo -e "\n--- end header output ---\n" #                       <----- section division

if [ -r "$OSGVO_CMSSW_Path"/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSGVO_CMSSW_Path/cmsset_default.sh"
    source "$OSGVO_CMSSW_Path"/cmsset_default.sh
elif [ -r "$OSG_APP"/cmssoft/cms/cmsset_default.sh ]; then
    echo "sourcing environment: source $OSG_APP/cmssoft/cms/cmsset_default.sh"
    source "$OSG_APP"/cmssoft/cms/cmsset_default.sh
elif [ -r /cvmfs/cms.cern.ch/cmsset_default.sh ]; then
    echo "sourcing environment: source /cvmfs/cms.cern.ch/cmsset_default.sh"
    source /cvmfs/cms.cern.ch/cmsset_default.sh
else
    echo "ERROR! Couldn't find $OSGVO_CMSSW_Path/cmsset_default.sh or /cvmfs/cms.cern.ch/cmsset_default.sh or $OSG_APP/cmssoft/cms/cmsset_default.sh"
    exit 1
fi

# Setup environment and build
export SCRAM_ARCH=${SCRAMARCH} && scramv1 project CMSSW ${CMSSWVERSION}
cd CMSSW_10_0_0/src/
tar xvf ../../package.tar.gz
cd PhysicsTools/NanoAODTools/
eval `scramv1 runtime -sh`
scram b

echo "before running: ls -lrth"
ls -lrth 

echo -e "\n--- begin running ---\n" #                           <----- section division

# # Download the file (prefetching) NOTE: The preferred solution is the XCache via "xcache-redirector.t2.ucsd.edu:2040" redirector
# echo -e "\n--- begin downloading via xrdcp ---\n" #                           <----- section division
# input=$(echo "${INPUTFILENAMES}" | sed 's/^.*\(\/store.*\).*$/\1/')
# dest="${input/\/store\//}"
# dest=$(dirname $dest)
# mkdir -p $dest
# echo $dest
# # if [ $# -gt 1 ]; then
# #     dest=$2
# # fi
# echo "Begin xrdcp"
# echo "Running... xrdcp root://xrootd.unl.edu/$input $dest"
# xrdcp root://xrootd.unl.edu/$input $dest
# echo "Done xrdcp"
# echo -e "\n--- end downloading via xrdcp ---\n" #                           <----- section division
# Get local filepath name
# localpath=$(echo ${INPUTFILENAMES} | sed 's/^.*\(\/store.*\).*$/\1/')
# localpath="${localpath/\/store\//}"
# echo "Input file name:" ${localpath}
# INPUTFILE=${localpath}

INPUTFILE=""
for i in $(echo ${INPUTFILENAMES} | tr ',' ' '); do
    localpath=$(echo ${i} | sed 's/^.*\(\/store.*\).*$/\1/')
    if [ -z ${INPUTFILE} ]; then
        INPUTFILE=root://xcache-redirector.t2.ucsd.edu:2040/${localpath}
    else
        INPUTFILE=${INPUTFILE},root://xcache-redirector.t2.ucsd.edu:2040/${localpath}
    fi
done
echo ${INPUTFILE}

# Figuring out nevents and neff_weights
echo 'void count_events(TString filename)                                                                                                                       ' > count_events.C
echo '{                                                                                                                                                         ' >> count_events.C
echo '    std::vector<TString> out;                                                                                                                             ' >> count_events.C
echo '    TObjArray *list = filename.Tokenize(",");                                                                                                             ' >> count_events.C
echo '    for (unsigned i = 0; i < (unsigned)list->GetEntries(); ++i)                                                                                           ' >> count_events.C
echo '    {                                                                                                                                                     ' >> count_events.C
echo '        TString token = ((TObjString*)list->At(i))->GetString();                                                                                          ' >> count_events.C
echo '        out.push_back(token);                                                                                                                             ' >> count_events.C
echo '    }                                                                                                                                                     ' >> count_events.C
echo '                                                                                                                                                          ' >> count_events.C
echo '    unsigned int ntot = 0;                                                                                                                                ' >> count_events.C
echo '    unsigned int npos = 0;                                                                                                                                ' >> count_events.C
echo '    unsigned int nneg = 0;                                                                                                                                ' >> count_events.C
echo '                                                                                                                                                          ' >> count_events.C
echo '    for (unsigned int ifilename = 0; ifilename < out.size(); ++ifilename)                                                                                 ' >> count_events.C
echo '    {                                                                                                                                                     ' >> count_events.C
echo '         TString fname = out.at(ifilename);                                                                                                               ' >> count_events.C
echo '         TFile* f = TFile::Open(fname);                                                                                                                   ' >> count_events.C
echo '         TTree* Events = (TTree*) f->Get("Events");                                                                                                       ' >> count_events.C
echo '         TTree* t = (TTree*) f->Get("Events");                                                                                                            ' >> count_events.C
echo '         ntot += Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "", "goff");                                                                   ' >> count_events.C
echo '         npos += Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))>0", "goff");                      ' >> count_events.C
echo '         nneg += Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))<0", "goff");                      ' >> count_events.C
echo '    }                                                                                                                                                     ' >> count_events.C
echo '                                                                                                                                                          ' >> count_events.C
echo '    std::cout << ntot << std::endl;                                                                                                                       ' >> count_events.C
echo '    std::cout << npos-nneg << std::endl;                                                                                                                  ' >> count_events.C
echo '                                                                                                                                                          ' >> count_events.C
echo '}                                                                                                                                                         ' >> count_events.C
# TChain* Events = new TChain("Events"); ch->Add(filename.Data()); std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "", "goff") << std::endl; std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))>0", "goff") << std::endl;  std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))<0", "goff") << std::endl; }' > count_events.C
root -l -b -q count_events.C\(\"${INPUTFILE}\"\) | tee nevents.txt

# Run the postprocessor
CMD="python scripts/nano_postproc.py \
    ./ ${INPUTFILE} \
    -b python/postprocessing/examples/keep_and_drop.txt \
    -I PhysicsTools.NanoAODTools.postprocessing.examples.vbsHwwSkimModule vbsHwwSkimModuleConstr"
echo $CMD
$CMD

RUN_STATUS=$?

if [[ $RUN_STATUS != 0 ]]; then
    echo "Removing output file because scripts/nano_postproc.py crashed with exit code $?"
    rm ${NANOPOSTPROCOUTPUTFILENAME}_Skim.root
    exit 1
fi

# Rename the output file
NANOPOSTPROCOUTPUTFILE=$(basename ${localpath})
NANOPOSTPROCOUTPUTFILENAME=${NANOPOSTPROCOUTPUTFILE%.root}
echo "Renaming the output file"
echo "mv ${NANOPOSTPROCOUTPUTFILENAME}_Skim.root output.root"
mv ${NANOPOSTPROCOUTPUTFILENAME}_Skim.root output.root

# Figuring out nevents and neff_weights
echo 'void count_events(TString filename) { TFile* f = TFile::Open(filename.Data()); TTree* Events = (TTree*) f->Get("Events"); std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "", "goff") << std::endl; std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))>0", "goff") << std::endl;  std::cout << Events->Draw("(Generator_weight>0)-(Generator_weight<0)", "((Generator_weight>0)-(Generator_weight<0))<0", "goff") << std::endl; }' > count_events.C
root -l -b -q count_events.C\(\"output.root\"\) | tee nevents_skimmed.txt

echo -e "\n--- end running ---\n" #                             <----- section division

echo "after running: ls -lrth"
ls -lrth

echo -e "\n--- begin copying output ---\n" #                    <----- section division
echo "Sending output file $OUTPUTNAME.root"
# Get local filepath name
OUTPUTDIRPATHNEW=$(echo ${OUTPUTDIR} | sed 's/^.*\(\/store.*\).*$/\1/')

# # Copying the output file
# COPY_SRC="file://`pwd`/${OUTPUTNAME}.root"
# COPY_DEST="https://redirector.t2.ucsd.edu:1094//${OUTPUTDIRPATHNEW}/${OUTPUTNAME}_${IFILE}.root"
# echo "Running: env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
# env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}
# COPY_STATUS=$?
# if [[ $COPY_STATUS != 0 ]]; then
#     echo "Removing output file because gfal-copy crashed with code $COPY_STATUS"
#     env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-rm --verbose ${COPY_DEST}
#     REMOVE_STATUS=$?
#     if [[ $REMOVE_STATUS != 0 ]]; then
#         echo "Uhh, gfal-copy crashed and then the gfal-rm also crashed with code $REMOVE_STATUS"
#     fi
# fi

# # Copying n events
# COPY_SRC="file://`pwd`/nevents.txt"
# COPY_DEST="https://redirector.t2.ucsd.edu:1094//${OUTPUTDIRPATHNEW}/${OUTPUTNAME}_${IFILE}_nevents.txt"
# echo "Running: env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
# env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}
# COPY_STATUS=$?
# if [[ $COPY_STATUS != 0 ]]; then
#     echo "Removing output file because gfal-copy crashed with code $COPY_STATUS"
#     env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-rm --verbose ${COPY_DEST}
#     REMOVE_STATUS=$?
#     if [[ $REMOVE_STATUS != 0 ]]; then
#         echo "Uhh, gfal-copy crashed and then the gfal-rm also crashed with code $REMOVE_STATUS"
#     fi
# fi

# # Copying n events skimmed
# COPY_SRC="file://`pwd`/nevents_skimmed.txt"
# COPY_DEST="https://redirector.t2.ucsd.edu:1094//${OUTPUTDIRPATHNEW}/${OUTPUTNAME}_${IFILE}_nevents_skimmed.txt"
# echo "Running: env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
# env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}
# COPY_STATUS=$?
# if [[ $COPY_STATUS != 0 ]]; then
#     echo "Removing output file because gfal-copy crashed with code $COPY_STATUS"
#     env -i X509_USER_PROXY=${X509_USER_PROXY} gfal-rm --verbose ${COPY_DEST}
#     REMOVE_STATUS=$?
#     if [[ $REMOVE_STATUS != 0 ]]; then
#         echo "Uhh, gfal-copy crashed and then the gfal-rm also crashed with code $REMOVE_STATUS"
#     fi
# fi
# echo -e "\n--- end copying output ---\n" #                    <----- section division


# echo -e "\n--- begin cleaning area ---\n" #                    <----- section division

# echo "rm -rf mc/"
# rm -rf mc/

# echo -e "\n--- end cleaning output ---\n" #                    <----- section division
