/******************************************************************************
 * XboxController.cpp
 *
 * This file provides a usefull interface for an xbox controller. Uses the
 * XInput API.
 *
 * History:
 *      maw 11/24/12    First.
 *
 *****************************************************************************/

/* No MFC */
#define WIN32_LEAN_AND_MEAN

#include <cassert>
#include <cmath>
#include "XboxController.h"

using namespace boost::chrono;

namespace khameleon {

namespace {
    const DWORD XBOX_CONTROLLER_POLL_TIMEOUT = 2 * (1000 * 1000 * 1000); // in nanoseconds


    /**************************************************************************
     * calcTriggerDeadzone()
     *
     * Takes a trigger value from an Xbox controller and calculates a normalized
     * output based on a deadzone passed in. The deadzone defaults to the XInput
     * default.
     *
     * Returns: N/A
     *************************************************************************/
    void calcTriggerDeadzone(BYTE triggerVal, double *outVal,
        BYTE deadzone=XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
    {
        if (triggerVal > deadzone) {
            /* Normalize input in the range deadzone-255 to 0-1 */
            double num = (double)(triggerVal - deadzone);
            double den = (double)(255 - deadzone);
            *outVal = (num/den);
            return;
        }

        *outVal = 0.0;
        return;
    }


    /**************************************************************************
     * calcThumbstickDeadzone()
     *
     * Takes the x and y position of an Xbox controller thumbstick and calculates
     * an output x,y that is normalized to the range 0.0-1.0. only updates the
     * output if the sticks are passed the deadzone, there is no default for this
     * deadzone because the XInput defaults for the left and right thumbstick are
     * different so the user is required to pass them in. The output values are
     * then squared so that there is greater control over the lower end values.
     *
     * Returns: N/A
     *************************************************************************/
    void calcThumbstickDeadzone(SHORT inX, SHORT inY, double *outX, double *outY, SHORT deadzone)
    {
        double x = (double) inX;
        double y = (double) inY;

        double magnitude = std::sqrt((x*x)+(y*y));
        if (magnitude > 32767) {
            magnitude = 32767;
        }

        if (magnitude > (double)deadzone) {
            /* scale inputs in the range deadzone-65535 to 0-1 */
            double scaleFactor = (magnitude - (double)deadzone)/magnitude;

            double den = (double)(32767-deadzone);
            *outX = (x*scaleFactor)/den;
            *outY = (y*scaleFactor)/den;
            return;
        }

        *outX = 0.0;
        *outY = 0.0;
        return;
    }


} /* End anonymouse namespace */


/**************************************************************************
 * InputState c'tor
 *************************************************************************/
InputState::InputState(void) :
    buttons      (0),
    leftTrigger  (0.0),
    rightTrigger (0.0),
    leftThumbX   (0.0),
    leftThumbY   (0.0),
    rightThumbX  (0.0),
    rightThumbY  (0.0)
{
}


/**************************************************************************
 * XboxController c'tor
 *************************************************************************/
XboxController::XboxController(DWORD id) :
    controllerConnected (false),
    controllerIndex     (id)
{
    /* XInput only supports 4 controllers */
    assert((id >= 0) && (id < 4));
}


/**************************************************************************
 * connected()
 *
 * Returns: True if the controller is connected, false otherwise.
 *************************************************************************/
bool XboxController::connected(void)
{
    return controllerConnected;
}


/**************************************************************************
 * periodic()
 *
 * This function must be called regularly to poll the state of the controller.
 * If the controller is not connected yet it polls less frequently per the
 * Windows SDK documentation. Apparantly polling on an unconnected controller
 * is fairly resource intensive. If the controller is connected we update our
 * internal state for the controller.
 *
 * Returns: N/A
 *************************************************************************/
void XboxController::periodic(void)
{
    DWORD result;
    XINPUT_STATE tmpState;

    /* Check for controller */
    if (!controllerConnected) {
        steady_clock::duration tmp = steady_clock::now() - lastControllerCheck;
        if (tmp.count() < XBOX_CONTROLLER_POLL_TIMEOUT) {
            /* Not enough time has passed since last time we polled */
            return;
        }

        lastControllerCheck = steady_clock::now();
        result = XInputGetState(controllerIndex, &tmpState);
        if (result != ERROR_SUCCESS) {
            /* The controller is not present */
            return;
        }

        /* We found the controller */
        controllerConnected = true;
    }

    /* Get the controller state */
    ZeroMemory(&tmpState, sizeof(XINPUT_STATE));
    result = XInputGetState(controllerIndex, &tmpState);
    if (result != ERROR_SUCCESS) {
        controllerConnected = false;
        return;
    }

    /* Update the controller state */
    if (lastPacketNumber != tmpState.dwPacketNumber) {
        controllerState = tmpState;
    }
}


/**************************************************************************
 * getState()
 *
 * Returns the current gamepad state after accounting for deadzones and
 * normalizing the output. Fills 'state' with the calculated values.
 *
 * Returns: true if 'state' was updated, false otherwise.
 *************************************************************************/
bool XboxController::getState(InputStateT *state)
{
    if (!controllerConnected) {
        return false;
    }

    /* Buttons */
    state->buttons = controllerState.Gamepad.wButtons;
    
    /* Triggers */
    calcTriggerDeadzone(controllerState.Gamepad.bLeftTrigger, &(state->leftTrigger));
    calcTriggerDeadzone(controllerState.Gamepad.bRightTrigger, &(state->rightTrigger));

    /* Thumbsticks */
    calcThumbstickDeadzone(controllerState.Gamepad.sThumbLX, controllerState.Gamepad.sThumbLY,
        &(state->leftThumbX), &(state->leftThumbY), XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

    calcThumbstickDeadzone(controllerState.Gamepad.sThumbRX, controllerState.Gamepad.sThumbRY,
        &(state->rightThumbX), &(state->rightThumbY), XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

    return true;
}


} /* namespace khameleon */
