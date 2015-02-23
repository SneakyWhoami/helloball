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

#include "HelloBallWindow.h"

int main (int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  HelloBallWindow window;
  Gtk::Main::run(window);

  return 0;
}
