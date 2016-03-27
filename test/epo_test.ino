#include "Adafruit_GPS.h"

Adafruit_GPS gps = Adafruit_GPS();


char satelite_1[60] = {
  0xE0, 0x18, 0x04, 0x1D, 0x49, 0x01, 0x00, 0x00, 0x18, 0x00, 0x30, 0x2A, 0x66, 0x2E, 0x92,
  0xF9, 0x70, 0x18, 0xA5, 0xF9, 0x25, 0x00, 0x30, 0x2A, 0x8A, 0x13, 0x0A, 0x00, 0xE3, 0xB8,
  0x04, 0x80, 0xFF, 0xAA, 0xFF, 0x31, 0x3F, 0x77, 0x3A, 0x30, 0x2E, 0xD4, 0x6A, 0x01, 0x4E,
  0x89, 0x0D, 0xA1, 0x17, 0xA6, 0x95, 0x7A, 0x7B, 0xD0, 0x2A, 0x27, 0x93, 0xF8, 0xDD, 0xCC
};

char nmea_baud[5] = { 0x00, 0x80, 0x25, 0x00, 0x00 };

bool test_format_acknowledge_packet() {
  char expected_packet[12] = {
    0x04, 0x24, 0x0C, 0x00, 0x01, 0x00, 0xFD, 0x00, 0x03, 0xF3, 0x0D, 0x0A
  };
  char ack_packet[12] = { 0 };
  gps.format_acknowledge_packet(expected_packet, 253);
  return strncmp(expected_packet, ack_packet, 12) == 0;
}

bool test_format_packet() {
  char packet_data[5] = { 0x00, 0x80, 0x25, 0x00, 0x00 };
  char expected_packet[14] = {
    0x04, 0x24, 0x0E, 0x00, 0xFD, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x56, 0x0D, 0x0A
  };
  char packet[14] = { 0 };
  gps.format_packet(253, packet_data, 5, packet);
  return strncmp(expected_packet, packet, 14) == 0;
}

bool test_waitForPacket() {
  char expected_packet[12] = { 0 };
  gps.format_acknowledge_packet(expected_packet, 253);

  Serial.println("Send command 253");
  gps.send_binary_command(253, nmea_baud, 5);

  Serial.println("Waiting for packet: ");
  if (!gps.waitForPacket(expected_packet, 12, 5000)) {
    Serial.println("Never got packet");
  } else {
    Serial.println("Found packet");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello friend");
  gps.begin(9600);
  delay(500);
  gps.startEpoUpload();
  delay(500);
  /*
  gps.sendEpoSatellite(satelite_1);
  if (!gps.flush_epo_packet()) {
    Serial.println("Couldn't flush packet");
  } else {
    Serial.println("Flushed packet");
  }
  */
}

void loop() {
  gps.dump_binary_packet();
  delay(1000);
}