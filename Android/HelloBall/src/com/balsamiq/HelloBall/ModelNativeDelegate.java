package com.balsamiq.HelloBall;

import android.util.Log;
import android.util.SparseArray;
import android.webkit.JavascriptInterface;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Iterator;

/**
 * Created by morsini on 10/02/15.
 */
public class ModelNativeDelegate {
    protected IModelObserver _observer;

    // The zero-argument constructor used by Rhino runtime to create instances
    public ModelNativeDelegate(IModelObserver observer) {
        _observer = observer;
    }

    @JavascriptInterface
    public void ballCountChanged(int number) {
        Log.d("ModelObserver", "ballCountChanged: " + number);
    }

    @JavascriptInterface
    public void displayListChanged(String json) {

        try {
            SparseArray<Ball> balls = new SparseArray<Ball>();
            JSONObject jsonObject = new JSONObject(json);
            Iterator<?> iterator = jsonObject.keys();
            while (iterator.hasNext()) {
                String key = (String) iterator.next();
                JSONObject ballDL = (JSONObject) jsonObject.get(key);
                double x = toDouble(ballDL.get("x"));
                double y = toDouble(ballDL.get("y"));
                double radius = toDouble(ballDL.get("radius"));
                int color = (Integer) ballDL.get("color");
                Ball ball = new Ball((float) x, (float) y, (float) radius, color);
                balls.put(Integer.parseInt(key), ball);

            }

            _observer.displayListChanged(balls);
        } catch (JSONException exception) {
            //silent
        }
    }

    protected double toDouble(Object x) {
        if (x instanceof Double) {
            return (Double) x;
        } else if (x instanceof Integer) {
            return (int) (Integer) x;
        }

        throw new Error("not a double value: " + x.toString());
    }

    @JavascriptInterface
    public void eventsPerSecond(int events) {

        _observer.eventsPerSecond(events);
    }

    @JavascriptInterface
    public void log(String message) {

        Log.d("ModelObserver", message);
    }
}
