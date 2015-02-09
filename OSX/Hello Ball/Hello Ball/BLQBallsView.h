//
//  BLQBallsView.h
//  Hello Ball
//
//  Created by Marco on 09/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface BLQBallsView : NSView

@property (nonatomic) NSInteger ballCount;

- (void)updateBalls:(NSDictionary *)balls;

@end
