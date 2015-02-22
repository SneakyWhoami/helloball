package com.balsamiq.HelloBallRhino;

import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.NativeObject;
import org.mozilla.javascript.Scriptable;

/**
 * Created by morsini on 11/02/15.
 */
public class ModelNativeController implements IModelController {

    Function _mouseDown; 
    Function _mouseMove;
    Function _mouseUp;
    Context _context;
    Scriptable _scope;
    NativeObject _this;
            
    public ModelNativeController(NativeObject nativeObject, Context context, Scriptable scope)
    {
        _context = context;
        _this = nativeObject;
        _scope = scope;
        _mouseDown = (Function) NativeObject.getProperty(nativeObject, "mouseDown");
        _mouseMove = (Function) NativeObject.getProperty(nativeObject, "mouseMove");
        _mouseUp = (Function) NativeObject.getProperty(nativeObject, "mouseUp");
    }

    @Override
    public void mouseDown(double x, double y) {
        Object functionArgs[] = { x, y };
        _mouseDown.call(_context, _scope, _this, functionArgs);
    }

    @Override
    public void mouseMove(double x, double y) {
        Object functionArgs[] = { x, y };
        _mouseMove.call(_context, _scope, _this, functionArgs);
    }

    @Override
    public void mouseUp(double x, double y) {
        Object functionArgs[] = { x, y };
        _mouseUp.call(_context, _scope, _this, functionArgs);
    }
}
