from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import sys

# Polygon vertices array.
polygonVertices = [
    [8, 4],   # Vertex 1
    [2, 4],   # Vertex 2
    [0, 8],   # Vertex 3
    [3, 12],  # Vertex 4
    [7, 12],  # Vertex 5
    [10, 8]   # Vertex 6
]

# Number of vertices
NUM_VERTICES = 6

# View boundaries constants
LEFT = -2.0
RIGHT = 15.0
BOTTOM = -2.0
TOP = 15.0

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
    
    for i in range(-2, 16):
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

def drawFilledPolygonRed():
    """a): Function to fill the polygon in Red (#FF0000)"""
    glColor3f(1.0, 0.0, 0.0)  # Set color to Red
    
    glBegin(GL_POLYGON)  # Begin drawing filled polygon using GL_POLYGON
    for i in range(NUM_VERTICES):
        glVertex2f(polygonVertices[i][0], polygonVertices[i][1])
    glEnd()

def drawPolygonOutline():
    """Draw polygon outline"""
    glColor3f(0.0, 0.0, 0.0)  # Set outline color
    glLineWidth(2.5)
    
    glBegin(GL_LINE_LOOP)
    for i in range(NUM_VERTICES):
        glVertex2f(polygonVertices[i][0], polygonVertices[i][1])
    glEnd()

def drawVertices():
    """Draw vertices function"""
    glColor3f(0.0, 0.0, 1.0)  # Set vertex color
    glPointSize(6.0)
    glBegin(GL_POINTS)
    for i in range(NUM_VERTICES):
        glVertex2f(polygonVertices[i][0], polygonVertices[i][1])
    glEnd()
    
    glColor3f(0.0, 0.0, 0.0)  # Set label color
    for i in range(NUM_VERTICES):
        label = "V%d(%.0f,%.0f)" % (i+1, polygonVertices[i][0], polygonVertices[i][1])
        glRasterPos2f(polygonVertices[i][0] + 0.3, polygonVertices[i][1] + 0.3)
        for ch in label:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(ch))

def display():
    """Display function"""
    glClear(GL_COLOR_BUFFER_BIT)
    
    drawAxes()
    drawFilledPolygonRed()  
    drawPolygonOutline()
    drawVertices()
    
    glColor3f(0.0, 0.0, 0.0)  # Set text color
    glRasterPos2f(LEFT + 0.3, TOP - 0.8)   
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
    glutCreateWindow(b"a): Red Filled Polygon")  
    
    init()
    glutDisplayFunc(display)
    glutReshapeFunc(reshape)
    glutKeyboardFunc(keyboard)
    
    glutMainLoop()

if __name__ == "__main__":
    main()