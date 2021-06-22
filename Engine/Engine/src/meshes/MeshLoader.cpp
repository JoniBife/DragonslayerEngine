#include <fstream>
#include <sstream>
#include "MeshLoader.h"

Mesh* MeshLoader::loadFromFile(const std::string& filePath) {
    std::ifstream meshFile;
    // ensure ifstream objects can throw exceptions:
    meshFile.exceptions(std::ifstream::badbit);
    try
    {
        // open files
        meshFile.open(filePath);

        //ifstream may set failbit when opening the file, however we do not want to consider it when calling getLine
        //failbit is set 
        if (meshFile.fail()) {
            std::cout << "Filestream ERROR [ Wavefront OBJ file not successfully read | Failbit occurred when opening the file ]" << std::endl;
            // streams must always be closed
            if (meshFile.is_open()) {
                meshFile.close();
            }

            // Mesh failed to load so we stop the engine
            exit(EXIT_FAILURE);
        }

        // read file's buffer contents into streams
        Mesh* mesh = createMeshFromFileStream(meshFile);
        // close file handlers
        meshFile.close();

        return mesh;
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Filestream ERROR [ Wavefront OBJ file not successfully read | Exception: " << e.what() << " ]" << std::endl;
        // streams must always be closed
        if (meshFile.is_open()) {
            meshFile.close();
        }

        // Mesh failed to load so we stop the engine
        exit(EXIT_FAILURE);
    }
}

Mesh* MeshLoader::createMeshFromFileStream(std::ifstream& meshFile) {

    std::vector<Vec4> verticesData;
    std::vector<Vec3> normalsData;
    std::vector<Vec2> textCoordsData;

    std::vector<unsigned int> verticesIdx, textCoordsIdx, normalsIdx;

    std::string line;
    while (std::getline(meshFile, line))
    {
        std::stringstream sline(line);
        std::string firstChar;
        sline >> firstChar;
        if (firstChar.compare("v") == 0) verticesData.push_back(parseVertex(sline));
        else if (firstChar.compare("vt") == 0) textCoordsData.push_back(parseTextCoord(sline));
        else if (firstChar.compare("vn") == 0) normalsData.push_back(parseNormal(sline));
        else if (firstChar.compare("f") == 0) parseFace(sline, verticesIdx, textCoordsIdx, normalsIdx, normalsData, textCoordsData);
    }

    Mesh* mesh = new Mesh();
    bool hasTextCoords = textCoordsData.size() > 0;
    bool hasNormals = normalsData.size() > 0;

    for (unsigned int i = 0; i < verticesIdx.size(); i++) {
        unsigned int vi = verticesIdx[i];
        mesh->vertices.push_back(verticesData[vi - 1]);

        if (hasTextCoords)
        {
            unsigned int ti = textCoordsIdx[i];
            mesh->textCoords.push_back(textCoordsData[ti - 1]);
        }
        if (hasNormals)
        {
            unsigned int ni = normalsIdx[i];
            mesh->normals.push_back(normalsData[ni - 1]);
        }
    }

    return mesh;
}

void MeshLoader::parseFace(std::stringstream& sline, 
    std::vector<unsigned int>& verticesIdx, 
    std::vector<unsigned int>& textCoordsIdx, 
    std::vector<unsigned int>& normalsIdx,
    std::vector<Vec3>& normalsData,
    std::vector<Vec2>& textCoordsData) {

    bool hasNormals = !normalsData.empty();
    bool hasTextCoords= !textCoordsData.empty();

    std::string token;
    if (!hasNormals && !hasTextCoords)
    {
        for (int i = 0; i < 3; i++)
        {
            sline >> token;
            verticesIdx.push_back(std::stoi(token));
        }
    }
    else if (!hasNormals && hasTextCoords)
    {
        for (int i = 0; i < 3; i++)
        {
            std::getline(sline, token, '/');
            if (token.size() > 0) verticesIdx.push_back(std::stoi(token));
            std::getline(sline, token, ' ');
            if (token.size() > 0) textCoordsIdx.push_back(std::stoi(token));
        }
    }
    else {
        for (int i = 0; i < 3; i++)
        {
            std::getline(sline, token, '/');
            if (token.size() > 0) verticesIdx.push_back(std::stoi(token));
            std::getline(sline, token, '/');
            if (token.size() > 0) textCoordsIdx.push_back(std::stoi(token));
            std::getline(sline, token, ' ');
            if (token.size() > 0) normalsIdx.push_back(std::stoi(token));
        }
    }
}

Vec4 MeshLoader::parseVertex(std::stringstream& sin) {
    Vec4 v;
    sin >> v.x >> v.y >> v.z >> v.w;

    if (v.w == 0.0f)
        v.w = 1.0f;

    return v;
}

Vec3 MeshLoader::parseNormal(std::stringstream& sin) {
    Vec3 n; 
    sin >> n.x >> n.y >> n.z;
    return n;
}

Vec2 MeshLoader::parseTextCoord(std::stringstream& sin) {
    Vec2 t; 
    sin >> t.x >> t.y; // x = u and y = v
    return t;
}