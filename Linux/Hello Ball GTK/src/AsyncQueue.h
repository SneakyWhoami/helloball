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
#include <deque>


typedef std::function<void ()> Function;


class AsyncQueue
{
public:
	~AsyncQueue();
	void enqueue(Function f);

protected:
	std::deque<Function> m_functions;

	static gboolean idle_callback(gpointer data);
	void callNextFunction();
};

#endif /* EVENTQUEUE_H_ */
