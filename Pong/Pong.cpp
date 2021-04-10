//Note: Contains a section of copied code from stackoverflow
// that gets the width and height of the console screen.
// Original poster of the code was quantum. The section is
// commented in the code bellow. Link to the forum:
// https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <thread>
#include <chrono>

//Define left and right controls 
#define RIGHT 'X'
#define LEFT 'Z'

int ballX, ballY, speedX, platX;
int columns, rows;
bool ballRight = true;
bool ballUp = false;
bool gameOut = false;
bool platDirectInfluence = true;

void startMenu() {

    printf("\Readjust window size now. Press Enter to start . . .\n");
    std::cin.ignore();
    system("cls");

}

void ballPos() {

    //X direction
    if (ballX >= columns && ballRight) { ballRight = false; }
    else if (ballX <= 0 && !ballRight) { ballRight = true; }

    //Y direction
    if (ballY >= rows && ballUp) { ballUp = false; }
    else if (ballY <= 0 && !ballUp) { ballUp = true; } //This checks if the ball hits the floor, can be removed when floor detection is added.

    //Platform detection
    else if (ballY == 5 && ballX >= platX && ballX <= platX + 16) { 
        ballUp = true;
        if (ballX <= platX + 8 && platDirectInfluence) { ballRight = false; }
        else if (platDirectInfluence) { ballRight = true; }\

        //Ball X speed
        if (ballX == platX + 8 || ballX == platX + 9) { speedX = 1; }
        else if (ballX >= platX + 4 || ballX <= 13) { speedX = 2;  }
        else if (ballX >= platX || ballX <= platX + 16) { speedX = 3; }
    }

    //X position
    if (ballRight) { ballX += speedX; }
    else { ballX -= speedX; }

    //Y position
    if (ballUp) { ballY += 1; }
    else { ballY -= 1; }

}

void platPos() {

    if (GetKeyState(RIGHT) & 0x8000 && platX + 16 < columns) { platX += 2; }
    else if (GetKeyState(LEFT) & 0x8000 && platX > 0) { platX -= 2; }

}

void redraw() {

    system("cls");

    if (ballY > 5) {

        //Choosing Ball Row
        for (int i = 0; i < (rows - ballY); i++) {
            printf("\n");
        }
        //Printing Ball
        for (int i = 1; i < ballX - 1; i++) {
            std::cout << ' ';
        }
        std::cout << 'O';
        //std::cout << std::right << std::setw(ballX) << 'O';
        //(Commented out to see if a for loop would be faster than setw)

        //Printing Platform
        for (int i = ballY; i >= 6; i--) {
            printf("\n");
        }
        std::cout << std::setw(platX) << '=' << "===============";

    }
    else if (ballY < 5) {

        //Printing Platform
        for (int i = 0; i < rows - 5; i++) {
            printf("\n");
        }
        std::cout << std::setw(platX) << '=' << "===============";

        //Choosing Ball Row
        for (int i = 4; i > ballY; i--) {
            printf("\n");
        }
        //Printing Ball
        std::cout << std::right << std::setw(ballX) << 'O';

    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));

}

void game() {

    ballX = 0;
    ballY = rows;
    speedX = 1;
    platX = columns / 2;

    while (!gameOut) {

        ballPos();

        platPos();

        redraw();

        if (ballY == 0) {
            gameOut = true;
        }

    }

}

void gameOver() {

    system("cls");

    printf("\n\n\n\nGame Over!\n\n\n\n\n\n");

}

int main() {

    startMenu();

    //Getting console size
    //Code bellow is copied from quantum on stackoverflow.
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    //End of copied code

    game();

    gameOver();

}