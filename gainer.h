#include <string>
#include <vector>

/**
 * Gainer C++ wrapper
 */
namespace Gainer {
class Port;

/**
 * Serial I/O
 */
class Serial {
public:
  typedef void (*callback_t)(void);
  enum pin_t { AIN=0, DIN, AOUT, DOUT };
  static const int CONFIG[][4];
  static const int MATRIX_LED_CONFIGURATION = 7;

  Serial(const std::string &path, int config=1);

  void set_led(bool flag);
  // void set_matrix(ary);
  void peek_digital_input();
  void peek_analog_input();
  void set_digital_outputl(int n);
  void process_next_event(int wait = 0);
  void set_on_pressed(callback_t funcp) {
    on_pressed = funcp;
  }
  void set_on_released(callback_t funcp) {
    on_released = funcp;
  }

private:
  void setup_port();
  void reboot();
  void set_configuration(int number);
  void command(const std::string &cmd, int wait=0);
  void command_send(const std::string &cmd);
  void process_event(std::string &event);
  std::string next_event();

  bool led_;
  std::vector<Port> analog_inputs, digital_inputs;
  std::vector<Port> analog_outputs, digital_outputs;

  int config_;
  callback_t on_pressed, on_released;
  int io_;
}; // Serial


class Port {
public:
  Port(Serial &b) : instance(b) {}

private:
  Serial &instance;
}; // Port

class Exception {
}; // Exception
} // Gainer
