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

#include "CairoFontHelper.h"


class BallsView: public Gtk::DrawingArea {

public:
	BallsView();
	virtual ~BallsView();

	void setFontHelper(CairoFontHelper *h);

	void setBallsCount(size_t count);
	void setBall(size_t index, double x, double y, double radius, int color);

	void setBackgroundPhase(double phase);

protected:
	CairoFontHelper *m_h;

	struct Ball {
		double x, y;
		double radius;
		double r, g, b;
	};

	std::vector<Ball> m_balls;
	double m_phase;

	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};

#endif /* BALLSVIEW_H_ */
