#include "Spline.h"

const int RESOLUTION = 30;
int Spline::spl_id_timer = 0;

void Spline::updatePoints() {
	this->control[0]->handleL = this->control[0]->point;
	this->control.back()->handleR = this->control.back()->point;

	this->computeLUT();
}

void Spline::addControlPoint(glm::vec2 p1) {
	if (this->control.size() > 0) {
		glm::vec2 last_point = this->control.back()->point;
		this->control.back()->handleR = this->control.back()->point + glm::normalize(p1 - last_point);
		this->control.push_back(new ControlPoint(p1, p1 + glm::normalize(last_point - p1), p1));
	}
	else {
		this->control.push_back(new ControlPoint(p1, p1 + glm::vec2(3.0, 3.0), p1));
	}
	this->updatePoints();
}

glm::vec2 lerp(glm::vec2 p1, glm::vec2 p2, float t) {
	return p1 + (p2 - p1) * t;
}

glm::vec2 Spline::pointAt(ControlPoint* p1, ControlPoint* p2,  float t) {
	glm::vec2 p3 = lerp(lerp(p1->point, p1->handleR, t), lerp(p1->handleR, p2->handleL, t), t);
	glm::vec2 p4 = lerp(lerp(p1->handleR, p2->handleL, t), lerp(p2->handleL, p2->point, t), t);
	return lerp(p3, p4, t);
}

double segmentLen(ControlPoint* p1, ControlPoint* p2) {
	double len = 0;
	glm::vec2 last = p1->point;
	for (int i = 1; i <= 10; i++) {
		glm::vec2 cur = Spline::pointAt(p1, p2, 1.0 * 10 / i);
		len += glm::distance(last, cur);
		last = cur;
	}
	return len;
}

void Spline::computeLUT() {
	this->LUT_lengths.resize(RESOLUTION * (this->control.size() - 1) + 1);

	this->LUT_lengths[0].first = 0.0;
	this->LUT_lengths[0].second = 1.0;

	ControlPoint* last = this->control[0];
	for (int i = 1; i < this->control.size(); i++) {

		ControlPoint* cur = this->control[i];

		glm::vec2 last_p = last->point;
		for (int j = 1; j <= RESOLUTION; j++) {
			double t = 1.0 * j / RESOLUTION;
			glm::vec2 p_j = pointAt(last, cur, t);
			this->LUT_lengths[(i - 1) * RESOLUTION + j].first = this->LUT_lengths[(i - 1) * RESOLUTION + j-1].first + glm::distance(last_p, p_j);
			this->LUT_lengths[(i - 1) * RESOLUTION + j].second = t;
			last_p = p_j;
		}

		last = this->control[i];
	}

	this->total_length = this->LUT_lengths[this->LUT_lengths.size() - 1].first;
}

void Spline::drawSpline(int def = 30) {
	
	for (ControlPoint* i : this->control) {
		if (i->handleL != i->point) Renderer::pq.push(make_tuple(1, &Renderer::circle_primitive, Transform(glm::vec3(i->handleL, 0.0), glm::quat(), glm::vec3(0.2))));
		Renderer::pq.push(make_tuple(1, &Renderer::circle_primitive, Transform(glm::vec3(i->point, 0.0), glm::quat(), glm::vec3(0.5))));
		if (i->handleR != i->point) Renderer::pq.push(make_tuple(1, &Renderer::circle_primitive, Transform(glm::vec3(i->handleR, 0.0), glm::quat(), glm::vec3(0.2))));
	}

	if (this->control.size() <= 1) return;

	vector<glm::vec2> points;

	double next_length = this->total_length / def;
	for (int i = 0; i < this->LUT_lengths.size()-1; i++) {
		int control_point_idx = i / RESOLUTION;
		double ta = this->LUT_lengths[i].second;
		double tb = this->LUT_lengths[i + 1].second;
		double da = this->LUT_lengths[i].first;
		double db = this->LUT_lengths[i + 1].first;
		if (i % RESOLUTION == 0) {
			ta = 0.0f;
			points.push_back(this->control[i/RESOLUTION]->point);
		}
		double delta_t = tb - ta;
		double delta_d = db - da;
		while (next_length <= this->LUT_lengths[i + 1].first) {
			double t = ta + delta_t * ((next_length - da) / delta_d);
			points.push_back(pointAt(this->control[control_point_idx], this->control[control_point_idx + 1], t));
			next_length += this->total_length / def;
		}
	}

	points.push_back(this->control.back()->point);

	//cout << "points size for spline_id: " << spl_id << " | " << points.size() << endl;
	
	vector<float> model_coeficients;
	vector<GLuint> indices;

	indices.push_back(0);
	for (int i = 0; i < points.size(); i++) {
		model_coeficients.push_back(points[i].x);
		model_coeficients.push_back(points[i].y);
		model_coeficients.push_back(0.0f);
		model_coeficients.push_back(1.0f);
		indices.push_back(i);
		indices.push_back(i);
	}

	Renderer::BuildTrianglesVAO(model_coeficients, indices, this->draw_object);
	this->draw_object->indexes_size = indices.size();
	Renderer::pq.push(make_tuple(2, this->draw_object, Transform()));
}