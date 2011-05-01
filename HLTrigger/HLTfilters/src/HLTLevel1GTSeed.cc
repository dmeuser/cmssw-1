/**
 * \class HLTLevel1GTSeed
 *
 *
 * Description: filter L1 bits and extract seed objects from L1 GT for HLT algorithms.
 *
 * Implementation:
 *    This class is an HLTFilter (-> EDFilter). It implements:
 *      - filtering on Level-1 bits, given via a logical expression of algorithm names
 *      - extraction of the seed objects from L1 GT object map record
 *
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 *
 * $Date$
 * $Revision$
 *
 */

// this class header
#include "HLTrigger/HLTfilters/interface/HLTLevel1GTSeed.h"

// system include files
#include <string>
#include <list>
#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>

// user include files
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtLogicParser.h"

#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticleFwd.h"

#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1EtMissParticle.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskAlgoTrigRcd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMaskTechTrigRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtCondition.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

// constructors
HLTLevel1GTSeed::HLTLevel1GTSeed(const edm::ParameterSet& parSet) :
            //    seeding done via L1 trigger object maps, with objects that fired
            m_l1UseL1TriggerObjectMaps(parSet.getParameter<bool> (
                    "L1UseL1TriggerObjectMaps")),

            // option used forL1UseL1TriggerObjectMaps = False only
            m_l1NrBxInEvent(parSet.getParameter<int> (
                    "L1NrBxInEvent")),

            // seeding done via technical trigger bits, if value is "true"
            m_l1TechTriggerSeeding(parSet.getParameter<bool> (
                    "L1TechTriggerSeeding")),

            // seeding uses algorithm aliases instead of algorithm names, if value is "true";
            m_l1UseAliasesForSeeding(parSet.getParameter<bool> (
                    "L1UseAliasesForSeeding")),

            // logical expression for the required L1 algorithms
            m_l1SeedsLogicalExpression(parSet.getParameter<std::string> (
                    "L1SeedsLogicalExpression")),

            // InputTag for the L1 Global Trigger DAQ readout record
            m_l1GtReadoutRecordTag(parSet.getParameter<edm::InputTag> (
                    "L1GtReadoutRecordTag")),

            // InputTag for L1 Global Trigger object maps
            m_l1GtObjectMapTag(parSet.getParameter<edm::InputTag> (
                    "L1GtObjectMapTag")),

            // InputTag for L1 particle collections (except muon)
            m_l1CollectionsTag(parSet.getParameter<edm::InputTag> (
                    "L1CollectionsTag")),

            // InputTag for L1 muon collection
            m_l1MuonCollectionTag(parSet.getParameter<edm::InputTag> (
                    "L1MuonCollectionTag")),

            /// cached InputTags
            m_l1MuonTag(edm::InputTag(m_l1MuonCollectionTag.label())),
            m_l1ExtraTag(edm::InputTag(m_l1CollectionsTag.label())),
            m_l1IsoEGTag(edm::InputTag(m_l1CollectionsTag.label(), "Isolated")),
            m_l1NoIsoEGTag(edm::InputTag(m_l1CollectionsTag.label(),
                    "NonIsolated")), m_l1CenJetTag(edm::InputTag(
                    m_l1CollectionsTag.label(), "Central")), m_l1ForJetTag(
                    edm::InputTag(m_l1CollectionsTag.label(), "Forward")),
            m_l1TauJetTag(edm::InputTag(m_l1CollectionsTag.label(), "Tau")),
            m_l1EtMissMET(edm::InputTag(m_l1CollectionsTag.label(), "MET")),
            m_l1EtMissMHT(edm::InputTag(m_l1CollectionsTag.label(), "MHT")),

            // save tags to TriggerFilterObjectWithRefs
            saveTags_(parSet.getParameter<bool>("saveTag")),
            m_isDebugEnabled(edm::isDebugEnabled()) {
    if (m_l1SeedsLogicalExpression != "L1GlobalDecision") {

        // check also the logical expression - add/remove spaces if needed
        m_l1AlgoLogicParser = L1GtLogicParser(m_l1SeedsLogicalExpression);

        // list of required algorithms for seeding
        // dummy values for tokenNumber and tokenResult
        m_l1AlgoSeeds.reserve((m_l1AlgoLogicParser.operandTokenVector()).size());
        m_l1AlgoSeeds = m_l1AlgoLogicParser.expressionSeedsOperandList();
        size_t l1AlgoSeedsSize = m_l1AlgoSeeds.size();

        //
        m_l1AlgoSeedsRpn.reserve(l1AlgoSeedsSize);
        m_l1AlgoSeedsObjType.reserve(l1AlgoSeedsSize);
    }

    // for seeding via technical triggers, convert the "name" to tokenNumber
    // (seeding via bit numbers)
    if (m_l1TechTriggerSeeding) {
        convertStringToBitNumber();
    }

    LogDebug("HLTLevel1GTSeed") << "\n"
            << "L1 Seeding using L1 trigger object maps:       "
            << m_l1UseL1TriggerObjectMaps << "\n"
            << "  if false: seeding with L1Extra\n"
            << "Number of BxInEvent when seeding with L1Extra: "
            << m_l1NrBxInEvent << "\n"
            << "  aka w/o object maps\n " << "\n"
            << "L1 Seeding via Technical Triggers:             "
            << m_l1TechTriggerSeeding << "\n"
            << "L1 Seeding uses algorithm aliases:             "
            << m_l1UseAliasesForSeeding << "\n"
            << "L1 Seeds Logical Expression:                   " << "\n      "
            << m_l1SeedsLogicalExpression << "\n"
            << "Input tag for L1 GT DAQ record:                "
            << m_l1GtReadoutRecordTag << " \n"
            << "Input tag for L1 GT object map record:         "
            << m_l1GtObjectMapTag << " \n"
            << "Input tag for L1 extra collections:            "
            << m_l1CollectionsTag << " \n"
            << "Input tag for L1 muon  collections:            "
            << m_l1MuonCollectionTag << " \n" << std::endl;

    // register the products
    produces<trigger::TriggerFilterObjectWithRefs>();

    // initialize cached IDs
    m_l1GtMenuCacheID = 0ULL;

    m_l1GtTmAlgoCacheID = 0ULL;
    m_l1GtTmTechCacheID = 0ULL;

}

// destructor
HLTLevel1GTSeed::~HLTLevel1GTSeed() {
    // empty now
}

// member functions

