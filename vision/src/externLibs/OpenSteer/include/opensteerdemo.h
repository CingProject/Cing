// ----------------------------------------------------------------------------
//
//
// OpenSteerDemo
//
// This class encapsulates the state of the OpenSteerDemo application and
// the services it provides to its plug-ins
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: recast App class as OpenSteerDemo 
// 06-26-02 cwr: App class created 
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_OPENSTEERDEMO_H
#define OPENSTEER_OPENSTEERDEMO_H

#include "externLibs/OpenSteer/include/Vec3.h"
#include "externLibs/OpenSteer/include/Clock.h"
#include "externLibs/OpenSteer/include/PlugIn.h"
#include "externLibs/OpenSteer/include/Utilities.h"

namespace OpenSteer {


    class OpenSteerDemo
    {
    public:
        // ------------------------------------------------------ component objects

        // clock keeps track of both "real time" and "simulation time"
        static Clock clock;

        // ------------------------------------------ addresses of selected objects

        // currently selected plug-in (user can choose or cycle through them)
        static PlugIn* selectedPlugIn;

        // -------------------------------------------- initialize, update and exit

        // initialize OpenSteerDemo
        static void initialize (void);

        // main update function: step simulation forward and redraw scene
        static void updateSimulationAndRedraw (void);

        // exit OpenSteerDemo with a given text message or error code
        static void errorExit (const char* message);
        static void exit (int exitCode);

        // ------------------------------------------------------- PlugIn interface

        // select the default PlugIn
        static void selectDefaultPlugIn (void);
        
        // select the "next" plug-in, cycling through "plug-in selection order"
        static void selectNextPlugIn (void);

        // handle function keys an a per-plug-in basis
        static void functionKeyForPlugIn (int keyNumber);

        // return name of currently selected plug-in
        static const char* nameOfSelectedPlugIn (void);

        // open the currently selected plug-in
        static void openSelectedPlugIn (void);

        // do a simulation update for the currently selected plug-in
        static void updateSelectedPlugIn (const float currentTime,
                                          const float elapsedTime);

        // redraw graphics for the currently selected plug-in
        
				static void redrawSelectedPlugIn (const float currentTime,
                                          const float elapsedTime);
																					

        // close the currently selected plug-in
        static void closeSelectedPlugIn (void);

        // reset the currently selected plug-in
        static void resetSelectedPlugIn (void);

        static const AVGroup& allVehiclesOfSelectedPlugIn(void);

        // ---------------------------------------------------- OpenSteerDemo phase
        static bool  phaseIsDraw     (void) {return phase == drawPhase;}
        static bool  phaseIsUpdate   (void) {return phase == updatePhase;}
        static bool  phaseIsOverhead (void) {return phase == overheadPhase;}

        static float phaseTimerDraw     (void) {return phaseTimers[drawPhase];}
        static float phaseTimerUpdate   (void) {return phaseTimers[updatePhase];}

        // XXX get around shortcomings in current implementation, see note
        // XXX in updateSimulationAndRedraw
        //static float phaseTimerOverhead(void){return phaseTimers[overheadPhase];}
        static float phaseTimerOverhead (void)
        {
            return (clock.getElapsedRealTime() -
                    (phaseTimerDraw() + phaseTimerUpdate()));
        }
        // ----------------------------------------------------------- console text

        // print a line on the console with "OpenSteerDemo: " then the given ending
        static void printMessage (const char* message);
        static void printMessage (const std::ostringstream& message);

        // like printMessage but prefix is "OpenSteerDemo: Warning: "
        static void printWarning (const char* message);
        static void printWarning (const std::ostringstream& message);

        // print list of known commands
        static void keyboardMiniHelp (void);


        // ---------------------------------------------------------------- private

    private:
        static bool enableAnnotation;

        static int phase;
        static int phaseStack[];
        static int phaseStackIndex;
        static float phaseTimers[];
        static float phaseTimerBase;
        static const int phaseStackSize;
        static void pushPhase (const int newPhase);
        static void popPhase (void);
        static void initPhaseTimers (void);
        static void updatePhaseTimers (void);

        static const int drawPhase;
        static const int updatePhase;
        static const int overheadPhase;
    };

} // namespace OpenSteer
// ----------------------------------------------------------------------------
#endif // OPENSTEER_OPENSTEERDEMO_H
