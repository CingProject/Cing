// Summary: Declares a geometry class and world class.  These are wrapper interfaces to ODE.
// Copyright: 2007  Philip Rideout.  All rights reserved.
// License: see source/bsd-license.txt

#pragma once
#include <flatland/ode/ode.h>
#include <flatland/ode/source/objects.h>
#include <flatland/aabb.hpp>
#include <flatland/vector.hpp>
#include <flatland/enums.hpp>
#include <flatland/intersection.hpp>
#include <cassert>
#include <stack>

/// Everything in the 2D collision library goes in the "Flatland" namespace.
namespace Flatland
{
    class ContactList;
    class Object;

    typedef unsigned int Mask;
    typedef dBodyID Body;
    typedef void (*Callback)(ContactList &contacts);

    /// Abstract 2D geometry class used for collision detection.
    class Geometry
    {
      public:
        Geometry();
        virtual void UpdateBounds() = 0;
        virtual void SetCenter(const vec2 &center);
        vec2 Center() const { return center; }
        virtual void SetAxis(const vec2 &axis);
        void Rotate(float theta);
        void Rotate(const vec2 &xform);
        vec2 Axis() const { return axis; }
        vec2 Axis(int i) const { return i ? axis.perp() : axis; }
        virtual void SetMass(Body body, float density) const {}
        virtual Shape GetShape() const = 0;
        const aabb &GetBounds() const { return bounds; }
      protected:
        aabb bounds;
        vec2 axis;
        vec2 center;
    };

    /// \note
    /// For physical properties, I don't see a need for accessor/mutator methods right now.
    /// In the future we can add proxies if need be.  For example, we'd turn the struct into a class,
    /// then we'd make friction a FrictionProxy instead of a float, then we'd add a private member
    /// to store the actual float for friction. FrictionProxy would use operator overloading
    /// to hide validation behind the scenes.  If the proxy code is tedious even using templates,
    /// then maybe we could add to the enums perl script to auto-generate code for us.

    /// Physical properties.
    struct ObjectProperties
    {
        float density;
        float friction;
        float bounceFactor;
        float bounceVelocity;
        Mask collisionMask;
        Mask frictionMask;
        Callback callback;
    };

    /// Abstract Flatland object to be overridden by the application.
    //
    /// Flatland::Object is an abstract class which can be dynamic or static.
    ///  - Dynamic objects use position/orientation information stored in an ODE body.
    ///  - Static objects have their own position/orientation information.  They cannot be moved by ODE.
    ///  - Dynamic and static objects both have geometry, friction, and bounciness.
    class Object
    {
      public:
        Object();
        virtual ~Object() {}
        virtual Geometry &GetGeometry() = 0;
        virtual const Geometry &GetGeometry() const = 0;
        virtual bool IsDynamic() const = 0;
        virtual Body GetBody() = 0;
        virtual void SetMass(float density) = 0;
        void Move();
        void Rotate(float theta);
        void SetCenter(const vec2 &center);
      public:
        ObjectProperties &Property() { return properties; }
        const ObjectProperties &Property() const { return properties; }
        static ObjectProperties &Default() { return defaults; }
        static void PushProperties() { defaultStack.push(defaults); }
        static void PopProperties() { defaults = defaultStack.top(); defaultStack.pop(); }
      private:
        ObjectProperties properties;
        static ObjectProperties defaults;
        static std::stack<ObjectProperties> defaultStack;
    };

    /// Dynamic object.
    template<class G>
    class Dynamic : public Object
    {
      public:
        Dynamic(G *geometry, Body b);
        virtual ~Dynamic() { dBodyDestroy(body); delete geometry; }
        G &GetGeometry() { return *geometry; }
        const G &GetGeometry() const { return *geometry; }
        bool IsDynamic() const { return true; }
        Body GetBody() { return body; }
        void SetMass(float density) { geometry->SetMass(body, density); }
      protected:
        G *geometry;
        Body body;
    };

    /// Static object (cannot be moved by the physics engine)
    template<class G>
    class Static : public Object
    {
      public:
        Static(G *geometry) : geometry(geometry) {}
        virtual ~Static() { delete geometry; }
        G &GetGeometry() { return *geometry; }
        const G &GetGeometry() const { return *geometry; }
        bool IsDynamic() const { return false; }
        Body GetBody() { return 0; }
        void SetMass(float) {}
      protected:
        G *geometry;
    };

    /// Encapsulates an array of contacts between two geometries.
    class ContactList
    {
      public:
        ContactList();
        void Reset(Object *o1, Object *o2);
        Object *Self() { return o1; }
        Object *Other() { return o2; }
        void ToggleNormalInversion() { invertNormals = !invertNormals; }
        void AddContact(const vec2 &position, const vec2 &normal, float depth);
        void Finalize();
        void CreateJoints(dWorldID world, dJointGroupID contactGroup) const;
        int Count() const { return count; }
        static const int Max = 64;
      private:
        Object *o1;
        Object *o2;
        dContact contacts[Max];
        int count;
        bool invertNormals;
    };

