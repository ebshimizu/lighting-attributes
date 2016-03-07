/*
==============================================================================

AttributeSearch.cpp
Created: 6 Jan 2016 11:25:42am
Author:  falindrith

==============================================================================
*/

#include "sampleTools.h"
#include <random>
#include <list>
//#include <vld.h>

Rig* _rig;

Rig* getRig() {
  return _rig;
}

map<EditType, vector<EditConstraint> > editConstraints = {
  { ALL,{ EditConstraint(L_KEY, RED), EditConstraint(L_KEY, GREEN), EditConstraint(L_KEY, BLUE),
  EditConstraint(L_KEY, INTENSITY), EditConstraint(L_KEY, POLAR), EditConstraint(L_KEY, AZIMUTH),
  EditConstraint(L_FILL, RED), EditConstraint(L_FILL, GREEN), EditConstraint(L_FILL, BLUE),
  EditConstraint(L_FILL, INTENSITY), EditConstraint(L_FILL, POLAR), EditConstraint(L_FILL, AZIMUTH),
  EditConstraint(L_RIM, RED), EditConstraint(L_RIM, GREEN), EditConstraint(L_RIM, BLUE),
  EditConstraint(L_RIM, INTENSITY), EditConstraint(L_RIM, POLAR), EditConstraint(L_RIM, AZIMUTH),
  EditConstraint(L_KEY, SOFT), EditConstraint(L_FILL, SOFT), EditConstraint(L_RIM, SOFT) } },
  { ALL_HSV,{ EditConstraint(L_KEY, HUE), EditConstraint(L_KEY, SAT), EditConstraint(L_KEY, VALUE),
  EditConstraint(L_FILL, HUE), EditConstraint(L_FILL, SAT), EditConstraint(L_FILL, VALUE),
  EditConstraint(L_RIM, HUE), EditConstraint(L_RIM, SAT), EditConstraint(L_RIM, VALUE) } },
  { ALL_RGB,{ EditConstraint(L_KEY, RED), EditConstraint(L_KEY, GREEN), EditConstraint(L_KEY, BLUE),
  EditConstraint(L_FILL, RED), EditConstraint(L_FILL, GREEN), EditConstraint(L_FILL, BLUE),
  EditConstraint(L_RIM, RED), EditConstraint(L_RIM, GREEN), EditConstraint(L_RIM, BLUE) } },
  { ALL_SAT,{ EditConstraint(L_KEY, SAT), EditConstraint(L_FILL, SAT), EditConstraint(L_RIM, SAT) } },
  { ALL_HUE,{ EditConstraint(L_KEY, HUE), EditConstraint(L_FILL, HUE), EditConstraint(L_RIM, HUE) } },
  { ALL_INTENS,{ EditConstraint(L_KEY, INTENSITY), EditConstraint(L_FILL, INTENSITY), EditConstraint(L_RIM, INTENSITY) } },
  { ALL_POS,{ EditConstraint(L_KEY, POLAR), EditConstraint(L_FILL, POLAR), EditConstraint(L_RIM, POLAR),
  EditConstraint(L_KEY, AZIMUTH), EditConstraint(L_FILL, AZIMUTH), EditConstraint(L_RIM, AZIMUTH) } },
  { KEY_HUE,{ EditConstraint(L_KEY, HUE) } },
  { FILL_HUE,{ EditConstraint(L_FILL, HUE) } },
  { RIM_HUE,{ EditConstraint(L_RIM, HUE) } },
  { KEY_INTENS,{ EditConstraint(L_KEY, INTENSITY) } },
  { FILL_INTENS,{ EditConstraint(L_FILL, INTENSITY) } },
  { RIM_INTENS,{ EditConstraint(L_RIM, INTENSITY) } },
  { KEY_POS,{ EditConstraint(L_KEY, AZIMUTH), EditConstraint(L_KEY, POLAR) } },
  { FILL_POS,{ EditConstraint(L_FILL, AZIMUTH), EditConstraint(L_FILL, POLAR) } },
  { RIM_POS,{ EditConstraint(L_RIM, AZIMUTH), EditConstraint(L_RIM, POLAR) } },
  { KEY_SAT,{ EditConstraint(L_KEY, SAT) } },
  { FILL_SAT,{ EditConstraint(L_FILL, SAT) } },
  { RIM_SAT,{ EditConstraint(L_RIM, SAT) } },
  { KEY_HSV,{ EditConstraint(L_KEY, HUE), EditConstraint(L_KEY, SAT), EditConstraint(L_KEY, VALUE) } },
  { FILL_HSV,{ EditConstraint(L_FILL, HUE), EditConstraint(L_FILL, SAT), EditConstraint(L_FILL, VALUE) } },
  { RIM_HSV,{ EditConstraint(L_RIM, HUE), EditConstraint(L_RIM, SAT), EditConstraint(L_RIM, VALUE) } },
  { KEY_SOFT,{ EditConstraint(L_KEY, SOFT) } },
  { FILL_SOFT,{ EditConstraint(L_FILL, SOFT) } },
  { RIM_SOFT,{ EditConstraint(L_RIM, SOFT) } },
  { ALL_SOFT,{ EditConstraint(L_KEY, SOFT), EditConstraint(L_FILL, SOFT), EditConstraint(L_RIM, SOFT) } },
  { KEY_FILL_INTENS,{ EditConstraint(L_KEY, INTENSITY), EditConstraint(L_FILL, INTENSITY) } },
  { KEY_RIM_INTENS,{ EditConstraint(L_KEY, INTENSITY), EditConstraint(L_RIM, INTENSITY) } },
  { FILL_RIM_INTENS,{ EditConstraint(L_FILL, INTENSITY), EditConstraint(L_RIM, INTENSITY) } },
  { KEY_FILL_HUE,{ EditConstraint(L_KEY, HUE), EditConstraint(L_FILL, HUE) } },
  { KEY_FILL_SAT,{ EditConstraint(L_KEY, SAT), EditConstraint(L_FILL, SAT) } },
  { KEY_FILL_HSV,{ EditConstraint(L_KEY, HUE), EditConstraint(L_KEY, SAT), EditConstraint(L_KEY, VALUE),
  EditConstraint(L_FILL, HUE), EditConstraint(L_FILL, SAT), EditConstraint(L_FILL, VALUE) } },
  { KEY_FILL_POS,{ EditConstraint(L_KEY, POLAR), EditConstraint(L_KEY, AZIMUTH), EditConstraint(L_FILL, POLAR), EditConstraint(L_FILL, POLAR) } },
  { KEY_RIM_HSV,{ EditConstraint(L_KEY, HUE), EditConstraint(L_KEY, SAT), EditConstraint(L_KEY, VALUE),
  EditConstraint(L_RIM, HUE), EditConstraint(L_RIM, SAT), EditConstraint(L_RIM, VALUE) } },
  { KEY_RIM_POS,{ EditConstraint(L_KEY, POLAR), EditConstraint(L_KEY, AZIMUTH), EditConstraint(L_RIM, POLAR), EditConstraint(L_RIM, AZIMUTH) } },
  { KEY_RGB,{ EditConstraint(L_KEY, RED), EditConstraint(L_KEY, GREEN), EditConstraint(L_KEY, BLUE) } },
  { FILL_RGB,{ EditConstraint(L_FILL, RED), EditConstraint(L_FILL, GREEN), EditConstraint(L_FILL, BLUE) } },
  { RIM_RGB,{ EditConstraint(L_RIM, RED), EditConstraint(L_RIM, GREEN), EditConstraint(L_RIM, BLUE) } },
  { KEY_POS_FILL_POS_MATCH,{ EditConstraint(L_KEY, AZIMUTH), EditConstraint(L_KEY, POLAR) } },
  { KEY_INTENS_RIM_CONTRAST_MATCH,{ EditConstraint(L_KEY, INTENSITY) } },
  { KEY_INTENS_FILL_CONTRAST_MATCH,{ EditConstraint(L_KEY, INTENSITY) } },
  { KEY_HUE_FILL_HUE_MATCH,{ EditConstraint(L_KEY, HUE) } },
  { KEY_HUE_FILL_RIM_HUE_MATCH,{ EditConstraint(L_KEY, HUE) } }
};

