package com.balsamiq.HelloBall;

import android.app.Activity;
import org.mozilla.javascript.Context;
import org.mozilla.javascript.Function;
import org.mozilla.javascript.Scriptable;
import org.mozilla.javascript.ScriptableObject;

import java.lang.reflect.InvocationTargetException;

public class Model {

    Context cx;
    Scriptable scope;

    public Model(Activity activity, IModelObserver observer) {
        cx = Context.enter();
        cx.setOptimizationLevel(-1);
        scope = cx.initStandardObjects();

        String model = AssetUtilities.readFromfile(activity, "model.js");

        cx.evaluateString(scope, model, "<cmd>", 1, null);

        Object fObj = scope.get("initApp", scope);
        Object result;

        if (!(fObj instanceof Function)) {
            throw new Error("initApp is undefined or not a function.");
        } else {

            try {
                ScriptableObject.defineClass(scope, ModelNativeDelegate.class);
                Object [] params = { cx.javaToJS(observer, scope) };
                Scriptable myModelObserver = cx.newObject(scope, "ModelObserver", params);
                Object functionArgs[] = {"500", "500", myModelObserver};
                Function f = (Function) fObj;
                // result will be an object with methods mouseUp, mouseMove, mouseDown
                result = f.call(cx, scope, scope, functionArgs);
            } catch (InvocationTargetException ite) {
                ite.printStackTrace();
            } catch (InstantiationException ie) {
                ie.printStackTrace();
            } catch (IllegalAccessException iae) {
                iae.printStackTrace();

            }
        }
    }

    public void finish() {
        Context.exit();
    }
}
