// Interactive Cone Builder using Circle Skinning
// Demonstrates Shape::Skin() with GUI controls and Frame positioning

#include "vsr/vsr_app.h"
#include "vsr/form/vsr_shapes.h"
#include "vsr/form/vsr_tangent.h"

using namespace vsr::cga;
using namespace gfx;

// Face enum for bounding box deformation
enum class Face {
  Front,   // z=min
  Back,    // z=max
  Left,    // x=min
  Right,   // x=max
  Bottom,  // y=min
  Top      // y=max
};

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

  // Get 8 corner points in CRD bit order (x=bit0, y=bit1, z=bit2)
  vector<Vec3f> getCorners() const {
    return {
      Vec3f(min[0], min[1], min[2]),  // 0: 000 (o)
      Vec3f(max[0], min[1], min[2]),  // 1: 100 (u)
      Vec3f(min[0], max[1], min[2]),  // 2: 010 (v)
      Vec3f(max[0], max[1], min[2]),  // 3: 110 (uv)
      Vec3f(min[0], min[1], max[2]),  // 4: 001 (w)
      Vec3f(max[0], min[1], max[2]),  // 5: 101 (uw)
      Vec3f(min[0], max[1], max[2]),  // 6: 011 (vw)
      Vec3f(max[0], max[1], max[2])   // 7: 111 (uvw)
    };
  }

  // Create TPointBox from bounding box corners
  TPointBox toTPointBox() const {
    auto corners = getCorners();
    Point pnts[8];
    for (int i = 0; i < 8; ++i) {
      pnts[i] = Round::null(corners[i][0], corners[i][1], corners[i][2]);
    }
    return TPointBox(pnts);
  }

  // Draw wireframe box
  void draw(float r = 1.0, float g = 1.0, float b = 1.0, float a = 1.0) const {
    auto corners = getCorners();

    glColor4f(r, g, b, a);
    glBegin(GL_LINES);

    // Bottom face (z=min): 0→1→3→2→0 (000→100→110→010→000)
    GL::vertex(corners[0]); GL::vertex(corners[1]);
    GL::vertex(corners[1]); GL::vertex(corners[3]);
    GL::vertex(corners[3]); GL::vertex(corners[2]);
    GL::vertex(corners[2]); GL::vertex(corners[0]);

    // Top face (z=max): 4→5→7→6→4 (001→101→111→011→001)
    GL::vertex(corners[4]); GL::vertex(corners[5]);
    GL::vertex(corners[5]); GL::vertex(corners[7]);
    GL::vertex(corners[7]); GL::vertex(corners[6]);
    GL::vertex(corners[6]); GL::vertex(corners[4]);

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

// Helper function to deform a TPointBox by applying boost through a face
void deformTPointBox(TPointBox& tpbox, float amt, Face face) {
  // Get circle from the specified face using TPointBox methods
  Circle circle;
  switch(face) {
    case Face::Front:
      circle = tpbox.cf();  // front circle
      break;
    case Face::Back:
      circle = tpbox.ck();  // back circle (k for back)
      break;
    case Face::Left:
      circle = tpbox.cl();  // left circle
      break;
    case Face::Right:
      circle = tpbox.cr();  // right circle
      break;
    case Face::Bottom:
      circle = tpbox.cb();  // bottom circle
      break;
    case Face::Top:
      circle = tpbox.ct();  // top circle
      break;
  }

  // Create boost generator from circle dual scaled by amount
  Pair generator = circle.dual() * amt;
  Bst K = Gen::bst(generator);

  // Apply boost to all 8 corner points in place
  for (int i = 0; i < 8; ++i) {
    Point transformed = tpbox.p[i].spin(K);
    tpbox.p[i] = Round::location(transformed);
  }
}

struct MyApp : App
{
  Mesh cone;
  Mesh deformedCone;  // Deformed version of cone
  Frame baseFrame;  // Controls position of cone base center
  Frame cornerFrame;  // Frame to control a corner of the bounding box

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
  bool bApplyDeformation = false;
  float edgeResolution = 10;  // Number of segments per edge
  bool bShowBaseFrame = true;
  bool bShowCornerFrame = true;

  // Mesh rendering options
  enum class RenderMode { Smooth, Checkerbox };
  RenderMode renderMode = RenderMode::Smooth;

  // Face deformation amounts
  float deformScale = 0.1;  // Global scale for all deformations
  float deformFront = 0.0;
  float deformBack = 0.0;
  float deformLeft = 0.0;
  float deformRight = 0.0;
  float deformBottom = 0.0;
  float deformTop = 0.0;

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

  // Draw TFrameBox edges by sampling calcMap
  void drawTFrameBoxEdges(TFrameBox& tbox, int resolution, float r = 1.0, float g = 1.0, float b = 0.0) {
    glColor3f(r, g, b);
    glLineWidth(2);
    glBegin(GL_LINES);

    // Bottom face (z=0): edges 0-1, 1-3, 3-2, 2-0
    for (int i = 0; i < resolution; ++i) {
      float t0 = (float)i / resolution;
      float t1 = (float)(i+1) / resolution;

      // Edge 0→1 (u varies, v=0, w=0)
      Point p0 = tbox.calcMap(t0, 0, 0);
      Point p1 = tbox.calcMap(t1, 0, 0);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 1→3 (u=1, v varies, w=0)
      p0 = tbox.calcMap(1, t0, 0);
      p1 = tbox.calcMap(1, t1, 0);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 3→2 (u varies reversed, v=1, w=0)
      p0 = tbox.calcMap(1-t0, 1, 0);
      p1 = tbox.calcMap(1-t1, 1, 0);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 2→0 (u=0, v varies reversed, w=0)
      p0 = tbox.calcMap(0, 1-t0, 0);
      p1 = tbox.calcMap(0, 1-t1, 0);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));
    }

    // Top face (z=1): edges 4-5, 5-7, 7-6, 6-4
    for (int i = 0; i < resolution; ++i) {
      float t0 = (float)i / resolution;
      float t1 = (float)(i+1) / resolution;

      // Edge 4→5 (u varies, v=0, w=1)
      Point p0 = tbox.calcMap(t0, 0, 1);
      Point p1 = tbox.calcMap(t1, 0, 1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 5→7 (u=1, v varies, w=1)
      p0 = tbox.calcMap(1, t0, 1);
      p1 = tbox.calcMap(1, t1, 1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 7→6 (u varies reversed, v=1, w=1)
      p0 = tbox.calcMap(1-t0, 1, 1);
      p1 = tbox.calcMap(1-t1, 1, 1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 6→4 (u=0, v varies reversed, w=1)
      p0 = tbox.calcMap(0, 1-t0, 1);
      p1 = tbox.calcMap(0, 1-t1, 1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));
    }

    // Vertical edges: 0-4, 1-5, 2-6, 3-7
    for (int i = 0; i < resolution; ++i) {
      float t0 = (float)i / resolution;
      float t1 = (float)(i+1) / resolution;

      // Edge 0→4 (u=0, v=0, w varies)
      Point p0 = tbox.calcMap(0, 0, t0);
      Point p1 = tbox.calcMap(0, 0, t1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 1→5 (u=1, v=0, w varies)
      p0 = tbox.calcMap(1, 0, t0);
      p1 = tbox.calcMap(1, 0, t1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 2→6 (u=0, v=1, w varies)
      p0 = tbox.calcMap(0, 1, t0);
      p1 = tbox.calcMap(0, 1, t1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));

      // Edge 3→7 (u=1, v=1, w varies)
      p0 = tbox.calcMap(1, 1, t0);
      p1 = tbox.calcMap(1, 1, t1);
      GL::vertex(Vec3f(p0[0], p0[1], p0[2]));
      GL::vertex(Vec3f(p1[0], p1[1], p1[2]));
    }

    glEnd();
    glLineWidth(1);
  }

  // Create TFrameBox from bounding box corners with optional face deformations
  TFrameBox createTFrameBox(const BoundingBox& bbox) {
    // Create TPointBox from bounding box
    TPointBox tpbox = bbox.toTPointBox();

    // Apply face deformations if any are non-zero (scaled by deformScale)
    if (deformFront != 0.0) deformTPointBox(tpbox, deformFront * deformScale, Face::Front);
    if (deformBack != 0.0) deformTPointBox(tpbox, deformBack * deformScale, Face::Back);
    if (deformLeft != 0.0) deformTPointBox(tpbox, deformLeft * deformScale, Face::Left);
    if (deformRight != 0.0) deformTPointBox(tpbox, deformRight * deformScale, Face::Right);
    if (deformBottom != 0.0) deformTPointBox(tpbox, deformBottom * deformScale, Face::Bottom);
    if (deformTop != 0.0) deformTPointBox(tpbox, deformTop * deformScale, Face::Top);

    // Move corner 111 based on cornerFrame
    cornerFrame.pos() = tpbox.puvw();

    // Create TFrameBox from deformed TPointBox
    Frame f111(tpbox.puvw(), cornerFrame.rotor());
    return TFrameBox(f111, tpbox.p);
  }

  // Apply TFrameBox deformation using bounding box corners and normalized coords
  void applyDeformation(Mesh& outputMesh, const Mesh& inputMesh, const BoundingBox& bbox, TFrameBox& tbox) {
    if (inputMesh.num() == 0 || bbox.coords.size() != inputMesh.num()) return;

    // Clear and rebuild output mesh
    outputMesh.clear();
    outputMesh = inputMesh;  // Copy structure

    // Recalculate each vertex position using TFrameBox and normalized coords
    for (int i = 0; i < inputMesh.num(); ++i) {
      const Vec3f& uvw = bbox.coords[i];
      Point newPos = tbox.calcMap(uvw[0], uvw[1], uvw[2]);
      outputMesh[i].Pos = Vec3f(newPos[0], newPos[1], newPos[2]);
    }

    // Recalculate normals based on new positions (similar to Shape::Skin)
    // For triangle strip topology, calculate normals from neighboring vertices
    int res = (int)slices;
    int num = (int)stacks;
    for (int i = 0; i < res; ++i) {
      for (int j = 0; j < num; ++j) {
        int a = i * num + j;
        int b = (i < (res - 1)) ? a + num : j;              // next col
        int c = (i > 0) ? a - num : (res - 1) * num + j;    // prev col
        int d = (j < (num - 1)) ? a + 1 : a;                // next row
        int e = (j > 0) ? a - 1 : a;                        // prev row

        Vec3f va = (outputMesh[b].Pos - outputMesh[c].Pos).cross(
                    outputMesh[d].Pos - outputMesh[e].Pos).unit();
        outputMesh[a].Norm = va;
      }
    }
  }

  virtual void onSetup()
  {
    // Attach frames to object controller for interactive positioning
    objectController.attach(&baseFrame);
    objectController.attach(&cornerFrame);


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
    ImGui::Separator();
    ImGui::Text("Display Options");
    gui(bShowBoundingBox, "Show BBox/Edges");
    gui(bShowBaseFrame, "Show Base Frame");
    gui(bShowCornerFrame, "Show Corner Frame");

    // Render mode selection
    ImGui::Text("Render Mode:");
    if (ImGui::RadioButton("Smooth", renderMode == RenderMode::Smooth)) {
      renderMode = RenderMode::Smooth;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Checkerbox", renderMode == RenderMode::Checkerbox)) {
      renderMode = RenderMode::Checkerbox;
    }

    gui(bApplyDeformation, "Apply Deformation");

    if (bApplyDeformation) {
      gui(edgeResolution, "Edge Resolution", 2, 50);

      // Face deformation controls
      ImGui::Separator();
      ImGui::Text("Face Deformations");
      gui(deformScale, "Deform Scale", 0.01, 1.0);
      gui(deformFront, "Front", -0.5f, 0.5f);
      gui(deformBack, "Back", -0.5f, 0.5f);
      gui(deformLeft, "Left", -0.5f, 0.5f);
      gui(deformRight, "Right", -0.5f, 0.5f);
      gui(deformBottom, "Bottom", -0.5f, 0.5f);
      gui(deformTop, "Top", -0.5f, 0.5f);
    }

    // Reset button
    if (ImGui::Button("Reset All Values")) {
      width = 2.0;
      height = 3.0;
      slices = 30;
      stacks = 20;
      cornerFrame.rotor() = Rot(1);
      baseFrame.rotor() = Rot(1);
      deformScale = 0.1;
      deformFront = deformBack = deformLeft = deformRight = deformBottom = deformTop = 0.0;
    }
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

    // Compute bounding box (with normalized coords)
    BoundingBox bbox = BoundingBox::fromMesh(cone);


    // Apply deformation if enabled
    Mesh* meshToDraw = &cone;
    TFrameBox* tboxPtr = nullptr;
    TFrameBox tbox = createTFrameBox(bbox);  // Create TFrameBox for drawing edges

    if (bApplyDeformation && cone.num() > 0) {
      applyDeformation(deformedCone, cone, bbox, tbox);
      meshToDraw = &deformedCone;
      tboxPtr = &tbox;
    }

    // Draw the cone (original or deformed)
    if (renderMode == RenderMode::Smooth) {
      // Smooth rendering - single color
      glColor3f(0.3, 0.8, 1.0);
      meshToDraw->drawElements();
    } else if (renderMode == RenderMode::Checkerbox) {
      // Checkerbox pattern - handle triangle strip topology
      // Shape::Skin generates GL_TRIANGLE_STRIP, not individual triangles
      glBegin(GL_TRIANGLES);
      bool bC = true;   // Color state
      bool bS = false;  // Toggle state

      int numIndices = meshToDraw->numIdx();
      // Triangle strip: each new index forms a triangle with previous 2
      for (int i = 0; i < numIndices - 2; ++i) {
        // Set color based on current state
        glColor3f(bC ? 0.3 : 0.1, bC ? 0.5 : 0.2, bC ? 1.0 : 0.5);

        // In triangle strip, every 3 consecutive indices form a triangle
        // But winding order alternates, so we need to handle that
        int idx0 = meshToDraw->indices()[i];
        int idx1 = meshToDraw->indices()[i + 1];
        int idx2 = meshToDraw->indices()[i + 2];

        // Triangle strips alternate winding order
        if (i % 2 == 0) {
          // Even triangles: normal order
          GL::normal((*meshToDraw)[idx0].Norm);
          GL::vertex((*meshToDraw)[idx0].Pos);
          GL::normal((*meshToDraw)[idx1].Norm);
          GL::vertex((*meshToDraw)[idx1].Pos);
          GL::normal((*meshToDraw)[idx2].Norm);
          GL::vertex((*meshToDraw)[idx2].Pos);
        } else {
          // Odd triangles: reverse order to maintain consistent winding
          GL::normal((*meshToDraw)[idx0].Norm);
          GL::vertex((*meshToDraw)[idx0].Pos);
          GL::normal((*meshToDraw)[idx2].Norm);
          GL::vertex((*meshToDraw)[idx2].Pos);
          GL::normal((*meshToDraw)[idx1].Norm);
          GL::vertex((*meshToDraw)[idx1].Pos);
        }

        // Update state for next triangle (GraphUtil logic)
        if (bS) bC = !bC;
        bS = !bS;
      }
      glEnd();
    }

    // Draw the base frame if visible
    if (bShowBaseFrame) {
      Draw(baseFrame, 0.5);
    }

    // Draw corner frame if deformation is enabled and visible
    if (bApplyDeformation && bShowCornerFrame) {
      Draw(cornerFrame, 0.5);
    }

    // Draw bounding box or TFrameBox edges if enabled
    if (bShowBoundingBox && cone.num() > 0) {
      if (bApplyDeformation && tboxPtr) {
        // Draw TFrameBox edges using calcMap sampling
        drawTFrameBoxEdges(*tboxPtr, (int)edgeResolution, 1.0, 1.0, 0.0);  // Yellow
      } else {
        // Draw regular bounding box
        bbox.draw(1.0, 1.0, 0.0, 0.8);        // Yellow wireframe
        bbox.drawCorners(1.0, 0.0, 1.0, 6.0); // Magenta corner points
      }
    }
  }
};

int main()
{
  MyApp app;
  app.start();

  return 0;
}

