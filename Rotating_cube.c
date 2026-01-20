#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

/*Global Variables*/
#define DIMENSION 16
#define PI 3.141592654


int con_height, con_width;
HANDLE hConsole;

/*Structure declaration*/
struct Vector
{
    double X, Y, Z;
}vector[DIMENSION][DIMENSION][DIMENSION];

/*Function Prototypes*/
void create_canvas();
void rot_abt_x(struct Vector*, double);
void rot_abt_y(struct Vector*, double);
void rot_abt_z(struct Vector*, double);
double illuminance(struct Vector, struct Vector);

/*Main Function*/
int main(){
    create_canvas();
    char *buffer = (char*)malloc(con_width * con_height);
    int *zbuffer = (int*)malloc(con_width * con_height * sizeof(int));
    COORD coord = {0, 0};
    DWORD written;
    int x_offset = -8, y_offset = -8, z_offset = -8;
    char show;

    struct Vector light = {0, 1, -1};

    /*Initialize angles*/
    double theta = 0;
    double d_theta = 1;

    /*Create coordinates in 2D plane for cube*/
    while(1){
        /*Animate cube offset if required.*/
        //if (x_offset > 6) x_offset = 0;
        //if (y_offset > 6) y_offset = 0;
        //if (z_offset > 6) z_offset = 0;

        //Reset angles
        if(theta == 0) d_theta = 1;
        if(theta > 360) d_theta = -1;

        //Clear buffers
        memset(buffer, ' ', con_width * con_height);
        for(int i = 0; i < con_width * con_height; i++) {
            zbuffer[i] = 999;
        }
        
        for (int x = x_offset; x < DIMENSION+x_offset; x++)
        {
            for (int y = y_offset; y < DIMENSION+y_offset; y++)
            {
                for (int z = z_offset; z < DIMENSION+z_offset; z++)
                {
                    vector[x-x_offset][y-y_offset][z-z_offset].X = x;
                    vector[x-x_offset][y-y_offset][z-z_offset].Y = y;
                    vector[x-x_offset][y-y_offset][z-z_offset].Z = z;

                    // Determine face normal
                    struct Vector normal;
                    if (x == DIMENSION + x_offset - 1) normal = (struct Vector){1,0,0};
                    else if (x == x_offset) normal = (struct Vector){-1,0,0};
                    else if (y == DIMENSION + y_offset - 1) normal = (struct Vector){0,1,0};
                    else if (y == y_offset) normal = (struct Vector){0,-1,0};
                    else if (z == DIMENSION + z_offset - 1) normal = (struct Vector){0,0,1};
                    else normal = (struct Vector){0,0,-1};

                    rot_abt_x(&(vector[x-x_offset][y-y_offset][z-z_offset]), theta);
                    rot_abt_y(&(vector[x-x_offset][y-y_offset][z-z_offset]), theta);
                    rot_abt_z(&(vector[x-x_offset][y-y_offset][z-z_offset]), theta);

                    // Rotate normal too
                    rot_abt_x(&normal, theta);
                    rot_abt_y(&normal, theta);
                    rot_abt_z(&normal, theta);

                    double illum = illuminance(normal, light);
                    char chars[] = ".,-~:;=!*#$@";
                    show = chars[(int)(illum * 11)];

                    int buff_x = con_width/2 + (vector[x-x_offset][y-y_offset][z-z_offset].X + vector[x-x_offset][y-y_offset][z-z_offset].Z/2)*2;
                    int buff_y = con_height/2 - (vector[x-x_offset][y-y_offset][z-z_offset].Y + vector[x-x_offset][y-y_offset][z-z_offset].Z/2);

                    if (buff_x >= 0 && buff_x < con_width && buff_y >= 0 && buff_y < con_height) {
                        // Draw edges where at least 2 coordinates are at boundaries
                        int is_edge = 0;
                        
                        // Check if at min/max boundaries
                        int x_boundary = (x == x_offset) || (x == x_offset + DIMENSION - 1);
                        int y_boundary = (y == y_offset) || (y == y_offset + DIMENSION - 1);
                        int z_boundary = (z == z_offset) || (z == z_offset + DIMENSION - 1);
                        
                        // Count how many boundaries this point is at
                        int boundary_count = x_boundary + y_boundary + z_boundary;
                        
                        if (boundary_count >= 2) {
                            is_edge = 1;
                        }

                        /*Converting 2D (x_buff, y_buff) coordinates to 1D console character coordinates*/
                        int index = buff_x + con_width * buff_y;

                        /*Applying z-buffering (Depth sensing)*/
                        if((int)vector[x-x_offset][y-y_offset][z-z_offset].Z < zbuffer[index]){
                            buffer[index] = is_edge ? show : show;
                            zbuffer[index] = (int)vector[x-x_offset][y-y_offset][z-z_offset].Z;
                        }
                    }
                }
            }
        }

        WriteConsoleOutputCharacter(hConsole, buffer, con_width*con_height, coord, &written);
        //x_offset++;
        //y_offset++;
        //z_offset++;
        Sleep(150);
        theta += d_theta*2;
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

void rot_abt_x(struct Vector *vect, double theta){
    double theta_rad = PI*theta/180;
    double old_y = vect->Y;
    double old_z = vect->Z;
    vect->Y = old_y * cos(theta_rad) - old_z * sin(theta_rad);
    vect->Z = old_y * sin(theta_rad) + old_z * cos(theta_rad);
}

void rot_abt_y(struct Vector *vect, double theta){
    double theta_rad = PI*theta/180;
    double old_x = vect->X;
    double old_z = vect->Z;
    vect->X = old_x * cos(theta_rad) + old_z * sin(theta_rad);
    vect->Z = -old_x * sin(theta_rad) + old_z * cos(theta_rad);
}

void rot_abt_z(struct Vector *vect, double theta){
    double theta_rad = PI*theta/180;
    double old_x = vect->X;
    double old_y = vect->Y;
    vect->X = old_x * cos(theta_rad) - old_y * sin(theta_rad);
    vect->Y = old_x * sin(theta_rad) + old_y * cos(theta_rad);
}

double illuminance(struct Vector normal, struct Vector light){
    // Normalize light vector
    double light_len = sqrt(light.X*light.X + light.Y*light.Y + light.Z*light.Z);
    light.X /= light_len;
    light.Y /= light_len;
    light.Z /= light_len;
    
    // Normalize normal
    double norm_len = sqrt(normal.X*normal.X + normal.Y*normal.Y + normal.Z*normal.Z);
    if(norm_len > 0){
        normal.X /= norm_len;
        normal.Y /= norm_len;
        normal.Z /= norm_len;
    }
    
    // Dot product between normal and light
    double illum = normal.X*light.X + normal.Y*light.Y + normal.Z*light.Z;
    
    // Map from -1..1 to 0..1
    illum = (illum + 1.0) / 2.0;
    
    if(illum < 0) illum = 0;
    if(illum > 1) illum = 1;
    return illum;
}

