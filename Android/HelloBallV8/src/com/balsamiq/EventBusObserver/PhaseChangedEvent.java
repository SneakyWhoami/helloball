package com.balsamiq.EventBusObserver;

/**
 * Created by morsini on 26/02/15.
 */
public class PhaseChangedEvent {
    
    double currentPhase;
    
    public PhaseChangedEvent(double phase)
    {
        currentPhase = phase;
    }

    public double getCurrentPhase() {
        return currentPhase;
    }
}
