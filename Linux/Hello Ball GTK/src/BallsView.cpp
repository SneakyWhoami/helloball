/*
 * BallsView.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

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
	m_phase = 0;
	add_events(Gdk::EventMask::BUTTON_PRESS_MASK | Gdk::EventMask::BUTTON_MOTION_MASK | Gdk::EventMask::BUTTON_RELEASE_MASK);
}

BallsView::~BallsView()
{

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

	cr->restore();

	return true;
}
