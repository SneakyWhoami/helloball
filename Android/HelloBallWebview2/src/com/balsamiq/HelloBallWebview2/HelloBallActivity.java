package com.balsamiq.HelloBallWebview2;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebView;
import android.widget.CheckBox;
import android.widget.TextView;

public class HelloBallActivity extends Activity {

    WebView _webView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        _webView = (WebView) findViewById(R.id.webView);
        _webView.getSettings().setJavaScriptEnabled(true);
        _webView.setVerticalScrollBarEnabled(false);
        _webView.setHorizontalScrollBarEnabled(false);
        _webView.loadUrl("file:///android_asset/helloball.html");
        WebView.setWebContentsDebuggingEnabled(true);
    }
}
