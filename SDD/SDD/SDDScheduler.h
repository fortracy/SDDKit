//
//  SDDState.h
//  SDD
//
//  Created by 黎玉华 on 16/3/25.
//  Copyright © 2016年 yy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SDDEventsPool.h"


typedef BOOL (^SDDCondition)(_Nullable id param);
typedef void (^SDDAction)(_Nullable id param);
typedef void (^SDDActivation)(_Nullable id param);
typedef void (^SDDDeactivation)();


#pragma mark -

@interface SDDState : NSObject
- (nullable)init UNAVAILABLE_ATTRIBUTE;
- (nonnull instancetype)initWithActivation:(nonnull SDDActivation)activation deactivation:(nonnull SDDDeactivation)deactivation;

- (void)activate:(nullable id)argument;
- (void)deactivate;
@end

#pragma mark -

@class SDDScheduler;

@protocol SDDSchedulerLogger <NSObject>
- (void)didStartScheduler:(nonnull SDDScheduler *)scheduler activates:(nonnull NSArray<SDDState *>*)activatedStates;
- (void)didStopScheduler:(nonnull SDDScheduler *)scheduler deactivates:(nonnull NSArray<SDDState *>*)deactivatedStates;

- (void)scheduler:(nonnull SDDScheduler *)scheduler
        activates:(nonnull NSArray<SDDState *>*)activatedStates
      deactivates:(nonnull NSArray<SDDState *>*)deactivatedStates
          byEvent:(nonnull SDDEvent *)event;
@end


typedef NS_OPTIONS(NSInteger, SDDSchedulerLogMasks) {
    SDDSchedulerLogMaskStart      = 1 << 0,
    SDDSchedulerLogMaskStop       = 1 << 1,
    SDDSchedulerLogMaskTransition = 1 << 2,
    SDDSchedulerLogMaskAll        = 0xFFFF,
};

// It is used for console logging

@interface SDDSchedulerConsoleLogger : NSObject<SDDSchedulerLogger>
- (nullable)init UNAVAILABLE_ATTRIBUTE;
- (nonnull instancetype)initWithMasks:(SDDSchedulerLogMasks)masks;
@end



@interface SDDScheduler : NSObject
- (nullable instancetype)init UNAVAILABLE_ATTRIBUTE;
- (nonnull instancetype)initWithOperationQueue:(nonnull NSOperationQueue*)queue logger:(nullable id<SDDSchedulerLogger>)logger;

- (void)addState:(nonnull SDDState*)state;
- (void)state:(nonnull SDDState*)state addMonoStates:(nonnull NSArray<SDDState*>*)states;
- (void)setState:(nonnull SDDState *)state defaultState:(nonnull SDDState*)defaultState;
- (void)setRootState:(nonnull SDDState*)state;

- (void)when:(nonnull SDDEvent*)event
   satisfied:(nullable SDDCondition)condition
 transitFrom:(nonnull SDDState*)from
          to:(nonnull SDDState*)to
  postAction:(nullable SDDAction)postAction;

- (void)startWithEventsPool:(nonnull SDDEventsPool*)epool;
- (void)startWithEventsPool:(nonnull SDDEventsPool*)epool initialArgument:(nullable id)argument;
- (void)stop;
@end