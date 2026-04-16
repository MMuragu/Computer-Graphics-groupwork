import math
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

# Global variables
origin = Point(100, 100)
scale = 10
color_idx = 0

# Colors for different sub-polygons
colors = [
        [1.0, 1.0, 1.0],
        [1.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 0.0, 1.0],
        [1.0, 1.0, 0.0],
        [1.0, 0.0, 1.0],
        [0.0, 1.0, 1.0]
        ]

def pscale(p):
    """Scale cartesian coordinates to screen-space coordinates"""
    return Point(origin.x + p.x * scale, origin.y + p.y * scale)

# Scaled polygon points
plot_pts = [
        pscale(Point(6, 1)),
        pscale(Point(6, 3)),
        pscale(Point(4, 3)),
        pscale(Point(4, 2)),
        pscale(Point(3, 2)),
        pscale(Point(3, 4)),
        pscale(Point(1, 3)),
        pscale(Point(1, 1))
        ]

def init():
    """Initialize OpenGL settings"""
    # Black background
    glClearColor(0.0, 0.0, 0.0, 1.0)

    # Set coordinate system
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    # Starting and ending position for the x and y axes of the canvas
    gluOrtho2D(0, 200, 0, 200)

def draw_text(x, y, text):
    """Draw text for labels"""
    glRasterPos2f(x, y)
    for c in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, ord(c))

def draw_axes():
    """Draw coordinate axes with tick marks and labels"""
    # White lines for axes
    glColor3f(1.0, 1.0, 1.0)
    glLineWidth(1.0)

    # Draw x-axis
    glBegin(GL_LINES)
    glVertex2f(0, origin.y)
    glVertex2f(200, origin.y)
    glEnd()

    # Draw y-axis
    glBegin(GL_LINES)
    glVertex2f(origin.x, 0)
    glVertex2f(origin.x, 200)
    glEnd()

    # Axis labels
    draw_text(195, 103, "X")
    draw_text(103, 195, "Y")

    # Tick marks and labels
    for i in range(-10, 11):
        if i == 0:
            continue

        screen_x = origin.x + i * scale
        screen_y = origin.y + i * scale

        # X axis ticks
        glBegin(GL_LINES)
        glVertex2f(screen_x, 98)
        glVertex2f(screen_x, 102)
        glEnd()

        # Y axis ticks
        glBegin(GL_LINES)
        glVertex2f(98, screen_y)
        glVertex2f(102, screen_y)
        glEnd()

        # X axis labels
        draw_text(screen_x - 3, 91, str(i))

        # Y axis labels
        draw_text(85, screen_y - 3, str(i))

    # Origin label
    draw_text(91, 91, "0")

def draw_polygon(pts, color):
    """Draw a filled polygon with an outline"""
    # Fill the polygon with the specified color
    glColor3fv(color)
    glBegin(GL_POLYGON)
    for p in pts:
        glVertex2f(p.x, p.y)
    glEnd()

    # Draw black outline
    glColor3f(0.0, 0.0, 0.0)
    glLineWidth(2.0)
    glBegin(GL_LINE_LOOP)
    for p in pts:
        glVertex2f(p.x, p.y)
    glEnd()

def crossZ(prev, current, next_pt):
    """
    Calculate Z component of cross product between two edges.
    Returns:
        + : convex point (< 180 degrees)
        - : concave/reflex point (> 180 degrees)
        0 : straight line
    """
    edgeA_x = current.x - prev.x
    edgeA_y = current.y - prev.y
    edgeB_x = next_pt.x - current.x
    edgeB_y = next_pt.y - current.y

    return (edgeA_x * edgeB_y) - (edgeA_y * edgeB_x)

def find_line_intersect_pt(reflex, ray_end, p1, p2):
    """
    Find intersection between ray from reflex to ray_end and line segment p1-p2.
    Uses parametric line operations.
    """
    # Find deltas
    d1x = reflex.x - ray_end.x
    d1y = reflex.y - ray_end.y
    d2x = p2.x - p1.x
    d2y = p2.y - p1.y

    denom = (d1x * d2y) - (d1y * d2x)

    # If denominator is almost zero, lines are parallel
    if abs(denom) < 1e-6:
        return Point(float('inf'), float('inf'))

    # Find intersection point
    t = ((p1.x - reflex.x) * d2y - (p1.y - reflex.y) * d2x) / denom
    intersection = Point(reflex.x + t * d1x, reflex.y + t * d1y)

    return intersection

