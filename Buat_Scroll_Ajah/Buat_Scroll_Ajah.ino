#include <HijelHID_BLEMouse.h>

HijelBLEMouse mouse("ESP32 Scroller", "DIY", 100, 3, false);

const int BTN_DOWN = 4;       // swipe ke atas (konten naik, lanjut ke bawah)
const int BTN_UP   = 5;       // swipe balik (opsional)

const int SWIPE_TICKS = 8;    // atur ini: makin besar = swipe makin jauh
const unsigned long DEBOUNCE_MS = 40;

bool lastDown = HIGH, lastUp = HIGH;
unsigned long tDown = 0, tUp = 0;

void setup() {
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  mouse.begin();
}

void loop() {
  unsigned long now = millis();
  bool d = digitalRead(BTN_DOWN);
  bool u = digitalRead(BTN_UP);

  // trigger cuma saat transisi HIGH -> LOW (baru ditekan)
  if (lastDown == HIGH && d == LOW && now - tDown > DEBOUNCE_MS) {
    if (mouse.isPaired()) mouse.scroll(-SWIPE_TICKS);  // negatif = turun
    tDown = now;
  }
  if (lastUp == HIGH && u == LOW && now - tUp > DEBOUNCE_MS) {
    if (mouse.isPaired()) mouse.scroll(SWIPE_TICKS);   // positif = naik
    tUp = now;
  }

  lastDown = d;
  lastUp = u;
}