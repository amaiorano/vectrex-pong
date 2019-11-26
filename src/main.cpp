// Pong

#include "base.h"
#include "bios.h"
#include "object.h"
#include "vector_list.h"

const uint8_t ScreenWidth = 255;
const uint8_t ScreenHeight = 255;
const int8_t ScreenMaxX = 127;
const int8_t ScreenMinX = -128;
const int8_t ScreenMaxY = 127;
const int8_t ScreenMinY = -128;

int main() {
    int8_t paddleVertices[] = {
        3,       // size - 1
        0,   5,  // Top left is local (0,0)
        -20, 0,  //
        0,   -5, //
        20,  0   //
    };

    const int SIZE = 4;
    int8_t crossVertices[] = {
        4,                   //
        SIZE / 2,  SIZE / 2, //
        -SIZE,     -SIZE,    //
        SIZE / 2,  SIZE / 2, //
        -SIZE / 2, SIZE / 2, //
        SIZE,      -SIZE,    //
    };

    int8_t borderVertices[] = {
        7,          //
        127,  0,    //
        127,  0,    //
        0,    127,  //
        0,    127,  //
        -128, 0,    //
        -128, 0,    //
        0,    -128, //
        0,    -128, //
    };

    Object paddle(VectorList::FromMemory(paddleVertices));
    Object cross(VectorList::FromMemory(crossVertices));
    Object border(VectorList::FromMemory(borderVertices));

    bios::Init();

    border.SetPos(-127, -127);

    while (true) {
        bios::WaitFrame();

        const int8_t maxSpeed = 2;

        static int8_t paddleDY = maxSpeed;
        paddle.Move(0, paddleDY);
        if (paddle.Top() >= (ScreenMaxY - maxSpeed)) {
            paddleDY = -paddleDY;
        } else if (paddle.Bottom() <= (ScreenMinY + maxSpeed)) {
            paddleDY = -paddleDY;
        }

        paddle.Draw();
        cross.Draw();
        border.Draw();
    }

    return 0;
}
