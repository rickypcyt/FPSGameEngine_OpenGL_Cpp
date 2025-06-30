#ifndef TRIPPY_VISUALS_H
#define TRIPPY_VISUALS_H

#include <glm/glm.hpp>
#include <vector>

namespace TrippyVisuals {
    // Main function to run trippy visuals
    void runTrippyVisuals();
    
    // Internal functions (for future expansion)
    bool initialize();
    void cleanup();
    void renderLoop();
    
    // Internal rendering functions
    void initializeMorphingVertices();
    glm::vec3 generatePsychedelicColor(float time, float offset = 0.0f);
    void applyWaveDistortion();
    void drawMeltingGrid();
    void drawMorphingSpheres();
    void drawMorphingCubes();
    void drawPsychedelicTunnel();
    // New fractal and space effects
    void drawFractalTrip();
    void drawStarfield();
    void updateRandomizedParams();
}

#endif // TRIPPY_VISUALS_H 