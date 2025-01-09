#include <GL/glut.h>
#include <cmath>

// Global variables for transformations
float rotationX = 0.0f;
float rotationY = 0.0f;
float sunPosition = 0.0f;
float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = 0.0f;
float scaleX = 1.0f;
float scaleY = 1.0f;
float scaleZ = 1.0f;
float angle = 0.0f;
const float towerTilt = 3.99f;

// Texture IDs
GLuint sandTexture;
GLuint stoneTexture;

// Function declarations
void drawCylinder(float radius, float height, int segments);
void drawTower();
void drawPyramid();
void createTextures();

// Texture creation functions
void createSandTexture() {
    const int size = 128;
    unsigned char* data = new unsigned char[size * size * 3];
    
    for(int i = 0; i < size * size; i++) {
        unsigned char value = 200 + (rand() % 56);
        data[i*3] = value;
        data[i*3+1] = value - 40;
        data[i*3+2] = value - 80;
    }

    glGenTextures(1, &sandTexture);
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] data;
}

void createStoneTexture() {
    const int size = 128;
    unsigned char* data = new unsigned char[size * size * 3];
    
    for(int i = 0; i < size * size; i++) {
        unsigned char value = 180 + (rand() % 76);
        data[i*3] = value;
        data[i*3+1] = value - 20;
        data[i*3+2] = value - 40;
    }

    glGenTextures(1, &stoneTexture);
    glBindTexture(GL_TEXTURE_2D, stoneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] data;
}

// Material functions
void setMarbleMaterial() {
    GLfloat marbleAmb[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat marbleDif[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat marbleSpec[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat marbleShininess = 80.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, marbleAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, marbleDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, marbleSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, marbleShininess);
}

void setGroundMaterial() {
    GLfloat groundAmb[] = {0.2f, 0.3f, 0.2f, 1.0f};
    GLfloat groundDif[] = {0.4f, 0.5f, 0.4f, 1.0f};
    GLfloat groundSpec[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat groundShininess = 10.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, groundAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, groundDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, groundSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, groundShininess);
}

void setWindowMaterial() {
    GLfloat windowAmb[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat windowDif[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat windowSpec[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat windowShininess = 30.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, windowAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, windowDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, windowSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, windowShininess);
}

// Drawing functions
void drawWindow(float width, float height) {
    setWindowMaterial();
    glPushMatrix();
    glScalef(width, 0.01f, height);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCylinder(float radius, float height, int segments) {
    float x, y, z;
    float angle;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        angle = (float)i * 2.0f * M_PI / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        angle = (float)i * 2.0f * M_PI / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = segments; i >= 0; i--) {
        angle = (float)i * 2.0f * M_PI / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

void drawTower() {
    glPushMatrix();
    glRotatef(towerTilt, 0.0f, 0.0f, 1.0f);
    
    setMarbleMaterial();
    
    // Main tower body
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * 1.0f);
        drawCylinder(0.8f - (i * 0.03f), 1.0f, 32);
        
        // Decorative columns
        for(int j = 0; j < 8; j++) {
            float angleCol = j * 45.0f;
            float radCol = 0.85f - (i * 0.03f);
            glPushMatrix();
            glRotatef(angleCol, 0.0f, 0.0f, 1.0f);
            glTranslatef(radCol, 0.0f, 0.0f);
            drawCylinder(0.05f, 1.0f, 8);
            glPopMatrix();
            
            // Windows
            glPushMatrix();
            glRotatef(angleCol + 22.5f, 0.0f, 0.0f, 1.0f);
            glTranslatef(radCol - 0.1f, 0.0f, 0.5f);
            drawWindow(0.2f, 0.4f);
            glPopMatrix();
        }
        glPopMatrix();
    }
    
    // Top dome
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 8.0f);
    glutSolidSphere(0.4f, 32, 32);
    glPopMatrix();
    
    glPopMatrix();
}

void drawGround() {
    glBindTexture(GL_TEXTURE_2D, sandTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for(int x = -20; x < 20; x++) {
        for(int z = -20; z < 20; z++) {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x, -1.0f, z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x+1, -1.0f, z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x+1, -1.0f, z+1);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x, -1.0f, z+1);
        }
    }
    glEnd();
}

void drawPyramid() {
    glBindTexture(GL_TEXTURE_2D, stoneTexture);
    float baseSize = 2.0f;
    float height = 3.0f;
    
    glBegin(GL_TRIANGLES);
    // Front face
    glNormal3f(0.0f, 0.5f, 0.5f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-baseSize, 0.0f, baseSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(baseSize, 0.0f, baseSize);

    // Right face
    glNormal3f(0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(baseSize, 0.0f, baseSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(baseSize, 0.0f, -baseSize);

    // Back face
    glNormal3f(0.0f, 0.5f, -0.5f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(baseSize, 0.0f, -baseSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-baseSize, 0.0f, -baseSize);

    // Left face
    glNormal3f(-0.5f, 0.5f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-baseSize, 0.0f, -baseSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-baseSize, 0.0f, baseSize);
    glEnd();

    // Base
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-baseSize, 0.0f, baseSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(baseSize, 0.0f, baseSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(baseSize, 0.0f, -baseSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-baseSize, 0.0f, -baseSize);
    glEnd();
}

void updateSunPosition() {
    sunPosition += 0.5f;
    if(sunPosition > 360.0f) sunPosition = 0.0f;
    
    float sunX = 20.0f * cos(sunPosition * M_PI / 180.0f);
    float sunY = 20.0f * sin(sunPosition * M_PI / 180.0f);
    
    GLfloat light_position[] = { sunX, sunY, 20.0f, 1.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Camera position and orientation
    gluLookAt(12.0f * cos(angle), 8.0f, 12.0f * sin(angle),
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Update lighting
    updateSunPosition();

    // Draw ground
    drawGround();

    // Draw pyramid with transformations
    glPushMatrix();
    glTranslatef(-5.0f + translateX, translateY, translateZ);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glScalef(scaleX, scaleY, scaleZ);
    drawPyramid();
    glPopMatrix();

    // Draw tower
    glPushMatrix();
    glTranslatef(5.0f, 0.0f, 0.0f);
    drawTower();
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void init() {
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    createSandTexture();
    createStoneTexture();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 0.1f;
    const float scaleSpeed = 0.1f;

    switch (key) {
        // Translation
        case 'w': translateZ -= moveSpeed; break;
        case 's': translateZ += moveSpeed; break;
        case 'a': translateX -= moveSpeed; break;
        case 'd': translateX += moveSpeed; break;
        case 'q': translateY += moveSpeed; break;
        case 'e': translateY -= moveSpeed; break;

        // Scaling
        case 'z':
            scaleX += scaleSpeed;
            scaleY += scaleSpeed;
            scaleZ += scaleSpeed;
            break;
        case 'x':
            if (scaleX > scaleSpeed) {
                scaleX -= scaleSpeed;
                scaleY -= scaleSpeed;
                scaleZ -= scaleSpeed;
            }
            break;

        // Reset transformations
        case 'r':
            translateX = translateY = translateZ = 0.0f;
            rotationX = rotationY = 0.0f;
            scaleX = scaleY = scaleZ = 1.0f;
            break;

        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            rotationX += 5.0f;
            break;
        case GLUT_KEY_DOWN:
            rotationX -= 5.0f;
            break;
        case GLUT_KEY_LEFT:
            rotationY -= 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            rotationY += 5.0f;
            break;
    }
    glutPostRedisplay();
}

void update(int value) {
    angle += 0.02f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ancient Wonders - OpenGL");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
