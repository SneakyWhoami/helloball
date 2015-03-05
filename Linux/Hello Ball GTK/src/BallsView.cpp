/*
 * BallsView.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#include <iostream>

#include "BallsView.h"

static void int2Color(int color, double rgb[3])
{
	rgb[0] = ((color >> 16) & 0xFF) / 255.;
	rgb[1] = ((color >> 8) & 0xFF) / 255.;
	rgb[2] = (color & 0xFF) / 255.;
}

BallsView::BallsView() :
	Glib::ObjectBase("BallsView"),
	Gtk::DrawingArea()
{
	m_phase = 1;
	add_events(Gdk::EventMask::BUTTON_PRESS_MASK | Gdk::EventMask::BUTTON_MOTION_MASK | Gdk::EventMask::BUTTON_RELEASE_MASK);
}

BallsView::~BallsView()
{

}

void BallsView::setFontHelper(CairoFontHelper *h)
{
	m_h = h;
}

void BallsView::setBallsCount(size_t count)
{
	m_balls.resize(count);
	queue_draw();
}

void BallsView::setBall(size_t index, double x, double y, double radius, int color)
{
	m_balls[index].x = x;
	m_balls[index].y = y;
	m_balls[index].radius = radius;
	double rgb[3];
	int2Color(color, rgb);
	m_balls[index].r = rgb[0];
	m_balls[index].g = rgb[1];
	m_balls[index].b = rgb[2];
	queue_draw();
}

void BallsView::setBackgroundPhase(double phase)
{
	m_phase = phase;
	queue_draw();
}

bool BallsView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	cr->save();

	cr->set_source_rgb(m_phase, m_phase, m_phase);
	cr->paint();

	size_t i;
	for (i = 0; i < m_balls.size(); i++) {
		Ball &b = m_balls[i];
		cr->arc(b.x, b.y, b.radius, 0, M_PI * 2);
		cr->set_source_rgb(b.r, b.g, b.b);
		cr->fill_preserve();
		cr->set_source_rgb(0, 0, 0);
		cr->stroke();
	}

	Pango::FontDescription font;
	//font.set_family("Balsamiq Sans");
	font.set_size(36 * PANGO_SCALE);
	Glib::RefPtr<Pango::Layout> layout = create_pango_layout("AVAVAVAVA");
	layout->set_font_description(font);
	int textWidth, textHeight;
	layout->get_pixel_size(textWidth, textHeight);
	cr->move_to(10, 100);
	layout->show_in_cairo_context(cr);

	m_h->fillText(cr->cobj(), 10, 200, "AVAVAVAVA", 18 * 96 / 72);

	cr->restore();

	return true;
}
