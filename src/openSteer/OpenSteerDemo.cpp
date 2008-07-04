// ----------------------------------------------------------------------------
//
// OpenSteerDemo
//
// This class encapsulates the state of the OpenSteerDemo application and the
// services it provides to its plug-ins.  It is never instantiated, all of
// its members are static (belong to the class as a whole.)
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 11-14-02 cwr: created 
//
// ----------------------------------------------------------------------------

#include <algorithm>
#include <sstream>
#include "externLibs/OpenSteer/include/opensteerdemo.h"

// keeps track of both "real time" and "simulation time"
OpenSteer::Clock OpenSteer::OpenSteerDemo::clock;

// currently selected plug-in (user can choose or cycle through them)
OpenSteer::PlugIn* OpenSteer::OpenSteerDemo::selectedPlugIn = NULL;

// phase: identifies current phase of the per-frame update cycle
int OpenSteer::OpenSteerDemo::phase = OpenSteer::OpenSteerDemo::overheadPhase;

// XXX apparently MS VC6 cannot handle initialized static const members,
// XXX so they have to be initialized not-inline.
const int OpenSteer::OpenSteerDemo::overheadPhase = 0;
const int OpenSteer::OpenSteerDemo::updatePhase   = 1;
const int OpenSteer::OpenSteerDemo::drawPhase     = 2;

// ----------------------------------------------------------------------------
// initialize OpenSteerDemo application

namespace {

    void printPlugIn (OpenSteer::PlugIn& pi) {std::cout << " " << pi << std::endl;} // XXX

} // anonymous namespace


void OpenSteer::OpenSteerDemo::initialize (void)
{
    // select the default PlugIn
    selectDefaultPlugIn ();

    // initialize the default PlugIn
    openSelectedPlugIn ();
}


// ----------------------------------------------------------------------------
// main update function: step simulation forward and redraw scene

void OpenSteer::OpenSteerDemo::updateSimulationAndRedraw (void)
{
    // update global simulation clock
    clock.update ();

    //  start the phase timer (XXX to accurately measure "overhead" time this
    //  should be in displayFunc, or somehow account for time outside this
    //  routine)
    initPhaseTimers ();

    // run selected PlugIn (with simulation's current time and step size)
    updateSelectedPlugIn (clock.getTotalSimulationTime (),
                          clock.getElapsedSimulationTime ());

    // redraw selected PlugIn (based on real time)
    redrawSelectedPlugIn (clock.getTotalRealTime (),
                          clock.getElapsedRealTime ());
}


// ----------------------------------------------------------------------------
// exit OpenSteerDemo with a given text message or error code

void OpenSteer::OpenSteerDemo::errorExit (const char* message)
{
    printMessage (message);
#ifdef _MSC_VER
	MessageBox(0, message, "OpenSteerDemo Unfortunate Event", MB_ICONERROR);
#endif
    exit (-1);
}


void OpenSteer::OpenSteerDemo::exit (int exitCode)
{
    ::exit (exitCode);
}


// ----------------------------------------------------------------------------
// select the default PlugIn

void OpenSteer::OpenSteerDemo::selectDefaultPlugIn (void)
{
    PlugIn::sortBySelectionOrder ();
    selectedPlugIn = PlugIn::findDefault ();
}


// ----------------------------------------------------------------------------
// select the "next" plug-in, cycling through "plug-in selection order"

void OpenSteer::OpenSteerDemo::selectNextPlugIn (void)
{
    closeSelectedPlugIn ();
    selectedPlugIn = selectedPlugIn->next ();
    openSelectedPlugIn ();
}

// ----------------------------------------------------------------------------
// handle function keys an a per-plug-in basis

void OpenSteer::OpenSteerDemo::functionKeyForPlugIn (int keyNumber)
{
    selectedPlugIn->handleFunctionKeys (keyNumber);
}


// ----------------------------------------------------------------------------
// return name of currently selected plug-in

const char* OpenSteer::OpenSteerDemo::nameOfSelectedPlugIn (void)
{
    return (selectedPlugIn ? selectedPlugIn->name() : "no PlugIn");
}


// ----------------------------------------------------------------------------
// open the currently selected plug-in

void OpenSteer::OpenSteerDemo::openSelectedPlugIn (void)
{
    selectedPlugIn->open ();
}


// ----------------------------------------------------------------------------
// do a simulation update for the currently selected plug-in

void OpenSteer::OpenSteerDemo::updateSelectedPlugIn (const float currentTime,
                                                const float elapsedTime)
{
    // switch to Update phase
    pushPhase (updatePhase);

    // invoke selected PlugIn's Update method
    selectedPlugIn->update (currentTime, elapsedTime);

    // return to previous phase
    popPhase ();
}

