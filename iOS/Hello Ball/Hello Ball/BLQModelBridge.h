//
//  BLQModelBridge.h
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface BLQModelBridge : NSObject

- (BOOL)startEngine;

- (void)hitTest:(CGPoint)point;

@end
