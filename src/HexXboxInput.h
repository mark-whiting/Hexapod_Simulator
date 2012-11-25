/******************************************************************************
 * HexXboxInput.h
 *
 * This file provides a wrapper class to convert an Xbox controller to the
 * HexInputInterface needed by the hexapod Simulator.
 *
 * History:
 *      maw 11/24/12    First.
 *
 *****************************************************************************/

#ifndef INC_GUARD_f883ef87_7292_4116_8c67_6c9192176b1c
#define INC_GUARD_f883ef87_7292_4116_8c67_6c9192176b1c

#include <vector>
#include <boost/shared_ptr.hpp>
#include "HexInputInterface.h"
#include "XboxController.h"

namespace khameleon {
namespace hexapod {


typedef std::vector<boost::shared_ptr<XboxController> > ControllerVecT;


class HexXboxInput : public HexInputInterface
{
public:
    HexXboxInput(void);
    void periodic(void);
    void getInput(HexInputT *);

private:
    ControllerVecT controllers;
};


} /* namespace hexapod */
} /* namespace khameleon */

#endif /* INC_GUARD_f883ef87_7292_4116_8c67_6c9192176b1c */
