//
//  BLQModelBridge.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQModelBridge.h"
#import <JavaScriptCore/JavaScriptCore.h>
#import "BLQBall.h"

NSString *const BLQModelBridgeBallsChangedNotification = @"BLQModelBridgeBallsChangedNotification";


@protocol BLQModelDelegate <JSExport>

- (void)ballsChanged:(NSArray *)balls;
- (void)selectedBallChanged:(int)index;

@end


@interface BLQModelBridge () <BLQModelDelegate>

@property (nonatomic) JSContext *context;

@end


@implementation BLQModelBridge

- (void)dealloc
{
    _context = nil;
}

- (BOOL)startEngine
{
    _context = [[JSContext alloc] initWithVirtualMachine:[[JSVirtualMachine alloc] init]];
    
    if (_context) {
        NSURL *codeURL = [[NSBundle mainBundle] URLForResource:@"model" withExtension:@"js"];
        NSString *code = [NSString stringWithContentsOfURL:codeURL encoding:NSUTF8StringEncoding error:nil];
        [_context evaluateScript:code withSourceURL:codeURL];
        _context[@"bridge"] = self;
        [_context evaluateScript:@"model.setDelegate(bridge);"];
    }
    return (_context != nil);
}

- (void)hitTest:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"model.hitTest(%f, %f);", point.x, point.y];
    JSValue *val = [_context evaluateScript:script];
    NSLog(@"value: %@", val.toString);
}

- (void)mouseDown:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"model.mouseDown(%f, %f);", point.x, point.y];
    [_context evaluateScript:script];
}

- (void)mouseMove:(CGPoint)delta
{
    NSString *script = [NSString stringWithFormat:@"model.mouseMove(%f, %f);", delta.x, delta.y];
    [_context evaluateScript:script];
}

- (void)mouseUp:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"model.mouseUp(%f, %f);", point.x, point.y];
    [_context evaluateScript:script];
//    JSGarbageCollect(_context.JSGlobalContextRef);
}

- (void)ballsChanged:(NSArray *)balls
{
//    NSLog(@"ballsChanged: %@", balls);
    NSMutableArray *a = [NSMutableArray arrayWithCapacity:balls.count];
    for (NSDictionary *d in balls) {
        [a addObject:[[BLQBall alloc] initBallWithDictionary:d]];
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeBallsChangedNotification object:self userInfo:@{ @"balls": a }];
}

- (void)selectedBallChanged:(int)index
{
    
}

@end
