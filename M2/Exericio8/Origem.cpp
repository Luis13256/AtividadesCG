/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Erico Luis
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 12/05/2023
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupShader();
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// C�digo fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//C�difo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

bool rotateX = false, rotateY = false, rotateZ = false;
float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
glm::vec3 position(0.0f, 0.0f, 0.0f);
float scale = 1.0f;
std::vector<glm::vec3> positions;

// Fun��o MAIN
int main()
{
    // Inicializa��o da GLFW
    glfwInit();

    //Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
    //Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
    //Sugest�o: comente essas linhas de c�digo para descobrir a vers�o e
    //depois atualize (por exemplo: 4.5 com 4 e 5)
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Essencial para computadores da Apple
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

    // Cria��o da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D -- Erico!", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da fun��o de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros de fun��es da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Obtendo as informa��es de vers�o
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Compilando e buildando o programa de shader
    GLuint shaderID = setupShader();

    // Gerando um buffer simples, com a geometria de um cubo
    GLuint VAO = setupGeometry();

    glUseProgram(shaderID);

    // Definindo a matriz de proje��o e vis�o
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    GLint viewLoc = glGetUniformLocation(shaderID, "view");
    GLint projLoc = glGetUniformLocation(shaderID, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);

    // Loop da aplica��o - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLineWidth(10);
        glPointSize(20);

        float deltaTime = (GLfloat)glfwGetTime();
        float rotationSpeed = glm::radians(0.1f); // Velocidade de rota��o reduzida

        if (rotateX)
        {
            rotationX += deltaTime * rotationSpeed;
        }
        if (rotateY)
        {
            rotationY += deltaTime * rotationSpeed;
        }
        if (rotateZ)
        {
            rotationZ += deltaTime * rotationSpeed;
        }

        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(scale));
        model = glm::rotate(model, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        for (const auto& pos : positions)
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(scale));
            model = glm::rotate(model, rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        rotateX = !rotateX;
    }
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        rotateY = !rotateY;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        rotateZ = !rotateZ;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        position.z -= 0.1f;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        position.z += 0.1f;
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        position.x -= 0.1f;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        position.x += 0.1f;
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
        position.y += 0.1f;
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
        position.y -= 0.1f;
    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
        scale -= 0.1f;
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
        scale += 0.1f;
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
        positions.push_back(position); // Instancia um novo cubo
}

// Esta fun��o est� bastante hardcoded - objetivo � compilar e "buildar" um programa de
// shader simples e �nico neste exemplo de c�digo
// O c�digo fonte do vertex e fragment shader est� nos arrays vertexShaderSource e
// fragmentShader source no in�cio deste arquivo
// A fun��o retorna o identificador do programa de shader
int setupShader()
{
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Checando erros de compila��o (exibi��o via log no terminal)
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Checando erros de compila��o (exibi��o via log no terminal)
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Linkando os shaders e criando o identificador do programa de shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Checando por erros de linkagem
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Esta fun��o est� bastante hardcoded - objetivo � criar os buffers que armazenam a 
// geometria de um cubo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
    GLfloat vertices[] = {
        // Posi��o            // Cor
        // Face 1 (vermelho)
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        // Face 2 (verde)
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        // Face 3 (azul)
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        // Face 4 (amarelo)
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         // Face 5 (ciano)
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         // Face 6 (magenta)
         -0.5f,   0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,    0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,    0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,    0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         -0.5f,    0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         -0.5f,    0.5f, -0.5f,  1.0f, 0.0f, 1.0f
    };

    GLuint VBO, VAO;

    // Gera��o do identificador do VBO
    glGenBuffers(1, &VBO);

    // Faz a conex�o (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Envia os dados do array de floats para o buffer da OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Gera��o do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);

    // Vincula (bind) o VAO primeiro, e em seguida conecta e seta o(s) buffer(s) de v�rtices
    // e os ponteiros para os atributos 
    glBindVertexArray(VAO);

    // Para cada atributo do v�rtice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
    // Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
    // N�mero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
    // Tipo do dado
    // Se est� normalizado (entre zero e um)
    // Tamanho em bytes 
    // Deslocamento a partir do byte zero 

    // Atributo posi��o (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo cor (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
    // atualmente vinculado - para que depois possamos desvincular com seguran�a
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}
