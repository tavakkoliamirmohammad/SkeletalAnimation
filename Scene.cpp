#include "Scene.h"

#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <algorithm>
#include <utility>

void Scene::addBone(const std::string &parent, std::string boneName, int length, float angle) {
    Bone *parentBone = this->getBone(parent);

    Bone *bone = new Bone(length, std::move(boneName));

    this->skeleton->addBone(parentBone, bone, glm::vec3(), glm::vec3(0, 0, angle));
}

Bone *Scene::getBone(const std::string &boneName) {
    return this->skeleton->getBone(boneName);
}

Bone *Scene::getBone(int boneIndex) {
    auto boneNames = this->skeleton->getBoneNames();
    return this->getBone(boneNames[boneIndex]);
}

int Scene::getCount() {
    return this->skeleton->getBoneNames().size();
}

void Scene::selectBone(const std::string &boneName) {
    auto bone = this->getBone(boneName);
    if (bone != nullptr) {
        this->selectedBone = bone;
    }
}

void Scene::selectBone(int boneIndex) {
    auto bone = this->getBone(boneIndex);
    if (bone != nullptr) {
        if (this->selectedBone == bone) {
            this->selectedBone = nullptr;
        } else {
            this->selectedBone = bone;
        }
    }
}

Bone *Scene::getSelectedBone() {
    return this->selectedBone;
}

void Scene::rotateSelectedBone(float delta_theta) {
    if (selectedBone != nullptr) {
        selectedBone->rotate(glm::vec3(0, 0, delta_theta));
    }
}

void Scene::setSkin(std::vector<Vertex> skin) {
    this->skin = std::move(skin);
}

void Scene::inverseKinematic(glm::vec2 pos) {
    auto target = glm::vec3(pos.x, pos.y, 1);
    Bone *endEffectorBone = this->getBone(this->getCount() - 1);
    glm::vec3 endEffectorVec;
    endEffectorVec = endEffectorBone->transform_from_bonespace(
            glm::vec3(endEffectorBone->getLength(), 0, 0));

    auto boneNames = skeleton->getBoneNames();
    std::reverse(boneNames.begin(), boneNames.end());
    int iteration = 0;

    while (glm::length(target - endEffectorVec) >= 3 && iteration++ < 4) {
        for (const auto &boneName : boneNames) {
            Bone *bone = this->getBone(boneName);
            glm::vec3 boneStartPosition = bone->transform_from_bonespace(glm::vec3(0, 0, 0));

            glm::vec3 endEffectorPosition = endEffectorBone->transform_from_bonespace(
                    glm::vec3(endEffectorBone->getLength(), 0, 0));

            auto u = glm::length(endEffectorPosition - boneStartPosition);
            auto f = glm::length(target - boneStartPosition);
            auto g = glm::length(target - endEffectorPosition);

            auto alpha = glm::acos((u * u + f * f - g * g) / (2 * u * f));

            auto theta = bone->getTheta().z;

            auto a = endEffectorPosition - boneStartPosition;
            auto b = target - boneStartPosition;

            auto crossProduct = glm::cross(a, b);
            if (crossProduct.z > 0) {
                alpha += theta;
            } else {
                alpha = theta - alpha;
            }
            if (glm::abs(alpha) < 0.001) continue;
            bone->rotate(glm::vec3(0, 0, alpha));
            bone->calculate_mi_a();
        }

    }
}

void Scene::init() {
    this->skeleton->calculate_mi_d();
}

void Scene::update() {
    if (isAnimating) {
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = currentTime - start;
        this->skeleton->update_mi_l(std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    }
    this->skeleton->calculate_mi_a();

    for (Vertex &vert : this->skin) {
        Bone *bone1 = this->getBone(vert.bone_id1);
        Bone *bone2 = this->getBone(vert.bone_id2);

        glm::vec3 default_pos = glm::vec3(vert.default_x, vert.default_y, 1);

        glm::vec3 pos1 = bone1->transform(default_pos);
        glm::vec3 pos2 = bone2->transform(default_pos);

        vert.animated_x = pos1.x * vert.bone_weight1 + pos2.x * vert.bone_weight2;
        vert.animated_y = pos1.y * vert.bone_weight1 + pos2.y * vert.bone_weight2;
    }
}

void Scene::render() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-20.0, 0.0);
    glVertex2f(20.0, 0.0);
    glVertex2f(0.0, 20.0);
    glVertex2f(0.0, -20.0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    auto boneNames = this->skeleton->getBoneNames();
    for (auto boneName : boneNames) {
        glm::vec3 startPoint, endPoint, topPoint(2, 2, 0), bottomPoint(2, -2, 0);

        auto bone = this->getBone(boneName);

        bone->calculate_bone_endpoints(startPoint, endPoint);

        topPoint = bone->transform_from_bonespace(topPoint);
        bottomPoint = bone->transform_from_bonespace(bottomPoint);

        if (bone == selectedBone) {
            glLineWidth(4);
        }

        glBegin(GL_LINE_LOOP);
        glVertex2f(startPoint.x, startPoint.y);
        glVertex2f(topPoint.x, topPoint.y);
        glVertex2f(endPoint.x, endPoint.y);
        glVertex2f(bottomPoint.x, bottomPoint.y);
        glEnd();

        glLineWidth(1.0);
    }

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (auto vert : this->skin) {
        glVertex2d(vert.animated_x, vert.animated_y);
    }
    glEnd();
}

void Scene::setKeyFrame() {
    this->skeleton->setKeyFrame();
}

void Scene::startAnimation() {
    this->isAnimating = true;
    this->start = std::chrono::system_clock::now();
}
