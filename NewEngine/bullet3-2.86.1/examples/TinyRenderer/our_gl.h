#ifndef __OUR_GL_H__
#define __OUR_GL_H__
#include "tgaimage.h"
#include "geometry.h"




Matrix viewport(int x, int y, int w, int h);
Matrix projection(float coeff=0.f); // coeff = -1/c
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up);

struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

void triangle(mat<4,3,float> &pts, IShader &shader, TGAImage &image, float *zbuffer, const Matrix& viewPortMatrix);
void triangle(mat<4,3,float> &pts, IShader &shader, TGAImage &image, float *zbuffer, int* segmentationMaskBuffer, const Matrix& viewPortMatrix, int objectIndex);
#endif //__OUR_GL_H__

