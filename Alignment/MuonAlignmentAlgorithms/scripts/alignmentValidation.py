#! /usr/bin/env python

import re,os,sys,shutil
import optparse

from mutypes import *

execfile("plotscripts.py")

ROOT.gROOT.SetBatch(1);

######################################################

######################################################
# To parse commandline args

usage='%prog [options]\n'+\
  'This script dumps muon alignment validation plots '+\
  '(see https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonAlignValidationPlots) '+\
  'in web-friendly png format into a predefined directory structure '+\
  'and also (to be implemented) does some quality checks for these plots.\n'+\
  'Script uses output of the first and last iterations of muon alignment runs '+\
  'that should be located in inputDir+i1 and inputDir+iN directories (see options descriptions). '+\
  'Each of these directories should contain\ni#prefix+"_plotting.root"\ni#prefix+".root"\ni#prefix+"_report.py"\n'+\
  'files, where # = 1 or N.\n'+\
  'Output will be located in outputDir, which must exist. Plots from the first and last iterations '+\
  'will be in outputDir+"iter1" and outputDir+"iterN" respectively, and plots relevant for all iterations will '+\
  'reside in outputDir+"common/".\n'+\
  'For a first run a --createDirSructure option must be present. If this option is present for subsequent runs '+\
  'with the same outputDir, all previous results in "iter1", "iterN" and and "common" would be deleted!\n'+\
  'If neither --dt or --csc is present, plots for both systems will be created.\n'+\
  'Options must include either -a or any of the following: --map, --segdiff, --fit, --median'

parser=optparse.OptionParser(usage)

parser.add_option("-l", "--runLabel",
  help="[REQUIRED] label to use for a run",
  type="string",
  default='',
  dest="runLabel")

parser.add_option("-i", "--inputDir",
  help="[REQUIRED] input directory: should contain directories with the first and the final iterations' results",
  type="string",
  default='',
  dest="inputDir")

parser.add_option("--i1",
  help="[REQUIRED] directory with the alignment 1st iteration's results relative to inputDir",
  type="string",
  default='',
  dest="i1")

parser.add_option("--iN",
  help="[REQUIRED] directory with the alignment last iteration's results relative to inputDir",
  type="string",
  default='',
  dest="iN")

parser.add_option("--i1prefix",
  help="filename prefix for the alignment 1st iteration's results. If not provided, i1prefix = i1",
  type="string",
  default='',
  dest="i1prefix")

parser.add_option("--iNprefix",
  help="filename prefix for the alignment last iteration's results. If not provided, iNprefix = iN",
  type="string",
  default='',
  dest="iNprefix")

parser.add_option("-o", "--outputDir",
  help="output directory: all plots will be saved with relation to outputDir. If not provided, consider outputDir = inputDir",
  type="string",
  default='',
  dest="outputDir")

parser.add_option("--createDirSructure",
  help="If present, new directory structure for storing plots will be first created for each iteration at outputDir+i1 and outputDir+iN. WARNING: this will delete any existing results!",
  action="store_true",
  default=False,
  dest="createDirSructure")

parser.add_option("--dt",
  help="If it is present, but not --csc, DT only plots will be created",
  action="store_true",
  default=False,
  dest="dt")

parser.add_option("--csc",
  help="If this is present, but not --dt, CSC only plots will be created",
  action="store_true",
  default=False,
  dest="csc")

parser.add_option("-a","--all",
  help="If present, all types of plots will be created",
  action="store_true",
  default=False,
  dest="all")

parser.add_option("--map",
  help="If present, map plots will be created",
  action="store_true",
  default=False,
  dest="map")

parser.add_option("--segdiff",
  help="If present, segdiff plots will be created",
  action="store_true",
  default=False,
  dest="segdiff")

parser.add_option("--curvature",
  help="If present, curvature plots will be created",
  action="store_true",
  default=False,
  dest="curvature")

parser.add_option("--fit",
  help="If present, fit functions plots will be created",
  action="store_true",
  default=False,
  dest="fit")

parser.add_option("--median",
  help="If present, median plots will be created",
  action="store_true",
  default=False,
  dest="median")

