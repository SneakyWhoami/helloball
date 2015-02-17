package com.balsamiq.HelloBall;

import android.app.Activity;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.View;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.TextView;

public class HelloBallActivity extends Activity implements IModelObserver {

    BallsDrawingView _view;
    TextView _fps;
    WebView _webView;
    JavaModelWrapper _model;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        _view = (BallsDrawingView) findViewById(R.id.drawing_view);
        _fps = (TextView) findViewById(R.id.fps);
        _model = new JavaModelWrapper(this, this);
        _webView = new WebView(this);
        _webView.getSettings().setJavaScriptEnabled(true);
        _webView.addJavascriptInterface(new ModelNativeDelegate(this), "ModelObserver");
        _webView.loadUrl("file:///android_asset/main.html");

        Button start = (Button) findViewById(R.id.start);
        WebView.setWebContentsDebuggingEnabled(true);
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                _model.start(_view.getWidth(), _view.getHeight(), _webView);
                _view.setModelController(new ModelNativeController(_webView));
            }
        });
    }

    @Override
    public void ballCountChanged(int number) {

    }

    @Override
    public void eventsPerSecond(final int events) {
        _fps.post(new Runnable() {
            @Override
            public void run() {
                _fps.setText("" + events);
            }
        });
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
