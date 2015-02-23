/*
 ============================================================================
 Name        : Hello.cpp
 Author      : Balsamiq
 Version     :
 Copyright   : 
 Description : Hello World in gttkmm
 ============================================================================
 */

#include <gtkmm/main.h>
#include <iostream>

#include "HelloBallWindow.h"
#include "Controller.h"

int main (int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  HelloBallWindow window;
  Controller controller(&window);
  if (controller.init()) {
	  Gtk::Main::run(window);
  } else {
	  std::cout << "ERROR: cannot init Controller";
  }

  return 0;
}
