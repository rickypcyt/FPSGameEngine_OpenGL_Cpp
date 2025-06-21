#!/bin/bash

# Graphics environment setup for NVIDIA GPU
# This script helps avoid EGL warnings and improves OpenGL performance

echo "Setting up graphics environment for NVIDIA GPU..."

# NVIDIA specific optimizations
export __GL_SYNC_TO_VBLANK=0
export __GL_THREADED_OPTIMIZATIONS=1
export __GL_YIELD=NOTHING

# Force NVIDIA GPU usage
export __NV_PRIME_RENDER_OFFLOAD=1
export __VK_LAYER_NV_optimus=NVIDIA_only

# OpenGL optimizations
export MESA_GL_VERSION_OVERRIDE=4.6
export MESA_GLSL_VERSION_OVERRIDE=460

# Disable EGL warnings (optional - only if warnings are annoying)
export EGL_LOG_LEVEL=error

echo "Graphics environment configured!"
echo "You can now run your game engine without EGL warnings."
echo ""
echo "To make these settings permanent, add them to your ~/.bashrc or ~/.zshrc" 