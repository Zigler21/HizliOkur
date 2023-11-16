// Main.cpp
#pragma comment(lib,"glew32.lib")
#include <gl/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Font.hpp"
#include "FPSLimiter.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>

glutText glText;
FPSLimiter fpsLimiter;

const int TARGET_FPS = 60;
unsigned int buttontexture;
int mode{ 0 };
int speed{ 1 };
std::vector<std::string> csvList;
int textSpeed;
auto lastFrameTime = std::chrono::high_resolution_clock::now();
void limitFPS(int targetFPS) {
    // Get the current time
    auto currentTime = std::chrono::high_resolution_clock::now();

    // CainitWindowlculate the time difference between the current frame and the last frame
    auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();

    // If the desired frame time (1.0 / FPS) is greater than the actual frame time, sleep for the remaining time
    if (frameTime < 1.0f / targetFPS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((1.0f / targetFPS - frameTime) * 1000)));
    }
    lastFrameTime = currentTime;
}
void initWindow() {
    glutInitContextVersion(2, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutCreateWindow("HizliOkur");
    //glClearColor(.6f, .3f, .3f, 1.0f);

    glClearColor(.0f, .0f, .0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, 1, 0, 1);
   // glViewport(0, 0, 1, 1); //pencere
    glOrtho(0, 1, 0, 1, -1.0, 1.0); // float range
    glMatrixMode(GL_MODELVIEW);
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        //std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        exit(EXIT_FAILURE);
    }
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
std::vector<std::string> loadCsvToList(const std::string& filePath) {
    csvList.clear();
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        // Handle the error, possibly by returning an empty list or throwing an exception
        return csvList;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string word;
        while (std::getline(ss, word, ',')) {
            csvList.push_back(word);
        }
    }

    inputFile.close();
    return csvList;
}

void load_texture() {
    int width, height, nrChannels;
    unsigned char* data = stbi_load("./assets/textures/button.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Error loading button texture" << std::endl;
        return;
    }

    glGenTextures(1, &buttontexture);
    glBindTexture(GL_TEXTURE_2D, buttontexture);

    // Set texture parameters for clamping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture parameters for filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Free loaded image
    stbi_image_free(data);
}


void renderButton() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttontexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1, -1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1, -0.7);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(-0.85, -0.7);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(-0.85, -1);

    glEnd();

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-0.6, -1); // Adjusted parameters

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-0.6, -0.7); // Adjusted parameters

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(-0.45, -0.7); // Adjusted parameters

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(-0.45, -1); // Adjusted parameters

    glEnd();
}


int oldmode = mode;
void displayText() {
    glBindTexture(GL_TEXTURE_2D, glText.getTextureID());
    glColor3f(0.5f, 0.2f, 0.5f);
   // glText.printText("Aa&@78t148736875678", 1, .0, 0.);
    switch (mode) {
    case 0:
        glText.printText("S", .5, -0.8, -0.775); break;
    case 1:
        glText.printText("1", .5, -0.8, -0.775);  break;
    case 2:
        glText.printText("2", .5, -0.8, -0.775);  break;
    case 3:
        glText.printText("3", .5, -0.8, -0.775); break;
    case 4:
        glText.printText("4", .5, -0.8, -0.775); break;
    }
    switch (speed) {
    case 0:
        glText.printText("20", .5, -0.4, -0.775); textSpeed = 20; break;
    case 1:
        glText.printText("50", .5, -0.4, -0.775); textSpeed = 50; break;
    case 2:
        glText.printText("100", .5, -0.4, -0.775); textSpeed = 100; break;
    case 3:
        glText.printText("150", .5, -0.4, -0.775); textSpeed = 150; break;
    case 4:
        glText.printText("200", .5, -0.4, -0.775); textSpeed = 200; break;
    }
}

void display() {
    fpsLimiter.beginFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    displayText();
    renderButton();

    glutSwapBuffers();

    fpsLimiter.endFrame(TARGET_FPS);
}


void mouse(int button, int state, int x, int y) {
    
    if (button == 0 && state == 0 ) {
        if (x < glutGet(GLUT_WINDOW_WIDTH) * 0.075 && y > glutGet(GLUT_WINDOW_HEIGHT) * 0.85) {
            if (y < glutGet(GLUT_WINDOW_HEIGHT) * 0.925) {
                if (mode < 4) { mode += 1; }
            }
            else if (y > glutGet(GLUT_WINDOW_HEIGHT) * 0.925 && mode>0) {
                mode -= 1;
            }

        }
        else if (y > glutGet(GLUT_WINDOW_HEIGHT) * 0.85 && x < glutGet(GLUT_WINDOW_WIDTH) * 0.275 && x > glutGet(GLUT_WINDOW_WIDTH) * 0.2) {
            if (y < glutGet(GLUT_WINDOW_HEIGHT) * 0.925) {
                if (speed < 4) { speed += 1; }
            }
            else if (y > glutGet(GLUT_WINDOW_HEIGHT) * 0.925 && speed > 0) {
                speed -= 1;
            }
           
        }
        else {
            std::size_t randomIndex = std::rand() % csvList.size();
            std::string tempstr = csvList[randomIndex];
            std::this_thread::sleep_for(std::chrono::milliseconds(1300));
            glText.printText(tempstr.c_str() , .9, 0, 0.2);
            glutSwapBuffers();
            std::this_thread::sleep_for(std::chrono::milliseconds(textSpeed));
        }
    }
    if (button == 2 && state == 0) {
        glutLeaveMainLoop();
    }
    if (oldmode != mode) {
        switch (mode) {
        case 0:
            csvList = loadCsvToList("./assets/lists/numaralar.csv"); break;
        case 1:
            csvList = loadCsvToList("./assets/lists/1kelime.csv"); break;
        case 2:
            csvList = loadCsvToList("./assets/lists/2kelime.csv"); break;
        case 3:
            csvList = loadCsvToList("./assets/lists/3kelime.csv"); break;
        case 4:
            csvList = loadCsvToList("./assets/lists/4kelime.csv"); break;
        }
        oldmode = mode;
    }

}
void idle() {
    glutPostRedisplay();
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//int main(){
    int argc=0;
    char** argv = NULL;
    glutInit(&argc, argv);
    initWindow();
    glEnable(GL_TEXTURE_2D);
    load_texture();
    glText.loadFont();
    csvList = loadCsvToList("./assets/lists/numaralar.csv");

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    glText.clearMem();
    return EXIT_SUCCESS;
}