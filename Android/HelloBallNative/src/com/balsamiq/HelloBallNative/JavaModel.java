package com.balsamiq.HelloBallNative;

import android.app.Activity;
import android.util.Log;
import android.util.SparseArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;

/**
 * Created by morsini on 19/02/15.
 */
public class JavaModel {

    protected IModelObserver _observer;
    protected Controller _controller;
    protected final int BALL_COUNT = 100;
    protected final boolean PENETRATION_TEST_ENABLED = true;

    public JavaModel(Activity activity, IModelObserver observer) {
        _observer = observer;
    }

    public void start(float sizeX, float sizeY) {
        _controller = initApp(sizeX, sizeY, _observer);
    }

    public void stop() {
        
    }

    public void mouseDown(float x, float y) {
        _controller.mouseDown(x, y);
    }

    public void mouseUp(float x, float y) {
        _controller.mouseUp(x, y);
    }

    public void mouseMove(float x, float y) {
        _controller.mouseMove(x, y);
    }

    public void onBallCountChanged(int number) {
        Log.d("ModelObserver", "ballCountChanged: " + number);
    }

    public void onDisplayListChanged(String json) {

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

    public void onEventsPerSecond(int events) {

        _observer.eventsPerSecond(events);
    }

    public void onLog(String message) {

        Log.d("ModelObserver", message);
    }

    class Model {

        public ArrayList<Ball> balls = new ArrayList<Ball>();

        public int size() {
            return balls.size();
        }

        public void addBall(Ball b) {
            balls.add(b);
        }

        public void setBallPosition(int ballIndex, float x, float y) {
            balls.get(ballIndex).setPosition(x, y);
        }

        public void setBallRadius(int ballIndex, int radius) {
            balls.get(ballIndex).setRadius(radius);
        }
    }

    public class EventsCounter {
        int eventsCount = 0;
        long lastTime = 0;
        IModelObserver delegate;

        public EventsCounter(IModelObserver delegatev) {
            eventsCount = 0;
            lastTime = 0;
            delegate = delegatev;
        }

        public void countEvent() {
            eventsCount += 1;
            long diffTime = new Date().getTime() - lastTime;
            if (diffTime > 200) {
                if (delegate != null) {
                    int eps = (int) Math.floor(eventsCount * 1000 / diffTime);
                    delegate.eventsPerSecond(eps);
                }
                this.lastTime = new Date().getTime();
                this.eventsCount = 0;
            }
        }
    }

    public class DisplayList {

        Ball[] items;
        boolean[] itemsChanged;

        public DisplayList(int ballCount) {
            items = new Ball[ballCount];
            itemsChanged = new boolean[ballCount];
            int i = 0;
            for (i = 0; i < ballCount; i++) {
                items[i] = new Ball(-1, -1, -1, -1);
                itemsChanged[i] = false;
            }
        }

        public void setBall(int ballIndex, Ball newObj) {

            if (!newObj.equals(items[ballIndex])) {
                items[ballIndex] = newObj;
                itemsChanged[ballIndex] = true;
            }
        }

        public SparseArray<Ball> getChangedItems() {
            SparseArray<Ball> result = new SparseArray<Ball>();
            for (int ballIndex = 0; ballIndex < itemsChanged.length; ++ballIndex) {
                if (itemsChanged[ballIndex]) {
                    result.put(ballIndex, items[ballIndex]);
                    itemsChanged[ballIndex] = false;
                }
            }

            return result;
        }
    }

    class Controller {

        IModelObserver delegate;
        Model model;
        int selectedBallIndex = -1;
        boolean mouseIsDown = false;
        EventsCounter eps;
        DisplayList displayList;

        public Controller(Model modelv, IModelObserver delegatev) {
            delegate = delegatev;
            this.model = modelv;
            this.selectedBallIndex = -1;
            this.mouseIsDown = false;
            this.eps = new EventsCounter(delegate);
            this.displayList = new DisplayList(model.balls.size());
        }

        public void mouseDown(double x, double y) {
            ArrayList<Integer> ballsHit = hitTest(model.balls, x, y);
            if (ballsHit.size() > 0) {
                selectedBallIndex = ballsHit.get(ballsHit.size() - 1);
            } else {
                this.selectedBallIndex = -1;
            }
            this.mouseIsDown = true;
            this.makeDisplayList();
            this.eps.countEvent();
        }

        public void mouseMove(float x, float y) {

            if (this.mouseIsDown && selectedBallIndex != -1) {
                this.model.setBallPosition(selectedBallIndex, x, y);
                this.makeDisplayList();
            }
            this.eps.countEvent();
        }

        public void mouseUp(double x, double y) {
            this.mouseIsDown = false;
            if (this.selectedBallIndex != -1) {
                this.makeDisplayList();
            }
            this.eps.countEvent();
        }

        public ArrayList<Integer> hitTest(ArrayList<Ball> balls, double x, double y) {
            ArrayList<Integer> ballsHit = new ArrayList<Integer>();
            int i;
            for (i = 0; i < balls.size(); i++) {
                if (Math.pow((x - balls.get(i).getX()), 2) + Math.pow((y - balls.get(i).getY()), 2) < Math.pow(balls.get(i).getRadius(), 2)) {
                    ballsHit.add(i);
                }
            }
            return ballsHit;
        }

        public ArrayList<ArrayList<Integer>> penetrationTest(ArrayList<Ball> balls) {
            ArrayList<ArrayList<Integer>> results = new ArrayList<ArrayList<Integer>>();
            int i, j;
            for (i = 0; i < balls.size(); i++) {
                results.add(new ArrayList<Integer>());
            }

            for (i = 0; i < balls.size() - 1; i++) {
                for (j = i + 1; j < balls.size(); j++) {
                    if (Math.pow((balls.get(i).getX() - balls.get(j).getX()), 2) + Math.pow((balls.get(i).getY() - balls.get(j).getY()), 2) < Math.pow(balls.get(i).getRadius()
                            + balls.get(j).getRadius(), 2)) {
                        results.get(i).add(j);
                        results.get(j).add(i);
                    }
                }
            }
            return results;
        }

        public void makeDisplayList() {
            ArrayList<ArrayList<Integer>> penetrationResults = PENETRATION_TEST_ENABLED ? penetrationTest(this.model.balls) : null;

            int i;
            int color;
            int numHits;
            for (i = 0; i < this.model.balls.size(); i++) {
                if (this.selectedBallIndex == i) {
                    if (this.mouseIsDown) {
                        color = 0x44FF44;
                    } else {
                        color = 0x00BB00;
                    }
                } else {
                    numHits = PENETRATION_TEST_ENABLED ? penetrationResults.get(i).size() : 0;
                    if (numHits == 0) {
                        color = 0x0000FF;
                    } else if (numHits == 1) {
                        color = 0xFF0000;
                    } else if (numHits == 2) {
                        color = 0xFF6666;
                    } else {
                        color = 0xFFBBBB;
                    }
                }
                
                this.displayList.setBall(i, new Ball(model.balls.get(i).getX(), model.balls.get(i).getY(), this.model.balls.get(i).getRadius(), color));
            }

            if (this.delegate != null) {
                this.delegate.displayListChanged(this.displayList.getChangedItems());
            }
        }
    }

    float getRandInt(float min, float max) {
        return (float)Math.random() * (max - min) + min;
    }

    protected void populateModel(Model model, float viewWidth, float viewHeight)
    {
        int ballMinRadius = 18;
        int ballMaxRadius = 44;
        int i;
        Ball ball;

        for (i = 0; i < BALL_COUNT; i += 1) {
            int radius = (int)getRandInt(ballMinRadius, ballMaxRadius);
            ball = new Ball(getRandInt(radius, (viewWidth - radius)),
                    getRandInt(radius, (viewHeight - radius)), radius);
            model.addBall(ball);
        }
    }

    protected Controller initApp(float viewWidth, float viewHeight, IModelObserver delegate)
    {
        Model model = new Model();
        populateModel(model, viewWidth, viewHeight);
        delegate.ballCountChanged(model.balls.size());
        Controller controller = new Controller(model, delegate);
        controller.makeDisplayList();
        return controller;
    }
}
