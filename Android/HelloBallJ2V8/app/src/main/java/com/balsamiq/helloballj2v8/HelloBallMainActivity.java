package com.balsamiq.helloballj2v8;

import android.annotation.TargetApi;
import android.os.Build;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewTreeObserver;
import android.widget.TextView;
import baslsamiq.com.helloballj2v8.R;
import com.balsamiq.EventBusObserver.*;
import com.balsamiq.HelloBall.IModel;
import com.balsamiq.HelloBall.JavaModelV8;
import com.balsamiq.HelloBall.TextRuler;
import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

public class HelloBallMainActivity extends AppCompatActivity {

//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_hello_ball_main);
//
//        V8 runtime = V8.createV8Runtime();
//        int result = runtime.executeIntegerScript(""
//                + "var hello = 'hello, ';\n"
//                + "var world = 'world!';\n"
//                + "hello.concat(world).length;\n");
//        System.out.println(result);
//        runtime.release();
//    }

    BallsDrawingView _view;
    TextView _fps;
    IModel _model;

    private Handler handler = new Handler();

    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            _model.triggerChangePhase();
            handler.postDelayed(this, 1000 / 60);
        }
    };

    protected static final String LOG_TAG = "HelloBallActivity";

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hello_ball_main);

        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
        JavaModelV8 model = new JavaModelV8(new ModelObserver(new TextRuler(this)));
        _model = model;
        model.load(AssetUtilities.readFromfile(this, "model.js"), "model.js");
        model.load(AssetUtilities.readFromfile(this, "custom.js"), "custom.js");
        _view.setModel(_model);
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
                    handler.postDelayed(runnable, 100);
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
    public void onStart() {
        super.onStart();
        EventBus.getDefault().register(this);
    }

    @Override
    public void onStop() {
        EventBus.getDefault().unregister(this);
        super.onStop();
    }

    @Subscribe
    public void onEvent(EventsPerSecondEvent event) {
        _fps.setText("" + event.getEvents());
    }

    @Subscribe
    public void onEvent(LogEvent event) {
        Log.d(LOG_TAG, event.getMessage());
    }

    @Subscribe
    public void onEvent(DisplayListChangedEvent event) {
        _view.displayListChanged(event.getBalls());
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Subscribe
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
//        Log.d(LOG_TAG, stringBuilder.toString());
        try {
            int color = Integer.parseInt(stringBuilder.toString(), 16) + 0xff000000;
//            Log.d(LOG_TAG, "" + color);
            _view.setBackgroundColor(color);

        } catch (java.lang.IllegalArgumentException exception) {
            Log.d(LOG_TAG, stringBuilder.toString());
        }
    }
}
