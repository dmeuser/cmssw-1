#include "CondTools/L1Trigger/src/DataWriter.h"

#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/ConnectionConfiguration.h"
#include "CondCore/PluginSystem/interface/ProxyFactory.h"

#include <utility>

namespace l1t
{

/* Data writting functions */
std::string DataWriter::findTokenForTag (const std::string & tag)
{
    // fast check in cashe
    TagToToken::const_iterator found = tagToToken.find (tag);
    if (found != tagToToken.end ())
        return found->second;

    // else slow way, also we may need to create db
    coral->connect (cond::ReadWriteCreate);
    coral->startTransaction (false);

    std::string tagToken;
    if (metadata->hasTag (tag))
        tagToken = metadata->getToken (tag);

    coral->commit ();
    coral->disconnect ();

    // if not found empty string is returned
    return tagToken;
}

void DataWriter::writeKey (L1TriggerKey * key, const std::string & tag, const int sinceRun)
{
    // writting key as bit more complicated. At this time we have to worry
    // about such things if the key already exists or not
    // Also we need to get IOVToken for given tag if key exist
    // if it does not, then we need to addMapping in the end

    // Bad part - get TagTokent for given tag.
    // We use layzy cash to save all tag adn tokens, in case we save key with same tag
    std::string tagToken = findTokenForTag (tag);
    bool requireMapping = tagToken.empty ();

    // work with pool db
    pool->connect ();
    pool->startTransaction (false);

    cond::Ref<L1TriggerKey> ref (*pool, key);
    ref.markWrite ("L1TriggerKeyRcd");

    cond::IOVService iov (*pool);

    // Create editor, with or wothoug TagToken
    cond::IOVEditor * editor;
    editor = iov.newIOVEditor (tagToken);

    // finally insert new IOV
    editor->insert (sinceRun, ref.token ());
    tagToken = editor->token ();
    delete editor;

    pool->commit ();
    pool->disconnect ();

    if (tagToToken.find (tag) != tagToToken.end ())
        tagToToken.insert (std::make_pair (tag, tagToken));

    // Assign payload token with IOV value
    if (requireMapping)
        addMappings (tag, tagToken);
}

void DataWriter::addMappings (const std::string tag, const std::string iovToken)
{
    coral->connect (cond::ReadWriteCreate);
    coral->startTransaction (false);

    metadata->addMapping (tag, iovToken);

    coral->commit ();
    coral->disconnect ();
}

static std::string buildName( const std::string& iRecordName, const std::string& iTypeName )
{
    return iRecordName+"@"+iTypeName+"@Writer";
}

void DataWriter::writePayload (const L1TriggerKey & key, const edm::EventSetup & setup,
        const std::string & record, const std::string & type)
{
    WriterFactory * factory = WriterFactory::get();
    const std::string name = buildName(record, type);
    WriterProxy * writer = factory->create(name, std::string ("ha"), std::string ("ha"));
    assert (writer != 0);

    pool->connect ();
    pool->startTransaction (false);

    std::string payloadToken = writer->save(setup, *pool);
    std::cerr << "DataWriter::writePayload: got token from write: " << payloadToken << std::endl;
    assert (!payloadToken.empty ());
    delete writer;

    // we know that this is new IOV token so we can skip test. Execption at this moment
    // is also ok, becaus it is not knwo what to do if we try to reuse token
    // However, exceptions will be rised from addMappings method
    cond::IOVService iov (*pool);
    cond::IOVEditor * editor = iov.newIOVEditor ();
    editor->insert (edm::IOVSyncValue::endOfTime ().eventID ().run (), payloadToken);
    std::string token = editor->token ();
    std::cerr << "DataWriter::writePayload: got token from IOV: " << token << std::endl;
    delete editor;

    pool->commit ();
    pool->disconnect ();

    // Assign payload token with IOV value
    std::cerr << "DataWriter::writePayload: mapping with tag: " << key.getKey () << std::endl;
    addMappings (key.getKey (), token);
}


} // ns
