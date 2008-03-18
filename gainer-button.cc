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
  //gainer->peek_digital_inputs();
  //gainer->peek_analog_inputs();
}

static void on_released() {
  cout << "released" << endl;

  cout << "digital : ";
  for (size_t i(0); i<gainer->digital_inputs.size(); i++) {
    cout << gainer->digital_inputs[i] << ' ';
  } cout << endl;

  cout << "analog : ";
  for (size_t i(0); i<gainer->analog_inputs.size(); i++) {
    cout << gainer->analog_inputs[i] << ' ';
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
    std::cerr << "process_next_event" << std::endl;
    //gainer->process_next_event();
    gainer->peek_analog_inputs();
    usleep(100000);
    /*
    gainer->peek_analog_inputs();
    gainer->peek_digital_inputs();
    */

    cout << "A: ";
    for (size_t i(0); i<gainer->analog_inputs.size(); i++) {
      cout << gainer->analog_inputs[i] << ' ';
    } cout << endl;

    cout << "D: ";
    for (size_t i(0); i<gainer->digital_inputs.size(); i++) {
      cout << gainer->digital_inputs[i] << ' ';
    } cout << endl;
  }

  delete gainer;

  return 0;
}

