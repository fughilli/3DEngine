import Image as im
import os

def ppmToPng(filename, verbose = False):
    ppmfile = open(filename)

    # read the first line (should be P1, P2, or P3)
    filetype = ppmfile.readline()

    # dimensions are next; parse as an array
    dims = [int(s) for s in ppmfile.readline().split() if s.isdigit()]

    if(verbose):
        print "dims: ", dims

    # then the color depth; parse as the first int on line 3
    colordepth = int(ppmfile.readline().split()[0])

    if(verbose):
        print "color depth: ", colordepth

    # read the remainder of the ppm file (the raw color data)
    strimgdata = ppmfile.read()

    if(verbose):
        print "string image data: ", strimgdata

    # parse the data into an array of integers
    imgdata = [int(s) for s in strimgdata.split() if s.isdigit()]

    if(verbose):
        print "image data: ", imgdata

    # break the array into 3-tuples (RGB)
    imgdatatuples = [tuple(imgdata[i:i+3]) for i in range(0, len(imgdata), 3)]

    if(verbose):
        print "image data as RGB tuples: ", imgdatatuples

    # create a new image with PIL and load its pixel data structure
    expimg = im.new("RGB", dims)
    pix = expimg.load()

    # write the 3-tuples into the pixel data structure
    for i in range(0, dims[0]):
        for j in range(0, dims[1]):
            pix[j,i] = imgdatatuples[(i*dims[0])+j]

    # save the image to the original filename with ".png" appended
    string = os.path.basename(ppmfile.name)
    nameNoExt = [string[:i] for i in range(len(string)-1,0,-1) if (string[i] == "." or i == 0)][0]
    string = ppmfile.name
    pathNoName = [string[:i] for i in range(len(string),0,-1) if (string[i-1] == "/" or i == 1)][0]

    if(len(pathNoName) == 0):
        pathNoName = "."

    os.mkdir(pathNoName + "png/")
    expimg.save(pathNoName + "png/" + nameNoExt + ".png")

    
    
    
def objToHeader(filename, verbose = False):
    objfile = open(filename)

    lines = objfile.read().split("\n")
    linesNoComments = []
    for line in lines:
        # seek through the string and pick the substring starting at zero to i when i is the position of the comment marker '#'
        lineNoComment = [line[0:i] for i in range(0, len(line)+1) if ( (i == (len(line)) or (line[i] == "#") ) )][0]
        if(len(lineNoComment) > 0 and not lineNoComment.isspace()):
            linesNoComments.append(lineNoComment)

    if(verbose):
        print linesNoComments

    # split all the lines by spaces
    splitLines = [x.split() for x in linesNoComments]

    # keep only the lines that have identifiers
    splitLinesWithIdentifiers = [x for x in splitLines if (x[0] == "v" or x[0] == "o" or x[0] == "f")]

    #objectLines = [x for x in splitLinesWithIdentifiers if (x[0] == "o")]
    #print [x[1] for x in objectLines]

    # find the indices of the object lines "o <name>"
    splitIndices = [i for i in range(0, len(splitLinesWithIdentifiers)) if splitLinesWithIdentifiers[i][0] == "o"]

    # append the length of the line list (to close the last list slice)
    splitIndices.append(len(splitLinesWithIdentifiers))

    if(verbose):
        print splitIndices

    # slice the list based upon those split indices
    objList = [splitLinesWithIdentifiers[splitIndices[i]:splitIndices[i+1]] for i in range(0, len(splitIndices)-1)]

    # extract the names of all of the objects
    objNameList = [x[1] for objLines in objList for x in objLines if x[0] == "o"]

    if(verbose):
        print objNameList

    # extract the vertices of all of the objects
    objVertexList = [[x[1:] for x in objLines if x[0] == "v"] for objLines in objList]

    if(verbose):
        print objVertexList

    # extract the triangle indices of all of the objects
    objTriangleIndexList = [[x[1:] for x in objLines if x[0] == "f"] for objLines in objList]

    if(verbose):
        print objTriangleIndexList

    # create the triangle lists from the triangle index and vertex lists (v = vertex in t = triangle in o = object)
    objTriangleList = [[[objVertexList[o][int(v) - 1] for v in objTriangleIndexList[o][t]] for t in range(0, len(objTriangleIndexList[o]))] for o in range(0, len(objNameList)) ]

    if(verbose):
        print objTriangleList

    string = os.path.basename(objfile.name)
    nameNoExt = [string[:i] for i in range(len(string)-1,0,-1) if (string[i] == "." or i == 0)][0]

    string = objfile.name
    pathNoName = [string[:i] for i in range(len(string),0,-1) if (string[i-1] == "/" or i == 1)][0]

    if(len(pathNoName) == 0):
        pathNoName = "."

    try:
        os.mkdir(pathNoName + "headers/")
    except Exception:
        pass

    # create a new .h file under ./headers with the same name as the obj file
    outputfile = open(pathNoName + "headers/" + nameNoExt + ".h", "w")

    guardSymbol = nameNoExt.upper().strip().replace(" ", "_") + "_H"
    outputfile.write("#ifndef " + guardSymbol + "\n")
    outputfile.write("#define " + guardSymbol + "\n")

    outputfile.write("#include \"G3DUtilities.h\"\n\n")

    sizeSymbols = [objName.upper().strip().replace(" ", "_") + "_MESH_SIZE" for objName in objNameList]

    for o in range(0, len(objNameList)):
        
        outputfile.write("#define " + sizeSymbols[o] + " " + str(len(objTriangleList[o])) + "\n\n")
        
        outputfile.write("extern const Triangle3d " + objNameList[o] + "_mesh[" + sizeSymbols[o] + "];\n\n")

    outputfile.write("#endif // " + guardSymbol + "\n")

    outputfile.flush()
    outputfile.close()

    # create a new .cpp file under ./headers with the same name as the obj file
    outputfile = open(pathNoName + "headers/" + nameNoExt + ".cpp", "w")

    outputfile.write("#include \"" + nameNoExt + ".h\"\n\n")

    for o in range(0, len(objNameList)):
        outputfile.write("const Triangle3d " + objNameList[o] + "_mesh[" + sizeSymbols[o] + "] = {\n")

        outputfile.write(",\n".join(["{ {" + vu + "} }" for vu in ["}, {".join([", ".join(v) for v in t]) for t in objTriangleList[o]]]))          

        outputfile.write("\n};\n\n");

    outputfile.flush()
    outputfile.close()

    
