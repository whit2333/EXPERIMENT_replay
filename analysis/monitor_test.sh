#!/bin/bash

export ROOT_INCLUDE_PATH=/opt/software/local/include:$HOME/include
export DB_DIR=hallc_replay/DBASE

#run_number=3214
#run_number=3566
#run_number=4067
run_number=3547

#config="hallc_replay/CONFIG/${SPEC}/PRODUCTION/${spec}_coin_production.cfg"

#script="shms_production_all.cxx"
script="coin_online_test.cxx"

#runHcana="./hcana -q \"${script}(${run_number}, 10000)\"
hcana "${script}(${run_number},5000)"
#root -l  "${script}(${run_number},50000)"
