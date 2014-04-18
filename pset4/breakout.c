//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of paddle
#define PADDLEWIDTH 50
#define PADDLEHEIGHT 8
#define PADDLEY 550

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial velocity
    double velocityX = drand48();
    double velocityY = 1.0;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // if we heard it
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // let paddle follow cursor
                double x = getX(event) - (PADDLEWIDTH / 2);
                setLocation(paddle, x, PADDLEY);
            }
        }
        
        move(ball, velocityX, velocityY);
                
        // if ball reaches bottom edge of window
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            // subtract 1 life
            lives = lives - 1;
            if (lives > 0)
            {
                setLocation(ball, 195, 300);
                waitForClick();
            }
        }
        
        // bounce off top edge of window
        if (getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        
        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }
        
        // linger before moving again
        pause(2);
        
        if (detectCollision(window, ball) != NULL)
        {
            GObject object = detectCollision(window, ball);
            // if ball hits paddle
            if (object == paddle)
            {
                velocityY = -velocityY;
            }
            // if ball hits a GRect
            else if (strcmp(getType(object), "GRect") == 0)
            {
                if (strcmp(getType(object), "GLabel") ==  0)
                {
                }
                // if GRect is a brick
                else 
                {
                    velocityY = -velocityY;
                    removeGWindow(window, object);
                    points = points + 1;
                    updateScoreboard(window, label, points);
                }
            }

        }
        if (points >= ROWS*COLS)
        {
            setLabel(label, "Congrats!");
            double x = (getWidth(window) - getWidth(label)) / 2;
            double y = (getHeight(window) - getHeight(label)) / 2;
            setColor(label, "ORANGE");
            setLocation(label, x, y);
            waitForClick();
            closeGWindow(window);
            return 0;
        }
        
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int x = 5;
    int y = 5;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, 32, 10);
            setColor(brick, "ORANGE");
            setFilled(brick, true);
            add(window, brick);
            x = x + 40;
        }
        y = y + 15;
        x = 5;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(195, 300, 10, 10);
    setColor(ball, "DARK_GRAY");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(175, PADDLEY, PADDLEWIDTH, PADDLEHEIGHT);
    add(window, paddle);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel scoreboard = newGLabel("0");
    add(window, scoreboard);
    setFont(scoreboard, "SansSerif-70");
    double x = (getWidth(window) - getWidth(scoreboard)) / 2;
    double y = (getHeight(window) - getHeight(scoreboard)) / 2;
    setLocation(scoreboard, x, y);
    return scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