bool HLTLevel1GTSeed::filter(edm::Event& iEvent, const edm::EventSetup& evSetup) {

    // all HLT filters must create and fill a HLT filter object,
    // recording any reconstructed physics objects satisfying
    // this HLT filter, and place it in the event.

    // the filter object
    std::auto_ptr<trigger::TriggerFilterObjectWithRefs> filterObject(
            new trigger::TriggerFilterObjectWithRefs(path(), module()));
    if (saveTags_) {
        filterObject->addCollectionTag(m_l1MuonTag);
        filterObject->addCollectionTag(m_l1ExtraTag);
        filterObject->addCollectionTag(m_l1IsoEGTag);
        filterObject->addCollectionTag(m_l1NoIsoEGTag);
        filterObject->addCollectionTag(m_l1CenJetTag);
        filterObject->addCollectionTag(m_l1ForJetTag);
        filterObject->addCollectionTag(m_l1TauJetTag);
        filterObject->addCollectionTag(m_l1EtMissMET);
        filterObject->addCollectionTag(m_l1EtMissMHT);
    }

    // get L1GlobalTriggerReadoutRecord and GT decision
    edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecord;
    iEvent.getByLabel(m_l1GtReadoutRecordTag, gtReadoutRecord);
    const L1GlobalTriggerReadoutRecord* gtReadoutRecordPtr =
            gtReadoutRecord.product();

    if (!gtReadoutRecord.isValid()) {
        edm::LogWarning("HLTLevel1GTSeed")
                << "\nWarning: L1GlobalTriggerReadoutRecord with input tag "
                << m_l1GtReadoutRecordTag
                << "\nrequested in configuration, but not found in the event."
                << std::endl;

        iEvent.put(filterObject);
        return false;
    }

    //
    boost::uint16_t gtFinalOR = gtReadoutRecord->finalOR();
    int physicsDaqPartition = 0;
    bool gtDecision =
            static_cast<bool> (gtFinalOR & (1 << physicsDaqPartition));

    // GT global decision "false" possible only when running on MC or on random triggers
    if (!gtDecision) {

        iEvent.put(filterObject);
        return false;

    } else {

        // by convention, "L1GlobalDecision" logical expression means global decision
        if (m_l1SeedsLogicalExpression == "L1GlobalDecision") {

            // return the full L1GlobalTriggerObjectMapRecord in filter format FIXME
            iEvent.put(filterObject);

            return true;

        }

    }

    // seeding done via technical trigger bits
    if (m_l1TechTriggerSeeding) {

        // get / update the trigger mask from the EventSetup
        // local cache & check on cacheIdentifier
        unsigned long long l1GtTmTechCacheID = evSetup.get<
                L1GtTriggerMaskTechTrigRcd>().cacheIdentifier();

        if (m_l1GtTmTechCacheID != l1GtTmTechCacheID) {

            edm::ESHandle<L1GtTriggerMask> l1GtTmTech;
            evSetup.get<L1GtTriggerMaskTechTrigRcd>().get(l1GtTmTech);
            m_l1GtTmTech = l1GtTmTech.product();

            m_triggerMaskTechTrig = m_l1GtTmTech->gtTriggerMask();

            m_l1GtTmTechCacheID = l1GtTmTechCacheID;

        }

        // get Global Trigger technical trigger word, update the tokenResult members
        // from m_l1AlgoLogicParser and get the result for the logical expression
        const std::vector<bool>& gtTechTrigWord =
                gtReadoutRecord->technicalTriggerWord();
        updateAlgoLogicParser(gtTechTrigWord, m_triggerMaskTechTrig,
                physicsDaqPartition);

        // always empty filter - GT not aware of objects for technical triggers
        iEvent.put(filterObject);

        bool seedsResult = m_l1AlgoLogicParser.expressionResult();

        if (seedsResult) {
            return true;
        } else {
            return false;
        }

    }

    // seeding via physics algorithms

    // get / update the trigger menu from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtMenuCacheID =
            evSetup.get<L1GtTriggerMenuRcd>().cacheIdentifier();

    if (m_l1GtMenuCacheID != l1GtMenuCacheID) {

        edm::ESHandle<L1GtTriggerMenu> l1GtMenu;
        evSetup.get<L1GtTriggerMenuRcd>().get(l1GtMenu);
        m_l1GtMenu = l1GtMenu.product();
        (const_cast<L1GtTriggerMenu*> (m_l1GtMenu))->buildGtConditionMap(); //...ugly

        m_l1GtMenuCacheID = l1GtMenuCacheID;

        const AlgorithmMap& algorithmMap = l1GtMenu->gtAlgorithmMap();
        const AlgorithmMap& algorithmAliasMap = l1GtMenu->gtAlgorithmAliasMap();

        LogTrace("HLTLevel1GTSeed") << "\n L1 trigger menu "
                << l1GtMenu->gtTriggerMenuInterface()
                << "\n    Number of algorithm names:   "
                << (algorithmMap.size())
                << "\n    Number of algorithm aliases: "
                << (algorithmAliasMap.size()) << "\n" << std::endl;

        // update also the tokenNumber members (holding the bit numbers) from m_l1AlgoLogicParser
        if (m_l1UseAliasesForSeeding) {
            updateAlgoLogicParser(m_l1GtMenu, algorithmAliasMap);
        } else {
            updateAlgoLogicParser(m_l1GtMenu, algorithmMap);
        }
    }

    // get / update the trigger mask from the EventSetup
    // local cache & check on cacheIdentifier

    unsigned long long l1GtTmAlgoCacheID = evSetup.get<
            L1GtTriggerMaskAlgoTrigRcd>().cacheIdentifier();

    if (m_l1GtTmAlgoCacheID != l1GtTmAlgoCacheID) {

        edm::ESHandle<L1GtTriggerMask> l1GtTmAlgo;
        evSetup.get<L1GtTriggerMaskAlgoTrigRcd>().get(l1GtTmAlgo);
        m_l1GtTmAlgo = l1GtTmAlgo.product();

        m_triggerMaskAlgoTrig = m_l1GtTmAlgo->gtTriggerMask();

        m_l1GtTmAlgoCacheID = l1GtTmAlgoCacheID;

    }

    // FinalOR is true, it was tested before
    if (m_l1UseL1TriggerObjectMaps) {
        if (!(seedsL1TriggerObjectMaps(iEvent, filterObject,
                gtReadoutRecordPtr, physicsDaqPartition))) {

            iEvent.put(filterObject);
            return false;
        }
    } else {
        if (!(seedsL1Extra(iEvent, filterObject))) {

            iEvent.put(filterObject);
            return false;
        }

    }

    if (m_isDebugEnabled) {
        dumpTriggerFilterObjectWithRefs(filterObject);
    }

    //

    iEvent.put(filterObject);

    return true;

}

const std::vector<L1GtObject>* HLTLevel1GTSeed::objectTypeVec(const int chipNr,
        const std::string& cndName) {

    bool foundCond = false;

    const ConditionMap& conditionMap =
            (m_l1GtMenu->gtConditionMap()).at(chipNr);

    CItCond itCond = conditionMap.find(cndName);
    if (itCond != conditionMap.end()) {
        foundCond = true;
        return (&((itCond->second)->objectType()));
    }

    if (!foundCond) {

        // it should never be happen, all conditions are in the maps
        throw cms::Exception("FailModule") << "\nCondition " << cndName
                << " not found in the condition map" << " for chip number "
                << chipNr << std::endl;
    }

    // dummy return - prevent compiler warning
    return 0;

}

