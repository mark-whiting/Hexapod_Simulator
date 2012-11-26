// hexapod_input.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <cstdio>
#include "HexXboxInput.h"

using namespace khameleon::hexapod;

double length(double x, double y)
{
    return std::sqrt((x*x)+(y*y));
}

int main(int argc, char* argv[])
{
    HexXboxInput hexController;

    while (true) {
        HexInputT input;
        hexController.periodic();
        hexController.getInput(&input);

        std::printf("radius (%f), crabAngle (%f), speed (%f)\n", input.radius,
            input.crabAngle, input.speed);

        Sleep(100);
    }

#if 0
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
#endif
	return 0;
}

