#include "Scene.h"

using namespace std;

Camera*			Scene::main_camera = 0;
vector<Spline*>	Scene::objects = vector<Spline*>();
float*			Scene::paramsf = 0;
int*			Scene::paramsi = 0;
bool*			Scene::paramsb = 0;
bool			Scene::left_click = 0;
bool			Scene::right_click = 0;
bool			Scene::mouse_draggin = 0;
bool*			Scene::is_pressed = 0;
glm::vec2		Scene::camera_pos(0.0,0.0);
float			Scene::Zoom = 0;