//
//  MyModelObserver.cpp
//  HelloBallV8Console
//
//  Created by Michele Orsini on 25/02/15.
//  Copyright (c) 2015 Michele Orsini. All rights reserved.
//

#include <iostream>
#include "MyModelObserver.h"

const std::string& MyModelObserver::display_list()
{
    return _display_list;
}

void MyModelObserver::ball_count(int bc)
{
    std::cout << "ball count: " << bc << std::endl;
}
int MyModelObserver::ball_count()
{
    return -1;
}

int MyModelObserver::events_per_second()
{
    return -1;
}

void MyModelObserver::events_per_second(int evt)
{
    
}

void MyModelObserver::display_list(const std::string& dl)
{
    _display_list = dl;
}

void MyModelObserver::phase(double phase_value)
{
    _phase = phase_value;
    std::cout << "phase: " << _phase << std::endl;
}

double MyModelObserver::phase()
{
    return _phase;
}

void MyModelObserver::log(const std::string& message)
{
    std::cout << message << std::endl;
}
