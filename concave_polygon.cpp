#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>
#include <cmath>
#include <array>

struct Point { float x, y; };
using Points = std::vector<Point>;

inline float sqr (float x) { return x*x;}

// polygon points
// origin point and scale for the screen space system.
Point origin {100, 100};
int scale = 10;

// point scaling function
// scales cartesian coordinates to screen-space coordinates
inline Point pscale (Point p)
{
  p.x = origin.x + p.x * scale;
  p.y = origin.y + p.y * scale;
  return p;
}

// scaled polygon points
Points plot_pts {
  pscale ({6, 1}),
  pscale ({6, 3}),
  pscale ({4, 3}),
  pscale ({4, 2}),
  pscale ({3, 2}),
  pscale ({3, 4}),
  pscale ({1, 3}),
  pscale ({1, 1})
};

// an array of colors that will be used to 
// differentiate between the sub convex polygons
using COL = std::array<float, 3>;
std::vector<COL> colors {
  {1.0f, 1.0f, 1.0f},
  {1.0f, 0.0f, 0.0f},
  {0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 1.0f},
  {1.0f, 1.0f, 0.0f},
  {1.0f, 0.0f, 1.0f},
  {0.0f, 1.0f, 1.0f}
};
int color_idx = 0;

// forward declarations
void init ();
void display ();
void draw_polygon (Points pts, float color[3]);
void rotational_poly_check (Points pts);

int main(int argc, char* argv[])
{
  // initialize and setup opengl canvas
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Midpoint Cirle Drawing");

  glewInit();
  // initializes the background, the coordinate system
  init();
  // draws the points and line using a callback fn 'display'
  glutDisplayFunc(display);
  // starts the main drawing loop
  glutMainLoop();

  return 0;
}

void init()
{
  // black background
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Set coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //starting and ending position for the x and y axes of the canvas
  gluOrtho2D(0, 200, 0, 200);
}

void draw_text(float x, float y, std::string text)
{
  // draw text for the labels of each tick of the axes
  glRasterPos2f(x, y);
  for (char c : text)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
}

void draw_axes()
{
  // white lines for axes with line width of 1
  glColor3f(1.0f, 1.0f, 1.0f); // white
  glLineWidth(1.0f);

  // draw the x-axis
  glBegin(GL_LINES);
  glVertex2f(0, origin.y);
  glVertex2f(200, origin.y);
  glEnd();

  // draw the y-axis
  glBegin(GL_LINES);
  glVertex2f(origin.x, 0);
  glVertex2f(origin.x, 200);
  glEnd();

  // axis labels
  draw_text(195, 103, "X");
  draw_text(103, 195, "Y");

  // tick marks and labels
  for (int i = -10; i <= 10; i++)
  {
    if (i == 0) continue; // skip origin

    float screen_x = origin.x + i * scale;
    float screen_y = origin.y + i * scale;

    // x axis ticks
    glBegin(GL_LINES);
    glVertex2f(screen_x, 98);
    glVertex2f(screen_x, 102);
    glEnd();

    // y axis ticks
    glBegin(GL_LINES);
    glVertex2f(98, screen_y);
    glVertex2f(102, screen_y);
    glEnd();

    // x axis labels
    draw_text(screen_x - 3, 91, std::to_string(i));

    // y axis labels
    draw_text(85, screen_y - 3, std::to_string(i));
  }

  // origin label
  draw_text(91, 91, "0");
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  draw_axes();

  // draw polygon by checking for concave polygons and dividing them
  // into smaller convex ones
  rotational_poly_check (plot_pts);

  glFlush();
}

void draw_polygon (Points pts, COL color)
{
  // pick a color from the color array which will be used to fill the polygon
  glColor3fv(color.data());
  // plot the polygon from the points passed into the function
  glBegin(GL_POLYGON);
  for (Point& p : pts) glVertex2f(p.x, p.y);
  glEnd();

  // draw a black outline of the polygons to show the separations.
  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINE_LOOP);
  for (Point& p : pts) glVertex2f(p.x, p.y);
  glEnd();
}

float crossZ (Point prev, Point current, Point next)
{
  /* edge A = curr - prev (curr.{x, y} - prev.{x,y})
   edge B = next - curr (next.{x, y} - curr.{x,y})
   Z component = (edgeA.x * edgeB.y) - (edgeA.y * edgeB.x)

   if ZComponent is:
    + -> convex point (< 180 degrees, normal)
    - -> concave/reflex point (> 180 degrees)
    0 -> straight line
  */
  Point edgeA = {.x = current.x - prev.x, .y = current.y - prev.y};
  Point edgeB = {.x = next.x - current.x, .y = next.y - current.y};
  return (edgeA.x * edgeB.y) - (edgeA.y * edgeB.x);
}

