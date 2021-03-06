#!/usr/bin/env python

import os, sys, string, re
#from ROOT import *  # importing ROOT takes a while, so do it only in __main__?
from array import array
from optparse import OptionParser
import MonitorElementsToCheck as DQMME


class RunValue:

    def __init__(self,run=0,value=0,error=0):
        self.run=run
        self.value=value
        self.error=error


def SortRuns(a,b):
    if (a.run>b.run):
        return 1
    elif (a.run<b.run):
        return -1
    else:
        return 0


def MainProgram(runlistdict={},outputdir="DQMOutput",debug=False,
                histmode=False):
    ''' This generates the history histograms, using all the run files in runlistdict, and all the histograms defined in DQMME.dqmMEs.'''
    if not os.path.isdir(outputdir):
        os.mkdir(outputdir)
    
    y_arrays_transposed = []
    ey_arrays_transposed = []
    x_arrays = []
    ex_arrays =[]

    for i in runlistdict.keys():
      print ("Processing %s"%(runlistdict[i]))
      y_tr = []
      ey_tr = []
      try:
        inputFile = TFile(runlistdict[i])
        hist = TH1F()
        for j in range(len(DQMME.dqmMEs)):
          histoName = ("DQMData/Run %i/JetMET/Run summary/"%(i)) + DQMME.dqmMEs[j].ME
          # At some point, add a value into dqmMEs so that those histograms automatically have SetMinimum(0) called?
          try:
            hist=inputFile.Get(histoName)
            if(DQMME.dqmMEs[j].extract=="mean"):
              y_tr.append(hist.GetMean())
              ey_tr.append(hist.GetMeanError())
            elif(DQMME.dqmMEs[j].extract=="rms"):
              y_tr.append(hist.GetRMS())
              ey_tr.append(hist.GetRMSError())
          except:
            print ("Could not find " + histoName + " in " + runlistdict[i])
            continue
      except:
        print ("Could not open " + runlistdict[i])
        continue
    
      if(len(y_tr)!=len(DQMME.dqmMEs) or len(ey_tr)!=len(DQMME.dqmMEs)):
        print ("Could not find some Monitor Elements. Please check that their names were typed in correctly")
        print ("Aborting")
        sys.exit()
      x_arrays.append(i)
      ex_arrays.append(0)
      y_arrays_transposed.append(y_tr)
      ey_arrays_transposed.append(ey_tr)

    if(len(y_arrays_transposed)!=len(runlistdict.keys()) or len(ey_arrays_transposed)!=len(runlistdict.keys())):
      print ("Could not open some DQM root files. These files might be corrupted")
      print ("Aborting")
      sys.exit()

    gROOT.SetBatch(kTRUE);
    gStyle.SetCanvasColor(0)
    gStyle.SetCanvasBorderMode(0)
    gStyle.SetPadColor(0)
    gStyle.SetPadBorderMode(0)
    gStyle.SetFrameBorderMode(0)
    gStyle.SetPadTickX(1)
    gStyle.SetPadTickY(1)
    gStyle.SetPalette(1)

    keys=runlistdict.keys()
    for i in range(len(DQMME.dqmMEs)):
      y = []
      ey=[]
      for j in range(len(runlistdict.keys())):
        y.append(y_arrays_transposed[j][i])
        ey.append(ey_arrays_transposed[j][i])
      c = TCanvas("c")
      c.cd()

      if histmode==False:
          graph = TGraphErrors(len(x_arrays),array('d',x_arrays),array('d',y),array('d',ex_arrays),array('d',ey))
          graph.SetTitle(DQMME.dqmMEs[i].title)
          graph.GetXaxis().SetTitle("Run Number")
          #graph.GetXaxis().LabelsOption("v")

      else:
          runs=[]
          for z in range(len(x_arrays)):
              runs.append(RunValue(run=x_arrays[z],value=y[z],error=ey[z]))
          #runs.sort(SortRuns)
          c.SetBottomMargin(0.3)
          
          graph=TH1F(DQMME.dqmMEs[i].title,
                     "%s_%i_to_%i"%(DQMME.dqmMEs[i].title,
                                    runs[0].run,
                                    runs[-1].run),
                     len(runs),runs[0].run-0.5,runs[-1].run+0.5)
          graph.GetXaxis().SetTitle("Run Number")
          graph.GetXaxis().SetTitleSize(0.03)
          graph.GetYaxis().SetTitleSize(0.03)
          graph.GetYaxis().SetLabelSize(0.03)
          graph.GetXaxis().SetTitleOffset(2.5)
          graph.GetYaxis().SetTitleOffset(2.)
          
          for z in range(len(runs)):
              graph.SetBinContent(z+1, runs[z].value)
              graph.SetBinError(z+1, runs[z].error)
              label="%i"%runs[z].run
              graph.GetXaxis().SetBinLabel(z+1,label)
          graph.GetXaxis().LabelsOption("v")
          
      graph.GetYaxis().SetTitle(DQMME.dqmMEs[i].ytitle)
      graph.SetMarkerStyle(20)
      graph.SetMarkerColor(kRed)
      if DQMME.dqmMEs[i].ymin<>None:
          graph.SetMinimum(DQMME.dqmMEs[i].ymin)
      if DQMME.dqmMEs[i].ymax<>None:
          graph.SetMaximum(DQMME.dqmMEs[i].ymax)
      if histmode==False:
          graph.Draw("ap")
      else:
          graph.Draw()
      c.SaveAs("%s.%s"%(os.path.join(outputdir,DQMME.dqmMEs[i].title), DQMME.dqmMEs[i].format))

      del c

    print ("Output files successfully created")
    return