parser.add_option("--diagnostic",
  help="If present, will run diagnostic checks",
  action="store_true",
  default=False,
  dest="diagnostic")

parser.add_option("-v", "--verbose",
  help="Degree of debug info verbosity",
  type="int",
  default=0,
  dest="verbose")

options,args=parser.parse_args()

if options.runLabel=='' or options.inputDir=='' or options.i1=='' or options.iN=='':
  print "\nOne or more of REQUIRED options is missing!\n"
  parser.print_help()
  # See \n"+sys.argv[0]+" --help"
  sys.exit()

outdir = options.outputDir
if outdir=='': outdir = options.inputDir

i1prefix = options.i1prefix
if i1prefix=='' : i1prefix = options.i1

iNprefix = options.iNprefix
if iNprefix=='' : iNprefix = options.iN

if not os.access(outdir,os.F_OK):
  print "\noutDir = "+outdir+"\ndoes not exist! Exiting..."
  sys.exit()

# If neither --dt or --csc is present, plots for both systems will be created
DO_DT  = False
DO_CSC = False
if options.dt or not ( options.dt or options.csc):
  DO_DT = True
if options.csc or not ( options.dt or options.csc):
  DO_CSC = True

if not (options.all or options.map or options.segdiff or options.fit or options.median):
  print "\nOptions must include either -a or any of the following: --map, --segdiff, --fit, --median. Exiting..."
  sys.exit()

DO_MAP = False
DO_SEGDIFF = False
DO_CURVATURE = False
DO_FIT = False
DO_MEDIAN = False
if options.map or options.all:
  DO_MAP = True
if options.segdiff or options.all:
  DO_SEGDIFF = True
if options.curvature or options.all:
  DO_CURVATURE = True
if options.fit or options.all:
  DO_FIT = True
if options.median or options.all:
  DO_MEDIAN = True

DO_DIAGNOSTIC = options.diagnostic

allOptions = "-l "+options.runLabel+" -i "+options.inputDir+" --i1 "+options.i1+" --iN "+options.iN
if options.i1prefix !='': allOptions += " --i1prefix " + options.i1prefix
if options.iNprefix !='': allOptions += " --iNprefix " + options.iNprefix
allOptions += " -o "+options.outputDir
if options.createDirSructure: allOptions += " --createDirSructure"
if DO_DT: allOptions += " --dt"
if DO_CSC: allOptions += " --csc"
if options.all: allOptions += " -a"
if options.map: allOptions += " --map"
if options.segdiff: allOptions += " --segdiff"
if options.curvature: allOptions += " --curvature"
if options.fit: allOptions += " --fit"
if options.median: allOptions += " --median"
if options.diagnostic: allOptions += " --diagnostic"
print sys.argv[0]+" "+allOptions


QUICKTESTN=999



######################################################

