#!/usr/bin/env python

import TauAnalysis.Configuration.tools.eos as eos

import os
import shlex
import string
import subprocess

samples = {
##     'simDYtoMuMu_noEvtSel_embedEqRH_cleanEqDEDX_replaceGenMuons_by_mutau_embedAngleEq90_noPolarization_wTauSpinner' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHGENEmbed_MuTau_Angle90_VisPtMu7Had15_NoPolarization_embedded_trans1_v9-5ef1c0fd428eb740081f19333520fdc8/USER',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##         'events_processed'                   : -1,
##         'events_per_job'                     : 20000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'EmbeddedMC',
##         'srcReplacedMuons'                   : 'genMuonsFromZs',
##         'muonRadCorrectionsApplied'          : True,
##         'channel'                            : 'mutau',
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : True,
##         'applyTauSpinnerWeight'              : True,
##         'applyZmumuEvtSelEffCorrWeight'      : False,
##         'produceEmbeddingKineReweightNtuple' : True
##     },
##     'simDYtoTauTau_mutau' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet',
##         'events_processed'                   : 30459503,
##         'events_per_job'                     : 100000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'MC',
##         'srcReplacedMuons'                   : '',
##         'channel'                            : 'mutau',        
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : '',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : False,
##         'applyTauSpinnerWeight'              : False,
##         'applyZmumuEvtSelEffCorrWeight'      : False,
##         'produceEmbeddingKineReweightNtuple' : True
##     },       
##     'simDYtoMuMu_noEvtSel_embedEqRH_cleanEqDEDX_replaceRecMuons_by_mutau_embedAngleEq90_muonCaloSF1_0' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHRECEmbed_MuTau_Angle90_VisPtMu13Had17_embedded_trans1_v9-5ef1c0fd428eb740081f19333520fdc8/USER',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##         'events_processed'                   : -1,
##         'events_per_job'                     : 20000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'EmbeddedMC',
##         'srcReplacedMuons'                   : 'goldenZmumuCandidatesGe2IsoMuons',
##         'muonRadCorrectionsApplied'          : False,
##         'channel'                            : 'mutau',
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : True,
##         'applyTauSpinnerWeight'              : False,
##         'applyZmumuEvtSelEffCorrWeight'      : True,
##         'produceEmbeddingKineReweightNtuple' : False
##     },
##     'simDYtoMuMu_noEvtSel_embedEqPF_cleanEqPF_replaceRecMuons_by_mutau_HCP' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_PFEmbed_embedded_trans1_tau116_ptmu1_13had1_17_v1-5ef1c0fd428eb740081f19333520fdc8/USER',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##         'events_processed'                   : -1,
##         'events_per_job'                     : 20000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'MC',
##         'srcReplacedMuons'                   : '',
##         'muonRadCorrectionsApplied'          : False,
##         'channel'                            : 'mutau',
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : False,
##         'applyTauSpinnerWeight'              : False,
##         'applyZmumuEvtSelEffCorrWeight'      : False,
##         'produceEmbeddingKineReweightNtuple' : False
##     },
     'simDYtoMuMu_noEvtSel_embedEqRH_cleanEqDEDX_replaceGenMuons_by_etau_embedAngleEq90_noPolarization_wTauSpinner' : {
         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHGENEmbed_ETau_Angle90_VisPtElec9Had15_WithCaloNoise_embedded_trans1_v12-5ef1c0fd428eb740081f19333520fdc8/USER',
         ##'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHGENEmbed_ETau_Angle90_VisPtElec9Had15_embedded_trans1_v12-5ef1c0fd428eb740081f19333520fdc8/USER',
         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
         'events_processed'                   : -1,
         'events_per_job'                     : 20000,
         'total_number_of_events'             : -1,
         'type'                               : 'EmbeddedMC',
         'srcReplacedMuons'                   : 'genMuonsFromZs',
         'muonRadCorrectionsApplied'          : True,
         'channel'                            : 'etau',
         'srcWeights'                         : [],
         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
         'addTauPolValidationPlots'           : False,
         'applyEmbeddingKineReweight'         : True,
         'applyTauSpinnerWeight'              : True,
         'applyZmumuEvtSelEffCorrWeight'      : False,
         'produceEmbeddingKineReweightNtuple' : False
     },
##    'simDYtoMuMu_noEvtSel_embedEqRH_cleanEqDEDX_replaceGenMuons_by_etau_embedAngleEq90_noPolarization_wTauSpinner_wConversionFix' : {
##        'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHGENEmbed_ETau_Angle90_VisPtElec9Had15_NoPolarization_ConversionFix_embedded_trans1_v9-5ef1c0fd428eb740081f19333520fdc8/USER',
##        'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##        'events_processed'                   : -1,
##        'events_per_job'                     : 20000,
##        'total_number_of_events'             : -1,
##        'type'                               : 'EmbeddedMC',
##        'srcReplacedMuons'                   : 'genMuonsFromZs',
##        'muonRadCorrectionsApplied'          : True,
##        'channel'                            : 'etau',
##        'srcWeights'                         : [],
##        'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##        'addTauPolValidationPlots'           : False,
##        'applyEmbeddingKineReweight'         : True,
##        'applyTauSpinnerWeight'              : True,
##        'applyZmumuEvtSelEffCorrWeight'      : False,
##        'produceEmbeddingKineReweightNtuple' : True
##    },    
    'simDYtoTauTau_etau' : {
        'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM',
        'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet',
        'events_processed'                   : 30459503,
        'events_per_job'                     : 100000,
        'total_number_of_events'             : -1,
        'type'                               : 'MC',
        'srcReplacedMuons'                   : '',
        'channel'                            : 'etau',        
        'srcWeights'                         : [],
        'srcGenFilterInfo'                   : '',
        'addTauPolValidationPlots'           : False,
        'applyEmbeddingKineReweight'         : False,
        'applyTauSpinnerWeight'              : False,
        'applyZmumuEvtSelEffCorrWeight'      : False,
        'produceEmbeddingKineReweightNtuple' : False
    },
##     'simDYtoMuMu_noEvtSel_embedEqRH_cleanEqDEDX_replaceRecMuons_by_etau_embedAngleEq90_muonCaloSF1_0' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_RHRECEmbed_ETau_Angle90_VisPtEl9Had15_embedded_trans1_v9-5ef1c0fd428eb740081f19333520fdc8/USER',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##         'events_processed'                   : -1,
##         'events_per_job'                     : 20000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'EmbeddedMC',
##         'srcReplacedMuons'                   : 'goldenZmumuCandidatesGe2IsoMuons',
##         'muonRadCorrectionsApplied'          : False,
##         'channel'                            : 'etau',
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : True,
##         'applyTauSpinnerWeight'              : False,
##         'applyZmumuEvtSelEffCorrWeight'      : True,
##         'produceEmbeddingKineReweightNtuple' : False,
##         'produceEmbeddingKineReweightNtuple' : False
##     },
##     'simDYtoMuMu_noEvtSel_embedEqPF_cleanEqPF_replaceRecMuons_by_etau_HCP' : {
##         'datasetpath'                        : '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/aburgmei-Summer12_DYJetsToLL_DR53X_PU_S10_START53_V7A_v2_PFEmbed_embedded_trans1_tau115_ptelec1_17had1_17_v1-5ef1c0fd428eb740081f19333520fdc8/USER',
##         'dbs_url'                            : 'http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet',
##         'events_processed'                   : -1,
##         'events_per_job'                     : 20000,
##         'total_number_of_events'             : -1,
##         'type'                               : 'MC',
##         'srcReplacedMuons'                   : '',
##         'muonRadCorrectionsApplied'          : False,
##         'channel'                            : 'etau',
##         'srcWeights'                         : [],
##         'srcGenFilterInfo'                   : 'generator:minVisPtFilter',
##         'addTauPolValidationPlots'           : False,
##         'applyEmbeddingKineReweight'         : False,
##         'applyTauSpinnerWeight'              : False,
##         'applyZmumuEvtSelEffCorrWeight'      : False,
##         'produceEmbeddingKineReweightNtuple' : False
##     }
}

