/*
 * JSBridge.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#include <iostream>
#include <stdlib.h>

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
	m_context = JSGlobalContextCreate(NULL);
	if (m_context) {
		JSStringRef name = JSStringCreateWithUTF8CString("Array");
		m_Array = (JSObjectRef)JSObjectGetProperty(m_context, JSContextGetGlobalObject(m_context), name, NULL);
		JSStringRelease(name);
	}
	return m_context != NULL;
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
