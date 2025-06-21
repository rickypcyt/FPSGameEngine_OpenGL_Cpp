#ifndef MENU_H
#define MENU_H

namespace Menu {
    // Main menu functions
    void showMenu();
    void printBanner();
    void printMenu();
    
    // Menu options
    void startGame();
    void startTrippyVisuals();
    void configureGraphics();
    void printSystemInfo();
    void printHelp();
    void exitGame();
    
    // Utility functions
    void clearScreen();
}

#endif // MENU_H 