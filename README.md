# GraphicApplication

Graphic Application with OpenGL in mode core profile, using **glew**, **glfw**, **glm** libraries. 

* **GLFW** ([OpenGL FrameWork](https://www.glfw.org/)) is a C++ library that provides an API to create windows with an associated *OpenGL Core Profile* graphical context and capable of receiving keyboard, mouse events.
Objects designed with the principle of revolution. 

* **GLEW** ([OpenGL Extension Wrangler](http://glew.sourceforge.net/)) is a C++ library that provides efficient access to the *OpenGL Core Profile* functions.

* **GLM** ([OpenGL Mathematics](https://glm.g-truc.net/0.9.9/index.html)) is a C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications that defines vectors and matrices for our application.
---

<a href="https://youtu.be/u_h4_5PdocM" target="_blank"><img src="https://i.imgur.com/T8Cok2Bm.png" 
alt="Graphic Application video demonstration" width="240" height="180" border="10" /></a>

<a href="https://youtu.be/u_h4_5PdocM" target="_blank"><img src="https://i.imgur.com/lSlDJpim.png" 
alt="Graphic Application video demonstration" width="240" height="180" border="10" /></a>

<a href="https://youtu.be/u_h4_5PdocM" target="_blank"><img src="https://i.imgur.com/6wqIZVd.png" 
alt="Graphic Application video demonstration" width="240" height="180" border="10" /></a>

---
Application is composed of a **callbacks** series to receive events and respond to them, for example:

```cpp
void window_refresh_callback(GLFWwindow *window) {
	PagRenderer::getInstance()->refreshCallback();
	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	PagRenderer::getInstance()->framebufferSizeCallback(width, height);
	window_refresh_callback(window);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	PagRenderer::getInstance()->mouseButtonCallback(button, action, mods, GLFW_PRESS, GLFW_RELEASE);
	window_refresh_callback(window);
}
```
---

App uses several design patterns, including the **Singleton** design pattern that you allow to have a single instance of the PagRenderer class. Also you allow access to it globally from any module.

```cpp
class PagRenderer {
public:
	~PagRenderer();

	void refreshCallback();
	void mouseButtonCallback(int button, int action, int mods, const int press, const int release);
	void keyCallback(int key, int scancode, int action, int mods);

	static PagRenderer *getInstance();
  
private:
	PagRenderer();

	static PagRenderer *instance;
};
```

```cpp
PagRenderer * PagRenderer::instance = NULL;

PagRenderer::PagRenderer() {
  //Initialization code
}

PagRenderer::~PagRenderer() {
  //Free memory
}

PagRenderer * PagRenderer::getInstance() {
	if (!instance) {
		instance = new PagRenderer();
	}
	return instance;
}
```

---

Or **Strategy** design pattern that determines how the dynamic exchange of messages between different objects takes place according to needs.

```cpp
class PagLightApplication {
public:
	virtual void apply(PagShaderProgram& ads, PagCamera camera) = 0;
	virtual LightType getType() = 0;
};
```

```cpp
class PagLightSource {
private:
	PagLightApplication *_strategy;

public:
	PagLightSource(PagLightApplication *strategy);
	PagLightSource& operator=(const PagLightSource& orig);
	~PagLightSource();

	void set_strategy(PagLightApplication *strategy);
	void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
};
```

```cpp
class PagDirectionalLight : public PagLightApplication {
public:
	PagDirectionalLight(glm::vec3 _Id, glm::vec3 _Is,
		glm::vec3 _direction, float _c1, float _c2, float _c3);
	virtual ~PagDirectionalLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	float c1, c2, c3;
	glm::vec3 Id, Is;
	glm::vec3 direction;
};
```

```cpp
class PagAmbientLight : public PagLightApplication {
public:
	PagAmbientLight(glm::vec3 _Ia);
	virtual ~PagAmbientLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	glm::vec3 Ia;
};
```

```cpp
class PagSpotLight : public PagLightApplication {
public:
	PagSpotLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _direction, glm::vec3 _position, 
				float _exponent, float _angle, float _c1, float _c2, float _c3);
	virtual ~PagSpotLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
private:
	glm::vec3 Id, Is;
	glm::vec3 direction;
	glm::vec3 position;
	float exponent, angle;
	float c1, c2, c3;
};
```

```cpp
class PagPointLight : public PagLightApplication {
public:
	PagPointLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _position, float _c1, float _c2, float _c3);
	virtual ~PagPointLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	float c1, c2, c3;
	glm::vec3 Id, Is;
	glm::vec3 position;
};
```

---

Also application includes camera movements through keyboard and mouse implemented in **PagCamera**.

|         | No Move | Orbit | Pan   | Truck | Crane | Dolly | Plan | Elevation | Section | Zoom         |
| ------- |:-------:|:-----:| :----:|:-----:| :----:| :----:| :---:| :--------:| :------:| :-----------:|
| **KEY** | 1       | 2     | 3     | 4     | 5     | 6     | 7    | 8         | 9       | Scroll Wheel |

Or display methods.

|         | All | Wireframe | Points   | Texture | Debug |
| ------- |:---:|:---------:| :-------:|:-------:| :----:| 
| **KEY** | a   | w         | p        | t       | d     | 


![alt text](https://i.imgur.com/DfwS3Nnm.png)
![alt text](https://i.imgur.com/mol9IZVm.png)
![alt text](https://i.imgur.com/H8d3f6hm.png)