def SearchDir(dir=None,dict={},subdircount=0,maxrecursion=2,debug=False):
    ''' This will search a directory and all its subdirectories (up to a maximum recursion level of "maxrecursion") for files of the type "*_R*.root". Any time in which multiple files are found for one run, an error message will be thrown, and the program will exit.'''

    # 'dict' stores the list of files for the runs, with each run used as a key
    subdircount=subdircount+1
    if (subdircount>maxrecursion):  # set maximum recursion level
        return dict
    if not os.path.isdir(dir):
        print("Sorry, directory '%s' does not exist -- skipping!"%dir)
        return dict
    for i in os.listdir(dir):  # Search all files in directory
        if os.path.isfile(os.path.join(dir,i)):
            # not sure about the re.search value here -- why not use string .endswith?
            if(i.find("_R")==-1 or not re.search(".root$",i)):  # look only at root files
                continue

            # Get the run number from the root file name
            try:
                run=string.atoi(i[(i.find("_R")+2):(i.find("_R")+11)])
            except:
                print("Unable to parse run number from file '%s'"%os.path.join(dir,i))
                continue
            # Add run to dictionary
            if run not in dict.keys():
                dict[run]=os.path.join(dir,i)
            else:
                print ("ERROR!  Run #%i has multiple files!"%run)
                print ("File #1:  ",dict[run])
                print ("File #2:  ",os.path.join(dir,i))
                sys.exit()

        # If subdirectory found, process files in that subdirectory
        elif os.path.isdir(os.path.join(dir,i)):
            dict=SearchDir(dir=os.path.join(dir,i),
                           dict=dict,
                           subdircount=subdircount,
                           maxrecursion=maxrecursion)
    return dict


#####################################################################




