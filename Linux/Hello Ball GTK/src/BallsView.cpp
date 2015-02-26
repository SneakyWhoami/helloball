/*
 * BallsView.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#include "BallsView.h"

BallsView::BallsView() {
	// TODO Auto-generated constructor stub

}

BallsView::~BallsView() {
	// TODO Auto-generated destructor stub
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
	queue_draw();
}

bool BallsView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	cr->save();

	cr->set_source_rgb(0.8, 1, 0.8);
	cr->paint();

	size_t i;
	for (i = 0; i < m_balls.size(); i++) {
		Ball &b = m_balls[i];
		cr->arc(b.x, b.y, b.radius, 0, M_PI * 2);
		cr->set_source_rgb(1, 0, 0);
		cr->fill_preserve();
		cr->set_source_rgb(0, 0, 0);
		cr->stroke();
	}

	cr->restore();
	return true;
}
