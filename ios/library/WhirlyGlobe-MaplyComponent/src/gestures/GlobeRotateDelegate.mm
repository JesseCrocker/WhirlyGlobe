/*
 *  RotateDelegate.mm
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

#import "WhirlyVector.h"
#import "gestures/GlobeRotateDelegate.h"
#import "SceneRenderer.h"
#import "IntersectionManager.h"
#import "ViewWrapper.h"

using namespace Eigen;
using namespace WhirlyKit;
using namespace WhirlyGlobe;

@implementation WhirlyGlobeRotateDelegate
{
    bool valid;
    GlobeView_iOS *globeView;
	WhirlyKit::Point3d startOnSphere;
	/// The view transform when we started
	Eigen::Matrix4d startTransform;
    /// Starting point for rotation
    Eigen::Quaterniond startQuat;
    /// Axis to rotate around
//    Eigen::Vector3d axis;
    float startRot;
    double sphereRadius;
}

- (id)initWithGlobeView:(GlobeView_iOS *)inView
{
	if ((self = [super init]))
	{
		globeView = inView;
        valid = false;
        _rotateAroundCenter = true;
        sphereRadius = 1.0;
	}
	
	return self;
}

+ (WhirlyGlobeRotateDelegate *)rotateDelegateForView:(UIView *)view globeView:(GlobeView_iOS *)globeView
{
	WhirlyGlobeRotateDelegate *rotateDelegate = [[WhirlyGlobeRotateDelegate alloc] initWithGlobeView:globeView];
    UIRotationGestureRecognizer *rotateRecog = [[UIRotationGestureRecognizer alloc] initWithTarget:rotateDelegate action:@selector(rotateGesture:)];
    rotateRecog.delegate = rotateDelegate;
	[view addGestureRecognizer:rotateRecog];
    rotateDelegate.gestureRecognizer = rotateRecog;
	return rotateDelegate;
}

// We'll let other gestures run
// We're expecting to run as well
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return TRUE;
}

- (void)updateWithCenter:(CGPoint)center touch:(CGPoint)touch wrapView:(UIView<WhirlyKitViewWrapper> *)wrapView
{
	SceneRenderer *sceneRender = wrapView.renderer;
    Point2f center2f(center.x,center.y);
    
    auto frameSizeScaled = sceneRender->getFramebufferSizeScaled();

    if (valid)
    {
//        NSLog(@"Rotate updated");
        Eigen::Quaterniond newRotQuat = startQuat;
        Point3d axis = globeView->currentUp();
        if (_rotateAroundCenter)
        {
            // Figure out where we are now
            // We have to roll back to the original transform with the current height
            //  to get the rotation we want
            Point3d hit;
            Eigen::Quaterniond oldQuat = globeView->getRotQuat();
            globeView->setRotQuat(startQuat,false);
            Eigen::Matrix4d curTransform = globeView->calcFullMatrix();
            if (globeView->pointOnSphereFromScreen(center2f, curTransform, frameSizeScaled, hit, true, sphereRadius))
            {
                // This gives us a direction to rotate around
                // And how far to rotate
                Eigen::Quaterniond endRot;
                endRot.setFromTwoVectors(startOnSphere,hit);
                axis = hit.normalized();
                newRotQuat = startQuat * endRot;
            } else {
                newRotQuat = oldQuat;
            }
        }
        
        // And do a rotation around the pinch
        CGPoint touch0 = touch;
        float dx = touch0.x-center.x,dy=touch0.y-center.y;
        double curRot = atan2(dy, dx);
        double diffRot = curRot-startRot;
        Eigen::AngleAxisd rotQuat(-diffRot,axis);
        newRotQuat = newRotQuat * rotQuat;
        
        globeView->setRotQuat(newRotQuat,false);
    }
}

// Called for rotate actions
- (void)rotateGesture:(id)sender
{
	UIRotationGestureRecognizer *rotate = sender;
    UIView<WhirlyKitViewWrapper> *wrapView = (UIView<WhirlyKitViewWrapper> *)rotate.view;
    SceneRenderer *sceneRender = wrapView.renderer;
    auto frameSizeScaled = sceneRender->getFramebufferSizeScaled();
    
    // Turn off rotation if we fall below two fingers
    if ([rotate numberOfTouches] < 2)
    {
        if (valid)
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidEnd object:globeView->tag];
        valid = false;
        return;
    }

    IntersectionManagerRef intManager = std::dynamic_pointer_cast<IntersectionManager>(sceneRender->getScene()->getManager(kWKIntersectionManager));

	switch (rotate.state)
	{
		case UIGestureRecognizerStateBegan:
        {
//            NSLog(@"Rotate started");
            globeView->cancelAnimation();

			startTransform = globeView->calcFullMatrix();
            startQuat = globeView->getRotQuat();
            valid = true;
            CGPoint startPoint = [rotate locationInView:wrapView];
            
            // Look for an intersection with grabbable objects
            Point3d interPt;
            double interDist;
            if (intManager->findIntersection(sceneRender, globeView, frameSizeScaled, Point2f(startPoint.x,startPoint.y), interPt, interDist))
            {
                sphereRadius = interPt.norm();
                startOnSphere = interPt.normalized();
                valid = true;
            } else {
                CGPoint pt = [rotate locationInView:wrapView];
                Point2f pt2f(pt.x,pt.y);
                if (globeView->pointOnSphereFromScreen(pt2f, startTransform, frameSizeScaled, startOnSphere, true))
                    valid = true;
                else
                    valid = false;
            }
            
            CGPoint center = [rotate locationInView:wrapView];
            CGPoint touch0 = [rotate locationOfTouch:0 inView:wrapView];
            float dx = touch0.x-center.x,dy=touch0.y-center.y;
            startRot = atan2(dy, dx);
            
            if (valid)
                globeView->cancelAnimation();
            
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidStart object:globeView->tag];
        }
			break;
		case UIGestureRecognizerStateChanged:
        {
            globeView->cancelAnimation();

            if (valid)
            {
//                NSLog(@"Rotate updated");
                Eigen::Quaterniond newRotQuat = startQuat;
                Point3d axis = globeView->currentUp();
                if (_rotateAroundCenter)
                {
                    // Figure out where we are now
                    // We have to roll back to the original transform with the current height
                    //  to get the rotation we want
                    Point3d hit;
                    Eigen::Quaterniond oldQuat = globeView->getRotQuat();
                    globeView->setRotQuat(startQuat,false);
                    Eigen::Matrix4d curTransform = globeView->calcFullMatrix();
                    CGPoint pt = [rotate locationInView:wrapView];
                    Point2f pt2f(pt.x,pt.y);
                    if (globeView->pointOnSphereFromScreen(pt2f, curTransform, frameSizeScaled, hit, true, sphereRadius))
                    {
                        // This gives us a direction to rotate around
                        // And how far to rotate
                        Eigen::Quaterniond endRot;
                        endRot.setFromTwoVectors(startOnSphere,hit);
                        axis = hit.normalized();
                        newRotQuat = startQuat * endRot;
                    } else {
                        newRotQuat = oldQuat;
                    }
                }
                
                // And do a rotation around the pinch
                CGPoint center = [rotate locationInView:wrapView];
                CGPoint touch0 = [rotate locationOfTouch:0 inView:wrapView];
                float dx = touch0.x-center.x,dy=touch0.y-center.y;
                double curRot = atan2(dy, dx);
                double diffRot = curRot-startRot;
                Eigen::AngleAxisd rotQuat(-diffRot,axis);
                newRotQuat = newRotQuat * rotQuat;
                
                globeView->setRotQuat(newRotQuat, true);
            }
        }
			break;
        case UIGestureRecognizerStateFailed:
        case UIGestureRecognizerStateCancelled:
        case UIGestureRecognizerStateEnded:
//            NSLog(@"Rotate ended");
            [[NSNotificationCenter defaultCenter] postNotificationName:kRotateDelegateDidEnd object:globeView->tag];
            valid = false;
            break;
        default:
            break;
	}
}

@end