    /// Encapsulates an ODE world and contact group.
    class World
    {
      public:
        World();
        ~World();
        void QuickStep(float);
        Body BodyCreate();
        template<class S> void GenerateContacts(const S &space);
        template<class S> bool IsCorrupt(const S &space) const;
        int ContactCount() const { return contactCount; }
        void SetCFM(float);
        void SetAutoDisableFlag(bool);
        void SetERP(float);
        void SetContactMaxCorrectingVel(float);
        void SetContactSurfaceLayer(float);
        void SetAutoDisableLinearThreshold(float);
        void SetAutoDisableAngularThreshold(float);
        void SetGravity(const vec2&);
        dJointID AddMotor(Object &object);
        dJointID Glue(Object &object1, Object &object2);
        dJointID AnchorAxis(Object &object, const vec2 &axis);
        dJointID Anchor(Object &o1, Object &o2, const vec2 &point, float mu, float erp);
        dJointID Anchor(Object &o1, const vec2 &point, float mu, float erp);
        void DeleteJoint(dJointID joint);
        static void SetMotorVelocity(dJointID joint, float velocity);
        static float GetMotorVelocity(dJointID joint);
      private:
        int contactCount;
        dWorldID world;
        dJointGroupID contactGroup;
        ContactList contactList;
    };

    /// Contact generator for 2D geometry.
    //
    /// GenerateContacts is a template function that takes any object that acts
    /// like an STL container of pointers.
    /// It generates all appropriate ODE joints for simulating 2D collisions.
    /// The given container does not need to be an actual STL container; it only needs to have these properties:
    ///  - Container::const_iterator
    ///  - Container::begin()
    ///  - Container::end()
    /// The objects in the container must have a GetFlatlandObject() method.
    template<class Container>
    void World::GenerateContacts(const Container &space)
    {
        dJointGroupEmpty(contactGroup);
        contactCount = 0;
        typename Container::const_iterator o1;
        for (o1 = space.begin(); o1 != space.end(); ++o1)
        {
            Object *object1 = (*o1)->GetFlatlandObject();
            if (!object1)
                continue;

            typename Container::const_iterator o2 = o1;
            for (++o2; o2 != space.end(); ++o2)
            {
                Object *object2 = (*o2)->GetFlatlandObject();
                if (!object2)
                    continue;

                if (!object1->IsDynamic() && !object2->IsDynamic())
                    continue;

                if (!(object1->Property().collisionMask & object2->Property().collisionMask))
                    if (!object1->Property().callback && !object2->Property().callback)
                        continue;

                Geometry &g1 = object1->GetGeometry();
                Geometry &g2 = object2->GetGeometry();

                if (Intersection::Test(g1, g2))
                {
                    contactList.Reset(object1, object2);
                    Intersection::Find(g1, g2, contactList);
                    contactList.Finalize();
                    if ((object1->Property().collisionMask & object2->Property().collisionMask))
                    {
                        contactList.CreateJoints(world, contactGroup);
                        contactCount += contactList.Count();
                    }
                }
            }
        }
    }

    /// Corruption detection in a scene.
    //
    /// IsCorrupt is a template function that takes any object that acts
    /// like an STL container of pointers.
    /// The given container does not need to be an actual STL container; it only needs to have these properties:
    ///  - Container::const_iterator
    ///  - Container::begin()
    ///  - Container::end()
    /// The objects in the container must have a GetFlatlandObject() method.
    template<class Container>
    bool World::IsCorrupt(const Container &space) const
    {
        typename Container::const_iterator o;
        for (o = space.begin(); o != space.end(); ++o)
        {
            Object *object = (*o)->GetFlatlandObject();
            if (!object)
                continue;

            Body body = object->GetBody();
            if (!body)
                continue;

            const dReal *lvel = dBodyGetLinearVel(body);
            const dReal *avel = dBodyGetAngularVel(body);

            if (is_nan(lvel[0])) return true;
            if (is_nan(lvel[1])) return true;
            if (is_nan(avel[0])) return true;
            if (is_nan(avel[1])) return true;
        }
        return false;
    }


    template<class G>
    Dynamic<G>::Dynamic(G *g, Body b) : geometry(g), body(b)
    {
        body->geom = this;

        dMatrix3 R;
        float theta = atan2f(geometry->Axis(0).y, geometry->Axis(0).x); // TODO inefficient
        dRFromAxisAndAngle(R, 0, 0, 1, theta);
        dBodyInit(body, geometry->Center().x, geometry->Center().y, R);

        SetMass(Object::Default().density);
    }
}

typedef Flatland::Object *dGeomID;
void dGeomMoved(dGeomID g);
dGeomID dGeomGetBodyNext(dGeomID);
void dGeomSetBody(dGeomID g, dBodyID b);
