//
//  BLQModelBridge.h
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "BLQEvent.h"

extern NSString *const BLQModelBridgeDisplayListChangedNotification;
extern NSString *const BLQModelBridgeEPSChangedNotification;
extern NSString *const BLQModelBridgeBallCountChangedNotification;
extern NSString *const BLQModelBridgePhaseChangedNotification;


@interface BLQModelBridge : NSObject

- (BOOL)startEngineWithViewSize:(CGSize)size;

- (void)handleEvent:(BLQEvent *)event;

@end
