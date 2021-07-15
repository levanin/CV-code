/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>
const float EPSILON = 0.02f; // margin of error for asymptotes - prevents artifacts
/**
* Cones's intersection method.  The input is a ray.
*/
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
float m = (radius*radius)/(height*height);
glm::vec3 H = glm::vec3(center.x, center.y + height, center.z); // the point at the tip of the cone
glm::vec3 h = glm::vec3(0,-height,0); // axis of the cone
glm::vec3 hhat = glm::normalize(h);
glm::vec3 w = p0 - H;
glm::vec3 v = dir;

float a = glm::dot(v,v) - (m+1) * pow(glm::dot(v, hhat),2);
float b = 2 * (glm::dot(v, w) - m * glm::dot(v, hhat) * glm::dot(w, hhat) - glm::dot(v, hhat) * glm::dot(w, hhat));
float c = glm::dot(w, w) - (m+1) * pow(glm::dot(w, hhat),2);


float delta = b*b - (4*a*c);


if(fabs(delta) < EPSILON) return -1.0; //tangent to cone

float t1 = (-b - sqrt(delta))/(2*a);
float t2 = (-b + sqrt(delta))/(2*a);
if(fabs(t1) < EPSILON )
{
    if (t2 > 0) return t2;
    else t1 = -1.0;
}
if(fabs(t2) < EPSILON ) t2 = -1.0;

float sect = (t1 < t2)? t1: t2;
glm::vec3 secpoint = p0 + sect*v;
if (secpoint.y < center.y || secpoint.y > H.y) return -1; //check point of intersection lies within the cones height

return sect;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    float theta = atan(radius/height);
    float alpha = atan((p.x-center.x)/(p.z-center.z));
    glm::vec3 n = glm::vec3(sin(alpha)*cos(theta), sin(theta), cos(alpha)*cos(theta)); // See page 44 of slides
    return n;
}