// for a new L1 Trigger menu, update the tokenNumber (holding the bit numbers)
// from m_l1AlgoLogicParser and from m_l1AlgoSeeds, and fill the m_l1AlgoSeedsRpn vector
void HLTLevel1GTSeed::updateAlgoLogicParser(const L1GtTriggerMenu* l1GtMenu,
        const AlgorithmMap& algorithmMap) {

    std::vector<L1GtLogicParser::OperandToken>& algOpTokenVector =
            m_l1AlgoLogicParser.operandTokenVector();

    size_t jSeed = 0;
    size_t l1AlgoSeedsSize = m_l1AlgoSeeds.size();

    // clear the content from the previous menu for the vector of RPN vectors m_l1AlgoSeedsRpn
    // and for the the vector of object-type vectors m_l1AlgoSeedsObjType
    m_l1AlgoSeedsRpn.clear();
    m_l1AlgoSeedsObjType.clear();

    //

    for (size_t i = 0; i < algOpTokenVector.size(); ++i) {

        CItAlgo itAlgo = algorithmMap.find((algOpTokenVector[i]).tokenName);
        if (itAlgo != algorithmMap.end()) {

            int bitNr = (itAlgo->second).algoBitNumber();
            int chipNr = (itAlgo->second).algoChipNumber();

            (algOpTokenVector[i]).tokenNumber = bitNr;

            // algOpTokenVector and m_l1AlgoSeeds must have the same ordering
            // of the algorithms
            if (jSeed < l1AlgoSeedsSize) {

                //LogTrace("HLTLevel1GTSeed") << "(m_l1AlgoSeeds[jSeed]).tokenName: "
                //    << (m_l1AlgoSeeds[jSeed]).tokenName
                //    << std::endl;

                if ((m_l1AlgoSeeds[jSeed]).tokenName
                        == (algOpTokenVector[i]).tokenName) {

                    (m_l1AlgoSeeds[jSeed]).tokenNumber = bitNr;

                    const std::vector<L1GtLogicParser::TokenRPN>& aRpnVector =
                            (itAlgo->second).algoRpnVector();
                    size_t aRpnVectorSize = aRpnVector.size();

                    m_l1AlgoSeedsRpn.push_back(&aRpnVector);

                    // loop over RpnVector to fill for each condition the object type
                    std::vector<const std::vector<L1GtObject>*> tmpObjTypeVec;
                    tmpObjTypeVec.reserve(aRpnVectorSize);

                    for (size_t opI = 0; opI < aRpnVectorSize; ++opI) {

                        std::string cName = (aRpnVector[opI]).operand;

                        if (!cName.empty()) {

                            tmpObjTypeVec.push_back(
                                    objectTypeVec(chipNr, cName));

                            //LogTrace("HLTLevel1GTSeed")
                            //    << "    Push object vector for condition: " << cName
                            //    << std::endl;
                        }
                    }

                    m_l1AlgoSeedsObjType.push_back(tmpObjTypeVec);

                    jSeed++;
                }
            }
        } else {

            throw cms::Exception("FailModule") << "\nAlgorithm  "
                    << (algOpTokenVector[i]).tokenName
                    << ", requested as seed by a HLT path, not found in the L1 trigger menu\n   "
                    << l1GtMenu->gtTriggerMenuName()
                    << "\nIncompatible L1 and HLT menus.\n" << std::endl;

        }

    }

    //
    if (m_isDebugEnabled) {
        bool newMenu = true;
        debugPrint(newMenu);
    }

}

// update the tokenResult members from m_l1AlgoLogicParser
// for a new event
void HLTLevel1GTSeed::updateAlgoLogicParser(const std::vector<bool>& gtWord,
        const std::vector<unsigned int>& triggerMask,
        const int physicsDaqPartition) {

    std::vector<L1GtLogicParser::OperandToken>& algOpTokenVector =
            m_l1AlgoLogicParser.operandTokenVector();

    for (size_t i = 0; i < algOpTokenVector.size(); ++i) {
        int iBit = (algOpTokenVector[i]).tokenNumber;
        bool iResult = gtWord.at(iBit);

        int triggerMaskBit = triggerMask[iBit] & (1 << physicsDaqPartition);
        //LogTrace("HLTLevel1GTSeed")
        //<< "\nTrigger bit: " << iBit
        //<< " mask = " << triggerMaskBit
        //<< " DAQ partition " << physicsDaqPartition
        //<< std::endl;

        if (triggerMaskBit) {
            iResult = false;

            //LogTrace("HLTLevel1GTSeed")
            //<< "\nMasked trigger: " << iBit << ". Result set to false\n"
            //<< std::endl;
        }

        (algOpTokenVector[i]).tokenResult = iResult;

    }

    for (size_t i = 0; i < m_l1AlgoSeeds.size(); ++i) {
        int iBit = (m_l1AlgoSeeds[i]).tokenNumber;
        bool iResult = gtWord.at(iBit);

        int triggerMaskBit = triggerMask[iBit] & (1 << physicsDaqPartition);
        //LogTrace("HLTLevel1GTSeed")
        //<< "\nTrigger bit: " << iBit
        //<< " mask = " << triggerMaskBit
        //<< " DAQ partition " << physicsDaqPartition
        //<< std::endl;

        if (triggerMaskBit) {
            iResult = false;

            //LogTrace("HLTLevel1GTSeed")
            //<< "\nMasked trigger: " << iBit << ". Result set to false\n"
            //<< std::endl;
        }

        (m_l1AlgoSeeds[i]).tokenResult = iResult;

    }

    if (m_isDebugEnabled) {
        bool newMenu = false;
        debugPrint(newMenu);
    }

}

// for seeding via technical triggers, convert the "name" to tokenNumber
// (seeding via bit numbers) - done once in constructor
void HLTLevel1GTSeed::convertStringToBitNumber() {

    std::vector<L1GtLogicParser::OperandToken>& algOpTokenVector =
            m_l1AlgoLogicParser.operandTokenVector();

    for (size_t i = 0; i < algOpTokenVector.size(); ++i) {

        std::string bitString = (algOpTokenVector[i]).tokenName;
        std::istringstream bitStream(bitString);
        int bitInt;

        if ((bitStream >> bitInt).fail()) {

            throw cms::Exception("FailModule")
                    << "\nL1 Seeds Logical Expression: = '"
                    << m_l1SeedsLogicalExpression << "'"
                    << "\n  Conversion to integer failed for " << bitString
                    << std::endl;
        }

        (algOpTokenVector[i]).tokenNumber = bitInt;

    }

    for (size_t i = 0; i < m_l1AlgoSeeds.size(); ++i) {

        std::string bitString = (m_l1AlgoSeeds[i]).tokenName;
        std::istringstream bitStream(bitString);
        int bitInt;

        if ((bitStream >> bitInt).fail()) {

            throw cms::Exception("FailModule")
                    << "\nL1 Seeds Logical Expression: = '"
                    << m_l1SeedsLogicalExpression << "'"
                    << "\n  Conversion to integer failed for " << bitString
                    << std::endl;
        }

        (m_l1AlgoSeeds[i]).tokenNumber = bitInt;
    }

}

