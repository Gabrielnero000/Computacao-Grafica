#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include "OBJ_Loader.h"

using namespace glm;

std::vector<vec4> loadMesh(const std::string &file_name){
    std::vector<vec4> vec_array;
    objl::Loader loader;
    bool load_out = loader.LoadFile(file_name);
    if(load_out){
        objl::Mesh mesh = loader.LoadedMeshes[0];
        for(int i = 0; i < mesh.Vertices.size(); i++){
            vec_array.push_back(vec4(mesh.Vertices[i].Position.X,
                                     mesh.Vertices[i].Position.Y,
                                     mesh.Vertices[i].Position.Z, 1.0));
        }

    }
    return vec_array;
}
