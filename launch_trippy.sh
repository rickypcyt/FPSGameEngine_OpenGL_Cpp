#!/bin/bash

# Trippy Visuals Launch Script
# Automatically configures graphics and launches the psychedelic experience

echo "🎭 TRIPPY VISUALS LAUNCHER 🎭"
echo "================================"
echo ""

# Configure graphics automatically
echo "🔧 Configurando gráficos para experiencia óptima..."
export __GL_SYNC_TO_VBLANK=0
export __GL_THREADED_OPTIMIZATIONS=1
export __GL_YIELD=NOTHING
export __NV_PRIME_RENDER_OFFLOAD=1
export __VK_LAYER_NV_optimus=NVIDIA_only
export MESA_GL_VERSION_OVERRIDE=4.6
export MESA_GLSL_VERSION_OVERRIDE=460
export EGL_LOG_LEVEL=error

echo "✅ Gráficos configurados!"
echo ""

# Build the project if needed
if [ ! -f "build/My3DGame" ]; then
    echo "🔨 Compilando proyecto..."
    cd build
    make
    cd ..
fi

echo "🚀 Lanzando Trippy Visuals..."
echo "Presiona ESC para salir"
echo ""

# Launch the game with trippy visuals
cd build
./My3DGame 