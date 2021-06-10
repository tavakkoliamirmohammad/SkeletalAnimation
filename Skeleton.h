#ifndef ANIMATED_SKELETON_SKELETON_H
#define ANIMATED_SKELETON_SKELETON_H

#include "Bone.h"
#include <map>

class Skeleton {
public:
    void addBone(Bone *parent, Bone *bone, glm::vec3 translation, glm::vec3 rotation);

    std::vector<std::string> getBoneNames();

    Bone *getBone(const std::string& boneName);

    void calculate_mi_d();

    void calculate_mi_a();


private:
    Bone *root;
    std::vector<std::string> boneNames;
    std::map<std::string, Bone *> bones;
};

#endif //ANIMATED_SKELETON_SKELETON_H
