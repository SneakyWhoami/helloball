//
//  ViewController.m
//  Hello Ball
//
//  Created by Marco on 09/02/15.
//  Copyright (c) 2015 Balsamiq. All rights reserved.
//

#import "ViewController.h"
#import "BLQModelBridge.h"
#import "BLQEvent.h"


@interface ViewController ()

@property (nonatomic) BLQModelBridge *model;

- (void)onModelBallsChanged:(NSNotification *)n;
- (void)onModelEPSChanged:(NSNotification *)n;
- (void)onModelBallCountChanged:(NSNotification *)n;
- (void)onModelBallPhaseChanged:(NSNotification *)n;

- (void)timerAfterLoad:(NSTimer *)timer;
- (void)taskTimer:(NSTimer *)timer;

@end


@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    NSArray *fontFileNames = @[ @"BalsamiqSansRegular", @"BalsamiqSansBold", @"BalsamiqSansItalic", @"BalsamiqSansBoldItalic" ];
    for (NSUInteger i = 0; i < fontFileNames.count; i += 1) {
        NSURL *url = [[NSBundle mainBundle] URLForResource:[fontFileNames objectAtIndex:i] withExtension:@"ttf"];
        CFErrorRef *err = NULL;
        BOOL ok = CTFontManagerRegisterFontsForURL((__bridge CFURLRef)url, kCTFontManagerScopeProcess, err);
    }

    _model = [BLQModelBridge alloc];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallsChanged:) name:BLQModelBridgeDisplayListChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelEPSChanged:) name:BLQModelBridgeEPSChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallCountChanged:) name:BLQModelBridgeBallCountChangedNotification object:_model];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onModelBallPhaseChanged:) name:BLQModelBridgePhaseChangedNotification object:_model];
    
    [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(timerAfterLoad:) userInfo:nil repeats:NO];
//    [NSTimer scheduledTimerWithTimeInterval:(1. / 60.) target:self selector:@selector(taskTimer:) userInfo:nil repeats:YES];
}

- (void)timerAfterLoad:(NSTimer *)timer
{
    [_model startEngineWithViewSize:self.view.bounds.size];
}

- (void)taskTimer:(NSTimer *)timer
{
    BLQTimerEvent *e = [BLQTimerEvent new];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)mouseDown:(NSEvent *)theEvent
{
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseDown location:theEvent.locationInWindow];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseMove location:theEvent.locationInWindow];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    BLQMouseEvent *e = [[BLQMouseEvent alloc] initWithType:BLQMouseUp location:theEvent.locationInWindow];
    [_model performSelectorOnMainThread:@selector(handleEvent:) withObject:e waitUntilDone:NO];
}

- (void)onModelBallsChanged:(NSNotification *)n
{
    [_ballsView updateBalls:[n.userInfo objectForKey:@"balls"]];
}

- (void)onModelBallCountChanged:(NSNotification *)n
{
    _ballsView.ballCount = ((NSNumber *)[n.userInfo objectForKey:@"ballCount"]).integerValue;
}

- (void)onModelEPSChanged:(NSNotification *)n
{
    NSNumber *eps = [n.userInfo objectForKey:@"eps"];
    _epsLabel.stringValue = [NSString stringWithFormat:@"EPS: %ld", (long)eps.integerValue];
}

- (void)onModelBallPhaseChanged:(NSNotification *)n
{
    _ballsView.backgroundPhase = ((NSNumber *)[n.userInfo objectForKey:@"phase"]).floatValue;
}

@end
