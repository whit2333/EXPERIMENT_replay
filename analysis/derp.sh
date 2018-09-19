#!/bin/bash

export ROOT_INCLUDE_PATH=/opt/software/local/include:$HOME/include
export DB_DIR=DBASE

SPEC=SHMS
spec=shms
#run_number=3566
#run_number=3372
run_number=3961

#config="hallc_replay/CONFIG/${SPEC}/PRODUCTION/${spec}_coin_production.cfg"

#script="shms_production_all.cxx"
script="analysis/coin_replay.cxx"
#script="analysis/coin_online_test.cxx"

#runHcana="./hcana -q \"${script}(${run_number}, 10000)\"
hcana "${script}(${run_number},100000)"
#root -l  "${script}(${run_number},50000)"
