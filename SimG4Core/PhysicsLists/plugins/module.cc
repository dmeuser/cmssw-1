#include "SimG4Core/Physics/interface/PhysicsListFactory.h"

#include "CMSModel.hh"
#include "DummyPhysics.hh"
#include "FTFCMS_BIC.hh"
#include "FTFPCMS.hh"
#include "FTFPCMS_BERT.hh"
#include "FTFPCMS_BERT_EML.hh"
#include "FTFPCMS_BERT_EMV.hh"
#include "LHEPCMS.hh"
#include "LHEPCMS_EMV.hh"
#include "QBBCCMS.hh"
#include "QGSBCMS_BERT_NOLEP1.hh"
#include "QGSBCMS_BERT_NOLEP1_EML.hh"
#include "QGSBCMS_BERT_NOLEP1_EMV.hh"
#include "QGSBCMS_BERT_NOLEP2.hh"
#include "QGSBCMS_BERT_NOLEP2_EML.hh"
#include "QGSBCMS_BERT_NOLEP2_EMV.hh"
#include "QGSCMS_BIC.hh"
#include "QGSCCMS.hh"
#include "QGSCCMS_BERT.hh"
#include "QGSPCMS.hh"
#include "QGSPCMS_BERT.hh"
#include "QGSPCMS_BERT_EML.hh"
#include "QGSPCMS_BERT_EMLGG.hh"
#include "QGSPCMS_BERT_EMLNoDR.hh"
#include "QGSPCMS_BERT_EMLSN.hh"
#include "QGSPCMS_BERT_EMLSYNC.hh"
#include "QGSPCMS_BERT_EMV.hh"
#include "QGSPCMS_BERT_EMX.hh"
#include "QGSPCMS_BERT_HP.hh"
#include "QGSPCMS_BERT_HP_EML.hh"
#include "QGSPCMS_BERT_LPM.hh"
#include "QGSPCMS_BERT_NOLEP1.hh"
#include "QGSPCMS_BERT_NOLEP1_EML.hh"
#include "QGSPCMS_BERT_NOLEP1_EMV.hh"
#include "QGSPCMS_BERT_NOLEP2.hh"
#include "QGSPCMS_BERT_NOLEP2_EML.hh"
#include "QGSPCMS_BERT_NOLEP2_EMV.hh"
#include "QGSPCMS_FTFP_BERT.hh"
#include "QGSPCMS_FTFP_BERT_EML.hh"
#include "QGSPCMS_BIC.hh"
#include "QGSPCMS_EMV.hh"

