#!/bin/bash

# $DIR is the path to the directory where this specific script is sitting
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Help
usage()
{
  echo "ERROR - Usage:"
  echo
  echo "      sh $(basename $0) OPTIONSTRINGS ..."
  echo
  echo "Options with arguments:"
  echo "  -h    Help                   (Display this message)"
  echo "  -d    nano skim dir          (Path to the NanoSkim outputs)"
  echo
  exit
}

# Command-line opts
while getopts ":d:h" OPTION; do
  case $OPTION in
    d) NANOSKIMDIR=${OPTARG};;
    h) usage;;
    :) usage;;
  esac
done

if [ -z ${NANOSKIMDIR} ]; then usage; fi

# to shift away the parsed options
shift $(($OPTIND - 1))

# Verbose
date
echo "================================================"
echo "$(basename $0) $*"
echo "$(basename $0) $*" >> $DIR/.$(basename $0).history
echo "------------------------------------------------"
echo "NANOSKIMDIR    : ${NANOSKIMDIR}"
echo "================================================"

for SAMPLEDIR in $(ls -d ${NANOSKIMDIR}/*); do
    echo ${SAMPLEDIR}
    mkdir -p ${SAMPLEDIR}/merged/
    if [ ! -f ${SAMPLEDIR}/merged/nevents.txt ]; then
        TOTALNEVENTS=0
        TOTALNEFFEVENTS=0
        for NEVENTSFILE in $(ls $SAMPLEDIR/*_nevents.txt); do
            POSWEIGHTS=$(tail -n 2 $NEVENTSFILE | head -n 1)
            NEGWEIGHTS=$(tail -n 1 $NEVENTSFILE)
            TOTALNEVENTS=$((TOTALNEVENTS + ${POSWEIGHTS}))
            TOTALNEVENTS=$((TOTALNEVENTS + ${NEGWEIGHTS}))
            TOTALNEFFEVENTS=$((TOTALNEFFEVENTS + ${POSWEIGHTS}))
            TOTALNEFFEVENTS=$((TOTALNEFFEVENTS - ${NEGWEIGHTS}))
            # echo ${POSWEIGHTS} ${NEGWEIGHTS}
        done
        echo ${TOTALNEVENTS} > nevents.txt
        echo ${TOTALNEFFEVENTS} >> nevents.txt
        cp nevents.txt ${SAMPLEDIR}/merged/
    fi
    # hadding
    if [ ! -f ${SAMPLEDIR}/merged/output.root ]; then
        python haddnano.py output.root ${SAMPLEDIR}/*.root
        rsync -ah --progress output.root ${SAMPLEDIR}/merged/output.root
        # SAMPLEDIRPATHNEW=$(echo ${SAMPLEDIR} | sed 's/^.*\(\/store.*\).*$/\1/')
        # COPY_SRC="file://`pwd`/output.root"
        # COPY_DEST="davs://redirector.t2.ucsd.edu:1094/${SAMPLEDIRPATHNEW}/merged/output.root"
        # echo "gfal-copy -p -f -t 4200 --verbose --checksum ADLER32 ${COPY_SRC} ${COPY_DEST}"
        # break
    fi
done
