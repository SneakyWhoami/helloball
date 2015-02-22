package com.balsamiq.HelloBall;

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

        // TODO: it works but cannot debug
//        String model = AssetUtilities.readFromfile(_activity, "model.js");
//        _webView.evaluateJavascript(model, null);
        String call = "vai(" + sizeX + ", " + sizeY + ");";
        _webView.evaluateJavascript(call, null);
    }

}
