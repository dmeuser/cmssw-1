#!/bin/bash

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

(cmsRun ${LOCAL_TEST_DIR}/test_esproducerlooper.cfg ) || die 'Failure using test_esproducerlooper.cfg' $?
