from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import sys

# Original polygon vertices array
originalVertices = [
    [8, 4],   # Vertex 1
    [2, 4],   # Vertex 2
    [0, 8],   # Vertex 3
    [3, 12],  # Vertex 4
    [7, 12],  # Vertex 5
    [10, 8]   # Vertex 6
]

# Scaled polygon vertices array
scaledVertices = [[0, 0] for _ in range(6)]

# Number of vertices
NUM_VERTICES = 6

# View boundaries constants (adjusted for scaled polygon)
LEFT = -5.0
RIGHT = 25.0
BOTTOM = -5.0
TOP = 25.0

def scalePolygonByFactor2():
    """b): Function to scale polygon by factor of 2"""
    scaleFactor = 2.0  # Scaling factor of 2
    
    # Calculate centroid of the polygon for scaling about center
    centerX = 0.0
    centerY = 0.0
    for i in range(NUM_VERTICES):
        centerX += originalVertices[i][0]
        centerY += originalVertices[i][1]
    centerX /= NUM_VERTICES
    centerY /= NUM_VERTICES
    
    # Scale each vertex about the centroid
    for i in range(NUM_VERTICES):
        scaledVertices[i][0] = centerX + (originalVertices[i][0] - centerX) * scaleFactor
        scaledVertices[i][1] = centerY + (originalVertices[i][1] - centerY) * scaleFactor

def drawAxes():
    """Draw axes function"""
    glColor3f(0.4, 0.4, 0.4)  # Set axis color
    glLineWidth(1.5)
    
    glBegin(GL_LINES)
    glVertex2f(LEFT, 0)
    glVertex2f(RIGHT, 0)  # Draw X-axis
    glVertex2f(0, BOTTOM)
    glVertex2f(0, TOP)    # Draw Y-axis
    glEnd()
    
    glColor3f(0.3, 0.3, 0.3)  # Set tick color
    glLineWidth(1.0)
    
    for i in range(-5, 26):
        if i == 0:
            continue
        
        glBegin(GL_LINES)
        glVertex2f(float(i), -0.2)
        glVertex2f(float(i), 0.2)
        glVertex2f(-0.2, float(i))
        glVertex2f(0.2, float(i))
        glEnd()
        
        if i % 2 == 0 or abs(i) == 1:
            buf = str(i)
            
            glRasterPos2f(float(i) - 0.2, -0.7)
            for ch in buf:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(ch))
            
            glRasterPos2f(0.25, float(i) - 0.15)
            for ch in buf:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(ch))
    
    glColor3f(0.0, 0.0, 0.0)  # Set label color
    glRasterPos2f(RIGHT - 0.6, -0.6)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord('X'))
    
    glRasterPos2f(0.3, TOP - 0.5)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord('Y'))
    
    glRasterPos2f(0.3, -0.5)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord('0'))

def drawOriginalPolygon():
    """Draw original polygon outline"""
    glColor3f(0.0, 0.0, 1.0)  # Set color to BLUE for original polygon
    glLineWidth(2.5)
    
    glBegin(GL_LINE_LOOP)
    for i in range(NUM_VERTICES):
        glVertex2f(originalVertices[i][0], originalVertices[i][1])
    glEnd()

def drawScaledPolygon():
    """Draw scaled polygon outline only"""
    glColor3f(1.0, 0.0, 0.0)  # Set color to RED for scaled polygon
    glLineWidth(3.0)
    
    glBegin(GL_LINE_LOOP)  # OUTLINE ONLY - no fill
    for i in range(NUM_VERTICES):
        glVertex2f(scaledVertices[i][0], scaledVertices[i][1])
    glEnd()

def drawVertices():
    """Draw vertices for both polygons"""
    # Draw original polygon vertices (BLUE)
    glColor3f(0.0, 0.0, 0.8)  # Set vertex color to dark blue
    glPointSize(6.0)
    glBegin(GL_POINTS)
    for i in range(NUM_VERTICES):
        glVertex2f(originalVertices[i][0], originalVertices[i][1])
    glEnd()
    
    # Draw scaled polygon vertices (RED)
    glColor3f(0.8, 0.0, 0.0)  # Set vertex color to dark red
    glPointSize(6.0)
    glBegin(GL_POINTS)
    for i in range(NUM_VERTICES):
        glVertex2f(scaledVertices[i][0], scaledVertices[i][1])
    glEnd()
    
    # Label original vertices
    glColor3f(0.0, 0.0, 0.8)  # Set label color to dark blue
    for i in range(NUM_VERTICES):
        label = "O%d(%.0f,%.0f)" % (i+1, originalVertices[i][0], originalVertices[i][1])
        glRasterPos2f(originalVertices[i][0] + 0.3, originalVertices[i][1] + 0.3)
        for ch in label:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(ch))
    
    # Label scaled vertices
    glColor3f(0.8, 0.0, 0.0)  # Set label color to dark red
    for i in range(NUM_VERTICES):
        label = "S%d(%.0f,%.0f)" % (i+1, scaledVertices[i][0], scaledVertices[i][1])
        glRasterPos2f(scaledVertices[i][0] + 0.5, scaledVertices[i][1] + 0.5)
        for ch in label:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(ch))

def display():
    """Display function"""
    glClear(GL_COLOR_BUFFER_BIT)
    
    scalePolygonByFactor2()  
    
    drawAxes()
    drawOriginalPolygon()  # Draw original polygon (BLUE outline)
    drawScaledPolygon()    # Draw scaled polygon (RED outline - EMPTY inside)
    drawVertices()         # Draw vertices for both polygons
    
    # Add legend/title
    glColor3f(0.0, 0.0, 0.0)  # Set text color to black
    glRasterPos2f(LEFT + 0.5, TOP - 0.8)   
    glFlush()
    glutSwapBuffers()

def reshape(w, h):
    """Reshape function"""
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP)
    glMatrixMode(GL_MODELVIEW)

def init():
    """Initialize function"""
    glClearColor(1.0, 1.0, 1.0, 1.0)  # Set clear color
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP)
    glMatrixMode(GL_MODELVIEW)

def keyboard(key, x, y):
    """Keyboard callback"""
    if key == b'\x1b':  # ESC key
        sys.exit(0)

def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB)
    glutInitWindowSize(900, 750)
    glutInitWindowPosition(100, 100)
    glutCreateWindow(b"b): Scaled Polygon by Factor of 2")  
    
    init()
    glutDisplayFunc(display)
    glutReshapeFunc(reshape)
    glutKeyboardFunc(keyboard)
    
    glutMainLoop()

if __name__ == "__main__":
    main()