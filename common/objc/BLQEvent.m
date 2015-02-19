//
//  BLQEvent.m
//  Hello Ball
//
//  Created by Marco on 19/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQEvent.h"

@implementation BLQEvent
@end

@implementation BLQMouseEvent

- (instancetype)initWithType:(BLQMouseEventType)type location:(CGPoint)location
{
    if (self = [super init]) {
        _type = type;
        _location = location;
    }
    return self;
}

@end
