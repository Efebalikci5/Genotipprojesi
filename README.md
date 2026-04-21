# Genotipprojesi
Wokwi.com adresine gidin ve yeni bir Arduino Uno projesi başlatın.

diagram.json içeriğini Wokwi'deki "diagram.json" sekmesine kopyalayın.

genetik_simulator.ino kodunu ana kod sekmesine yapıştırın.

Simülasyonu başlatmak için "Play" butonuna basın.
Bağlantı Şeması (Pin Layout)
Kod içerisinde tanımlanan ve diagram.json dosyasında uygulanan pin bağlantıları aşağıdadır:

1. LED Grupları (Fenotip Göstergeleri)
Her birey için 3 renkli bir LED grubu kullanılır. Bu LED'ler genotipteki baskın (A) gen sayısını temsil eder.

Her birey için 3 renkli bir led kullanıldı.
Yeşil led (AA) sarı led(Aa) kırmızı led (aa)
koda göre
Anne yeşil pin5e sarı pin6 ya kırmızı pin 7 ye 
Baba yeşil pin8 e Sarı pin9 a kırmızı Pin10 a 
1.çocuk yeşil pin11 sarı Pin12 kırmızı Pin 13 e
2.çocuk yeşil Pin A0 sarı Pin A1 kırmızı Pin A2 ye bağlanmalıdır.
Dirençler: LED'leri korumak için 220kΩ (simülasyonda görsel amaçlıdır, fiziksel devrede 220Ω - 330Ω kullanılmalıdır).

GND: Tüm LED'lerin Katot (A) uçları ve butonların birer ucu Arduino'nun GND pinine bağlıdır.
Çalışma Mantığı
AA (Baskın): Yeşil LED yanar.

Aa (Heterozigot): Sarı LED yanar.

aa (Çekinik): Kırmızı LED yanar.
Teknik Detaylar
Random Seed: analogRead(A5) kullanılarak her açılışta farklı rastgele sonuçlar üretilmesi sağlanmıştır.

Debounce: Butonlara ardı ardına yanlışlıkla basılmasını önlemek için 1000ms'lik bir bekleme süresi (debounce) eklenmiştir.
Işık yanışlında bazen tutarsızlık var butonla hesaplama başlatılacak şekilde ayarlanmalı
Kontroller
Başlangıç: Sistem başladığında Anne ve Baba "Aa" (Sarı) olarak ayarlanır ve çocuklar bu genlerden rastgele türetilir.

Anne Butonu: Basıldığında annenin genotipini rastgele (AA, Aa veya aa) değiştirir ve çocukları yeniden hesaplar.

Baba Butonu: Basıldığında babanın genotipini rastgele değiştirir ve çocukları yeniden hesaplar.
