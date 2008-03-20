#include <termios.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#include "gainer.h"
#include <iomanip>

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

void *Gainer::receiver(void *arg) {
  Gainer *self(reinterpret_cast<Gainer *>(arg));

  struct timeval timeout;
  timeout.tv_sec  = 1;
  timeout.tv_usec = 0;

  while (not self->end_) {
    DEBUG_PRINT("next_event...");

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(self->io_, &fds);

    int ret(select(self->io_+1, &fds, NULL, NULL, &timeout));
    if (0 != ret) {
      //DEBUG_PRINT("receiver: selected");
      const size_t SIZE(80);
      char buf[SIZE];
      read(self->io_, buf, SIZE);
      std::string s(buf);

      self->process_event(s);
    }
  }
  return NULL;
}

namespace Gainer {
  Gainer::Gainer(const std::string &path, int config) :
    led_(false)
    , config_(config)
    , end_(false)
    , on_pressed(NULL)
    , on_released(NULL)
  {
    ABORT_UNLESS(-1 != (io_ = open(path.c_str(), O_RDWR)));
    ABORT_UNLESS(-1 != fsync(io_));
    setup_port();
    pthread_create(&thread_, NULL, receiver, this);
    reboot();
    set_configuration(config_);
  }

  Gainer::~Gainer() {
    end_ = true;
    pthread_join(thread_, NULL);
    close(io_);
  }

  void Gainer::setup_port() {
    struct termios term;
    ABORT_UNLESS(0 == tcgetattr(io_, &term));
    ABORT_UNLESS(0 == cfsetispeed(&term, B38400));
    ABORT_UNLESS(0 == cfsetospeed(&term, B38400));
    term.c_cflag |= CS8;
    ABORT_UNLESS(0 == tcsetattr(io_, TCSANOW, &term));
  }

  void Gainer::set_configuration(int number) {
    if (1 > number or number > 7)
      throw GainerInternal::Exception();

    config_ = number;
    std::stringstream ss("");
    ss << "KONFIGURATION_" << number;
    command(ss.str(), 1);

    for (int i(0); i<CONFIG[config_][AIN]; i++) // create analog inputs
      analog_inputs.push_back(0);

    for (int i(0); i<CONFIG[config_][DIN]; i++) // create digital inputs
      digital_inputs.push_back(false);
  }

  // void Gainer::set_matrix(ary) // TODO

  void Gainer::set_led(bool flag)          { command(flag ? "h" : "l"); }
  void Gainer::reboot()                    { command("Q", 1); }
  void Gainer::peek_digital_inputs()       { command("R"); }
  void Gainer::peek_analog_inputs()        { command("I"); }
  void Gainer::continuous_digital_inputs() { command("r"); }
  void Gainer::continuous_analog_inputs()  { command("i"); }
  void Gainer::exit_continuos()            { command("E"); }

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
    sleep(wait);
  }

  void Gainer::command_send(const std::string &cmd) {
    write(io_, cmd.c_str(), cmd.size());
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
      case 'i':
      case 'I': { // analog_input
                  std::string::size_type ast(event.find('*'));
                  std::string s(event.substr(1, ast-1));
                  sscanf(s.c_str(), "%02X%02X%02X%02X*",
                      &analog_inputs[0], &analog_inputs[1],
                      &analog_inputs[2], &analog_inputs[3]);
                  break;
                }
      case 'r':
      case 'R': { // digital input
                  std::string::size_type ast(event.find('*'));
                  std::stringstream ss(event.substr(1, ast-1));
                  int num;
                  ss >> std::hex >> num;
                  for (size_t i(0); i<digital_inputs.size(); i++)
                    digital_inputs[i] = num & (1<<i);
                  break;
                }
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
