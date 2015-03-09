//
//  BLQModelBridge.m
//  Hello Ball
//
//  Created by Marco on 05/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "BLQModelBridge.h"
#import <JavaScriptCore/JavaScriptCore.h>
#import <CoreText/CoreText.h>
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

JSExportAs(measureText,
- (double)measureText:(NSString *)fontFamily fontSize:(double)fontSize italic:(BOOL)italic bold:(BOOL)bold text:(NSString *)text
);

@end


@interface BLQModelBridge () <BLQModelDelegate>

@property (nonatomic) JSContext *context;

- (void)mouseDown:(CGPoint)point;
- (void)mouseMove:(CGPoint)delta;
- (void)mouseUp:(CGPoint)point;

@end

static CFMutableAttributedStringRef makeAttributedString(NSString *fontFamily, double fontSize, bool bold, bool italic, bool underline, NSString *text)
{
    CFMutableAttributedStringRef attributedString = CFAttributedStringCreateMutable(kCFAllocatorDefault, text.length);
    
    CFStringRef string = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)text.UTF8String, strlen(text.UTF8String), kCFStringEncodingUTF8, FALSE);
    CFAttributedStringReplaceString(attributedString, CFRangeMake(0, 0), string);
    CFRelease(string);
    
    CFIndex stringLength = CFAttributedStringGetLength(attributedString);
    
//    std::stringstream ss;
//    ss << "BalsamiqSans"; // fontFamily
//    if (bold || italic) { ss << "-"; }
//    if (bold) { ss << "Bold"; }
//    if (italic) { ss << "Italic"; }
    NSString *fontFamilyPostScript = @"BalsamiqSans";
    CFStringRef fontFamilyString = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)fontFamilyPostScript.UTF8String, strlen(fontFamilyPostScript.UTF8String), kCFStringEncodingUTF8, false);
    CTFontRef font = CTFontCreateWithName(fontFamilyString, fontSize, NULL);
    CFRelease(fontFamilyString);
    CFAttributedStringSetAttribute(attributedString, CFRangeMake(0, stringLength), kCTFontAttributeName, font);
    CFRelease(font);
    
//    CGFloat components[4];
//    int2rgb(textColor, components);
//    CGColorRef color = CGColorCreateGenericRGB(components[0], components[1], components[2], components[3]);
//    CFAttributedStringSetAttribute(attributedString, CFRangeMake(0, stringLength), kCTForegroundColorAttributeName, color);
//    CGColorRelease(color);
    
    return attributedString;
}

static double measureText(NSString *fontFamily, double fontSize, bool bold, bool italic, NSString *str)
{
    CFMutableAttributedStringRef attributedString = makeAttributedString(fontFamily, fontSize, bold, italic, false, str);
    
    CTLineRef line = CTLineCreateWithAttributedString(attributedString);
    CGRect r = CTLineGetBoundsWithOptions(line, 0);
    
    CFRelease(line);
    CFRelease(attributedString);
    
    return r.size.width;
}


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

- (double)measureText:(NSString *)fontFamily fontSize:(double)fontSize italic:(BOOL)italic bold:(BOOL)bold text:(NSString *)text
{
    return measureText(fontFamily, fontSize, bold, italic, text);
}

@end
