
#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>
#include "sampleTools.h"
#include <stdio.h>
#include <dirent.h>
#include <sstream>
#include <fstream>

using namespace std;

// Input: Path to folder containing .rig.json files to be rendered
// Output: PNGs placed in the dest folder (can be the same location)
// note - for resuming projgress, just remove completed files from the
// directory being read.
int main(int argc, char** argv) {
  string source = string(argv[1]);
  string dest = string(argv[2]);

  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (source.c_str())) != NULL) {
    // If file is json, we assume we can load it.
    // We then load and sample the scene
    while ((ent = readdir (dir)) != NULL) {
      string fname = string(ent->d_name);
      if (fname.find(".rig.json") != string::npos) {
        cout << "Loading " + source + "/" +  fname << "\n";
        getRig()->load(source + "/" +fname);
	getRig()->init();

        ArnoldAnimationPatch* p = nullptr;
        for (const auto& kvp : getRig()->getPatches()) {
          if (kvp.second->getType() == "ArnoldAnimationPatch") {
            p = (ArnoldAnimationPatch*)kvp.second;
          }
        }

        if (p == nullptr)
          continue;

        // get the first part of the rig filename
        string n = fname.substr(0, fname.find(".rig.json"));

        // render settings
        p->setSamples(2);
        p->setDims(1280, 720);
        p->renderSingleFrame(getRig()->getDeviceRaw(), dest, n);
        p->close();
        cout << "Rendered " << n << ".png\n"; 
      }
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
  }

  getRig()->stop();
}
