/*
 *  MaplyBaseViewController.h
 *  MaplyComponent
 *
 *  Created by Ranen Ghosh on 11/23/16.
 *  Copyright 2012-2022 mousebird consulting
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "math/MaplyCoordinate.h"

#define LOC_TRACKER_POS_MARKER_SIZE 32

@class MaplyBaseViewController;

typedef struct
{
    float lonDeg;
    float latDeg;
    float headingDeg;
} MaplyLocationTrackerSimulationPoint;

typedef enum {MaplyLocationLockNone, MaplyLocationLockNorthUp, MaplyLocationLockHeadingUp, MaplyLocationLockHeadingUpOffset} MaplyLocationLockType;

/* 
    Implement the MaplyLocationTrackerDelegate protocol to receive location services callbacks.
*/
@protocol MaplyLocationTrackerDelegate

// This is to handle problems / failures further up the line.
- (void) locationManager:(CLLocationManager * __nonnull)manager didFailWithError:(NSError * __nonnull)error;

- (void) locationManager:(CLLocationManager * __nonnull)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status;

@optional

- (void) updateLocation:(CLLocation * __nonnull)location;

@end

/*
    Implement the MaplyLocationSimulatorDelegate protocol to provide simulated locations
*/
@protocol MaplyLocationSimulatorDelegate

- (MaplyLocationTrackerSimulationPoint)getSimulationPoint;

@optional

- (bool)hasValidLocation;

@end

/* 
    The MaplyLocationTracker class provides support for showing current position and heading on the map or globe.
   
    Be sure to set NSLocationWhenInUseUsageDescription in your app's Info.plist before using.
 */
@interface MaplyLocationTracker : NSObject <CLLocationManagerDelegate>

/// Exposes MaplyLocationTracker's location manager for use elsewhere
@property (nonatomic, readonly, nullable) CLLocationManager *locationManager;

/**
    MaplyLocationTracker constructor
 
    @param viewC The globe or map view controller
 
    @param useHeading Use location services heading information (requires physical magnetometer)
 
    @param useCourse Use location services course information as fallback if heading unavailable
 */
- (nonnull instancetype)initWithViewC:(MaplyBaseViewController *__nullable)viewC
                           useHeading:(bool)useHeading
                            useCourse:(bool)useCourse;

/** 
    MaplyLocationTracker constructor
 
    @param viewC The globe or map view controller
 
    @param delegate The MaplyLocationTrackerDelegate for receiving location event callbacks
 
    @param useHeading Use location services heading information (requires physical magnetometer)
 
    @param useCourse Use location services course information as fallback if heading unavailable
 */
- (nonnull instancetype)initWithViewC:(MaplyBaseViewController *__nullable)viewC
                             delegate:(NSObject<MaplyLocationTrackerDelegate> *__nullable)delegate
                           useHeading:(bool)useHeading
                            useCourse:(bool)useCourse;

/**
    MaplyLocationTracker constructor
 
    @param viewC The globe or map view controller
 
    @param delegate The MaplyLocationTrackerDelegate for receiving location event callbacks

    @param simulator  The MaplyLocationSimulatorDelegate for generating simulated locations

    @param useHeading Use location services heading information (requires physical magnetometer)
 
    @param useCourse Use location services course information as fallback if heading unavailable
 */
- (nonnull instancetype)initWithViewC:(MaplyBaseViewController *__nullable)viewC
                             delegate:(NSObject<MaplyLocationTrackerDelegate> *__nullable)delegate
                            simulator:(NSObject<MaplyLocationSimulatorDelegate> *__nullable)simulator
                          simInterval:(NSTimeInterval)simInterval
                           useHeading:(bool)useHeading
                            useCourse:(bool)useCourse;

/**
    Min/max visibility for the marker assigned to follow location.
  */
@property (nonatomic,assign) float markerMinVis,markerMaxVis;

/**
    Draw priority for the marker assigned to follow location.
  */
@property (nonatomic,assign) int markerDrawPriority;

/** 
    Change lock type
 
    @param lockType The MaplyLocationLockType value for lock behavior
 
    @param forwardTrackOffset The vertical offset if using MaplyLocationLockHeadingUpOffset (positive values are below the view center)
 */
- (void) changeLockType:(MaplyLocationLockType)lockType forwardTrackOffset:(int)forwardTrackOffset;

/** 
    Stop the MaplyLocationTracker behavior and shut it down.
 */
- (void) teardown;

/** 
    Get the current device location
 
    @return The coordinate if valid, else kMaplyNullCoordinate
 */
- (MaplyCoordinate)getLocation;

/**
    Set the current simulated location.
 */
- (void) setLocation:(MaplyLocationTrackerSimulationPoint)point
            altitude:(double)altitude;

/**
    Set the current simulated location.
 */
- (void) setLocation:(MaplyLocationTrackerSimulationPoint)point
            altitude:(double)altitude
  horizontalAccuracy:(double)horizontalAccuracy
    verticalAccuracy:(double)verticalAccuracy
               speed:(double)speed;
;

@end

