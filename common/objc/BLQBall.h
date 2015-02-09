//
//  BLQBall.h
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

@interface BLQBall : NSObject

@property (nonatomic) CGPoint position;
@property (nonatomic) CGFloat radius;
@property (nonatomic) NSInteger color;

- (instancetype)initBallWithPosition:(CGPoint)pos radius:(CGFloat)radius color:(NSInteger)color;
- (instancetype)initBallWithDictionary:(NSDictionary *)d;

@end
