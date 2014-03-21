// Summary: Define a small class for 2D axis-aligned bounding boxes.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see source/bsd-license.txt

#pragma once

namespace Flatland
{
    /// axis-aligned bounding box in 2D
    struct aabb
    {
        aabb() {}
        aabb(float left, float top, float right, float bottom) :
            left(left), top(top), right(right), bottom(bottom) {}

        bool intersects(const aabb &x) const
        {
            return ((left < x.right && x.left < right) && (top < x.bottom && x.top < bottom)); 
        }

        float left;
        float top;
        float right;
        float bottom;
    };
}
