#!/usr/bin/env python

import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet import DictTypes

import sys, os, os.path

# enable tracing cms.Sequences, cms.Paths and cms.EndPaths for all imported modules (thus, process.load(...), too)
import tracingImport

sys.path.append(os.environ["PWD"])

filename = sys.argv[1].rstrip('.py')

theConfig = __import__(filename)

result = dict()
result['procname']       = ''
result['main_input']     = DictTypes.SortedKeysDict()
result['looper']         = DictTypes.SortedKeysDict()
result['psets']          = DictTypes.SortedKeysDict()
result['modules']        = DictTypes.SortedKeysDict()
result['es_modules']     = DictTypes.SortedKeysDict()
result['es_sources']     = DictTypes.SortedKeysDict()
result['es_prefers']     = DictTypes.SortedKeysDict()
result['output_modules'] = list()
result['sequences']      = DictTypes.SortedKeysDict()
result['paths']          = DictTypes.SortedKeysDict()
result['endpaths']       = DictTypes.SortedKeysDict()
result['services']       = DictTypes.SortedKeysDict()
result['schedule']       = ''

def dumpObject(obj,key):
    if key in ('es_modules','es_sources','es_prefers'):
        classname = obj['@classname']
        label = obj['@label']
        del obj['@label']
        del obj['@classname']
        returnString = "{'@classname': %s, '@label': %s, %s" %(classname, label, str(obj).lstrip('{'))
        return returnString
    elif key in ('modules','services'):
        classname = obj['@classname']
        del obj['@label']
        del obj['@classname']
        returnString = "{'@classname': %s, %s" %(classname, str(obj).lstrip('{'))
        return returnString
    elif key in ('psets',):
        returnString = "('PSet', 'untracked', %s)"%str(obj)
        return returnString
    else:
        return str(obj)


def trackedness(item):
  if item.isTracked:
    return 'tracked'
  else:
    return 'untracked'

# the problem are non empty VPsets
def fixup(item):
  if type(item) == bool:
    if item: return 'true'
    else: return 'false'
  elif type(item) == list:
      return [str(i) for i in item]
  elif type(item) == str:
      return '"%s"' %item
  else:
      return str(item)

def prepareParameter(parameter):
    if isinstance(parameter, cms.VPSet):
        configValue = []
        for item in parameter:
            configValue.append((prepareParameter(item)[2]))
        return (type(parameter).__name__, trackedness(parameter), configValue )
    if isinstance(parameter, cms.PSet):
        configValue = {}
        for name, item in parameter.parameters_().iteritems():
          configValue[name] = prepareParameter(item)
        return (type(parameter).__name__, trackedness(parameter), configValue )
    else:
        return (type(parameter).__name__, trackedness(parameter), fixup(parameter.value()) )

def parsePSet(module):
  config = DictTypes.SortedKeysDict()
  for parameterName,parameter in module.parameters_().iteritems():
    config[parameterName] = prepareParameter(parameter)
  return config

def parseSource(module):
  config = DictTypes.SortedKeysDict()
  config['@classname'] = ('string','tracked',module.type_())
  for parameterName,parameter in module.parameters_().iteritems():
    config[parameterName] = prepareParameter(parameter)
  return config

def parseModule(name, module):
  config = DictTypes.SortedKeysDict()
  config['@classname'] = ('string','tracked',module.type_())
  config['@label'] = ('string','tracked',name)
  for parameterName,parameter in module.parameters_().iteritems():
    config[parameterName] = prepareParameter(parameter)
  return config

def parseModules(process):
  result['procname'] = process.process
  
  result['main_input']['source'] = parseSource(process.source)

  for name,item in process.producers.iteritems():
    result['modules'][name] = parseModule(name, item)

  for name,item in process.filters.iteritems():
    result['modules'][name] = parseModule(name, item)

  for name,item in process.analyzers.iteritems():
    result['modules'][name] = parseModule(name, item)

  for name,item in process.outputModules.iteritems():
    result['modules'][name] = parseModule(name, item)
    result['output_modules'].append(name)

  for name,item in process.es_sources.iteritems():
    result['es_sources'][name + '@'] = parseModule(name, item)

  for name,item in process.es_producers.iteritems():
    result['es_modules'][name + '@'] = parseModule(name, item)

  for name,item in process.es_prefers.iteritems():
    result['es_prefers'][name + '@'] = parseModule(name, item)

  for name,item in process.psets.iteritems():
    result['psets'][name] = parsePSet(item)

  for name,item in process.sequences.iteritems():
    result['sequences'][name] = "'" + item.dumpConfig("")[1:-2] + "'"

  for name,item in process.paths.iteritems():
    result['paths'][name] = "'" + item.dumpConfig("")[1:-2] + "'"

  for name,item in process.endpaths.iteritems():
    result['endpaths'][name] = "'" + item.dumpConfig("")[1:-2] + "'"

  for name,item in process.services.iteritems():
    result['services'][name] = parseModule(name, item)

  # TODO still missing:
  #   process.vpsets
  #   process.looper
  #   process.schedule
 
  # use the ordering seen at module import time for sequence, paths and endpaths,
  # keeping only those effectively present in the process
  # (some might have been commented, removed, not added, whatever...)
  result['paths'].list     = [ path for path in tracingImport.original_paths     if path in result['paths'].list ]
  result['endpaths'].list  = [ path for path in tracingImport.original_endpaths  if path in result['endpaths'].list ]
  result['sequences'].list = [ path for path in tracingImport.original_sequences if path in result['sequences'].list ]

  # sort alphabetically everything else
  result['main_input'].list.sort()
  result['modules'].list.sort()
  result['output_modules'].sort()
  result['es_sources'].list.sort()
  result['es_modules'].list.sort()
  result['es_prefers'].list.sort()
  result['psets'].list.sort()
  result['services'].list.sort()


# parse the configuration
parseModules(theConfig.process)

# now dump it to the screen as wanted by the HLT parser
hltAcceptedOrder = ['main_input','looper', 'psets', 'modules', 'es_modules', 'es_sources', 'es_prefers', 'output_modules', 'sequences', 'paths', 'endpaths', 'services', 'schedule']

print '{'
print "'procname': '%s'" %result['procname']

for key in hltAcceptedOrder:
    if key in ('output_modules', 'schedule'):
        print ", '%s': %s" %(key, result[key])
    elif key in ('main_input',):
        print ", '%s':  {" %key
        comma = ''
        for object in result[key].itervalues():
            print comma+str(dumpObject(object,key))[1:-1]
            comma = ', '
        print '} # end of %s' %key
    else:
        print ", '%s':  {" %key
        comma = ''
        for name,object in result[key].iteritems():
            print comma+"'%s': %s" %(name, dumpObject(object,key))
            comma = ', '
        print '} # end of %s' %key

print '}'

