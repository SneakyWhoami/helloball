//
//  ViewController.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "ViewController.h"
#import "BLQBall.h"
#import "BLQModelBridge.h"
#import "BLQEvent.h"

@interface ViewController ()

@property (nonatomic) BLQModelBridge *model;

- (void)onModelBallsChanged:(NSNotification *)n;
- (void)onModelEPSChanged:(NSNotification *)n;
- (void)onModelBallCountChanged:(NSNotification *)n;
- (void)onModelBallPhaseChanged:(NSNotification *)n;

- (void)taskTimer:(NSTimer *)timer;

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _model = [BLQModelBridge alloc];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallsChanged:) name:BLQModelBridgeDisplayListChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelEPSChanged:) name:BLQModelBridgeEPSChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallCountChanged:) name:BLQModelBridgeBallCountChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallPhaseChanged:) name:BLQModelBridgePhaseChangedNotification object:_model];

    [_model startEngineWithViewSize:self.view.bounds.size];
    
    [NSTimer scheduledTimerWithTimeInterval:(1. / 30.) target:self selector:@selector(taskTimer:) userInfo:nil repeats:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)taskTimer:(NSTimer *)timer
{
    BLQTimerEvent *e = [BLQTimerEvent new];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseDown location:[t locationInView:_ballsView]];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseMove location:[t locationInView:_ballsView]];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseUp location:[t locationInView:_ballsView]];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseUp location:[t locationInView:_ballsView]];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
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
    _epsLabel.text = [NSString stringWithFormat:@"EPS: %d", eps.integerValue];
}

- (void)onModelBallPhaseChanged:(NSNotification *)n
{
    _ballsView.backgroundPhase = ((NSNumber *)[n.userInfo objectForKey:@"phase"]).floatValue;
}

@end
