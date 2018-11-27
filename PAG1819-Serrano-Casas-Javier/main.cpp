#include <iostream>
#include <stdlib.h>

// - IMPORTANTE: El include de Glew debe llamarse siempre ANTES de llamar al de GLFW.
#include <Windows.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "PagRenderer.h"
#include "PagEnumerations.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; // - Ejecuta la aplicacion con el procesador NVIDIA
}

// - Funci�n callback que ser� llamada cada vez que el �rea de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback(GLFWwindow *window) {
	// - Para llamar al m�todo que refresca el �rea de dibujo OpenGL
	// primero hay que acceder al singleton. Esto se hace a trav�s del m�todo de clase
	// getInstance() que devuelve un puntero al singleton.
	PagRenderer::getInstance()->refreshCallback();
	// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
	// intercambia el buffer back (que se ha estado dibujando) por el
	// que se mostraba hasta ahora front.
	glfwSwapBuffers(window);
}

// - Esta funci�n callback ser� llamada cada vez que se cambie el tama�o
// del �rea de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	PagRenderer::getInstance()->framebufferSizeCallback(width, height);
	window_refresh_callback(window);
}

// - Esta funci�n callback ser� llamada cada vez que se pulse una tecla
// dirigida al �rea de dibujo OpenGL.
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		PagRenderer::getInstance()->keyCallback(key, scancode, action, mods);
		window_refresh_callback(window);
	}
}

// - Esta funci�n callback ser� llamada cada vez que se pulse alg�n bot�n
// del rat�n sobre el �rea de dibujo OpenGL.
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	PagRenderer::getInstance()->mouseButtonCallback(button, action, mods, GLFW_PRESS, GLFW_RELEASE);
	window_refresh_callback(window);
}

// - Esta funci�n callback ser� llamada cada vez que se mueva la rueda
// del rat�n sobre el �rea de dibujo OpenGL.
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	PagRenderer::getInstance()->scrollCallback(xoffset, yoffset);
	window_refresh_callback(window);
}

// - Funci�n callback que ser� llamada cada vez cambie la posicion del cursor
// del rat�n sobre el �rea de dibujo OpenGL.
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
	PagRenderer::getInstance()->cursorPosCallback(xpos, ypos);
	window_refresh_callback(window);
}

int main() {
	std::cout << "Starting application PAG1819-Serrano-Casas-Javier" << std::endl;

	// - Inicializar GLFW. Es un proceso que s�lo debe realizarse una vez.
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// - Definimos las caracter�sticas que queremos que tenga nuestro contexto gr�fico
	// OpenGL. Por ejemplo, el n�mero de muestras o modo Core Profile.
	glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialiasing con 4 muestras.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL Core Profile 4.1.
												   // - Definimos la ventana de la aplicaci�n y la creamos
	GLFWwindow *window;

	// - Tama�o, t�tulo de la ventana, en ventana y no en pantalla completa,
	// sin compartir recursos con otras ventanas.
	window = glfwCreateWindow(PAG_INIT_WIDTH, PAG_INIT_HEIGHT, "PAG1819-Serrano-Casas-Javier", NULL, NULL);

	// - Comprobamos si la creaci�n de la ventana ha tenido �xito.
	if (window == NULL) {
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
		return -2;
	}

	// - Hace que la ventana que se pasa como argumento use el contexto
	// de GLFW cuyas caracter�sticas se han definido previamente.
	glfwMakeContextCurrent(window);

	// - Ahora inicializamos GLEW.
	// IMPORTANTE: Glew debe inicializarse siempre DESPUES de que se haya
	// inicializado y creado GLFW.
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
		return -3;
	}

	// - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
	// 3D constru�do.
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// - Registramos los callbacks que responder�n a los eventos principales
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	// - Establecemos un gris medio como color con el que se borrar� el frame buffer.
	// No tiene por qu� ejecutarse en cada paso por el ciclo de eventos.
	glClearColor(0.6, 0.6, 0.6, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glPrimitiveRestartIndex(0xFFFFFFFF);
	glEnable(GL_PRIMITIVE_RESTART);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_MULTISAMPLE);

	// Recupera la posici�n del cursor relativa al �rea de cliente de la ventana.
	glfwGetCursorPos(window, PagRenderer::getInstance()->getCursorX(), 
							 PagRenderer::getInstance()->getCursorY());

	PagRenderer::getInstance()->prepareOpenGL();

	// - Ciclo de eventos de la aplicaci�n. La condici�n de parada es que la
	// ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
	// bot�n de cerrar la ventana (la X).
	while (!glfwWindowShouldClose(window)) {
		// - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
		// teclas o de rat�n, etc. Siempre al final de cada iteraci�n del ciclo de
		// eventos y despu�s de glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// - Una vez terminado el ciclo de eventos, liberar recursos, etc.
	std::cout << "Finishing application PAG1819-Serrano-Casas-Javier" << std::endl;
	glfwDestroyWindow(window); // - Cerramos y destruimos la ventana de la aplicaci�n.
	glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
					 // - Esta llamada es para impedir que la consola se cierre inmediatamente tras la
					 // ejecuci�n y poder leer los mensajes.
	system("pause");
}