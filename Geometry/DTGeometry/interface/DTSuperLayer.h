#ifndef DTSUPERLAYER_H
#define DTSUPERLAYER_H

/** \class DTSuperLayer
 *
 *  Model of a superlayer in Muon Drift Tube chambers.
 *  
 *  A superlayer is composed by 4 staggered DTLayer s.
 *
 *  $date   : 13/01/2006 11:47:03 CET $
 *  $Revision: 1.6 $
 *  \author Stefano Lacaprara - INFN Padova <stefano.lacaprara@pd.infn.it>
 *
 */

/* Base Class Headers */
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

/* Collaborating Class Declarations */
#include "DataFormats/MuonDetId/interface/DTSuperLayerId.h"
class DTLayer;
class DTChamber;

/* C++ Headers */

/* ====================================================================== */

/* Class DTSuperLayer Interface */

class DTSuperLayer : public GeomDet {

  public:

/* Constructor */ 
    DTSuperLayer(DTSuperLayerId id,
                 ReferenceCountingPointer<BoundPlane>& plane,
                 const DTChamber* ch=0);

/* Destructor */ 
    virtual ~DTSuperLayer() ;

/* Operations */ 
    /// Return the DetId of this SL
    virtual DetId geographicalId() const;

    /// Return the DetId of this SL
    DTSuperLayerId id() const;

    /// true if id are the same
    bool operator==(const DTSuperLayer& sl) const ;

    /// Return the layers in the SL
    virtual std::vector< const GeomDet*> components() const;

    /// Return the layers in the SL
    std::vector< const DTLayer*> layers() const;

    /// Add layer to the SL which owns it
    void add(DTLayer* l);

    /// Return the chamber this SL belongs to (0 if any, eg if a SL is
    /// built on his own)
    const DTChamber* chamber() const;

  private:
    DTSuperLayerId theId;
    // The SL owns its Layer
    std::vector< const DTLayer*> theLayers;
    const DTChamber* theCh;

  protected:

};
#endif // DTSUPERLAYER_H

