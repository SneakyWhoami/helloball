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
#include "EventQueue.h"


class JSBridge : public EventTarget
{
public:
	JSBridge(EventQueue *q);
	virtual ~JSBridge();

	bool startEngine(int viewWidth, int viewHeight);

	virtual void handleEvent(Event *e);

	JSValueRef executeScript(const char *script);

	std::string makeString(JSValueRef str);
	NativeValuePtr makeNativeValue(JSValueRef value);

	sigc::signal<void, size_t> signal_ball_count_changed;
	sigc::signal<void, double> signal_events_per_second;
	sigc::signal<void, double> signal_phase_changed;
	sigc::signal<void, NativeValuePtr> signal_displaylist_changed;

protected:
	EventQueue *m_queue;

	void mouseDown(int x, int y);
	void mouseMove(int x, int y);
	void mouseUp(int x, int y);

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
};

#endif /* JSBRIDGE_H_ */