version = "v2_1_0_kineReweighted"

crab_template_mc = string.Template('''
[CRAB]
jobtype = cmssw
scheduler = gLite
use_server = 0

[CMSSW]
datasetpath = $datasetpath
dbs_url = $dbs_url
pset = $pset
output_file = validateMCEmbedding_plots.root
total_number_of_events = $total_number_of_events
events_per_job = $events_per_job

[USER]
ui_working_dir = $ui_working_dir
return_data = 0
copy_data = 1
publish_data = 0
storage_element = T2_CH_CERN
user_remote_dir = $user_remote_dir
debug_wrapper = 1

##[GRID]
##SE_white_list = T2_DE_DESY
''')

crab_template_data = string.Template('''
[CRAB]
jobtype = cmssw
scheduler = glite
use_server = 0
 
[CMSSW]
datasetpath = $datasetpath
dbs_url = $dbs_url
pset = $pset
output_file = $output_file
lumi_mask = /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt
total_number_of_lumis = -1
lumis_per_job = $lumis_per_job
#runselection = 190450-190790

[USER]
ui_working_dir = $ui_working_dir
return_data = 0
copy_data = 1
publish_data = 0
storage_element = T2_CH_CERN
user_remote_dir = $user_remote_dir
debug_wrapper = 1

##[GRID]
##SE_white_list = T2_DE_DESY
''')

