// Pong

#include "base.h"
#include "bios.h"
#include "joystick.h"
#include "object.h"
#include "psg_util.h"
#include "text.h"
#include "vector_list.h"
#include "vectrexy.h"

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

    static const uint8_t paddleHitSound[] = {PSG_TONE(0, 192, 0),
                                             PSG_NOISE(0),
                                             PSG_MIXER(PsgMixer::ToneB),
                                             PSG_AMPLITUDE(0, 31, 0),
                                             PSG_ENVELOPE(3327, PsgEnvelopeShape::Down),
                                             PSG_TERMINATOR};

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
                bios::PlaySound(paddleHitSound);

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
            bios::PlaySound(paddleHitSound);

            ballsy = -ballsy;
            ball.Move(ballsx, ballsy);
        }

        // If ball hits side, someone wins a point
        const bool hitRight = ball.Right() >= (ScreenMaxX - ballSpeedX);
        if (hitRight || ball.Left() <= (ScreenMinX + ballSpeedX)) {
            bios::PlaySound(paddleHitSound);

            ballsx = -ballsx;
            ball.Move(ballsx, ballsy);

            if (hitRight) {
                ++score1;
            } else {
                ++score2;
            }

            vectrexy::printf("score 1: %d, score2: %d\n", score1, score2);
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
