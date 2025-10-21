
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

//  DATOS DE VÉRTICES 
float vertices[] =
{
	/* Position */ /* Color */
	-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f, // Vértice 1 (Rojo)
	0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f, // Vértice 2 (Verde)
	0.0f, 0.5f,0.0f, 0.0f,0.0f,1.0f, // Vértice 3 (Azul)
};

//  SHADER #1 (NORMAL) 
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

//  VARIABLE BANDERA PARA EL INTERCAMBIO
bool modo_invertido = false;

//FUNCIÓN DE CALLBACK DE TECLADO - Cuando se presione una tecla.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Si se presiona la tecla 'F'
	if (key == GLFW_KEY_F && action == GLFW_PRESS) //para  cambiar la tecla se cambia GLFW_KEY_"*"
	{
		// Invierte el valor del booleano
		modo_invertido = !modo_invertido;
		std::cout << "Modo Invertido: " << (modo_invertido ? "Activado" : "Desactivado") << std::endl;
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
	GLFWwindow* window = glfwCreateWindow(1000, 800, "Presiona F para invertir colores", NULL, NULL);
	if (!window)
	{
		std::cout << "Error al crear la ventana\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// LLAMA A LA FUNCION  CALLBACK 
	glfwSetKeyCallback(window, key_callback);
	// REVISA Glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error al iniciar Glew\n";
		glfwTerminate();
	}

	//Compilar Vertex Shader 
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &VertexShaderData, NULL);
	glCompileShader(vertex);
	int success;
	char infolog[1024];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 1024, NULL, infolog);
		std::cout << "FAILED::SHADER::VERTEX::<" << infolog << std::endl;
	}
	//EMPIEZA A COMPILARSE EL PROGRAMA UNO (NORMAL)
	// Compilar Fragment Shader 1 (Normal) 
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &FragmentShaderData, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 1024, NULL, infolog);
		std::cout << "Error en el SHADER...FRAGMENTO::<" << infolog << ">" << std::endl;
	}
	//--- Crear Programa 1 (Normal) ---
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 1024, NULL, infolog);
		std::cout << "FAILED::PROGRAM::<" << infolog << ">" << std::endl;
	}

	//EMPIEZA A COMPILARSE EL PROGRAMA DOS (INVERTIDO)

	//Compilar Fragment Shader 2 (Invertido) 
	GLuint fragment_invertido = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_invertido, 1, &FragmentShaderData_Invertido, NULL);
	glCompileShader(fragment_invertido);
	glGetShaderiv(fragment_invertido, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_invertido, 1024, NULL, infolog);
		std::cout << "Error en el SHADER...FRAGMENTO_INVERTIDO::<" << infolog << ">" << std::endl;
	}

	//Crear Programa 2 (Invertido)
	GLuint program_invertido = glCreateProgram();
	glAttachShader(program_invertido, vertex); // Reutilizamos el mismo Vertex Shader
	glAttachShader(program_invertido, fragment_invertido);
	glLinkProgram(program_invertido);
	glGetProgramiv(program_invertido, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_invertido, 1024, NULL, infolog);
		std::cout << "FAILED::PROGRAM_INVERTIDO::<" << infolog << ">" << std::endl;
	}

	
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(fragment_invertido); //BORRAR EL PROGRAMA INVERTIDO

	// Configuración de VBO y VAO
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

	//  BUCLE PARA RENDERIZADO 
	while (!glfwWindowShouldClose(window))
	{
		

		// cambiar color
		float time = glfwGetTime();
		// MAS ESTETICO
		//float xValue = std::sin(time * 1.0f) / 2.0f + 0.5f; // 0.0f - 1.0f
		//float yValue = std::sin(time * 0.5f) / 2.0f + 0.5f; // 0.0f - 1.0f
		//float zValue = std::sin(time * 0.2f) / 2.0f + 0.5f;
		// 
		//ORIGINAL

		float xValue = std::sin(time) / 2.0f + 0.5f;// 0.0f - 1.0f
		float yValue = std::sin(time) / 2.0f + 0.5f;// 0.0f - 1.0f
		float zValue = std::sin(time) / 2.0f + 0.5f;// 0.0f - 1.0f

		// Limpiar la pantalla
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// SELECCIÓN DE SHADER -> activa la variable bool
		if (modo_invertido)// cuestiona la variable bool si se presiono la tecla F
		{
			// Usar el programa INVERTIDO
			glUseProgram(program_invertido);
			// Actualizar programa INVERTIDO
			glUniform1f(glGetUniformLocation(program_invertido, "xColor"), xValue);
			glUniform1f(glGetUniformLocation(program_invertido, "yColor"), yValue);
			glUniform1f(glGetUniformLocation(program_invertido, "zColor"), zValue);
		}
		else
		{
			// Usar el programa NORMAL
			glUseProgram(program);
			// Actualizar programa NORMAL
			glUniform1f(glGetUniformLocation(program, "xColor"), xValue);
			glUniform1f(glGetUniformLocation(program, "yColor"), yValue);
			glUniform1f(glGetUniformLocation(program, "zColor"), zValue);
		}

		// Dibujar el triángulo
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Intercambiar búferes
		glfwSwapBuffers(window);
		// Llama 'key_callback' 
		glfwPollEvents();
	}

	
	glfwDestroyWindow(window);
	glfwTerminate();
}




