#include <GL/glut.h>
#include <cmath>
#include <string>
#include <vector>

//=========================================================Variabel untuk rotasi dan posisi===========================================================//
float angle = 0.0f;
const float towerTilt = 3.99f; // Sudut kemiringan menara
float cameraDistance = 5.0f; // Jarak kamera dari pusat

//tambahkan di bagian deklarasi fungsi
bool showPisaTowerDetails();
bool showPyramidDetails();
void initTextRegions(int windowHeight);

//==================================================================membuat menara pisa==(suci)========================================================//

//Fungsi untuk menggambar silinder (suci)
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
// Fungsi untuk menggambar jendela
void drawWindow(float width, float height) {
    setMarbleMaterial();
    glPushMatrix();
    glScalef(width, 0.01f, height);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Fungsi untuk menggambar menara
void drawTower() { // suci
    glPushMatrix();
    glRotatef(towerTilt, 0.0f, 1.0f, 0.0f); //memutar seluruh menara
    glScalef(0.5f, 0.5f, 0.5f);  // Menjadikan menara lebih kecil
    
    setMarbleMaterial();
    
    // Badan menara (8 tingkat)
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, i * 1.0f);// Memindahkan setiap tingkat ke atas
        drawCylinder(0.8f - (i * 0.03f), 1.0f, 32);
        
        // Kolom dekoratif di sekitar setiap tingkat
        for(int j = 0; j < 8; j++) {
            float angleCol = j * 45.0f;
            float radCol = 0.85f - (i * 0.03f);
            glPushMatrix();
            glRotatef(angleCol, 0.0f, 0.0f, 1.0f);//  Memutar kolom di sekitar sumbu Z
            glTranslatef(radCol, 0.0f, 0.0f);// Memindahkan kolom secara radial
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

//===========================================================Fungsi untuk inisialisasi pencahayaan=(hafizh)========================================================//
void initLighting() { // hafizh
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
//===============================================================Fungsi untuk menggambar piramida=======================================================//
void drawPyramid() { // karina
    float baseSize = 1.0f; // Set ukuran basis
    float height = 2.0f;   // Set tinggi piramida
    glDisable(GL_LIGHTING);
    
    // Geser piramida ke posisi yang diinginkan
    glPushMatrix();
    glTranslatef(4.0f, height / 2.0f, 0.0f); // Posisi di samping dan di atas lantai
    glRotatef(90.0f, 10.0f, 0.0f, 0.0f);     // Rotasi 45 derajat pada sumbu Y (y-axis)
    glScalef(1.5, 1.5, 1.5);
    
    // Gambar sisi piramida (segitiga)
    GLfloat pyramidColor[] = {0.96f, 0.87f, 0.70f};  // Warna krem/pasir
    glBegin(GL_TRIANGLES);
    
    // Front face
    glNormal3f(0.0f, height, baseSize);
    glColor3f(0.6f, 0.5f, 0.4f); // Warna
    glVertex3f(0.0f, height, 0.0f); // Puncak
    glVertex3f(-baseSize, 0.0f, baseSize); // Basis kiri depan
    glVertex3f(baseSize, 0.0f, baseSize);  // Basis kanan depan
    
    // Right face
    glNormal3f(baseSize, height, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, -baseSize);

    // Back face
    glNormal3f(0.0f, height, -baseSize);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, -baseSize);

    // Left face
    glNormal3f(-baseSize, height, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, baseSize);
    glEnd();

    // Gambar dasar piramida (persegi)
    glBegin(GL_QUADS);
    glColor3fv(pyramidColor);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glColor3f(0.5f, 0.4f, 0.3f); // Warna dasar
    glVertex3f(-baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, baseSize);
    glVertex3f(baseSize, 0.0f, -baseSize);
    glVertex3f(-baseSize, 0.0f, -baseSize);
    glEnd();
    
    glPopMatrix();
    glEnable(GL_LIGHTING);
}
 //=================================================================menggambar pohon=================================================================//
void drawTree(){ // hafizh
	glDisable(GL_LIGHTING);
    // untuk mengambar batang pohon
	glPushMatrix();
	glTranslatef(-3, 3, 0);
	glRotated(-80, 0.0, 0.0, 0.0); // Rotasi batang pohon -90 derajat pada sumbu X, dengan sedikit pengaruh pada sumbu Z.
	glColor3f(0.6f, 0.4f, 0.2f);  // Warna coklat muda
	gluCylinder(gluNewQuadric(), 0.2, 0.2, 2.0, 20, 50); // Menggambar silinder sebagai batang pohon dengan radius atas 0.5, radius bawah 0.3, dan tinggi 4.0.
	glPopMatrix();
	
	// Menggambar daun (daun utama di atas batang)
	glPushMatrix();
	glTranslatef(-3, 3, 3); // Mentranslasikan posisi ke atas batang (y = 4.0) untuk menempatkan daun.
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glutSolidSphere(1.0, 20, 20); // Menggambar bola dengan radius 1.0 sebagai daun utama.
	glPopMatrix();
	
	// Menggambar daun tambahan di atas
	glPushMatrix();
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glTranslatef(-3, 3, 3.4); // Mentranslasikan posisi lebih ke atas (y = 5.0) untuk menempatkan daun kecil.
	glutSolidSphere(0.9, 20, 20); // Menggambar bola dengan radius 0.9 sebagai daun kecil di atas.
	glPopMatrix();
	
	// Menggambar daun samping kanan
	glPushMatrix();
	glTranslatef(-3, 2, 3); // Mentranslasikan posisi ke kanan (x = 0.8) dan sedikit naik (y = 4.3).
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glutSolidSphere(0.7, 20, 20); // Menggambar bola dengan radius 0.7 sebagai daun samping kanan.
	glPopMatrix();
	
	// Menggambar daun samping depan
	glPushMatrix();
	glTranslatef(-3.6, 2.5, 3); // Mentranslasikan posisi ke depan (z = 0.8) dan sedikit naik (y = 4.3).
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glutSolidSphere(0.7, 20, 20); // Menggambar bola dengan radius 0.7 sebagai daun samping depan.
	glPopMatrix();
	
	// Menggambar daun samping belakang
	glPushMatrix();
	glTranslatef(-2.9, 3.6, 3); // Mentranslasikan posisi ke belakang (z = -0.8) dan sedikit naik (y = 4.3).
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glutSolidSphere(0.7, 20, 20); // Menggambar bola dengan radius 0.7 sebagai daun samping belakang.
	glPopMatrix();
	
	// Menggambar daun samping kiri
	glPushMatrix();
	glTranslatef(-2.3, 3, 3); // Mentranslasikan posisi ke kiri (x = -0.9) dan sedikit naik (y = 4.3).
	glColor3f(0.2f, 0.6f, 0.2f);  // Warna hijau tua
	glutSolidSphere(0.65, 20, 20); // Menggambar bola dengan radius 0.65 sebagai daun samping kiri.
	glPopMatrix();
	glEnable(GL_LIGHTING);
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
struct TextRegion { // suci
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
        500, windowHeight - 30, 100, 20, 
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
void drawCartesius(float length){
    // Nonaktifkan pencahayaan untuk menggambar garis
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);

    // Sumbu X (Merah)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);  // Warna merah untuk sumbu X
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();

    // Sumbu Y (Hijau)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);  // Warna hijau untuk sumbu Y
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);
    glEnd();

    // Sumbu Z (Biru)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);  // Warna biru untuk sumbu Z
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);
    glEnd();

    // Aktifkan kembali pencahayaan
    glEnable(GL_LIGHTING);
}
bool showCartesius = false;
// Tambahkan variabel global untuk kontrol rotasi
bool isRotating = true;
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

    // Gambar garis kartesius hanya jika showCartesius true
    if (showCartesius) {
        drawCartesius(10.0f);  // Panjang garis
    }
     
    // Menggambar menara dan piramida
    drawTower();
    drawPyramid();
    drawTree();
    
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

    // Render informasi teks dengan area klik (suci)
    renderBitmapString(10, viewport[3] - 30, GLUT_BITMAP_TIMES_ROMAN_24, "Menara Pisa");
    
    // Tampilkan detail Menara Pisa jika menu aktif
    if (showPisaTowerMenu) {
        renderBitmapString(10, viewport[3] - 50, GLUT_BITMAP_HELVETICA_18, "Sejarah Singkat:");
        renderBitmapString(10, viewport[3] - 70, GLUT_BITMAP_HELVETICA_18, "- Lokasi: Kota Pisa, Italia");
        renderBitmapString(10, viewport[3] - 90, GLUT_BITMAP_HELVETICA_18, "- Lahir Miring (1173): Karena tanahnya lembek");
        renderBitmapString(10, viewport[3] - 110, GLUT_BITMAP_HELVETICA_18, "- Arsitek: Bonanno Pisano");
        renderBitmapString(10, viewport[3] - 130, GLUT_BITMAP_HELVETICA_18, "- Ketinggian: 55.86 meter");
        renderBitmapString(10, viewport[3] - 150, GLUT_BITMAP_HELVETICA_18, "- Kemiringan: 3.99 derajat");
    }

    renderBitmapString(500, viewport[3] - 30, GLUT_BITMAP_TIMES_ROMAN_24, "Piramida");
    
    // Tampilkan detail Piramida jika menu aktif
    if (showPyramidMenu) {
        renderBitmapString(500, viewport[3] - 50, GLUT_BITMAP_HELVETICA_18, "Sejarah Singkat:");
        renderBitmapString(500, viewport[3] - 70, GLUT_BITMAP_HELVETICA_18, "- Lokasi: Giza, Mesir");
        renderBitmapString(500, viewport[3] - 90, GLUT_BITMAP_HELVETICA_18, "- Dibangun: Sekitar 2560 SM");
        renderBitmapString(500, viewport[3] - 110, GLUT_BITMAP_HELVETICA_18, "- Keajaiban Kuno sekitar 2600 SM");
        renderBitmapString(500, viewport[3] - 130, GLUT_BITMAP_HELVETICA_18, "- Tinggi Asli: 146.5 meter");
        renderBitmapString(500, viewport[3] - 150, GLUT_BITMAP_HELVETICA_18, "- Bahan: Batu kapur");
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
    if (isRotating) {
        angle += 0.02f;
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
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
        case 'c': // Tombol 'c' untuk kartesius
            showCartesius = !showCartesius;
            glutPostRedisplay(); // Meminta redraw ulang layar
            break;
        case 'r': // Tombol 'r' untuk berhenti rotasi
            isRotating = !isRotating;
            glutPostRedisplay();
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
    glutCreateWindow("Landmark Negara");
    
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