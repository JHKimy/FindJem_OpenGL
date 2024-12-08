#include "Actor.h"

#include <iostream>
#include <fstream>
#include <sstream>

void Actor::InitializeBuffers()
{
// VAO
	// VAO ����
	glGenVertexArrays(1, &vao);
	// VBO ����
	glGenBuffers(1, &vbo);

	// VAO Ȱ��ȭ
	glBindVertexArray(vao);
	
// VBO
	// ť�� VBO Ȱ��ȭ
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// VBO�� ������ ���ε�
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);


// ��ġ
	// VBO�� ����� ������ �� ���� ��ġ �����͸� ���̴��� ����
	// (�Ӽ� ��ȣ, �Ӽ��� ����, ������ Ÿ��, ������ ����ȭ, stride, ���� �ּ�)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

// ����
	// VBO�� ����� ������ �� ���� ���� ������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


// VAO
	// VAO Ȱ��ȭ ����
	glBindVertexArray(0);
}

void Actor::LoadObj(const std::string& objFilePath)
{
    std::ifstream file(objFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << objFilePath << std::endl;
        return;
    }

	vector<glm::vec3> vertex;
	vector<glm::vec3> vNormal;

	vector<glm::ivec3> vFace;
	vector<glm::ivec3> vnFace;

	ifstream in(objFilePath);
	if (!in) {
		cout << "OBJ File No Have" << endl;
		return;
	}

	while (in) {
		string temp;
		getline(in, temp);

		if (temp[0] == 'v' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			char tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vertex.push_back(vertemp);
		}

		else if (temp[0] == 'v' && temp[1] == 'n' && temp[2] == ' ') {
			istringstream slice(temp);

			glm::vec3 vertemp;
			string tmpword;
			slice >> tmpword >> vertemp.x >> vertemp.y >> vertemp.z;

			vNormal.push_back(vertemp);
		}

		else if (temp[0] == 'f' && temp[1] == ' ') {
			istringstream slice(temp);

			glm::ivec3 vfacetemp;
			glm::ivec3 vnfacetemp;
			for (int i = -1; i < 3; ++i) {
				string word;
				getline(slice, word, ' ');
				if (i == -1) continue;                  // f ������ �ǳʶڴ�
				if (word.find("/") != string::npos) {
					istringstream slash(word);
					string slashtmp;
					getline(slash, slashtmp, '/');

					vfacetemp[i] = atoi(slashtmp.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����

					getline(slash, slashtmp, '/');
					getline(slash, slashtmp, '/');
					vnfacetemp[i] = atoi(slashtmp.c_str()) - 1;
				}
				else {
					vfacetemp[i] = atoi(word.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����
				}
			}
			vFace.push_back(vfacetemp);
			vnFace.push_back(vnfacetemp);
		}
	}

	for (int i = 0; i < vFace.size(); ++i) {
		vertices.push_back(vertex[vFace[i].x]);
		vertices.push_back(vNormal[vnFace[i].x]);
		
		vertices.push_back(vertex[vFace[i].y]);
		vertices.push_back(vNormal[vnFace[i].y]);
		
		vertices.push_back(vertex[vFace[i].z]);
		vertices.push_back(vNormal[vnFace[i].z]);
	}
}

Actor::Actor(
	string objFilePath,
	glm::vec3 position, 
	glm::vec3 scale, 
	glm::vec3 rotation, 
	glm::vec3 color)

	:
	position(position), 
    scale(scale),
    rotation(rotation), 
    color(color) 
{
	LoadObj(objFilePath);
	InitializeBuffers();
}

Actor::~Actor()
{
    // VAO ����
    glDeleteVertexArrays(1, &vao);
    // VBO ����
    glDeleteBuffers(1, &vbo);
    
    if (!vertices.empty()) {
        glDeleteBuffers(1, &ebo);
    }
}

void Actor::Render(GLuint shaderProgram)
{
    //glUseProgram(shaderProgram);


	glBindVertexArray(vao);


	// ���� ����
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	glUniform3f(colorLoc, color.r, color.g, color.b);

    //glBindVertexArray(vao);

    // �� ��ȯ ��� ���
    glm::mat4 translate = glm::mat4(1.0f);	// �̵� ���
    glm::mat4 size = glm::mat4(0.1f);		// ������
    //glm::vec3 size = glm::vec3(1.f);
    glm::mat4 rotate = glm::mat4(0.1f);		// ȸ��

    glm::mat4 model = glm::mat4(1.0f);

    translate = glm::translate(model, position);
    size = glm::scale(size, scale);
    //model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 TRANS = glm::mat4(1.0f);		// �ռ� ��ȯ ���

    TRANS = translate * size;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TRANS));



    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
    // glBindVertexArray(0);
}

void Actor::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

void Actor::SetScale(const glm::vec3& scl)
{
    scale = scl;
}

void Actor::SetRotation(const glm::vec3& rot)
{
    rotation = rot;
}

glm::vec3 Actor::GetPosition() const
{
	return position;
}

glm::vec3 Actor::GetScale() const
{
	return scale;
}

glm::vec3 Actor::GetRotation() const
{
	return rotation;
}
