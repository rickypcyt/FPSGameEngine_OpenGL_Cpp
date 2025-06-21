# Guía de OpenGL Moderno para tu Motor de Juegos

## ¿Qué hemos creado?

He modernizado tu motor de renderizado reemplazando el OpenGL en modo inmediato (obsoleto) con técnicas modernas que usan todos los motores de juegos actuales.

## Componentes Principales

### 1. **Shader Class** (`include/graphics/shader.h`, `src/graphics/shader.cpp`)

**¿Qué hace?** Maneja la compilación y gestión de shaders GLSL.

**Características:**
- Carga shaders desde archivos o strings
- Compilación automática de vertex y fragment shaders
- Gestión de uniforms (variables que pasan datos CPU → GPU)
- Manejo de errores de compilación

**Ejemplo de uso:**
```cpp
Shader shader;
shader.loadFromFiles("vertex.glsl", "fragment.glsl");
shader.use();
shader.setMat4("model", modelMatrix);
shader.setVec3("lightPos", lightPosition);
```

### 2. **Mesh Class** (`include/graphics/mesh.h`, `src/graphics/mesh.cpp`)

**¿Qué hace?** Maneja VAOs y VBOs para renderizado eficiente.

**Características:**
- Crea y gestiona Vertex Array Objects (VAOs)
- Crea y gestiona Vertex Buffer Objects (VBOs)
- Configura atributos de vértices (posición, normal, textura, color)
- Métodos factory para crear geometrías comunes (cubo, plano, esfera)

**Ejemplo de uso:**
```cpp
// Crear un cubo
Mesh cube = Mesh::createCube(1.0f);
cube.draw(); // Renderiza el cubo

// Crear un plano
Mesh plane = Mesh::createPlane(10.0f, 10.0f);
plane.draw(); // Renderiza el plano
```

### 3. **ModernRenderer Class** (`include/graphics/modern_renderer.h`, `src/graphics/modern_renderer.cpp`)

**¿Qué hace?** Sistema de renderizado completo que integra todo.

**Características:**
- Gestión de cámara con matrices de vista
- Sistema de iluminación
- Transformaciones con matrices GLM
- Renderizado de objetos 3D

**Ejemplo de uso:**
```cpp
ModernRenderer renderer;
renderer.initialize();

// Configurar cámara
renderer.setCameraPosition(glm::vec3(0, 0, 3));
renderer.setCameraDirection(-90.0f, 0.0f);

// Configurar iluminación
renderer.setLightPosition(glm::vec3(2, 2, 2));
renderer.enableLighting(true);

// Renderizar
renderer.beginFrame();
renderer.drawCube(glm::vec3(0, 0, 0));
renderer.drawSphere(glm::vec3(2, 0, 0), 0.5f);
renderer.endFrame();
```

## Shaders GLSL

### Vertex Shader (`src/shaders/modern_vertex.glsl`)
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;        // Posición del vértice
layout (location = 1) in vec3 aNormal;     // Normal del vértice
layout (location = 2) in vec2 aTexCoords;  // Coordenadas de textura
layout (location = 3) in vec3 aColor;      // Color del vértice

out vec3 FragPos;    // Posición en espacio mundial
out vec3 Normal;     // Normal transformada
out vec2 TexCoords;  // Coordenadas de textura
out vec3 Color;      // Color

uniform mat4 model;      // Matriz de modelo
uniform mat4 view;       // Matriz de vista
uniform mat4 projection; // Matriz de proyección

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    Color = aColor;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```

### Fragment Shader (`src/shaders/modern_fragment.glsl`)
```glsl
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Color;

uniform vec3 lightPos;    // Posición de la luz
uniform vec3 lightColor;  // Color de la luz
uniform vec3 viewPos;     // Posición de la cámara
uniform bool useLighting; // ¿Usar iluminación?

void main() {
    vec3 result;
    
    if (useLighting) {
        // Iluminación ambiental
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        
        // Iluminación difusa
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // Iluminación especular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
        
        result = (ambient + diffuse + specular) * Color;
    } else {
        result = Color;
    }
    
    FragColor = vec4(result, 1.0);
}
```

## Matrices GLM

### Transformaciones Básicas
```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Crear matriz de modelo
glm::mat4 model = glm::mat4(1.0f);

// Traslación
model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

// Rotación (eje Y, 45 grados)
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// Escala
model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
```

### Matrices de Cámara
```cpp
// Matriz de proyección (perspectiva)
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

// Matriz de vista (cámara)
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

## Diferencias con tu Código Anterior

### ❌ **Antes (OpenGL Inmediato - Obsoleto):**
```cpp
void drawCube(float size) {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glEnd();
}
```

### ✅ **Ahora (OpenGL Moderno):**
```cpp
// Se crea una vez
Mesh cube = Mesh::createCube(1.0f);

// Se renderiza cada frame
void renderFrame() {
    shader.use();
    shader.setMat4("model", modelMatrix);
    cube.draw(); // ¡Mucho más rápido!
}
```

## Ventajas del Enfoque Moderno

1. **Rendimiento:** 10-100x más rápido
2. **GPU:** Toda la lógica se ejecuta en la GPU
3. **Flexibilidad:** Shaders personalizables
4. **Estandarización:** Usado por todos los motores modernos
5. **Escalabilidad:** Puede manejar miles de objetos

## Próximos Pasos

1. **Instalar GLM:** `sudo pacman -S glm` (Arch Linux)
2. **Compilar:** `mkdir build && cd build && cmake .. && make`
3. **Ejecutar demo:** `./My3DGame`
4. **Experimentar:** Modifica los shaders y matrices

## Recursos Adicionales

- [LearnOpenGL.com](https://learnopengl.com/) - El mejor tutorial
- [OpenGL Documentation](https://www.opengl.org/documentation/)
- [GLM Documentation](https://glm.g-truc.net/)

¡Ahora tu motor está listo para el siglo XXI! 🚀 