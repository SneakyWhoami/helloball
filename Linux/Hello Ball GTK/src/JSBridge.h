/*
 * JSBridge.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#ifndef JSBRIDGE_H_
#define JSBRIDGE_H_

#include <JavaScriptCore/JavaScript.h>
#include <glibmm/ustring.h>

class JSBridge {
public:
	JSBridge();
	virtual ~JSBridge();

	bool startEngine(int viewWidth, int viewHeight);

	JSValueRef executeScript(const char *script);

	Glib::ustring makeString(JSValueRef str);

protected:
	JSGlobalContextRef m_context;
};

#endif /* JSBRIDGE_H_ */
