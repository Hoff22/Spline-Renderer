#include <iostream>
#include <string>
#include "Scene.h"
#include "camera.h"
#include "Spline.h"
#include "Renderer.h"
#include "MainWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

float	p_f[8] = { 25.0f / 255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 255.0f / 255.0f, 10.0, 3.0, 2.0, 5.5 };
int		p_i[8] = { 10, 0, 0, 0, 0, 0, 0, 0 };
bool	p_b[8] = { true, true, false, false, false, false, false, false };
bool	i_p[6] = { false, false ,false ,false ,false ,false };

glm::vec2 screen_to_world_pos(float xpos, float ypos) {
	xpos -= MainWindow::SCR_WIDTH / 2;
	ypos -= MainWindow::SCR_HEIGHT / 2;
	float off = MainWindow::SCR_HEIGHT / (Scene::Zoom * 2);
	return glm::vec2((xpos / off) + Scene::camera_pos.x, (-ypos / off) + Scene::camera_pos.y);
}

void initScene() {
	Scene::paramsf = p_f;
	Scene::paramsi = p_i;
	Scene::paramsb = p_b;
	Scene::is_pressed = i_p;
	Scene::left_click = false;
	Scene::right_click = false;
	Scene::main_camera = new LookAtCamera(glm::vec3(0.0, 0.0, 10.0));
	Scene::camera_pos = glm::vec2(0.0);
	Scene::objects = vector<Spline*>();
	Scene::Zoom = 10.0f;
}

void updateCamera() {
	Scene::main_camera->ScreenRatio = 1.0f * MainWindow::SCR_WIDTH / MainWindow::SCR_HEIGHT;
	((LookAtCamera*)(Scene::main_camera))->target = glm::vec3(Scene::camera_pos, 0.0);
	Scene::main_camera->Zoom = Scene::Zoom;
	Renderer::FRAME_WIDTH = MainWindow::SCR_WIDTH;
	Renderer::FRAME_HEIGHT = MainWindow::SCR_HEIGHT;
}

int main() {

	glfwSetErrorCallback(MainWindow::glfw_error_callback);
	if (!glfwInit()) return 1;

	glfwWindowHint(GLFW_SAMPLES, 4);

	MainWindow::window = glfwCreateWindow(MainWindow::SCR_WIDTH, MainWindow::SCR_HEIGHT, "Program", NULL, NULL);
	if (MainWindow::window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(MainWindow::window);
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(MainWindow::window, MainWindow::framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// init ui
	MainWindow::initUI();
	// setup
	initScene();
	Renderer::setupPrimitives();

	glm::vec2* selectedPointA = 0;
	glm::vec2* selectedPointB = 0;
	glm::vec2* selectedPointC = 0;
	Spline* selectedSpline = 0;

	// while loop
	while (MainWindow::is_open()) {

		glfwPollEvents();
		double mouseX, mouseY;
		glfwGetCursorPos(MainWindow::window, &mouseX, &mouseY);
		MainWindow::handle_input(MainWindow::window, 1.0f);
			
		if (Scene::paramsb[0]) {
			Scene::objects.clear();
			Scene::paramsb[0] = false;
		}

		// calculate shit
		if (Scene::left_click) {
			glm::vec2 mousePos = screen_to_world_pos(mouseX, mouseY);
			
			for (auto s : Scene::objects) {
				bool found = 0;
				for (ControlPoint* i : s->control) {
					if (i->handleL != i->point) {
						if (glm::distance(mousePos, i->handleL) <= 0.2) {
							selectedPointA = &(i->handleL);
							selectedPointB = &(i->handleR);
							selectedPointC = &(i->point);
							selectedSpline = s;
							found = 1;
							break;
						}
					}
					if (glm::distance(mousePos, i->point) <= 0.5) {
						selectedPointA = &(i->point);
						selectedPointB = &(i->point);
						selectedPointC = 0;
						selectedSpline = s;
						found = 1;
						break;
					}
					
					if (i->handleR != i->point) {
						if (glm::distance(mousePos, i->handleR) <= 0.2) {
							selectedPointA = &(i->handleR);
							selectedPointB = &(i->handleL);
							selectedPointC = &(i->point);
							selectedSpline = s;
							found = 1;
							break;
						}
					}
				}
				
				if (found) break;

				selectedSpline = 0;
			}
		}
		else if (Scene::right_click) {
			glm::vec2 mousePos = screen_to_world_pos(mouseX, mouseY);
			if (selectedSpline == 0) {
				Scene::objects.push_back(new Spline(mousePos));
				selectedSpline = Scene::objects.back();
			}
			else {
				cout << "add spline control point to (" << selectedSpline->control.back()->point.x << "," << selectedSpline->control.back()->point.y << ")" << endl;
				selectedSpline->addControlPoint(mousePos);
				selectedSpline->updatePoints();
			}
		}
		if ((Scene::left_click or Scene::mouse_draggin) and selectedPointA != 0) {
			*selectedPointA = screen_to_world_pos(mouseX, mouseY);
			if (Scene::paramsb[2] and selectedPointC != 0) {
				*selectedPointB = *selectedPointC - (*selectedPointA - *selectedPointC);
			}
			selectedSpline->updatePoints();
		}
		else {
			selectedPointA = 0;
		}

		if (Scene::paramsb[1]) {
			for (auto s : Scene::objects) s->drawSpline(Scene::paramsi[0]);
		}

		//update camera
		updateCamera();

		// render
		float* pf = Scene::paramsf;
		Renderer::drawFrame(glm::vec4(pf[0], pf[1], pf[2], 1.0f), Scene::main_camera);

		MainWindow::drawUI();

		glfwSwapBuffers(MainWindow::window);
	}
	// clean up
	MainWindow::cleanupUI();

	return 0;
}