// ----------------------------------------------------------------------------
// redraw graphics for the currently selected plug-in

void OpenSteer::OpenSteerDemo::redrawSelectedPlugIn (const float currentTime,
                                                const float elapsedTime)
{
    // switch to Draw phase
    pushPhase (drawPhase);

    // invoke selected PlugIn's Draw method
    selectedPlugIn->redraw (currentTime, elapsedTime);

    // return to previous phase
    popPhase ();
}


// ----------------------------------------------------------------------------
// close the currently selected plug-in

void OpenSteer::OpenSteerDemo::closeSelectedPlugIn (void)
{
    selectedPlugIn->close ();
}


// ----------------------------------------------------------------------------
// reset the currently selected plug-in


void 
OpenSteer::OpenSteerDemo::resetSelectedPlugIn (void)
{
    selectedPlugIn->reset ();
}

// ----------------------------------------------------------------------------
// return a group (an STL vector of AbstractVehicle pointers) of all
// vehicles(/agents/characters) defined by the currently selected PlugIn

const OpenSteer::AVGroup& OpenSteer::OpenSteerDemo::allVehiclesOfSelectedPlugIn (void)
{
    return selectedPlugIn->allVehicles ();
}

// ----------------------------------------------------------------------------

void OpenSteer::OpenSteerDemo::printMessage (const char* message)
{
    std::cout << "OpenSteerDemo: " <<  message << std::endl << std::flush;
}

void OpenSteer::OpenSteerDemo::printMessage (const std::ostringstream& message)
{
    printMessage (message.str().c_str());
}

void OpenSteer::OpenSteerDemo::printWarning (const char* message)
{
    std::cout << "OpenSteerDemo: Warning: " <<  message << std::endl << std::flush;
}


void OpenSteer::OpenSteerDemo::printWarning (const std::ostringstream& message)
{
    printWarning (message.str().c_str());
}

bool OpenSteer::OpenSteerDemo::enableAnnotation = true;

// ------------------------------------------------------------------------
// print list of known commands
//
// XXX this list should be assembled automatically,
// XXX perhaps from a list of "command" objects created at initialization

void OpenSteer::OpenSteerDemo::keyboardMiniHelp (void)
{
    printMessage ("");
    printMessage ("defined single key commands:");
    printMessage ("  r      restart current PlugIn.");
    printMessage ("  s      select next vehicle.");
    printMessage ("  c      select next camera mode.");
    printMessage ("  f      select next preset frame rate");
    printMessage ("  Tab    select next PlugIn.");
    printMessage ("  a      toggle annotation on/off.");
    printMessage ("  Space  toggle between Run and Pause.");
    printMessage ("  ->     step forward one frame.");
    printMessage ("  Esc    exit.");
    printMessage ("");

    // allow PlugIn to print mini help for the function keys it handles
    selectedPlugIn->printMiniHelpForFunctionKeys ();
}

// ----------------------------------------------------------------------------
// manage OpenSteerDemo phase transitions (xxx and maintain phase timers)

int OpenSteer::OpenSteerDemo::phaseStackIndex = 0;
const int OpenSteer::OpenSteerDemo::phaseStackSize = 5;
int OpenSteer::OpenSteerDemo::phaseStack [OpenSteer::OpenSteerDemo::phaseStackSize];


void OpenSteer::OpenSteerDemo::pushPhase (const int newPhase)
{
    // update timer for current (old) phase: add in time since last switch
    updatePhaseTimers ();

    // save old phase
    phaseStack[phaseStackIndex++] = phase;

    // set new phase
    phase = newPhase;

    // check for stack overflow
    if (phaseStackIndex >= phaseStackSize) errorExit ("phaseStack overflow");
}


void OpenSteer::OpenSteerDemo::popPhase (void)
{
    // update timer for current (old) phase: add in time since last switch
    updatePhaseTimers ();

    // restore old phase
    phase = phaseStack[--phaseStackIndex];
}


// ----------------------------------------------------------------------------

float OpenSteer::OpenSteerDemo::phaseTimerBase = 0;
float OpenSteer::OpenSteerDemo::phaseTimers [drawPhase+1];

void OpenSteer::OpenSteerDemo::initPhaseTimers (void)
{
    phaseTimers[drawPhase] = 0;
    phaseTimers[updatePhase] = 0;
    phaseTimers[overheadPhase] = 0;
    phaseTimerBase = clock.getTotalRealTime ();
}


void OpenSteer::OpenSteerDemo::updatePhaseTimers (void)
{
    const float currentRealTime = clock.realTimeSinceFirstClockUpdate();
    phaseTimers[phase] += currentRealTime - phaseTimerBase;
    phaseTimerBase = currentRealTime;
}
