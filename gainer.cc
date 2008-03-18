#include <termios.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <sys/select.h>
#include "gainer.h"
#include <cassert>

#define ABORT_UNLESS(stmt) \
if (!((stmt))) { \
  std::stringstream msg(""); \
  msg << "failed in L" <<  __LINE__ << ": "; \
  perror(msg.str().c_str()); \
  abort(); \
}

#ifdef DEBUG
#define DEBUG_PRINT(msg) std::cerr << msg << std::endl;
#else
#define DEBUG_PRINT(msg) ;
#endif // DEBUG

namespace Gainer {

Gainer::Gainer(const std::string &path, int config) :
    led_(false)
  , config_(config)
  , on_pressed(NULL)
  , on_released(NULL)
{
  ABORT_UNLESS(-1 != (io_ = open(path.c_str(), O_RDWR)));
  ABORT_UNLESS(-1 != fsync(io_));
  setup_port();
  reboot();
  set_configuration(config_);
}

Gainer::~Gainer() {
  close(io_);
}

void Gainer::set_led(bool flag) {
  command(flag ? "h" : "l");
}

void Gainer::setup_port() {
  struct termios term;
  ABORT_UNLESS(0 == tcgetattr(io_, &term));
  ABORT_UNLESS(0 == cfsetispeed(&term, B38400));
  ABORT_UNLESS(0 == cfsetospeed(&term, B38400));
  term.c_cflag |= CS8;
  ABORT_UNLESS(0 == tcsetattr(io_, TCSANOW, &term));
}

void Gainer::reboot() {
  command("Q", 1);
}

void Gainer::set_configuration(int number) {
  if (1 > number or number > 7)
    throw GainerInternal::Exception();

  config_ = number;
  std::stringstream ss("");
  ss << "KONFIGURATION_" << number;
  command(ss.str(), 1);

  // create analog inputs
  for (int i(0); i<CONFIG[config_][AIN]; i++) {
    analog_inputs.push_back(0);
  }

  // create digital inputs
  for (int i(0); i<CONFIG[config_][DIN]; i++) {
    digital_inputs.push_back(false);
  }
}

// void Gainer::set_matrix(ary)


void Gainer::peek_digital_inputs() {
  command("R");
}

void Gainer::peek_analog_inputs() {
  command("I");
}

void Gainer::set_digital_output(int n) {
  std::stringstream ss("D");
  for (int i(0); i< CONFIG[config_][DOUT]-1; i++) {
    ss << ' ';
  }
  ss << n;
  command(ss.str());
}

void Gainer::command(const std::string &cmd, int wait) {
  command_send(cmd + '*');
  process_next_event(wait);
}

void Gainer::process_next_event(int wait) {
  //std::cerr << "process_next_event" << std::endl;
  std::string reply(next_event());
  sleep(wait);
  process_event(reply);
}

void Gainer::command_send(const std::string &cmd) {
  write(io_, cmd.c_str(), cmd.size());
}

std::string Gainer::next_event() {
  while (true) {
    std::cerr << "next_event..." << std::endl;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(io_, &fds);

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int ret(select(io_+1, &fds, NULL, NULL, &timeout));
    if (0 != ret) {
      const size_t SIZE(80);
      char buf[SIZE];
      read(io_, buf, SIZE);
      return buf;
    }
  }
  return "";
}

void Gainer::process_event(std::string &event) {
  DEBUG_PRINT("event: " << event);
  switch(event[0]) {
    case '!': // something wrong
      throw GainerInternal::Exception();
    case 'h': // led on
      led_ = true;
      break;
    case 'l': // led off
      led_ = false;
      break;
    case 'N': // button pressed
      on_pressed();
      break;
    case 'F': // button released
      on_released();
      break;
    case 'I': // analog_input
      for (int i(1); ; i++) {
        char ch(event[i]);
        if (isdigit(ch) or ('A' <= ch and 'F' >= ch))
          digital_inputs[i-1] = ch-'0';
        else
          break;
      }
      break;
    case 'R': // digital input
      for (int i(1); ; i++) {
        char ch(event[i]);
        if (isdigit(ch) or ('A' <= ch and 'F' >= ch))
          digital_inputs[i-1] = ch-'0';
        else
          break;
      }
      break;
    default:
      break;
  }
}

const int Gainer::CONFIG[][4] = {
  // N_AIN, N_DIN, N_AOUT, N_DOUT
  {0,},         // dummy
  {4, 4, 4, 4}, // 1
  {8, 0, 4, 4}, // 2
  {4, 4, 8, 0}, // 3
  {8, 0, 8, 0}, // 4
  {0,16, 0, 0}, // 5
  {0, 0, 0,16}, // 6
  {0,}          // 7 for matrix LED
};

} // Gainer
