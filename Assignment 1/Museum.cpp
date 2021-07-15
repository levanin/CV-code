//  ========================================================================
//  COSC363: Computer Graphics (2020); CSSE  University of Canterbury.
//
//  FILE NAME: Yard.cpp
//  See Lab03.pdf for details.
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "loadTGA.h"
#include "loadBMP.h"
#define GL_CLAMP_TO_EDGE 0x812F
#define NUM_TEXTURES 8
#define SKYBOX_RECTANGLE 200
#define SKYBOX_HEIGHT 100
#define N 10
using namespace std;

GLuint txId[NUM_TEXTURES];   //Texture ids
float anglehorizontal=0, anglevertical=0, look_x, look_z=-1., eye_x, eye_z, look_y = 0; //Camera parameters
int xstart, ystart = -1; //Mouse look params
float theta = 0; // cylinder animations param
float increment = 0.1;
float t = 0; // time value for our pendulum simulation
float white[4] = {1, 1, 1, 1};
float black[4] = {0, 0, 0, 1};
float grey[4] = {0.4, 0.4, 0.4, 1};
float yellow[3] = {0.63, 0.63, 0.30};
int lightswitch = 1; //toggle for pendulum light

//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(NUM_TEXTURES, txId); 	// Create NUM_TEXTURES texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("banana.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("header.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}
//--------------------------------------------------------------------------------
int collision(float updatex, float updatez)
{
    float posx = eye_x + updatex;
    float posz = eye_z + updatez;

    if (posx < 5 && posx > -5 && posz < -8.5 && posz > -21.8) // inside the museum bounds
    {
        if (posx > -3.3 && posx < 2.3 && posz > -18 && posz < -12)// inside the interior
        {
            return 0;
        } else if(posx > -0.8 && posx < 0.8 && posz > -15) //inside the doorway
        {
            return 0;
        }
        return 1;
    }



    return 0;
}
//--------------------------------------------------------------------------------
void myTimer(int value)
{
    t += 0.02; //update for time for pendulum

    theta -= increment;
    if (theta < -10 || theta > 10)
    {
        increment = -increment;
    }
    glutPostRedisplay();
    glutTimerFunc(2, myTimer, 0);
}
//--------------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y){
if (key == 'a' && eye_x > 2 && eye_x < 2.3 && eye_z > -14 && eye_z < -12)
{
    lightswitch = -lightswitch; //switch toggle if in bounds
}
glutPostRedisplay(); //update display
}
//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
    float shift = (float) M_PI/2; // 90 degree shift for left right movement
    float deye_x = 0;
    float deye_z = 0;

    if(key == GLUT_KEY_LEFT)  //Move Left
    {
        deye_x = 0.1*sin(anglehorizontal - shift);
        deye_z = -0.1*cos(anglehorizontal - shift);
        if  (collision(deye_x, deye_z) == 1)
        {
            deye_x = 0;
            deye_z =0;
        }
    }
    else if(key == GLUT_KEY_RIGHT) // Move right
    {
        deye_x = 0.1*sin(anglehorizontal + shift);
        deye_z = -0.1*cos(anglehorizontal + shift);
        if  (collision(deye_x, deye_z) == 1)
        {
            deye_x = 0;
            deye_z =0;
        }
    }

    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        deye_x = -0.1*sin(anglehorizontal);
        deye_z = 0.1*cos(anglehorizontal);
        if  (collision(deye_x, deye_z) == 1)
        {
            deye_x = 0;
            deye_z =0;
        }

    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        deye_x = 0.1*sin(anglehorizontal);
        deye_z = -0.1*cos(anglehorizontal);
        if  (collision(deye_x, deye_z) == 1)
        {
            deye_x = 0;
            deye_z =0;
        }

    }

    eye_x += deye_x;
    eye_z += deye_z;
    look_x = eye_x + 100*sin(anglehorizontal);
    look_z = eye_z - 100*cos(anglehorizontal);
    glutPostRedisplay();
}

//--------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        xstart = x;
        ystart = y;
    }
    else
    {
        xstart = -1;
        ystart = -1;
    }
}

//--------------------------------------------------------------------------------

void motion(int x, int y)
{
    if (xstart >= 0 && ystart >= 0)
    {
        anglehorizontal += (x - xstart) * 0.00001;
        anglevertical += (y - ystart) * 0.00001;
    }

    look_x = eye_x + 100*sin(anglehorizontal);
    look_z = eye_z - 100*cos(anglehorizontal);
    look_y = -80*anglevertical;

    glutPostRedisplay();
}

//--------------------------------------------------------------------------------
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

    glNormal3f(nx, ny, nz);
}

