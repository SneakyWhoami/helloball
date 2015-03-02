/*
 * EventQueue.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#include <gdk/gdk.h>

#include "EventQueue.h"


gboolean EventQueue::idle_callback(gpointer data)
{
	EventQueue *q = (EventQueue *)data;
	q->dispatchNextEvent();
	return false;
}

void EventQueue::postEvent(EventTargetPtr t, EventPtr e)
{
	m_targets.push_back(t);
	m_events.push_back(e);
	gdk_threads_add_idle(&EventQueue::idle_callback, this);
}

void EventQueue::dispatchNextEvent()
{
	EventPtr e = m_events.front();
	m_events.pop_front();
	EventTargetPtr t = m_targets.front();
	m_targets.pop_front();

	t->handleEvent(e.get());
}
