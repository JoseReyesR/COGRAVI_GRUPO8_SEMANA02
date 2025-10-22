# COGRAVI_GRUPO8_SEMANA02
// EJERCICIO DE LA SEMANA 02 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

float vertices[] =
{
	/* Position */ /* Color */
	-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
	0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,
	0.0f, 0.5f,0.0f, 0.0f,0.0f,1.0f,
};

void estatico(GLFWwindow* window, int width, int height);

const char* VertexShaderData =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
" ourColor = aColor;\n"
" gl_Position = vec4(aPos , 1.0f);\n"
"}\0";

const char* FragmentShaderData =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform float xColor;\n"
"uniform float yColor;\n"
"uniform float zColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(xColor * ourColor.x, yColor * ourColor.y,zColor * ourColor.z ,1.0f);\n"
"}\0";

// SHADER #2 (INVERTIDO) 
// 1.	Creen una segunda cadena de Fragment Shader, FragmentShaderData_Invertido. 
//		Hagan que invierta el color: FragColor = vec4(1.0 - (xColor * ourColor.x), ...);.
const char* FragmentShaderData_Invertido =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform float xColor;\n"
"uniform float yColor;\n"
"uniform float zColor;\n"
"void main()\n"
"{\n"
// Invierte el color restando de 1.0 a todo RGB
" FragColor = vec4(1.0 - (xColor * ourColor.x), 1.0 - (yColor * ourColor.y), 1.0 - (zColor * ourColor.z) ,1.0f);\n"
"}\0";

// 3. En C++, creen una variable bool modo_invertido = false;.
bool modo_invertido = false;

// 4. Cuando 'F' se presione, cambien modo_invertido = !modo_invertido;.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		modo_invertido = !modo_invertido; // alternar modo
		std::cout << "Modo invertido: " << (modo_invertido ? "ACTIVADO" : "DESACTIVADO") << std::endl;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(1000, 800, "Mi primera ventana en OpenGL 3.3", NULL, NULL);
	if (!window)
	{
		std::cout << "Error al crear la ventana\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, estatico);

	// 4. Usen glfwSetKeyCallback para detectar un solo pulso de tecla (p.ej., 'F'). 
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error al iniciar Glew\n";
		glfwTerminate();
	}
	
	//shader
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex, 1, &VertexShaderData, NULL);
	glCompileShader(vertex);

	int success;
	char infolog[1024];

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infolog);
		std::cout << "Error en el shader del Vertice<" << infolog << std::endl;
	}

	// *********************** PROGRAMA NORMAL ***************************** //
	//fragmento
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment, 1, &FragmentShaderData, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 1024, NULL, infolog);
		std::cout << "Error en el SHADER...FRAGMENTO::<" << infolog << ">" << std::endl;
	}

	//Programa
	GLuint program = glCreateProgram();

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infolog);
		std::cout << "Error en el programa<" << infolog << ">" << std::endl;
	}

	// *********************** PROGRAMA INVERTIDO ***************************** //
	// 2.	Compilen y vinculen un segundo programa de shader (GLuint program_invertido;).
	//fragmento
	GLuint fragment_invertido = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragment_invertido, 1, &FragmentShaderData_Invertido, NULL);
	glCompileShader(fragment_invertido);
	glGetShaderiv(fragment_invertido, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment_invertido, 1024, NULL, infolog);
		std::cout << "Error en el SHADER...FRAGMENTO INVERTIDO::<" << infolog << ">" << std::endl;
	}

	//Programa
	GLuint program_invertido = glCreateProgram();

	glAttachShader(program_invertido, vertex);
	glAttachShader(program_invertido, fragment_invertido);
	glLinkProgram(program_invertido);
	glGetProgramiv(program_invertido, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program_invertido, 1024, NULL, infolog);
		std::cout << "Error en el programa invertido<" << infolog << ">" << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(fragment_invertido);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();

		float xValue = std::cos(time) / 2.0f + 0.5f; // 0.0f - 1.0f
		float yValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f
		float zValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f

		glUniform1f(glGetUniformLocation(program, "zColor"), zValue);
		glUniform1f(glGetUniformLocation(program, "yColor"), yValue);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // 0.0 - 1.0
		glClear(GL_COLOR_BUFFER_BIT);

		if (modo_invertido)
		{
			glUseProgram(program_invertido);
			glUniform1f(glGetUniformLocation(program_invertido, "xColor"), xValue);
			glUniform1f(glGetUniformLocation(program_invertido, "yColor"), yValue);
			glUniform1f(glGetUniformLocation(program_invertido, "zColor"), zValue);
		}
		else {
			glUseProgram(program);
			glUniform1f(glGetUniformLocation(program, "xColor"), xValue);
			glUniform1f(glGetUniformLocation(program, "yColor"), yValue);
			glUniform1f(glGetUniformLocation(program, "zColor"), zValue);
		}

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

}

void estatico(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
