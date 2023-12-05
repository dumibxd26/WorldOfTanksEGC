#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "./Classes/Building.h";
#include "./Classes/Building.h"
#include "./Classes/TankComponent.h"
#include "./Classes/Tank.h"
#include "./Classes/Projectile.h"
#include "./Classes/PowerUp.h"

#define BUILDINGS_NUMBER 10
#define POWER_UPS_NUMBER 9
#define ENEMY_TANKS_NUMBER 6

namespace m1
{
    class Lab7 : public gfxc::SimpleScene
    {
     public:
        Lab7();
        ~Lab7();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        float startTime;
        int prevTime = 60;
        bool gameOver;
        int score;

        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        std::vector <Projectile *> projectiles_vector;

        Mesh* building;
        std::vector<Building *> buildings;
        // 10 buildings
        std::vector<glm::vec3> buildingsPositions = {
            {2, 1, 1},
            {2, 1, 7.5f},
            {8, 1, -1},
            {7, 1, -6.5f},
            {-3, 1, 4},
            {-8, 1, 2.5f},
            {-6, 1, -9},
            {-2, 1, -5.5f},
            {6, 1, 5},
            {10, 1, 2},
        };
        std::vector<glm::vec3> buildingsColors = {
            {0.2f, 0.5f, 0.2f},
            {1, 0.5f, 0.2f},
            {0.2f, 0.5f, 1},
            {0.2f, 1, 0.5f},
            {0.5f, 0.2f, 0.5f},
            {0.5f, 0.2f, 1},
            {1, 0.2f, 0.5f},
            {0.2f, 1, 0.2f},
            {0.5f, 0.5f, 0.2f},
            {0.5f, 0.5f, 1}
        };
        // those are scales, can and will be floats
        std::vector<glm::vec3> buildingsSizes = {
            {1.5f, 2, 1.3f},
            {1.3f, 2.5f, 1.2f},
            {1.2f, 2.2f, 1.5f},
			{1.5f, 2.5f, 1.3f},
			{1.3f, 2.3f, 1.2f},
			{1.2f, 2.2f, 1.5f},
			{1.5f, 2.5f, 1.3f},
			{1.3f, 2.3f, 1.2f},
			{1.2f, 2.2f, 1.5f},
			{1.5f, 2.75f, 1.3f}
        };

        std::vector<glm::vec3> powerUpsPositions = {
            {3, 0.075f, 2},
			{3, 0.075f, 8.5f},
			{7, 0.075f, -2},
			{4, 0.075f, -3.5f},
			{-2, 0.075f, 1},
			{-5, 0.075f, 8.5f},
			{-8, 0.075f, -7},
			{-1, 0.075f, -5.5f},
			{4, 0.075f, 4},
		};

        std::vector<PowerUp *> powerUps_vector;
        
        Tank* my_tank;

        std::vector <Tank*> enemy_tanks;

        std::vector<glm::vec3> enemy_tanks_positions = {
            {-7, 0, -2},
            {-2, 0, -7},
            {3, 0, 1},
            {7, 0, -3},
            {-9, 0 , 7},
            {8, 0, 8}
		};

        std::vector<float> enemy_tanks_rotations = {
            -90, 0, 0, 90, -90, 90
        };
    };
}   // namespace m1
