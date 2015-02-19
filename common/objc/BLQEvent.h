//
//  BLQEvent.h
//  Hello Ball
//
//  Created by Marco on 19/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface BLQEvent : NSObject

@end


typedef enum {
    BLQMouseDown = 1,
    BLQMouseMove = 2,
    BLQMouseUp = 3
} BLQMouseEventType;


@interface BLQMouseEvent : BLQEvent

@property (readonly) CGPoint location;
@property (readonly) BLQMouseEventType type;

- (instancetype)initWithType:(BLQMouseEventType)type location:(CGPoint)location;

@end


@interface BLQTimerEvent : BLQEvent

@end