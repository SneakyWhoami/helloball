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

	void on_ball_count_changed(size_t count);
	void on_events_per_second(double eps);
	void on_displaylist_changed(NativeValuePtr obj);
	void on_phase_changed(double phase);
};

#endif /* CONTROLLER_H_ */