DEFINE_PHYSICSLIST(CMSModel);
DEFINE_PHYSICSLIST(DummyPhysics);
typedef FTFCMS_BIC FTF_BIC;
DEFINE_PHYSICSLIST(FTF_BIC);
typedef FTFPCMS FTFP;
DEFINE_PHYSICSLIST(FTFP);
typedef FTFPCMS_BERT FTFP_BERT;
DEFINE_PHYSICSLIST(FTFP_BERT);
typedef FTFPCMS_BERT_EML FTFP_BERT_EML;
DEFINE_PHYSICSLIST(FTFP_BERT_EML);
typedef FTFPCMS_BERT_EMV FTFP_BERT_EMV;
DEFINE_PHYSICSLIST(FTFP_BERT_EMV);
typedef LHEPCMS LHEP;
DEFINE_PHYSICSLIST(LHEP);
typedef LHEPCMS_EMV LHEP_EMV;
DEFINE_PHYSICSLIST(LHEP_EMV);
typedef QBBCCMS QBBC;
DEFINE_PHYSICSLIST(QBBC);
typedef QGSCMS_BIC QGS_BIC;
DEFINE_PHYSICSLIST(QGS_BIC);
typedef QGSBCMS_BERT_NOLEP1 QGSB_BERT_NOLEP1;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP1);
typedef QGSBCMS_BERT_NOLEP1_EML QGSB_BERT_NOLEP1_EML;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP1_EML);
typedef QGSBCMS_BERT_NOLEP1_EMV QGSB_BERT_NOLEP1_EMV;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP1_EMV);
typedef QGSBCMS_BERT_NOLEP2 QGSB_BERT_NOLEP2;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP2);
typedef QGSBCMS_BERT_NOLEP2_EML QGSB_BERT_NOLEP2_EML;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP2_EML);
typedef QGSBCMS_BERT_NOLEP2_EMV QGSB_BERT_NOLEP2_EMV;
DEFINE_PHYSICSLIST(QGSB_BERT_NOLEP2_EMV);
typedef QGSCCMS QGSC;
DEFINE_PHYSICSLIST(QGSC);
typedef QGSCCMS_BERT QGSC_BERT;
DEFINE_PHYSICSLIST(QGSC_BERT);
typedef QGSPCMS QGSP;
DEFINE_PHYSICSLIST(QGSP);
typedef QGSPCMS_BERT QGSP_BERT;
DEFINE_PHYSICSLIST(QGSP_BERT);
typedef QGSPCMS_BERT_EML QGSP_BERT_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_EML);
typedef QGSPCMS_BERT_EMLGG QGSP_BERT_EMLGG;
DEFINE_PHYSICSLIST(QGSP_BERT_EMLGG);
typedef QGSPCMS_BERT_EMLNoDR QGSP_BERT_EMLNoDR;
DEFINE_PHYSICSLIST(QGSP_BERT_EMLNoDR);
typedef QGSPCMS_BERT_EMLSN QGSP_BERT_EMLSN;
DEFINE_PHYSICSLIST(QGSP_BERT_EMLSN);
typedef QGSPCMS_BERT_EMLSYNC QGSP_BERT_EMLSYNC;
DEFINE_PHYSICSLIST(QGSP_BERT_EMLSYNC);
typedef QGSPCMS_BERT_EMV QGSP_BERT_EMV;
DEFINE_PHYSICSLIST(QGSP_BERT_EMV);
typedef QGSPCMS_BERT_EMX QGSP_BERT_EMX;
DEFINE_PHYSICSLIST(QGSP_BERT_EMX);
typedef QGSPCMS_BERT_HP QGSP_BERT_HP;
DEFINE_PHYSICSLIST(QGSP_BERT_HP);
typedef QGSPCMS_BERT_HP_EML QGSP_BERT_HP_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_HP_EML);
typedef QGSPCMS_BERT_LPM QGSP_BERT_LPM;
DEFINE_PHYSICSLIST(QGSP_BERT_LPM);
typedef QGSPCMS_BERT_NOLEP1 QGSP_BERT_NOLEP1;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP1);
typedef QGSPCMS_BERT_NOLEP1_EML QGSP_BERT_NOLEP1_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP1_EML);
typedef QGSPCMS_BERT_NOLEP1_EMV QGSP_BERT_NOLEP1_EMV;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP1_EMV);
typedef QGSPCMS_BERT_NOLEP2 QGSP_BERT_NOLEP2;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP2);
typedef QGSPCMS_BERT_NOLEP2_EML QGSP_BERT_NOLEP2_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP2_EML);
typedef QGSPCMS_BERT_NOLEP2_EMV QGSP_BERT_NOLEP2_EMV;
DEFINE_PHYSICSLIST(QGSP_BERT_NOLEP2_EMV);
typedef QGSPCMS_FTFP_BERT QGSP_FTFP_BERT;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT);
typedef QGSPCMS_FTFP_BERT_EML QGSP_FTFP_BERT_EML;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT_EML);
typedef QGSPCMS_BIC QGSP_BIC;
DEFINE_PHYSICSLIST(QGSP_BIC);
typedef QGSPCMS_EMV QGSP_EMV;
DEFINE_PHYSICSLIST(QGSP_EMV);
