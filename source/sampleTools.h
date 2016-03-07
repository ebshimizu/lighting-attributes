#ifndef ATTRIBUTESEARCH_H_INCLUDED
#define ATTRIBUTESEARCH_H_INCLUDED

#include <functional>
#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>
#include <list>

using namespace std;
using namespace Lumiverse;
using namespace Lumiverse::ShowControl;

Rig* getRig();

enum EditType {
  // Normal edits - edits that manipulate specific lighting parameters
  ALL,
  ALL_HSV,
  ALL_RGB,
  ALL_SAT,
  ALL_HUE,
  ALL_INTENS,
  ALL_POS,
  KEY_HUE,
  FILL_HUE,
  RIM_HUE,
  KEY_INTENS,
  FILL_INTENS,
  RIM_INTENS,
  KEY_POS,
  FILL_POS,
  RIM_POS,
  KEY_SAT,
  FILL_SAT,
  RIM_SAT,
  KEY_HSV,
  FILL_HSV,
  RIM_HSV,
  KEY_SOFT,
  FILL_SOFT,
  RIM_SOFT,
  ALL_SOFT,
  KEY_FILL_INTENS,
  KEY_RIM_INTENS,
  FILL_RIM_INTENS,
  KEY_FILL_HUE,
  KEY_FILL_SAT,
  KEY_FILL_HSV,
  KEY_FILL_POS,
  KEY_RIM_HSV,
  KEY_RIM_POS,
  KEY_RGB,
  FILL_RGB,
  RIM_RGB,
  // Special edits - edits that require additional constraints to be matched
  // when they are performed.
  KEY_POS_FILL_POS_MATCH,             // Move the key but maintain fill relative position
  KEY_INTENS_RIM_CONTRAST_MATCH,      // Adjust key intensity but keep key-rim contrast constant
  KEY_INTENS_FILL_CONTRAST_MATCH,     // Adjust key intensity but keep key-fill contrast constant
  KEY_HUE_FILL_HUE_MATCH,             // Adjust key color, apply same relative change to fill color (rotate around color wheel basically)
  KEY_HUE_FILL_RIM_HUE_MATCH,         // Adjust key color, apply same relative change to rim/fill color
                                      // Special type for GUI
  CLUSTER_CENTER
};

// Don't want to use strings for this for speed reasons
enum EditLightType {
  L_KEY,
  L_FILL,
  L_RIM
};

// controllable lighting parameters. Split here since don't want to waste time
// parsing strings for things like color.hue
enum EditParam {
  INTENSITY,
  HUE,
  SAT,
  VALUE,
  RED,
  GREEN,
  BLUE,
  POLAR,
  AZIMUTH,
  SOFT
};

// these constraints define an edit (or rather, which parameters an edit can deal with)
// Some more uncommon edits may have additional constraints (maintain position of
// fill for example) and will be treated separately
struct EditConstraint {
  EditConstraint(EditLightType t, EditParam p) : _light(t), _param(p) { }

  EditLightType _light;
  EditParam _param;
};

// Remove vectors from the set that are within a specified threshold of other elements
// in the set
void filterResults(list<Eigen::VectorXd>& results, double t);

void filterResults(list<Snapshot*>& results, double t);

// Returns a vector representation of the rig state contained in the snapshot
// Order of parameters detailed in implementation
Eigen::VectorXd snapshotToVector(Snapshot* s);

// Returns a snapshot consisting of the state contained in the vector
Snapshot* vectorToSnapshot(Eigen::VectorXd v);

// Given an EditLightType, get the corresponding light in the rig
Device* getSpecifiedDevice(EditLightType l, Snapshot* s);

list<Snapshot*> sampleScenes(list<Snapshot*> start);

// Runs a single level iteration of the search algorithm, starting at the given scenes.
list<Snapshot*> runSingleLevelSearch(list<Snapshot*> startScenes);

// Given a current configuration, use MCMC to perform an edit on the configuration
list<Eigen::VectorXd> performEdit(EditType t, Snapshot* orig);

// Do MCMC with the given parameters. Returns the list of samples and number of accepted samples.
// Samples list will be empty if saveSamples is false.
pair<list<Eigen::VectorXd>, int> doMCMC(EditType t, Snapshot* start, int iters, double sigma, bool saveSamples);

// updates the value for a Lumiverse parameter and returns the actual value after the update
double setDeviceValue(EditConstraint c, EditType t, double val, Snapshot* s);

// Retrieves the current value for a Lumiverse parameter
double getDeviceValue(EditConstraint c, Snapshot* s);

#endif  // ATTRIBUTESEARCH_H_INCLUDED
