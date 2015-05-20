#include <LumiverseCore.h>
#include <LumiverseShowControl\LumiverseShowControl.h>

using namespace Lumiverse;
using namespace Lumiverse::ShowControl;

// http://en.wikipedia.org/wiki/CIELUV
Eigen::Vector2d xyTouv(float x, float y) {
  return Eigen::Vector2d((4 * x) / (-2 * x + 12 * y + 3), (9 * y) / (-2 * x + 12 * y + 3));
}

Eigen::Vector2d uvToxy(float u, float v) {
  return Eigen::Vector2d((9 * u) / (6 * u - 16 * v + 12), (4 * v) / (6 * u - 16 * v + 12));
}

Eigen::Vector3d getColor(DeviceSet d) {
  Eigen::Vector3d xyz(0,0,0);

  for (auto d : d.getDevices()) {
    auto color = d->getColor();
    auto intens = (LumiverseFloat*) d->getParam("intensity");

    //if (intens->getVal() == 0)
    //  continue;

    if (color == nullptr) {
      xyz += ColorUtils::getApproxColor("N/C", intens->getVal());
    }
    else {
      xyz += color->getXYZ();
    }
  }

  auto xyy = ColorUtils::convXYZtoxyY(xyz);
  auto uv = xyTouv(xyy[0], xyy[1]);
  return Eigen::Vector3d(uv[0], uv[1], xyy[2]);
}

float getIntensity(DeviceSet d) {
  float val = 0;

  for (auto d : d.getDevices()) {
    // ASSUMES NO GELS AT THE MOMENT.

    auto intens = (LumiverseFloat*)d->getParam("intensity");
    val += intens->getVal();
  }

  return val;
}

void writeAngle(Rig* r, string q, ofstream& file) {
  auto lights = r->select(q);
  auto color = getColor(lights);
  auto intensity = getIntensity(lights);
  file << "," << intensity << "," << color[0] << "," << color[1];
}

void writeFeature(string name, Rig* r, ofstream& file) {
  // Detailed mode - collect triplets of data

  // Couple of assumptions real quick:
  // - Test rig at the moment is all LED
  // - Intensity is simply added from relative non-physically based levels
  // - Vector is in the form (intensity, x, y) ...

  file << name;
  
  // Front
  writeAngle(r, "$angle=front", file);

  // Front left
  writeAngle(r, "$angle=front left", file);

  // Front right
  writeAngle(r, "$angle=front right", file);
  
  // Top
  writeAngle(r, "$angle=top", file);

  // High Side Left
  writeAngle(r, "$angle=high side left", file);

  // High Side Right
  writeAngle(r, "$angle=high side right", file);

  // Back
  writeAngle(r, "$angle=back", file);

  // Back right
  writeAngle(r, "$angle=back right", file);

  // Back left
  writeAngle(r, "$angle=back left", file);

  // Side left
  writeAngle(r, "[$angle=side right,$angle=right shin]", file);

  // Side right
  writeAngle(r, "[$angle=side left,$angle=left shin]", file);

  // Low angle front
  writeAngle(r, "$angle=front up", file);

  // Low angle back
  writeAngle(r, "$angle=back up", file);

  file << "\n";
}

int main(int argc, char**argv) {
  Rig* rig = new Rig(string(argv[1]));
  Playback* pb = new Playback(rig, string(argv[2]));

  rig->init();
  pb->attachToRig();
  pb->start();
  pb->getProgrammer()->clearAndReset();
  rig->run();

  pb->getLayer("main")->activate();
  pb->getLayer("main")->resume();

  ofstream file;
  file.open("features_uv.csv", ios::out);

  // Generate features for each element in the playback timelines
  for (const auto& name : pb->getTimelineNames()) {
    pb->getLayer("main")->play(name);
    Sleep(50);
    cout << "Writing feature vector for example " << name << "\n";
    writeFeature(name, rig, file);
  }
}