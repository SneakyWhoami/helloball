//
//  ViewController.h
//  Hello Ball
//
//  Created by Marco on 09/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BLQBallsView.h"

@interface ViewController : NSViewController

@property (assign) IBOutlet BLQBallsView *ballsView;
@property (assign) IBOutlet NSTextField *epsLabel;

@end

