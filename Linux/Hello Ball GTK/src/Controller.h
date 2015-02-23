/*
 * Controller.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "JSBridge.h"
#include "HelloBallWindow.h"

class Controller {
public:
	Controller(HelloBallWindow *window);
	virtual ~Controller();

	bool init();

protected:
	JSBridge *m_bridge;
	HelloBallWindow *m_window;

	void on_execute_button_clicked();
};

#endif /* CONTROLLER_H_ */
