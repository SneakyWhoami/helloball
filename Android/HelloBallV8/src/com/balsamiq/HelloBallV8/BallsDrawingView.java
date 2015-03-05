package com.balsamiq.HelloBallV8;

import android.content.Context;
import android.graphics.*;
import android.util.AttributeSet;
import android.util.Log;
import android.util.SparseArray;
import android.view.MotionEvent;
import android.view.View;
import com.balsamiq.HelloBall.Ball;
import com.balsamiq.HelloBall.IModel;

import java.util.Date;
import java.util.HashMap;

public class BallsDrawingView extends View  {
    private static final String TAG = "BoxDrawingView";
    Ball[] _balls;
    private Paint mBackgroundPaint;
    IModel _controller;

    // used when creating the view in code
    public BallsDrawingView(Context context) {
        this(context, null);
    }

    // used when inflating the view from XML
    public BallsDrawingView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mBackgroundPaint = new Paint();
        mBackgroundPaint.setColor(0xffffefe0);
    }

    public void displayListChanged(HashMap<Integer, Ball> balls)
    {
        if (_balls == null)
        {
            _balls = new Ball[balls.size()];
        }
        
        for (int i : balls.keySet()) {
            _balls[i] = balls.get(i);
        }

//        Log.d("BallsDrawingView", "" + new Date().getTime());
        invalidate();
    }
    
    @Override
    public void setBackgroundColor(int color)
    {
        mBackgroundPaint.setColor(color);
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        // fill the background
//        Log.d("BallsDrawingView.onDraw", "" + new Date().getTime());
        SparseArray<Paint> paintSparseArray = new SparseArray<Paint>();
        canvas.drawPaint(mBackgroundPaint);
        if (_balls != null) {
            for (Ball ball : _balls) {
                Paint p = paintSparseArray.get(ball.getColor());
                if (p == null) {
                    p = new Paint();
                    p.setColor(0xff000000 + Integer.parseInt(Integer.toHexString(ball.getColor()), 16));
                    paintSparseArray.put(ball.getColor(), p);
                }
                
                canvas.drawCircle(ball.getX(), ball.getY(), ball.getRadius(), p);
            }
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        PointF curr = new PointF(event.getX(), event.getY());
        if (_controller == null) {
            return true;
        }
        
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                // reset our drawing state
                _controller.mouseDown(curr.x, curr.y);
                break;

            case MotionEvent.ACTION_MOVE:
                _controller.mouseMove(curr.x, curr.y);
                break;

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_CANCEL:
                _controller.mouseUp(curr.x, curr.y);
                break;
        }

        return true;
    }

    public void setModel(IModel model)
    {
        _controller = model;
    }
}