# template for canvases list
CANVASES_LIST_TEMPLATE = [
['Common',' ',
 ['medians distribution','medians.png']
],
['DT',' ',
 ['Wheel&Station: map of dxdz residual vs phi','map_DTvsphi_dxdz.png'],
 ['Wheel&Station: map of dydz residual vs phi','map_DTvsphi_dydz.png'],
 ['Wheel&Station: map of x residual vs phi','map_DTvsphi_x.png'],
 ['Wheel&Station: map of y residual vs phi','map_DTvsphi_y.png'],
 ['Station&Sector: map of dxdz residual vs z','map_DTvsz_dxdz.png'],
 ['Station&Sector: map of dydz residual vs z','map_DTvsz_dydz.png'],
 ['Station&Sector: map of x residual vs z','map_DTvsz_x.png'],
 ['Station&Sector: map of y residual vs z','map_DTvsz_y.png'],
 ['Wheel: segdiff in x residuals vs phi','segdifphi_dt13_resid.png'],
 ['Wheel: segdiff in dxdz residuals vs phi','segdifphi_dt13_slope.png'],
 ['Wheel: segdiff in y residuals vs phi','segdifphi_dt2_resid.png'],
 ['Wheel: segdiff in dydz residuals vs phi','segdifphi_dt2_slope.png'],
 ['Chamber: segdiff in x residuals','segdif_dt13_resid.png'],
 ['Chamber: segdiff in dxdz residuals','segdif_dt13_slope.png'],
 ['Chamber: segdiff in y residuals','segdif_dt2_resid.png'],
 ['Chamber: segdiff in dydz residuals','segdif_dt2_slope.png'],
 ['Chamber: residuals distributions','dt_bellcurves.png'],
 ['Chamber: residuals relations to misalignments','dt_polynomials.png']
],
['CSC',' ',
 ['Station&Ring: map of d(rphi)/dz residual vs phi','map_CSCvsphi_dxdz.png'],
 ['Station&Ring: map of rphi residual vs phi','map_CSCvsphi_x.png'],
 ['Station&Chamber: map of d(rphi)/dz residual vs r','map_CSCvsr_dxdz.png'],
 ['Station&Chamber: map of rphi residual vs r','map_CSCvsr_x.png'],
 ['Chamber: segdiff in rphi residuals','segdif_csc_resid.png'],
 ['Chamber: segdiff in d(rphi)/dz residuals','segdif_csc_slope.png'],
 ['Chamber: residuals distributions','csc_bellcurves.png'],
 ['Chamber: residuals relations to misalignments','csc_polynomials.png']
]
]


######################################################
# functions definitions


def isFileUnderDir(dir_name, file_name):
  '''Recursively looks for file named file_name under dir_name directory
  '''
  for f in os.listdir(dir_name):
    dirfile = os.path.join(dir_name, f)
    if os.path.isfile(dirfile) and f==file_name:
      return True
    # recursively access file names in subdirectories
    elif os.path.isdir(dirfile):
      #print "Accessing directory:", dirfile
      if isFileUnderDir(dirfile, file_name): return True
  return False


# to time saving of plots
def saveAs(nm): 
  t1 = time.time()
  ddt[15] += 1
  c1.SaveAs(nm)
  tn = time.time()
  ddt[16] = 1./ddt[15]*((ddt[15]-1)*ddt[16] + tn-t1)


def createDirectoryStructure(iteration_name):
  
  if not os.access(iteration_name,os.F_OK):
    os.mkdir(iteration_name)

  csc_basedir = iteration_name+'/'
  for endcap in CSC_TYPES:
    #print csc_basedir+endcap[0]
    shutil.rmtree(csc_basedir+endcap[0],True)
    os.mkdir(csc_basedir+endcap[0])
    for station in endcap[2]:
      #print csc_basedir+endcap[0]+'/'+station[1]
      os.mkdir(csc_basedir+endcap[0]+'/'+station[1])
      for ring in station[2]:
        #print csc_basedir+endcap[0]+'/'+station[1]+'/'+ring[1]
        os.mkdir(csc_basedir+endcap[0]+'/'+station[1]+'/'+ring[1])
        for chamber in range(1,ring[2]+1):
          schamber = "%02d" % chamber
          #print csc_basedir+endcap[0]+'/'+station[1]+'/'+ring[1]+'/'+schamber
          os.mkdir(csc_basedir+endcap[0]+'/'+station[1]+'/'+ring[1]+'/'+schamber)

  dt_basedir = iteration_name+'/MB/'
  #print dt_basedir
  shutil.rmtree(dt_basedir,True)
  os.mkdir(dt_basedir)
  for wheel in DT_TYPES:
    #print dt_basedir+wheel[0]
    os.mkdir(dt_basedir+wheel[0])
    for station in wheel[2]:
      #print dt_basedir+wheel[0]+'/'+station[1]
      os.mkdir(dt_basedir+wheel[0]+'/'+station[1])
      for sector in range(1,station[2]+1):
        ssector = "%02d" % sector
        #print dt_basedir+wheel[0]+'/'+station[1]+'/'+ssector
        os.mkdir(dt_basedir+wheel[0]+'/'+station[1]+'/'+ssector)

  print os.getcwd()

######################################################

