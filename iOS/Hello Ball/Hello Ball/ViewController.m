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

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _model = [BLQModelBridge alloc];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallsChanged:) name:BLQModelBridgeBallsChangedNotification object:_model];

    [_model startEngine];
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
    _ballsView.balls = [n.userInfo objectForKey:@"balls"];
}

@end
