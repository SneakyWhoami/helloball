package com.balsamiq.HelloBall;

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
    
    public void setColor(int color)
    {
        _color = color;
    }

    public void setPosition(float xv, float yv) {
        _x = xv;
        _y = yv;
    }

    public void setRadius(float radiusv) {
        _radius = radiusv;
    }
    
    public Ball(float x, float y, float radius, int color)
    {
        _x = x;
        _y = y;
        _radius = radius;
        _color = color;
    }

    public Ball(float x, float y, float radius)
    {
        _x = x;
        _y = y;
        _radius = radius;
    }

    public boolean equals(Ball other) {
        return _x == other._x && _y == other._y && _color == other._color && _radius == other._radius;
    }
}