Point find_line_intersect_pt (
    Point reflex,
    Point ray_end,
    Point p1,
    Point p2
)
{
  /* find if there's an intersection between the ray/line extended from
   * the reflex point to ray_end point and points p1 and p2
   * uses parametric line operations
   * */

  // find deltas between reflex and ray_end
  // and between p1 and p2
  float d1x = reflex.x - ray_end.x;
  float d1y = reflex.y - ray_end.y;
  float d2x = p2.x - p1.x;
  float d2y = p2.y - p1.y;

  float denom = (d1x * d2y) - (d1y * d2x);
  // if the denominator is so small, almost zero, return an invalid point
  if (std::fabsf(denom) < 1e-6f) return {INFINITY, INFINITY};

  // find the intersection point and return it
  float t = ((p1.x - reflex.x) * d2y - (p1.y - reflex.y) * d2x) / denom;
  Point intersection_point {.x = reflex.x + t * d1x, .y = reflex.y + t * d1y};

  return intersection_point;
}

void rotational_poly_check (Points pts)
{
  // index for the reflex point(point where angle > 180
  size_t reflexIdx = -1;
  size_t n = pts.size();

  // look for a concave/reflex point in the pts vector
  for (size_t i = 0; i < n; i++)
  {
    Point prev = pts[(i - 1 + n) % n];
    Point curr = pts[i];
    Point next = pts[(i + 1) % n];

    float cross_product = crossZ(prev, curr, next);
    if (cross_product < 0)
    {
      // we have found a reflex point at index i
      reflexIdx = i;
      break;
    }
  }
  if (reflexIdx == (size_t) -1)
  {
    // just draw the polygon since it is already convex
    draw_polygon(pts, colors[color_idx++ % (colors.size() - 1)]);
    return;
  }

  // since we found a reflex point in the points,
  // we extend its edge, from the previous point to the current point, the current
  // point being the reflex point.
  //
  Point reflex = pts[reflexIdx];
  Point b4_reflex = pts[(reflexIdx - 1 + n) % n];
  float dx = reflex.x - b4_reflex.x;
  float dy = reflex.y - b4_reflex.y;
  // find magnitude of the two points, reflex and b4_reflex
  float len = sqrtf(sqr(dx) + sqr(dy));
  Point ray_end {
    .x = reflex.x + (dx/len)*2000.0f,
    .y = reflex.y + (dy/len)*2000.0f,
  };

  // find intersection between the extended ray from edge b4_reflex----->reflex
  // and any other two points in the polygon.
  //
  Point splitPt {};
  int splitEdge {-1};

  for (size_t i = 0; i < n; i++)
  {
    if (i == reflexIdx || i == (reflexIdx - 1 + n) % n) continue;

    Point p1 = pts[i];
    Point p2 = pts[(i + 1) % n];
    Point intersect = find_line_intersect_pt (reflex, ray_end, p1, p2);
    if (intersect.x != INFINITY && intersect.y != INFINITY)
    {
      float d2x = p2.x - p1.x;
      float d2y = p2.y - p1.y;
      float t_edge;

      // make sure the point lies on the segnem of p1-p2 and not outside it.
      if (std::fabsf(d2x) > std::fabsf(d2y))
        t_edge = (intersect.x - p1.x) / d2x;
      else
        t_edge = (intersect.y - p1.y) / d2y;

      // Intersection is not on the actual edge
      if (t_edge < 0.0f || t_edge > 1.0f) continue; 

      float dot = (intersect.x - reflex.x) * dx + (intersect.y - reflex.y) * dy;
      if (dot > 1e-4f)
      {
        splitEdge = i;
        splitPt = intersect;
        break;
      }
    }
  }
  if (splitEdge ==  -1)
  {
    // couldn't split the polygon, draw as-is
    draw_polygon(pts, colors[color_idx++ % (colors.size() - 1)]);
    return;
  }

  Points subA {}, subB {};
  size_t curr = reflexIdx;
  // Sub-polygon A: From reflexIdx, to splitPt
  while (curr != (splitEdge + 1) % n)
  {
    subA.push_back(pts[curr]);
    curr = (curr + 1) % n;
  }
  subA.push_back(splitPt);

  // Sub-polygon B: From splitPt, then splitEdge+1 back to reflexIdx
  subB.push_back(splitPt);
  curr = (splitEdge + 1) % n;
  while (curr != reflexIdx)
  {
    subB.push_back(pts[curr]);
    curr = (curr + 1) % n;
  }

  // recurse upon each of the sub-polygons to find it those have any
  // reflex points
  rotational_poly_check(subA);
  rotational_poly_check(subB);
}
