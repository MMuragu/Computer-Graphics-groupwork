#include <GL/glut.h> // OpenGL graphics.
#include <cmath> //math functions.
#include <cstdio> //C-style I/O.
#include <iostream> //input/output stream.
using namespace std; // Use standard namespace.

// Original polygon vertices array.
float originalVertices[6][2] = { // Define original polygon vertices.
    {8, 4},   // Vertex 1.
    {2, 4},   // Vertex 2.
    {0, 8},   // Vertex 3.
    {3, 12},  // Vertex 4.
    {7, 12},  // Vertex 5.
    {10, 8}   // Vertex 6.
}; // End of array.

// Scaled polygon vertices array.
float scaledVertices[6][2]; // Array for scaled vertices.

// Number of vertices.
const int NUM_VERTICES = 6; // Set vertex count.

// View boundaries constants (adjusted for scaled polygon).
const float LEFT = -5.0f; // Left boundary (expanded for scaled polygon).
const float RIGHT = 25.0f; // Right boundary (expanded for scaled polygon).
const float BOTTOM = -5.0f; // Bottom boundary (expanded for scaled polygon).
const float TOP = 25.0f; // Top boundary (expanded for scaled polygon).

// PART (b): Function to scale polygon by factor of 2
void scalePolygonByFactor2() // Scale polygon function.
{ // Begin function.
    // Scale factor
    float scaleFactor = 2.0f; // Scaling factor of 2.
    
    // Calculate centroid of the polygon for scaling about center
    float centerX = 0.0f, centerY = 0.0f; // Initialize centroid coordinates.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        centerX += originalVertices[i][0]; // Sum X coordinates.
        centerY += originalVertices[i][1]; // Sum Y coordinates.
    } // End for loop.
    centerX /= NUM_VERTICES; // Calculate centroid X.
    centerY /= NUM_VERTICES; // Calculate centroid Y.
    
    // Scale each vertex about the centroid
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        // Translate to origin, scale, translate back
        scaledVertices[i][0] = centerX + (originalVertices[i][0] - centerX) * scaleFactor; // Scale X.
        scaledVertices[i][1] = centerY + (originalVertices[i][1] - centerY) * scaleFactor; // Scale Y.
    } // End for loop.
} // End function.

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
    
    for (int i = -5; i <= 25; i++) { // Loop for ticks (expanded range).
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

void drawOriginalPolygon() // Draw original polygon outline.
{ // Begin function.
    glColor3f(0.0f, 0.0f, 1.0f);  // Set color to BLUE for original polygon.
    glLineWidth(2.5f); // Set line width.
    
    glBegin(GL_LINE_LOOP); // Begin drawing loop.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(originalVertices[i][0], originalVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing loop.
} // End function.

void drawScaledPolygon() // Draw scaled polygon outline only.
{ // Begin function.
    glColor3f(1.0f, 0.0f, 0.0f);  // Set color to RED for scaled polygon.
    glLineWidth(3.0f); // Set line width.
    
    glBegin(GL_LINE_LOOP); // Begin drawing loop (OUTLINE ONLY - no fill).
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(scaledVertices[i][0], scaledVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing loop.
} // End function.

void drawVertices() // Draw vertices for both polygons.
{ // Begin function.
    // Draw original polygon vertices (BLUE)
    glColor3f(0.0f, 0.0f, 0.8f);  // Set vertex color to dark blue.
    glPointSize(6.0f); // Set point size.
    glBegin(GL_POINTS); // Begin drawing points.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(originalVertices[i][0], originalVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing points.
    
    // Draw scaled polygon vertices (RED)
    glColor3f(0.8f, 0.0f, 0.0f);  // Set vertex color to dark red.
    glPointSize(6.0f); // Set point size.
    glBegin(GL_POINTS); // Begin drawing points.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        glVertex2f(scaledVertices[i][0], scaledVertices[i][1]); // Set vertex.
    } // End for loop.
    glEnd(); // End drawing points.
    
    // Label original vertices
    glColor3f(0.0f, 0.0f, 0.8f);  // Set label color to dark blue.
    char label[25]; // Label character array.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        sprintf(label, "O%d(%.0f,%.0f)", i+1, // Format label string for original.
                originalVertices[i][0], originalVertices[i][1]); // With vertex info.
        glRasterPos2f(originalVertices[i][0] + 0.3f, originalVertices[i][1] + 0.3f); // Set label position.
        for (char* c = label; *c != '\0'; c++) { // Loop through label.
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c); // Draw character.
        } // End for loop.
    } // End for loop.
    
    // Label scaled vertices
    glColor3f(0.8f, 0.0f, 0.0f);  // Set label color to dark red.
    for (int i = 0; i < NUM_VERTICES; i++) { // Loop through vertices.
        sprintf(label, "S%d(%.0f,%.0f)", i+1, // Format label string for scaled.
                scaledVertices[i][0], scaledVertices[i][1]); // With vertex info.
        glRasterPos2f(scaledVertices[i][0] + 0.5f, scaledVertices[i][1] + 0.5f); // Set label position.
        for (char* c = label; *c != '\0'; c++) { // Loop through label.
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c); // Draw character.
        } // End for loop.
    } // End for loop.
} // End function.

void display() // Display function.
{ // Begin function.
    glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer.
    
    scalePolygonByFactor2(); // PART (b): Scale the polygon.
    
    drawAxes(); // Call draw axes.
    
    drawOriginalPolygon(); // Draw original polygon (BLUE outline).
    drawScaledPolygon(); // Draw scaled polygon (RED outline - EMPTY inside).
    drawVertices(); // Draw vertices for both polygons.
    
    // Add legend/title
    glColor3f(0.0f, 0.0f, 0.0f); // Set text color to black.
    glRasterPos2f(LEFT + 0.5f, TOP - 0.8f); // Set text position.      
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
    glLoadIdentity(); // Set projection matrix.
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
    glutCreateWindow("b): Scaled Polygon by Factor of 2"); // Create the window.
    
    init(); // Call initialize function.
    glutDisplayFunc(display); // Set display function.
    glutReshapeFunc(reshape); // Set reshape function.
    glutKeyboardFunc(keyboard); // Set keyboard function.
    
    glutMainLoop(); // Enter main loop.
    
    return 0; // Exit program.
} // End function.