def doMapPlotsDT(dt_basedir, tfiles_plotting):
  """write DT map plots

 "DTvsphi_st%dwh%s" % (station, wheelletter):

  plots "integrated" over ALL SECTORS:
  of x, y, dxdz, dydz vs. phi (y and dydz only for stations 1-3)
  made for all (station,wheel) combinations

  Access interface may be arranged into station(1 .. 4) vs. wheel(-2 .. +2) map.
  It could be incorporated into a general DT chambers map (column1: wheel, column2: station,
  columns3-16 correspond to sector #) by making station numbers in column 2 clickable.
  

 "DTvsz_st%dsec%02d" % (station, sector)

  plots "integrated" over ALL WHEELS:
  of x, y, dxdz, dydz vs. z (y and dydz only for stations 1-3)
  made for all (station,sector) combinations

  Interface: may be arranged into station(1 .. 4) vs. sector(1 .. 14) map with sector range
  (1 .. 12) for stations 1-3.
  It could be incorporated into an EXTENDED general DT chambers map (extended by adding an
  identifier "ALL" in column1 for wheel number)."""

  for wheel in DT_TYPES:
    if wheel[1]=="ALL": continue
    for station in wheel[2]:
      pdir = dt_basedir+'/'+wheel[0]+'/'+station[1]+'/'
      label = "DTvsphi_st%dwh%s" % (int(station[1]), wheelLetter(int(wheel[1])))
      htitle = "wheel %+d, station %s" % (int(wheel[1]), station[1])
      mapplot(tfiles_plotting, label, "x", window=15., title=htitle)
      c1.SaveAs(pdir+'map_DTvsphi_x.png')
      mapplot(tfiles_plotting, label, "dxdz", window=15., title=htitle)
      c1.SaveAs(pdir+'map_DTvsphi_dxdz.png')

      if station[1]=='4': continue
      mapplot(tfiles_plotting, label, "y", window=15., title=htitle)
      c1.SaveAs(pdir+'map_DTvsphi_y.png')
      mapplot(tfiles_plotting, label, "dydz", window=15., title=htitle)
      c1.SaveAs(pdir+'map_DTvsphi_dydz.png')

  qcount=0
  for wheel in DT_TYPES:
    if wheel[1]!="ALL": continue
    for station in wheel[2]:
      for sector in range(1,station[2]+1):
        if qcount>QUICKTESTN: break
        qcount += 1
        ssector = "%02d" % sector
        pdir = dt_basedir+'/'+wheel[0]+'/'+station[1]+'/'+ssector+'/'
        label = "DTvsz_st%ssec%s" % (station[1], ssector)
        htitle = "station %s, sector %d" % (station[1], sector)
        mapplot(tfiles_plotting, label, "x", window=15., title=htitle)
        c1.SaveAs(pdir+'map_DTvsz_x.png')
        mapplot(tfiles_plotting, label, "dxdz", window=15., title=htitle)
        c1.SaveAs(pdir+'map_DTvsz_dxdz.png')
        
        if station[1]=='4': continue
        mapplot(tfiles_plotting, label, "y", window=15., title=htitle)
        c1.SaveAs(pdir+'map_DTvsz_y.png')
        mapplot(tfiles_plotting, label, "dydz", window=15., title=htitle)
        c1.SaveAs(pdir+'map_DTvsz_dydz.png')


