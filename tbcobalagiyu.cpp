#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>

// Variabel untuk rotasi dan posisi
float angle = 0.0f;
const float towerTilt = 3.99f; // Sudut kemiringan menara
float cameraDistance = 5.0f; // Jarak kamera dari pusat

// Tambahkan di bagian deklarasi fungsi
bool showPisaTowerDetails();
bool showPyramidDetails();
void initTextRegions(int windowHeight);

// Fungsi untuk menggambar silinder
void drawCylinder(float radius, float height, int segments) {
    float x, y;
    float angle;

    // Menggambar tabung
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        angle = (float)i * 2.0f * M_PI / segments;
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Menggambar lingkaran atas dan bawah
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

// Fungsi untuk inisialisasi pencahayaan
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    // Posisi cahaya
    GLfloat lightPos[] = {50.0f, 50.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Warna pencahayaan
    GLfloat ambientLight[] = {0.3f, 0.25f, 0.2f, 1.0f};
    GLfloat diffuseLight[] = {0.8f, 0.7f, 0.6f, 1.0f};
    GLfloat specularLight[] = {1.0f, 0.9f, 0.8f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Ambient global
    GLfloat globalAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
}

// Fungsi untuk mengatur material marmer
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

// Fungsi untuk mengatur material tanah
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

// Fungsi untuk menggambar jendela
void drawWindow(float width, float height) {
    setMarbleMaterial();
    glPushMatrix();
    glScalef(width, 0.01f, height);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi untuk menggambar menara
void drawTower() {
    glPushMatrix();
    glRotatef(towerTilt, 0.0f, 1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);  // Menjadikan menara lebih kecil
    
    setMarbleMaterial();
    
    // Badan menara (8 tingkat)
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * 1.0f);
        drawCylinder(0.8f - (i * 0.03f), 1.0f, 32);
        
        // Kolom dekoratif di sekitar setiap tingkat
        for(int j = 0; j < 8; j++) {
            float angleCol = j * 45.0f;
            float radCol = 0.85f - (i * 0.03f);
            glPushMatrix();
            glRotatef(angleCol, 0.0f, 0.0f, 1.0f);
            glTranslatef(radCol, 0.0f, 0.0f);
            drawCylinder(0.05f, 1.0f, 8);
            glPopMatrix();
            
            // Menambahkan jendela di antara kolom
            glPushMatrix();
            glRotatef(angleCol + 22.5f, 0.0f, 0.0f, 1.0f);
            glTranslatef(radCol - 0.1f, 0.0f, 0.5f);
            drawWindow(0.2f, 0.4f);
            glPopMatrix();
        }
        glPopMatrix();
    }
    
    // Kubah atas
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 8.0f);
    glutSolidSphere(0.4f, 32, 32);
    glPopMatrix();
    
    glPopMatrix();
}

