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
: executeButton("Execute")   // creates a new button with label "Hello World".
{
	set_title("Hello Ball");
	set_size_request(800, 600);
	set_resizable(false);
	set_border_width(0);

	add(fixed);

	inputText.set_size_request(800, 70);
	fixed.put(inputText, 0, 0);

//	executeButton.signal_clicked().connect(sigc::mem_fun(*this, &HelloBallWindow::on_button_clicked));
	executeButton.set_size_request(800, 40);
	fixed.put(executeButton, 0, 70);

	outputText.set_size_request(800, 70);
	outputText.set_editable(false);
	fixed.put(outputText, 0, 110);

	show_all_children();
}

HelloBallWindow::~HelloBallWindow()
{
}

void HelloBallWindow::on_button_clicked()
{
	Glib::ustring text = inputText.get_buffer()->get_text();
	Glib::RefPtr<Gtk::TextBuffer> buffer = outputText.get_buffer();
	buffer->insert(buffer->end(), text + "\n");
}
