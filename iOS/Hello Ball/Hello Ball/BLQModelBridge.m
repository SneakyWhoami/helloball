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
NSString *const BLQModelBridgeEPSChangedNotification = @"BLQModelBridgeEPSChangedNotification";


@protocol BLQModelDelegate <JSExport>

- (void)displayListChanged:(NSDictionary *)displayList;
- (void)eventsPerSecond:(NSNumber *)eps;

@end


@interface BLQModelBridge () <BLQModelDelegate>

@property (nonatomic) JSContext *context;

@end


@implementation BLQModelBridge

- (void)dealloc
{
    _context = nil;
}

- (BOOL)startEngineWithViewSize:(CGSize)size
{
    _context = [[JSContext alloc] initWithVirtualMachine:[[JSVirtualMachine alloc] init]];
    
    if (_context) {
        NSURL *codeURL = [[NSBundle mainBundle] URLForResource:@"model" withExtension:@"js"];
        NSString *code = [NSString stringWithContentsOfURL:codeURL encoding:NSUTF8StringEncoding error:nil];
        [_context evaluateScript:code withSourceURL:codeURL];
        _context[@"bridge"] = self;
        NSString *script = [NSString stringWithFormat:@"var controller = initApp(%d, %d, bridge);", (int)size.width, (int)size.height];
        [_context evaluateScript:script];
    }
    return (_context != nil);
}

- (void)mouseDown:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"controller.mouseDown(%f, %f);", point.x, point.y];
    [_context evaluateScript:script];
}

- (void)mouseMove:(CGPoint)delta
{
    NSString *script = [NSString stringWithFormat:@"controller.mouseMove(%f, %f);", delta.x, delta.y];
    [_context evaluateScript:script];
}

- (void)mouseUp:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"controller.mouseUp(%f, %f);", point.x, point.y];
    [_context evaluateScript:script];
//    JSGarbageCollect(_context.JSGlobalContextRef);
}

- (void)displayListChanged:(NSDictionary *)displayList
{
    [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeBallsChangedNotification object:self userInfo:@{ @"balls": displayList }];
}

- (void)eventsPerSecond:(NSNumber *)eps
{
    [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeEPSChangedNotification object:self userInfo:@{ @"eps": eps }];
}

@end
