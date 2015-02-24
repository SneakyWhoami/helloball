package com.balsamiq.HelloBall;

/**
 * Created by morsini on 24/02/15.
 */
public interface IModel {
    void start(float sizeX, float sizeY);
    void stop();
    public void mouseDown(double x, double y);
    public void mouseUp(double x, double y);
    public void mouseMove(double x, double y);
}
