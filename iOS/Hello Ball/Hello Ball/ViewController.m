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
    
//    NSMutableArray *balls = [NSMutableArray array];
//    [balls addObject:[[BLQBall alloc] initBallWithPosition:CGPointMake( 80, 100) radius:20 color:0x00FF00]];
//    [balls addObject:[[BLQBall alloc] initBallWithPosition:CGPointMake(200, 100) radius:40 color:0xFF0000]];
//    [balls addObject:[[BLQBall alloc] initBallWithPosition:CGPointMake(200, 300) radius:30 color:0x0000FF]];
//    
//    _ballsView.balls = balls;
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
    CGPoint p = [t locationInView:_ballsView];
    CGPoint g = [t previousLocationInView:_ballsView];
    [_model mouseMove:CGPointMake(p.x - g.x, p.y - g.y)];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *t = touches.anyObject;
    [_model mouseUp:[t locationInView:_ballsView]];
}

- (void)onModelBallsChanged:(NSNotification *)n
{
    _ballsView.balls = [n.userInfo objectForKey:@"balls"];
}

@end