def doMapPlotsCSC(csc_basedir, tfiles_plotting):
  """write CSC map plots

 "CSCvsphi_me%s%d%d" % (endcap, station, ring)

  plots "integrated" over ALL SECTORS:
  of rphi, drphi/dz vs. phi
  made for all (endcap,station,ring) combinations

  Interface: may be arranged into two station(1 .. 4) vs. R(1 .. 4) maps for both endcaps
  with R range (1 .. 4) for stations 2-4
 It could be incorporated into a general CSC chambers map (column1: endcap, column2: station,
  column3: ring, columns4-40 correspond to chamber #) by making ring numbers in column 3
  clickable.


 "CSCvsr_me%s%dch%02d" % (endcap, station, chamberNumber)

  plots "integrated" over ALL RINGS:
  of rphi, drphi/dz vs. z
  made for all (endcap,station,chamber) combinations

  Interface: may be arranged into two station(1 .. 4) vs. chamber(1 .. 36) maps for both endcaps
  It could be incorporated into an EXTENDED general CSC chambers map (extended by adding an
  identifier "ALL" in column3 for ring number)."""

  for endcap in CSC_TYPES:
    for station in endcap[2]:
      for ring in station[2]:
        if ring[1]=="ALL": continue
        pdir = csc_basedir+'/'+endcap[0]+'/'+station[1]+'/'+ring[1]+'/'
        label = "CSCvsphi_me%s%s%s" % (endcap[1], station[1], ring[1])
        htitle = "%s%s/%s" % (endcap[0], station[1],ring[1])
        mapplot(tfiles_plotting, label, "x", window=15., title=htitle)
        c1.SaveAs(pdir+'map_CSCvsphi_x.png')
        mapplot(tfiles_plotting, label, "dxdz", window=15., title=htitle)
        c1.SaveAs(pdir+'map_CSCvsphi_dxdz.png')

  qcount = 0
  for endcap in CSC_TYPES:
    for station in endcap[2]:
      for ring in station[2]:
        if ring[1]!="ALL": continue
        for chamber in range(1,ring[2]+1):
          if qcount>QUICKTESTN: break
          qcount += 1
          schamber = "%02d" % chamber
          pdir = csc_basedir+'/'+endcap[0]+'/'+station[1]+'/'+ring[1]+'/'+schamber+'/'
          label = "CSCvsr_me%s%sch%s" % (endcap[1], station[1], schamber)
          htitle = "%s%s/ALL/%d" % (endcap[0], station[1],chamber)
          mapplot(tfiles_plotting, label, "x", window=15., title=htitle)
          c1.SaveAs(pdir+'map_CSCvsr_x.png')
          mapplot(tfiles_plotting, label, "dxdz", window=15., title=htitle)
          c1.SaveAs(pdir+'map_CSCvsr_dxdz.png')


