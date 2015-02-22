package com.balsamiq.HelloBallRhino;

import android.util.Log;
import android.util.SparseArray;
import org.mozilla.javascript.Context;
import org.mozilla.javascript.NativeObject;
import org.mozilla.javascript.Scriptable;
import org.mozilla.javascript.ScriptableObject;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by morsini on 10/02/15.
 */
public class ModelNativeDelegate extends ScriptableObject {
    private static final long serialVersionUID = 438270592527335642L;
    protected IModelObserver _observer;

    // The zero-argument constructor used by Rhino runtime to create instances
    public ModelNativeDelegate() {
    }

    // Method jsConstructor defines the JavaScript constructor
    public void jsConstructor(Object observer) {
        _observer = (IModelObserver)Context.jsToJava(observer, IModelObserver.class);;
    }

    // The class name is defined by the getClassName method
    @Override
    public String getClassName() {
        return "ModelObserver";
    }

    public void jsFunction_ballCountChanged(int number) {
        Log.d("ModelObserver", "ballCountChanged: " + number);
    }

    public void jsFunction_displayListChanged(Scriptable scriptable) {
        Object[] propIds = scriptable.getIds();
        SparseArray<Ball> balls = new SparseArray<Ball>();
        for (int i = 0; i < propIds.length; i++) {
            Object propId = propIds[i];
            if (propId instanceof Integer) {
                Integer key = (Integer) propId;
                NativeObject value = (NativeObject) scriptable.get(key, scriptable);
                double x = (Double) NativeObject.getProperty(value, "x");
                double y = (Double) NativeObject.getProperty(value, "y");
                double radius = (Double) NativeObject.getProperty(value, "radius");
                int color = (int)Math.round((Double)NativeObject.getProperty(value, "color"));
                Ball ball = new Ball((float)x, (float) y, (float) radius, color);
                balls.put(key, ball);
            }
        }
        _observer.displayListChanged(balls);
    }

    public void jsFunction_eventsPerSecond(int events) {
        
        _observer.eventsPerSecond(events);
    }

    public void jsFunction_log(String message) {
        
        Log.d("ModelObserver", message);
    }
}
