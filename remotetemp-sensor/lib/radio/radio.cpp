#include <NRFLite.h>
#include <SPI.h>
#include <pinout.h>

#define RADIO_ID 2              // Our radio's id, 0 = hub
#define DESTINATION_RADIO_ID 0  // Id of the radio we will transmit to.

extern float temperature;

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

// Set up NRF24 radio
NRFLite radio;
RadioPacket radioData;

void setupRadio() {
  if (!radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1)
      ;  // Wait here forever.
  }
}

void updateRadio() {
  while (radio.hasData()) {
    RadioPacket radioData;
    radio.readData(&radioData);

    if (radioData.PacketType == Heartbeat) {
      String msg = "Heartbeat from ";
      msg += radioData.FromRadioId;
      Serial.println(msg);
    } else if (radioData.PacketType == BeginGetData) {
      Serial.println("Received data request, adding ACK packet");

      RadioPacket ackData;
      ackData.PacketType = ReceiverData;
      ackData.FromRadioId = RADIO_ID;
      ackData.temperature = temperature;

      // Add the data to send back to the transmitter.
      // The next packet we receive will be acknowledged with this data.
      radio.addAckData(&ackData, sizeof(ackData));
    } else if (radioData.PacketType == EndGetData) {
      // The transmitter hopefully received the acknowledgement data packet at this point.
    }
  }
}
