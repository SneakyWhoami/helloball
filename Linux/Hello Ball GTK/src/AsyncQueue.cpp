/*
 * EventQueue.cpp
 *
 *  Created on: Feb 26, 2015
 *      Author: pepo
 */

#include <gdk/gdk.h>

#include "AsyncQueue.h"

AsyncQueue::~AsyncQueue()
{

}

gboolean AsyncQueue::idle_callback(gpointer data)
{
	AsyncQueue *q = (AsyncQueue *)data;
	q->callNextFunction();
	return false;
}

void AsyncQueue::enqueue(Function f)
{
	m_functions.push_back(f);
	gdk_threads_add_idle(&AsyncQueue::idle_callback, this);
}

void AsyncQueue::callNextFunction()
{
	Function f = m_functions.front();
	m_functions.pop_front();
	f();
}
