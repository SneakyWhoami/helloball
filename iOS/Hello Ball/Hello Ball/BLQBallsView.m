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

- (void)setBallCount:(NSInteger)ballCount
{
    if (!_ballLayers || (_ballLayers.count != ballCount)) {
        if (_ballLayers) {
            for (CALayer *layer in _ballLayers) {
                [layer removeFromSuperlayer];
            }
        }
        
        _ballLayers = [NSMutableArray arrayWithCapacity:ballCount];
        for (NSInteger i = 0; i < ballCount; i++) {
            CAShapeLayer *layer = [CAShapeLayer layer];
            [self.layer addSublayer:layer];
            [_ballLayers addObject:layer];
        }
    }
}

- (void)updateBalls:(NSDictionary *)balls
{
    NSLog(@"updateBalls: %d balls", balls.count);
    for (NSString *ballIndex in balls) {
        NSInteger ballIndexInt = ballIndex.integerValue;
        BLQBall *ball = [[BLQBall alloc] initBallWithDictionary:[balls objectForKey:ballIndex]];
        [self updateBallLayer:[_ballLayers objectAtIndex:ballIndexInt] withBall:ball];
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
