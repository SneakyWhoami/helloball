/*
 * Controller.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#include <iostream>

#include "Controller.h"

gboolean Controller::on_timer(gpointer data)
{
	Controller *c = static_cast<Controller *>(data);
	return c->handle_timer();
}

Controller::Controller(HelloBallWindow *window) {
	m_window = window;
	m_window->executeButton.signal_clicked().connect(sigc::mem_fun(*this, &Controller::on_execute_button_clicked));
	m_window->ballsArea.signal_button_press_event().connect(sigc::mem_fun(*this, &Controller::on_balls_mouse_down));
	m_window->ballsArea.signal_motion_notify_event().connect(sigc::mem_fun(*this, &Controller::on_balls_mouse_move));
	m_window->ballsArea.signal_button_release_event().connect(sigc::mem_fun(*this, &Controller::on_balls_mouse_up));
}

Controller::~Controller() {
	delete m_queue;
}

bool Controller::init()
{
	if (!initFonts()) {
		return false;
	}

	m_queue = new AsyncQueue();

	m_bridge = std::shared_ptr<JSBridge>(new JSBridge(m_queue));

	m_bridge->signal_ball_count_changed.connect(sigc::mem_fun(this, &Controller::on_ball_count_changed));
	m_bridge->signal_events_per_second.connect(sigc::mem_fun(this, &Controller::on_events_per_second));
	m_bridge->signal_phase_changed.connect(sigc::mem_fun(this, &Controller::on_phase_changed));
	m_bridge->signal_displaylist_changed.connect(sigc::mem_fun(this, &Controller::on_displaylist_changed));

	m_bridge->startEngine(800, 420);

	gdk_threads_add_timeout(1000. / 5., on_timer, this);

	return true;
}

bool Controller::initFonts()
{
	return true;
}

void Controller::on_execute_button_clicked()
{
	Glib::ustring text = m_window->inputText.get_buffer()->get_text();
	JSValueRef value = m_bridge->executeScript(text.c_str());
	NativeValuePtr n = m_bridge->makeNativeValue(value);
	std::string d = n->toString();
	Glib::RefPtr<Gtk::TextBuffer> buffer = m_window->outputText.get_buffer();
	buffer->insert(buffer->end(), d + "\n");
}

bool Controller::on_balls_mouse_down(GdkEventButton* event)
{
	m_queue->enqueue([=]() {
		m_bridge->mouseDown(event->x, event->y);
	});
	return true;
}

bool Controller::on_balls_mouse_move(GdkEventMotion* event)
{
	m_queue->enqueue([=]() {
		m_bridge->mouseMove(event->x, event->y);
	});
	return true;
}

bool Controller::on_balls_mouse_up(GdkEventButton* event)
{
	m_queue->enqueue([=]() {
		m_bridge->mouseUp(event->x, event->y);
	});
	return true;
}

void Controller::on_ball_count_changed(size_t count)
{
	m_window->ballsArea.setBallsCount(count);
}

void Controller::on_events_per_second(double eps)
{
	std::ostringstream s;
	s << "EPS: " << (int)eps;
	m_window->epsLabel.set_text(s.str());
}

void Controller::on_displaylist_changed(NativeValuePtr obj)
{
	std::vector<std::string> ballIndexes = obj->objectKeys();
	size_t i, l = ballIndexes.size();
	for (i = 0; i < l; i++) {
		NativeValuePtr ballData = obj->objectEntry(ballIndexes[i]);
		size_t ballIndex = std::stoi(ballIndexes[i]);
		double x = ballData->objectEntry("x")->doubleValue();
		double y = ballData->objectEntry("y")->doubleValue();
		double radius = ballData->objectEntry("radius")->doubleValue();
		int color = ballData->objectEntry("color")->doubleValue();
		m_window->ballsArea.setBall(ballIndex, x, y, radius, color);
	}
}

void Controller::on_phase_changed(double phase)
{
	m_window->ballsArea.setBackgroundPhase(phase);
}

bool Controller::handle_timer()
{
	m_bridge->executeScript("controller.task();");
	return true;
}