def doSegDiffPlotsDT(dt_basedir, tfiles_plotting, iter_reports):
  """write segment-difference plots for DT

 segdiff "dt13_resid" and "dt13_slope"

  set of plots of
  x vs qpt, x for positive, x for negative ("dt13_resid")
  dxdz vs qpt, dxdz for positive, dxdz for negative ("dt13_slope")
  done for MB1-MB2, MB2-MB3, and MB3-MB4 stations combinations with all possible (wheel, sector)

  Interface: could be accessed through a general DT chambers map, but only for chambers in
  stations 2-4 (e.g., station 2 would provide MB1-MB2 plots).

 segdiff "dt2_resid" and "dt2_slope"

  set of plots of
  y vs q/pt, y for positive, y for negative ("dt2_resid")
  dydz vs q/pt, dydz for positive, dydz for negative ("dt2_slope")
  done for MB1-MB2, MB2-MB3 stations combinations with all possible (wheel, sector)

  Interface: then the interface would still be a general DT map,
  but the info only available from station 2 & 3 chambers."""

  qcount = 0
  for iwheel in DT_TYPES:
    if iwheel[1]=="ALL": continue
    for istation in iwheel[2]:
      if istation[1]=="1": continue
      dstations = (int(istation[1])-1)*10 + int(istation[1])
      #print dstations
      for isector in range(1, istation[2] + 1):
        if isector > 12: continue
        if qcount>QUICKTESTN: break
        qcount += 1
        ssector = "%02d" % isector
        pdir = dt_basedir + '/' + iwheel[0] + '/' + istation[1] + '/' + ssector + '/'
        
        segdiff(tfiles_plotting, "dt13_resid", dstations, wheel=int(iwheel[1]), sector=isector, window=10.)
        c1.SaveAs(pdir + 'segdif_dt13_resid.png')
        segdiff(tfiles_plotting, "dt13_slope", dstations, wheel=int(iwheel[1]), sector=isector, window=10.)
        c1.SaveAs(pdir + 'segdif_dt13_slope.png')
        
        if istation[1] == '4': continue
        segdiff(tfiles_plotting, "dt2_resid", dstations, wheel=int(iwheel[1]), sector=isector, window=10.)
        c1.SaveAs(pdir + 'segdif_dt2_resid.png')
        segdiff(tfiles_plotting, "dt2_slope", dstations, wheel=int(iwheel[1]), sector=isector, window=10.)
        c1.SaveAs(pdir + 'segdif_dt2_slope.png')

  """segdiffvsphi "dt13_resid" and "dt13_slope"

  plot for a specific wheel #:
  x vs phi of pair ("dt13_resid")
  dxdz vs phi of pair ("dt13_slope")
  contains all three combinations of neighboring stations
  made for all possible wheel values

  Interface: could be accessed by clicking on wheel number under the "wheel" column
  in a general DT map

 segdiffvsphi "dt2_resid" and "dt2_slope"

  plot for a specific wheel #:
  y vs phi of pair ("dt2_resid")
  dydz vs phi of pair ("dt2_slope")
  contains both MB1-MB2 and MB2-MB3 combinations
  made for all possible wheel values

  Interface: could be accessed by clicking on wheel number under the "wheel" column
  in a general DT map"""

  for iwheel in DT_TYPES:
    if iwheel[1]=="ALL": continue
    pdir = dt_basedir + '/' + iwheel[0] + '/'
    segdiffvsphi(tfiles_plotting, iter_reports, "dt13_resid", int(iwheel[1]), window=15., excludesectors=(1,7))
    c1.SaveAs(pdir + 'segdifphi_dt13_resid.png')
    segdiffvsphi(tfiles_plotting, iter_reports, "dt13_slope", int(iwheel[1]), window=15., excludesectors=(1,7))
    c1.SaveAs(pdir + 'segdifphi_dt13_slope.png')
    segdiffvsphi(tfiles_plotting, iter_reports, "dt2_resid", int(iwheel[1]), window=15., excludesectors=(1,7))
    c1.SaveAs(pdir + 'segdifphi_dt2_resid.png')
    segdiffvsphi(tfiles_plotting, iter_reports, "dt2_slope", int(iwheel[1]), window=15., excludesectors=(1,7))
    c1.SaveAs(pdir + 'segdifphi_dt2_slope.png')


def doSegDiffPlotsCSC(csc_basedir, tfiles_plotting, iter_reports):
  """write segment-difference plots for CSC
 
 segdiff "csc_resid" and "csc_slope"

  set of plots of
  rphi vs qpt, rphi for positive, rphi for negative ("csc_resid")
  drphidz vs qpt, drphidz for positive, drphidz for negative ("csc_slope")
  done for ME1-ME2, ME2-ME3, and ME3-ME4 stations combinations with
    endcap "m" or "p" 
    ring 1 or 2
    chamber 1-18 (r1) or 1-36 (r2)
  note: there's no ME3-ME4 plots for R2
  
  Interface: could be accessed through a general CSC chambers map, but only for chambers in
  stations 2-4 (e.g., station 2 would provide ME1-ME2 plots)."""

  qcount = 0
  for iendcap in CSC_TYPES:
    for istation in iendcap[2]:
      if istation[1]=="1": continue
      dstations = (int(istation[1])-1)*10 + int(istation[1])
      for iring in istation[2]:
        if iring[1]=="ALL": continue
        if istation[1]=="4" and iring[1]=="2": continue
        for ichamber in range(1,iring[2]+1):
          if qcount>QUICKTESTN: break
          qcount += 1
          schamber = "%02d" % ichamber
          pdir = csc_basedir+'/'+iendcap[0]+'/'+istation[1]+'/'+iring[1]+'/'+schamber+'/'
          segdiff(tfiles_plotting, "csc_resid", dstations, 
                  endcap=iendcap[1], ring=int(iring[1]), chamber=ichamber, window=10.)
          c1.SaveAs(pdir + 'segdif_csc_resid.png')
          segdiff(tfiles_plotting, "csc_slope", dstations, 
                  endcap=iendcap[1], ring=int(iring[1]), chamber=ichamber, window=10.)
          c1.SaveAs(pdir + 'segdif_csc_slope.png')


