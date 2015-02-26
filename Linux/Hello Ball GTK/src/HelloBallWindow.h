#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/fixed.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>

#include "BallsView.h"


class HelloBallWindow : public Gtk::Window
{

public:
  HelloBallWindow();
  virtual ~HelloBallWindow();

  Gtk::Fixed fixed;
  Gtk::Button executeButton;
  Gtk::ScrolledWindow inputScroll;
  Gtk::TextView inputText;
  Gtk::ScrolledWindow outputScroll;
  Gtk::TextView outputText;
  BallsView ballsArea;
  Gtk::Label epsLabel;
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
