package com.balsamiq.HelloBall;

import android.util.SparseArray;
import org.mozilla.javascript.Scriptable;

/**
 * Created by morsini on 11/02/15.
 */
public interface IModelObserver {
    void ballCountChanged(int number);

    void displayListChanged(SparseArray<Ball> balls);

    void eventsPerSecond(int events);

    void log(String message);

}
