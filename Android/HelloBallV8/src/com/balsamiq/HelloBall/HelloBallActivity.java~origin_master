package com.balsamiq.HelloBall;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.util.SparseArray;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class HelloBallActivity extends Activity implements IModelObserver {

    BallsDrawingView _view;
    TextView _fps;
    JavaModelWrapper _model;
//    JavaModel _model;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
//        _model = new JavaModel(this, this);
        _model = new JavaModelWrapper(this, this);
        _view.setModelWrapper(_model);
        _fps = (TextView) findViewById(R.id.fps);

        Button start = (Button) findViewById(R.id.start);
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                _model.start(_view.getWidth(), _view.getHeight());
            }
        });
    }

    @Override
    public void ballCountChanged(int number) {
        Log.d("HelloBallActivity.ballCountChanged", "" + number);
    }

    @Override
    public void eventsPerSecond(final int events) {
        _fps.setText("" + events);
    }

    @Override
    public void log(String message) {

    }

    public void displayListChanged(SparseArray<Ball> balls) {
        _view.displayListChanged(balls);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }
}
