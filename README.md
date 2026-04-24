## Setup

1. Wokwi.com adresine gidin ve yeni bir Arduino Uno projesi baslatin.
2. Proje klasöründeki en son diagram.json icerigini Wokwi'deki "diagram.json" sekmesine kopyalayin.
3. Kod_24.04.ino kodunu ana kod sekmesine yapistirin.
4. Simulasyonu baslatmak icin "Play" butonuna basin.

## Devre Baglantilari

### LED'ler (Fenotip Gostergeleri)

Her birey icin 3 renkli bir LED grubu kullanilir:

| Kisi   | Yesil | Sari | Kirmizi |
|--------|-------|------|---------|
| Anne   | 5     | 6    | 7       |
| Baba   | 8     | 9    | 10      |
| Cocuk1 | 11    | 12   | 13      |
| Cocuk2 | 14(A0)|15(A1)|16(A2)   |

**FenotipLED Iliskisi:**
- Yesil LED: Homozigot Dominant (AA) - 2 baskın gen
- Sari LED: Heterozigot (Aa) - 1 baskın gen
- Kirmizi LED: Homozigot Resesif (aa) - 0 baskın gen

### Buton

- Pin 2: Çaprazlama başlatma butonu (INPUT_PULLUP)

### Direncler

- LED onayi icin 220Ω - 330Ω (fiziksel devre)
- Simülasyonda 220kΩ (görsel amaç)

## Calisma Mantigi

1. Sistem basladiginda Anne ve Baba "Aa" (heterozigot) olarak ayarlanir ve LED'leri sarı yanar.
2. Crossover butonuna her basilinda:
   - **Adım 1:** Anne ve Baba icin yeni genler rastgele hesaplanir ve LED'leri anında güncellenir.
   - **Adım 2:** 1-1.5 saniye arasında rastgele bir sure beklenir.
   - **Adım 3:** Çocuk 1 ve Çocuk 2 genotipleri hesaplanır ve LED'leri yakılır.
3. LED'ler bir sonraki buton basinina kadar kalir (söndürülmez).
4. Her butona bastırmada tum ailede farkli sonuclar elde edilir (random secim).

## Teknik Detaylar

- **Random Seed:** analogRead(A5) kullanilarak her açilişta farkli rastgele sonuçlar üretilir.
- **Debounce:** 200ms - buton tiklamasini önler.
- **Bekleme suresi:** 1000-1500 ms arası rastgele.
- **Genetik Kurallar:**
  - Anne ve Baba hesaplanırken: her iki gen de rastgele 'A' veya 'a' olabilir.
  - Çocuklar: Anneden ve Babadan rastgele secilen birer gen miras alir.
  - Cesitli kombinasyonlarla cocuklar olusturulur.
