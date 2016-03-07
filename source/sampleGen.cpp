#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>
#include "sampleTools.h"
#include <stdio.h>
#include <dirent.h>

using namespace std;

// Input: Start number, Path to folder containing rig .json files, path to output folder
// Will take those files and sample randomly using edits using those configurations
// as starting points for the random sampling
// Output: Feature vectors files (.csv) and .rig.json files containing Lumiverse
// configurations for those feature vectors. One per generated scene.
int main(int argc, char** argv) {
  int start = stoi(string(argv[1]));
  string source = string(argv[2]);
  string dest = string(argv[3]);

  list<Snapshot*> scenesToSample;

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (source.c_str())) != NULL) {
    // If file is json, we assume we can load it.
    // We then load and sample the scene
    while ((ent = readdir (dir)) != NULL) {
      string fname = string(ent->d_name);
      if (fname.find(".json") != string::npos) {
        cout << source + "/" +  fname << "\n";
        getRig()->load(source + "/" +fname);
	getRig()->init();

        Snapshot* s = new Snapshot(getRig(), nullptr);
        scenesToSample.push_back(s);
        auto dat = s->getRigData();
        cout << getRig()->getDevice("left")->getIntensity()->asPercent() << "\n";
      }
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
  }

  cout << scenesToSample.size() << "\n";
  list<Snapshot*> samples = sampleScenes(scenesToSample);
  cout << samples.size() << "\n";

}
