/*
 * Controller.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: pepo
 */

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
	return m_bridge->startEngine(400, 400);
}

void Controller::on_execute_button_clicked()
{
	Glib::ustring text = m_window->inputText.get_buffer()->get_text();
	JSValueRef value = m_bridge->executeScript(text.c_str());
	Glib::ustring result = m_bridge->makeString(value);
	Glib::RefPtr<Gtk::TextBuffer> buffer = m_window->outputText.get_buffer();
	buffer->insert(buffer->end(), result + "\n");
}
