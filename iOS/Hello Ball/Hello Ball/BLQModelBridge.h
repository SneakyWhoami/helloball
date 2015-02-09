//
//  BLQModelBridge.h
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

NSString *const BLQModelBridgeBallsChangedNotification;


@interface BLQModelBridge : NSObject

- (BOOL)startEngine;

- (void)mouseDown:(CGPoint)point;
- (void)mouseMove:(CGPoint)delta;
- (void)mouseUp:(CGPoint)point;

@end
