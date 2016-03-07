#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>

using namespace Lumiverse;
using namespace Lumiverse::ShowControl;

string baseDir = "/usr2/eshimizu/lighting-attributes/scenes/gen/";

// Creates a timeline containing the current state of the rig and nothing else.
bool createTimeline(Rig* r, Playback* pb, int index) {
  string name;
  stringstream ss;
  ss << index;

  name = ss.str();

  auto tl = shared_ptr<Timeline>(new Timeline());

  tl->setKeyframe(r, 0, false);
  if (!pb->addTimeline(name, tl)) {
    Logger::log(ERR, "Failed to save timeline " + name);
    return false;
  }
  return true;
}

void renderAllScenes(Rig* r, Playback* pb) {
  ArnoldAnimationPatch* p = (ArnoldAnimationPatch*) r->getPatch("arnold");
  pb->start();

  auto timelines = pb->getTimelineNames();
  pb->getProgrammer()->clearAndReset();

  cout << "Number of Scenes: " << timelines.size() << "\n";

  int i = 1;
  for (const auto& name : timelines) {
//    if (i < 350) { i++; continue; }
    pb->getLayer("main")->play(name);
    usleep(50000);
    Logger::log(INFO, "Rendering Scene " + name);
    cout << "Rendering Scene " << i << "/" << timelines.size() << "\n";
    p->renderSingleFrame(r->getAllDevices().getDevices(), baseDir + name + ".png");
    i++;
  }

  pb->stop();
}

int permuteAngles(Rig* r, Playback* pb, int index, vector<string> angles, int steps) {
  if (angles.size() == 0) {
    return index;
  }

  float inc = 1.0f / steps;

  string query = "$angle=" + angles.back();
  angles.pop_back();

  for (int i = 1; i <= steps; i++) {
    r->select(query).setIntensity((float)(inc * i));
    if (angles.size() > 0) {
      index = permuteAngles(r, pb, index, angles, steps);
    }
    else {
      createTimeline(r, pb, index);
      index++;
    }
  }

  return index;
}

int permute3Point(Rig* r, Playback* pb, int index, int steps) {
  index = permuteAngles(r, pb, index, { "front left", "top", "front right" }, steps);

  r->select("$angle=top").setIntensity(0);
  index = permuteAngles(r, pb, index, { "front left", "front right" }, steps);
  
  r->select("$angle=front right").setIntensity(0);
  index = permuteAngles(r, pb, index, { "front left", "top" }, steps);

  r->select("$angle=front left").setIntensity(0);
  index = permuteAngles(r, pb, index, { "top", "front right" }, steps);
  
  return index;
}

int permuteScene(Rig* r, Playback* pb, int index = 1) {
  // All intensities to zero
  r->getAllDevices().setParam("intensity", 0);

  // Single angle scenes
  auto allDevices = r->getAllDevices();
  for (auto d : allDevices.getDevices()) {
    d->setParam("intensity", 1.0);
    createTimeline(r, pb, index);
    index++;

    d->setIntensity(0.75);
    createTimeline(r, pb, index);
    index++;

    d->setParam("intensity", 0.5);
    createTimeline(r, pb, index);
    index++;

    d->setParam("intensity", 0.2);
    createTimeline(r, pb, index);
    index++;

    d->setParam("intensity", 0.0);
  }

  // Other combos
  auto left = r->select("$angle=front left");
  auto top = r->select("$angle=top");
  auto right = r->select("$angle=front right");
  auto bl = r->select("$angle=back left");
  auto b = r->select("$angle=back");
  auto br = r->select("$angle=back right");

  // Three point scenes
  allDevices.setParam("intensity", 0.0);
  index = permute3Point(r, pb, index, 4);

  // Three point +/- various configurations
  vector<string> angles = { "back", "back left", "back right", "high side left", "high side right", "side left", "side right", "left shin", "right shin", "front up", "back up", "front" };

  for (const auto& a : angles) {
    string query = "$angle=" + a;

    left.setIntensity(1);
    right.setIntensity(1);
    top.setIntensity(1);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0.5);
    right.setIntensity(0.5);
    top.setIntensity(0.5);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0.2);
    right.setIntensity(0.2);
    top.setIntensity(0.2);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0.75);
    right.setIntensity(0.75);
    top.setIntensity(0);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0);
    right.setIntensity(0.75);
    top.setIntensity(0.75);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0.75);
    right.setIntensity(0);
    top.setIntensity(0.75);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0);
    right.setIntensity(0);
    top.setIntensity(0.75);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0);
    right.setIntensity(0.75);
    top.setIntensity(0);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    left.setIntensity(0.75);
    right.setIntensity(0);
    top.setIntensity(0);
    r->select(query).setIntensity(1);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.5);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0.2);
    createTimeline(r, pb, index);
    index++;

    r->select(query).setIntensity(0);
  }

  // All back
  r->getAllDevices().setIntensity(0);
  index = permuteAngles(r, pb, index, { "back", "back left", "back right" }, 3);

  r->getAllDevices().setIntensity(0);
  index = permuteAngles(r, pb, index, { "front", "back", "high side left", "high side right" }, 3);

  r->getAllDevices().setIntensity(0);
  index = permuteAngles(r, pb, index, { "top", "front up", "back up" }, 3);

  r->getAllDevices().setIntensity(0);
  index = permuteAngles(r, pb, index, { "front left", "front right", "side left", "side right" }, 3);

  // Special
  // Misc scene configurations that are common and/or interesting

  return index;
}

int main(int argc, char**argv) {
  Logger::setLogFile("/usr2/eshimizu/lighting-attributes/scenes/lumiverse.log");
  Rig* r = new Rig("/usr2/eshimizu/lighting-attributes/scenes/single_area_box.rig.json");
  Playback* pb = new Playback(r, "/usr2/eshimizu/lighting-attributes/scenes/samples.playback.json");
  pb->attachToRig();

  // pb->addLayer(shared_ptr<Layer>(new Layer(r, pb, "main", 1, 1)));
  pb->getLayer("main")->activate();
  pb->getLayer("main")->resume();

  ArnoldAnimationPatch* p = (ArnoldAnimationPatch*) r->getPatch("arnold");
  r->init();
  r->run();

  p->disableContinuousRenderMode();
  p->setSamples(3);

  //r->save("/usr2/eshimizu/lighting-attributes/scenes/samples.rig.json", true);
  //pb->save("/usr2/eshimizu/lighting-attributes/scenes/samples.playback.json", true);
  renderAllScenes(r, pb);
  delete r;
}