// debug print grouped in a single function
// can be called for a new menu (bool "true") or for a new event
void HLTLevel1GTSeed::debugPrint(bool newMenu) {

    if (m_l1TechTriggerSeeding) {
        LogDebug("HLTLevel1GTSeed")
                << "\n\nupdateAlgoLogicParser: seeding via technical trigger"
                << "\n   update event quantities." << std::endl;

    } else {

        if (newMenu) {
            LogDebug("HLTLevel1GTSeed")
                    << "\n\nupdateAlgoLogicParser: L1 trigger menu changed to "
                    << m_l1GtMenu->gtTriggerMenuName() << std::endl;
        } else {
            LogDebug("HLTLevel1GTSeed")
                    << "\n\nupdateAlgoLogicParser: L1 trigger menu unchanged ("
                    << m_l1GtMenu->gtTriggerMenuName()
                    << ")\n   update event quantities." << std::endl;
        }
    }

    std::vector<L1GtLogicParser::OperandToken>& algOpTokenVector =
            m_l1AlgoLogicParser.operandTokenVector();

    LogTrace("HLTLevel1GTSeed")
            << "\n\nupdateAlgoLogicParser: algOpTokenVector.size() = "
            << algOpTokenVector.size() << std::endl;

    for (size_t i = 0; i < algOpTokenVector.size(); ++i) {

        LogTrace("HLTLevel1GTSeed") << "      " << std::setw(5)
                << (algOpTokenVector[i]).tokenNumber << "\t" << std::setw(25)
                << (algOpTokenVector[i]).tokenName << "\t"
                << (algOpTokenVector[i]).tokenResult << std::endl;
    }

    LogTrace("HLTLevel1GTSeed") << std::endl;

    LogTrace("HLTLevel1GTSeed")
            << "\nupdateAlgoLogicParser: m_l1AlgoSeeds.size() = "
            << m_l1AlgoSeeds.size() << std::endl;

    for (size_t i = 0; i < m_l1AlgoSeeds.size(); ++i) {

        LogTrace("HLTLevel1GTSeed") << "      " << std::setw(5)
                << (m_l1AlgoSeeds[i]).tokenNumber << "\t" << std::setw(25)
                << (m_l1AlgoSeeds[i]).tokenName << "\t"
                << (m_l1AlgoSeeds[i]).tokenResult << std::endl;
    }

    LogTrace("HLTLevel1GTSeed") << std::endl;

    if (!newMenu) {
        return;
    }

    LogTrace("HLTLevel1GTSeed")
            << "\nupdateAlgoLogicParser: m_l1AlgoSeedsRpn.size() = "
            << m_l1AlgoSeedsRpn.size() << std::endl;

    for (size_t i = 0; i < m_l1AlgoSeedsRpn.size(); ++i) {

        LogTrace("HLTLevel1GTSeed") << "  Rpn vector size: "
                << (m_l1AlgoSeedsRpn[i])->size() << std::endl;

        for (size_t j = 0; j < (m_l1AlgoSeedsRpn[i])->size(); ++j) {

            LogTrace("HLTLevel1GTSeed") << "      ( "
                    << (*(m_l1AlgoSeedsRpn[i]))[j].operation << ", "
                    << (*(m_l1AlgoSeedsRpn[i]))[j].operand << " )" << std::endl;

        }
    }

    LogTrace("HLTLevel1GTSeed") << std::endl;

    LogTrace("HLTLevel1GTSeed") << "\nupdateAlgoLogicParser: "
            << "algorithms in seed expression: m_l1AlgoSeedsObjType.size() = "
            << m_l1AlgoSeedsObjType.size() << std::endl;

    for (size_t i = 0; i < m_l1AlgoSeedsObjType.size(); ++i) {

        LogTrace("HLTLevel1GTSeed")
                << "  Conditions for an algorithm: vector size: "
                << (m_l1AlgoSeedsObjType[i]).size() << std::endl;

        for (size_t j = 0; j < (m_l1AlgoSeedsObjType[i]).size(); ++j) {

            LogTrace("HLTLevel1GTSeed")
                    << "    Condition object type vector: size: "
                    << ((m_l1AlgoSeedsObjType[i])[j])->size() << std::endl;

            for (size_t k = 0; k < ((m_l1AlgoSeedsObjType[i])[j])->size(); ++k) {

                L1GtObject obj = (*((m_l1AlgoSeedsObjType[i])[j]))[k];
                LogTrace("HLTLevel1GTSeed") << "      " << obj << " ";

            }

            LogTrace("HLTLevel1GTSeed") << std::endl;

        }
    }

    LogTrace("HLTLevel1GTSeed") << std::endl;

}


