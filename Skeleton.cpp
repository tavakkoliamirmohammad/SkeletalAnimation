#include "Skeleton.h"
#include <iostream>

void Skeleton::addBone(Bone *parent, Bone *bone, glm::vec3 translation, glm::vec3 rotation) {
    if (parent == nullptr) {
        this->root = bone;
    } else {
        parent->children.push_back(bone);
        bone->parent = parent;

        bone->mi_p = glm::identity<glm::mat4>();
        bone->mi_p = glm::translate(bone->mi_p, glm::vec3(parent->length, 0, 0));
        bone->mi_p = glm::translate(bone->mi_p, translation);
        bone->mi_p = glm::rotate(bone->mi_p, rotation.z, glm::vec3(0, 0, 1));
        bone->mi_p = glm::rotate(bone->mi_p, rotation.y, glm::vec3(0, 1, 0));
        bone->mi_p = glm::rotate(bone->mi_p, rotation.x, glm::vec3(1, 0, 0));
    }

    this->boneNames.push_back(bone->name);
    this->bones[bone->name] = bone;
}

std::vector<std::string> Skeleton::getBoneNames() {
    return this->boneNames;
}

Bone *Skeleton::getBone(const std::string &boneName) {
    if (this->bones.count(boneName) != 0) {
        return this->bones[boneName];
    } else {
        return nullptr;
    }
}

void Skeleton::calculate_mi_d() {
    this->root->calculate_mi_d();
}

void Skeleton::calculate_mi_a() {
    this->root->calculate_mi_a();
}

void Skeleton::update_mi_l(float animatedTime) {
    int index = animatedTime / 5000;
    if (index == numberOfKeyFrames - 1) return;
    float time = (index + 1) * 5000.0;
    auto prevKeyFrame = keyFrames[index];
    auto nextKeyFrame = keyFrames[index + 1];
    for (const auto &name: boneNames) {
        this->getBone(name)->rotate(slerp(prevKeyFrame[name], nextKeyFrame[name], 1 - (time - animatedTime) / 5000));
    }
}

void Skeleton::setKeyFrame() {
    std::map<std::string, glm::quat> frame;
    for (const std::string &boneName: boneNames) {
        frame[boneName] = getBone(boneName)->getQuat();
    }
    keyFrames[numberOfKeyFrames++] = frame;
}

glm::quat Skeleton::slerp(glm::quat q1, glm::quat q2, float u) {
    const float theta = glm::acos(glm::dot(q1, q2));
    return sinf((1 - u) * theta) / sinf(theta) * q1 + sinf(u * theta) / sinf(theta) * q2;
}