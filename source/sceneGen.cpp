#include <LumiverseCore.h>
#include <LumiverseShowControl\LumiverseShowControl.h>
#include <random>

using namespace Lumiverse;
using namespace Lumiverse::ShowControl;

string baseDir = "C:/Users/eshimizu/Documents/Lumiverse/attributes/scenes/gen/";
auto rng = minstd_rand0(1991);

// Set up colors
//                      R02    R05    R06    R15   R99
vector<double> warmx = { 0.488, 0.477, 0.457, 0.560, 0.503 };
vector<double> warmy = { 0.409, 0.399, 0.415, 0.433, 0.404 };
//                      R360   R51    R3206 D65 R3206 A R54    R62    R71
vector<double> coolx = { 0.390, 0.291, 0.282,    0.420,  0.424, 0.373, 0.261 };
vector<double> cooly = { 0.377, 0.286, 0.299,    0.394,  0.377, 0.390, 0.383 };
//                        R316   R318   R19    R27    R39    R49    R53 D65 R65    R73    R80    R382   R91    R88 D65 R368   R10    R3408  R3206 D65
vector<double> accentx = { 0.528, 0.568, 0.650, 0.692, 0.584, 0.496, 0.287,  0.283, 0.274, 0.157, 0.189, 0.280, 0.338,  0.249, 0.511, 0.503, 0.282    };
vector<double> accenty = { 0.412, 0.381, 0.342, 0.302, 0.268, 0.229, 0.302,  0.352, 0.418, 0.209, 0.103, 0.561, 0.417,  0.299, 0.468, 0.414, 0.299    };

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
//    Sleep(50);
    Logger::log(INFO, "Rendering Scene " + name);
    cout << "Rendering Scene " << i << "/" << timelines.size() << "\n";
    p->renderSingleFrame(r->getAllDevices().getDevices(), baseDir + name + ".png");
    i++;
  }

  pb->stop();
}

int sampleAngles(Rig* r, Playback* pb, int index, vector<string> angles, int steps, float accept) {
  if (angles.size() == 0) {
    return index;
  }

  float inc = 1.0f / steps;

  string query = "$angle=" + angles.back();
  angles.pop_back();

  for (int i = 1; i <= steps; i++) {
    r->select(query).setIntensity((float)(inc * i));
    if (angles.size() > 0) {
      index = sampleAngles(r, pb, index, angles, steps, accept);
    }
    else {
      if (rng() % 1000 < accept * 1000) {
      createTimeline(r, pb, index);
      index++;
    }
  }
  }

  return index;
}

int permuteAngles(Rig* r, Playback* pb, int index, vector<string> angles, int steps) {
  return sampleAngles(r, pb, index, angles, steps, 1);
}

int permute3Point(Rig* r, Playback* pb, int index, int steps) {
  index = sampleAngles(r, pb, index, { "front left", "top", "front right" }, steps, 0.5);

  //r->select("$angle=top").setIntensity(0);
  //index = permuteAngles(r, pb, index, { "front left", "front right" }, steps);
  
  //r->select("$angle=front right").setIntensity(0);
  //index = permuteAngles(r, pb, index, { "front left", "top" }, steps);

  //r->select("$angle=front left").setIntensity(0);
  //index = permuteAngles(r, pb, index, { "top", "front right" }, steps);
  
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

    d->setIntensity(0.50);
    createTimeline(r, pb, index);
    index++;

    d->setParam("intensity", 0.2);
    createTimeline(r, pb, index);
    index++;

    d->setParam("intensity", 0.1);
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

    r->select(query).setIntensity(0);
  }

  // Samples
  r->getAllDevices().setIntensity(0);
  index = sampleAngles(r, pb, index, { "back", "back left", "back right" }, 3, 0.25);

  r->getAllDevices().setIntensity(0);
  index = sampleAngles(r, pb, index, { "front", "back", "high side left", "high side right" }, 3, 0.25);

  r->getAllDevices().setIntensity(0);
  index = sampleAngles(r, pb, index, { "top", "front up", "back up" }, 3, 0.25);

  r->getAllDevices().setIntensity(0);
  index = sampleAngles(r, pb, index, { "front left", "front right", "side left", "side right" }, 3, 0.25);

  // Special
  // Misc scene configurations that are common and/or interesting

  return index;
}

