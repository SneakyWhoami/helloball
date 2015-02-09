//
//  BLQBallsView.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQBallsView.h"
#import "BLQBall.h"


@interface BLQBallsView ()

@property NSMutableArray *ballLayers;

- (void)updateBallLayer:(CAShapeLayer *)layer withBall:(BLQBall *)ball;
- (UIColor *)colorFromInt:(int)color;

@end


@implementation BLQBallsView

- (void)setBalls:(NSArray *)balls
{
    _balls = balls;
    
    if (!_ballLayers || (_ballLayers.count != _balls.count)) {
        if (_ballLayers) {
            for (CALayer *layer in _ballLayers) {
                [layer removeFromSuperlayer];
            }
        }
        
        _ballLayers = [NSMutableArray arrayWithCapacity:_balls.count];
        for (NSInteger i = 0; i < _balls.count; i++) {
            CAShapeLayer *layer = [CAShapeLayer layer];
            [self.layer addSublayer:layer];
            [_ballLayers addObject:layer];
        }
    }

    for (NSInteger i = 0; i < _balls.count; i++) {
        [self updateBallLayer:[_ballLayers objectAtIndex:i] withBall:[_balls objectAtIndex:i]];
    }
}

- (void)updateBallLayer:(CAShapeLayer *)layer withBall:(BLQBall *)ball
{
    [CATransaction begin];
    [CATransaction setValue:(id)kCFBooleanTrue forKey:kCATransactionDisableActions];
    
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathAddArc(path, NULL, 0, 0, ball.radius, 0, M_PI * 2, NO);
    layer.path = path;
    CGPathRelease(path);
    
    layer.position = ball.position;
    layer.borderWidth = 1; // to test
    layer.fillColor = [self colorFromInt:ball.color].CGColor;
    layer.strokeColor = [UIColor blackColor].CGColor;

    [CATransaction commit];
}

- (UIColor *)colorFromInt:(int)color
{
    CGFloat r = ((color >> 16) & 0xFF) / 255.;
    CGFloat g = ((color >> 8) & 0xFF) / 255.;
    CGFloat b = (color & 0xFF) / 255.;
    return [UIColor colorWithRed:r green:g blue:b alpha:1];
}

@end
