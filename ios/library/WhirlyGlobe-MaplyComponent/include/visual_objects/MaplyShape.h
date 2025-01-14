/*
 *  MaplyShape.h
 *  WhirlyGlobe-MaplyComponent
 *
 *  Created by Steve Gifford on 9/28/12.
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
#import "math/MaplyCoordinate.h"
#import "math/MaplyMatrix.h"
#import "visual_objects/MaplyTexture.h"

/** 
    Maply Shape is the base class for the actual shape objects.
    
    The maply shape is just the base class.  Look to MaplyShapeCircle, MaplyShapeCylinder, MaplyShapeSphere, MaplyShapeGreatCircle, and MaplyShapeLinear.
  */
@interface MaplyShape : NSObject

/** 
    The color of the shape.
    
    We can set object color in the NSDictionary passed in with the add method.  We can also override that here.
 */
@property (nonatomic,strong) UIColor * __nullable color;

/** 
    If set, the object is selectable
 
    The object is selectable if this is set when the object is passed in to an add call.  If not set, you'll never see it in selection.
 */
@property (nonatomic,assign) bool selectable;

/** 
    User data object for selection
 
    When the user selects a feature and the developer gets it in their delegate, this is an object they can use to figure out what the shape means to them.
 */
@property (nonatomic,strong) id  __nullable userObject;

/** 
    If set, this shape is in clip coordinates and will not be transformed.
    
    Some objects (the rectangle) can be used as overlays in clip coordinates.  This is set if that's the case.
  */
@property (nonatomic,assign) bool clipCoords;

@end

/** 
    Shows a circle at the given location on the globe or map.
    
    This object represents a circle at the given geographic location.  It needs a radius (in display coordinates) and can optionally have a height above the globe or map.
 */
@interface MaplyShapeCircle : MaplyShape

/** 
    Center of the circle in local coordinates.
    
    This is the center of the circle in geographic.
  */
@property (nonatomic,assign) MaplyCoordinate center;

/** 
    Radius of the circle in display units.
    
    This is the radius of the circle, but not in geographic.  It's in display units.  Display units for the globe are based on a radius of 1.0.  Scale accordingly.  For the map, display units typically run from -PI to +PI, depending on the coordinate system.
  */
@property (nonatomic,assign) float radius;

/** 
    Offset height above the globe in display units.
    
    This is the height above the globe for the center of the circle.  It's also in display units, like the radius.
  */
@property (nonatomic,assign) float height;

@end

typedef MaplyShapeCircle WGShapeCircle;

/** 
    Display a sphere at the given location with the given radius.
    
    This object represents a sphere at the
  */
@interface MaplyShapeSphere : MaplyShape

/** 
    Center of the sphere in local coordinates.
    
    The x and y coordinates correspond to longitude and latitude and are in geographic (radians).  The Z value is in display coordinates.  For that globe that's based on a radius of 1.0.  Scale accordingly.
 */
@property (nonatomic,assign) MaplyCoordinate center;

/** 
    Radius of the sphere in display units.
    
    This is the radius of the sphere, but not in geographic.  It's in display units.  Display units for the globe are based on a radius of 1.0.  Scale accordingly.  For the map, display units typically run from -PI to +PI, depending on the coordinate system.
 */
@property (nonatomic,assign) float radius;

/** 
    Offset height above the globe in display units.
    
    This is the height above the globe for the center of the sphere.  It's also in display units, like the radius.
 */
@property (nonatomic,assign) float height;

@end

typedef MaplyShapeSphere WGShapeSphere;

/** 
    Represent a cyclinder on the globe or map.
    
    This object represents a cylinder with it's base tied to the surface of the globe or map and it's top pointed outward (on the globe anyway).  The base can be offset and the overall radius and height are adjustable.
  */
@interface MaplyShapeCylinder : MaplyShape

/** 
    Center of the cylinder's base in geographic.
    
    The x and y coordinates correspond to longitude and latitude and are in geographic (radians).
 */
@property (nonatomic,assign) MaplyCoordinate baseCenter;

/** 
    Base height above the globe in display units.
    
    This is an optional base offset from the globe or map.  The cylinder will be offset by this amount.  It's also in display units, like the radius.
 */
@property (nonatomic,assign) float baseHeight;

/** 
    Radius of the cylinder in display units.
    
    This is the radius of the cylinder, but not in geographic.  It's in display units.  Display units for the globe are based on a radius of 1.0.  Scale accordingly.  For the map, display units typically run from -PI to +PI, depending on the coordinate system.
 */
@property (nonatomic,assign) float radius;

/** 
    Height of the cylinder in display units.
    
    This is the height of the cylinder.  The top of the cylinder will be at baseHeight+height.  It's also in display units, like the radius.
 */
@property (nonatomic,assign) float height;

@end

typedef MaplyShapeCylinder WGShapeCylinder;

