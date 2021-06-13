#ifndef ANIMATED_SKELETON_SKELETON_H
#define ANIMATED_SKELETON_SKELETON_H

#include "Bone.h"
#include <map>

class Skeleton {
public:
    void addBone(Bone *parent, Bone *bone, glm::vec3 translation, glm::vec3 rotation);

    std::vector<std::string> getBoneNames();

    Bone *getBone(const std::string &boneName);

    void calculate_mi_d();

    void calculate_mi_a();

    void setKeyFrame();

    void update_mi_l(float currentTime);

    static glm::quat slerp(glm::quat q1, glm::quat q2, float t);

private:
    Bone *root;
    std::vector<std::string> boneNames;
    std::map<std::string, Bone *> bones;
    std::map<int, std::map<std::string, glm::quat>> keyFrames;
    int numberOfKeyFrames = 0;
};

#endif //ANIMATED_SKELETON_SKELETON_H
