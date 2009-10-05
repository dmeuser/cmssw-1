#include "SimDataFormats/Vertex/interface/SimVertex.h"

SimVertex::SimVertex() :vtxId(0){}
 
SimVertex::SimVertex(const math::XYZVectorD& v, float tof) :
    Core(v,tof), itrack(-1), vtxId(0) {}

SimVertex::SimVertex(const math::XYZVectorD& v, float tof, int it) :
    Core(v,tof), itrack(it), vtxId(0) {}

SimVertex::SimVertex(const CoreSimVertex & v, int it) :
    Core(v), itrack(it), vtxId(0) {}

SimVertex::SimVertex(const math::XYZVectorD& v, float tof, unsigned int vId) :
    Core(v,tof), itrack(-1), vtxId(vId) {}
 
SimVertex::SimVertex(const math::XYZVectorD& v, float tof, int it, unsigned int vId) :
    Core(v,tof), itrack(it), vtxId(vId) {}
 
SimVertex::SimVertex(const CoreSimVertex & v, int it, unsigned int vId) :
    Core(v), itrack(it), vtxId(vId) {}
 
std::ostream & operator <<(std::ostream & o , const SimVertex & v) 
{ return o << (SimVertex::Core)(v) << ", " <<  v.parentIndex() <<", "<<v.vertexId(); }
