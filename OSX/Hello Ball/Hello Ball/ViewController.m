//
//  ViewController.m
//  Hello Ball
//
//  Created by Marco on 09/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "ViewController.h"
#import "BLQModelBridge.h"


@interface ViewController ()

@property (nonatomic) BLQModelBridge *model;

- (void)onModelBallsChanged:(NSNotification *)n;
- (void)onModelEPSChanged:(NSNotification *)n;
- (void)onModelBallCountChanged:(NSNotification *)n;

- (void)timer:(NSTimer *)timer;

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _model = [BLQModelBridge alloc];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallsChanged:) name:BLQModelBridgeDisplayListChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelEPSChanged:) name:BLQModelBridgeEPSChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallCountChanged:) name:BLQModelBridgeBallCountChangedNotification object:_model];
    
    [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(timer:) userInfo:nil repeats:NO];
}

- (void)timer:(NSTimer *)timer
{
    [_model startEngineWithViewSize:self.view.bounds.size];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)mouseDown:(NSEvent *)theEvent
{
    [_model mouseDown:theEvent.locationInWindow];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    [_model mouseMove:theEvent.locationInWindow];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    [_model mouseUp:theEvent.locationInWindow];
}

- (void)onModelBallsChanged:(NSNotification *)n
{
    [_ballsView updateBalls:[n.userInfo objectForKey:@"balls"]];
}

- (void)onModelBallCountChanged:(NSNotification *)n
{
    _ballsView.ballCount = ((NSNumber *)[n.userInfo objectForKey:@"ballCount"]).integerValue;
}

- (void)onModelEPSChanged:(NSNotification *)n
{
    NSNumber *eps = [n.userInfo objectForKey:@"eps"];
    _epsLabel.stringValue = [NSString stringWithFormat:@"EPS: %ld", (long)eps.integerValue];
}

@end
