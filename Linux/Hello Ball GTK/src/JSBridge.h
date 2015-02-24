/*
 * JSBridge.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#ifndef JSBRIDGE_H_
#define JSBRIDGE_H_

#include <JavaScriptCore/JavaScript.h>

#include "NativeValue.h"


class JSBridge {
public:
	JSBridge();
	virtual ~JSBridge();

	bool startEngine(int viewWidth, int viewHeight);

	JSValueRef executeScript(const char *script);

	std::string makeString(JSValueRef str);
	NativeValuePtr makeNativeValue(JSValueRef value);

protected:
	JSGlobalContextRef m_context;
	JSObjectRef m_Array;

	std::string makeString(JSStringRef str);

	JSValueRef getProperty(JSObjectRef object, const char *propertyName);
	JSObjectRef getConstructor(JSObjectRef object);
};

#endif /* JSBRIDGE_H_ */

