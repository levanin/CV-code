/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Cone.h"
#include "Plane.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "TextureBMP.h"
using namespace std;

const float ENABLE_AA = 1; // Enable AA 0 or 1
const float ENABLE_FOG = 1; // Enable fog 0 or 1

const float WIDTH = 25.0; // Adapted to allow for 16:9 aspect ratio - nice for generating desktop backgrounds
const float HEIGHT = 14.0625;
const float EDIST = 40.0;
const int NUMDIVX = 1600;
const int NUMDIVY = 900;
const int MAX_STEPS = 7;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
const float AMBIENT = 0.2f;
const float FOG_RANGE_1 = -50;
const float FOG_RANGE_2 = -300;
TextureBMP textures[6];

vector<SceneObject*> sceneObjects;

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0.69,0.71,0.72);						// Background/Fog Colour
    glm::vec3 lightPos(5, 20, -5);					//Light's position
    glm::vec3 color(0);
    SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
    obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found


    if (ray.index == 5) // Striped floor pattern
    {
        //Stripe pattern
        int stripeWidth = 5;
        int iz = (ray.hit.z) / stripeWidth;
        int k = iz % 2; //2 colors
        int ix = ((ray.hit.x)-50) / stripeWidth;
        int j = ix % 2;
        if (k == 0 && j!=0) color = glm::vec3(1, 0, 1);
        else if (k!=0 && j==0) color = glm::vec3(1,0,1);
        else color = glm::vec3(0.3, 0.55, 0.25);
        obj->setColor(color);

    }

    if (ray.index == 0) // Texture mapping for front wall
    {
        float texcoords = ((ray.hit.x +50.)/100.) +0.0206;
        float texcoordt = (ray.hit.y + 40.)/110.;
        if(texcoords >= 1) texcoords = texcoords - 1; // this is to correct a strange bug related to BMPs being read incorrectly
        color = textures[0].getColorAt(texcoords, texcoordt);
    }


    if (ray.index == 1) // Texture mapping for right wall
    {
        float texcoords = (ray.hit.z + 200.)/200. + 0.0206;
        float texcoordt = (ray.hit.y + 40.)/110.;
        if(texcoords >= 1) texcoords = texcoords - 1;
        color = textures[1].getColorAt(texcoords, texcoordt);
    }


    if (ray.index == 2) // Texture mapping for left wall
    {
        float texcoords = (ray.hit.z)/-200. + 0.02;
        float texcoordt = (ray.hit.y + 40.)/110.;
        if(texcoords >= 1) texcoords = texcoords - 1;
        color = textures[4].getColorAt(texcoords, texcoordt);
    }


    if (ray.index == 3) // Texture mapping for top wall
    {
        float texcoordt = (ray.hit.x + 50.)/100.;
        float texcoords = (ray.hit.z)/-200. + 0.02;
        if(texcoords >= 1) texcoords = texcoords - 1;
        color = textures[3].getColorAt(texcoords, texcoordt);
    }

    if (ray.index == 4) // Texture mapping for back wall
    {
        float texcoords = (ray.hit.x - 50)/-100. + 0.02;
        float texcoordt = (ray.hit.y + 40.)/110.;
        if(texcoords >= 1) texcoords = texcoords - 1;
        color = textures[2].getColorAt(texcoords, texcoordt);
    }



    if (ray.index == 8) // Texture mapping for marble sphere
    {
        glm::vec3 d = -obj->normal(ray.hit);
        float texcoordu = 0.5 + atan2(d.x, d.z)/(M_PI*2);
        float texcoordv = 0.5 - asin(d.y)/M_PI;
        color=textures[5].getColorAt(texcoordu, texcoordv);
        obj->setColor(color);
    }

    if (ray.index > 4)
    { // disregard skyboxes for lighting and shadow calculations:
        color = obj->lighting(lightPos, -ray.dir, ray.hit);	//Get Object's colour with lighting

        glm::vec3 lightVec = lightPos - ray.hit;
        Ray shadowRay(ray.hit, lightVec);
        shadowRay.closestPt(sceneObjects);

        float transShadow = 1;
        if (sceneObjects[shadowRay.index]->isTransparent() || sceneObjects[shadowRay.index]->isRefractive()) transShadow = 1.8; // adjust shadow intensity if object is transparent or reflective

        if (shadowRay.index > 4) color = (transShadow * AMBIENT) * obj->getColor(); // disregard skyboxes for shadow calculation
    }
    if (obj->isReflective() && step < MAX_STEPS)
    {
        float rho = obj->getReflectionCoeff();
        glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
        Ray reflectedRay(ray.hit, reflectedDir);
        glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
        color = color + (rho * reflectedColor);
    }

    if (obj->isTransparent() && step < MAX_STEPS)
    {
        Ray transRay(ray.hit, ray.dir);
        transRay.closestPt(sceneObjects);
        Ray outRay(transRay.hit , transRay.dir);
        glm::vec3 transColor = trace(outRay, step+1);
        color = color + (obj->getTransparencyCoeff() * transColor);
    }

    if (obj->isRefractive() && step < MAX_STEPS)
    {
        float eta = obj->getRefractiveIndex();
        glm::vec3 n = obj->normal(ray.hit);
        glm::vec3 g = glm::refract(ray.dir,n,eta);
        Ray refrRay(ray.hit, g);
        refrRay.closestPt(sceneObjects);
        n = obj->normal(refrRay.hit);
        g = glm::refract(g,-n, 1.0f/eta);
        Ray outRay(refrRay.hit, g);
        glm::vec3 refrColor = trace(outRay, step+1);
        color = color + (obj->getRefractionCoeff() * refrColor);
    }

    if (ENABLE_FOG == 1)
    {
        float t = (ray.hit.z - FOG_RANGE_1)/(FOG_RANGE_2 - FOG_RANGE_1);
        color = (1-t)*color + t*backgroundCol;
    }

    return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  //grid point
    float cellX = (XMAX-XMIN)/NUMDIVX;  //cell width
    float cellY = (YMAX-YMIN)/NUMDIVY;  //cell height
    glm::vec3 eye(0., 0., 0.);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  //Each cell is a tiny quad.

    for(int i = 0; i < NUMDIVX; i++)	//Scan every cell of the image plane
    {
        xp = XMIN + i*cellX;
        for(int j = 0; j < NUMDIVY; j++)
        {
            yp = YMIN + j*cellY;

            glm::vec3 col;

            if (ENABLE_AA)
            {
                // Supersampling Implementation
                glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);	//direction of the primary ray
                glm::vec3 dir2(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
                glm::vec3 dir3(xp+0.75*cellX, yp+0.75*cellY, -EDIST);
                glm::vec3 dir4(xp+0.25*cellX, yp+0.75*cellY, -EDIST);

                Ray ray1 = Ray(eye, dir1);
                Ray ray2 = Ray(eye, dir2);
                Ray ray3 = Ray(eye, dir3);
                Ray ray4 = Ray(eye, dir4);

                glm::vec3 col1 = trace (ray1, 1); //Trace the primary ray and get the colour value
                glm::vec3 col2 = trace (ray2, 1);
                glm::vec3 col3 = trace (ray3, 1);
                glm::vec3 col4 = trace (ray4, 1);

                col = (col1 + col2 + col3 + col4) * 0.25f;
            }
            else
            {
                glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);
                Ray ray = Ray(eye, dir);
                col = trace (ray, 1);
            }

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);				//Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    textures[0] = TextureBMP("bluecloud_ft.bmp");
    textures[1] = TextureBMP("bluecloud_lf.bmp");
    textures[2] = TextureBMP("bluecloud_bk.bmp");
    textures[3] = TextureBMP("bluecloud_up.bmp");
    textures[4] = TextureBMP("bluecloud_rt.bmp");
    textures[5] = TextureBMP("marble.bmp");

    // SKYBOX PLANES

    Plane *front = new Plane (glm::vec3(-50., -40, -200), //Front face
                              glm::vec3(50, -40, -200), //Point B
                              glm::vec3(50., 70, -200), //Point C
                              glm::vec3(-50., 70, -200)); //Point D
    front->setColor(glm::vec3(1,1,1));
    sceneObjects.push_back(front);

    Plane *right = new Plane (glm::vec3(50., -40, -200), //Right face
                              glm::vec3(50, -40, 0), //Point B
                              glm::vec3(50., 70, 0), //Point C
                              glm::vec3(50., 70, -200)); //Point D
    right->setColor(glm::vec3(1,1,1));;
    sceneObjects.push_back(right);

    Plane *left = new Plane (glm::vec3(-50., -40, -200), //Left face
                             glm::vec3(-50, -40, 0), //Point B
                             glm::vec3(-50., 70, 0), //Point C
                             glm::vec3(-50., 70, -200)); //Point D
    left->setColor(glm::vec3(1,1,1));
    sceneObjects.push_back(left);


    Plane *top = new Plane (glm::vec3(-59., 70, 0), //Top face
                            glm::vec3(50, 70, 0), //Point B
                            glm::vec3(50, 70, -200), //Point C
                            glm::vec3(-50., 70, -200)); //Point D
    top->setColor(glm::vec3(1,1,1));
    sceneObjects.push_back(top);

    Plane *back = new Plane (glm::vec3(50., -40, 0), //Back face
                             glm::vec3(-50, -40, 0), //Point B
                             glm::vec3(-50., 70, 0), //Point C
                             glm::vec3(50., 70, 0)); //Point D
    back->setColor(glm::vec3(1,1,1));
    sceneObjects.push_back(back);

    Plane *floor = new Plane (glm::vec3(-100., -15, 0), //Floor
                              glm::vec3(100, -15, 0), //Point B
                              glm::vec3(100, -15, -200), //Point C
                              glm::vec3(-100., -15, -200)); //Point D
    floor->setColor(glm::vec3(1,0,1));
    floor->setSpecularity(false);
    floor->setReflectivity(true, 0.8);
    sceneObjects.push_back(floor);

    //Mirror Sphere
    Sphere *sphere1 = new Sphere(glm::vec3(-10.0, -5.0, -130.0), 10.0);
    sphere1->setColor(glm::vec3(0.8, 0.2, 0.2));
    sphere1->setReflectivity(true, 0.8);
    sceneObjects.push_back(sphere1);

    // Refractive sphere
    Sphere *sphere2 = new Sphere(glm::vec3(15.0, -10.0, -150.0), 5.0);
    sphere2->setColor(glm::vec3(0, 0.1, 0.1));   //Set colour to blue
    //sphere2->setTransparency(true, 0.9);
    sphere2->setRefractivity(true, 1, 1/1.02);
    sphere2->setReflectivity(true, 0.1);
    sceneObjects.push_back(sphere2);

    //Textured Sphere
    Sphere *sphere3 = new Sphere(glm::vec3(-18.0, -7.2, -90.0), 2.0);
    sphere3->setReflectivity(true, 0.1);
    sceneObjects.push_back(sphere3);

    //transparent Cone
    Cone *cone1 = new Cone(glm::vec3(-18,-15,-90),2,6);
    cone1->setColor(glm::vec3(1,0,1));
    cone1->setTransparency(true, 0.4);
    cone1->setReflectivity(true, 0.1);
    sceneObjects.push_back(cone1);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