if __name__=="__main__":

    parser=OptionParser()
    parser.add_option("-r","--run",
                      action="append",
                      default=[],
                      type="int",
                      dest="run",
                      help="Enter run(s) to be evaluated")
    parser.add_option("-f","--reference",
                      action="append",
                      default=[],
                      type="int",
                      dest="referencerun",
                      help="Enter reference run(s) to be evaluated")
    parser.add_option("-R","--runlistfile",
                      default=None,
                      dest="runlistfile",
                      help="use a text file of line-separated run numbers for the run list")
    parser.add_option("-m","--runRangeMode",
                      default=False,
                      dest="runRangeMode",
                      action="store_true",
                      help="If set true, all runs in the range [min(run), max(run)] will be evaluated.")
    parser.add_option("-d","--dir",
                      dest="fileLocations",
                      action="append",
                      default=[],
                      #default="/home/hatake/DQM_ROOT_files",
                      help="Specify directory containing root files.  If none specified, will used default of /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress on lxplus and /home/hatake/DQM_ROOT_files elsewhere")
    parser.add_option("-z","--recursionlevel",
                      dest="recursionlevel",
                      type="int",
                      default=2,
                      help="Specify recursion level of subdirectories in fileLocations in which to check for root files")
    parser.add_option("-o","--outputdir",
                      dest="outputdir",
                      default="DQMOutput",
                      help="Specify output directory name")
    parser.add_option("-v","--verbose",
                      dest="verbose",
                      default=False,
                      action="store_true",
                      help="Turn on debugging info")
    parser.add_option("-s","--histmode",
                      dest="histmode",
                      default=False,
                      action="store_true",
                      help="If set active, will display history plots as TH1 histograms.  (If not active, plots will be generated as TGraphs.)")
    parser.add_option("-H","--Help",
                      dest="Help",
                      action="store_true",
                      default=False,
                      help="alternate help action")

    (options,args)=parser.parse_args()
    if options.Help==True:
        parser.print_help()
        sys.exit()

    print (" Looking for runs...")

    # Get list of runs to use from command line
    runs=options.run
    runs.sort()

    reference=options.referencerun
    
    # Get runs from txt file, if provided
    if options.runlistfile<>None:
        if not os.path.isfile(options.runlistfile):
            print ("Sorry, run file '%s' does not exist!"%options.runlistfile)
        else:
            temp=open(options.runlistfile,'r')
            for i in temp:
                try:
                    thisrun=string.strip(i)
                    thisrun=string.atoi(thisrun)
                    if thisrun>0:
                        runs.append(thisrun)
                    else:
                        reference.append(abs(thisrun))
                except:
                    print "Error:  Cannot parse line '%s' in file %s"%(i,options.runlistfile)


    # No runs specified
    if len(runs)==0:
        print ("Sorry, no valid runs have been specified on command line!")
        sys.exit()

    # If runRangeMode==True, make list of all runs in a given range
    if options.runRangeMode==True:
        if len(runs)<2:  # need at least 2 runs to make a range
            print ("You must specify at least two runs to make a valid run range!")
            sys.exit()
        newruns=[]  # new list that includes all runs in range
        for i in range(runs[0],runs[-1]+1):  # runs have already been sorted
            newruns.append(i)
        runs=newruns  # replace original runs with inclusive list



    runs.sort()  
    outputdir=options.outputdir
    if outputdir=="DQMOutput":
        outputdir="DQMOutput_%i_%i"%(runs[0],runs[-1])
        
    if len(options.fileLocations)==0:
        env=os.getenv("HOSTNAME")
        if env.startswith("lxplus") and env.endswith(".cern.ch"):
            options.fileLocations=["/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OfflineData/Run2010/StreamExpress"]
        else:
            options.fileLocations=["/home/hatake/DQM_ROOT_files"]
            
    if not os.path.isdir(outputdir):
        os.mkdir(outputdir)
        print "Made directory %s"%outputdir
    RunFile=open(os.path.join(outputdir,"RunsToBeCertified.txt"),'w')
    for r in runs:
        RunFile.write("%s\n"%r)
    RunFile.close()
    RefFile=open(os.path.join(outputdir,"RunsUsedAsReference.txt"),'w')
    for f in reference: 
        runs.append(f)
        RefFile.write("%s\n"%f)
    RefFile.close()
    runs.sort()

    # Now let's check files
    runlistdict = {}

    print ("Found a total of %i runs"%len(runs))
    allfiles={}
    for dir in options.fileLocations:  # Search all directories

        thisdir=SearchDir(dir=dir,
                          maxrecursion=options.recursionlevel,
                          debug=options.verbose)
        for i in thisdir.keys():
            if i not in allfiles.keys():
                allfiles[i]=thisdir[i]
            else:
                print "ERROR!  Run already found! ",i, thisdir[i]

    for run in allfiles.keys():
        if (options.runRangeMode==True):
            # Check if run is in ran
            if (run in runs):
                if(run in runlistdict.keys()):
                    print ("Multiple DQM files found for run %i"%(run))
                    print ("Aborting")
                    sys.exit()
                runlistdict[run]=allfiles[run]
        else:
            for j in options.run:
                if(run==j):
                    if(run in runlistdict.keys()):
                        print ("Multiple DQM files found for run %i"%(run))
                        print ("Aborting")
                        sys.exit()
                    runlistdict[run]=allfiles[run]

    if (len(runlistdict)==0):
        print ("No DQM files corresponding to the requested run numbers found in '%s'"%options.fileLocations)
        sys.exit()

    print ("Importing root.  This takes a few seconds...")
    from ROOT import *
    print("Done importing root.  Beginning check of runs...")
    sortedkeys=runlistdict.keys()
    sortedkeys.sort()
    print("Program will check %i runs"%len(sortedkeys))
    print(sortedkeys)
    MainProgram(runlistdict=runlistdict,
                outputdir=outputdir,
                histmode=options.histmode,
                debug=options.verbose)
