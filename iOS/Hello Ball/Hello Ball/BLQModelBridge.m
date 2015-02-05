//
//  BLQModelBridge.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQModelBridge.h"
#import <JavaScriptCore/JavaScriptCore.h>

@interface BLQModelBridge ()

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
        JSValue *val = [_context evaluateScript:code withSourceURL:codeURL];
        NSLog(@"value: %@", val.toString);
    }
    return (_context != nil);
}

- (void)hitTest:(CGPoint)point
{
    NSString *script = [NSString stringWithFormat:@"model.findBallAtPosition(%f, %f);", point.x, point.y];
    JSValue *val = [_context evaluateScript:script];
    NSLog(@"value: %@", val.toString);
}

@end
