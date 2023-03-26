#include "Renderer.h"

GLsizei Renderer::FRAME_WIDTH = 800;
GLsizei Renderer::FRAME_HEIGHT = 600;
Shader Renderer::lineShader;
DrawObject Renderer::circle_primitive;
priority_queue<tuple<int, DrawObject*, Transform>> Renderer::pq;

GLuint Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<float>& normal_coefficients, const vector<GLuint>& indices)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO_model_coefficients_id;
	glGenBuffers(1, &VBO_model_coefficients_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!normal_coefficients.empty())
	{
		GLuint VBO_normal_coefficients_id;
		glGenBuffers(1, &VBO_normal_coefficients_id);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_coefficients_id);
		glBufferData(GL_ARRAY_BUFFER, normal_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, normal_coefficients.size() * sizeof(float), normal_coefficients.data());
		location = 1; // "(location = 1)" em "shader_vertex.glsl"
		number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
		glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(location);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//if (!texture_coefficients.empty())
	//{
	//	GLuint VBO_texture_coefficients_id;
	//	glGenBuffers(1, &VBO_texture_coefficients_id);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_coefficients_id);
	//	glBufferData(GL_ARRAY_BUFFER, texture_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, texture_coefficients.size() * sizeof(float), texture_coefficients.data());
	//	location = 2; // "(location = 1)" em "shader_vertex.glsl"
	//	number_of_dimensions = 2; // vec2 em "shader_vertex.glsl"
	//	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(location);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	GLuint indices_id;
	glGenBuffers(1, &indices_id);

	// "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
	//

	// "Desligamos" o VAO, evitando assim que operações posteriores venham a
	// alterar o mesmo. Isso evita bugs.
	glBindVertexArray(0);

	// Retornamos o ID do VAO. Isso é tudo que será necessário para renderizar
	// os triângulos definidos acima. Veja a chamada glDrawElements() em main().

	return VAO;
}
void Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<GLuint>& indices, DrawObject* obj)
{
#if 1
	if (obj->VAO == 0) {
		glGenVertexArrays(1, &obj->VAO);
	}
	glBindVertexArray(obj->VAO);

	if (obj->VBO == 0) {
		glGenBuffers(1, &obj->VBO);
	}
	GLuint VBO_model_coefficients_id = obj->VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (obj->VIO == 0) {
		glGenBuffers(1, &obj->VIO);
	}
	GLuint indices_id = obj->VIO;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);
#endif
}
GLuint Renderer::BuildTrianglesVAO(const vector<float>& model_coefficients, const vector<GLuint>& indices)
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO_model_coefficients_id;
	glGenBuffers(1, &VBO_model_coefficients_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
	glBufferData(GL_ARRAY_BUFFER, model_coefficients.size() * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_coefficients.size() * sizeof(float), model_coefficients.data());
	GLuint location = 0; // "(location = 0)" em "shader_vertex.glsl"
	GLint  number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
	glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint indices_id;
	glGenBuffers(1, &indices_id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());

	glBindVertexArray(0);

	return VAO;
}

void Renderer::RenderTriangles(DrawObject* obj, const Transform& tr, Camera* camera, bool lines) {
	lineShader.use();
	lineShader.setMatrix4("view_m", camera->GetViewMatrix());
	lineShader.setMatrix4("proj_m", camera->GetProjectionMatrix());
	lineShader.setMatrix4("mode_m", tr.GetModelMatrix());

	cout << "rendering obj VAO: " << obj->VAO << endl;

	glBindVertexArray(obj->VAO);
	glCullFace(GL_BACK);
	glLineWidth(2);
	if(lines) glDrawElements(GL_LINES, obj->indexes_size, GL_UNSIGNED_INT, 0);
	else glDrawElements(GL_TRIANGLES, obj->indexes_size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::initFrame(glm::vec4 bg_color) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0);

	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CW);
	glClear(GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);
}

void Renderer::drawFrame(glm::vec4 bg_color, Camera* camera) {
	
	initFrame(bg_color);
	
	//pq.push(make_tuple(1, circle_primitive, Transform(glm::vec3(5.0, 0.0, 0.0), glm::quat(1, vec3(0.0)), glm::vec3(0.5, 0.5, 0.5))));
	//pq.push(make_tuple(1, circle_primitive, Transform(glm::vec3(0.0, 5.0, 0.0), glm::quat(1, vec3(0.0)), glm::vec3(0.5, 0.5, 0.5))));
	//pq.push(make_tuple(1, circle_primitive, Transform(glm::vec3(0.0, 0.0, 0.0), glm::quat(1, vec3(0.0)), glm::vec3(0.5, 0.5, 0.5))));

	while (pq.size()) {
		int prio;
		DrawObject* obj;
		Transform tr;
		tie(prio, obj, tr) = pq.top();
		RenderTriangles(obj, tr, camera, (prio == 2));
		pq.pop();
	}

	glViewport(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

	//// "Pintamos" todos os pixels do framebuffer com a cor definida acima

	//while (!renderQ.empty()) {
	//	
	//}
}

void Renderer::setupPrimitives() {
	Mesh circle_mesh("meshes/circle.obj");

	circle_primitive.indexes_size = (int)circle_mesh.indices.size();
	circle_primitive.VAO = BuildTrianglesVAO(
		circle_mesh.model_coefficients,
		circle_mesh.normal_coefficients,
		circle_mesh.indices
		);

	lineShader = Shader("src/vertex.glsl", "src/fragment.glsl");
}