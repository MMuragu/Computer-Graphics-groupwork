#include <GL/glut.h> //  OpenGL graphics.
#include <cmath> //math functions.
#include <cstdio> //C-style I/O.
#include <iostream> //input/output stream.
#include <vector> //vector library.
using namespace std; // Use standard namespace.

// Polygon vertices array.
float polygonVertices[6][2] = { // Define polygon vertices.
    {8, 4},   // Vertex 1.
    {2, 4},   // Vertex 2.
    {0, 8},   // Vertex 3.
    {3, 12},  // Vertex 4.
    {7, 12},  // Vertex 5.
    {10, 8}   // Vertex 6.
}; // End of array.

// Number of vertices.
const int NUM_VERTICES = 6; // Set vertex count.

// View boundaries constants.
const float LEFT = -2.0f; // Left boundary.
const float RIGHT = 15.0f; // Right boundary.
const float BOTTOM = -2.0f; // Bottom boundary.
const float TOP = 15.0f; // Top boundary.

// Store interior points.
vector<pair<float, float>> interiorPoints; // Vector for points.

void drawAxes() // Draw axes function.
{ // Begin function.
    glColor3f(0.4f, 0.4f, 0.4f); // Set axis color.
    glLineWidth(1.5f); // Set line width.
    
    glBegin(GL_LINES); // Begin drawing lines.
        glVertex2f(LEFT, 0);   glVertex2f(RIGHT, 0); // Draw X-axis.
        glVertex2f(0, BOTTOM); glVertex2f(0, TOP); // Draw Y-axis.
    glEnd(); // End drawing lines.
    
    glColor3f(0.3f, 0.3f, 0.3f); // Set tick color.
    glLineWidth(1.0f); // Set tick width.
    
    for (int i = -2; i <= 15; i++) { // Loop for ticks.
        if (i == 0) continue; // Skip origin tick.
        
        glBegin(GL_LINES); // Begin drawing lines.
            glVertex2f((float)i, -0.2f); // Set vertex.
            glVertex2f((float)i, 0.2f); // Set vertex.
            glVertex2f(-0.2f, (float)i); // Set vertex.
            glVertex2f(0.2f, (float)i); // Set vertex.
        glEnd(); // End drawing lines.
        
        if (i % 2 == 0 || abs(i) == 1) { // If 'i' is even/one.
            char buf[8]; // Character buffer.
            sprintf(buf, "%d", i); // Format 'i' to string.
            
            glRasterPos2f((float)i - 0.2f, -0.7f); // Set label position.
            for (char* p = buf; *p; p++) // Loop through buffer.
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *p); // Draw character.
            
            glRasterPos2f(0.25f, (float)i - 0.15f); // Set label position.
            for (char* p = buf; *p; p++) // Loop through buffer.
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *p); // Draw character.
        } // End if.
    } // End for loop.
    
    glColor3f(0.0f, 0.0f, 0.0f); // Set label color.
    glRasterPos2f(RIGHT - 0.6f, -0.6f); // Set X label position.
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X'); // Draw 'X'.
    
    glRasterPos2f(0.3f, TOP - 0.5f); // Set Y label position.
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y'); // Draw 'Y'.
    
    glRasterPos2f(0.3f, -0.5f); // Set origin label position.
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0'); // Draw '0'.
} // End function.

bool isPointInsidePolygon(float x, float y, float vertices[6][2], int n) // Check if inside.
{ // Begin function.
    bool inside = false; // Initialize inside flag.
    for (int i = 0, j = n-1; i < n; j = i++) { // Loop through vertices.
        float xi = vertices[i][0], yi = vertices[i][1]; // Current vertex.
        float xj = vertices[j][0], yj = vertices[j][1]; // Previous vertex.
        
        if (((yi > y) != (yj > y)) && // Check y-crossing.
            (x < (xj - xi) * (y - yi) / (yj - yi) + xi)) { // Check x-position.
            inside = !inside; // Flip inside flag.
        } // End if.
    } // End for loop.
    return inside; // Return inside flag.
} // End function.

