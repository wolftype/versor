// Interactive Cone Builder using Circle Skinning
// Demonstrates Shape::Skin() with GUI controls and Frame positioning

#include "vsr/vsr_app.h"
#include "vsr/form/vsr_shapes.h"

using namespace vsr::cga;
using namespace gfx;

// Helper to compute and draw bounding box
struct BoundingBox {
  Vec3f min, max;
  vector<Vec3f> coords;  // Normalized coordinates (0-1) for each mesh vertex

  static BoundingBox fromMesh(const Mesh& mesh) {
    if (mesh.num() == 0) return {Vec3f(0,0,0), Vec3f(0,0,0), {}};

    Vec3f min = mesh[0].Pos;
    Vec3f max = mesh[0].Pos;

    for (int i = 1; i < mesh.num(); ++i) {
      const Vec3f& p = mesh[i].Pos;
      min[0] = std::min(min[0], p[0]);
      min[1] = std::min(min[1], p[1]);
      min[2] = std::min(min[2], p[2]);
      max[0] = std::max(max[0], p[0]);
      max[1] = std::max(max[1], p[1]);
      max[2] = std::max(max[2], p[2]);
    }

    BoundingBox bbox = {min, max, {}};
    bbox.calculateNormalizedCoords(mesh);
    return bbox;
  }

  // Calculate normalized (0-1) coordinates for each vertex within bounding box
  void calculateNormalizedCoords(const Mesh& mesh) {
    coords.clear();
    coords.reserve(mesh.num());

    Vec3f extents = max - min;

    // Handle zero-extent cases (flat in one or more dimensions)
    Vec3f safeExtents(
      extents[0] > 0.0001 ? extents[0] : 1.0,
      extents[1] > 0.0001 ? extents[1] : 1.0,
      extents[2] > 0.0001 ? extents[2] : 1.0
    );

    for (int i = 0; i < mesh.num(); ++i) {
      const Vec3f& p = mesh[i].Pos;
      Vec3f normalized(
        (p[0] - min[0]) / safeExtents[0],
        (p[1] - min[1]) / safeExtents[1],
        (p[2] - min[2]) / safeExtents[2]
      );
      coords.push_back(normalized);
    }
  }

  // Get 8 corner points
  vector<Vec3f> getCorners() const {
    return {
      Vec3f(min[0], min[1], min[2]),  // 0: bottom-front-left
      Vec3f(max[0], min[1], min[2]),  // 1: bottom-front-right
      Vec3f(max[0], max[1], min[2]),  // 2: bottom-back-right
      Vec3f(min[0], max[1], min[2]),  // 3: bottom-back-left
      Vec3f(min[0], min[1], max[2]),  // 4: top-front-left
      Vec3f(max[0], min[1], max[2]),  // 5: top-front-right
      Vec3f(max[0], max[1], max[2]),  // 6: top-back-right
      Vec3f(min[0], max[1], max[2])   // 7: top-back-left
    };
  }

  // Draw wireframe box
  void draw(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0) const {
    auto corners = getCorners();

    glColor4f(r, g, b, a);
    glBegin(GL_LINES);

    // Bottom face
    GL::vertex(corners[0]); GL::vertex(corners[1]);
    GL::vertex(corners[1]); GL::vertex(corners[2]);
    GL::vertex(corners[2]); GL::vertex(corners[3]);
    GL::vertex(corners[3]); GL::vertex(corners[0]);

    // Top face
    GL::vertex(corners[4]); GL::vertex(corners[5]);
    GL::vertex(corners[5]); GL::vertex(corners[6]);
    GL::vertex(corners[6]); GL::vertex(corners[7]);
    GL::vertex(corners[7]); GL::vertex(corners[4]);

    // Vertical edges
    GL::vertex(corners[0]); GL::vertex(corners[4]);
    GL::vertex(corners[1]); GL::vertex(corners[5]);
    GL::vertex(corners[2]); GL::vertex(corners[6]);
    GL::vertex(corners[3]); GL::vertex(corners[7]);

    glEnd();
  }

  // Draw corner points
  void drawCorners(float r = 1.0, float g = 0.0, float b = 0.0, float pointSize = 5.0) const {
    auto corners = getCorners();

    glColor3f(r, g, b);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for (const auto& corner : corners) {
      GL::vertex(corner);
    }
    glEnd();
    glPointSize(1);
  }
};

struct MyApp : App
{
  Mesh cone;
  Frame baseFrame;  // Controls position of cone base center

