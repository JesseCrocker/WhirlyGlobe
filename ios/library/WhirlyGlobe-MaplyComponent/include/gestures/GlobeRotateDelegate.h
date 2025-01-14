/*
 *  RotateDelegate.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 6/10/11.
 *  Copyright 2011-2022 mousebird consulting
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
#import "GlobeView_iOS.h"

@protocol WhirlyKitViewWrapper;

namespace WhirlyGlobe
{

// Sent out when the rotation delegate takes control
#define kRotateDelegateDidStart @"WKRotateDelegateStarted"
// Sent out when the rotation delegate finished (but hands off to momentum)
#define kRotateDelegateDidEnd @"WKRotateDelegateEnded"

/// The state of our rotation
///  which is not currently in use.
typedef enum {RotNone,RotFree} RotationType;
    
}

/** Rotation delegate 
    is for two fingered rotation around the axis at the middle of the screen
 */
@interface WhirlyGlobeRotateDelegate : NSObject <UIGestureRecognizerDelegate> 

/// Create a rotation gesture and a delegate and write them up to the given UIView
+ (WhirlyGlobeRotateDelegate *)rotateDelegateForView:(UIView *)view globeView:(WhirlyGlobe::GlobeView_iOS *)globeView;

/// If set, the rotation will occur around the center between the two fingers rather than the current viewpoint
@property (nonatomic) bool rotateAroundCenter;

/// Gesture recognizer attached to this delegate (or vice versa, actually)
@property (nonatomic,weak) UIGestureRecognizer *gestureRecognizer;

/// Can be called by a cooperating delegate (which is also messing with rotation) (HACK!)
- (void)updateWithCenter:(CGPoint)center touch:(CGPoint)touch wrapView:(UIView<WhirlyKitViewWrapper> *)glView;

@end
