#include "../VectorLib/Vector.h"
#include "../Graphics3D/G3DUtilities.h"
#include <string>

class OBJImporter
{
public:
    static Triangle3d * importOBJ(string path, unsigned long& numTris);
};
