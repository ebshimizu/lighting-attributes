#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>
#include "sampleTools.h"
#include <stdio.h>
#include <dirent.h>
#include <sstream>
#include <fstream>

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
      }
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
  }

  list<Snapshot*> samples = sampleScenes(scenesToSample);

  for (Snapshot* s : samples) {
    stringstream ss;
    ss << dest << "/" << start;
    string baseFile = ss.str();

    // Write rig file
    s->loadSnapshot(getRig(), nullptr);
    getRig()->save(baseFile + ".rig.json", true);

    // Write feature vector file
    ofstream csv;
    csv.open(baseFile + ".csv", ofstream::out | ofstream::trunc);
    auto vec = snapshotToVector(s);
    bool first = true;
    for (int i = 0; i <  vec.size() ; i++) {
      if (!first)
        csv << ",";
      first = false;
      csv << vec[i];
    }
    csv.close();
    start++;
  }

}
