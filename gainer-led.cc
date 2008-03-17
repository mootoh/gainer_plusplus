/**
 * application example
 *   from http://8-p.info/gainer-ruby/
 */
#include "gainer.h"

using namespace std;

int main(int argc, char **argv) {
  Gainer::Serial gainer(argv[1]);

  gainer.set_led(true);
  sleep(1);
  gainer.set_led(false);

  return 0;
}

