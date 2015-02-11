package com.balsamiq.HelloBall;

import android.app.Activity;
import android.os.Bundle;
import android.util.SparseArray;

public class HelloBallActivity extends Activity implements IModelObserver {
    
    Model _model;
    BallsDrawingView _view;
    
    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
        _model = new Model(this, this);
    }

    @Override
    public void ballCountChanged(int number) {

    }

    @Override
    public void displayListChanged(SparseArray<Ball> balls) {
        _view.displayListChanged(balls);
    }

    @Override
    public void eventsPerSecond(int events) {

    }

    @Override
    public void log(String message) {

    }
}
