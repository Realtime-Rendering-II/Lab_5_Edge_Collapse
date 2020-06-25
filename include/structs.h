#ifndef LAB_5_STRUCTS_H
#define LAB_5_STRUCTS_H

#include <glm/glm.hpp>
#include <vector>

struct Triangle;
struct Edge;

struct Vertex{
    glm::vec3 position;
    std::vector<Triangle*> incident_triangles;
    std::vector<Edge*> incident_edges;
};

struct Edge{
    Vertex* vertex_a;
    Vertex* vertex_b;
    std::vector<Triangle*> incident_triangles;
    float cost;
};

struct Triangle{
    Vertex* vertex_a;
    Vertex* vertex_b;
    Vertex* vertex_c;
    Edge* a_b;
    Edge* a_c;
    Edge* b_c;
};
#endif //LAB_5_STRUCTS_H
