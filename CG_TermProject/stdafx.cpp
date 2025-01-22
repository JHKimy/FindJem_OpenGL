//#pragma once
//#include "stdafx.h"
//
//char* filetobuf(string file)
//{
//	ifstream in(file);
//
//	string* str_buf = new string;
//	while (in) {
//		string temp;
//		getline(in, temp);
//		str_buf->append(temp);
//		str_buf->append("\n");
//	}
//
//	return const_cast<char*>(str_buf->c_str());
//}
//
//void ReadObj(string file, vector<glm::vec3>& vertex, vector<glm::vec3>& color, vector<glm::ivec3>& face)
//{
//	ifstream in(file);
//	if (!in) {
//		cout << "OBJ File NO Have" << endl;
//		return;
//	}
//
//	random_device rd;
//	default_random_engine dre(rd());
//	uniform_real_distribution<float> urd{ 0.f, 1.f };
//	while (in) {
//		string temp;
//		getline(in, temp);
//
//		if (temp[0] == 'v' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::vec3 vertemp;
//			glm::vec3 clrtemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;                  // v ������ �ǳʶڴ�
//
//				vertemp[i] = atof(word.c_str());
//				clrtemp[i] = urd(dre);
//			}
//
//			vertex.push_back(vertemp);
//			color.push_back(clrtemp);
//			//::vcolor[0].push_back(glm::vec3(0.8, 0.0, 0.5));
//			//::vcolor[1].push_back(glm::vec3(0.6, 0.3, 0.7));
//			//::vcolor[2].push_back(glm::vec3(0.6, 0.1, 0.3));
//
//
//		}
//
//		else if (temp[0] == 'f' && temp[1] == ' ') {
//			istringstream slice(temp);
//
//			glm::ivec3 facetemp;
//			for (int i = -1; i < 3; ++i) {
//				string word;
//				getline(slice, word, ' ');
//				if (i == -1) continue;                  // f ������ �ǳʶڴ�
//				if (word.find("/") != string::npos) {
//					istringstream slash(word);
//					string slashtmp;
//					getline(slash, slashtmp, '/');
//
//					facetemp[i] = atoi(slashtmp.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����
//				}
//				else {
//					facetemp[i] = atoi(word.c_str()) - 1;         //f ������ ù��°����(v)   //�迭�ε��� ���Ŷ� -1����
//				}
//			}
//			face.push_back(facetemp);
//		}
//	}
//
//
//}
//
//bool make_vertexShaders()
//{
//	GLchar* vertexSource;
//
//	vertexSource = filetobuf("vertex.glsl");
//
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexSource, NULL);
//	glCompileShader(vertexShader);
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
//		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
//		return false;
//	}
//}
//
//bool make_fragmentShaders()
//{
//	GLchar* fragmentSource;
//	//--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
//	fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//	glCompileShader(fragmentShader);
//	GLint result;
//	GLchar errorLog[512];
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
//	if (!result)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
//		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
//		return false;
//	}
//}
//
//GLuint make_shaderProgram()
//{
//	make_vertexShaders();
//	make_fragmentShaders();
//
//	GLuint ShaderProgramID;
//	ShaderProgramID = glCreateProgram();
//	glAttachShader(ShaderProgramID, vertexShader);
//	glAttachShader(ShaderProgramID, fragmentShader);
//	glLinkProgram(ShaderProgramID);
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//	GLint result;
//	GLchar errorLog[512];
//
//	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
//	if (!result) {
//		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
//		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
//		return false;
//	}
//	glUseProgram(ShaderProgramID);
//	return ShaderProgramID;
//}