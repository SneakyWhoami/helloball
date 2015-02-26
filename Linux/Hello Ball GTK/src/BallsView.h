/*
 * BallsView.h
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#ifndef BALLSVIEW_H_
#define BALLSVIEW_H_

#include <gtkmm/drawingarea.h>
#include <vector>

typedef struct Ball {
	double x, y;
	double radius;
	double r, g, b;
};


class BallsView: public Gtk::DrawingArea {

public:
	BallsView();
	virtual ~BallsView();

	void setBallsCount(size_t count);
	void setBall(size_t index, double x, double y, double radius, int color);

protected:
	std::vector<Ball> m_balls;

	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif /* BALLSVIEW_H_ */
