package com.balsamiq.HelloBallRhino;

import android.content.Context;
import android.graphics.*;
import android.util.AttributeSet;
import android.util.SparseArray;
import android.view.MotionEvent;
import android.view.View;

public class BallsDrawingView extends View  {
    private static final String TAG = "BoxDrawingView";
    Ball [] _balls;
    private Paint mBackgroundPaint;
    IModelController _controller;
    
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

    public void setModelController(IModelController controller)
    {
        _controller = controller;
    }
}

