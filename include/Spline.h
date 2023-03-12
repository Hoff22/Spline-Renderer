#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"

using namespace std;

struct ControlPoint {
	glm::vec2 point;
	glm::vec2 handleL;
	glm::vec2 handleR;
	glm::vec4 color;

	ControlPoint() :
		point(glm::vec2(0.0)),
		handleL(glm::vec2(0.0)),
		handleR(glm::vec2(0.0)),
		color(glm::vec4(1.0))
	{

	}

	ControlPoint(glm::vec2 pos) :
		point(pos),
		handleL(pos + glm::vec2(1.0,1.0)),
		handleR(pos),
		color(glm::vec4(1.0))
	{

	}

	ControlPoint(glm::vec2 p1, glm::vec2 p2) :
		point(p1),
		handleL(p1),
		handleR(p2),
		color(glm::vec4(1.0))
	{

	}

	ControlPoint(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) :
		point(p1),
		handleL(p2),
		handleR(p3),
		color(glm::vec4(1.0))
	{

	}
};

class Spline
{
public:
	vector<ControlPoint*> control;
	vector<glm::vec2> points;
	DrawObject* draw_object;

	void updatePoints();
	void addControlPoint(glm::vec2 p1);
	glm::vec2 pointAt(float t);
	void drawSpline(int def);

	Spline() :
		control(vector<ControlPoint*>()),
		draw_object(new DrawObject())
	{
		
	}

	Spline(glm::vec2 p1) : Spline() {
		control.push_back(new ControlPoint(p1, p1 + glm::vec2(1.0, 1.0)));
		updatePoints();
	}



};

