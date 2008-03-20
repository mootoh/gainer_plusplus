/*
 * continuous analog input read
 */
#include <iostream>
#include "gainer.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: ./gainer-cain [device]" << endl;
    exit(1);
  }

  Gainer gainer(Gainer(argv[1], 1));
  gainer.continuous_analog_inputs();
  cout << "hoge" << endl;

  for (int i(0); i<100; i++) {
    cout << "A: ";
    /*
    for (size_t j(0); j<gainer.analog_inputs.size(); j++) {
      cout << gainer.analog_inputs[j] << ' ';
    } cout << endl;
    */
    cout << gainer.analog_inputs[2]-127 << ' ';
    cout << endl;

    //if (5 == i) 
      //gainer.exit_continuos();
    usleep(100000);
  }
  sleep(1);

  return 0;
}