// seeding is done via L1 trigger object maps, considering the objects which fired in L1
bool HLTLevel1GTSeed::seedsL1TriggerObjectMaps(edm::Event& iEvent,
        std::auto_ptr<trigger::TriggerFilterObjectWithRefs>& filterObject,
        const L1GlobalTriggerReadoutRecord* gtReadoutRecordPtr,
        const int physicsDaqPartition) {

    // get Global Trigger decision word, update the tokenResult members
    // from m_l1AlgoLogicParser and get the result for the logical expression
    const std::vector<bool>& gtDecisionWord = gtReadoutRecordPtr->decisionWord();
    updateAlgoLogicParser(gtDecisionWord, m_triggerMaskAlgoTrig, physicsDaqPartition);

    bool seedsResult = m_l1AlgoLogicParser.expressionResult();

    if (m_isDebugEnabled ) {
        // define an output stream to print into
        // it can then be directed to whatever log level is desired
        std::ostringstream myCoutStream;
        gtReadoutRecordPtr->printGtDecision(myCoutStream);

        LogTrace("HLTLevel1GTSeed")
        << myCoutStream.str()
        << "\nHLTLevel1GTSeed::filter "
        << "\nLogical expression (names) = '" << m_l1SeedsLogicalExpression << "'"
        << "\n  Result for logical expression: " << seedsResult << "\n"
        << std::endl;
    }

    // the evaluation of the logical expression is false - skip event
    if ( !seedsResult) {

        return false;

    }

    // define index lists for all particle types

    std::list<int> listMuon;

    std::list<int> listIsoEG;
    std::list<int> listNoIsoEG;

    std::list<int> listCenJet;
    std::list<int> listForJet;
    std::list<int> listTauJet;

    std::list<int> listETM;
    std::list<int> listETT;
    std::list<int> listHTT;
    std::list<int> listHTM;

    std::list<int> listJetCounts;

    // get handle to object maps (one object map per algorithm)
    edm::Handle<L1GlobalTriggerObjectMapRecord> gtObjectMapRecord;
    iEvent.getByLabel(m_l1GtObjectMapTag, gtObjectMapRecord);

    if (!gtObjectMapRecord.isValid()) {
        edm::LogWarning("HLTLevel1GTSeed")
        << "\nWarning: L1GlobalTriggerObjectMapRecord with input tag "
        << m_l1GtReadoutRecordTag
        << "\nrequested in configuration, but not found in the event." << std::endl;

        return false;
    }

    // TODO check that the L1GlobalTriggerObjectMapRecord corresponds to the same menu as
    // the menu run by HLTLevel1GTSeed
    //     true normally online (they are run in the same job)
    //     can be false offline, when re-running HLT without re-running the object map producer

    // loop over the list of required algorithms for seeding
    int iAlgo = -1;

    for (std::vector<L1GtLogicParser::OperandToken>::const_iterator
            itSeed = m_l1AlgoSeeds.begin(); itSeed != m_l1AlgoSeeds.end(); ++itSeed) {

        //
        iAlgo++;
        //
        int algBit = (*itSeed).tokenNumber;
        std::string algName = (*itSeed).tokenName;
        bool algResult = (*itSeed).tokenResult;

        LogTrace("HLTLevel1GTSeed")
        << "\nHLTLevel1GTSeed::filter "
        << "\n  Algorithm " << algName << " with bit number " << algBit
        << " in the object map seed list"
        << "\n  Algorithm result = " << algResult << "\n"
        << std::endl;

        // algorithm result is false - no seeds
        if ( !algResult) {
            continue;
        }

        // algorithm result is true - get object map, loop over conditions in the algorithm
        const L1GlobalTriggerObjectMap* objMap = gtObjectMapRecord->getObjectMap(algBit);

        if (objMap == 0) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1GlobalTriggerObjectMap for algorithm  " << algName
            << " (bit number " << algBit << ") does not exist.\nReturn false.\n"
            << std::endl;

            iEvent.put(filterObject);
            return false;
        }

        const std::vector<L1GtLogicParser::OperandToken>& opTokenVecObjMap =
        objMap->operandTokenVector();

        const std::vector<L1GtLogicParser::TokenRPN>& algoSeedsRpn =
        * ( m_l1AlgoSeedsRpn.at(iAlgo) );

        const std::vector<const std::vector<L1GtObject>*>& algoSeedsObjTypeVec =
        m_l1AlgoSeedsObjType[iAlgo];

        //
        L1GtLogicParser logicParserConditions(algoSeedsRpn, opTokenVecObjMap);

        // get list of required conditions for seeding - loop over
        std::vector<L1GtLogicParser::OperandToken> condSeeds =
        logicParserConditions.expressionSeedsOperandList();

        if (m_isDebugEnabled ) {

            LogTrace("HLTLevel1GTSeed")
            << "\n  HLTLevel1GTSeed::filter "
            << "\n    condSeeds.size() = "
            << condSeeds.size()
            << std::endl;

            for (size_t i = 0; i < condSeeds.size(); ++i) {

                LogTrace("HLTLevel1GTSeed")
                << "      " << std::setw(5) << (condSeeds[i]).tokenNumber << "\t"
                << std::setw(25) << (condSeeds[i]).tokenName << "\t"
                << (condSeeds[i]).tokenResult
                << std::endl;
            }

            LogTrace("HLTLevel1GTSeed")
            << std::endl;
        }

        for (std::vector<L1GtLogicParser::OperandToken>::const_iterator
                itCond = condSeeds.begin(); itCond != condSeeds.end(); itCond++) {

            std::string cndName = (*itCond).tokenName;
            int cndNumber = (*itCond).tokenNumber;
            bool cndResult = (*itCond).tokenResult;

            const std::vector<L1GtObject>* cndObjTypeVec = algoSeedsObjTypeVec.at(cndNumber);

            //LogTrace("HLTLevel1GTSeed")
            //    << "\n  HLTLevel1GTSeed::filter "
            //    << "\n    Condition " << cndName << " with number " << cndNumber
            //    << " in the seed list"
            //    << "\n    Condition result = " << cndResult << "\n"
            //    << std::endl;

            if ( !cndResult) {
                continue;
            }

            // loop over combinations for a given condition

            const CombinationsInCond* cndComb = objMap->getCombinationsInCond(cndNumber);

            for (std::vector<SingleCombInCond>::const_iterator
                    itComb = (*cndComb).begin(); itComb != (*cndComb).end(); itComb++) {

                // loop over objects in a combination for a given condition
                int iObj = 0;
                for (SingleCombInCond::const_iterator
                        itObject = (*itComb).begin(); itObject != (*itComb).end(); itObject++) {

                    // get object type and push indices on the list
                    const L1GtObject objTypeVal = (*cndObjTypeVec).at(iObj);

                    //LogTrace("HLTLevel1GTSeed")
                    //    << "\n    HLTLevel1GTSeed::filter "
                    //    << "\n      Add object of type " << objTypeVal
                    //    << " and index " << (*itObject) << " to the seed list."
                    //    << std::endl;

                    switch (objTypeVal) {
                        case Mu: {
                            listMuon.push_back(*itObject);
                        }

                        break;
                        case NoIsoEG: {
                            listNoIsoEG.push_back(*itObject);
                        }

                        break;
                        case IsoEG: {
                            listIsoEG.push_back(*itObject);
                        }

                        break;
                        case CenJet: {
                            listCenJet.push_back(*itObject);
                        }

                        break;
                        case ForJet: {
                            listForJet.push_back(*itObject);
                        }

                        break;
                        case TauJet: {
                            listTauJet.push_back(*itObject);
                        }

                        break;
                        case ETM: {
                            listETM.push_back(*itObject);

                        }

                        break;
                        case ETT: {
                            listETT.push_back(*itObject);

                        }

                        break;
                        case HTT: {
                            listHTT.push_back(*itObject);

                        }

                        break;
                        case HTM: {
                            listHTM.push_back(*itObject);

                        }

                        break;
                        case JetCounts: {
                            listJetCounts.push_back(*itObject);
                        }

                        break;
                        default: {
                            // should not arrive here

                            LogDebug("HLTLevel1GTSeed")
                            << "\n    HLTLevel1GTSeed::filter "
                            << "\n      Unknown object of type " << objTypeVal
                            << " and index " << (*itObject) << " in the seed list."
                            << std::endl;
                        }
                        break;
                    }

                    iObj++;

                }

            }

        }

    }

    // eliminate duplicates

    listMuon.sort();
    listMuon.unique();

    listIsoEG.sort();
    listIsoEG.unique();

    listNoIsoEG.sort();
    listNoIsoEG.unique();

    listCenJet.sort();
    listCenJet.unique();

    listForJet.sort();
    listForJet.unique();

    listTauJet.sort();
    listTauJet.unique();

    listETM.sort();
    listETM.unique();

    listETT.sort();
    listETT.unique();

    listHTT.sort();
    listHTT.unique();

    listHTM.sort();
    listHTM.unique();

    listJetCounts.sort();
    listJetCounts.unique();

    //
    // record the L1 physics objects in the HLT filterObject
    //

    // muon
    if (listMuon.size()) {

        edm::Handle<l1extra::L1MuonParticleCollection> l1Muon;
        iEvent.getByLabel(m_l1MuonTag, l1Muon);

        if (!l1Muon.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1MuonParticleCollection with input tag " << m_l1MuonTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo muon added to filterObject." << std::endl;

        } else {

            for (std::list<int>::const_iterator itObj = listMuon.begin(); itObj != listMuon.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1Mu, l1extra::L1MuonParticleRef(
                                l1Muon, *itObj));

            }
        }

    }

    // EG (isolated)
    if (listIsoEG.size()) {
        edm::Handle<l1extra::L1EmParticleCollection> l1IsoEG;
        iEvent.getByLabel(m_l1IsoEGTag, l1IsoEG);

        if (!l1IsoEG.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EmParticleCollection with input tag " << m_l1IsoEGTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo IsoEG added to filterObject." << std::endl;

        } else {
            for (std::list<int>::const_iterator itObj = listIsoEG.begin(); itObj != listIsoEG.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1IsoEG, l1extra::L1EmParticleRef(
                                l1IsoEG, *itObj));

            }
        }
    }

    // EG (no isolation)
    if (listNoIsoEG.size()) {
        edm::Handle<l1extra::L1EmParticleCollection> l1NoIsoEG;
        iEvent.getByLabel(m_l1NoIsoEGTag, l1NoIsoEG);

        if (!l1NoIsoEG.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EmParticleCollection with input tag " << m_l1NoIsoEGTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo NoIsoEG added to filterObject." << std::endl;

        } else {
            for (std::list<int>::const_iterator itObj = listNoIsoEG.begin(); itObj
                    != listNoIsoEG.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1NoIsoEG, l1extra::L1EmParticleRef(
                                l1NoIsoEG, *itObj));

            }
        }
    }

    // central jets
    if (listCenJet.size()) {
        edm::Handle<l1extra::L1JetParticleCollection> l1CenJet;
        iEvent.getByLabel(m_l1CenJetTag, l1CenJet);

        if (!l1CenJet.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1JetParticleCollection with input tag " << m_l1CenJetTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo CenJet added to filterObject." << std::endl;

        } else {
            for (std::list<int>::const_iterator itObj = listCenJet.begin(); itObj
                    != listCenJet.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1CenJet, l1extra::L1JetParticleRef(
                                l1CenJet, *itObj));

            }
        }
    }

    // forward jets
    if (listForJet.size()) {
        edm::Handle<l1extra::L1JetParticleCollection> l1ForJet;
        iEvent.getByLabel(m_l1ForJetTag, l1ForJet);

        if (!l1ForJet.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1JetParticleCollection with input tag " << m_l1ForJetTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo ForJet added to filterObject." << std::endl;

        } else {
            for (std::list<int>::const_iterator itObj = listForJet.begin(); itObj
                    != listForJet.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1ForJet, l1extra::L1JetParticleRef(
                                l1ForJet, *itObj));

            }
        }
    }

    // tau jets
    if (listTauJet.size()) {
        edm::Handle<l1extra::L1JetParticleCollection> l1TauJet;
        iEvent.getByLabel(m_l1TauJetTag, l1TauJet);

        if (!l1TauJet.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1JetParticleCollection with input tag " << m_l1TauJetTag
            << "\nrequested in configuration, but not found in the event."
            << "\nNo TauJet added to filterObject." << std::endl;

        } else {
            for (std::list<int>::const_iterator itObj = listTauJet.begin(); itObj
                    != listTauJet.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1TauJet, l1extra::L1JetParticleRef(
                                l1TauJet, *itObj));

            }
        }
    }

    // energy sums
    if (listETM.size()) {
        edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
        iEvent.getByLabel(m_l1EtMissMET, l1EnergySums);

        if (!l1EnergySums.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMET
            << "\nrequested in configuration, but not found in the event."
            << "\nNo ETM added to filterObject." << std::endl;
        } else if (l1EnergySums->size() == 0) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMET
            << "\nfound in the event but with 0 size." << "\nNo ETM added to filterObject."
            << std::endl;

        } else {

            for (std::list<int>::const_iterator itObj = listETM.begin(); itObj != listETM.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1ETM, l1extra::L1EtMissParticleRef(
                                l1EnergySums, *itObj));

            }

        }

    }

    if (listETT.size()) {
        edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
        iEvent.getByLabel(m_l1EtMissMET, l1EnergySums);

        if (!l1EnergySums.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMET
            << "\nrequested in configuration, but not found in the event."
            << "\nNo ETT added to filterObject." << std::endl;
        } else if (l1EnergySums->size() == 0) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMET
            << "\nfound in the event but with 0 size." << "\nNo ETT added to filterObject."
            << std::endl;

        } else {

            for (std::list<int>::const_iterator itObj = listETT.begin(); itObj != listETT.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1ETT, l1extra::L1EtMissParticleRef(
                                l1EnergySums, *itObj));

            }

        }

    }

    if (listHTT.size()) {
        edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
        iEvent.getByLabel(m_l1EtMissMHT, l1EnergySums);

        if (!l1EnergySums.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMHT
            << "\nrequested in configuration, but not found in the event."
            << "\nNo HTT added to filterObject." << std::endl;

        } else if (l1EnergySums->size() == 0) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMHT
            << "\nfound in the event but with 0 size." << "\nNo HTT added to filterObject."
            << std::endl;

        } else {

            for (std::list<int>::const_iterator itObj = listHTT.begin(); itObj != listHTT.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1HTT, l1extra::L1EtMissParticleRef(
                                l1EnergySums, *itObj));

            }

        }
    }

    if (listHTM.size()) {
        edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
        iEvent.getByLabel(m_l1EtMissMHT, l1EnergySums);

        if (!l1EnergySums.isValid()) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMHT
            << "\nrequested in configuration, but not found in the event."
            << "\nNo HTM added to filterObject." << std::endl;

        } else if (l1EnergySums->size() == 0) {
            edm::LogWarning("HLTLevel1GTSeed")
            << "\nWarning: L1EtMissParticleCollection with input tag " << m_l1EtMissMHT
            << "\nfound in the event but with 0 size." << "\nNo HTM added to filterObject."
            << std::endl;

        } else {

            for (std::list<int>::const_iterator itObj = listHTM.begin(); itObj != listHTM.end(); ++itObj) {

                filterObject->addObject(trigger::TriggerL1HTM, l1extra::L1EtMissParticleRef(
                                l1EnergySums, *itObj));

            }
        }
    }

    // TODO FIXME uncomment if block when JetCounts implemented

    //    // jet counts
    //    if (listJetCounts.size()) {
    //        edm::Handle<l1extra::L1JetCounts> l1JetCounts;
    //        iEvent.getByLabel(m_l1CollectionsTag.label(), l1JetCounts);
    //
    //        for (std::list<int>::const_iterator itObj = listJetCounts.begin();
    //                itObj != listJetCounts.end(); ++itObj) {
    //
    //            filterObject->addObject(trigger::TriggerL1JetCounts,l1extra::L1JetCountsRefProd(l1JetCounts));
    //                  // FIXME: RefProd!
    //
    //        }
    //
    //    }

    return seedsResult;

}

