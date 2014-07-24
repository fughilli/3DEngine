#include "VectorLib/Vector.h"
#include "3DEngineRenderModule/G3DUtilities.h"
#include <string>

class OBJImporter
{
public:
    static Triangle3d * importOBJ(string path, unsigned long& numTris);
};
