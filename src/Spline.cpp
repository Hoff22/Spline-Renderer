#include "Spline.h"

void Spline::updatePoints() {
	this->control[0]->handleL = this->control[0]->point;
	this->control.back()->handleR = this->control.back()->point;

	this->points.clear();
	for (ControlPoint* i : this->control) {
		if (i->handleL != i->point) this->points.push_back(i->handleL);
		this->points.push_back(i->point);
		if (i->handleR != i->point) this->points.push_back(i->handleR);
	}
}

void Spline::addControlPoint(glm::vec2 p1) {
	glm::vec2 last_point = this->control.back()->point;
	this->control.back()->handleR = this->control.back()->point + glm::normalize(p1 - last_point);
	this->control.push_back(new ControlPoint(p1, p1 + glm::normalize(last_point - p1), p1));
	this->updatePoints();
}

glm::vec2 lerp(glm::vec2 p1, glm::vec2 p2, float t) {
	return p1 + (p2 - p1) * t;
}

double segmentLen(ControlPoint* p1, ControlPoint* p2) {
	return glm::distance(p1->point, p1->handleR) + glm::distance(p1->handleR, p2->handleL) + glm::distance(p2->handleL, p2->point);
}

glm::vec2 Spline::pointAt(float t) {
	double length = 0.0;
	for (int i = 1; i < this->control.size(); i++) {
		length += +segmentLen(this->control[i-1], this->control[i]);
	}

	double cur = 0.0;
	for (int i = 0; i < this->control.size(); i++) {
		if (i == this->control.size() - 2 or (cur + segmentLen(this->control[i], this->control[i+1])) / length >= t) {
			glm::vec2 p1 = this->control[i]->point;
			glm::vec2 p2 = this->control[i]->handleR;
			glm::vec2 p3 = this->control[i+1]->handleL;
			glm::vec2 p4 = this->control[i+1]->point;

			double tl = cur / length;
			double tr = (cur + segmentLen(this->control[i], this->control[i + 1])) / length;

			t = (t-tl) / (tr - tl);

			return lerp(lerp(lerp(p1, p2, t), lerp(p2, p3, t), t), lerp(lerp(p2,p3,t), lerp(p3,p4,t), t), t);
		}
		cur += segmentLen(this->control[i], this->control[i + 1]);
	}

	vector<glm::vec2> p;
	for (auto i : this->points) p.push_back(i);

	while (p.size() > 1) {
		vector<glm::vec2> np;
		for (int i = 0; i < p.size() - 1; i++) np.push_back(lerp(p[i], p[i + 1], t));
		p = np;
	}

	return p[0];
}

void Spline::drawSpline(int def = 30) {
	
	if (this->control.size() == 1) {
		for (ControlPoint* i : this->control) {
			if (i->handleL != i->point) Renderer::pq.push(make_tuple(1, Renderer::circle_primitive, Transform(glm::vec3(i->handleL, 0.0), glm::quat(), glm::vec3(0.2))));
			Renderer::pq.push(make_tuple(1, Renderer::circle_primitive, Transform(glm::vec3(i->point, 0.0), glm::quat(), glm::vec3(0.5))));
		}
		return;
	}
	vector<float> model_coeficients;
	vector<GLuint> indices;

	glm::vec2 cur = this->control[0]->point;
	model_coeficients.push_back(cur.x);
	model_coeficients.push_back(cur.y);
	model_coeficients.push_back(0.0f);
	model_coeficients.push_back(1.0f);

	indices.push_back(0);
	for (int i = 1; i <= def+(def%2==0); i++) {
		cur = this->pointAt((1.0f * i) / (def + (def % 2 == 0)));
		model_coeficients.push_back(cur.x);
		model_coeficients.push_back(cur.y);
		model_coeficients.push_back(0.0f);
		model_coeficients.push_back(1.0f);
		indices.push_back(i);
		indices.push_back(i);
	}
#if 1	
	Renderer::BuildTrianglesVAO(model_coeficients, indices, this->draw_object);
	this->draw_object->indexes_size = indices.size();
	Renderer::pq.push(make_tuple(2, *this->draw_object, Transform()));
	model_coeficients.clear();
	indices.clear();
#endif
	

	for (ControlPoint* i : this->control) {
		if (i->handleL != i->point) Renderer::pq.push(make_tuple(1, Renderer::circle_primitive, Transform(glm::vec3(i->handleL, 0.0), glm::quat(), glm::vec3(0.2))));
		Renderer::pq.push(make_tuple(1, Renderer::circle_primitive, Transform(glm::vec3(i->point, 0.0), glm::quat(), glm::vec3(0.5))));
		if (i->handleR != i->point) Renderer::pq.push(make_tuple(1, Renderer::circle_primitive, Transform(glm::vec3(i->handleR, 0.0), glm::quat(), glm::vec3(0.2))));
	}

}