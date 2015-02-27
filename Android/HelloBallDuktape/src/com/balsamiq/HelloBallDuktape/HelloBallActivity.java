package com.balsamiq.HelloBallDuktape;

import android.annotation.TargetApi;
import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.util.SparseArray;
import android.view.ViewTreeObserver;
import android.widget.TextView;

public class HelloBallActivity extends Activity implements IModelObserver {

    BallsDrawingView _view;
    TextView _fps;
    JavaModelWrapper _model;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
        _model = new JavaModelWrapper(AssetUtilities.readFromfile(this, "model.js"), this);
        _view.setModelWrapper(_model);
        _fps = (TextView) findViewById(R.id.fps);

        ViewTreeObserver viewTreeObserver = _view.getViewTreeObserver();
        if (viewTreeObserver.isAlive()) {
            viewTreeObserver.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
                @Override
                public void onGlobalLayout() {
                    if (Build.VERSION.SDK_INT < 16) {
                        removeLayoutListenerPre16(_view, this);
                    } else {
                        removeLayoutListenerPost16(_view, this);
                    }
                    _model.start(_view.getWidth(), _view.getHeight());
                }
            });
        }
    }

    @SuppressWarnings("deprecation")
    private void removeLayoutListenerPre16(BallsDrawingView view, ViewTreeObserver.OnGlobalLayoutListener listener) {
        view.getViewTreeObserver().removeGlobalOnLayoutListener(listener);
    }

    @TargetApi(16)
    private void removeLayoutListenerPost16(BallsDrawingView view, ViewTreeObserver.OnGlobalLayoutListener listener) {
        view.getViewTreeObserver().removeOnGlobalLayoutListener(listener);
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