// Fungsi untuk menggambar piramida
void drawPyramid() {
    float baseSize = 2.0f;
    float height = 3.0f;
    
    glBegin(GL_TRIANGLES);
    // Front face
    glNormal3f(0.0f, 0.5f, 0.5f);
    glColor3f(0.6f, 0.5f, 0.4f); // Warna batu
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, baseSize);
    // Right face
    glNormal3f(0.5f, 0.5f, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, -baseSize);
    // Back face
    glNormal3f(0.0f, 0.5f, -0.5f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, -baseSize);
    // Left face
    glNormal3f(-0.5f, 0.5f, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, baseSize);
    glEnd();
    // Base
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.5f, 0.4f, 0.3f); // Warna dasar
    glVertex3f(-baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, -baseSize);
    glEnd();
    glPopMatrix(); // Mengembalikan keadaan sebelumnya
}
void renderBitmapString(float x, float y, void *font, const char *string) {
    const char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
// Variabel global untuk status menu
bool showPisaTowerMenu = false;
bool showPyramidMenu = false;

// Struktur untuk menyimpan koordinat teks
struct TextRegion {
    float x, y;
    float width, height;
    bool (*onClick)();
};

// Fungsi untuk menampilkan menu Menara Pisa
bool showPisaTowerDetails() {
    showPisaTowerMenu = !showPisaTowerMenu;
    showPyramidMenu = false;
    return true;
}

// Fungsi untuk menampilkan menu Piramida
bool showPyramidDetails() {
    showPyramidMenu = !showPyramidMenu;
    showPisaTowerMenu = false;
    return true;
}
std::vector<TextRegion> textRegions;

// Isi vektor di dalam fungsi display atau fungsi inisialisasi
void initTextRegions(int windowHeight) {
    textRegions.clear();
    textRegions.push_back({
        10, windowHeight - 30, 100, 20, 
        showPisaTowerDetails
    });
    textRegions.push_back({
        10, windowHeight - 150, 100, 20, 
		showPyramidDetails
    });
}
// Fungsi untuk menangani klik mouse
void mouse(int button, int state, int x, int y) {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Konversi koordinat y karena OpenGL menggunakan koordinat dari bawah ke atas
        y = viewport[3] - y;
        
        // Periksa apakah klik berada dalam area teks
        for (const auto& region : textRegions) {
            if (x >= region.x && x <= region.x + region.width &&
                y >= region.y && y <= region.y + region.height) {
                region.onClick();
                glutPostRedisplay();
                break;
            }
        }
    }
}
// Fungsi untuk menggambar tampilan
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Mengatur posisi kamera
    gluLookAt(cameraDistance * cos(angle), cameraDistance * sin(angle), 5.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f);
    
    // Menggambar tanah
    setGroundMaterial();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-10.0f, -10.0f, 0.0f);
    glVertex3f(-10.0f, 10.0f, 0.0f);
    glVertex3f(10.0f, 10.0f, 0.0f);
    glVertex3f(10.0f, -10.0f, 0.0f);
    glEnd();
    
    // Menggambar menara dan piramida
    drawTower();
    drawPyramid();
    
    // Simpan proyeksi matriks
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(0, viewport[2], 0, viewport[3]);
    initTextRegions(viewport[3]);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Nonaktifkan pencahayaan untuk teks
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Render informasi teks dengan area klik
    renderBitmapString(10, viewport[3] - 30, GLUT_BITMAP_HELVETICA_18, "Menara Pisa");
    
    // Tampilkan detail Menara Pisa jika menu aktif
    if (showPisaTowerMenu) {
        renderBitmapString(10, viewport[3] - 50, GLUT_BITMAP_HELVETICA_12, "Sejarah Lengkap:");
        renderBitmapString(10, viewport[3] - 65, GLUT_BITMAP_HELVETICA_12, "- Lokasi: Kota Pisa, Italia");
        renderBitmapString(10, viewport[3] - 80, GLUT_BITMAP_HELVETICA_12, "- Dibangun: 1173 M");
        renderBitmapString(10, viewport[3] - 95, GLUT_BITMAP_HELVETICA_12, "- Arsitek: Bonanno Pisano");
        renderBitmapString(10, viewport[3] - 110, GLUT_BITMAP_HELVETICA_12, "- Ketinggian: 55.86 meter");
        renderBitmapString(10, viewport[3] - 125, GLUT_BITMAP_HELVETICA_12, "- Kemiringan: 3.99 derajat");
    }

    renderBitmapString(10, viewport[3] - 150, GLUT_BITMAP_HELVETICA_18, "Piramida");
    
    // Tampilkan detail Piramida jika menu aktif
    if (showPyramidMenu) {
        renderBitmapString(10, viewport[3] - 170, GLUT_BITMAP_HELVETICA_12, "Sejarah Lengkap:");
        renderBitmapString(10, viewport[3] - 185, GLUT_BITMAP_HELVETICA_12, "- Lokasi: Giza, Mesir");
        renderBitmapString(10, viewport[3] - 200, GLUT_BITMAP_HELVETICA_12, "- Dibangun: Sekitar 2560 SM");
        renderBitmapString(10, viewport[3] - 215, GLUT_BITMAP_HELVETICA_12, "- Firaun: Khufu");
        renderBitmapString(10, viewport[3] - 230, GLUT_BITMAP_HELVETICA_12, "- Tinggi Asli: 146.5 meter");
        renderBitmapString(10, viewport[3] - 245, GLUT_BITMAP_HELVETICA_12, "- Bahan: Batu kapur");
    }
    // Kembalikan pencahayaan
    glEnable(GL_LIGHTING);
    
    // Kembalikan matriks proyeksi
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glutSwapBuffers(); // Tukar buffer untuk menampilkan gambar
}
// Fungsi untuk mengubah ukuran jendela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk memperbarui posisi
void update(int value) {
    angle += 0.02f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Maju
            cameraDistance -= 0.1f;
            break;
        case 's': // Mundur
            cameraDistance += 0.1f;
            break;
        case 'a': // Geser ke kiri
            angle -= 0.1f;
            break;
        case 'd': // Geser ke kanan
            angle += 0.1f;
            break;
        case 27: // ESC untuk keluar
            exit(0);
            break;
    }
}
// Fungsi utama
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Piramida dan Menara Pisa - OpenGL");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    
    // Inisialisasi pencahayaan
    initLighting();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); // Menambahkan fungsi keyboard
    glutMouseFunc(mouse); //fungsi mouse
    glutTimerFunc(0, update, 0);
    
    glutMainLoop(); // Memulai loop utama GLUT
    return 0; // Kembali dari fungsi utama
}