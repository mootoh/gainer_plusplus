/*
 * application example
 *   from http://8-p.info/gainer-ruby/
 */
#include <iostream>
#include "gainer.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: ./gainer-button [device]" << endl;
    exit(1);
  }

  Gainer gainer(Gainer(argv[1], 1));
  gainer.continuous_digital_inputs();

  for (int i(0); i<10; i++) {
    usleep(100000);

    cout << "D: ";
    for (size_t j(0); j<gainer.digital_inputs.size(); j++) {
      cout << gainer.digital_inputs[j] << ' ';
    } cout << endl;

    if (5 == i) 
      gainer.exit_continuos();
  }
  sleep(1);

  return 0;
}