//--------------------------------------------------------------------------------
void skybox()
{
    glColor4f(1,1,1,1);

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    ////////////////////// FRONT FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(-SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0); glVertex3f(SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0); glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 1.0); glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);

    glEnd();

    ////////////////////// LEFT FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);glVertex3f(SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0);glVertex3f(SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);

    glEnd();

    ////////////////////// RIGHT FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0);glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 1.0);glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);

    glEnd();

    ////////////////////// BACK FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0); glVertex3f(SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0); glVertex3f(-SKYBOX_RECTANGLE, -SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0); glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 1.0); glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);

    glEnd();

    ////////////////////// UP FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, -SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, SKYBOX_HEIGHT, SKYBOX_RECTANGLE);

    glEnd();

    ////////////////////// DOWN FACE ///////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, -1.8, SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 1.0);glVertex3f(SKYBOX_RECTANGLE, -1.8, -SKYBOX_RECTANGLE);
    glTexCoord2f(1.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, -1.8, -SKYBOX_RECTANGLE);
    glTexCoord2f(0.0, 0.0);glVertex3f(-SKYBOX_RECTANGLE, -1.8, SKYBOX_RECTANGLE);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

//--------------------------------------------------------------------------------
void doricPillar()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[5]); // Using the ground texture for a sandstone-eqsue texture
    float vx[N] = {0.4, 0.4, 0.25, 0.3, 0.25, 0.25, 0.3, 0.25, 0.4, 0.4};
    float vy[N] = {0, 0.2, 0.3, 0.34, 0.38, 2.62, 2.66, 2.7, 2.8, 3 };
    float vz[N] = {0};
    float theta = M_PI/18.;

    float wx[N], wy[N], wz[N];

    for (int j = 0; j < 36; j++)
    {
        for (int i = 0; i < N; i++)
        {
            wx[i] = vx[i]*cos(theta) + vz[i]*sin(theta);
            wy[i] = vy[i];
            wz[i] = -vx[i]*sin(theta) + vz[i]*cos(theta);
        }
        glBegin((GL_TRIANGLE_STRIP));
        for(int i = 0; i < N; i++)
        {
            if (i>0){ normal(vx[i-1], vy[i-1], vz[i-1],
                        wx[i-1], wy[i-1], wz[i-1],
                        vx[i], vy[i], vz[i]
                        );}
            glTexCoord2f((float)j/36, (float)i/(N-1));
            glVertex3f(vx[i], vy[i], vz[i]);
            if (i>0){ normal(wx[i-1], wy[i-1], wz[i-1],
                        wx[i], wy[i], wz[i],
                        vx[i], vy[i], vz[i]);}
            glTexCoord2f((float)(j+1)/36, (float)i/(N-1));
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();
        for (int i = 0; i < N; i++)
        {
            vx[i] = wx[i];
            vy[i] = wy[i];
            vz[i] = wz[i];
        }
    }
    glDisable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void paintingframe()
{
    glColor4f(0.52, 0.37, 0.26, 0);
    glPushMatrix();
    glTranslatef(0,0.5,0);
    glScalef(1, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.45,0,0);
    glScalef(0.1, 1, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45,0,0);
    glScalef(0.1, 1, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-0.5,0);
    glScalef(1, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.2,0.2); glVertex3f(-0.5, -0.5, 0);
    glTexCoord2f(0.8,0.2);glVertex3f(0.5, -0.5, 0);
    glTexCoord2f(0.8,1);glVertex3f(0.5, 0.5, 0);
    glTexCoord2f(0.2,1);glVertex3f(-0.5, 0.5, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void museumbillboard()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glColor4f(yellow[0], yellow[1], yellow[2], 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,1); glVertex3f(-4, 0, 0);
    glTexCoord2f(0.9,1);glVertex3f(4, 0, 0);
    glTexCoord2f(0.9,0.8);glVertex3f(4, 1, 0);
    glTexCoord2f(0,0.8);glVertex3f(-4, 1, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
//--------------------------------------------------------------------------------
void animatedcylinder()
{

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glColor4f(0.1,0.1,0.1,0);
    for(float i = 0; i < 100; i++)
    {
        glPushMatrix();//cylinder 1
        glRotatef(theta*0.1*i,0,0,1);
        glTranslatef(0, -1.8 + (i*0.03),0);
        glRotatef(90,1,0,0);
        glutSolidCylinder(0.5, 0.1, 100,2);
        glPopMatrix();
    }
    glPushMatrix();
    glRotatef(theta*10,0,0,1);
    glTranslatef(0,-1.8+3,0);
    glScalef(1, 0.5, 1);
    glutSolidSphere(0.5, 100, 100);
    glPopMatrix();
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
}
//--------------------------------------------------------------------------------
void museumdoorblock()
{
    const int size = 6;
    float vx[size] = {0, 0.8, 0.8,1,1,0};
    float vy[size] = {0, 0, 0.8, 0.85, 1, 1};
    float vz[size] = {0};
    float wz[size] = {-1,-1,-1,-1,-1,-1};

    glBegin(GL_POLYGON);
    for(int i = 0; i<size; i++)
    {
        glVertex3f(vx[i], vy[i], vz[i]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for(int i = 0; i<size; i++)
    {
        glVertex3f(vx[i], vy[i], wz[i]);
    }
    glEnd();
    glBegin((GL_QUAD_STRIP));
    for(int i = 0; i < size; i++)
    {
        glVertex3f(vx[i], vy[i], vz[i]);
        if(i > 0) normal(vx[i-1], vy[i-1], wz[i-1],
                vx[i], vy[i], wz[i], vx[i], vy[i], vz[i]);
        glVertex3f(vx[i], vy[i], wz[i]);
    }
    glEnd();

}
//--------------------------------------------------------------------------------
void wallcube()//cube with missing top bottom and side vertices for optimising polygon count
{
    float points[] = {-0.5, 0.5};
    glBegin(GL_QUADS);
    glVertex3f(points[0], points[0], points[0]);
    glVertex3f(points[0], points[0], points[1]);
    glVertex3f(points[0], points[1], points[1]);
    glVertex3f(points[0], points[1], points[0]);
    glVertex3f(points[1], points[0], points[0]);
    glVertex3f(points[1], points[1], points[0]);
    glVertex3f(points[1], points[1], points[1]);
    glVertex3f(points[1], points[0], points[1]);
    glEnd();
}
//--------------------------------------------------------------------------------
void museumwall(int numsquares) // each wall is constructed from multiple cubes to have nice shadows and lighting
{

    float size = (float)1.0/numsquares;

    for (float i = 0; i < numsquares; i++)
    {
        for (float j = 0; j < numsquares; j++)
        {
            glPushMatrix();
            glTranslatef(0, -0.5 + size*j, -0.5 + size*(i+0.5));
            glScalef(1, size, size);
            wallcube();
            glPopMatrix();
        }
    }
}
//--------------------------------------------------------------------------------
void lightswitchmodel()
{
    glPushMatrix(); // lightswitch for pendulum
    glPushMatrix();

        glScalef(0.05, 0.1, 0.02);
        glColor4f(0.8,0.5,1,1);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glColor4f(0.8,0.8,0.8,0);
        glTranslatef(0,0,0.01);
        glScalef(0.02, 0.04, 0.01);
        glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}
//--------------------------------------------------------------------------------
void spotlight()
{
    glScalef(0.2,0.2,0.2);
    glColor4f(1,1,1,0);
    glPushMatrix();
    glScalef(1.5,1.5,0.5);
    glutSolidSphere(0.5,30,10);
    glPopMatrix();
    glPushMatrix();
    glColor4f(0.5,0.5,0.4,1);
    glutSolidCone(1,1,20,1);
    glPopMatrix();
}
//--------------------------------------------------------------------------------
void pendulumframe()
{
    glColor4f(0.6,0.5,0.5,1);

    glPushMatrix();
    glTranslatef(-1,-1,0);
    glScalef(0.25, 2.25, 0.5);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1,-1,0);
    glScalef(0.25, 2.25, 0.5);
    glutSolidCube(1);
    glPopMatrix();

    glColor4f(0.4,0.4,0.4,1);
    glPushMatrix();
    glTranslatef(-1,0,0);
    glRotatef(90,0,1,0);
    glutSolidCylinder(0.05,2,10,1);
    glPopMatrix();
}
//--------------------------------------------------------------------------------
void pendulum() // simple physics model of a pendulum
{
    if (lightswitch == 1) // check for lightswitch toggle
    {
        glDisable(GL_LIGHT3);
    } else {
        glEnable(GL_LIGHT3);
    }

    float light3_pos[] = {0, 0, 0, 1};
    float spot3_dir[] = {0.0,0.0,1.0};
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot3_dir);

    pendulumframe();

    float gravity = 9.8; // acceleration due to gravity in m/s^2
    float theta_init = M_PI*(1.0/5.0); // position from the (0,-1,0) direction, in radians
    float length = 1; // length of pendulum cable in metres
    /* Note the arc-length from the downward direction to any point during the swing
     is length*theta, and the force of the swing is equal to mass*gravity
        our function in terms of theta is used to determine position at given point t*/

    float frequency = sqrt(gravity/length);

    // theta is converted to degrees for opengl transformations
    float theta = (180.0/M_PI) * theta_init*cos(frequency*t);

    glRotatef(theta, 0,0,1); // rotate the pendulum and string depending on theta

    glPushMatrix();//Pendulum ball
        glTranslatef(0,-1,0.1);
        glRotatef(180,0,1,0);
        glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
        spotlight();
    glPopMatrix();
    glPushMatrix();//Pendulum string
        glRotatef(90,1,0,0);
        glutSolidCylinder(0.005, length, 10, 2);
    glPopMatrix();


}
//--------------------------------------------------------------------------------
void museum(int complexity)
{
// note we can increase the quad count for the museum walls to make lighting more complex at the cost of processing power

    glColor4f(yellow[0], yellow[1], yellow[2], 1);



    glPushMatrix();
    glTranslatef(2, -1.8,-10);
    doricPillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2, -1.8,-10);
    doricPillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3, -1.8,-10);
    doricPillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3, -1.8,-10);
    doricPillar();
    glPopMatrix();

    glPushMatrix();// roof
    glTranslatef(0, 1.4, -15);
    glScalef(8,0.5,12);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.9,-0.2,-16);
    glScalef(0.2, 3.2, 10);
        museumwall(complexity);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-0.2,-20.9);
    glRotatef(90, 0, 1,0);
    glScalef(0.2, 3.2, 8);
        museumwall(complexity); // walls
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.9,-0.2,-16);
    glScalef(0.2, 3.2, 10);
        museumwall(complexity); // walls
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -1, -11.2);
    glPushMatrix();
    glTranslatef(-4,-0.8,0);
    glScalef(4, 3.2, -0.2);
    museumdoorblock(); // door half one
    glPopMatrix();
    glPushMatrix();
            glTranslatef(4,-0.8,0);
            glScalef(-4, 3.2, -0.2);
            museumdoorblock();
        glPopMatrix();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,1.15,-8.9999);
        glScalef(0.5,0.5,0);
        museumbillboard();
    glPopMatrix();
}
//--------------------------------------------------------------------------------
void initialise()
{
    loadTexture(); // Load texture params

    glClearColor(1,1,1,0); //Background colour

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // spotlight on painting
    glEnable(GL_LIGHT1); // spotlight on cylinder
    glEnable(GL_LIGHT2); // spotlight on pendulum
    glEnable(GL_LIGHT3);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);



    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);

   glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, grey);
    glLightfv(GL_LIGHT0, GL_SPECULAR, black);


    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 7.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,200);


    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);

    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 18);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,200);

    glLightfv(GL_LIGHT3, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT3, GL_SPECULAR, white);

    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 12);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT,200);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45., 1., 1., 500.);
}

