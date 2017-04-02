package com.balsamiq.EventBusObserver;

import com.balsamiq.HelloBall.Ball;
import com.balsamiq.HelloBall.IModelObserver;
import com.balsamiq.HelloBall.TextRuler;
import org.greenrobot.eventbus.EventBus;

import java.util.HashMap;

/**
 * Created by morsini on 26/02/15.
 */
public class ModelObserver implements IModelObserver {

    TextRuler ruler;
    
    public ModelObserver(TextRuler ruler)
    {
        this.ruler = ruler;
    }
    
    @Override
    public void ballCountChanged(int number) {
        EventBus.getDefault().post(new com.balsamiq.EventBusObserver.BallCountChangedEvent(number));
    }

    @Override
    public void displayListChanged(HashMap<Integer, Ball> balls) {
        EventBus.getDefault().post(new com.balsamiq.EventBusObserver.DisplayListChangedEvent(balls));
    }

    @Override
    public void eventsPerSecond(int events) {
        EventBus.getDefault().post(new com.balsamiq.EventBusObserver.EventsPerSecondEvent(events));
    }

    @Override
    public void log(String message) {
        EventBus.getDefault().post(new com.balsamiq.EventBusObserver.LogEvent(message));
    }

    @Override
    public void phaseChanged(double value) {
        EventBus.getDefault().post(new com.balsamiq.EventBusObserver.PhaseChangedEvent(value));
    }

    @Override
    public float measureText(String font, int fontSize, boolean bold, boolean italic, String text)
    {
        return ruler.measureTextWidth(font, fontSize, bold, italic, text);
    }
}
