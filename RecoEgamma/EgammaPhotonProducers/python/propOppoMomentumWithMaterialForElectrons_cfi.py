import FWCore.ParameterSet.Config as cms

import TrackingTools.MaterialEffects.OppositeMaterialPropagator_cfi
#PropagatorWithMaterialESProducer 
oppositeToMomElePropagator = TrackingTools.MaterialEffects.OppositeMaterialPropagator_cfi.OppositeMaterialPropagator.clone()
oppositeToMomElePropagator.Mass = 0.000511
oppositeToMomElePropagator.ComponentName = 'oppositeToMomElePropagator'

