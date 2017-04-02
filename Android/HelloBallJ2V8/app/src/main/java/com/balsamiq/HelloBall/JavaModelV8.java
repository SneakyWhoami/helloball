package com.balsamiq.HelloBall;

import android.content.Context;
import com.balsamiq.EventBusObserver.ModelObserver;
import com.eclipsesource.v8.V8;
import com.eclipsesource.v8.V8Array;
import com.eclipsesource.v8.V8Object;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.HashMap;
import java.util.Iterator;

public class JavaModelV8 implements IModel {

    protected IModelObserver _observer;
    protected V8 runtime;

    public JavaModelV8(IModelObserver observer) {
        _observer = observer;
        runtime = V8.createV8Runtime();
    }
    
    public void load(String javascript, String jsName)
    {
        runtime.executeVoidScript(javascript);
    }

    public void triggerChangePhase()
    {
        V8Object modelObserver = runtime.getObject("ModelObserver");
        V8Object appController = modelObserver.getObject("appController");

        if (!appController.isUndefined()) {
            V8Array args = new V8Array(runtime);
            appController.executeFunction("task", args);
        }
    }
    
    public void start(float sizeX, float sizeY) {

        V8Object v8Console = new V8Object(runtime);
        runtime.add("ModelObserver", v8Console);
        v8Console.registerJavaMethod(this, "onBallCountChanged", "ballCountChanged", new Class[] { Integer.class });
        v8Console.registerJavaMethod(this, "onEventsPerSecond", "eventsPerSecond", new Class[] { Double.class });
        v8Console.registerJavaMethod(this, "onLog", "log", new Class[] { String.class });
        v8Console.registerJavaMethod(this, "onPhaseChanged", "phaseChanged", new Class[] { Number.class });
//        v8Console.registerJavaMethod(this, "onPhaseChangedDouble", "phaseChanged", new Class[] { Double.class });
//        v8Console.registerJavaMethod(this, "onPhaseChangedInteger", "phaseChanged", new Class[] { Integer.class });
        v8Console.registerJavaMethod(this, "onDisplayListChanged", "displayListChanged", new Class[] { String.class });
        v8Console.registerJavaMethod(this, "onTextSize", "measureText", new Class[] { String.class,
            Integer.class, Boolean.class, Boolean.class, String.class });
        v8Console.release();
        V8Array args = new V8Array(runtime).push(sizeX).push(sizeY);
        runtime.executeFunction("modelStart", args);
    }

    public void stop() {
//        stopModel();
    }

    public void mouseDown(double x, double y) {
        V8Object modelObserver = runtime.getObject("ModelObserver");
        V8Object appController = modelObserver.getObject("appController");

        if (!appController.isUndefined()) {
            V8Array args = new V8Array(runtime);
            args.push(x);
            args.push(y);
            appController.executeFunction("mouseDown", args);
        }
    }

    public void mouseUp(double x, double y) {
        V8Object modelObserver = runtime.getObject("ModelObserver");
        V8Object appController = modelObserver.getObject("appController");

        if (!appController.isUndefined()) {
            V8Array args = new V8Array(runtime);
            args.push(x);
            args.push(y);
            appController.executeFunction("mouseUp", args);
        }
    }

    public void mouseMove(double x, double y) {
        V8Object modelObserver = runtime.getObject("ModelObserver");
        V8Object appController = modelObserver.getObject("appController");

        if (!appController.isUndefined()) {
            V8Array args = new V8Array(runtime);
            args.push(x);
            args.push(y);
            appController.executeFunction("mouseMove", args);
        }
    }

    public void onBallCountChanged(Integer number) {
        _observer.log("ballCountChanged: " + number);
    }

    public void onDisplayListChanged(String json) {

        try {
            HashMap<Integer, Ball> balls = new HashMap<Integer, Ball>();
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

    public void onEventsPerSecond(Double events) {

        _observer.eventsPerSecond(events.intValue());
    }

    public void onLog(String message) {
        _observer.log(message);
    }

    public void onPhaseChanged(Number phaseValue)
    {
        _observer.phaseChanged(phaseValue.doubleValue());
    }

    public void onPhaseChangedInteger(Integer phaseValue)
    {
        _observer.phaseChanged(phaseValue);
    }

    public void onPhaseChangedDouble(Double phaseValue)
    {
        _observer.phaseChanged(phaseValue);
    }

    public float onTextSize(String font, Integer fontSize, Boolean bold, Boolean italic, String text) {
        return _observer.measureText(font, fontSize, bold, italic, text);
    }
}
