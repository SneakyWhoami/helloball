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

NSString *const BLQModelBridgeDisplayListChangedNotification;
NSString *const BLQModelBridgeEPSChangedNotification;
NSString *const BLQModelBridgeBallCountChangedNotification;
NSString *const BLQModelBridgePhaseChangedNotification;


@interface BLQModelBridge : NSObject

- (BOOL)startEngineWithViewSize:(CGSize)size;

- (void)handleEvent:(BLQEvent *)event;

@end
