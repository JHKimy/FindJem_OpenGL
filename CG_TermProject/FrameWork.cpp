//					
// 
// 
// 
//								| Scene |
//					---------------------------------- +
//					| -mainCamera : Camera            |
//					| -mainLight : Light              |
//					| -actors : vector<Actor*>        |
//					| -player : Character * |
//					| -shaderProgram : GLuint |
//					+---------------------------------- +
//					| +Initialize()                   |
//					| +Update(deltaTime : float)      |
//					| +Render()                       |
//					| +Shutdown() |
//					+---------------------------------- +
//								|
//				+------------------------------------ +
//				|                                    |
//	+-------------- +																+---------------- - +
//	| Camera     |							|										|   Light |
//	+-------------- +						v										+---------------- - +
//	| -cameraPos  |																	| -lightPos      |
//	| -cameraUp   |					+ ---------------------------------- +			| -lightColor |
//	+-------------- +				| Actor |										+---------------- - +
//	| +GetViewMatrix()				+---------------------------------- +			| +ApplyLighting()|
//	| +GetProjectionMatrix()		| -position : glm::vec3           |				|                 |
//	+-------------- +				| -scale : glm::vec3              |				+---------------- - +
//									| -rotation : glm::vec3           |
//									| -color : glm::vec3              |
//									| -vao : GLuint                   |
// 									| -vbo : GLuint |
//									+---------------------------------- +
//									| +LoadObj()                      |
//									| +InitializeBuffers()            |
//									| +Render(shaderProgram : GLuint) |
//									+---------------------------------- +
//	
//	
//	
//	
//	
//	
//	
//	
//	
//					|
//					|
//	+-------------------- + ------------------ +
//	|                    |                  |
//	+-------- - +	+-------------------------------- - +
//	| Character|	|   Bullet    |     |   Enemy |
//	+-------- - +	+-------------------------------- - +
//	| -moveSpeed	| -direction : glm::vec3			|
//	| -isJumping	| -isActive : bool					|
//	| -health		| +Update()							|
//	| +Move()		| +Deactivate()						|
//	| +Jump()       |									|
//	+-------- - +	+-------------------------------- - +