cfg_template = "validateMCEmbedding_cfg.py"

currentDirectory    = os.getcwd()
submissionDirectory = os.path.join(currentDirectory, "crab")

def getStringRep_bool(flag):
    retVal = None
    if flag:
        retVal = "True"
    else:
        retVal = "False"
    return retVal

def runCommand(commandLine):
    print(commandLine)
    subprocess.call(commandLine, shell = True)

def createFilePath(filePath):
    try:
        eos.lsl(filePath)
    except IOError:
        print "filePath = %s does not yet exist, creating it." % filePath
        eos.mkdir(filePath)

crabCommands_create_and_submit = []
crabCommands_publish           = []

for sampleName, sampleOption in samples.items():
    # create config file for cmsRun
    cfgFileName = "validateMCEmbedding_%s_%s_cfg.py" % (sampleName, version)
    cfgFileName_full = os.path.join(submissionDirectory, cfgFileName)
    runCommand('rm -f %s' % cfgFileName_full)
    sedCommand  = "sed 's/#__//g"
    sedCommand += ";s/$type/%s/g" % sampleOption['type']
    sedCommand += ";s/$srcReplacedMuons/%s/g" % sampleOption['srcReplacedMuons']
    muonRadCorrectionsApplied = False
    if 'muonRadCorrectionsApplied' in sampleOption.keys():
        muonRadCorrectionsApplied = sampleOption['muonRadCorrectionsApplied']
    sedCommand += ";s/$muonRadCorrectionsApplied/%s/g" % getStringRep_bool(muonRadCorrectionsApplied)
    sedCommand += ";s/$channel/%s/g" % sampleOption['channel']
    srcWeights_string = "[ "
    for srcWeight in sampleOption['srcWeights']:
        if len(srcWeights_string) > 0:
            srcWeights_string += ", "
        srcWeights_string += "'%s'" % srcWeight
    srcWeights_string += " ]"
    sedCommand += ";s/$srcWeights/%s/g" % srcWeights_string
    sedCommand += ";s/$srcGenFilterInfo/%s/g" % sampleOption['srcGenFilterInfo']
    sedCommand += ";s/$addTauPolValidationPlots/%s/g" % getStringRep_bool(sampleOption['addTauPolValidationPlots'])
    sedCommand += ";s/$applyEmbeddingKineReweight/%s/g" % getStringRep_bool(sampleOption['applyEmbeddingKineReweight'])
    sedCommand += ";s/$applyTauSpinnerWeight/%s/g" % getStringRep_bool(sampleOption['applyTauSpinnerWeight'])
    sedCommand += ";s/$applyZmumuEvtSelEffCorrWeight/%s/g" % getStringRep_bool(sampleOption['applyZmumuEvtSelEffCorrWeight'])
    sedCommand += ";s/$produceEmbeddingKineReweightNtuple/%s/g" % getStringRep_bool(sampleOption['produceEmbeddingKineReweightNtuple'])
    sedCommand += ";s/$applyPileUpWeight/%s/g" % getStringRep_bool(sampleOption['type'] == "MC" or sampleOption['type'] == "EmbeddedMC")
    sedCommand += "'"
    sedCommand += " %s > %s" % (cfg_template, cfgFileName_full)
    runCommand(sedCommand)

    output_files = None
    if sampleOption['produceEmbeddingKineReweightNtuple']:
        output_files = [ "validateMCEmbedding_plots.root", "embeddingKineReweightNtuple.root" ]
    else:
        output_files = [ "validateMCEmbedding_plots.root" ]
        
    # create crab config file
    crabOptions = None
    crab_template = None
    if sampleOption['type'] == "MC" or sampleOption['type'] == "EmbeddedMC":
        crabOptions = {
            'datasetpath'            : sampleOption['datasetpath'],
            'dbs_url'                : sampleOption['dbs_url'],
            'total_number_of_events' : sampleOption['total_number_of_events'],
            'events_per_job'         : sampleOption['events_per_job'],
            'pset'                   : cfgFileName_full,
            'output_file'            : ",".join(output_files),
            'ui_working_dir'         : os.path.join(submissionDirectory, "crabdir_%s_%s" % (sampleName, version)),
            'user_remote_dir'        : "CMSSW_5_3_x/plots/EmbeddingValidation/%s/%s" % (version, sampleName)
        }
        crab_template = crab_template_mc
    elif sampleOption['type'] == "Data" or sampleOption['type'] == "EmbeddedData":
        crabOptions = {
            'datasetpath'            : sampleOption['datasetpath'],
            'dbs_url'                : sampleOption['dbs_url'],
            'lumis_per_job'          : sampleOption['lumis_per_job'],
            'pset'                   : cfgFileName_full,
            'output_file'            : ",".join(output_files),
            'ui_working_dir'         : os.path.join(submissionDirectory, "crabdir_%s_%s" % (sampleName, version)),
            'user_remote_dir'        : "CMSSW_5_3_x/plots/EmbeddingValidation/%s/%s" % (version, sampleName)
        }
        crab_template = crab_template_data
    else:
        raise ValueError("Invalid sample type = %s !!" % sampleOption['type'])
    crabFileName = "crab_validateMCEmbedding_%s_%s.cfg" % (sampleName, version)
    crabFileName_full = os.path.join(submissionDirectory, crabFileName)
    crabFile = open(crabFileName_full, 'w')
    crabConfig = crab_template.substitute(crabOptions)
    crabFile.write(crabConfig)
    crabFile.close()

    # create output directory
    # (in principle crab will do this, but sometimes fails with 'Permission denied' error, causing all jobs to fail with error code 60307)
    createFilePath("/store/user/veelken/CMSSW_5_3_x/plots/EmbeddingValidation/%s" % version)
    createFilePath("/store/user/veelken/CMSSW_5_3_x/plots/EmbeddingValidation/%s/%s" % (version, sampleName))

    # keep track of commands necessary to create, submit and publish crab jobs
    crabCommands_create_and_submit.append('crab -create -cfg %s' % crabFileName_full)
    if 'events_per_job' in sampleOption.keys(): # MC
        events_total = None
        if sampleOption['total_number_of_events'] == -1:
            events_total = sampleOption['events_processed']
        else:
            events_total = min(sampleOption['total_number_of_events'], sampleOption['events_processed'])
        if (events_total / sampleOption['events_per_job']) < 500:
            crabCommands_create_and_submit.append('crab -submit -c %s' % crabOptions['ui_working_dir'])
        else:
            numJobs = (events_total / sampleOption['events_per_job'])
            if (events_total % sampleOption['events_per_job']) != 0:
                numJobs = numJobs + 1
            numJobs_per_submitCall = 500
            numSubmitCalls = (numJobs / numJobs_per_submitCall)
            if (numJobs % numJobs_per_submitCall) != 0:
                numSubmitCalls = numSubmitCalls + 1
            for submitIdx in range(numSubmitCalls):
                jobId_first = submitIdx*500 + 1
                jobId_last  = (submitIdx + 1)*500
                if jobId_last > numJobs:
                    jobId_last = numJobs
                crabCommands_create_and_submit.append('echo "pausing for 10 seconds before submitting next batch of jobs..."')
                crabCommands_create_and_submit.append('sleep 10')
                crabCommands_create_and_submit.append('crab -submit %i-%i -c %s' % (jobId_first, jobId_last, crabOptions['ui_working_dir']))
    else: # Data
        crabCommands_create_and_submit.append('crab -submit -c %s' % crabOptions['ui_working_dir'])
    
shellFileName_create_and_submit = "validateMCEmbedding_crab_create_and_submit.sh"
shellFile_create_and_submit = open(shellFileName_create_and_submit, "w")
for crabCommand in crabCommands_create_and_submit:
    shellFile_create_and_submit.write("%s\n" % crabCommand)
shellFile_create_and_submit.close()

print("Finished building config files. Now execute 'source %s' to create & submit crab jobs." % shellFileName_create_and_submit)

