#include <LumiverseCore.h>
#include <LumiverseShowControl/LumiverseShowControl.h>
#include "sampleTools.h"

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


}