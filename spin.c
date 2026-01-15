#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#define PI 3.141592654

 /*Function prototype*/
void clear_space();
void print_coord(int, int, char);
void get_console_dimensions();

/*Global variable*/
int height;
int width;
HANDLE hConsole;
char *buffer;

struct points{
    int x, y;
};

struct states{
    struct points point[121];
};


int main(){

    int theta = 0;
    int d_theta = 5;
    int size = 5;
    double theta_rad;

    struct states state[360];

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    get_console_dimensions();
    buffer = (char *)malloc(width * height);
    clear_space();

    while (theta < 360){
        theta_rad = theta*PI/180;
        for (int x_coord = -size; x_coord <= size; x_coord++){
            for (int y_coord = -size; y_coord <= size; y_coord++)
            { 
                int index = (x_coord+size) * (size*2+1) + (y_coord+size);
                state[theta].point[index].x = (int)(x_coord*cos(theta_rad) - y_coord*sin(theta_rad));
                state[theta].point[index].y = (int)(x_coord*sin(theta_rad) + y_coord*cos(theta_rad));
            }
        
        }
        theta += d_theta;
    }

    while(1){
        if (theta >=360) theta = 0;
        
        // Fill buffer with spaces
        memset(buffer, ' ', width * height);
        
        // Draw to buffer
        for (int x_coord = -size; x_coord <= size; x_coord++){
            for (int y_coord = -size; y_coord <= size; y_coord++){
                int index = (x_coord+size) * (size*2+1) + (y_coord+size);
                int bx = width/2 + (state[theta % 360].point[index].x*2);
                int by = height/2 - state[theta % 360].point[index].y;
                if (bx >= 0 && bx < width && by >= 0 && by < height) {
                    buffer[by * width + bx] = '.';
                }
            }
        }
        
        // Write entire buffer to console at once
        COORD coord = {0, 0};
        DWORD written;
        WriteConsoleOutputCharacter(hConsole, buffer, width * height, coord, &written);
        
        theta+=d_theta;
        Sleep(60);

    }
    
    return 0;
}

void clear_space(){
    COORD coord = {0, 0};
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', width * height, coord, &written);
    SetConsoleCursorPosition(hConsole, coord);
}

void get_console_dimensions(){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
}