// seeding is done ignoring if a L1 object fired or not
// if the event is selected at L1, fill all the L1 objects of types corresponding to the
// L1 conditions from the seeding logical expression for bunch crosses F, 0, 1
// directly from L1Extra and use them as seeds at HLT
// method and filter return true if at least an object is filled
bool HLTLevel1GTSeed::seedsL1Extra(edm::Event& iEvent, std::auto_ptr<
        trigger::TriggerFilterObjectWithRefs>& filterObject) {



    //    if (m_isDebugEnabled) {
    //
    //        LogTrace("HLTLevel1GTSeed") << "\n Printing muons from gtDigis\n " << std::endl;
    //
    //        edm::Handle<std::vector<L1MuGMTCand> > muonData;
    //        iEvent.getByLabel("gtDigis", muonData);
    //
    //        if (!muonData.isValid()) {
    //            edm::LogWarning("HLTLevel1GTSeed")
    //                    << "\nWarning: std::vector<L1MuGMTCand> with input tag "
    //                    << "gtDigis"
    //                    << "\nrequested in configuration, but not found in the event.\n"
    //                    << std::endl;
    //        } else {
    //
    //            std::vector<L1MuGMTCand>::const_iterator itMuon;
    //            for (itMuon = muonData->begin(); itMuon != muonData->end(); itMuon++) {
    //
    //                LogTrace("HLTLevel1GTSeed") << (*itMuon) << std::endl;
    //
    //            }
    //
    //        }
    //    }

    // define bools to prevent entering more copies of the objects
    bool includeMuon = true;

    bool includeIsoEG = true;
    bool includeNoIsoEG = true;

    bool includeCenJet = true;
    bool includeForJet = true;
    bool includeTauJet = true;

    bool includeETM = true;
    bool includeETT = true;
    bool includeHTT = true;
    bool includeHTM = true;

    bool includeJetCounts = true;

    //
    bool objectsInFilter = false;

    // loop over the list of required algorithms for seeding
    int iAlgo = -1;

    for (std::vector<L1GtLogicParser::OperandToken>::const_iterator itSeed =
            m_l1AlgoSeeds.begin(); itSeed != m_l1AlgoSeeds.end(); ++itSeed) {

        //
        iAlgo++;
        //
        int algBit = (*itSeed).tokenNumber;
        std::string algName = (*itSeed).tokenName;
        bool algResult = (*itSeed).tokenResult;

        LogTrace("HLTLevel1GTSeed") << "\nHLTLevel1GTSeed::filter "
                << "\n  Algorithm " << algName << " with bit number " << algBit
                << " in the object map seed list" << "\n  Algorithm result = "
                << algResult << std::endl;

        const std::vector<const std::vector<L1GtObject>*>& algoSeedsObjTypeVec =
                m_l1AlgoSeedsObjType[iAlgo];

        int minBxInEvent = (m_l1NrBxInEvent + 1)/2 - m_l1NrBxInEvent;
        int maxBxInEvent = (m_l1NrBxInEvent + 1)/2 - 1;

        // loop over all object types found for an algorithm and fill the lists
        //
        for (std::vector<const std::vector<L1GtObject>*>::const_iterator itVec =
                algoSeedsObjTypeVec.begin(); itVec != algoSeedsObjTypeVec.end(); ++itVec) {

            const std::vector<L1GtObject>* condObj = *itVec;
            for (std::vector<L1GtObject>::const_iterator itObj =
                    (*condObj).begin(); itObj != (*condObj).end(); ++itObj) {

                LogTrace("HLTLevel1GTSeed")
                        << "  Object type in conditions from this algorithm = "
                        << (*itObj) << std::endl;

                switch (*itObj) {
                    case Mu: {
                        if (includeMuon) {

                            edm::Handle<l1extra::L1MuonParticleCollection> l1Muon;
                            iEvent.getByLabel(m_l1MuonTag, l1Muon);

                            if (!l1Muon.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1MuonParticleCollection with input tag "
                                        << m_l1MuonTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo muon added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1MuonParticleCollection::const_iterator
                                        objIter = l1Muon->begin(); objIter
                                        != l1Muon->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1Mu,
                                                l1extra::L1MuonParticleRef(
                                                        l1Muon, iObj));
                                    }

                                }
                            }
                            includeMuon = false;
                        }
                    }

                        break;
                    case IsoEG: {
                        if (includeIsoEG) {
                            edm::Handle<l1extra::L1EmParticleCollection> l1IsoEG;
                            iEvent.getByLabel(m_l1IsoEGTag, l1IsoEG);

                            if (!l1IsoEG.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EmParticleCollection with input tag "
                                        << m_l1IsoEGTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo IsoEG added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EmParticleCollection::const_iterator
                                        objIter = l1IsoEG->begin(); objIter
                                        != l1IsoEG->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1IsoEG,
                                                l1extra::L1EmParticleRef(
                                                        l1IsoEG, iObj));

                                    }
                                }
                            }
                            includeIsoEG = false;
                        }

                    }
                        break;
                    case NoIsoEG: {
                        if (includeNoIsoEG) {
                            edm::Handle<l1extra::L1EmParticleCollection> l1NoIsoEG;
                            iEvent.getByLabel(m_l1NoIsoEGTag, l1NoIsoEG);

                            if (!l1NoIsoEG.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EmParticleCollection with input tag "
                                        << m_l1NoIsoEGTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo NoIsoEG added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EmParticleCollection::const_iterator
                                        objIter = l1NoIsoEG->begin(); objIter
                                        != l1NoIsoEG->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1NoIsoEG,
                                                l1extra::L1EmParticleRef(
                                                        l1NoIsoEG, iObj));

                                    }
                                }
                            }
                            includeNoIsoEG = false;
                        }

                    }
                        break;
                    case CenJet: {
                        if (includeCenJet) {
                            edm::Handle<l1extra::L1JetParticleCollection> l1CenJet;
                            iEvent.getByLabel(m_l1CenJetTag, l1CenJet);

                            if (!l1CenJet.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1JetParticleCollection with input tag "
                                        << m_l1CenJetTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo CenJet added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1JetParticleCollection::const_iterator
                                        objIter = l1CenJet->begin(); objIter
                                        != l1CenJet->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1CenJet,
                                                l1extra::L1JetParticleRef(
                                                        l1CenJet, iObj));

                                    }
                                }
                            }
                            includeCenJet = false;
                        }

                    }

                        break;
                    case ForJet: {
                        if (includeForJet) {
                            edm::Handle<l1extra::L1JetParticleCollection> l1ForJet;
                            iEvent.getByLabel(m_l1ForJetTag, l1ForJet);

                            if (!l1ForJet.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1JetParticleCollection with input tag "
                                        << m_l1ForJetTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo ForJet added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1JetParticleCollection::const_iterator
                                        objIter = l1ForJet->begin(); objIter
                                        != l1ForJet->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1ForJet,
                                                l1extra::L1JetParticleRef(
                                                        l1ForJet, iObj));

                                    }
                                }
                            }
                            includeForJet = false;
                        }

                    }

                        break;
                    case TauJet: {
                        if (includeTauJet) {
                            edm::Handle<l1extra::L1JetParticleCollection> l1TauJet;
                            iEvent.getByLabel(m_l1TauJetTag, l1TauJet);

                            if (!l1TauJet.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1JetParticleCollection with input tag "
                                        << m_l1TauJetTag
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo TauJet added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1JetParticleCollection::const_iterator
                                        objIter = l1TauJet->begin(); objIter
                                        != l1TauJet->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1TauJet,
                                                l1extra::L1JetParticleRef(
                                                        l1TauJet, iObj));

                                    }
                                }
                            }
                            includeTauJet = false;
                        }

                    }

                        break;
                    case ETM: {
                        if (includeETM) {
                            edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
                            iEvent.getByLabel(m_l1EtMissMET, l1EnergySums);

                            if (!l1EnergySums.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMET
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo ETM added to filterObject."
                                        << std::endl;

                            } else if (l1EnergySums->size() == 0) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMET
                                        << "\nfound in the event but with 0 size."
                                        << "\nNo ETM added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EtMissParticleCollection::const_iterator
                                        objIter = l1EnergySums->begin(); objIter
                                        != l1EnergySums->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1ETM,
                                                l1extra::L1EtMissParticleRef(
                                                        l1EnergySums, iObj));

                                    }
                                }
                            }
                            includeETM = false;
                        }

                    }

                        break;
                    case ETT: {
                        if (includeETT) {
                            edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
                            iEvent.getByLabel(m_l1EtMissMET, l1EnergySums);

                            if (!l1EnergySums.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMET
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo ETT added to filterObject."
                                        << std::endl;

                            } else if (l1EnergySums->size() == 0) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMET
                                        << "\nfound in the event but with 0 size."
                                        << "\nNo ETT added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EtMissParticleCollection::const_iterator
                                        objIter = l1EnergySums->begin(); objIter
                                        != l1EnergySums->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1ETT,
                                                l1extra::L1EtMissParticleRef(
                                                        l1EnergySums, iObj));

                                    }
                                }
                            }
                            includeETT = false;
                        }

                    }

                        break;
                    case HTT: {
                        if (includeHTT) {
                            edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
                            iEvent.getByLabel(m_l1EtMissMHT, l1EnergySums);

                            if (!l1EnergySums.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMHT
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo HTT added to filterObject."
                                        << std::endl;

                            } else if (l1EnergySums->size() == 0) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMHT
                                        << "\nfound in the event but with 0 size."
                                        << "\nNo HTT added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EtMissParticleCollection::const_iterator
                                        objIter = l1EnergySums->begin(); objIter
                                        != l1EnergySums->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1HTT,
                                                l1extra::L1EtMissParticleRef(
                                                        l1EnergySums, iObj));

                                    }
                                }
                            }
                            includeHTT = false;
                        }
                    }

                        break;
                    case HTM: {
                        if (includeHTM) {
                            edm::Handle<l1extra::L1EtMissParticleCollection> l1EnergySums;
                            iEvent.getByLabel(m_l1EtMissMHT, l1EnergySums);

                            if (!l1EnergySums.isValid()) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMHT
                                        << "\nrequested in configuration, but not found in the event."
                                        << "\nNo HTM added to filterObject."
                                        << std::endl;

                            } else if (l1EnergySums->size() == 0) {
                                edm::LogWarning("HLTLevel1GTSeed")
                                        << "\nWarning: L1EtMissParticleCollection with input tag "
                                        << m_l1EtMissMHT
                                        << "\nfound in the event but with 0 size."
                                        << "\nNo HTM added to filterObject."
                                        << std::endl;

                            } else {
                                int iObj = -1;
                                for (l1extra::L1EtMissParticleCollection::const_iterator
                                        objIter = l1EnergySums->begin(); objIter
                                        != l1EnergySums->end(); ++objIter) {

                                    iObj++;

                                    int bxNr = objIter->bx();
                                    if ((bxNr >= minBxInEvent) && (bxNr <= maxBxInEvent)) {

                                        objectsInFilter = true;
                                        filterObject->addObject(
                                                trigger::TriggerL1HTM,
                                                l1extra::L1EtMissParticleRef(
                                                        l1EnergySums, iObj));

                                    }
                                }
                            }
                            includeHTM = false;
                        }
                    }

                        break;
                    case JetCounts: {
                        if (includeJetCounts) {
                            // do nothing, JetCounts do not exist now
                        }
                    }

                        break;
                    default: {
                        // should not arrive here

                        LogDebug("HLTLevel1GTSeed")
                                << "\n    HLTLevel1GTSeed::filter "
                                << "\n      Unknown object of type " << *itObj
                                << " in the seed list." << std::endl;
                    }
                        break;
                }

            }

        }

        LogTrace("HLTLevel1GTSeed") << std::endl;
    }

    return objectsInFilter;

}

