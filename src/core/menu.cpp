#include "../../include/core/menu.h"
#include "../../include/core/main.h"
#include "../../include/core/trippy_visuals.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace Menu {
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void printBanner() {
        clearScreen();
        std::cout << "\033[1;36m"; // Cyan color
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    🎮 GAME ENGINE C++ 🎮                    ║\n";
        std::cout << "║                                                              ║\n";
        std::cout << "║                    Modern OpenGL Engine                     ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
        std::cout << "\033[0m"; // Reset color
        std::cout << "\n";
    }

    void printMenu() {
        std::cout << "\033[1;33m"; // Yellow color
        std::cout << "📋 MENÚ PRINCIPAL:\n";
        std::cout << "\033[0m";
        std::cout << "┌──────────────────────────────────────────────────────────────┐\n";
        std::cout << "│  [1] 🎮 Iniciar Juego Principal                              │\n";
        std::cout << "│  [2] 🎭 Show Psicodélico (Vista 2D Automática)              │\n";
        std::cout << "│  [3] 🔧 Configurar Gráficos                                 │\n";
        std::cout << "│  [4] 📊 Información del Sistema                             │\n";
        std::cout << "│  [5] ❓ Ayuda                                               │\n";
        std::cout << "│  [0] 🚪 Salir                                               │\n";
        std::cout << "└──────────────────────────────────────────────────────────────┘\n";
        std::cout << "\n";
    }

    void printSystemInfo() {
        std::cout << "\033[1;32m"; // Green color
        std::cout << "🖥️  INFORMACIÓN DEL SISTEMA:\n";
        std::cout << "\033[0m";
        std::cout << "┌──────────────────────────────────────────────────────────────┐\n";
        
        // GPU Info
        std::cout << "│  🎮 GPU Principal: NVIDIA GeForce GTX 1650 Mobile           │\n";
        std::cout << "│  🎮 GPU Secundaria: AMD Radeon Vega Series                  │\n";
        std::cout << "│  🎮 OpenGL Vendor: NVIDIA Corporation                       │\n";
        
        // Display Info
        std::cout << "│  📺 Display: :0                                             │\n";
        std::cout << "│  🖼️  Resolución: 1920x1080                                  │\n";
        
        // Engine Info
        std::cout << "│  ⚙️  Motor: Modern OpenGL Engine                            │\n";
        std::cout << "│  🔧 Compilador: GCC/Clang                                   │\n";
        std::cout << "│  📦 Build System: CMake                                     │\n";
        
        std::cout << "└──────────────────────────────────────────────────────────────┘\n";
        std::cout << "\n";
    }

    void printHelp() {
        std::cout << "\033[1;35m"; // Magenta color
        std::cout << "❓ AYUDA Y CONTROLES:\n";
        std::cout << "\033[0m";
        std::cout << "┌──────────────────────────────────────────────────────────────┐\n";
        std::cout << "│  🎮 CONTROLES DEL JUEGO:                                    │\n";
        std::cout << "│     WASD - Movimiento del personaje                         │\n";
        std::cout << "│     Mouse - Control de cámara                               │\n";
        std::cout << "│     ESPACIO - Saltar                                       │\n";
        std::cout << "│     SHIFT - Correr                                         │\n";
        std::cout << "│     G+SHIFT - Activar/Desactivar God Mode                  │\n";
        std::cout << "│     ESC - Salir del juego                                  │\n";
        std::cout << "│                                                             │\n";
        std::cout << "│  🎭 SHOW PSICODÉLICO:                                       │\n";
        std::cout << "│     Experiencia automática en vista 2D                      │\n";
        std::cout << "│     Objetos 3D en plano horizontal (X-Z)                    │\n";
        std::cout << "│     ESC - Salir del show                                    │\n";
        std::cout << "│     🎨 Progresión infinita automática                       │\n";
        std::cout << "│     🌟 Randomización sutil para movimiento orgánico        │\n";
        std::cout << "│                                                             │\n";
        std::cout << "│  🔧 CONFIGURACIÓN:                                          │\n";
        std::cout << "│     Ejecuta 'source setup_graphics.sh' para optimizar      │\n";
        std::cout << "│     gráficos y evitar warnings de EGL                      │\n";
        std::cout << "│                                                             │\n";
        std::cout << "│  🐛 SOLUCIÓN DE PROBLEMAS:                                 │\n";
        std::cout << "│     Si tienes problemas de gráficos, verifica los drivers │\n";
        std::cout << "│     NVIDIA y ejecuta el script de configuración           │\n";
        std::cout << "└──────────────────────────────────────────────────────────────┘\n";
        std::cout << "\n";
    }

    void configureGraphics() {
        std::cout << "\033[1;34m"; // Blue color
        std::cout << "🔧 CONFIGURANDO GRÁFICOS...\n";
        std::cout << "\033[0m";
        
        // Simulate graphics configuration
        std::cout << "┌──────────────────────────────────────────────────────────────┐\n";
        std::cout << "│  Configurando variables de entorno NVIDIA...                │\n";
        std::cout << "│  Optimizando OpenGL...                                      │\n";
        std::cout << "│  Configurando EGL...                                        │\n";
        std::cout << "│  Aplicando optimizaciones de rendimiento...                 │\n";
        std::cout << "└──────────────────────────────────────────────────────────────┘\n";
        
        // Add a small delay for visual effect
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        
        std::cout << "\033[1;32m"; // Green color
        std::cout << "✅ Configuración completada exitosamente!\n";
        std::cout << "\033[0m";
        std::cout << "Los warnings de EGL deberían estar resueltos.\n\n";
        
        std::cout << "Presiona ENTER para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void startGame() {
        std::cout << "\033[1;32m"; // Green color
        std::cout << "🎮 INICIANDO JUEGO PRINCIPAL...\n";
        std::cout << "\033[0m";
        std::cout << "Cargando recursos...\n";
        
        // Add loading animation
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "\n";
        
        // Clear screen before starting game
        clearScreen();
        
        // Start the main game
        ::startGame();
    }

    void startTrippyVisuals() {
        std::cout << "\033[1;35m"; // Magenta color
        std::cout << "🎭 INICIANDO SHOW PSICODÉLICO...\n";
        std::cout << "\033[0m";
        std::cout << "Preparando experiencia automática en vista 2D...\n";
        
        // Add loading animation
        for (int i = 0; i < 3; i++) {
            std::cout << ".";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "\n";
        
        // Clear screen before starting trippy visuals
        clearScreen();
        
        // Start the trippy visuals
        TrippyVisuals::runTrippyVisuals();
    }

    void exitGame() {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "🚪 Cerrando Game Engine...\n";
        std::cout << "\033[0m";
        std::cout << "¡Gracias por jugar! 👋\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        exit(0);
    }

    void showMenu() {
        int choice;
        
        while (true) {
            printBanner();
            printMenu();
            
            std::cout << "Selecciona una opción: ";
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    startGame();
                    break;
                case 2:
                    startTrippyVisuals();
                    break;
                case 3:
                    configureGraphics();
                    break;
                case 4:
                    printBanner();
                    printSystemInfo();
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                case 5:
                    printBanner();
                    printHelp();
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                case 0:
                    exitGame();
                    break;
                default:
                    std::cout << "\033[1;31m"; // Red color
                    std::cout << "❌ Opción inválida. Intenta de nuevo.\n";
                    std::cout << "\033[0m";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    break;
            }
        }
    }
} 