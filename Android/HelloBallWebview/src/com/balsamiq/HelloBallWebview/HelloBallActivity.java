package com.balsamiq.HelloBallWebview;

import android.annotation.TargetApi;
import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.SparseArray;
import android.view.ViewTreeObserver;
import android.webkit.WebView;
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

        WebView.setWebContentsDebuggingEnabled(true);

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

                    Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            _model.start(_view.getWidth(), _view.getHeight(), _webView);
                            _view.setModelController(new ModelNativeController(_webView));
                        }
                    }, 500);
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
