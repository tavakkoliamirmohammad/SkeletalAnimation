#ifndef ANIMATED_SKELETON_SCENE_H
#define ANIMATED_SKELETON_SCENE_H

#pragma once

#include "Skeleton.h"
#include "Vertex.h"

class Scene {
public:
    Scene() : selectedBone(nullptr) {
        this->skeleton = new Skeleton();
    }

    void addBone(const std::string& parent, std::string boneName, int length, float angle);

    Bone *getBone(const std::string& boneName);

    Bone *getBone(int boneIndex);

    int getCount();

    Bone *getSelectedBone();

    void selectBone(const std::string& boneName);

    void selectBone(int boneIndex);

    void rotateSelectedBone(float delta_theta);

    void setSkin(std::vector <Vertex> skin);

    void inverseKinematic(glm::vec2 pos);

    void init();

    void update();

    void render();

private:
    std::vector <Vertex> skin;
    Skeleton *skeleton;
    Bone *selectedBone;
};

#endif //ANIMATED_SKELETON_SCENE_H
