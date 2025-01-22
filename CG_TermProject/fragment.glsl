#version 330 core

in vec3 FragPos; //--- 노멀값을 계산하기 위해 객체의 위치값을 버텍스 세이더에서 받아온다.
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;		//--- 조명의 위치
uniform vec3 lightColor;	//--- 조명의 색
uniform vec3 objectColor;	//--- 객체의 색

uniform vec3 viewPos;		//--- 카메라 위치

void main()
{
	// 주변 조명 계수
	float ambientLight = 0.3;
	vec3 ambient = ambientLight * lightColor;

	// 산란 반사
	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir),0.0);
	vec3 diffuse = diffuseLight * lightColor;

	// 거울 반사
	int shininess = 128; 
	vec3 viewDir = normalize (viewPos - FragPos);	

	vec3 reflectDir = reflect (-lightDir, normalVector); //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
	float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V와 R의 내적값으로 강도 조절: 음수 방지
	specularLight = pow(specularLight, shininess); //--- shininess 승을 해주어 하이라이트를 만들어준다.
	vec3 specular = specularLight * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4 (result, 1.0);
}