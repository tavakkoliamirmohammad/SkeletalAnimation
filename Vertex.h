#ifndef ANIMATED_SKELETON_VERTEX_H
#define ANIMATED_SKELETON_VERTEX_H

struct Vertex {
    float default_x;
    float default_y;
    float animated_x;
    float animated_y;
    int bone_id1;
    int bone_id2;
    double bone_weight1;
    double bone_weight2;
};


#endif //ANIMATED_SKELETON_VERTEX_H
