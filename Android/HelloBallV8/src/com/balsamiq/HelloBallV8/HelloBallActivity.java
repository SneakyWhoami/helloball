package com.balsamiq.HelloBallV8;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.balsamiq.EventBusObserver.*;
import com.balsamiq.HelloBall.IModel;
import com.balsamiq.HelloBall.JavaModelV8;
import de.greenrobot.event.EventBus;

public class HelloBallActivity extends Activity {

    BallsDrawingView _view;
    TextView _fps;
    IModel _model;

    private Handler handler = new Handler();

    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            _model.triggerChangePhase();
            handler.postDelayed(this, 1000/60);
        }
    };

    protected static final String LOG_TAG = "HelloBallActivity";

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
        JavaModelV8 model = new JavaModelV8(new ModelObserver());
        _model = model;
        model.load(AssetUtilities.readFromfile(this, "model.js"), "model.js");
        model.load(AssetUtilities.readFromfile(this, "custom.js"), "custom.js");
        _view.setModel(_model);
        _fps = (TextView) findViewById(R.id.fps);

        Button start = (Button) findViewById(R.id.start);
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                _model.start(_view.getWidth(), _view.getHeight());
                handler.postDelayed(runnable, 100);
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    public void onEvent(EventsPerSecondEvent event) {
        _fps.setText("" + event.getEvents());
    }

    public void onEvent(LogEvent event) {
        Log.d(LOG_TAG, event.getMessage());
    }

    public void onEvent(DisplayListChangedEvent event) {
        _view.displayListChanged(event.getBalls());
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public void onEvent(PhaseChangedEvent event) {
        String string = Integer.toHexString((int) Math.floor(event.getCurrentPhase() * 255));
        if (string.length() == 1) {
            string = "0" + string;
        }
        StringBuilder stringBuilder = new StringBuilder();
//        stringBuilder.append("#");
        stringBuilder.append(string);
        stringBuilder.append(string);
        stringBuilder.append(string);
        Log.d(LOG_TAG, stringBuilder.toString());
        try {
            int color = Integer.parseInt(stringBuilder.toString(), 16) + 0xff000000;
            Log.d(LOG_TAG, "" + color);
            _view.setBackgroundColor(color);
            
        } catch (java.lang.IllegalArgumentException exception) {
            Log.d(LOG_TAG, stringBuilder.toString());
        }
    }
}
