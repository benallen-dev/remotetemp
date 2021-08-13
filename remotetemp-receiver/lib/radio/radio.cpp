#include <NRFLite.h>
#include <SPI.h>

#include <pinout.h>

extern float temperature;

const static uint8_t RADIO_ID = 0;
const static uint8_t DESTINATION_RADIO_ID = 1;

enum RadioPacketType {
  Heartbeat,
  BeginGetData,
  EndGetData,
  ReceiverData
};

struct RadioPacket {
  RadioPacketType PacketType;
  uint8_t FromRadioId;
  float temperature;
};

NRFLite _radio;
uint32_t _lastHeartbeatSendTime, _lastMessageRequestTime;

void setupRadio() {
  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1)
      ;  // Wait here forever.
  }
}

void updateRadio() {
  // Send a heartbeat once every second.
  if (millis() - _lastHeartbeatSendTime > 999) {
    _lastHeartbeatSendTime = millis();

    Serial.print("Sending heartbeat");
    RadioPacket radioData;
    radioData.PacketType = Heartbeat;
    radioData.FromRadioId = RADIO_ID;

    if (_radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData))) {
      Serial.println("...Success");
    } else {
      Serial.println("...Failed");
    }
  }

  // Request data from the primary receiver once every 4 seconds.
  if (millis() - _lastMessageRequestTime > 3999) {
    _lastMessageRequestTime = millis();

    Serial.println("Requesting data");
    Serial.println("  Sending BeginGetData");
    RadioPacket radioData;
    radioData.PacketType = BeginGetData;  // When the receiver sees this packet type, it will load an ACK data packet.
    _radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData));

    Serial.println("  Sending EndGetData");
    radioData.PacketType = EndGetData;  // When the receiver replies to this packet, we will get the ACK data that was loaded.
    _radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData));

    while (_radio.hasAckData()) {
      RadioPacket ackData;
      _radio.readData(&ackData);
      Serial.println(ackData.temperature);

      temperature = ackData.temperature;
    }
  }
}
