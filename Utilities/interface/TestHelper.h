#ifndef Utilities_TestHelper
#define Utilities_TestHelper
// -*- C++ -*-

//------------------------------------------------------------
//
// Function to drive test programs and scripts.
//
// Write your test program with whatever name you want; the 
// implementation should be:
//
//    int main(int argc, char* argv[]) { return ptomaine(argc, argv); }
//    
//
// Asumming you call your program RunThis, invocation of this program
// should look like:
//
//   RunThis <shell name> <command> [args ...]
// e.g.
//   RunThis /bin/bash ls
//   RunThis /bin/bash cmsRun -p somefile.cfg
//   RunThis /bin/bash some_script.sh a b c
//
//
// $Id$
//------------------------------------------------------------

int ptomaine(int argc, char* argv[]);

#define RUNTEST() int main(int argc, char* argv[]) { return ptomaine(argc, argv); }

#endif
