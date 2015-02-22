package com.balsamiq.HelloBall;

import android.app.Activity;
import org.mozilla.javascript.*;

import java.lang.reflect.InvocationTargetException;

public class JavaModelWrapper {

    Context cx;
    Scriptable scope;
    Activity _activity;
    IModelObserver _observer;
    
    public JavaModelWrapper(Activity activity, IModelObserver observer) {
        _observer = observer;
        _activity = activity;
    }
    
    public IModelController start(float sizeX, float sizeY)
    {
        cx = Context.enter();
        cx.setOptimizationLevel(-1);
        scope = cx.initStandardObjects();

        String model = AssetUtilities.readFromfile(_activity, "model.js");

        cx.evaluateString(scope, model, "<cmd>", 1, null);

        Object fObj = scope.get("initApp", scope);
        Object result;

        if (!(fObj instanceof Function)) {
            throw new Error("initApp is undefined or not a function.");
        } else {

            try {
                ScriptableObject.defineClass(scope, ModelNativeDelegate.class);
                Object [] params = { cx.javaToJS(_observer, scope) };
                Scriptable myModelObserver = cx.newObject(scope, "ModelObserver", params);
                Object functionArgs[] = {"" + sizeX, "" + sizeY, myModelObserver};
                Function f = (Function) fObj;
                // result will be an object with methods mouseUp, mouseMove, mouseDown
                result = f.call(cx, scope, scope, functionArgs);
                return new ModelNativeController((NativeObject)result, cx, scope);
            } catch (InvocationTargetException ite) {
                ite.printStackTrace();
            } catch (InstantiationException ie) {
                ie.printStackTrace();
            } catch (IllegalAccessException iae) {
                iae.printStackTrace();
            }
        }
        
        return null;
    }
    
    public void finish() {
        Context.exit();
    }
}
