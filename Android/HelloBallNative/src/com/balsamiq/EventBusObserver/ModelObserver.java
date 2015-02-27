package com.balsamiq.EventBusObserver;

import com.balsamiq.HelloBall.Ball;
import com.balsamiq.HelloBall.IModelObserver;
import de.greenrobot.event.EventBus;

import java.util.HashMap;

/**
 * Created by morsini on 26/02/15.
 */
public class ModelObserver implements IModelObserver {

    @Override
    public void ballCountChanged(int number) {
        EventBus.getDefault().post(new BallCountChangedEvent(number));
    }

    @Override
    public void displayListChanged(HashMap<Integer, Ball> balls) {
        EventBus.getDefault().post(new DisplayListChangedEvent(balls));
    }

    @Override
    public void eventsPerSecond(int events) {
        EventBus.getDefault().post(new EventsPerSecondEvent(events));
    }

    @Override
    public void log(String message) {
        EventBus.getDefault().post(new LogEvent(message));
    }

    @Override
    public void phaseChanged(double value) {
        EventBus.getDefault().post(new PhaseChangedEvent(value));
    }
}
