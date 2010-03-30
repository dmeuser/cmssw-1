#ifndef INCLUDE_ORA_OBJECTSTREAMER_H
#define INCLUDE_ORA_OBJECTSTREAMER_H

#include "IRelationalStreamer.h"
#include "RelationalStreamerFactory.h"
// externals
#include "Reflex/Type.h"

namespace ora {

  class MappingElement;
  class ContainerSchema;
  class DataElement;

  class ObjectStreamerBase {
    public:
    ObjectStreamerBase( const Reflex::Type& objectType, MappingElement& mapping, ContainerSchema& contSchema );
    virtual ~ObjectStreamerBase();
    virtual void processDataMember( DataElement& dataElement, IRelationalData& relationalData, Reflex::Type& dataMemberType, MappingElement& dataMemberMapping, RelationalBuffer* operationBuffer ) = 0;
    void buildBaseDataMembers( DataElement& dataElement, IRelationalData& relationalData, const Reflex::Type& objType, RelationalBuffer* operationBuffer );
    bool buildDataMembers( DataElement& dataElement, IRelationalData& relationalData, RelationalBuffer* operationBuffer );
    protected:
    RelationalStreamerFactory m_streamerFactory;
    private:
    Reflex::Type m_objectType;
    MappingElement& m_mapping;
  };
  
  class ObjectWriter : public ObjectStreamerBase, public IRelationalWriter{

    public:

      ObjectWriter( const Reflex::Type& objectType, MappingElement& mapping, ContainerSchema& contSchema );

      virtual ~ObjectWriter();
      
      bool build(DataElement& dataElement, IRelationalData& relationalData, RelationalBuffer& operationBuffer);

      void setRecordId( const std::vector<int>& identity );
      
      /// Writes a data element
      void write( int oid, const void* data );

    public:

      void processDataMember( DataElement& dataElement, IRelationalData& relationalData, Reflex::Type& dataMemberType, MappingElement& dataMemberMapping, RelationalBuffer* operationBuffer );

    private:

      std::vector< IRelationalWriter* > m_writers;
  };
  
  class ObjectUpdater : public ObjectStreamerBase, public IRelationalUpdater {

    public:

      ObjectUpdater( const Reflex::Type& objectType, MappingElement& mapping, ContainerSchema& contSchema );

      virtual ~ObjectUpdater();
      
      bool build(DataElement& dataElement, IRelationalData& relationalData, RelationalBuffer& operationBuffer);
      
      void setRecordId( const std::vector<int>& identity );
      
      /// Updates a data element
      void update( int oid,
                   const void* data );

    public:

      void processDataMember( DataElement& dataElement, IRelationalData& relationalData, Reflex::Type& dataMemberType, MappingElement& dataMemberMapping, RelationalBuffer* operationBuffer );

    private:
      
      std::vector< IRelationalUpdater* > m_updaters;
  };

  class ObjectReader : public ObjectStreamerBase, public IRelationalReader {

      public:

      ObjectReader( const Reflex::Type& objectType, MappingElement& mapping, ContainerSchema& contSchema );

      virtual ~ObjectReader();
      
      bool build( DataElement& offset, IRelationalData& relationalData);

      void select( int oid );
      
      void setRecordId( const std::vector<int>& identity );

      /// Reads a data element
      void read( void* destination );

    public:

      void processDataMember( DataElement& dataElement, IRelationalData& relationalData, Reflex::Type& dataMemberType, MappingElement& dataMemberMapping, RelationalBuffer* operationBuffer );

    private:
      
      std::vector< IRelationalReader* > m_readers;
  };

  class ObjectStreamer : public IRelationalStreamer 
  {
    public:
    ObjectStreamer( const Reflex::Type& objectType, MappingElement& mapping, ContainerSchema& contSchema );

    ~ObjectStreamer();

    IRelationalWriter* newWriter();

    IRelationalUpdater* newUpdater();

    IRelationalReader* newReader();
    
    private:
    Reflex::Type m_objectType;
    MappingElement& m_mapping;
    ContainerSchema& m_schema;
  }; 

}
#endif

    
      
