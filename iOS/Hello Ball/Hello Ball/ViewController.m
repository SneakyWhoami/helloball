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

@interface ViewController ()

@property (nonatomic) BLQModelBridge *model;

- (void)onModelBallsChanged:(NSNotification *)n;
- (void)onModelEPSChanged:(NSNotification *)n;

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _model = [BLQModelBridge alloc];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallsChanged:) name:BLQModelBridgeBallsChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelEPSChanged:) name:BLQModelBridgeEPSChangedNotification object:_model];

    [_model startEngineWithViewSize:self.view.bounds.size];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    [_model mouseDown:[t locationInView:_ballsView]];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    [_model mouseMove:[t locationInView:_ballsView]];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    [_model mouseUp:[t locationInView:_ballsView]];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    [_model mouseUp:[t locationInView:_ballsView]];
}

- (void)onModelBallsChanged:(NSNotification *)n
{
    [_ballsView updateBalls:[n.userInfo objectForKey:@"balls"]];
}

- (void)onModelEPSChanged:(NSNotification *)n
{
    NSNumber *eps = [n.userInfo objectForKey:@"eps"];
    _epsLabel.text = [NSString stringWithFormat:@"EPS: %d", eps.integerValue];
}

@end