void filterResults(list<Eigen::VectorXd>& results, double t)
{
  // starting at the first element
  for (auto it = results.begin(); it != results.end(); it++) {
    // See how close all other elements are
    for (auto it2 = results.begin(); it2 != results.end(); ) {
      if (it == it2) {
        it2++;
        continue;
      }

      double dist = (*it - *it2).norm();

      // delete element if it's too close
      if (dist < t) {
        it2 = results.erase(it2);
      }
      else {
        it2++;
      }
    }
  }
}

void filterResults(list<Snapshot*>& results, double t) {
  // starting at the first element
  for (auto it = results.begin(); it != results.end(); it++) {
    // See how close all other elements are
    for (auto it2 = results.begin(); it2 != results.end(); ) {
      if (it == it2) {
        it2++;
        continue;
      }

      double dist = (snapshotToVector(*it) - snapshotToVector(*it2)).norm();

      // delete element if it's too close
      if (dist < t) {
        delete *it2;
        it2 = results.erase(it2);
      }
      else {
        it2++;
      }
    }
  }
}

Eigen::VectorXd snapshotToVector(Snapshot * s)
{
  // Param order: Intensity, polar, azimuth, R, G, B, Softness (penumbra angle)
  // Device order: Key, Fill, Rim, L/R indicator
  int numFeats = 7;
  Eigen::VectorXd features;
  features.resize(numFeats * 3 + 1);

  auto& device = s->getRigData();

  auto key = getSpecifiedDevice(L_KEY, s);
  auto fill = getSpecifiedDevice(L_FILL, s);
  auto rim = getSpecifiedDevice(L_RIM, s);

  // Normalize features if needed
  features[0] = key->getParam<LumiverseFloat>("intensity")->asPercent();
  features[1] = key->getParam<LumiverseOrientation>("polar")->asPercent();
  features[2] = key->getParam<LumiverseOrientation>("azimuth")->asPercent();
  features[3] = key->getParam<LumiverseColor>("color")->getColorChannel("Red");
  features[4] = key->getParam<LumiverseColor>("color")->getColorChannel("Green");
  features[5] = key->getParam<LumiverseColor>("color")->getColorChannel("Blue");
  features[6] = key->getParam<LumiverseFloat>("penumbraAngle")->asPercent();
  features[7] = fill->getParam<LumiverseFloat>("intensity")->asPercent();
  features[8] = fill->getParam<LumiverseOrientation>("polar")->asPercent();
  features[9] = fill->getParam<LumiverseOrientation>("azimuth")->asPercent();
  features[10] = fill->getParam<LumiverseColor>("color")->getColorChannel("Red");
  features[11] = fill->getParam<LumiverseColor>("color")->getColorChannel("Green");
  features[12] = fill->getParam<LumiverseColor>("color")->getColorChannel("Blue");
  features[13] = fill->getParam<LumiverseFloat>("penumbraAngle")->asPercent();
  features[14] = rim->getParam<LumiverseFloat>("intensity")->asPercent();
  features[15] = rim->getParam<LumiverseOrientation>("polar")->asPercent();
  features[16] = rim->getParam<LumiverseOrientation>("azimuth")->asPercent();
  features[17] = rim->getParam<LumiverseColor>("color")->getColorChannel("Red");
  features[18] = rim->getParam<LumiverseColor>("color")->getColorChannel("Green");
  features[19] = rim->getParam<LumiverseColor>("color")->getColorChannel("Blue");
  features[20] = rim->getParam<LumiverseFloat>("penumbraAngle")->asPercent();
  features[21] = (key->getId() == "right") ? 1e-6 : -1e-6;  // tiny little sign bit for recreating snapshot

  return features;
}

