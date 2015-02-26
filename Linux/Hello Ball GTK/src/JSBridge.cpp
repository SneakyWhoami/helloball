/*
 * JSBridge.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#include <iostream>
#include <stdlib.h>
#include <glibmm.h>

#include "JSBridge.h"


JSBridge::JSBridge() {
	m_context = NULL;
	m_Array = NULL;
}

JSBridge::~JSBridge() {
	JSGlobalContextRelease(m_context);
}

bool JSBridge::startEngine(int viewWidth, int viewHeight)
{
	// define a custom global object class that accepts private data
	JSClassDefinition def = kJSClassDefinitionEmpty;
	def.className = "Object";
	JSClassRef ref = JSClassCreate(&def);
	m_context = JSGlobalContextCreate(ref);
	JSObjectSetPrivate(JSContextGetGlobalObject(m_context), this);

	// save Array and Function constructors
	JSStringRef arrayString = JSStringCreateWithUTF8CString("Array");
	m_Array = (JSObjectRef)JSObjectGetProperty(m_context, JSContextGetGlobalObject(m_context), arrayString, NULL);
	JSStringRelease(arrayString);
	JSStringRef functionString = JSStringCreateWithUTF8CString("Function");
	m_Function = (JSObjectRef)JSObjectGetProperty(m_context, JSContextGetGlobalObject(m_context), functionString, NULL);
	JSStringRelease(functionString);

	// load and execute Model.js
	std::string script = Glib::file_get_contents("/home/pepo/code/helloball/common/js/model.js");
	JSValueRef v = executeScript(script.c_str());
	std::cout << makeNativeValue(v)->toString() << std::endl;

	JSObjectRef delegate = JSObjectMake(m_context, NULL, NULL);
	JSStringRef delegateName = JSStringCreateWithUTF8CString("delegate");
	JSObjectSetProperty(m_context, JSContextGetGlobalObject(m_context), delegateName, delegate, kJSPropertyAttributeNone, NULL);
	JSStringRelease(delegateName);

	m_ballCountChangedCallback = defineFunctionWithCallback(delegate, "ballCountChanged");
	m_displayListChangedCallback = defineFunctionWithCallback(delegate, "displayListChanged");
	m_epsCallback = defineFunctionWithCallback(delegate, "eventsPerSecond");
	m_phaseChangedCallback = defineFunctionWithCallback(delegate, "phaseChanged");

	std::ostringstream s;
	s << "var controller = initApp(" << viewWidth << ", " << viewHeight << ", delegate);";
	executeScript(s.str().c_str());

	return m_context != NULL;
}

void JSBridge::mouseDown(int x, int y)
{
	std::ostringstream s;
	s << "controller.mouseDown(" << x << ", " << y << ");";
	executeScript(s.str().c_str());
}

void JSBridge::mouseMove(int x, int y)
{
	std::ostringstream s;
	s << "controller.mouseMove(" << x << ", " << y << ");";
	executeScript(s.str().c_str());
}

void JSBridge::mouseUp(int x, int y)
{
	std::ostringstream s;
	s << "controller.mouseUp(" << x << ", " << y << ");";
	executeScript(s.str().c_str());
}

JSValueRef JSBridge::executeScript(const char *script)
{
    JSValueRef exception = NULL;
    JSStringRef str = JSStringCreateWithUTF8CString(script);
    JSValueRef res = JSEvaluateScript(m_context, str, JSContextGetGlobalObject(m_context), NULL, 1, &exception);
    JSStringRelease(str);
    if (exception) {
        std::cout << "Exception caught executing script" << std::endl;
    }
    return exception ? exception : res;
}

std::string JSBridge::makeString(JSValueRef value)
{
	JSStringRef str = JSValueToStringCopy(m_context, value, NULL);
	std::string result = makeString(str);
	JSStringRelease(str);
	return result;
}

std::string JSBridge::makeString(JSStringRef str)
{
	size_t s = JSStringGetMaximumUTF8CStringSize(str);
	char *buf = (char *) malloc(s * sizeof(char));
	JSStringGetUTF8CString(str, buf, s);
	std::string result;
	result.append(buf); // copy buf
	free(buf);
	return result;
}

NativeValuePtr JSBridge::makeNativeValue(JSValueRef value)
{
	NativeValuePtr n(new NativeValue());
	if (JSValueIsUndefined(m_context, value)) {
		n->setUndefinedValue();
	} else if (JSValueIsNull(m_context, value)) {
		n->setNullValue();
	} else if (JSValueIsBoolean(m_context, value)) {
		n->setBoolValue(JSValueToBoolean(m_context, value));
	} else if (JSValueIsNumber(m_context, value)) {
		n->setDoubleValue(JSValueToNumber(m_context, value, NULL));
	} else if (JSValueIsString(m_context, value)) {
		n->setStringValue(makeString(value));
	} else if (JSValueIsObject(m_context, value)) {
		JSObjectRef o = JSValueToObject(m_context, value, NULL);
		JSObjectRef c = getConstructor(o);
		if (c == m_Array) {
			n->setArrayValue();
			size_t l = JSValueToNumber(m_context, getProperty(o, "length"), NULL);
			size_t i;
			for (i = 0; i < l; i++) {
				JSValueRef v = JSObjectGetPropertyAtIndex(m_context, o, i, NULL);
				n->pushArrayItem(makeNativeValue(v));
			}
		} else if (c == m_Function) {
			// do nothing
		} else {
			n->setObjectValue();
			JSPropertyNameArrayRef names = JSObjectCopyPropertyNames(m_context, o);
			size_t count = JSPropertyNameArrayGetCount(names);
			size_t i;
			for (i = 0; i < count; i++) {
				JSStringRef name = JSPropertyNameArrayGetNameAtIndex(names, i);
				JSValueRef v = JSObjectGetProperty(m_context, o, name, NULL);
				n->addObjectEntry(makeString(name), makeNativeValue(v));
			}
			JSPropertyNameArrayRelease(names);
		}
	}
	return n;
}

JSValueRef JSBridge::getProperty(JSObjectRef object, const char *propertyName)
{
	JSStringRef s = JSStringCreateWithUTF8CString(propertyName);
	JSValueRef v = JSObjectGetProperty(m_context, object, s, NULL);
	JSStringRelease(s);
	return v;
}

JSObjectRef JSBridge::getConstructor(JSObjectRef object)
{
	JSValueRef v = getProperty(object, "constructor");
	return (JSObjectRef)v;
}

JSObjectRef JSBridge::defineFunctionWithCallback(JSObjectRef object, const char *name)
{
	JSStringRef functionName = JSStringCreateWithUTF8CString(name);
	JSObjectRef function = JSObjectMakeFunctionWithCallback(m_context, functionName, &JSBridge::staticCallback);
	JSObjectSetProperty(m_context, object, functionName, function, kJSPropertyAttributeNone, NULL);
	JSStringRelease(functionName);
	return function;
}

JSValueRef JSBridge::staticCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
	JSBridge *b = (JSBridge *)JSObjectGetPrivate(JSContextGetGlobalObject(ctx));
	return b->callback(function, thisObject, argumentCount, arguments, exception);
}

JSValueRef JSBridge::callback(JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
	JSValueRef result = JSValueMakeUndefined(m_context);

	if (function == m_ballCountChangedCallback) {
		signal_ball_count_changed.emit(makeNativeValue(arguments[0])->doubleValue());
	} else if (function == m_displayListChangedCallback) {
		signal_displaylist_changed.emit(makeNativeValue(arguments[0]));
	} else if (function == m_epsCallback) {
		signal_events_per_second.emit(makeNativeValue(arguments[0])->doubleValue());
	} else if (function == m_phaseChangedCallback) {
		signal_phase_changed.emit(makeNativeValue(arguments[0])->doubleValue());
	}

	return result;
}
