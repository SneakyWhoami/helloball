#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <gtkmm/fixed.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/textview.h>

class HelloBallWindow : public Gtk::Window
{

public:
  HelloBallWindow();
  virtual ~HelloBallWindow();

  Gtk::Fixed fixed;
  Gtk::Button executeButton;
  Gtk::TextView inputText;
  Gtk::TextView outputText;

  //Signal handlers:
  void on_button_clicked();

  //Member widgets:
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
