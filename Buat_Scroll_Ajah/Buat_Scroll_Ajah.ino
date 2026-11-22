/*
 * ============================================================
 *  TIKTOK SCROLLER BUAT ORANG YANG LAGI MAKAN
 * ============================================================
 *  Masalah : Tangan berminyak, layar HP jadi korban.
 *  Solusi  : ESP32 + 1 tombol + jempol kaki.
 *
 *  Cara pakai (baca, jangan langsung skip):
 *    1. Flash kode ini ke ESP32
 *    2. Buka Bluetooth HP, pair ke "Jempol Kaki Scroller"
 *    3. Taruh tombol di lantai
 *    4. Makan nasgor sambil nginjek tombol
 *    5. Hidup terasa seperti raja/ratu
 *
 *  Disclaimer:
 *    - Penulis ga bertanggung jawab kalau lu kebablasan
 *      scroll sampai nasinya dingin
 *    - Kalau tombolnya keinjek pas lagi jalan ke kamar mandi,
 *      itu salah kaki lu, bukan salah kode
 *    - Jempol kaki wajib bersih. Ini bukan saran, ini etika.
 *
 *  Library : HijelHID_BLEMouse (+ NimBLE-Arduino >= 2.3.8)
 * ============================================================
 */

#include <HijelHID_BLEMouse.h>

// Nama yang muncul di Bluetooth HP. Ganti sesuka hati,
HijelBLEMouse mouse("Jempol Kaki Scroller", "Member kos Ambarawa", 100, 3, false);

// ---------- PIN ----------
const int BTN_DOWN = 4;   // Tombol "Next, lanjut, skip"
const int BTN_UP   = 5;   // Tombol Scroll Atas
                          


// ---------- SETTINGAN ----------
const int SWIPE_TICKS = 8;

// Anti "tombol nge-getar". Kalau lu injek 1x tapi kehitung 3x,
// naikin angka ini. Kalau kerasa lemot, turunin.
const unsigned long DEBOUNCE_MS = 40;

// ---------- VARIABEL INTERNAL (jangan diutak-atik kecuali berani) ----------
bool lastDown = HIGH, lastUp = HIGH;
unsigned long tDown = 0, tUp = 0;

void setup() {
  // INPUT_PULLUP = ga perlu resistor. Kabel satu ke pin, satu ke GND.
  // Semudah itu, ga perlu gelar sarjana elektro.
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);

  mouse.begin();

  // Kalau HP ga mau connect dan lu mulai emosi, jangan ngamuk:
  // 1. Hapus "Jempol Kaki Scroller" di Bluetooth HP
  // 2. Uncomment baris di bawah, flash sekali
  // 3. Comment lagi biar ga lupa-ulang tiap boot
  // mouse.clearBonds();
}

void loop() {
  unsigned long now = millis();
  bool d = digitalRead(BTN_DOWN);
  bool u = digitalRead(BTN_UP);


  if (lastDown == HIGH && d == LOW && now - tDown > DEBOUNCE_MS) {
    if (mouse.isPaired()) {          // belum pair = tombolnya cuma pajangan
      mouse.scroll(-SWIPE_TICKS);    // minus = turun. Kalau kebalik, hapus minusnya.
    }
    tDown = now;
  }

  if (lastUp == HIGH && u == LOW && now - tUp > DEBOUNCE_MS) {
    if (mouse.isPaired()) {
      mouse.scroll(SWIPE_TICKS);     // balik ke video sebelumnya
    }
    tUp = now;
  }

  lastDown = d;
  lastUp = u;

  // Selamat, lu baru aja bikin perangkat keras paling mager sedunia.
  // Silakan lanjut makan bro ku.
}