Snapshot* vectorToSnapshot(Eigen::VectorXd v)
{
  Snapshot* s = new Snapshot(getRig(), nullptr);
  auto devices = s->getRigData();

  auto key = (v[21] > 0) ? devices["right"] : devices["left"];
  auto fill = (v[21] > 0) ? devices["left"] : devices["right"];
  auto rim = devices["rim"];

  key->getParam<LumiverseFloat>("intensity")->setValAsPercent(v[0]);
  key->getParam<LumiverseOrientation>("polar")->setValAsPercent(v[1]);
  key->getParam<LumiverseOrientation>("azimuth")->setValAsPercent(v[2]);
  key->getParam<LumiverseColor>("color")->setColorChannel("Red", v[3]);
  key->getParam<LumiverseColor>("color")->setColorChannel("Green", v[4]);
  key->getParam<LumiverseColor>("color")->setColorChannel("Blue", v[5]);
  key->getParam<LumiverseFloat>("penumbraAngle")->setValAsPercent(v[6]);
  fill->getParam<LumiverseFloat>("intensity")->setValAsPercent(v[7]);
  fill->getParam<LumiverseOrientation>("polar")->setValAsPercent(v[8]);
  fill->getParam<LumiverseOrientation>("azimuth")->setValAsPercent(v[9]);
  fill->getParam<LumiverseColor>("color")->setColorChannel("Red", v[10]);
  fill->getParam<LumiverseColor>("color")->setColorChannel("Green", v[11]);
  fill->getParam<LumiverseColor>("color")->setColorChannel("Blue", v[12]);
  fill->getParam<LumiverseFloat>("penumbraAngle")->setValAsPercent(v[13]);
  rim->getParam<LumiverseFloat>("intensity")->setValAsPercent(v[14]);
  rim->getParam<LumiverseOrientation>("polar")->setValAsPercent(v[15]);
  rim->getParam<LumiverseOrientation>("azimuth")->setValAsPercent(v[16]);
  rim->getParam<LumiverseColor>("color")->setColorChannel("Red", v[17]);
  rim->getParam<LumiverseColor>("color")->setColorChannel("Green", v[18]);
  rim->getParam<LumiverseColor>("color")->setColorChannel("Blue", v[19]);
  rim->getParam<LumiverseFloat>("penumbraAngle")->setValAsPercent(v[20]);

  return s;
}

