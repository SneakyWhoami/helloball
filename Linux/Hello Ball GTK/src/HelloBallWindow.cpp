/*
 ============================================================================
 Name        : Hello.cpp
 Author      : Balsamiq
 Version     :
 Copyright   : 
 Description : Hello World in gtkmm
 ============================================================================
 */

#include <iostream>

#include "HelloBallWindow.h"

HelloBallWindow::HelloBallWindow()
: m_executeButton("Execute")   // creates a new button with label "Hello World".
{
	set_title("Hello Ball");
	set_size_request(800, 600);
	set_resizable(false);
	set_border_width(0);

	add(m_fixed);

	m_inputText.set_size_request(800, 70);
	m_fixed.put(m_inputText, 0, 0);

	m_executeButton.signal_clicked().connect(sigc::mem_fun(*this, &HelloBallWindow::on_button_clicked));
	m_executeButton.set_size_request(800, 40);
	m_fixed.put(m_executeButton, 0, 70);

	m_outputText.set_size_request(800, 70);
	m_outputText.set_editable(false);
	m_fixed.put(m_outputText, 0, 110);

	show_all_children();
}

HelloBallWindow::~HelloBallWindow()
{
}

void HelloBallWindow::on_button_clicked()
{
	Glib::ustring text = m_inputText.get_buffer()->get_text();
	Glib::RefPtr<Gtk::TextBuffer> buffer = m_outputText.get_buffer();
	buffer->insert(buffer->end(), text + "\n");
}
