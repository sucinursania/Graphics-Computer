#include <GL/glut.h>
#include <cmath>

float angle = 0.0f;
const float towerTilt = 3.99f; // Tower's tilt in degrees

void drawCylinder(float radius, float height, int segments) {
    float x, y, z;
    float angle;

    // Draw the tube
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        angle = (float)i * 2.0f * M_PI / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Draw top and bottom circles
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
void initLighting() {
    // Aktifkan pencahayaan
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // Posisi cahaya
    GLfloat lightPos[] = {50.0f, 50.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Warna pencahayaan (hangat untuk kedalaman)
    GLfloat ambientLight[] = {0.3f, 0.25f, 0.2f, 1.0f};  // Ambient hangat
    GLfloat diffuseLight[] = {0.8f, 0.7f, 0.6f, 1.0f};   // Diffuse hangat
    GLfloat specularLight[] = {1.0f, 0.9f, 0.8f, 1.0f};  // Specular keemasan
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Global ambient light untuk kedalaman
    GLfloat globalAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

void setMarbleMaterial() {
    GLfloat marbleAmb[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Menara putih murni
    GLfloat marbleDif[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Diffuse putih
    GLfloat marbleSpec[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Specular putih
    GLfloat marbleShininess = 80.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, marbleAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, marbleDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, marbleSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, marbleShininess);
}

void setGroundMaterial() {
    // Material untuk tanah dengan warna hijau-coklat
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
    GLfloat windowAmb[] = {0.8f, 0.8f, 0.8f, 1.0f}; // Warna ambient putih keabuan
	GLfloat windowDif[] = {0.9f, 0.9f, 0.9f, 1.0f}; // Warna diffuse putih terang keabuan
	GLfloat windowSpec[] = {0.7f, 0.7f, 0.7f, 1.0f}; // Warna specular abu-abu lembut
	GLfloat windowShininess = 30.0f;                // Tingkat shininess lebih rendah


    glMaterialfv(GL_FRONT, GL_AMBIENT, windowAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, windowDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, windowSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, windowShininess);
}

void drawWindow(float width, float height) {
    setWindowMaterial();
    glPushMatrix();
    glScalef(width, 0.01f, height);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawTower() {
    glPushMatrix();
    glRotatef(towerTilt, 0.0f, 1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);  // Make the tower smaller
    
    setMarbleMaterial();
    
    // Main tower body (8 levels)
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * 1.0f);
        drawCylinder(0.8f - (i * 0.03f), 1.0f, 32);
        
        // Decorative columns around each level
        for(int j = 0; j < 8; j++) {
            float angleCol = j * 45.0f;
            float radCol = 0.85f - (i * 0.03f);
            glPushMatrix();
            glRotatef(angleCol, 0.0f, 0.0f, 1.0f);
            glTranslatef(radCol, 0.0f, 0.0f);
            drawCylinder(0.05f, 1.0f, 8);
            glPopMatrix();
            
            // Add windows between columns
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Position camera closer to the tower
    gluLookAt(3.0f * cos(angle), 3.0f * sin(angle), 3.0f,
              0.0f, 1.0f, 3.0f,
              0.0f, 1.0f, 1.0f);
    
    // Update light position to rotate with camera
    GLfloat lightPos[] = {3.0f * cos(angle), 3.0f * sin(angle), 3.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    // Draw ground
    setGroundMaterial();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // Normal pointing up
    glVertex3f(-10.0f, -10.0f, 0.0f);
    glVertex3f(-10.0f, 10.0f, 0.0f);
    glVertex3f(10.0f, 10.0f, 0.0f);
    glVertex3f(10.0f, -10.0f, 0.0f);
    glEnd();
    
    drawTower();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Menara Pissa");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // Initialize lighting
    initLighting();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    
    glutMainLoop();
    return 0;
}
