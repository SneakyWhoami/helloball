package com.balsamiq.HelloBallDuktape;

import android.util.SparseArray;

import java.util.ArrayList;

/**
 * Created by morsini on 24/02/15.
 */
public class MockModelObserver implements IModelObserver {

    Ball _ballToMove;
    ArrayList<Integer> _eventsPerSecond = new ArrayList<Integer>();
    
    @Override
    public void ballCountChanged(int number) {
        
    }

    @Override
    public void displayListChanged(SparseArray<Ball> balls) {
        _ballToMove = balls.get(balls.keyAt(0));
    }

    public Ball getBallToMove() {
        return _ballToMove;
    }

    public int getEventsPerSecond() {
        int average = 0;
        for (int i = 0; i < _eventsPerSecond.size(); ++i) {
            average += _eventsPerSecond.get(i);
        }
        return average/_eventsPerSecond.size();
    }

    @Override
    public void eventsPerSecond(int events) {
        _eventsPerSecond.add(events);

    }

    @Override
    public void log(String message) {

    }
}
