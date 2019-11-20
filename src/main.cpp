// Pong

#include "base.h"
#include "bios.h"
#include "vector_list.h"

int main() {
    int8_t paddleVertices[] = {
        3,       // size - 1
        0,   5,  //
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

    VectorList paddle(VectorListData::FromMemory(paddleVertices));
    // VectorList cross(VectorListData::FromMemory(crossVertices));
    VectorListData* crossData = VectorListData::Create(&crossVertices[1], 10);
    VectorList cross(*crossData);

    bios::Init();

    while (true) {
        bios::WaitFrame();

        paddle.Draw();
        cross.Draw();
    }

    return 0;
}
