import FWCore.ParameterSet.Config as cms

def egammaUserDataProducts(moduleName):
  return [
    cms.InputTag(moduleName, "e1x3"),
    cms.InputTag(moduleName, "e3x1"),
    cms.InputTag(moduleName, "e1x5"),
    cms.InputTag(moduleName, "e2x2"),
    cms.InputTag(moduleName, "e3x2"),
    cms.InputTag(moduleName, "e3x3"),
    cms.InputTag(moduleName, "e4x4"),
    cms.InputTag(moduleName, "e5x5"),

    cms.InputTag(moduleName, "e2x5Right"),
    cms.InputTag(moduleName, "e2x5Left"),
    cms.InputTag(moduleName, "e2x5Top"),
    cms.InputTag(moduleName, "e2x5Bottom"),
    cms.InputTag(moduleName, "e2x5Max"),

    cms.InputTag(moduleName, "eLeft"),
    cms.InputTag(moduleName, "eRight"),
    cms.InputTag(moduleName, "eTop"),
    cms.InputTag(moduleName, "eBottom"),

    cms.InputTag(moduleName, "eMax"),
    cms.InputTag(moduleName, "e2nd"),

    cms.InputTag(moduleName, "etaLat"),
    cms.InputTag(moduleName, "phiLat"),
    cms.InputTag(moduleName, "lat"),

    cms.InputTag(moduleName, "covEtaEta"),
    cms.InputTag(moduleName, "covEtaPhi"),
    cms.InputTag(moduleName, "covPhiPhi"),

    cms.InputTag(moduleName, "covIEtaIEta"),
    cms.InputTag(moduleName, "covIEtaIPhi"),
    cms.InputTag(moduleName, "covIPhiIPhi"),

    cms.InputTag(moduleName, "zernike20"),
    cms.InputTag(moduleName, "zernike42"),
  ]
# egammaUserDataProducts(moduleName) <--------------------------------------
