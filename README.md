EXPERIMENT replay 
=================

## Quick Start

```
git clone --recurse-submodules https://github.com/whit2333/EXPERIMENT_replay.git
mkdir EXPERIMENT_replay/build && cd EXPERIMENT_replay/build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME && make install
```
This  installs a few scripts and a header file.



## Tips 

To make the directories look like the original `hallc_replay` run the script 
`make_symbolic_links`:
```
$ mkdir -p /tmp/my_replay_dir && cd /tmp/my_replay_dir
make_hallc_symlinks
Adding linke CALIBRATION --> hallc_CALIBRATION/CALIBRATION
Adding linke DBASE --> hallc_replay/DBASE
Adding linke DEF-files --> hallc_replay/DEF-files
Adding linke MAPS --> hallc_replay/MAPS
Adding linke PARAM --> hallc_replay/PARAM
Adding linke TEMPLATES --> hallc_replay/TEMPLATES
Adding linke SCRIPTS --> hallc_replay/SCRIPTS
```

## About 

This repo should be forked by EXPERIMENT, renamed (eg EXPERIMENT->SIDIS), and 
setup a new "upstream" remote. (Please keep the commit history so new 
non-experiment specific features can always be pushed upstream).

This repo was forked from `UTIL_SIDIS` repository and all files were moved into 
the `UTIL` directory.



### Note about submodules

Notice that on github, there is an @COMMIT hash after `hallc_replay` and 
`hallc_CALIBRATIONS`.  This means that a specific commit is used, that is it 
will never just grab the master. It can be changed by doing the following:

```
cd hallc_replay
git checkout COMMITHASH # or pull master
cd ..
git add hallc_replay
git commit -m "updated the hallc_replay commit used"
```

In each `EXPERIMENT_replay`'s commit, a unique submodule commit is also 
specified, therefore,  if `hallc_CALIBRATIONS` is  updated for the latest 
running experiment this will not affect the calibrations being used in an older 
experiment.  You will need to checkout the desired submodule-commit and commit 
it to the older `EXPERIMENT_replay` (like above).





