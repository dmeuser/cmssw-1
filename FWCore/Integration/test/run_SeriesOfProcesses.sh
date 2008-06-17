#!/bin/bash

test=testSeriesOfProcesses

function die { echo Failure $1: status $2 ; exit $2 ; }

pushd ${LOCAL_TMP_DIR}

  cmsRun -p ${LOCAL_TEST_DIR}/${test}HLT_cfg.py || die "cmsRun ${test}HLT_cfg.py" $?

  cmsRun -p ${LOCAL_TEST_DIR}/${test}PROD_cfg.py || die "cmsRun ${test}PROD_cfg.py" $?

  cmsRun -p ${LOCAL_TEST_DIR}/${test}TEST_cfg.py || die "cmsRun ${test}TEST_cfg.py" $?

  cmsRun -p ${LOCAL_TEST_DIR}/${test}TEST1_cfg.py || die "cmsRun ${test}TEST1_cfg.py" $?

  cmsRun -p ${LOCAL_TEST_DIR}/${test}TEST2_cfg.py 2> ${test}TEST2.txt
  grep "Duplicate Process" ${test}TEST2.txt || die "cmsRun ${test}TEST2_cfg.py" $?

popd

exit 0