//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <cmath>
//float vertices[] =
//{
//	/* Position */ /* Color */
//	-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
//	0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,
//	0.0f, 0.5f,0.0f, 0.0f,0.0f,1.0f,
//};
//
//const char* VertexShaderData =
//"#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//" ourColor = aColor;\n"
//" gl_Position = vec4(aPos , 1.0f);\n"
//"}\0";
//const char* FragmentShaderData =
//"#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec3 ourColor;\n"
//"uniform float xColor;\n"
//"uniform float yColor;\n"
//"uniform float zColor;\n"
//"void main()\n"
//"{\n"
//" FragColor = vec4(xColor * ourColor.x, yColor * ourColor.y,zColor * ourColor.z ,1.0f);\n"
//"}\0";
//
//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//	GLFWwindow* window = glfwCreateWindow(1000, 800, "Mi primera ventana en OpenGL 3.3", NULL, NULL);
//	if (!window)
//	{
//		std::cout << "Error al crear la ventana\n";
//		glfwTerminate();
//	}
//	/* The most important part */
//	glfwMakeContextCurrent(window);
//	if (glewInit() != GLEW_OK)
//	{
//		std::cout << "Error al iniciar Glew\n";
//		glfwTerminate();
//	}
//	//shader
//	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex, 1, &VertexShaderData, NULL);
//	glCompileShader(vertex);
//	int success;
//	char infolog[1024];
//	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(vertex, 1024, NULL, infolog);
//		std::cout << "FAILED::SHADER::VERTEX::<" << infolog << std::endl;
//	}
//	//fragmento
//	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment, 1, &FragmentShaderData, NULL);
//	glCompileShader(fragment);
//	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragment, 1024, NULL, infolog);
//		std::cout << "Error en el SHADER...FRAGMENTO::<" << infolog << ">" << std::endl;
//	}
//	//Programa
//	GLuint program = glCreateProgram();
//	glAttachShader(program, vertex);
//	glAttachShader(program, fragment);
//	glLinkProgram(program);
//	glGetProgramiv(program, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetProgramInfoLog(program, 1024, NULL, infolog);
//		std::cout << "FAILED::PROGRAM::<" << infolog << ">" << std::endl;
//	}
//	glDeleteShader(vertex);
//	glDeleteShader(fragment);
//	unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	while (!glfwWindowShouldClose(window))
//	{
//		float time = glfwGetTime();
//		float xValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f
//		float yValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f
//		float zValue = std::sin(time) / 2.0f + 0.5f; // 0.0f - 1.0f
//		glUniform1f(glGetUniformLocation(program, "xColor"), xValue);
//		glUniform1f(glGetUniformLocation(program, "yColor"), yValue);
//		glUniform1f(glGetUniformLocation(program, "zColor"), zValue);
//		glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // 0.0 - 1.0
//		glClear(GL_COLOR_BUFFER_BIT);
//		glUseProgram(program);
//		glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 3);
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	glfwDestroyWindow(window);
//	glfwTerminate();
//}
//
//