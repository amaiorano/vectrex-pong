// Pong

#include "base.h"
#include "bios.h"
#include "joystick.h"
#include "object.h"
#include "vector_list.h"

const uint8_t ScreenWidth = 255;
const uint8_t ScreenHeight = 255;
const int8_t ScreenMaxX = 127;
const int8_t ScreenMinX = -128;
const int8_t ScreenMaxY = 127;
const int8_t ScreenMinY = -128;

bool Collides(const Object& o1, const Object& o2) {
    return (o1.Left() < o2.Right() && //
            o1.Right() > o2.Left() && //
            o1.Bottom() < o2.Top() && //
            o1.Top() > o2.Bottom()    //
    );
}

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

    Object paddle1(VectorList::FromMemory(paddleVertices));
    Object paddle2(VectorList::FromMemory(paddleVertices));
    Object ball(VectorList::FromMemory(crossVertices));
    Object border(VectorList::FromMemory(borderVertices));

    paddle1.SetPos(-100, 0);
    paddle2.SetPos(100, 0);
    ball.SetPos(0, 50);

    border.SetPos(-127, -127);

    const Joystick& joystick1 = Joystick::Get(0);
    const Joystick& joystick2 = Joystick::Get(1);

    // Speeds
    const int8_t paddlesy = 4;
    int8_t ballsx = 2;
    int8_t ballsy = 1;

    const size_t numPlayers = 2;

    Object* objects[] = {&paddle1, &paddle2, &ball, &border};
    Object* paddles[] = {&paddle1, &paddle2};
    const Joystick* joysticks[] = {&joystick1, &joystick2};


    bios::Init();

    while (true) {
        bios::WaitFrame();

        for (size_t i = 0; i < numPlayers; ++i) {
            Object* paddle = paddles[i];
            const Joystick* joystick = joysticks[i];

            if (joystick->IsDown(Dpad::Up) && paddle->Top() <= (ScreenMaxX - paddlesy)) {
                paddle->Move(0, paddlesy);
            } else if (joystick->IsDown(Dpad::Down) &&
                       paddle->Bottom() >= (ScreenMinY + paddlesy)) {
                paddle->Move(0, -paddlesy);
            }
        }

        ball.Move(ballsx, ballsy);

        for (size_t i = 0; i < std::size(paddles); ++i) {
            Object* paddle = paddles[i];
            if (Collides(ball, *paddle)) {
                ballsx = -ballsx;
                while (Collides(ball, *paddle)) {
                    ball.Move(ballsx, ballsy);
                }
            }
        }

        int ballSpeedX = std::abs(ballsx);
        int ballSpeedY = std::abs(ballsy);

        if ((ball.Top() >= (ScreenMaxY - ballSpeedY)) ||
            ball.Bottom() <= (ScreenMinY + ballSpeedY)) {
            ballsy = -ballsy;
            ball.Move(ballsx, ballsy);
        }

        if ((ball.Right() >= (ScreenMaxX - ballSpeedX)) ||
            ball.Left() <= (ScreenMinX + ballSpeedX)) {
            ballsx = -ballsx;
            ball.Move(ballsx, ballsy);
        }

        for (size_t i = 0; i < std::size(objects); ++i) {
            objects[i]->Draw();
        }
    }

    return 0;
}
