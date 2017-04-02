package com.balsamiq.EventBusObserver;
import com.balsamiq.HelloBall.Ball;

import java.util.HashMap;

/**
 * Created by morsini on 26/02/15.
 */
public class DisplayListChangedEvent {

    HashMap<Integer, com.balsamiq.HelloBall.Ball> balls;

    public HashMap<Integer, Ball> getBalls() {
        return balls;
    }

    public DisplayListChangedEvent(HashMap<Integer, Ball> balls) {
        this.balls = balls;
    }
}
