// Pong

#include "base.h"
#include "bios.h"
#include "joystick.h"
#include "object.h"
#include "text.h"
#include "vector_list.h"

const uint8_t thruster_sound[] = {0x00, 0x10, // Tone A low
                                  0x01, 0x00, // Tone A high
                                  0x06, 0x1F, // Noise
                                  0x07, 0x06, // Mixer
                                  0x08, 0x0F, // Amplitude A
                                  0xFF};

const uint8_t mine_pop_sound[] = {0x00, 0x00, // Tone A low
                                  0x01, 0x00, // Tone A high
                                  0x02, 0x30, // Tone B low
                                  0x03, 0x00, // Tone B high
                                  0x04, 0x00, // Tone C low
                                  0x05, 0x00, // Tone C high
                                  0x06, 0x1F, // Noise
                                  0x07, 0x3D, // Mixer
                                  0x08, 0x00, // Amplitude A
                                  0x09, 0x0F, // Amplitude B
                                  0x0A, 0x00, // Amplitude C
                                  0x0B, 0x00, // EnvelopePeriodLow
                                  0x0C, 0x00, // EnvelopePeriodHigh
                                  0x0D, 0x00, // EnvelopeShape
                                  0xFF};

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
    bios::Init();

    uint8_t numPlayers = 0, gameMode = 0;
    bios::ShowSelectGameScreen(2, 5, numPlayers, gameMode);

    static const int8_t paddleVertices[] = {
        3,       // size - 1
        0,   5,  // Top left is local (0,0)
        -20, 0,  //
        0,   -5, //
        20,  0   //
    };

    const int SIZE = 4;
    static const int8_t crossVertices[] = {
        4,                   //
        SIZE / 2,  SIZE / 2, //
        -SIZE,     -SIZE,    //
        SIZE / 2,  SIZE / 2, //
        -SIZE / 2, SIZE / 2, //
        SIZE,      -SIZE,    //
    };

    static const int8_t borderVertices[] = {
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

    static Object paddle1(VectorList::FromMemory(paddleVertices));
    static Object paddle2(VectorList::FromMemory(paddleVertices));
    static Object ball(VectorList::FromMemory(crossVertices));
    static Object border(VectorList::FromMemory(borderVertices));
    static Text<3> scoreText1(-110, 120);
    static Text<9> scoreText2(60, 120);

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

    Object* objects[] = {&paddle1, &paddle2, &ball, &border};
    Object* paddles[] = {&paddle1, &paddle2};
    const Joystick* joysticks[] = {&joystick1, &joystick2};

    int8_t score1 = 0;
    int8_t score2 = 0;

    while (true) {
        bios::WaitFrame();

        // Move paddles on player input
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

        // Move the ball
        ball.Move(ballsx, ballsy);

        // Ball-paddle collision
        for (size_t i = 0; i < std::size(paddles); ++i) {
            Object* paddle = paddles[i];
            if (Collides(ball, *paddle)) {
                bios::PlaySound(mine_pop_sound);

                ballsx = -ballsx;
                while (Collides(ball, *paddle)) {
                    ball.Move(ballsx, ballsy);
                }
            }
        }

        int ballSpeedX = std::abs(ballsx);
        int ballSpeedY = std::abs(ballsy);

        // If ball hits top or bottom, bounce
        if ((ball.Top() >= (ScreenMaxY - ballSpeedY)) ||
            ball.Bottom() <= (ScreenMinY + ballSpeedY)) {
            bios::PlaySound(thruster_sound);

            ballsy = -ballsy;
            ball.Move(ballsx, ballsy);
        }

        // If ball hits side, someone wins a point
        const bool hitRight = ball.Right() >= (ScreenMaxX - ballSpeedX);
        if (hitRight || ball.Left() <= (ScreenMinX + ballSpeedX)) {
            bios::PlaySound(thruster_sound);

            ballsx = -ballsx;
            ball.Move(ballsx, ballsy);

            if (hitRight) {
                ++score1;
            } else {
                ++score2;
            }
        }

        char scoreString[10];
        scoreText1.SetString(itoa(score1, scoreString));
        scoreText2.SetString(itoa(score2, scoreString));

        // Draw everything
        for (size_t i = 0; i < std::size(objects); ++i) {
            objects[i]->Draw();
        }
        scoreText1.Draw();
        scoreText2.Draw();
    }

    return 0;
}