  // Cone parameters
  float width = 2.0;    // Diameter of base (radius = width/2)
  float height = 3.0;   // Height of cone
  float slices = 30;    // Circumferential resolution
  float stacks = 20;    // Vertical resolution (number of circles)

  // Track previous values to detect changes
  float prevWidth = 2.0;
  float prevHeight = 3.0;
  float prevSlices = 30;
  float prevStacks = 20;
  Vec prevPos = Vec(0, 0, 0);

  // Display options
  bool bShowBoundingBox = false;

  // Build cone from circles using skinning
  Mesh buildCone(const Point& baseCenter, float width, float height, int stacks, int slices) {
    vector<Circle> circles;
    float baseRadius = width / 2.0;

    // Create circles from base to apex
    for (int i = 0; i < stacks; ++i) {
      float t = (float)i / (stacks - 1);  // 0 at base, 1 at apex
      float y = t * height;

      // Linearly interpolate radius from baseRadius to nearly zero
      float radius = baseRadius * (1.0 - t);

      // Skip apex if radius gets too small (causes degenerate triangles)
      if (radius < 0.01) continue;

      // Get world position from frame
      Vec basePos = Vec(baseCenter);
      Point center = Round::null(basePos[0], basePos[1] + y, basePos[2]);

      // All circles horizontal (parallel to XZ plane)
      circles.push_back(Construct::circle(center, Biv::xz, radius));
    }

    // Add tiny circle at apex for clean point
    Vec basePos = Vec(baseCenter);
    Point apex = Round::null(basePos[0], basePos[1] + height, basePos[2]);
    circles.push_back(Construct::circle(apex, Biv::xz, 0.01));

    return Shape::Skin(circles.data(), circles.size(), false, slices);
  }

  virtual void onSetup()
  {
    // Attach frame to object controller for interactive positioning
    objectController.attach(&baseFrame);

    // Initial camera position
    scene.camera.pos(0, 0, 10);

    // Build initial cone
    Point baseCenter = Round::location(baseFrame.pos());
    cone = buildCone(baseCenter, width, height, (int)stacks, (int)slices);
  }

  virtual void onDrawGui()
  {
    // Setup GUI sliders
    gui(width, "width", 0.5, 5.0);
    gui(height, "height", 0.5, 8.0);
    gui(slices, "slices", 3, 60);
    gui(stacks, "stacks", 3, 50);

    // Display options
    gui(bShowBoundingBox, "Show BBox");
  }

  virtual void onDraw()
  {
    Point baseCenter = Round::location(baseFrame.pos());
    Vec currentPos = Vec(baseCenter);

    // Check if any parameter has changed
    bool needsRebuild = (width != prevWidth) ||
                        (height != prevHeight) ||
                        (slices != prevSlices) ||
                        (stacks != prevStacks) ||
                        (currentPos - prevPos).norm() > 0.001;

    // Rebuild cone geometry if parameters changed
    if (needsRebuild) {
      // Clear existing mesh data - THIS IS KEY!
      cone.clear();

      // Build new cone
      cone = buildCone(baseCenter, width, height, (int)stacks, (int)slices);

      // Update previous values
      prevWidth = width;
      prevHeight = height;
      prevSlices = slices;
      prevStacks = stacks;
      prevPos = currentPos;
    }

    // Draw the cone
    glColor3f(0.3, 0.8, 1.0);
    cone.drawElements();

    // Draw the base frame for reference
    Draw(baseFrame, 0.5);

    // Highlight base circle
    Circle baseCircle = Construct::circle(baseCenter, Biv::xz, width / 2.0);
    glLineWidth(2);
    Draw(baseCircle, 1.0, 0.5, 0.2);  // Orange
    glLineWidth(1);

    // Draw apex point
    Vec basePos = Vec(baseCenter);
    Point apex = Round::null(basePos[0], basePos[1] + height, basePos[2]);
    glPointSize(8);
    Draw(apex, 1.0, 0.0, 0.0);  // Red
    glPointSize(1);

    // Draw bounding box if enabled
    if (bShowBoundingBox && cone.num() > 0) {
      BoundingBox bbox = BoundingBox::fromMesh(cone);
      bbox.draw(1.0, 1.0, 0.0, 0.8);        // Yellow wireframe
      bbox.drawCorners(1.0, 0.0, 1.0, 6.0); // Magenta corner points
    }
  }
};

int main()
{
  MyApp app;
  app.start();

  return 0;
}

