/*
 * application example
 *   from http://8-p.info/gainer-ruby/
 */
#include <iostream>
#include "gainer.h"

using namespace std;

Gainer *gainer;

static void on_pressed() {
  cout << "pressed" << endl;
}

static void on_released() {
  cout << "released" << endl;

#if 0
  cout << "digital : ";
  for (size_t i(0); i<gainer->digital_inputs.size(); i++) {
    cout << gainer->digital_inputs[i] << ' ';
  } cout << endl;

  cout << "analog : ";
  for (size_t i(0); i<gainer->analog_inputs.size(); i++) {
    cout << gainer->analog_inputs[i] << ' ';
  } cout << endl;
#endif // 0
    cout << "A: ";
    for (size_t i(0); i<gainer->analog_inputs.size(); i++) {
      cout << gainer->analog_inputs[i] << ' ';
    } cout << endl;

    cout << "D: ";
    for (size_t i(0); i<gainer->digital_inputs.size(); i++) {
      cout << gainer->digital_inputs[i] << ' ';
    } cout << endl;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: ./gainer-button [device]" << endl;
    exit(1);
  }
  gainer = new Gainer(argv[1], 1);
  gainer->set_on_pressed(on_pressed);
  gainer->set_on_released(on_released);

  while (true) {
    //gainer->peek_analog_inputs();
    gainer->peek_digital_inputs();
    sleep(1);
    /*
    gainer->peek_analog_inputs();
    gainer->peek_digital_inputs();
    */
  }

  delete gainer;

  return 0;
}

