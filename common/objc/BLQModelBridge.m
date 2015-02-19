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

NSString *const BLQModelBridgeDisplayListChangedNotification = @"BLQModelBridgeBallsChangedNotification";
NSString *const BLQModelBridgeEPSChangedNotification = @"BLQModelBridgeEPSChangedNotification";
NSString *const BLQModelBridgeBallCountChangedNotification = @"BLQModelBridgeBallCountChangedNotification";
NSString *const BLQModelBridgePhaseChangedNotification = @"BLQModelBridgePhaseChangedNotification";

@protocol BLQModelDelegate <JSExport>

- (void)ballCountChanged:(NSNumber *)count;
- (void)displayListChanged:(NSDictionary *)displayList;
- (void)eventsPerSecond:(NSNumber *)eps;
- (void)phaseChanged:(NSNumber *)phase;
- (void)log:(NSString *)str;

@end


@interface BLQModelBridge () <BLQModelDelegate>

@property (nonatomic) JSContext *context;

- (void)mouseDown:(CGPoint)point;
- (void)mouseMove:(CGPoint)delta;
- (void)mouseUp:(CGPoint)point;

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

- (void)handleEvent:(BLQEvent *)event
{
    NSString *script;
    
    if ([event isKindOfClass:[BLQMouseEvent class]]) {
        BLQMouseEvent *e = (BLQMouseEvent *)event;
        if (e.type == BLQMouseDown) {
            script = [NSString stringWithFormat:@"controller.mouseDown(%f, %f);", e.location.x, e.location.y];
        } else if (e.type == BLQMouseMove) {
            script = [NSString stringWithFormat:@"controller.mouseMove(%f, %f);", e.location.x, e.location.y];
        } else if (e.type == BLQMouseUp) {
            script = [NSString stringWithFormat:@"controller.mouseUp(%f, %f);", e.location.x, e.location.y];
        }
    } else if ([event isKindOfClass:[BLQTimerEvent class]]) {
        script = @"controller.task();";
    }
    
    if (script) {
        [_context evaluateScript:script];
    }
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
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeDisplayListChangedNotification object:self userInfo:@{ @"balls": displayList }];
    });
}

- (void)eventsPerSecond:(NSNumber *)eps
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeEPSChangedNotification object:self userInfo:@{ @"eps": eps }];
    });
}

- (void)ballCountChanged:(NSNumber *)count
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgeBallCountChangedNotification object:self userInfo:@{ @"ballCount": count }];
    });
}

- (void)phaseChanged:(NSNumber *)phase
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [[NSNotificationCenter defaultCenter] postNotificationName:BLQModelBridgePhaseChangedNotification object:self userInfo:@{ @"phase": phase }];
    });
}

- (void)log:(NSString *)str
{
    NSLog(@"JS: %@", str);
}

@end