// detailed print of filter content
void HLTLevel1GTSeed::dumpTriggerFilterObjectWithRefs(std::auto_ptr<
        trigger::TriggerFilterObjectWithRefs>& filterObject) {

    LogDebug("HLTLevel1GTSeed") << "\nHLTLevel1GTSeed::filter "
            << "\n  Dump TriggerFilterObjectWithRefs\n" << std::endl;

    std::vector<l1extra::L1MuonParticleRef> seedsL1Mu;

    std::vector<l1extra::L1EmParticleRef> seedsL1IsoEG;
    std::vector<l1extra::L1EmParticleRef> seedsL1NoIsoEG;

    std::vector<l1extra::L1JetParticleRef> seedsL1CenJet;
    std::vector<l1extra::L1JetParticleRef> seedsL1ForJet;
    std::vector<l1extra::L1JetParticleRef> seedsL1TauJet;

    std::vector<l1extra::L1EtMissParticleRef> seedsL1ETM;
    std::vector<l1extra::L1EtMissParticleRef> seedsL1ETT;
    std::vector<l1extra::L1EtMissParticleRef> seedsL1HTT;
    std::vector<l1extra::L1EtMissParticleRef> seedsL1HTM;

    filterObject->getObjects(trigger::TriggerL1Mu, seedsL1Mu);
    const size_t sizeSeedsL1Mu = seedsL1Mu.size();

    filterObject->getObjects(trigger::TriggerL1IsoEG, seedsL1IsoEG);
    const size_t sizeSeedsL1IsoEG = seedsL1IsoEG.size();

    filterObject->getObjects(trigger::TriggerL1NoIsoEG, seedsL1NoIsoEG);
    const size_t sizeSeedsL1NoIsoEG = seedsL1NoIsoEG.size();

    filterObject->getObjects(trigger::TriggerL1CenJet, seedsL1CenJet);
    const size_t sizeSeedsL1CenJet = seedsL1CenJet.size();

    filterObject->getObjects(trigger::TriggerL1ForJet, seedsL1ForJet);
    const size_t sizeSeedsL1ForJet = seedsL1ForJet.size();

    filterObject->getObjects(trigger::TriggerL1TauJet, seedsL1TauJet);
    const size_t sizeSeedsL1TauJet = seedsL1TauJet.size();

    filterObject->getObjects(trigger::TriggerL1ETM, seedsL1ETM);
    const size_t sizeSeedsL1ETM = seedsL1ETM.size();

    filterObject->getObjects(trigger::TriggerL1ETT, seedsL1ETT);
    const size_t sizeSeedsL1ETT = seedsL1ETT.size();

    filterObject->getObjects(trigger::TriggerL1HTT, seedsL1HTT);
    const size_t sizeSeedsL1HTT = seedsL1HTT.size();

    filterObject->getObjects(trigger::TriggerL1HTM, seedsL1HTM);
    const size_t sizeSeedsL1HTM = seedsL1HTM.size();

    LogTrace("HLTLevel1GTSeed") << "  L1Mu seeds:      " << sizeSeedsL1Mu << "\n"
            << "  L1IsoEG seeds:   " << sizeSeedsL1IsoEG << "\n"
            << "  L1NoIsoEG seeds: " << sizeSeedsL1NoIsoEG << "\n"
            << "  L1CenJet seeds:  " << sizeSeedsL1CenJet << "\n"
            << "  L1ForJet seeds:  " << sizeSeedsL1ForJet << "\n"
            << "  L1TauJet seeds:  " << sizeSeedsL1TauJet << "\n"
            << "  L1ETM seeds:     " << sizeSeedsL1ETM << "\n"
            << "  L1ETT seeds:     " << sizeSeedsL1ETT << "\n"
            << "  L1HTT seeds:     " << sizeSeedsL1HTT << "\n"
            << "  L1HTM seeds:     " << sizeSeedsL1HTM << "\n" << std::endl;

    for (size_t i = 0; i != sizeSeedsL1Mu; i++) {

        l1extra::L1MuonParticleRef obj = l1extra::L1MuonParticleRef(
                seedsL1Mu[i]);

        LogTrace("HLTLevel1GTSeed") << "L1Mu     " << "\t" << "q*PT = "
                << obj->charge() * obj->pt() << "\t" << "eta =  " << obj->eta()
                << "\t" << "phi =  " << obj->phi() << "\t" << "BX = "
                << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1IsoEG; i++) {

        l1extra::L1EmParticleRef obj =
                l1extra::L1EmParticleRef(seedsL1IsoEG[i]);

        LogTrace("HLTLevel1GTSeed") << "L1IsoEG   " << "\t" << "ET =   "
                << obj->et() << "\t" << "eta =  " << obj->eta() << "\t"
                << "phi =  " << obj->phi() << "\t" << "BX = " << obj->bx();
        ;
    }

    for (size_t i = 0; i != sizeSeedsL1NoIsoEG; i++) {

        l1extra::L1EmParticleRef obj = l1extra::L1EmParticleRef(
                seedsL1NoIsoEG[i]);

        LogTrace("HLTLevel1GTSeed") << "L1NoIsoEG" << "\t" << "ET =   "
                << obj->et() << "\t" << "eta =  " << obj->eta() << "\t"
                << "phi =  " << obj->phi() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1CenJet; i++) {

        l1extra::L1JetParticleRef obj = l1extra::L1JetParticleRef(
                seedsL1CenJet[i]);

        LogTrace("HLTLevel1GTSeed") << "L1CenJet " << "\t" << "ET =   "
                << obj->et() << "\t" << "eta =  " << obj->eta() << "\t"
                << "phi =  " << obj->phi() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1ForJet; i++) {

        l1extra::L1JetParticleRef obj = l1extra::L1JetParticleRef(
                seedsL1ForJet[i]);

        LogTrace("HLTLevel1GTSeed") << "L1ForJet " << "\t" << "ET =   "
                << obj->et() << "\t" << "eta =  " << obj->eta() << "\t"
                << "phi =  " << obj->phi() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1TauJet; i++) {

        l1extra::L1JetParticleRef obj = l1extra::L1JetParticleRef(
                seedsL1TauJet[i]);

        LogTrace("HLTLevel1GTSeed") << "L1TauJet " << "\t" << "ET =   "
                << obj->et() << "\t" << "eta =  " << obj->eta() << "\t"
                << "phi =  " << obj->phi() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1ETM; i++) {

        l1extra::L1EtMissParticleRef obj = l1extra::L1EtMissParticleRef(
                seedsL1ETM[i]);

        LogTrace("HLTLevel1GTSeed") << "L1ETM    " << "\t" << "ET =   "
                << obj->etMiss() << "\t" << "phi =  " << obj->phi() << "BX =  "
                << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1ETT; i++) {

        l1extra::L1EtMissParticleRef obj = l1extra::L1EtMissParticleRef(
                seedsL1ETT[i]);

        LogTrace("HLTLevel1GTSeed") << "L1ETT    " << "\t" << "ET =   "
                << obj->etTotal() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1HTT; i++) {

        l1extra::L1EtMissParticleRef obj = l1extra::L1EtMissParticleRef(
                seedsL1HTT[i]);

        LogTrace("HLTLevel1GTSeed") << "L1HTT    " << "\t" << "ET =   "
                << obj->etTotal() << "\t" << "BX = " << obj->bx();
    }

    for (size_t i = 0; i != sizeSeedsL1HTM; i++) {

        l1extra::L1EtMissParticleRef obj = l1extra::L1EtMissParticleRef(
                seedsL1HTM[i]);

        LogTrace("HLTLevel1GTSeed") << "L1HTM    " << "\t" << "ET =   "
                << obj->etMiss() << "\t" << "phi =  " << obj->phi() << "BX =  "
                << obj->bx();
    }

    LogTrace("HLTLevel1GTSeed") << " \n\n" << std::endl;

}
