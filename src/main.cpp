// hexapod_input.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include "XboxController.h"
#include "HexInputInterface.h"

using namespace khameleon;

double length(double x, double y)
{
    return std::sqrt((x*x)+(y*y));
}

int main(int argc, char* argv[])
{
    XboxController *tmp[4];

    tmp[0] = new XboxController(0);
    tmp[1] = new XboxController(1);
    tmp[2] = new XboxController(2);
    tmp[3] = new XboxController(3);

    while (true) {
        for (int i=0; i<4; i++) {
            tmp[i]->periodic();

            InputStateT input;
            if (tmp[i]->getState(&input)) {
                std::printf("controller %d: buttons (0x%08x), trigger (%f, %f), lThumb (%f, %f, %f), rThumb (%f, %f, %f)\n",
                    i, input.buttons, input.leftTrigger, input.rightTrigger, input.leftThumbX, input.leftThumbY,
                    length(input.leftThumbX, input.leftThumbY), input.rightThumbX, input.rightThumbY,
                    length(input.rightThumbX, input.rightThumbY));
            }
        }
    }

	return 0;
}

