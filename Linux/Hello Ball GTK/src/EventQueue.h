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
	virtual ~Event();
};
typedef std::shared_ptr<Event> EventPtr;

class EventTarget
{
public:
	virtual ~EventTarget() { }
	virtual void handleEvent(Event *e) = 0;
};
typedef std::shared_ptr<EventTarget> EventTargetPtr;


class EventQueue
{
public:
	void postEvent(EventTargetPtr &t, EventPtr &e);

protected:
	std::vector<EventTargetPtr> m_targets;
	std::vector<EventPtr> m_events;

	static gboolean idle_callback(gpointer data);
	void dispatchNextEvent();
};

#endif /* EVENTQUEUE_H_ */
