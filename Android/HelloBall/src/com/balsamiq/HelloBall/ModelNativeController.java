package com.balsamiq.HelloBall;

import android.webkit.WebView;

/**
 * Created by morsini on 11/02/15.
 */
public class ModelNativeController implements IModelController {


    WebView _webView;
    
    public ModelNativeController(WebView webView)
    {
        _webView = webView;
    }
    
    @Override
    public void mouseDown(double x, double y) {
        _webView.evaluateJavascript("ModelObserver.controller.mouseDown(" + x + "," + y + ")", null);
    }

    @Override
    public void mouseMove(double x, double y) {
        _webView.evaluateJavascript("ModelObserver.controller.mouseMove(" + x + "," + y + ")", null);
    }

    @Override
    public void mouseUp(double x, double y) {
        _webView.evaluateJavascript("ModelObserver.controller.mouseUp(" + x + "," + y + ")", null);
    }
}
