package com.balsamiq.HelloBall;

import java.util.HashMap;

/**
 * Created by morsini on 11/02/15.
 */
public interface IModelObserver {
    void ballCountChanged(int number);

    void displayListChanged(HashMap<Integer, Ball> balls);

    void eventsPerSecond(int events);

    void log(String message);
    
    void phaseChanged(double value);

    int measureText(String font, int fontSize, boolean bold, boolean italic, String text);

}
