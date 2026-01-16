#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

/*Global Variables*/
#define DIMENSION 8

int con_height, con_width;
HANDLE hConsole;

/*Structure declaration*/
struct Vector
{
    int X, Y, Z;
}vector[DIMENSION][DIMENSION][DIMENSION];

/*Function Prototypes*/
void create_canvas();

/*Main Function*/
int main(){
    char *buffer;
    float angle = 0;
    COORD coord = {0, 0};
    DWORD written;
    int x_offset = -6, y_offset = 0, z_offset = 0;

    create_canvas();
    buffer = (char *)malloc(con_width * con_height);

    /*Create coordinates in 2D plane for cube*/
    /*X = x - z/2 and Y = y - z/2*/
    while(1){
        if (x_offset > 6) x_offset = -6;
        //if (y_offset > 6) y_offset = 0;
        if (z_offset > 6) z_offset = 0;

        memset(buffer, ' ', con_width * con_height);
        
        for (int x = x_offset; x < DIMENSION+x_offset; x++)
        {
            for (int y = y_offset; y < DIMENSION+y_offset; y++)
            {
                for (int z = z_offset; z < DIMENSION+z_offset; z++)
                {
                    vector[x-x_offset][y-y_offset][z-z_offset].X = x;
                    vector[x-x_offset][y-y_offset][z-z_offset].Y = y;
                    vector[x-x_offset][y-y_offset][z-z_offset].Z = z;

                    int buff_x = con_width/2 + (x - z/2)*2;
                    int buff_y = con_height/2 - (y - z/2);

                    if (buff_x >= 0 && buff_x < con_width && buff_y >= 0 && buff_y < con_height) {
                        buffer[buff_y * con_width + buff_x] = '0';
                    }
                }
            }
        }

        WriteConsoleOutputCharacter(hConsole, buffer, con_width*con_height, coord, &written);
        x_offset++;
        y_offset = sqrt(36 - pow(x_offset, 2));
        //z_offset++;
        Sleep(100);
    }
    return 0;
}

/*User Defined Functions*/
void create_canvas(){
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD coord = {0,0};
    DWORD written;

    if(GetConsoleScreenBufferInfo(hConsole, &csbi)){
        con_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        con_height = csbi.srWindow.Bottom - csbi.srWindow.Top +1;
    }

    FillConsoleOutputCharacter(hConsole,' ', con_width*con_height,coord, &written);
    SetConsoleCursorPosition(hConsole, coord);
}
   