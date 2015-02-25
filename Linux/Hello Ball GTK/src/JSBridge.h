/*
 * JSBridge.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#ifndef JSBRIDGE_H_
#define JSBRIDGE_H_

#include <JavaScriptCore/JavaScript.h>
#include <sigc++/sigc++.h>

#include "NativeValue.h"


typedef sigc::signal<void, size_t> BallCountChangedSignal;


class JSBridge {
public:
	JSBridge();
	virtual ~JSBridge();

	bool startEngine(int viewWidth, int viewHeight);

	JSValueRef executeScript(const char *script);

	std::string makeString(JSValueRef str);
	NativeValuePtr makeNativeValue(JSValueRef value);

	BallCountChangedSignal signal_ball_count_changed() { return m_ballCountChangedSignal; }

protected:
	JSGlobalContextRef m_context = NULL;
	JSObjectRef m_Array = NULL;
	JSObjectRef m_Function = NULL;

	std::string makeString(JSStringRef str);

	JSValueRef getProperty(JSObjectRef object, const char *propertyName);
	JSObjectRef getConstructor(JSObjectRef object);

	JSObjectRef defineFunctionWithCallback(JSObjectRef object, const char *name);
	JSObjectRef m_ballCountChangedCallback = NULL;
	JSObjectRef m_displayListChangedCallback = NULL;
	JSObjectRef m_epsCallback = NULL;
	JSObjectRef m_phaseChangedCallback = NULL;

	static JSValueRef staticCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
	JSValueRef callback(JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

	BallCountChangedSignal m_ballCountChangedSignal;
};

#endif /* JSBRIDGE_H_ */