void pickColors(Rig* r, int lr) {
  // Pick warm in left or right
  int dummy = rng(); // advance RNG once to compensate
  if (lr == 0) {
    int w = rng() % warmx.size();
    int c = rng() % coolx.size();
    r->select("$angle=front left").setParam("color", warmx[w], warmy[w]);
    r->select("$angle=front right").setParam("color", coolx[c], cooly[c]);
  }
  else {
    int w = rng() % warmx.size();
    int c = rng() % coolx.size();
    r->select("$angle=front right").setParam("color", warmx[w], warmy[w]);
    r->select("$angle=front left").setParam("color", coolx[c], cooly[c]);
  }

  // Pick front
  int f = rng() % (warmx.size() + coolx.size());
  if (f < warmx.size()) {
    r->select("$angle=front").setParam("color", warmx[f], warmy[f]);
  }
  else {
    f -= warmx.size();
    r->select("$angle=front").setParam("color", coolx[f], cooly[f]);
  }

  // Pick accents
  vector<string> angles = { "back", "back left", "back right", "high side left", "high side right", "side left", "side right", "left shin", "right shin", "front up", "back up" };
  for (const auto& a : angles) {
    int ai = rng() % accentx.size();
    r->select("$angle=" + a).setParam("color", accentx[ai], accenty[ai]);
  }
}

int main(int argc, char**argv) {
  //Logger::setLogFile("E:/Users/falindrith/Documents/Programming/Lumiverse/lighting-attributes/scenes/lumiverse.log");
  Rig* r = new Rig("C:/Users/eshimizu/Documents/Lumiverse/attributes/scenes/single_area_box.rig.json");
  Playback* pb = new Playback(r);
  pb->attachToRig();

  // pb->addLayer(shared_ptr<Layer>(new Layer(r, pb, "main", 1, 1)));
  pb->getLayer("main")->activate();
  pb->getLayer("main")->resume();

  ArnoldAnimationPatch* p = (ArnoldAnimationPatch*) r->getPatch("arnold");
  r->init();
  r->run();

  p->disableContinuousRenderMode();
  p->setSamples(3);

  // Pre-defined color palletes
  r->select("$angle=front left").setParam("color", 0.488, 0.409, 1);
  r->select("$angle=front right").setParam("color", 0.271, 0.290, 1);
  r->select("$angle=front").setParam("color", 0.505, 0.427);
  r->select("$angle=top").setParam("color", 0.568, 0.381);
  r->select("$angle=high side left").setParam("color", 0.608, 0.385);
  r->select("$angle=high side right").setParam("color", 0.496, 0.229);
  r->select("$angle=back").setParam("color", 0.692, 0.302);
  r->select("$angle=back left").setParam("color", 0.259, 0.437);
  r->select("$angle=back right").setParam("color", 0.610, 0.308);
  r->select("$angle=side right").setParam("color", 0.385, 0.361);
  r->select("$angle=side left").setParam("color", 0.457, 0.425);
  r->select("$angle=right shin").setParam("color", 0.584, 0.268);
  r->select("$angle=left shin").setParam("color", 0.162, 0.108);
  r->select("$angle=front up").setParam("color", 0.488, 0.409);
  r->select("$angle=back up").setParam("color", 0.488, 0.409);
  int numScenes = permuteScene(r, pb);

  r->select("$angle=front").setParam("color", 0.291, 0.286, 1);
  r->select("$angle=front right").setParam("color", 0.477, 0.419, 1);
  r->select("$angle=front left").setParam("color", 0.333, 0.355, 1);
  r->select("$angle=top").setParam("color", 0.157, 0.209, 1);
  r->select("$angle=high side left").setParam("color", 0.147, 0.070, 1);
  r->select("$angle=high side right").setParam("color", 0.584, 0.268, 1);
  r->select("$angle=back").setParam("color", 0.457, 0.415);
  r->select("$angle=back left").setParam("color", 0.635, 0.336);
  r->select("$angle=back right").setParam("color", 0.280, 0.561);
  r->select("$angle=side right").setParam("color", 0.560, 0.433);
  r->select("$angle=side left").setParam("color", 0.277, 0.187);
  r->select("$angle=right shin").setParam("color", 0.264, 0.311);
  r->select("$angle=left shin").setParam("color", 0.490, 0.396);
  r->select("$angle=front up").setParam("color", 0.692, 0.302);
  r->select("$angle=back up").setParam("color", 0.147, 0.333);
  numScenes = permuteScene(r, pb, numScenes);

  for (int i = 0; i < 10; i++) {
    pickColors(r, i % 2);
    numScenes = permuteScene(r, pb, numScenes);
  }

  r->save("C:/Users/eshimizu/Documents/Lumiverse/attributes/scenes/samples.rig.json", true);
  pb->save("C:/Users/eshimizu/Documents/Lumiverse/attributes/scenes/samples.playback.json", true);
  // renderAllScenes(r, pb);
  delete r;
}