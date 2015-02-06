//
//  BLQBall.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQBall.h"

@implementation BLQBall

- (instancetype)initBallWithPosition:(CGPoint)pos radius:(CGFloat)radius color:(NSInteger)color
{
    if (self = [super init]) {
        _position = pos;
        _radius = radius;
        _color = color;
    }
    return self;
}

- (instancetype)initBallWithDictionary:(NSDictionary *)d
{
    return [self initBallWithPosition:CGPointMake(
                                                  ((NSNumber *)[d objectForKey:@"x"]).floatValue,
                                                  ((NSNumber *)[d objectForKey:@"y"]).floatValue)
                               radius:((NSNumber *)[d objectForKey:@"radius"]).floatValue
                                color:((NSNumber *)[d objectForKey:@"color"]).floatValue];
}

@end