def doFitFunctionsPlotsDT(dt_basedir, iter_tfile, iter_reports):
  """write fit functions plots for DT

 DT bellcurves and polynomials

  set of plots of bellcurves
    x, dxdz, x vs. dxdz (for all 4 stations)
    y, dydz, x vs. dxdz (only for stations 1-3?)

  set of plots of polynomials -- for stations 1-3 only??
    x vs. xpos,    x vs ypos,    x vs dxdz angle,    x vs dydz angle
    y vs. xpos,    y vs ypos,    y vs dxdz angle,    y vs dydz angle
    dxdz vs. xpos, dxdz vs ypos, dxdz vs dxdz angle, dxdz vs dydz angle
    dydz vs. xpos, dydz vs ypos, dydz vs dxdz angle, dydz vs dydz angle

  set of plots of polynomials -- for station 4 only??
    x vs. xpos,    x vs dxdz angle
    dxdz vs. xpos, dxdz vs dxdz angle

  made for all (wheel,station,sector) combinations

  Interface: could be accesses through a general DT chambers map."""

  qcount = 0
  clearDDT()
  for wheel in DT_TYPES:
    if wheel[1]=="ALL": continue
    for station in wheel[2]:
      print wheel[0]+'/'+station[1]
      for sector in range(1,station[2]+1):
        if qcount>QUICKTESTN: break
        qcount += 1
        ssector = "%02d" % sector
        pdir = dt_basedir+'/'+wheel[0]+'/'+station[1]+'/'+ssector+'/'
        label = "MBwh%sst%ssec%s" % (wheelLetter(int(wheel[1])),station[1],ssector)
        bellcurves(iter_tfile, iter_reports, label, False)
        #c1.SaveAs(pdir+'dt_bellcurves.png')
        saveAs(pdir+'dt_bellcurves.png')
        polynomials(iter_tfile, iter_reports, label, False)
        #c1.SaveAs(pdir+'dt_polynomials.png')
        saveAs(pdir+'dt_polynomials.png')
  #printDeltaTs()


def doFitFunctionsPlotsCSC(csc_basedir, iter_tfile, iter_reports):
  """write fit functions plots for CSC

 CSC bellcurves and polynomials

  set of plots of bellcurves
    rphi, drphidz, rphi vs. drphidz

  set of plots of polynomials
    rphi vs. rphi pos,    rphi vs drphidz angle
    drphidz vs. rphi pos, drphidz vs drphidz angle

  made for all (endcap,station,ring,chamber) combinations

  Interface: could be accesses through a general CSC chambers map."""

  qcount = 0
  clearDDT()
  for endcap in CSC_TYPES:
    for station in endcap[2]:
      for ring in station[2]:
        if ring[1]=="ALL": continue
        print endcap[0]+'/'+station[1]+'/'+ring[1]
        for chamber in range(1,ring[2]+1):
          if qcount>QUICKTESTN: break
          qcount += 1
          schamber = "%02d" % chamber
          pdir = csc_basedir+'/'+endcap[0]+'/'+station[1]+'/'+ring[1]+'/'+schamber+'/'
          label = "ME%s%s%s_%s" % (endcap[1], station[1], ring[1], schamber)
          bellcurves(iter_tfile, iter_reports, label, False)
          #c1.SaveAs(pdir+'csc_bellcurves.png')
          saveAs(pdir+'csc_bellcurves.png')
          polynomials(iter_tfile, iter_reports, label, False)
          #c1.SaveAs(pdir+'csc_polynomials.png')
          saveAs(pdir+'csc_polynomials.png')
  #printDeltaTs()



