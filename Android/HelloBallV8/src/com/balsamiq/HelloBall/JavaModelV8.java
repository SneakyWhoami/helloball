package com.balsamiq.HelloBall;

import org.json.JSONException;
import org.json.JSONObject;
import java.util.HashMap;
import java.util.Iterator;

public class JavaModelV8 implements IModel {

    private native void loadJS(String js);

    private native void startModel(double sizeX, double sizeY);

    private native void initialize();

    private native void stopModel();

    private native void onMouseDown(double x, double y);

    private native void onMouseUp(double x, double y);

    private native void onMouseMove(double x, double y);

    protected IModelObserver _observer;

    static {
        System.loadLibrary("modelV8");
    }

    public JavaModelV8(String javascript, IModelObserver observer) {
        _observer = observer;
        initialize();
        loadJS(javascript);
    }

    public void start(float sizeX, float sizeY) {
        startModel(sizeX, sizeY);
    }

    public void stop() {
        stopModel();
    }

    public void mouseDown(double x, double y) {
        onMouseDown(x, y);
    }

    public void mouseUp(double x, double y) {
        onMouseUp(x, y);
    }

    public void mouseMove(double x, double y) {
        onMouseMove(x, y);
    }

    public void onBallCountChanged(int number) {
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

    public void onEventsPerSecond(int events) {

        _observer.eventsPerSecond(events);
    }

    public void onLog(String message) {
        _observer.log(message);
    }

}
