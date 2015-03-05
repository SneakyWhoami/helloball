//
//  main.cpp
//  HelloBallV8Console
//
//  Created by Michele Orsini on 25/02/15.
//  Copyright (c) 2015 Michele Orsini. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "v8Model.h"
#include "MyModelObserver.h"
#include "rapidjson/document.h"

int main(int argc, const char * argv[]) {

    MyModelObserver *observer = new MyModelObserver();
    v8Model *model = new v8Model(observer);
    
    std::ifstream t("model.js");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    model->loadJS(str.c_str(), "model.js");
    
    std::ifstream t2("custom.js");
    std::string str2((std::istreambuf_iterator<char>(t2)),
                    std::istreambuf_iterator<char>());
    model->loadJS(str2.c_str(), "custom.js");
    
    model->startModel(1000, 1000);
    
    rapidjson::Document d;
    d.Parse<0>(observer->display_list().c_str());

    double x;
    double y;
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    bool do_for_first = true;
    for (rapidjson::Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        if (do_for_first) {
            do_for_first = false;
            for (rapidjson::Value::ConstMemberIterator itr2 = itr->value.MemberBegin(); itr2 != itr->value.MemberEnd(); ++itr2) {
                printf("Type of member %s is %s\n", itr2->name.GetString(), kTypeNames[itr2->value.GetType()]);
                
                if (strcmp(itr2->name.GetString(), "x") == 0) {
                    x = itr2->value.GetDouble();
                }
                if (strcmp(itr2->name.GetString(), "y") == 0) {
                    y = itr2->value.GetDouble();
                }
            }
            
            break;
        }
        
    }
    
    model->doText();
    model->task();
    model->callMouseMethod("mouseDown", x, y);
    model->callMouseMethod("mouseMove", x + 10, y + 10);
    model->callMouseMethod("mouseUp", x + 10, y + 10);
    
    delete model;
    delete observer;
    return 0;
}