//--------------------------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   float light0_pos[4] = {0, 5., -1., 1.0};
    float light1_pos[] = {0, 0, 0, 1};
    float spot1_dir[] = {0.0,0.0,-1.0};
    float light2_pos[] = {0, 0, 0, 1};
    float spot2_dir[] = {0.0,0.0,-1.0};

    //the ambient scene light
    gluLookAt(eye_x, 0, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

    //this is to represent the light effects of 2 lamps
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot1_dir);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glPushMatrix();
    glTranslatef(0, 1, -15);
    glRotatef(-30,1,0,0);
    glPushMatrix();
    glTranslatef(0,-0.05,0.1);
    glRotatef(-30,1,0,0);
    glutSolidCylinder(0.02,0.5,10,1);
    glPopMatrix();
    spotlight();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0.8,-11.1);
    glRotatef(-90, 1,0,0);
    spotlight();
    glPopMatrix();

    //light for painting
    glPushMatrix();
    glTranslatef(-1,-1.7,-15.2);
    glRotatef(90, 0,1,0);
    glRotatef(30, 1,0,0);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot1_dir);
    glLightfv(GL_LIGHT2, GL_POSITION, light1_pos);
    spotlight();
    glPopMatrix();

    skybox();

    museum(20);

    // pendulum exhibit
    glPushMatrix();
        glTranslatef(3.4,0,-15);
        glRotatef(-90, 0,1,0);
        pendulum();
    glPopMatrix();

    // code for planar shadow of the animated cylinder
    float shadowMat[16] = {
                          25,0,0,0,
                          0,25,0,0,
                          0,0,21.85,-1,
                          0,0,0,1
                          };
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glMultMatrixf(shadowMat);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glTranslatef(0,-0.2,-20);
        glScalef(0.8, 0.8, 0.8);
        animatedcylinder();
    glPopMatrix();

    // magic cylinder exhibit
    glEnable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(0,-0.2,-20);
        glScalef(0.8, 0.8, 0.8);
        animatedcylinder();
    glPopMatrix();

    // frame exhibit
    glPushMatrix();
    glTranslatef(-3.7, 0, -15);
    glRotatef(-90, 0, 1, 0);
    paintingframe();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.8,0, -12);
    glRotatef(-90, 0, 1, 0);
    glScalef(2,2,2);
    lightswitchmodel();
    glPopMatrix();

    glutSwapBuffers();


}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
    glutInitWindowSize (1000, 1000);
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Museum");
    initialise();
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutTimerFunc(0, myTimer, 0);
    glutMainLoop();

    return 0;
}