/** 
    Represents an great circle or great circle with height.
    
    Great circles are the shortest distance between two points on a globe.  We extend that a bit here, by adding height.  The result is a curved object that can either sit on top of the globe or rise above it.  In either case it begins and ends at the specified points on the globe.
 */
@interface MaplyShapeGreatCircle : MaplyShape

/// Starting point in geographic coordinates.
@property (nonatomic,assign) MaplyCoordinate startPt;

/// End point in geographic coordinates
@property (nonatomic,assign) MaplyCoordinate endPt;

/** 
    Height of the great circle shape right in its middle.
    
    This is the height of the great circle right in the middle.  It will built toward that height and then go back down as it reaches the endPt.  The height is in display units.  For the globe that's based on a radius of 1.0.
 */
@property (nonatomic,assign) float height;

/** 
    Line width for the great circle geometry.
    
    The great circle is implemented as a set of lines. This is the width, in pixels, of those lines.
  */
@property (nonatomic,assign) float lineWidth;

/** 
    Angle between start and end points in radians
  */
- (float)calcAngleBetween;

@end

/** 
    Represents a simple rectangle in 3D.
    
    The rectangle is a 2D object in 3D.  Specify the lower left and upper right coordinates as
    well as an optional texture.
  */
@interface MaplyShapeRectangle : MaplyShape

/// Lower left corner in 3D
@property (nonatomic,assign) MaplyCoordinate3dD ll;

/// Upper right corner in 3D
@property (nonatomic,assign) MaplyCoordinate3dD ur;

/// If set, the textures to stretch across the rectangle.
@property (nonatomic,nullable,strong) NSMutableArray *textures;

/// Add a texture to stretch across the rectangle
- (void)addTexture:(MaplyTexture * __nonnull)texture;

@end

/** 
    A linear feature offset from the globe.
    
    The main difference between this object and a regular MaplyVectorObject is that you specify coordiantes in 3D.  You can use this to create linear features that are offset from the globe.
  */
@interface MaplyShapeLinear : MaplyShape

/** 
    Line width in pixels
    
    The linear is implemented as a set of line segments in OpenGL ES.  This is their line width in pixels.
  */
@property (nonatomic,assign) float lineWidth;

/** 
    Initialize with coordinates and coordinate array size
    
    This initializer will make a copy of the coordinates and use them to draw the lines.  The x and y values are in geographic.  The z values are offsets from the globe (or map) and are in display units.  For the globe display units are based on a radius of 1.0.
  */
- (nullable instancetype)initWithCoords:(MaplyCoordinate3d * __nonnull)coords numCoords:(int)numCoords;

/** 
    Return the coordinates for this linear feature.
    
    @return Returns the number of coordinates and a pointer to the coordinate array.
  */
- (int)getCoords:(MaplyCoordinate3d *__nullable *__nonnull)retCoords;

@end

/** 
    An extruded shape with an arbitrary outline.
    
    This object represents an extruded shape with the given thickness.  It can be oriented according to the pitch, roll, yaw and height.
  */
@interface MaplyShapeExtruded : MaplyShape

/** 
    Construct with the coordinates for the outline to extrude.
 
    Pass in pairs of doubles that correspond to the
 */
- (nonnull instancetype)initWithOutline:(NSArray * __nonnull)coords;

/** 
    Construct with the coordinates for the outline to extrude.
    
    Pass in pairs of doubles that correspond to the
  */
- (nonnull instancetype)initWithOutline:(double * __nonnull)coords numCoordPairs:(int)numCoordPairs;

/** 
    Number of coordinate pairs in this shape.
  */
@property (nonatomic,readonly) int numCoordPairs;

/** 
    Array of coordinate values.
  */
@property (nonatomic,readonly) double * __nullable coordData;

/** 
    Where we'd like to place the extruded shape.
    
    This is the center of the object in geographic radians.
  */
@property (nonatomic) MaplyCoordinate center;

/** 
    Scale for coordinates.  Set to 1/EarthRadiusInMeters by default.
    
    The coordinates will be scaled by this before creating the geometry.  By default this is set to 1/EarthRadius(m) so you can build your shape in meters.  Also applies to thickness and height.
  */
@property (nonatomic,assign) double scale;

/** 
    Thickness for the resulting shape.
    
    We build an extruded shape out of this information and this is its thickness.  If this is zero, you just get a double sided polygon.
  */
@property (nonatomic,assign) double thickness;

/** 
    Height to place the resulting shape at.
    
    We'll put this shape at the given height above the surface of the globe or map.
  */
@property (nonatomic,assign) double height;

/** 
    The transform to apply to this shape.
    
    If set, this transform is applied before placing the feature.  You can set a transform matrix up with roll, pitch, and yaw.
  */
@property (nonatomic,strong) MaplyMatrix * __nullable transform;

@end
