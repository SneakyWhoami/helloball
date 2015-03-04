/*
 * EventQueue.h
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <glibmm.h>
#include <functional>


class Event
{
public:
	std::function<void ()> func;

	Event(std::function<void ()> func) { this->func = func; }
};

typedef std::shared_ptr<Event> EventPtr;

/*
class MouseEvent : public Event
{
public:
	typedef enum {
		MouseDown,
		MouseMove,
		MouseUp
	} MouseEventType;

	MouseEventType type;
	int x;
	int y;

	MouseEvent(MouseEventType t, int x, int y) {
		this->type = t;
		this->x = x;
		this->y = y;
	}
};

*/

class EventQueue
{
public:
	~EventQueue();
	void postEvent(EventPtr e);

protected:
	std::deque<EventPtr> m_events;

	static gboolean idle_callback(gpointer data);
	void dispatchNextEvent();
};

#endif /* EVENTQUEUE_H_ */
