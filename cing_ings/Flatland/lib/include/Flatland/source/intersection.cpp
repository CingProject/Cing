// Summary: Calls appropriate Test and Find methods for all shape-shape combinations.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see bsd-license.txt

#include <flatland/flatland.hpp>
#include <flatland/shapes.hpp>
#include <flatland/intersection.hpp>

bool Flatland::Intersection::Test(const Geometry &g1, const Geometry &g2)
{
    if (!g1.GetBounds().intersects(g2.GetBounds())) // TODO shouldn't be a member func; should go in intersection.h
        return false;

    typedef bool TestFunction(const Geometry &g1, const Geometry &g2);
    static TestFunction *table[Shape::Count][Shape::Count] =
    {
        TestQuadQuad, TestQuadQuad, TestQuadCircle, TestQuadTerrain, TestQuadComposite,
        TestQuadQuad, TestQuadQuad, TestQuadCircle, TestQuadTerrain, TestQuadComposite,
        TestCircleQuad, TestCircleQuad, TestCircleCircle, TestCircleTerrain, TestCircleComposite,
        TestTerrainQuad, TestTerrainQuad, TestTerrainCircle, TestTerrainTerrain, TestTerrainComposite,
        TestCompositeQuad, TestCompositeQuad, TestCompositeCircle, TestCompositeTerrain, TestCompositeComposite
    };

    return table[g1.GetShape()][g2.GetShape()](g1, g2);
}

void Flatland::Intersection::Find(const Geometry &g1, const Geometry &g2, ContactList &contacts)
{
    if (!g1.GetBounds().intersects(g2.GetBounds()))
        return;

    typedef void FindFunction(const Geometry &g1, const Geometry &g2, ContactList &contacts);
    static FindFunction *table[Shape::Count][Shape::Count] =
    {
        FindQuadQuad, FindQuadQuad, FindQuadCircle, FindQuadTerrain, FindQuadComposite,
        FindQuadQuad, FindQuadQuad, FindQuadCircle, FindQuadTerrain, FindQuadComposite,
        FindCircleQuad, FindCircleQuad, FindCircleCircle, FindCircleTerrain, FindCircleComposite,
        FindTerrainQuad, FindTerrainQuad, FindTerrainCircle, FindTerrainTerrain, FindTerrainComposite,
        FindCompositeQuad, FindCompositeQuad, FindCompositeCircle, FindCompositeTerrain, FindCompositeComposite
    };

    table[g1.GetShape()][g2.GetShape()](g1, g2, contacts);
}
