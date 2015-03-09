package com.balsamiq.HelloBallWebview;

import android.app.Activity;
import android.util.Log;
import android.webkit.ValueCallback;
import android.webkit.WebView;

public class JavaModelWrapper {

    WebView _webView;
    Activity _activity;
    IModelObserver _observer;

    public JavaModelWrapper(Activity activity, IModelObserver observer) {
        _observer = observer;
        _activity = activity;
    }

    public void start(float sizeX, float sizeY, WebView wv) {
        _webView = wv;
        String call = "modelStart(" + sizeX + ", " + sizeY + ");";
        _webView.evaluateJavascript(call, null);
    }

}
