/******************************************************************************
 * HexInputInterface.h
 *
 * This file provides a base class that defines the interface between an input
 * device and the hexapod simulation. Any class that derives and implements this
 * base class can be used as a controller for the hexapod.
 *
 * History:
 *      maw 11/24/12    First.
 *
 *****************************************************************************/

#ifndef INC_GUARD_782156ed_39ea_4641_a759_6201a9e493d4
#define INC_GUARD_782156ed_39ea_4641_a759_6201a9e493d4

namespace khameleon {
namespace hexapod {


/**************************************************************************
 * HexInput - struct
 *
 * This structure defines the input control to the hexapod.
 *
 * Valid values are:
 * -----------------
 * radius       (0.0 to INF)
 * crabAngle    (-PI to PI)
 * speed        (0.0 to 1.0)
 *************************************************************************/
typedef struct HexInput
{
    double radius;
    double crabAngle;
    double speed;
} HexInputT;


/**************************************************************************
 * HexInputInterface - class
 *
 * This virtual class provides two functions, one of which must be overridden
 * by the deriving class. The 'periodic' function may be overridden if the
 * deriving class needs to process things regularly. The 'getInput' function
 * must be overridden and must always return a valid HexInputT struct in the
 * argument passed in.
 *************************************************************************/
class HexInputInterface
{
public:
    virtual void periodic(void) {};
    virtual void getInput(HexInputT *) = 0;
};


} /* namespace hexapod */
} /* namespace khameleon */

#endif /* INC_GUARD_782156ed_39ea_4641_a759_6201a9e493d4 */
