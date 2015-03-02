/*
 * EventQueue.h
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <glibmm.h>


class Event
{
public:
	virtual ~Event() { }
};
typedef std::shared_ptr<Event> EventPtr;

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


class EventTarget
{
public:
	virtual ~EventTarget() { }
	virtual void handleEvent(Event *e) { };
};
typedef std::shared_ptr<EventTarget> EventTargetPtr;


class EventQueue
{
public:
	void postEvent(EventTargetPtr t, EventPtr e);

protected:
	std::deque<EventTargetPtr> m_targets;
	std::deque<EventPtr> m_events;

	static gboolean idle_callback(gpointer data);
	void dispatchNextEvent();
};

#endif /* EVENTQUEUE_H_ */
