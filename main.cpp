#ifndef ENERGIA

#include <cmath>
#include <iostream>
#include "image.h"
#include "ImageDisplay.h"
#include "3DEngineRenderModule/DrawableObject.h"
#include "VectorLib/Vector.h"
#include "objimporter.h"
#include <cstdio>
#include <cstdlib>
#include "res/headers/monkey.h"

using namespace std;

Camera3d cam;
MeshObject3d mesh;

Triangle3d triMesh[] =
{
    {{0,0,50}, {0,50,0}, {50,-50,0}},
    {{0,0,50}, {50,-50,0}, {-50,-50,0}},
    {{0,0,50}, {-50,-50,0}, {0,50,0}},
    {{-50,-50,0}, {0,50,0}, {50,-50,0}}
};

#define TEST_STATIC

int main(void)
{

#ifdef TEST_STATIC
    mesh.mesh = (Triangle3d*)(void *)Suzanne_mesh;
    mesh.numTris = SUZANNE_MESH_SIZE;
#else
    unsigned long numTris;
    mesh.mesh = OBJImporter::importOBJ("res/cube.obj", numTris);
    mesh.numTris = numTris;
#endif

    mesh.scale(200);

    mesh.setPos({0,0,0});

    cam.focalLength = 6000;
    cam.setPos({0, -350, 50});

    cam.forward = Vector3d::j;
    cam.up = Vector3d::k;

    Image img(500, 500);

    ImageDisplay id(img);

    char filename[150];

    for(double i = 0; i < TWO_PI; i+=0.5)
    {
        Quaternion rotQuat = Quaternion::fromAxisAngle(Vector3d::k, i);
        Quaternion baseRotQuat = Quaternion::fromAxisAngle(Vector3d::i, PI/2);
        mesh.setRot(rotQuat * baseRotQuat);

        Quaternion camRotQuat = Quaternion::fromAxisAngle(Vector3d::i, -i/10);
        cam.setRot(camRotQuat);
        cam.translate({0,0,1});

        cout << rotQuat << endl;

        id.clear({255,0,0});
        mesh.draw(cam, id);


        sprintf(filename, "images/imgfile%d.ppm", (int)(i*100));

        img.writeToFile(filename);
    }

#ifndef TEST_STATIC
    free(mesh.mesh);
#endif

    return 0;
}

#endif
