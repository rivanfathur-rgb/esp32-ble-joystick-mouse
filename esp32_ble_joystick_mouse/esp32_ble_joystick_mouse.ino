#include <HijelHID_BLEMouse.h>

HijelBLEMouse mouse("ESP32 Mouse", "DIY");

const int pinVRx = 34;
const int pinVRy = 35;
const int pinSW  = 27;

int centerX = 2048;
int centerY = 2048;

const int deadzone = 250;
const float sensitivity = 0.006;
const int maxSpeed = 15; // batas kecepatan maksimum per loop, biar ga nyentak

bool lastButtonState = false;

int clampSpeed(int val) {
  if (val > maxSpeed) return maxSpeed;
  if (val < -maxSpeed) return -maxSpeed;
  return val;
}

void setup() {
  Serial.begin(115200);
  pinMode(pinSW, INPUT_PULLUP);

  long sumX = 0, sumY = 0;
  const int samples = 50;
  for (int i = 0; i < samples; i++) {
    sumX += analogRead(pinVRx);
    sumY += analogRead(pinVRy);
    delay(10);
  }
  centerX = sumX / samples;
  centerY = sumY / samples;

  mouse.begin();
}

void loop() {
  if (mouse.isPaired()) {
    int rawX = analogRead(pinVRx);
    int rawY = analogRead(pinVRy);

    // X dan Y ditukar (fix kebalik)
    int deltaX = -(rawY - centerY);
    int deltaY = rawX - centerX;

    if (abs(deltaX) < deadzone) deltaX = 0;
    if (abs(deltaY) < deadzone) deltaY = 0;

    int moveX = clampSpeed((int)(deltaX * sensitivity));
    int moveY = clampSpeed((int)(deltaY * sensitivity));

    if (moveX != 0 || moveY != 0) {
      mouse.move(moveX, moveY);
    }

    bool pressed = (digitalRead(pinSW) == LOW);
    if (pressed != lastButtonState) {
      mouse.setButton(MouseButton::Left, pressed);
      lastButtonState = pressed;
    }
  }

  delay(8);
}