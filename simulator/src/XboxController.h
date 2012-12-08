/******************************************************************************
 * XboxController.h
 *
 * This file provides a usefull interface for an xbox controller. Uses the
 * XInput API.
 *
 * History:
 *      maw 11/24/12    First.
 *
 *****************************************************************************/

#ifndef INC_GUARD_12bde22a_2e0f_4bfb_bb65_a33d7980e6ff
#define INC_GUARD_12bde22a_2e0f_4bfb_bb65_a33d7980e6ff

#include <Windows.h>
#include <XInput.h>
#include <boost/chrono.hpp>

namespace khameleon {


/**************************************************************************
 * InputState - struct
 * 
 * This struct contains the current Xbox controller state. The buttons value
 * is the same as that found in XINPUT_STATE and the button masks from the
 * XInput header will work. The trigger and thumbstick values are normalized
 * to the range 0.0-1.0.
 *************************************************************************/
typedef struct InputState
{
    DWORD buttons;
    double leftTrigger;
    double rightTrigger;
    double leftThumbX;
    double leftThumbY;
    double rightThumbX;
    double rightThumbY;

    InputState(void);
} InputStateT;


/**************************************************************************
 * XboxController - class
 *
 * This class hides the details of polling an Xbox controller and properly
 * handling the deadzones of trigger and thumbsticks.
 *************************************************************************/
class XboxController
{
public:
    XboxController(DWORD);
    bool connected(void);
    void periodic(void);
    bool getState(InputStateT *);

private:
    bool  controllerConnected;
    DWORD controllerIndex;

    boost::chrono::steady_clock::time_point lastControllerCheck;

    XINPUT_STATE controllerState;
    DWORD lastPacketNumber;

};


} /* namespace khameleon */

#endif /* INC_GUARD_12bde22a_2e0f_4bfb_bb65_a33d7980e6ff */
