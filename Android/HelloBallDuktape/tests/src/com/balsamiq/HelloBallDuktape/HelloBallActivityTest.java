package com.balsamiq.HelloBallDuktape;

import android.test.ActivityInstrumentationTestCase2;
import android.util.Log;

/**
 * This is a simple framework for a test of an Application.  See
 * {@link android.test.ApplicationTestCase ApplicationTestCase} for more information on
 * how to write and extend Application tests.
 * <p/>
 * To run this test, you can type:
 * adb shell am instrument -w \
 * -e class com.balsamiq.HelloBallDuktape.HelloBallActivityTest \
 * com.balsamiq.HelloBallDuktape.tests/android.test.InstrumentationTestRunner
 */
public class HelloBallActivityTest extends ActivityInstrumentationTestCase2<HelloBallActivity> {

    MockModelObserver _observer;
    JavaModelWrapper _wrapper;


    public HelloBallActivityTest() {
        super(HelloBallActivity.class);
    }

    @Override
    protected void setUp() {
        _observer = new MockModelObserver();
        _wrapper = new JavaModelWrapper(AssetUtilities.readFromfile(this.getActivity(), "model.js"), _observer);
        _wrapper.start(1000, 1000);
    }

    @Override
    protected void tearDown() {


    }

    public void testPerformances() {
        Ball ball = _observer.getBallToMove();
        double x = ball.getX();
        double y = ball.getY();
        _wrapper.mouseDown(x, y);
        for (int i = 0; i <  1000; ++i) {
            int add = i % 2 != 0 ? +10 : -10;
            _wrapper.mouseMove(x + add, y + add); 
        }
        _wrapper.mouseUp(x, y);
        Log.d("HelloBallActivityTest, events per second:", "" + _observer.getEventsPerSecond());
    }
}
