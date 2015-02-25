/*
 * Controller.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

#include <iostream>

#include "Controller.h"


Controller::Controller(HelloBallWindow *window) {
	m_window = window;
	m_window->executeButton.signal_clicked().connect(sigc::mem_fun(*this, &Controller::on_execute_button_clicked));
}

Controller::~Controller() {
	delete m_bridge;
}

bool Controller::init()
{
	m_bridge = new JSBridge();

	m_bridge->signal_ball_count_changed.connect(sigc::mem_fun(this, &Controller::on_ball_count_changed));
	m_bridge->signal_events_per_second.connect(sigc::mem_fun(this, &Controller::on_events_per_second));
	m_bridge->signal_phase_changed.connect(sigc::mem_fun(this, &Controller::on_phase_changed));
	m_bridge->signal_displaylist_changed.connect(sigc::mem_fun(this, &Controller::on_displaylist_changed));

	return m_bridge->startEngine(400, 400);
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

void Controller::on_ball_count_changed(size_t count)
{
	std::cout << "ball count changed: " << count << std::endl;
}

void Controller::on_events_per_second(double eps)
{

}

void Controller::on_displaylist_changed(NativeValuePtr obj)
{

}

void Controller::on_phase_changed(double phase)
{

}
