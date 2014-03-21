// Summary: Declares Quad, Wall, Block, Circle, Line, Terrain.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see source/bsd-license.txt

#pragma once
#include <flatland/flatland.hpp>
#include <flatland/vector.hpp>
#include <vector>
#include <list>
#include <map>

namespace Flatland
{
    /// abstract rectangular shape; cannot be instantiated
    class Quad : public Geometry
    {
      public:
        Quad(float width, float height);
        void UpdateBounds();
        void SetMass(Body body, float density) const;
        Shape GetShape() const { return Shape::Quad; }
        virtual float Extent(int i) const { return extent[i]; }
        vec2 EAxis(int i) const { return Axis(i) * extent[i]; } // TODO try caching in UpdateBounds and see if it helps
        const vec2* GetCorners() const { return corners; }
        bool	Contains(const vec2 &v) const;
      protected:
        float extent[2];
        vec2 corners[4];
    };

    /// TODO merge with Quad (after you make a line collider)
    class Block : public Quad
    {
      public:
        Block(vec2 topleft, vec2 bottomright);
        Block(vec2 center, float width, float height);
        Block(float left, float top, float right, float bottom);
    };

    /// finite line segment
    class Line : public Quad
    {
      public:
        Line(vec2 origin, vec2 end);
        vec2 Origin() const { return center - axis * extent[0]; } // TODO this should be cached in static segments
        vec2 End() const { return center + axis * extent[0]; }
    };

    typedef std::vector<vec2> VList;
    typedef std::vector<Line> LineStrip;
    typedef std::map<float, int> SpanMap;

    /// static strip of line segments
    //
    /// Terrains must be built strictly west-to-east; each x value must be greater than the previous x value.
    class Terrain : public Geometry
    {
      public:
        Terrain(const vec2 &start);
        void UpdateBounds();
        void Finalize() { UpdateBounds(); }
        Shape GetShape() const { return Shape::Terrain; }
        bool GetIndexRange(float left, float right, int &lower, int &upper) const;

        // container methods
        typedef VList::const_iterator const_iterator;
        const_iterator begin() const { return vertices.begin(); }
        const_iterator end() const { return vertices.end(); }
        const vec2 &front() const { return vertices.front(); }
        const vec2 &back() const { return vertices.back(); }
        void push_back(const vec2 &v);
        size_t size() const { return lines.size(); }
        bool empty() const { return lines.empty(); }
        Line &operator[](int i) { return lines[i]; }
        const Line &operator[](int i) const { return lines[i]; }

      private:
        vec2 previous;
        LineStrip lines;
        SpanMap spans;
        VList vertices;
    };

    /// ball shape
    class Circle : public Geometry
    {
      public:
        Circle(const vec2 &center, float radius);
        void UpdateBounds();
        void SetMass(Body body, float density) const;
        Shape GetShape() const { return Shape::Circle; }
        float Radius() const { return radius; }
      private:
        float radius;
    };

    typedef std::list<Geometry*> GList;

    /// dynamic shape composed of other shapes
    class Composite : public Geometry
    {
      public:
        Composite(vec2 centroid);
        ~Composite();
        void UpdateBounds();
        void Finalize() { UpdateBounds(); }
        Shape GetShape() const { return Shape::Composite; }
        void SetCenter(const vec2 &center);
        void SetAxis(const vec2 &axis);
        void SetMass(Body body, float density) const;

        // container methods
        typedef GList::const_iterator const_iterator;
        typedef GList::iterator iterator;
        const_iterator begin() const { return geometries.begin(); }
        const_iterator end() const { return geometries.end(); }
        const Geometry &front() const { return *geometries.front(); }
        const Geometry &back() const { return *geometries.back(); }
        void push_back(Geometry *geometry);
        size_t size() const { return geometries.size(); }

      private:
        GList geometries;
    };

	/// dynamic shape composed of other shapes
	class StaticComposite : public Geometry
	{
	public:
		StaticComposite(vec2 centroid);
		~StaticComposite();
		void UpdateBounds();
		void Finalize() { UpdateBounds(); }
		Shape GetShape() const { return Shape::Composite; }
		void SetCenter(const vec2 &center);
		void SetAxis(const vec2 &axis);
		void SetMass(Body body, float density) const;

		// container methods
		typedef GList::const_iterator const_iterator;
		typedef GList::iterator iterator;
		const_iterator begin() const { return geometries.begin(); }
		const_iterator end() const { return geometries.end(); }
		const Geometry &front() const { return *geometries.front(); }
		const Geometry &back() const { return *geometries.back(); }
		void push_back(Geometry *geometry);
		size_t size() const { return geometries.size(); }

	private:
		GList geometries;
	};
}
