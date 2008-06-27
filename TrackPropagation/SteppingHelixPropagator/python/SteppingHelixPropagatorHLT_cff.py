import FWCore.ParameterSet.Config as cms

import TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi
SteppingHelixPropagatorL2Any = TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi.SteppingHelixPropagatorAny.clone()
import TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi
SteppingHelixPropagatorL2Along = TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi.SteppingHelixPropagatorAlong.clone()
import TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi
SteppingHelixPropagatorL2Opposite = TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi.SteppingHelixPropagatorOpposite.clone()
SteppingHelixPropagatorL2Any.ComponentName = 'SteppingHelixPropagatorL2Any'
SteppingHelixPropagatorL2Any.useTuningForL2Speed = True
SteppingHelixPropagatorL2Along.ComponentName = 'SteppingHelixPropagatorL2Along'
SteppingHelixPropagatorL2Along.useTuningForL2Speed = True
SteppingHelixPropagatorL2Opposite.ComponentName = 'SteppingHelixPropagatorL2Opposite'
SteppingHelixPropagatorL2Opposite.useTuningForL2Speed = True

