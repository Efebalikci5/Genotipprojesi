# Genetik Simülatör Projesi

## Genel Bakış
Bu proje, Arduino tabanlı bir genetik simülatör. Otosomal (normal) ve X-bağlı kalıtım modlarını destekler. Kullanıcı düğmelerle simülasyon çalıştırabilir ve mod değiştirebilir. LED'ler genotip ve cinsiyet durumunu gösterir.

## Özellikler
- **İki Mod**: Otosomal (AA/Aa/aa) ve X-linked (XA/Xa/Y).
- **LED Gösterimi**: 4 kişi (Anne, Baba, Çocuk1, Çocuk2) için genotip LED'leri (Yeşil: AA/sağlıklı, Sarı: Aa/taşıyıcı, Kırmızı: aa/hasta).
- **Cinsiyet LED'leri**: Sadece X-linked modda, Çocuk1 ve Çocuk2 için. Erkek sürekli yanar, kız yanıp söner (blink).
- **Animasyon**: Mod değişikliğinde tüm LED'ler yanıp söner (non-blocking, 500ms aralıklarla).
- **Debounce**: Düğmeler on-release trigger ile yanlış basışları önler.
- **Rastgelelik**: randomSeed A0-A5 pinlerinden karmaşık değer alır.

## Donanım Yapısı
### Pinler
- **LED Pinleri**:
  - Anne: 5 (Yeşil), 6 (Sarı), 7 (Kırmızı)
  - Baba: 11 (Yeşil), 12 (Sarı), 13 (Kırmızı)
  - Çocuk1: 8 (Yeşil), 9 (Sarı), 10 (Kırmızı), 18 (Cinsiyet)
  - Çocuk2: 14 (Yeşil), 15 (Sarı), 16 (Kırmızı), 19 (Cinsiyet)
- **Düğme Pinleri**:
  - Simülasyon: Pin 2 (INPUT_PULLUP)
  - Mod Değiştirme: Pin 4 (INPUT_PULLUP)
- **Analog Pinler**: A0-A5, randomSeed için.

### Bağlantı Şeması
LED'ler dirençlerle bağlı (örneğin, Baba'nın dirençleri 11,12,13 pinlerine). Detaylar için JSON diyagramına bakın.

## Yazılım Yapısı
### Ana Dosya: Kod_ortak_25.4.ino
- **Struct Individual**: gene1, gene2, isMale.
- **Global Değişkenler**:
  - Mod: isXLinked
  - Debounce: lastSimPress, lastModePress, simButtonWasPressed, modeButtonWasPressed
  - Blink: lastBlink, blinkState
  - Flash: flashing, flashTimes, currentFlash, lastFlashToggle, flashState
- **Fonksiyonlar**:
  - setup(): Serial, pinMode, randomSeed, başlangıç LED'ler.
  - loop(): Düğme kontrolleri (on-release), blink/flash güncellemeleri.
  - simulateGenetics(): Mod'a göre gen hesaplaması.
  - updateLEDs(): LED güncellemeleri.
  - printResults(): Seri çıktı.
  - displayGenotype(): LED görselleştirme (X-linked erkekler için özel mantık).
  - flashAllLEDs(): Flash başlatma (non-blocking).

### Mod Mantığı
- **Otosomal**: Rastgele genler, dominantCount'a göre LED.
- **X-linked**: Anne XX, Baba XY, Çocuklar miras alır. Erkekler XA Y (sağlıklı) veya Xa Y (hasta), kadınlar XX kombinasyonları.

## Kullanım
1. Arduino'yu yükleyin.
2. Seri monitörü açın (9600 baud).
3. Simülasyon düğmesi (Pin 2): Genetik hesapla ve göster.
4. Mod düğmesi (Pin 4): Otosomal ↔ X-linked değiştir, flash animasyonu.
5. X-linked'de cinsiyet LED'leri blink/kalıcı yanar.

## Geliştirme Notları
- Kod Arduino IDE ile derlenir.
- Pin eşleşmeleri JSON diyagramıyla uyumlu.
- Non-blocking animasyonlar millis() kullanır.
- Güvenlik için on-release debounce.

## Gereksinimler
- Arduino Uno (veya benzeri)
- LED'ler, dirençler, düğmeler
- Arduino IDE

Bu dokümantasyon projeyi anlamak için kullanılır.