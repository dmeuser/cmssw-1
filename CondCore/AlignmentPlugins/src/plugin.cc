/*
 *  plugin.cc
 *  CMSSW
 *
 *  Created by Frederic Ronga on June 7, 2006
 *
 */

#include "CondCore/PluginSystem/interface/registration_macros.h"
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/Alignment/interface/AlignmentErrors.h"
#include "CondFormats/Alignment/interface/SurveyErrors.h"
#include "CondFormats/DataRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/DataRecord/interface/TrackerAlignmentErrorRcd.h"
#include "CondFormats/DataRecord/interface/DTAlignmentRcd.h"
#include "CondFormats/DataRecord/interface/DTAlignmentErrorRcd.h"
#include "CondFormats/DataRecord/interface/CSCAlignmentRcd.h"
#include "CondFormats/DataRecord/interface/CSCAlignmentErrorRcd.h"
#include "CondFormats/DataRecord/interface/TrackerSurveyRcd.h"
#include "CondFormats/DataRecord/interface/TrackerSurveyErrorRcd.h"

REGISTER_PLUGIN(TrackerAlignmentRcd,Alignments);
REGISTER_PLUGIN(TrackerAlignmentErrorRcd,AlignmentErrors);
REGISTER_PLUGIN(DTAlignmentRcd,Alignments);
REGISTER_PLUGIN(DTAlignmentErrorRcd,AlignmentErrors);
REGISTER_PLUGIN(CSCAlignmentRcd,Alignments);
REGISTER_PLUGIN(CSCAlignmentErrorRcd,AlignmentErrors);
REGISTER_PLUGIN(TrackerSurveyRcd,Alignments);
REGISTER_PLUGIN(TrackerSurveyErrorRcd,SurveyErrors);
