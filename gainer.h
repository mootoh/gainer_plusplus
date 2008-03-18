/*
 * Gainer C++ wrapper
 */
#include <string>
#include <vector>

class Gainer;

namespace GainerInternal {
  class Exception {
  }; // Exception
} // GainerInternal

/**
 * Gainer Serial I/O.
 */
class Gainer {
public:
  //! event handler calback
  typedef void (*callback_t)(void);

  /**
   * @brief setup Gainer
   * @param path  device path (/dev/cu.usbserial*)
   * @param config configutation mode (1-7)
   */
  Gainer(const std::string &path, int config=1);
  ~Gainer();

  void set_led(bool flag); //! set led state (true: on/ false:off)

  // void set_matrix(ary);

  void peek_digital_inputs(); //! capture all digital inputs
  void peek_analog_inputs();   //! capture all analog inputs

  void set_digital_output(int n);
  void set_analog_output(int n);

  void process_next_event(int wait = 0);

  void set_on_pressed(callback_t funcp) { on_pressed = funcp; }
  void set_on_released(callback_t funcp) { on_released = funcp; }

  // properties
  bool led_;
  std::vector<int> analog_inputs;
  std::vector<bool> digital_inputs;

private:
  void setup_port();
  void reboot();
  void set_configuration(int number);
  void command(const std::string &cmd, int wait=0);
  void command_send(const std::string &cmd);
  void process_event(std::string &event);
  std::string next_event();

  int config_;
  int io_;
  callback_t on_pressed, on_released;

  enum pin_t {AIN=0, DIN, AOUT, DOUT};
  static const int CONFIG[][4];
  static const int MATRIX_LED_CONFIGURATION = 7;
}; // Gainer

