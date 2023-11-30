#include "lab_m1/lab7/lab7.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

#define COLLISION_ERROR 0.25f

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab7::Lab7()
{
}


Lab7::~Lab7()
{
}


void Lab7::Init()
{
	// time when the game starts
	startTime = glfwGetTime();
	gameOver = false;

	{
		building = new Mesh("building");
		building->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[building->GetMeshID()] = building;

		// create 10 buildings using buildingsPositions, buildingsColors and buildingsSizes vectors
		for (int i = 0; i < BUILDINGS_NUMBER; i++) {
			Building* building_obj = new Building(buildingsPositions[i], buildingsColors[i], buildingsSizes[i], max(buildingsSizes[i][0], buildingsSizes[i][2]) / 2);
			buildings.push_back(building_obj);
		}
	}

	TankComponent* my_tank_head, * my_tank_cannon, * my_tank_base, * my_tank_tracks;

	{
		Mesh* mesh = new Mesh("TANK_BASE");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "TANK_BASE_CAP_CENTRAT.obj");
		meshes[mesh->GetMeshID()] = mesh;
		my_tank_base = new TankComponent("TANK_BASE", glm::vec3(0, .04f, 0), glm::vec3(0, 1, 0), 1, -90.0f, 1, 0, 0, 1);
	}

	{
		Mesh* mesh = new Mesh("TANK_CANNON_LONG");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "TANK_TUN_BETTER.obj");
		meshes[mesh->GetMeshID()] = mesh;
		my_tank_cannon = new TankComponent("TANK_CANNON_LONG", glm::vec3(0, .14f, 0), glm::vec3(0.5f), 1, 0, 0.5f, 0, 1, 0);
	}

	{
		Mesh* mesh = new Mesh("TANK_HEAD");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "TANK_CENTERED_HEAD.obj");
		meshes[mesh->GetMeshID()] = mesh;
		my_tank_head = new TankComponent("TANK_HEAD", glm::vec3(0, 0.13f, 0), glm::vec3(0, 0.5f, 0), 1, -90.0f, 1, 0, 0, 1);
	}

	{
		Mesh* mesh = new Mesh("TANK_TRACKS");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "TANK_SENILE_BETTER.obj");
		meshes[mesh->GetMeshID()] = mesh;
		my_tank_tracks = new TankComponent("TANK_TRACKS", glm::vec3(-0.005f, 0.04f, 0), glm::vec3(0.5f), 1, 90.0f, 1, 0, 0, -1);
	}

	my_tank = new Tank(glm::vec3(0), 1, 0, my_tank_head, my_tank_cannon, my_tank_tracks, my_tank_base);
	// calculated on onShape app and chatgpt -> 0.17 / 2
	my_tank->radius = 0.17f / 2;

	{
		Mesh* mesh = new Mesh("AGORA");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "AGORA_2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("heart");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "HEART.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("star");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "STAR.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("thunder");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "THUNDER.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	{
		// CREATE 9 POWER UPS
		for (int i = 0; i < POWER_UPS_NUMBER; i++) {
			PowerUp* power_up = new PowerUp(powerUpsPositions[i], i % 3);
			powerUps_vector.push_back(power_up);
		}
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("LabShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab7", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab7", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}
}


void Lab7::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Lab7::Update(float deltaTimeSeconds)
{
	// check if the game stops after 1 minute
	if (glfwGetTime() - startTime >= 60) {
		gameOver = true;
	}

	{
		// render the power ups
		for (int i = 0; i < POWER_UPS_NUMBER; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, powerUps_vector[i]->bottom_position);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(powerUps_vector[i]->radius * 5));

			string name = "";

			switch (powerUps_vector[i]->type) {
			case 0:
				name = "heart";
				break;
			case 1:
				name = "star";
				break;
			case 2:
				name = "thunder";
				break;
			}

			RenderMesh(meshes[name], shaders["LabShader"], modelMatrix, powerUps_vector[i]->color);
		}
	}


	{
		// render te buildings
		for (int i = 0; i < BUILDINGS_NUMBER; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, buildings[i]->bottom_position);
			modelMatrix = glm::scale(modelMatrix, buildings[i]->size);
			RenderMesh(building, shaders["LabShader"], modelMatrix, buildings[i]->color);
		}
	}

	// render each tank components
	for (int i = 0; i < 4; i++) {

		// check for collisions first

		// collision with buildings

		for (int j = 0; j < BUILDINGS_NUMBER; j++) {
			// sum of the radiuses < distance between the centers
			float sum_radiuses = my_tank->radius + buildings[j]->radius;
			float distance = sqrt((my_tank->bottom_position[0] - buildings[j]->bottom_position[0]) * (my_tank->bottom_position[0] - buildings[j]->bottom_position[0]) +
				(my_tank->bottom_position[2] - buildings[j]->bottom_position[2]) * (my_tank->bottom_position[2] - buildings[j]->bottom_position[2]));

			/*	Daca Distanta_Intre_Tancuri < Raza_Tanc_1 + Raza_Tanc_2, atunci acestea s - au ciocnit.

				Si se suprapun cu o distanta egala cu | P | = (Raza_Tanc_1 + Raza_Tanc_2 - Distanta_Intre_Tancuri)

				pentru a evita suprapunerea lor, trebuie sa mutam tancurile în direcții opuse astfel încât distanța dintre acestea sa creasca cu | P | .*/

			float P = abs(sum_radiuses + COLLISION_ERROR - distance);

			if (distance <= sum_radiuses + COLLISION_ERROR) {
				cout << "Hitting" << endl;
				my_tank->bottom_position[0] += P * (my_tank->bottom_position[0] - buildings[j]->bottom_position[0]) / distance;
				my_tank->bottom_position[2] += P * (my_tank->bottom_position[2] - buildings[j]->bottom_position[2]) / distance;
			}
		}

		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(my_tank->bottom_position));

		// to make it point front
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, my_tank->components[i]->bottom_position);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(my_tank->components[i]->default_rotation_angle), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(my_tank->components[i]->default_scale_factor));

		// movement
		modelMatrix = glm::rotate(modelMatrix, RADIANS(my_tank->rotation_angle + my_tank->components[i]->rotation_angle), glm::vec3(my_tank->components[i]->rotationX_Y, my_tank->components[i]->rotationY_Y, my_tank->components[i]->rotationZ_Y));

		RenderMesh(meshes[my_tank->components[i]->name], shaders["LabShader"], modelMatrix, my_tank->components[i]->color);
	}

	for (int i = 0; i < projectiles_vector.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, projectiles_vector[i]->position);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(projectiles_vector[i]->radius * 2));
		RenderMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 0.5, 0.5));

		// adding movement to the projectile
		projectiles_vector[i]->position[0] -= sin(projectiles_vector[i]->angle) * deltaTimeSeconds;
		projectiles_vector[i]->position[2] -= cos(projectiles_vector[i]->angle) * deltaTimeSeconds;
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.01f, 0.5f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		RenderMesh(meshes["AGORA"], shaders["Simple"], modelMatrix);
	}

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

		RenderMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));
	}

}


