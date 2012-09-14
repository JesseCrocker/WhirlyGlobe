/*
 *  SphericalMercator.h
 *  WhirlyGlobeLib
 *
 *  Created by Steve Gifford on 4/19/12.
 *  Copyright 2011-2012 mousebird consulting
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
#import "CoordSystem.h"

namespace WhirlyKit 
{

/** The Mercator Projection, bane of cartographers everywhere.
 It stretches out the world in a familiar way, making the US
 look almost as big as our collective ego.  And Greenland.  For some reason.
 */
class SphericalMercatorCoordSystem : public WhirlyKit::CoordSystem
{
public:
    /// Construct with an optional origin for the projection in radians
    /// The equator is default
    SphericalMercatorCoordSystem(float originLon=0.0);
    
    /// Convert from the local coordinate system to lat/lon
    GeoCoord localToGeographic(Point3f);
    /// Convert from lat/lon t the local coordinate system
    Point3f geographicToLocal(GeoCoord);
    
    /// Convert from the local coordinate system to geocentric
    Point3f localToGeocentric(Point3f);
    /// Convert from display coordinates to geocentric
    Point3f geocentricToLocal(Point3f);
        
protected:
    float originLon;
};

}
