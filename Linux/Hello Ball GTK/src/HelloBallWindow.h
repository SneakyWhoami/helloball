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

protected:
  //Signal handlers:
  void on_button_clicked();

  //Member widgets:
  Gtk::Fixed m_fixed;
  Gtk::Button m_executeButton;
  Gtk::TextView m_inputText;
  Gtk::TextView m_outputText;
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
