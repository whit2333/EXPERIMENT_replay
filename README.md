EXPERIMENT replay 
=================

This repo should be forked by EXPERIMENT, renamed (eg EXPERIMENT->SIDIS), and 
setup a new "upstream" remote. (Please keep the commit history so new 
non-experiment specific features can always be pushed upstream).

This repo was forked from `UTIL_SIDIS` repository and all files were moved into 
the `UTIL` directory.



## Note about submodules

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



## Tips 

```
ln -s /work/hallc/c-csv/work/rootfiles
ln -s /cache/mss/hallc/blab/raw

.
├── CALIBRATION -> hallc_CALIBRATION/CALIBRATION
├── DBASE -> hallc_replay/DBASE
├── DEF-files -> hallc_replay/DEF-files
├── MAPS -> hallc_replay/MAPS
├── PARAM -> hallc_replay/PARAM/
├── raw -> /home/whit/globus_files/hallc/raw/
├── rootfiles -> /home/whit/globus_files/hallc/rootfiles/

```
