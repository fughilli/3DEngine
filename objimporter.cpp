#include "objimporter.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <limits>
#include <cstdlib>

//#define DEBUG

using namespace std;

static const unsigned long lineBufferSize = 512;

static unsigned long countIdentifiers(ifstream& file, unsigned long& pos, const char identifier)
{
    file.seekg(0);

    unsigned long ret = 0;

    char linebuffer[lineBufferSize];

    pos = numeric_limits<unsigned long>::max();

    unsigned long tempPos;

    while(true)
    {
        tempPos = file.tellg();

        file.getline(linebuffer, lineBufferSize);

        if(linebuffer[0] == identifier)
        {
            ret++;

            if(pos == numeric_limits<unsigned long>::max())
                pos = tempPos;
        }

        if(file.rdstate() & file.eofbit)
        {
            break;
        }
    }

    return ret;
}

static unsigned long countTris(ifstream& file, unsigned long& pos)
{
    return countIdentifiers(file, pos, 'f');
}

static unsigned long countVerts(ifstream& file, unsigned long& pos)
{
    return countIdentifiers(file, pos, 'v');
}

Triangle3d* OBJImporter::importOBJ(string path, unsigned long& numTris)
{
    ifstream file;
    file.open(path);

    unsigned long startPos;
    unsigned long numVerts = countVerts(file, startPos);

    char linebuffer[lineBufferSize];

    char identifierChar;

    Vector3d* verts = (Vector3d*)malloc(sizeof(Vector3d)*numVerts);

    file.close();
    file.open(path);
    file.seekg(startPos);

    for(unsigned long i = 0; i < numVerts; i++)
    {
        file.getline(linebuffer, lineBufferSize);

        sscanf(linebuffer, "%c %lf %lf %lf", &identifierChar, &(verts[i].x), &(verts[i].y), &(verts[i].z));

#if defined(DEBUG)
        cout << verts[i] << endl;
#endif
    }

    file.close();
    file.open(path);

    numTris = countTris(file, startPos);

    Triangle3d* ret = (Triangle3d*)malloc(sizeof(Triangle3d)*numTris);

    file.close();
    file.open(path);
    file.seekg(startPos);

    for(unsigned long i = 0; i < numTris; i++)
    {
        file.getline(linebuffer, lineBufferSize);

        int vert1i, vert2i, vert3i;

        sscanf(linebuffer, "%c %d %d %d", &identifierChar, &vert1i, &vert2i, &vert3i);

#if defined(DEBUG)
        cout << vert1i << ", " << vert2i << ", " << vert3i << endl;
#endif

        ret[i] = {verts[vert1i - 1],verts[vert2i - 1],verts[vert3i - 1]};
    }

    free(verts);

    return ret;
}


