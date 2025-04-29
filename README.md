# Sosyal Ağ ve Kırmızı-Siyah Ağaç Simülasyonu

Bu proje, kullanıcıların ve arkadaşlık ilişkilerinin yönetildiği basit bir **sosyal ağ** simülasyonunu C programlama dili ile gerçekleştirir. 
Aynı zamanda kullanıcı kimliklerini Kırmızı-Siyah Ağaç (Red-Black Tree) veri yapısında depolayarak hızlı arama imkanı sunar.

##Özellikler

- Kullanıcı ekleme ve arkadaşlık oluşturma
- Derinlik bazlı arkadaş arama (örneğin: 2 mesafedeki arkadaşlar)
- Ortak arkadaşları listeleme
- Topluluk (bağlı bileşen) tespiti
- Etki alanı (kaç kişiyi etkiliyor?) hesaplama
- Kırmızı-Siyah Ağaç ile kullanıcı ID arama

#Yapı

Proje iki ana bileşenden oluşur:

1. Kırmızı-Siyah Ağaç (Red-Black Tree):
   - Kullanıcı kimlikleri burada tutulur.
   - `ks_basit_ekle`, `ks_arama` fonksiyonları ile kullanıcı ekleme ve arama yapılır.
   - Tam Kırmızı-Siyah dengelemesi uygulanmamıştır (temel ekleme gerçekleştirilmiştir).

2. Arkadaşlık Grafiği:
   - Kullanıcılar ve arkadaşlıkları bağlı liste ile gösterilir.
   - Derinlik arama, bağlı bileşen tespiti, etki alanı gibi grafik algoritmaları içerir.

##Derleme ve Çalıştırma

Bu program temel bir C derleyicisi ile kolayca derlenebilir.

```bash
gcc -o sosyal_ag sosyal_ag.c
./sosyal_ag
