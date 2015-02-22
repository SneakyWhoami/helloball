package com.balsamiq.HelloBallNative;

import android.util.SparseArray;

/**
 * Created by morsini on 11/02/15.
 */
public interface IModelObserver {
    void ballCountChanged(int number);

    void displayListChanged(SparseArray<Ball> balls);

    void eventsPerSecond(int events);

    void log(String message);

}
