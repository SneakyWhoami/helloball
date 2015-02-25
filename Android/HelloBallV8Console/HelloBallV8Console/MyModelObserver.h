//
//  MyModelObserver.h
//  HelloBallV8Console
//
//  Created by Michele Orsini on 25/02/15.
//  Copyright (c) 2015 Michele Orsini. All rights reserved.
//

#ifndef __HelloBallV8Console__MyModelObserver__
#define __HelloBallV8Console__MyModelObserver__

#include <stdio.h>
#include "IModelObserver.h"

class MyModelObserver : public IModelObserver
{
private:
    std::string _display_list;
    double _phase;
public:
    const std::string& display_list();
    void ball_count(int bc);
    int ball_count();
    int events_per_second();
    void events_per_second(int evt);
    void display_list(const std::string& dl);
    void phase(double phase_value);
    double phase();
    void log(const std::string& message);
};
#endif /* defined(__HelloBallV8Console__MyModelObserver__) */
