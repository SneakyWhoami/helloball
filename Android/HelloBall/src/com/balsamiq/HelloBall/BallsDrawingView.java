package com.balsamiq.HelloBall;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.provider.CalendarContract;
import android.util.AttributeSet;
import android.util.SparseArray;
import android.view.View;

public class BallsDrawingView extends View {
    private static final String TAG = "BoxDrawingView";
    Ball [] _balls;
    private Paint mBackgroundPaint;
    // used when creating the view in code
    public BallsDrawingView(Context context) {
        this(context, null);
    }

    // used when inflating the view from XML
    public BallsDrawingView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mBackgroundPaint = new Paint();
        mBackgroundPaint.setColor(0xfff8efe0);
    }

    public void displayListChanged(SparseArray<Ball> balls)
    {
        if (_balls == null)
        {
            _balls = new Ball[balls.size()];
        }
        
        for (int i = 0; i < balls.size(); ++i) {
            _balls[balls.keyAt(i)] = balls.get(balls.keyAt(i));
        }

        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        // fill the background
        canvas.drawPaint(mBackgroundPaint);

        for (Ball ball : _balls) {
            Paint p = new Paint();
            p.setColor(0xff000000 + Integer.parseInt(Integer.toHexString(ball.getColor()), 16));
            canvas.drawCircle(ball.getX(), ball.getY(), ball.getRadius(), p);
        }
    }
}

