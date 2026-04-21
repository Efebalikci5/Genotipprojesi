# Genotipprojesi

## Setup

1. Wokwi.com adresine gidin ve yeni bir Arduino Uno projesi baslatin.
2. diagram.json icerigini Wokwi'deki "diagram.json" sekmesine kopyalayin.
3. genetik_simulator.ino kodunu ana kod sekmesine yapistirin.
4. Simulasyonu baslatmak icin "Play" butonuna basin.

## Devre Baglantilari

### LED'ler (Fenotip Gostergeleri)

Her birey icin 3 renkli bir LED grubu kullanilir:

| Kisi   | Yesil | Sari | Kirmizi |
|--------|-------|------|---------|
| Anne   | 5     | 6    | 7       |
| Baba   | 8     | 9    | 10      |
| Cocuk1 | 11    | 12   | 13      |
| Cocuk2 | 14    | 15   | 16      |

**FenotipLED Iliskisi:**
- Yesil LED: Homozigot Dominant (AA) - 2 baskın gen
- Sari LED: Heterozigot (Aa) - 1 baskın gen
- Kirmizi LED: Homozigot Resesif (aa) - 0 baskın gen

### Buton

- Pin 2: Crossover baslatma butonu ( INPUT_PULLUP )

### Direncler

- LED onayi icin 220Ω - 330Ω (fiziksel devre)
- Simülasyonda 220kΩ (görsel amaç)

## Calisma Mantigi

1. Sistem basladiginda Anne ve Baba "Aa" (heterozigot) olarak ayarlanir.
2. Crossover butonuna her basilinda:
   - Anne ve Baba'dan rastgele genler alinarak yeni cocuklar hesaplanir.
   - Sonuclar LED'lerde gosterilir.
3. Her butona basista farkli sonuc elde edilir (random secim).

## Teknik Detaylar

- **Random Seed:** analogRead(A5) kullanilarak her açilişta farkli rastgele sonuçlar üretilir.
- **Debounce:** 200ms - buton tiklamasini önler.
- **Genetik Kurallar:**
  - Anneden bir gen (A veya a) rastgele secilir.
  - Babadan bir gen (A veya a) rastgele secilir.
  - Cesitli kombinasyonlarla cocuklar olusturulur.

## Olasiliklar

Anne (Aa) x Baba (Aa) kesisiminde:
- %25 AA (Yesil)
- %50 Aa (Sari)
- %25 aa (Kirmizi)
