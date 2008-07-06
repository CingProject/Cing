// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// OpenSteer Boids
// 
// 09-26-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#include <sstream>
#include <vector>

#include "SimpleVehicle.h"
#include "OpenSteerDemo.h"
#include "Proximity.h"
#include "PlugIn.h"
// graphics
#include "graphics/Box.h"
#include "graphics/GraphicsPrereqs.h"
#include "externLibs/Ogre3d/include/OgreSceneNode.h"

#include "..\Ribbon.h"
// Include names declared in the OpenSteer namespace into the namespaces to search to find names.
using namespace OpenSteer;

// ----------------------------------------------------------------------------

typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;

// ----------------------------------------------------------------------------


class Boid : public SimpleVehicle
{
public:

    // type for a flock: an STL vector of Boid pointers
    typedef std::vector<Boid*> groupType;

    // constructor
    Boid (ProximityDatabase& pd)
    {
        // allocate a token for this boid in the proximity database
        proximityToken = NULL;
        newPD (pd);

				// init the mesh
				m_Model.init(1);
				m_Model.setScale(0.5,1.5,0.5);
				m_Life = 0;

				std::vector< Vector > path;
				path.push_back( Vector( 0, 0, 0 ) );

				m_Ribbon = new Ribbon(	100, 
													100, 
													100,
													100,
													1800, 
													3,
													path );
        // reset all boid state
        reset ();
    }


    // destructor
    ~Boid ()
    {
        // delete this boid's token in the proximity database
        delete proximityToken;
    }


    // reset state
    void reset (void)
    {
        // reset the vehicle
        SimpleVehicle::reset ();
				// steering force is clipped to this magnitude
				setMaxForce (12);
				// velocity is clipped to this magnitude
				setMaxSpeed (17.2);
        // initial slow speed
        setSpeed (maxSpeed() * 0.3f);
        // notify proximity database that our position has changed
        proximityToken->updateForNewPosition (position());
    }


    // draw this boid into the scene
		void draw (void)
		{
			float k = 10.0f; //scale

			// Update selected vehicle position

			Vec3 position = this->position();

			float x = position.x * k;
			float y = position.y * k;
			float z = position.z * k;

			m_Model.setPosition( x,y,z );

			// and orientation
			Ogre::Vector3	forwDir	= Ogre::Vector3(	forward().x,
																							forward().y,
																							forward().z);

			m_Model.getSceneNode()->setDirection(forwDir, Ogre::SceneNode::TS_WORLD, Ogre::Vector3::UNIT_Y);  
		
			//update ribbon trail
			m_Ribbon->setPosition(x, y, z);
			//m_Ribbon->update();
		}

    // per frame simulation update
    void update (const float currentTime, const float elapsedTime)
    {
				// Eliminates y component to maintain boids in a plane
				Vec3 force = steerToFlock ();
				//force.y = 0;
        // steer to flock and perhaps to stay within the spherical boundary
        applySteeringForce ( (const Vec3) force + handleBoundary(), elapsedTime);

        // notify proximity database that our position has changed
        proximityToken->updateForNewPosition (position());
				
				m_Life += 1;
    }


