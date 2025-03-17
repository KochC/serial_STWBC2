#ifndef __STWBC2__
#define __STWBC2__

#define STWBC2_HEADER 0x54
#define STWBC2_TYPE_MONITOR 0xB3
#define STWBC2_MAX_MESSAGE_SIZE 256 // Maximum message size

#include <iostream>

namespace STWBC2 {

struct Data {
  uint8_t state;
  uint32_t frequency;
  uint16_t controlError;
  double dutyCycle;
  double bridgeVoltage;
  double rxPower;
  double inputVoltage;
};

Data data;

Stream *serialPort;

// Message collection variables
uint8_t messageBuffer[STWBC2_MAX_MESSAGE_SIZE];
int messageLength = 0;
bool collectingMessage = false;

void init(Stream *serialPort) { STWBC2::serialPort = serialPort; }

enum StState {
  NO_STATE = 0x00,
  SELECTION_AP_STATE = 0x01,
  SELECTION_QF_STATE = 0x02,
  FSM_PRE_PING_STATE = 0x03,
  FSM_PING_STATE = 0x04,
  CONFIG_CHECK_ID_STATE = 0x05,
  CONFIG_CHECK_EXT_ID_STATE = 0x06,
  CONFIG_CHECK_CONF_STATE = 0x07,
  NEGOTIATION_STATE = 0x08,
  RENEGOTIATION_STATE = 0x09,
  CALIBRATION_STATE = 0x0A,
  POWER_TRANSFER_STATE = 0x0B,
  POWER_PID_STATE = 0x0C
};

uint8_t resolve_ss(uint8_t ss) {
  return ss;
}

uint32_t resolve_frequency(uint8_t byte1, uint8_t byte2, uint8_t byte3,
                           uint8_t byte4) {
  // Combine the 4 bytes into a 32-bit value
  uint32_t frequencyValue = (static_cast<uint32_t>(byte4) << 24) |
                            (static_cast<uint32_t>(byte3) << 16) |
                            (static_cast<uint32_t>(byte2) << 8) |
                            static_cast<uint32_t>(byte1);
  return frequencyValue;
}

uint16_t resolve_control_error(uint8_t byte1) {
  uint16_t controlError = static_cast<uint16_t>(byte1);
  return controlError;
}

double resolve_duty_cycle(uint8_t byte1) {
  double dutyCycle = static_cast<uint16_t>(byte1) / 100.0;
  return dutyCycle;
}

double resolve_bridge_voltage(uint8_t byte1, uint8_t byte2) {
  double bridgeVoltage =
      ((static_cast<uint16_t>(byte2) << 8) | static_cast<uint16_t>(byte1)) /
      1000.0;
  return bridgeVoltage;
}

double resolve_rx_power(uint8_t byte1, uint8_t byte2) {
  double rxPower =
      ((static_cast<uint16_t>(byte2) << 8) | static_cast<uint16_t>(byte1)) /
      1000.0;
  return rxPower;
}

double resolve_input_voltage(uint8_t byte1, uint8_t byte2) {
  double inputVoltage =
      ((static_cast<uint16_t>(byte2) << 8) | static_cast<uint16_t>(byte1)) /
      1000.0;
  return inputVoltage;
}

void resolve_message(uint8_t *message, int length) {
  switch (message[1]) {
  case STWBC2_TYPE_MONITOR:
    data.state = resolve_ss(message[3]);
    data.frequency =
        resolve_frequency(message[4], message[5], message[6], message[7]);
    data.controlError = resolve_control_error(message[8]);
    data.dutyCycle = resolve_duty_cycle(message[9]);
    data.bridgeVoltage = resolve_bridge_voltage(message[10], message[11]);
    data.rxPower = resolve_rx_power(message[12], message[13]);
    data.inputVoltage = resolve_input_voltage(message[16], message[17]);
    break;
  default:
    // std::cout << "Type 0x" << std::hex << static_cast<int>(message[1]);
    // std::cout << ", Length=" << std::dec << static_cast<int>(message[2]);
    // std::cout << ", Data=";
    break;
  }
}

void loop() {
  if (serialPort->available()) {
    char c = serialPort->read();

    // If we find an ST_HEADER, start a new message
    if (c == STWBC2_HEADER) {
      // Start a new message
      messageLength = 0;
      messageBuffer[messageLength++] = c;
      collectingMessage = true;
    }
    // Add to the current message if we're collecting
    else if (collectingMessage) {
      // Add to buffer if there's space
      if (messageLength < STWBC2_MAX_MESSAGE_SIZE - 1) {
        messageBuffer[messageLength++] = c;

        // If we have at least 3 bytes (header, type, length) and have received
        // the complete message
        if (messageLength >= 3) {
          uint8_t messageType = messageBuffer[1];
          uint8_t expectedLength =
              messageBuffer[2] + 3; // +3 for header, type, and length bytes

          // If we've received the complete message
          if (messageLength >= expectedLength) {
            resolve_message(messageBuffer, messageLength);
            collectingMessage = false;
            messageLength = 0;
          }
        }
      }
      // If buffer is full, print the message and reset
      else {
        std::cout << "\n-> Message buffer full, printing collected data:"
                  << std::endl;
        std::cout.write(reinterpret_cast<char *>(messageBuffer), messageLength);
        messageLength = 0;
        collectingMessage = false;
      }
    }
    // If not collecting a message, just pass through the character
    else {
      std::cout << std::hex << static_cast<int>(c);
    }
  }
}

} // namespace STWBC2

#endif // __STWBC2__
