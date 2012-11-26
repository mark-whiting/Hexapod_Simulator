/******************************************************************************
 * HexXboxInput.cpp
 *
 * This file provides a wrapper class to convert an Xbox controller to the
 * HexInputInterface needed by the hexapod Simulator.
 *
 * History:
 *      maw 11/24/12    First.
 *
 *****************************************************************************/

#include "HexXboxInput.h"

using namespace boost;

namespace khameleon {
namespace hexapod {


/**************************************************************************
 * c'tor
 *************************************************************************/
HexXboxInput::HexXboxInput(void)
{
    controllers.push_back(shared_ptr<XboxController>(new XboxController(0)));
    controllers.push_back(shared_ptr<XboxController>(new XboxController(1)));
    controllers.push_back(shared_ptr<XboxController>(new XboxController(2)));
    controllers.push_back(shared_ptr<XboxController>(new XboxController(3)));

    /* Clear the shared pointer */
    currentController.reset();
}

/**************************************************************************
 *************************************************************************/
void HexXboxInput::periodic(void)
{
    /* Check if we have a controller and if we do only poll it. */
    if (currentController) {
        currentController->periodic();
        return;
    }

    /* We don't have one, poll all controllers */
    ControllerVecT::iterator iter;
    for (iter=controllers.begin(); iter!=controllers.end(); iter++) {
        (*iter)->periodic();
        if ((*iter)->connected()) {
            currentController = *iter;
            return;
        }
    }
}


/**************************************************************************
 *************************************************************************/
void HexXboxInput::getInput(HexInputT *input)
{
    /* Set default input */
    input->radius = 0.0;
    input->crabAngle = 0.0;
    input->speed = 0.0;

    /* Check if we have a controller */
    if (!currentController) {
        return;
    }
    
    /* Get controller state */
    InputStateT state;
    if (!currentController->getState(&state)) {
        /* Failed to get state, go back to polling for a controller */
        currentController.reset();
        return;
    }

    /* Generate hexapod input from controller state. */
    double radiusVal = state.rightThumbX * state.rightThumbX * state.rightThumbX;
    if (radiusVal > 0.0) {
        input->radius = (1.0 / radiusVal) - 1.0;
    }
    else {
        input->radius = (1.0 / radiusVal) + 1.0;
    }

    input->crabAngle = std::atan2(-state.leftThumbX, state.leftThumbY);

    input->speed = 1.0;
}


} /* namespace hexapod */
} /* namespace khameleon */
