/*
 * application example
 *   from http://8-p.info/gainer-ruby/
 */
#include <iostream>
#include "gainer.h"

using namespace std;

static void on_pressed() {
  cout << "pressed" << endl;
}

static void on_released() {
  cout << "released" << endl;
}

int main(int argc, char **argv) {
  Gainer::Serial gainer(argv[1]);
  gainer.set_on_pressed(on_pressed);
  gainer.set_on_released(on_released);

  while (true) {
    std::cerr << "process_next_event" << std::endl;
    gainer.process_next_event();
  }

  return 0;
}

