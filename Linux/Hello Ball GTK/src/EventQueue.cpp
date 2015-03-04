/*
 * EventQueue.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#include <gdk/gdk.h>

#include "EventQueue.h"

EventQueue::~EventQueue()
{

}

gboolean EventQueue::idle_callback(gpointer data)
{
	EventQueue *q = (EventQueue *)data;
	q->dispatchNextEvent();
	return false;
}

void EventQueue::postEvent(EventPtr e)
{
	m_events.push_back(e);
	gdk_threads_add_idle(&EventQueue::idle_callback, this);
}

void EventQueue::dispatchNextEvent()
{
	EventPtr e = m_events.front();
	m_events.pop_front();
	e->func();
}