void generateInteriorPoints() // Generate interior points.
{ // Begin function.
    interiorPoints.clear(); // Clear points vector.
    
    float minX = polygonVertices[0][0], maxX = polygonVertices[0][0]; // Initialize min/max X.
    float minY = polygonVertices[0][1], maxY = polygonVertices[0][1]; // Initialize min/max Y.
    
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        if (polygonVertices[i][0] < minX) minX = polygonVertices[i][0]; // Update min X.
        if (polygonVertices[i][0] > maxX) maxX = polygonVertices[i][0]; // Update max X.
        if (polygonVertices[i][1] < minY) minY = polygonVertices[i][1]; // Update min Y.
        if (polygonVertices[i][1] > maxY) maxY = polygonVertices[i][1]; // Update max Y.
    } // End for loop.
    
    float step = 0.6f; // Set step size.
    
    for (float x = minX + 0.3f; x <= maxX; x += step) { // Loop through x-range.
        for (float y = minY + 0.3f; y <= maxY; y += step) { // Loop through y-range.
            if (isPointInsidePolygon(x, y, polygonVertices, NUM_VERTICES)) { // If point is inside.
                interiorPoints.push_back({x, y}); // Add to vector.
            } // End if.
        } // End for loop.
    } // End for loop.
} // End function.

void drawPolygonOutline() // Draw polygon outline.
{ // Begin function.
    glColor3f(0.0f, 0.0f, 0.0f);  // Set outline color.
    glLineWidth(2.5f); // Set line width.
    
    glBegin(GL_LINE_LOOP); // Begin drawing loop.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(polygonVertices[i][0], polygonVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing loop.
} // End function.

void drawVertices() // Draw vertices function.
{ // Begin function.
    glColor3f(0.0f, 0.0f, 1.0f);  // Set vertex color.
    glPointSize(6.0f); // Set point size.
    glBegin(GL_POINTS); // Begin drawing points.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(polygonVertices[i][0], polygonVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing points.
    
    glColor3f(0.0f, 0.0f, 0.0f);  // Set label color.
    char label[20]; // Label character array.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        sprintf(label, "V%d(%.0f,%.0f)", i+1, // Format label string.
                polygonVertices[i][0], polygonVertices[i][1]); // With vertex info.
        glRasterPos2f(polygonVertices[i][0] + 0.3f, polygonVertices[i][1] + 0.3f); // Set label position.
        for (char* c = label; *c != '\0'; c++) { // Loop through label.
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c); // Draw character.
        } // End for loop.
    } // End for loop.
} // End function.

void drawAsteriskFill() // Draw asterisk fill.
{ // Begin function.
    for (const auto& p : interiorPoints) { // Loop through interior points.
        float shade = 0.5f + (p.first - 2.0f) / 20.0f; // Calculate shade.
        if (shade > 1.0f) shade = 1.0f; // Clamp shade value.
        if (shade < 0.4f) shade = 0.4f; // Clamp shade value.
        
        glColor3f(shade, 0.0f, 0.0f); // Set asterisk color.
        
        glRasterPos2f(p.first, p.second); // Set asterisk position.
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '*'); // Draw asterisk.
    } // End for loop.
} // End function.

void display() // Display function.
{ // Begin function.
    glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer.
    
    drawAxes(); // Call draw axes.
    
    generateInteriorPoints(); // Call generate points.
    
    drawAsteriskFill(); // Call draw fill.
    
    drawPolygonOutline(); // Call draw outline.
    
    drawVertices(); // Call draw vertices.
    
    glColor3f(0.0f, 0.0f, 0.0f); // Set text color.
    glRasterPos2f(LEFT + 0.3f, TOP - 0.8f); // Set text position.
    glFlush(); // Flush the buffer.
    glutSwapBuffers(); // Swap the buffers.
} // End function.

void reshape(int w, int h) // Reshape function.
{ // Begin function.
    glViewport(0, 0, w, h); // Set viewport.
    glMatrixMode(GL_PROJECTION); // Set projection matrix.
    glLoadIdentity(); // Load identity matrix.
    gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP); // Set orthographic view.
    glMatrixMode(GL_MODELVIEW); // Set modelview matrix.
} // End function.

void init() // Initialize function.
{ // Begin function.
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Set clear color.
    glMatrixMode(GL_PROJECTION); // Set projection matrix.
    glLoadIdentity(); // Load identity matrix.
    gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP); // Set orthographic view.
    glMatrixMode(GL_MODELVIEW); // Set modelview matrix.
} // End function.

void keyboard(unsigned char key, int x, int y) // Keyboard callback.
{ // Begin function.
    if (key == 27) exit(0); // Exit on ESC.
} // End function.

int main(int argc, char** argv) // Main program function.
{ // Begin function.
    glutInit(&argc, argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode.
    glutInitWindowSize(900, 750); // Set window size.
    glutInitWindowPosition(100, 100); // Set window position.
    glutCreateWindow(" Polygon "); // Create the window.
    
    init(); // Call initialize function.
    glutDisplayFunc(display); // Set display function.
    glutReshapeFunc(reshape); // Set reshape function.
    glutKeyboardFunc(keyboard); // Set keyboard function.
    
    
    glutMainLoop(); // Enter main loop.
    
    return 0; // Exit program.
} // End function.