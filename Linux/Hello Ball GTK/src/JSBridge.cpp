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
}

JSBridge::~JSBridge() {
	JSGlobalContextRelease(m_context);
}

bool JSBridge::startEngine(int viewWidth, int viewHeight)
{
	m_context = JSGlobalContextCreate(NULL);
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

Glib::ustring JSBridge::makeString(JSValueRef value)
{
	JSStringRef str = JSValueToStringCopy(m_context, value, NULL);
	size_t s = JSStringGetMaximumUTF8CStringSize(str);
	char *buf = (char *) malloc(s * sizeof(char));
	JSStringGetUTF8CString(str, buf, s);
	Glib::ustring result;
	result.append(buf); // copy buf
	free(buf);
	return result;
}
