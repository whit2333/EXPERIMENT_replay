EXPERIMENT replay 
=================

## Quick Start

### Install and configure
```
git clone --recurse-submodules https://github.com/whit2333/EXPERIMENT_replay.git
mkdir EXPERIMENT_replay/build && cd EXPERIMENT_replay/build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME && make install
```
This installs a few scripts, header files, and cmake config to locate these 
headers. If you install into `$HOME` then make sure you have something like 
this in your `.bashrc`:
```
export PATH=$HOME/bin:$PATH
export LD_LIBRARY_PATH=$HOME/lib:$HOME/lib64:$LD_LIBRARY_PATH
```

### New replay dir

The following will setup a new replay directory in your home directory.

```
mkdir $HOME/my_replay_dir && cd $HOME/my_replay_dir
make_hallc_replay_symlinks \
  -R ~/globus_files/hallc/rootfiles \
  -r ~/globus_files/hallc/raw
tree                                                                                    
.
├── CALIBRATION -> /home/whit/projects/EXPERIMENT_replay/hallc_CALIBRATION/CALIBRATION
├── DBASE -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/DBASE
├── DEF-files -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/DEF-files
├── MAPS -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/MAPS
├── onlineGUI -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/onlineGUI
├── PARAM -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/PARAM
├── raw -> /home/whit/globus_files/hallc/raw
├── ROOTfiles -> /home/whit/globus_files/hallc/rootfiles
├── SCRIPTS -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/SCRIPTS
└── TEMPLATES -> /home/whit/projects/EXPERIMENT_replay/hallc_replay/TEMPLATES
```

### `make_hallc_replay_symlinks` usage
```
make_hallc_replay_symlinks - emulate hallc_replay directory with symbolic links
  Usage:
     make_hallc_replay_symlinks [options]

  options:
     -c, --create         create output directories (REPORT_OUTPUT)
     -r, --raw <dir>      create link to raw directory
     -R, --root <dir>     create link to rootfile directory
     -m, --monitor <dir>  create link to monitoring directory
     -d, --remove         remove symlinks
     -h, --help           show brief help
```


## Tips 

To make the directories look like the original `hallc_replay` run the script 
`make_hallc_replay_symlinks`:
```
$ mkdir -p /tmp/my_replay_dir && cd /tmp/my_replay_dir
make_hallc_replay_symlinks
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






