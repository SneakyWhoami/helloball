package com.balsamiq.EventBusObserver;

/**
 * Created by morsini on 26/02/15.
 */
public class BallCountChangedEvent {
    
    int ballCount;

    public int getBallCount() {
        return ballCount;
    }

    public BallCountChangedEvent(int count)
    {
        ballCount = count;

    }
}
