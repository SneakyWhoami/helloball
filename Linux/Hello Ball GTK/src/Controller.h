/*
 * Controller.h
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "AsyncQueue.h"
#include "JSBridge.h"
#include "HelloBallWindow.h"
#include "CairoFontHelper.h"


class Controller {
public:
	Controller(HelloBallWindow *window);
	virtual ~Controller();

	bool init();

protected:
	CairoFontHelper *m_fontHelper;
	bool initFonts();

	AsyncQueue *m_queue;
	std::shared_ptr<JSBridge> m_bridge;
	HelloBallWindow *m_window;

	void on_execute_button_clicked();
	bool on_balls_mouse_down(GdkEventButton* event);
	bool on_balls_mouse_move(GdkEventMotion* event);
	bool on_balls_mouse_up(GdkEventButton* event);

	void on_ball_count_changed(size_t count);
	void on_events_per_second(double eps);
	void on_displaylist_changed(NativeValuePtr obj);
	void on_phase_changed(double phase);
	double on_measure_text(const std::string &fontFace, double fontSize, bool italic, bool bold, const std::string &text);

	static gboolean on_timer(gpointer data);
	bool handle_timer();
};

#endif /* CONTROLLER_H_ */
