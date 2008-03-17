#include <termios.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <sys/select.h>
#include "gainer.h"

namespace Gainer {

Serial::Serial(const std::string &path, int config) :
    led_(false)
  , config_(config)
  , on_pressed(NULL)
  , on_released(NULL)
{
  io_ = open(path.c_str(), O_RDWR);
  fsync(io_);
  setup_port();
  reboot();
  set_configuration(config_);
}

void Serial::set_led(bool flag) {
  command(flag ? "h" : "l");
}

void Serial::setup_port() {
  struct termios term;
  tcgetattr(io_, &term);
  cfsetispeed(&term, B38400);
  cfsetospeed(&term, B38400);
  term.c_cflag |= CS8;
  tcsetattr(io_, TCSANOW, &term);
}

void Serial::reboot() {
  command("Q", 1);
}

void Serial::set_configuration(int number) {
  if (1 <= number and number <= 7) {
    config_ = number;
    std::stringstream ss("");
    ss << "KONFIGURATION_" << number;
    command(ss.str(), 1);
  }
}

// void Serial::set_matrix(ary)


void Serial::peek_digital_input() {
  command("R");
}

void Serial::peek_analog_input() {
  command("I");
}

void Serial::set_digital_outputl(int n) {
  std::stringstream ss("D");
  for (int i(0); i< CONFIG[config_][DOUT]-1; i++) {
    ss << ' ';
  }
  ss << n;
  command(ss.str());
}

void Serial::command(const std::string &cmd, int wait) {
  command_send(cmd + '*');
  process_next_event(wait);
}

void Serial::process_next_event(int wait) {
  std::string reply(next_event());
  sleep(wait);
  process_event(reply);
}

void Serial::command_send(const std::string &cmd) {
  write(io_, cmd.c_str(), cmd.size());
}

std::string Serial::next_event() {
  while (true) {
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

void Serial::process_event(std::string &event) {
}

const int Serial::CONFIG[][4] = {
  {0,},
  {4, 4, 4, 4},
  {8, 0, 4, 4},
  {4, 4, 8, 0},
  {8, 0, 8, 0},
  {0,16, 0, 0},
  {0, 0,16, 0}
};

} // Gainer
