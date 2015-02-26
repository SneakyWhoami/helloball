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
#include <gtkmm/scrolledwindow.h>

#include "HelloBallWindow.h"

HelloBallWindow::HelloBallWindow()
: executeButton("Execute")   // creates a new button with label "Hello World".
{
	set_title("Hello Ball");
	set_size_request(800, 600);
	set_resizable(false);
	set_border_width(0);

	add(fixed);

	inputScroll.set_size_request(800, 70);
	inputScroll.add(inputText);
	fixed.put(inputScroll, 0, 0);

	executeButton.set_size_request(800, 40);
	fixed.put(executeButton, 0, 70);

	outputScroll.set_size_request(800, 70);
	outputScroll.add(outputText);
	outputText.set_editable(false);
	fixed.put(outputScroll, 0, 110);

	ballsArea.set_size_request(800, 420);
	fixed.put(ballsArea, 0, 180);

	show_all_children();
}

HelloBallWindow::~HelloBallWindow()
{
}
