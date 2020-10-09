#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# $DIR is the path to the directory where this specific script is sitting

sh $DIR/exec.sh
sh $DIR/hadd.sh
python $DIR/cutflow.py
python $DIR/plot.py
