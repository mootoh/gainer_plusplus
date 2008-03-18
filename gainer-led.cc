/**
 * application example
 *   from http://8-p.info/gainer-ruby/
 */
#include "gainer.h"

int main(int argc, char **argv) {
  Gainer gainer(argv[1]);

  gainer.set_led(true);
  sleep(1);
  gainer.set_led(false);

  return 0;
}