void Lab7::FrameEnd()
{
	DrawCoordinateSystem();
}


void Lab7::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO(student): Set light position uniform
	GLint location_light_pos = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location_light_pos, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
	// TODO(student): Set eye position (camera position) uniform
	GLint location_eye_pos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location_eye_pos, 1, glm::value_ptr(eyePosition));

	// TODO(student): Set material property uniforms (shininess, kd, ks, object color)
	GLint location_material = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location_material, materialShininess);

	location_material = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location_material, materialKd);

	location_material = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location_material, materialKs);

	location_material = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(location_material, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab7::OnInputUpdate(float deltaTime, int mods)
{

	// only if right click is not pressed

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		glm::vec3 forward = glm::normalize(glm::vec3(sin(RADIANS(my_tank->rotation_angle)), 0, cos(RADIANS(my_tank->rotation_angle))));

		if (window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_S)) {

			int goes_positive = window->KeyHold(GLFW_KEY_W) ? -1 : 1;

			my_tank->bottom_position[0] += goes_positive * forward[0] * deltaTime * my_tank->translation_speed;
			my_tank->bottom_position[2] += goes_positive * forward[2] * deltaTime * my_tank->translation_speed;

			//cout << my_tank->bottom_position[0] << " " << my_tank->bottom_position[1] << " " << my_tank->bottom_position[2] << endl;
		}

		if (window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_D)) {

			int goes_positive = window->KeyHold(GLFW_KEY_A) ? 1 : -1;

			my_tank->rotation_angle += goes_positive * deltaTime * my_tank->rotation_speed;
		}

		if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
			if (glfwGetTime() - my_tank->last_projectile_time >= 1.0) {

				float radius = 0.0125f;
				float angle = RADIANS(my_tank->cannon->rotation_angle);
				TankComponent* cannon = my_tank->cannon;
				Projectile* projectile = new Projectile(1, my_tank->bottom_position);
				// glm::vec3(-0.05f, .14f, 0) -> cannon
				projectile->position[0] = my_tank->bottom_position[0];
				projectile->position[1] = my_tank->cannon->bottom_position[1];
				projectile->position[2] = my_tank->bottom_position[2];

				projectile->angle = angle + my_tank->rotation_angle;
				projectile->radius = radius;
				projectiles_vector.push_back(projectile);

				my_tank->last_projectile_time = projectile->appear_time = glfwGetTime();
			}
		}
	}
}


void Lab7::OnKeyPress(int key, int mods)
{
	// Add key press event
}


void Lab7::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Lab7::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// if mouse points to the left, rotate the tank head to the left
	if (deltaX < 0) {
		TankComponent* head = my_tank->head;
		TankComponent* cannon = my_tank->cannon;
		head->rotation_angle += 1;
		cannon->rotation_angle += 1;
	}
	else if (deltaX > 0) {
		TankComponent* head = my_tank->head;
		TankComponent* cannon = my_tank->cannon;
		head->rotation_angle -= 1;
		cannon->rotation_angle -= 1;
	}
}


void Lab7::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Lab7::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Lab7::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab7::OnWindowResize(int width, int height)
{
}