Device* getSpecifiedDevice(EditLightType l, Snapshot * s)
{
  auto devices = s->getRigData();

  // Rim is easy to identify
  if (l == L_RIM) {
    Device* d = devices["rim"];
    return d;
  }

  Device* key = devices["right"];
  Device* fill = devices["left"];

  // If right is key assumption is false, swap
  if (fill->getIntensity()->getVal() > key->getIntensity()->getVal()) {
    Device* temp = key;
    key = fill;
    fill = temp;
  }

  if (l == L_FILL)
    return fill;
  else if (l == L_KEY)
    return key;

  // If this ever happens, something's gone terribly wrong
  return nullptr;
}

//=============================================================================

list<Snapshot*> sampleScenes(list<Snapshot*> start)
{
  list<Snapshot*> newResults = runSingleLevelSearch(start);

  filterResults(newResults, 0.5);
  return newResults;
}

list<Snapshot*> runSingleLevelSearch(list<Snapshot*> startScenes)
{
  list<Snapshot*> searchResults;

  // For each scene in the initial set
  for (const auto& scene : startScenes) {
    for (const auto& edits : editConstraints) {
      list<Eigen::VectorXd> editScenes = performEdit(edits.first, scene);

      for (auto s : editScenes) {
        Snapshot* sn = vectorToSnapshot(s);

        // We evaluate the function value on demand and just save the function itself
        searchResults.push_back(sn);
      }
    }
  }

  return searchResults;
}

list<Eigen::VectorXd> performEdit(EditType t, Snapshot* orig) {
  // Determine accept parameters
  double sigma = 0.05;

  auto res = doMCMC(t, orig, 1000, sigma, true);
  delete orig;
  return res.first;
}

pair<list<Eigen::VectorXd>, int> doMCMC(EditType t, Snapshot * start, int iters, double sigma, bool saveSamples)
{
  // duplicate initial state
  Snapshot* s = new Snapshot(*start);

  // Set up return list
  list<Eigen::VectorXd> results;

  // RNG
  unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine gen(seed1);
  normal_distribution<double> gdist(0, 0.003);  // start with sdev 2
  uniform_real_distribution<double> udist(0.0, 1.0);

  // Constants
  int maxIters = iters;

  // Set up relevant feature vector
  int vecSize = editConstraints[t].size();
  Eigen::VectorXd x;
  x.resize(vecSize);

  int i = 0;
  for (const auto& c : editConstraints[t]) {
    x[i] = getDeviceValue(c, s);
    i++;
  }

  // diagnostics
  int accepted = 0;
  Snapshot* sp = new Snapshot(*start);

  for (int i = 0; i < maxIters; i++) {
    // generate candidate x'
    Eigen::VectorXd xp = x;

    // displace by gaussian dist
    for (int j = 0; j < xp.size(); j++) {
      xp[j] += gdist(gen);
      // The next line acts as a physically based clamp function of sorts.
      // It updates the lighting scene and also returns the value of the parameter after the update.
      xp[j] = setDeviceValue(editConstraints[t][j], t, xp[j], sp);
    }

    // check for acceptance
    double a = 1;

    // accept if a >= 1 or with probability a
    if (a >= 1 || udist(gen) <= a) {
      // save sample in list
      results.push_back(snapshotToVector(sp));
      // update x
      x = xp;
      accepted++;
    }
  }

  if (s != nullptr)
    delete s;
  if (s != sp && sp != nullptr)
    delete sp;

  // filter results
  filterResults(results, 0.25);

  return pair<list<Eigen::VectorXd>, int>(results, accepted);
}

