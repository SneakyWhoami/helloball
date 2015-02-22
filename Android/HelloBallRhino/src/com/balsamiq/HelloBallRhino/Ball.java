package com.balsamiq.HelloBallRhino;

/**
 * Created by morsini on 11/02/15.
 */
public class Ball {
    float _x;
    float _y;
    float _radius;
    int _color;

    public float getX() {
        return _x;
    }

    public float getY() {
        return _y;
    }

    public float getRadius() {
        return _radius;
    }

    public int getColor() {
        return _color;
    }

    public Ball(float x, float y, float radius, int color)
    {
        _x = x;
        _y = y;
        _radius = radius;
        _color = color;
    }
}
