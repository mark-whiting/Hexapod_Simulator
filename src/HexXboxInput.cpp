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
}

/**************************************************************************
 *************************************************************************/
void HexXboxInput::periodic(void)
{
    ControllerVecT::iterator iter;
    for (iter=controllers.begin(); iter!=controllers.end(); iter++) {
        (*iter)->periodic();
    }
}


/**************************************************************************
 *************************************************************************/
void HexXboxInput::getInput(HexInputT *input)
{
    input->radius = 0.0;
    input->crabAngle = 0.0;
    input->speed = 0.0;
}


} /* namespace hexapod */
} /* namespace khameleon */