double setDeviceValue(EditConstraint c, EditType t, double val, Snapshot * s)
{
  Device* d = getSpecifiedDevice(c._light, s);

  switch (c._param) {
  case INTENSITY:
  {
    d->getIntensity()->setValAsPercent(val);

    if (c._light == L_KEY) {
      if (t == KEY_INTENS_RIM_CONTRAST_MATCH) {
        Device* rim = getSpecifiedDevice(L_RIM, s);
        double r = rim->getIntensity()->asPercent() / d->getIntensity()->asPercent();
        rim->getIntensity()->setValAsPercent(val * r);
      }
      if (t == KEY_INTENS_FILL_CONTRAST_MATCH) {
        Device* fill = getSpecifiedDevice(L_FILL, s);
        double r = fill->getIntensity()->asPercent() / d->getIntensity()->asPercent();
        fill->getIntensity()->setValAsPercent(val * r);
      }
    }

    return d->getIntensity()->asPercent();
  }
  case HUE:
  {
    val *= 360;
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(val, hsv[1], hsv[2]);

    if (c._light == L_KEY) {
      if (t == KEY_HUE_FILL_HUE_MATCH || t == KEY_HUE_FILL_RIM_HUE_MATCH) {
        auto fill = getSpecifiedDevice(L_FILL, s);
        Eigen::Vector3d fhsv = fill->getColor()->getHSV();
        fill->getColor()->setHSV(fhsv[0] + (val - hsv[0]), fhsv[1], fhsv[2]);
      }
      if (t == KEY_HUE_FILL_RIM_HUE_MATCH) {
        auto rim = getSpecifiedDevice(L_RIM, s);
        Eigen::Vector3d rhsv = rim->getColor()->getHSV();
        rim->getColor()->setHSV(rhsv[0] + (val - hsv[0]), rhsv[1], rhsv[2]);
      }
    }

    return d->getColor()->getHSV()[0] / 360.0;
  }
  case SAT:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], val, hsv[2]);
    return d->getColor()->getHSV()[1];
  }
  case VALUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], hsv[1], val);
    return d->getColor()->getHSV()[2];
  }
  case RED:
    d->getColor()->setColorChannel("Red", val);
    return d->getColor()->getColorChannel("Red");
  case BLUE:
    d->getColor()->setColorChannel("Blue", val);
    return d->getColor()->getColorChannel("Blue");
  case GREEN:
    d->getColor()->setColorChannel("Green", val);
    return d->getColor()->getColorChannel("Green");
  case POLAR:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("polar");
    o->setValAsPercent(val);

    if (c._light == L_KEY && t == KEY_POS_FILL_POS_MATCH) {
      auto fill = getSpecifiedDevice(L_FILL, s);
      fill->getParam<LumiverseOrientation>("polar")->setValAsPercent(-val);
    }

    return o->asPercent();
  }
  case AZIMUTH:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("azimuth");
    o->setValAsPercent(val);

    if (c._light == L_KEY && t == KEY_POS_FILL_POS_MATCH) {
      auto fill = getSpecifiedDevice(L_FILL, s);
      fill->getParam<LumiverseOrientation>("polar")->setValAsPercent(val);
    }

    return o->asPercent();
  }
  case SOFT:
  {
    LumiverseFloat* s = d->getParam<LumiverseFloat>("penumbraAngle");
    s->setValAsPercent(val);
    return s->asPercent();
  }
  default:
    return 0;
  }
}

double getDeviceValue(EditConstraint c, Snapshot * s)
{
  Device* d = getSpecifiedDevice(c._light, s);

  switch (c._param) {
  case INTENSITY:
    return d->getIntensity()->asPercent();
  case HUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[0] / 360.0;
  }
  case SAT:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[1];
  }
  case VALUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[2];
  }
  case RED:
    return d->getColor()->getColorChannel("Red");
  case BLUE:
    return d->getColor()->getColorChannel("Blue");
  case GREEN:
    return d->getColor()->getColorChannel("Green");
  case POLAR:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("polar");
    return o->asPercent();
  }
  case AZIMUTH:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("azimuth");
    return o->asPercent();
  }
  case SOFT:
  {
    LumiverseFloat* s = d->getParam<LumiverseFloat>("penumbraAngle");
    return s->asPercent();
  }
  default:
    break;
  }
}