    // basic flocking
    Vec3 steerToFlock (void)
    {
				
        const float separationRadius =  30.0f;
        const float separationAngle  = -0.707f;
        const float separationWeight =  6.01f;

        const float alignmentRadius = 35.5f;
        const float alignmentAngle  = -0.3f;
        const float alignmentWeight = 4.0f;

        const float cohesionRadius = 60.0f;
        const float cohesionAngle  = -0.15f;
        const float cohesionWeight = 5.0f;

				const float wanderWeight = 5.0f;

        const float maxRadius = maxXXX (separationRadius,
                                        maxXXX (alignmentRadius,
                                                cohesionRadius));

        // find all flockmates within maxRadius using proximity database
        neighbors.clear();
        proximityToken->findNeighbors (position(), maxRadius, neighbors);

        // determine each of the three component behaviors of flocking
        const Vec3 separation = steerForSeparation (separationRadius,
                                                    separationAngle,
                                                    neighbors);
        const Vec3 alignment  = steerForAlignment  (alignmentRadius,
                                                    alignmentAngle,
                                                    neighbors);
        const Vec3 cohesion   = steerForCohesion   (cohesionRadius,
                                                    cohesionAngle,
                                                    neighbors);

				const Vec3 wander     = steerForWander(0.01);

        // apply weights to components (save in variables for annotation)
        const Vec3 separationW = separation * separationWeight;
        const Vec3 alignmentW  = alignment  * alignmentWeight;
        const Vec3 cohesionW   = cohesion   * cohesionWeight;
        const Vec3 wanderW     = wander     * wanderWeight;

        return separationW + alignmentW + cohesionW + wanderW;
    }


    // Take action to stay within spherical boundary.  Returns steering
    // value (which is normally zero) and may take other side-effecting
    // actions such as kinematically changing the Boid's position.
    Vec3 handleBoundary (void)
    {
        // while inside the sphere do noting
        if (position().length() < worldRadius) return Vec3::zero;

        // once outside, select strategy
        switch (boundaryCondition)
        {
        case 0:
            {
                // steer back when outside
                const Vec3 seek = xxxsteerForSeek (Vec3::zero);
                const Vec3 lateral = seek.perpendicularComponent (forward ());
                return lateral;
            }
        case 1:
            {
                // wrap around (teleport)
                setPosition (position().sphericalWrapAround (Vec3::zero,
                                                             worldRadius));
                return Vec3::zero;
            }
        }
        return Vec3::zero; // should not reach here
    }

    // make boids "bank" as they fly
    void regenerateLocalSpace (const Vec3& newVelocity,
                               const float elapsedTime)
    {
        regenerateLocalSpaceForBanking (newVelocity, elapsedTime);
    }

    // switch to new proximity database -- just for demo purposes
    void newPD (ProximityDatabase& pd)
    {
        // delete this boid's token in the old proximity database
        delete proximityToken;

        // allocate a token for this boid in the proximity database
        proximityToken = pd.allocateToken (this);
    }

    // cycle through various boundary conditions
    static void nextBoundaryCondition (void)
    {
        const int max = 2;
        boundaryCondition = (boundaryCondition + 1) % max;
    }

    static int boundaryCondition;

    // a pointer to this boid's interface object for the proximity database
    ProximityToken* proximityToken;

    // allocate one and share amoung instances just to save memory usage
    // (change to per-instance allocation to be more MP-safe)
    static AVGroup neighbors;

    static float worldRadius;

		// graphics
		Graphics::Box m_Model;
		Ribbon*			m_Ribbon;

		// life time
		int m_Life;

};


AVGroup Boid::neighbors;
float Boid::worldRadius = 400;
int Boid::boundaryCondition = 0;


// ----------------------------------------------------------------------------
// PlugIn for OpenSteerDemo

class BoidsPlugIn : public PlugIn
{
public:
    
    const char* name (void) {return "Boids";}

    float selectionOrderSortKey (void) {return 0.03f;}

    virtual ~BoidsPlugIn() {} // be more "nice" to avoid a compiler warning

    void open (void)
    {
        // make the database used to accelerate proximity queries
        cyclePD = -1;
        nextPD ();

        // make default-sized flock
        population = 0;
				for (int i = 0; i < 60; i++) addBoidToFlock (RandomUnitVector().x,
					RandomUnitVector().y,RandomUnitVector().z);
    }

    void update (const float currentTime, const float elapsedTime)
    {
			const static int iterations = 6;
			for (int s = 0; s < iterations ; s++)
			{
				// update flock simulation for each boid
				for (iterator i = flock.begin(); i != flock.end(); i++)
				{
					(**i).update (currentTime, elapsedTime);
				}	
			}

    }