def doIterationPlots(iteration_directory, tfiles_plotting, iter_tfile, iter_reports):
  dt_basedir = iteration_directory+'/'+'MB'
  csc_basedir = iteration_directory+'/'

  if DO_DT and DO_MAP:
    doMapPlotsDT(dt_basedir, tfiles_plotting)
  if DO_CSC and DO_MAP:
    doMapPlotsCSC(csc_basedir, tfiles_plotting)

  if DO_DT and DO_SEGDIFF:
    doSegDiffPlotsDT(dt_basedir, tfiles_plotting, iter_reports)
  if DO_CSC and DO_SEGDIFF:
    doSegDiffPlotsCSC(csc_basedir, tfiles_plotting, iter_reports)

  if DO_DT and DO_FIT:
    doFitFunctionsPlotsDT(dt_basedir, iter_tfile, iter_reports)
  if DO_CSC and DO_FIT:
    doFitFunctionsPlotsCSC(csc_basedir, iter_tfile, iter_reports)


def createCanvasesList(fname="canvases_list.js"):
  '''Use CANVASES_LIST_TEMPLATE as a template to create a canvases list include for the browser.
     Write out only those canvases which have existing filename.png plots.
  '''
  CANVASES_LIST = []
  for scope in CANVASES_LIST_TEMPLATE:
    scope_entry = []
    if len(scope)>2:
      scope_entry = [scope[0],scope[1]]
      for canvas_entry in scope[2:]:
        if isFileUnderDir("./",canvas_entry[1]):
          scope_entry.append(canvas_entry)
    CANVASES_LIST.append(scope_entry)

  ff = open(fname,mode="w")
  print >>ff, "var CANVASES_LIST = "
  json.dump(CANVASES_LIST,ff)
  ff.close()


############################################################################################################
############################################################################################################
# main script

# open input files:

#basedir='/disks/sdb5/home_reloc/khotilov/db/cms/alignment'
#iteration1 = "iteration_01"
#iteration3 = "iteration_03"
#iteration1 = "NOV4DT_PASS3noweight_TkHIP_01"
#iteration3 = "NOV4DT_PASS3noweight_TkHIP_05"
#os.chdir(options.inputDir)
#iteration1 = options.i1
#iteration3 = options.iN

fname = options.inputDir+'/'+options.i1+'/'+i1prefix
if (DO_MAP or DO_SEGDIFF) and not os.access(fname+"_plotting.root",os.F_OK):
  print "no file "+fname+"_plotting.root"
  sys.exit()
tfiles1_plotting = []
if (DO_MAP or DO_SEGDIFF): tfiles1_plotting.append(ROOT.TFile(fname+"_plotting.root"))
iter1_tfile = ROOT.TFile(fname+".root")
execfile(fname+"_report.py");
iter1_reports = reports

fname = options.inputDir+'/'+options.iN+'/'+iNprefix
if (DO_MAP or DO_SEGDIFF) and not os.access(fname+"_plotting.root",os.F_OK):
  print "no file "+fname+"_plotting.root"
  sys.exit()
tfilesN_plotting = []
if (DO_MAP or DO_SEGDIFF): tfilesN_plotting.append(ROOT.TFile(fname+"_plotting.root"))
iterN_tfile = ROOT.TFile(fname+".root")
execfile(fname+"_report.py");
iterN_reports = reports

######################################################
# setup output:

# cd to outputDIr
os.chdir(options.outputDir)

comdir = "common/"
iteration1 = "iter1"
iterationN = "iterN"

# create directory structure
if options.createDirSructure:
  print "WARNING: all existing results in "+options.outputDir+" will be deleted!"
  createDirectoryStructure(iteration1)
  createDirectoryStructure(iterationN)
  if not os.access(comdir,os.F_OK):
    os.mkdir(comdir)

######################################################
# do drawing

c1 = ROOT.TCanvas("c1","c1",800,600)

set_palette("blues")

doIterationPlots(iteration1, tfiles1_plotting, iter1_tfile, iter1_reports)
doIterationPlots(iterationN, tfilesN_plotting, iterN_tfile, iterN_reports)

# write distributions of medians plots

if DO_MEDIAN:
  plotmedians(iter1_reports, iterN_reports)
  c1.SaveAs(comdir+'medians.png')

# perform diagnostic
if DO_DIAGNOSTIC:
  doTests(reports,"mu_list.js","dqm_report.js",options.runLabel)
  createCanvasesList("canvases_list.js")