def rotational_poly_check(pts):
    """
    Check if polygon is concave and split it into convex sub-polygons.
    Recursively processes the polygon until all parts are convex.
    """
    global color_idx

    reflex_idx = -1
    n = len(pts)

    # Look for a reflex (concave) point
    for i in range(n):
        prev = pts[(i - 1 + n) % n]
        curr = pts[i]
        next_pt = pts[(i + 1) % n]

        cross_product = crossZ(prev, curr, next_pt)
        if cross_product < 0:
            reflex_idx = i
            break

    # If no reflex point found, polygon is convex
    if reflex_idx == -1:
        draw_polygon(pts, colors[color_idx % len(colors)])
        color_idx += 1
        return

    # Extend edge from previous point through reflex point
    reflex = pts[reflex_idx]
    b4_reflex = pts[(reflex_idx - 1 + n) % n]
    dx = reflex.x - b4_reflex.x
    dy = reflex.y - b4_reflex.y

    # Find magnitude and create extended ray
    length = math.sqrt(dx * dx + dy * dy)
    ray_end = Point(
            reflex.x + (dx / length) * 2000.0,
            reflex.y + (dy / length) * 2000.0
            )

    # Find intersection with other edges
    split_pt = None
    split_edge = -1

    for i in range(n):
        # Skip the reflex point and its previous edge
        if i == reflex_idx or i == (reflex_idx - 1 + n) % n:
            continue

        p1 = pts[i]
        p2 = pts[(i + 1) % n]
        intersect = find_line_intersect_pt(reflex, ray_end, p1, p2)

        if intersect.x != float('inf') and intersect.y != float('inf'):
            d2x = p2.x - p1.x
            d2y = p2.y - p1.y

            # Check if intersection is on the edge segment
            if abs(d2x) > abs(d2y):
                t_edge = (intersect.x - p1.x) / d2x
            else:
                t_edge = (intersect.y - p1.y) / d2y

            # Intersection must be on the actual edge
            if t_edge < 0.0 or t_edge > 1.0:
                continue

            # Check if intersection is in the correct direction
            dot = (intersect.x - reflex.x) * dx + (intersect.y - reflex.y) * dy
            if dot > 1e-4:
                split_edge = i
                split_pt = intersect
                break

    # If no valid split found, draw as-is
    if split_edge == -1:
        draw_polygon(pts, colors[color_idx % len(colors)])
        color_idx += 1
        return

    # Create two sub-polygons
    subA = []
    subB = []

    # Sub-polygon A: From reflexIdx to splitPt
    curr = reflex_idx
    while curr != (split_edge + 1) % n:
        subA.append(pts[curr])
        curr = (curr + 1) % n
    subA.append(split_pt)

    # Sub-polygon B: From splitPt back to reflexIdx
    subB.append(split_pt)
    curr = (split_edge + 1) % n
    while curr != reflex_idx:
        subB.append(pts[curr])
        curr = (curr + 1) % n

    # Recursively check each sub-polygon
    rotational_poly_check(subA)
    rotational_poly_check(subB)

def display():
    """Main display callback function"""
    glClear(GL_COLOR_BUFFER_BIT)
    draw_axes()

    # Draw polygon by checking for concave parts and splitting
    rotational_poly_check(plot_pts)

    glFlush()

def main():
    """Main function to initialize and run OpenGL"""
    # Initialize GLUT
    glutInit()
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutInitWindowSize(800, 600)
    glutCreateWindow(b"Concave Polygon Drawing")

    # Initialize OpenGL settings
    init()

    # Set display callback
    glutDisplayFunc(display)

    # Start main loop
    glutMainLoop()

if __name__ == "__main__":
    main()