    void redraw (const float currentTime, const float elapsedTime)
    {

      // draw each boid in flock
			for (iterator i = flock.begin(); i != flock.end(); i++)
			{
				(**i).draw ();
			}
    }

    void close (void)
    {
        // delete each member of the flock
        while (population > 0) removeBoidFromFlock ();

        // delete the proximity database
        delete pd;
        pd = NULL;
    }

    void reset (void)
    {
        // reset each boid in flock
        for (iterator i = flock.begin(); i != flock.end(); i++) (**i).reset();

    }

    // for purposes of demonstration, allow cycling through various
    // types of proximity databases.  this routine is called when the
    // OpenSteerDemo user pushes a function key.
    void nextPD (void)
    {
        // save pointer to old PD
        ProximityDatabase* oldPD = pd;

        // allocate new PD
        const int totalPD = 2;
        switch (cyclePD = (cyclePD + 1) % totalPD)
        {
        case 0:
            {
                const Vec3 center;
                const float div = 10.0f;
                const Vec3 divisions (div, div, div);
                const float diameter = Boid::worldRadius * 1.1f * 2;
                const Vec3 dimensions (diameter, diameter, diameter);
                typedef LQProximityDatabase<AbstractVehicle*> LQPDAV;
                pd = new LQPDAV (center, dimensions, divisions);
                break;
            }
        case 1:
            {
                pd = new BruteForceProximityDatabase<AbstractVehicle*> ();
                break;
            }
        }

        // switch each boid to new PD
        for (iterator i=flock.begin(); i!=flock.end(); i++) (**i).newPD(*pd);

        // delete old PD (if any)
        delete oldPD;
    }

    void handleFunctionKeys (int keyNumber)
    {
        switch (keyNumber)
        {
        case 1:  addBoidToFlock ();               break;
        case 2:  removeBoidFromFlock ();          break;
        case 3:  nextPD ();                       break;
        case 4:  Boid::nextBoundaryCondition ();  break;
        }
    }

    void printMiniHelpForFunctionKeys (void)
    {
        std::ostringstream message;
        message << "Function keys handled by ";
        message << '"' << name() << '"' << ':' << std::ends;
        OpenSteerDemo::printMessage (message);
        OpenSteerDemo::printMessage ("  F1     add a boid to the flock.");
        OpenSteerDemo::printMessage ("  F2     remove a boid from the flock.");
        OpenSteerDemo::printMessage ("  F3     use next proximity database.");
        OpenSteerDemo::printMessage ("  F4     next flock boundary condition.");
        OpenSteerDemo::printMessage ("");
    }

    void addBoidToFlock (void)
    {
        population++;
        Boid* boid = new Boid (*pd);
        flock.push_back (boid);
    }

		void addBoidToFlock (float x, float y, float z)
		{
			population++;
			Boid* boid = new Boid (*pd);
			boid->setPosition( x, y, z );
			boid->setForward(0,0,-1);
			boid->setSpeed(y);
			flock.push_back (boid);
		}

    void removeBoidFromFlock (void)
    {
        if (population > 0)
        {
            // save a pointer to the last boid, then remove it from the flock
            const Boid* boid = flock.back();

            flock.pop_back();
            population--;

            // delete the Boid
            delete boid;
        }
    }

    // return an AVGroup containing each boid of the flock
    const AVGroup& allVehicles (void) {return (const AVGroup&)flock;}

    // flock: a group (STL vector) of pointers to all boids
    Boid::groupType flock;
    typedef Boid::groupType::const_iterator iterator;

    // pointer to database used to accelerate proximity queries
    ProximityDatabase* pd;

    // keep track of current flock size
    int population;

    // which of the various proximity databases is currently in use
    int cyclePD;
};

BoidsPlugIn gBoidsPlugIn;

// ----------------------------------------------------------------------------
