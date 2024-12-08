#version 330 core
 // ���� ���α׷����� �޾ƿ� ���� ��ǥ��
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

// �����׸�Ʈ ���̴� ���� ����
out vec3 FragPos; // ��ü�� ��ġ��
out vec3 Normal; // ��ְ�

// ���ؽ� ��ȯ ��� (uniform)
uniform mat4 modelTransform; // �𵨸� ��ȯ ���: uniform ������ ����
uniform mat4 projectionTransform;
uniform mat4 viewTransform;

void main()
{
   gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0); //--- ��ǥ���� modelTransform ��ȯ�� �����Ѵ�.
   
   
   FragPos = vec3(modelTransform * vec4(vPos, 1.0));
   Normal = vNormal;
}