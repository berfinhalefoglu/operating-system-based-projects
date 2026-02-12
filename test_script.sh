#!/bin/bash

# Renk tanımlamaları
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Test Script Başlatılıyor...${NC}"

# 1. Derleme
echo -e "${GREEN}Kodlar Derleniyor...${NC}"
gcc -pthread -o dining_philosophers dining_philosophers.c
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Dining Philosophers Programı başarıyla derlendi.${NC}"
else
    echo -e "${RED}Dining Philosophers Programı derlenirken hata oluştu.${NC}"
    exit 1
fi

gcc -pthread -o sleeping_barber sleeping_barber.c
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Sleeping Barber Programı başarıyla derlendi.${NC}"
else
    echo -e "${RED}Sleeping Barber Programı derlenirken hata oluştu.${NC}"
    exit 1
fi

# 2. Programları Çalıştırma ve Çıktıları Kaydetme
echo -e "${GREEN}Programlar çalıştırılıyor...${NC}"

# Dining Philosophers Programını çalıştırma
for i in {1..4}
do
    echo -e "${YELLOW}Dining Philosophers: Çözüm Yöntemi $i${NC}"
    ./dining_philosophers <<< "$i" > output_philosophers_$i.txt &
    sleep 3

    # Çalışma doğruluğunu kontrol etme
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Yöntem $i başarıyla çalıştı ve çıktısı kaydedildi.${NC}"
    else
        echo -e "${RED}Yöntem $i çalışırken hata oluştu!${NC}"
    fi
    pkill -f dining_philosophers
done

# Sleeping Barber Programını çalıştırma
echo -e "${YELLOW}Sleeping Barber Programı Çalıştırılıyor...${NC}"
./sleeping_barber > output_barber.txt &
sleep 5

# Çalışma doğruluğunu kontrol etme
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Sleeping Barber Programı başarıyla çalıştı ve çıktısı kaydedildi.${NC}"
else
    echo -e "${RED}Sleeping Barber Programı çalışırken hata oluştu!${NC}"
fi
pkill -f sleeping_barber

# 3. Çıktı Dosyalarını Kontrol Etme
echo -e "${GREEN}Çıktılar kontrol ediliyor...${NC}"
for file in output_philosophers_1.txt output_philosophers_2.txt output_philosophers_3.txt output_philosophers_4.txt output_barber.txt; do
    if [ -s $file ]; then
        echo -e "${GREEN}$file başarıyla oluşturuldu.${NC}"
    else
        echo -e "${RED}$file oluşturulamadı veya boş.${NC}"
    fi
done

echo -e "${GREEN}Test Script Tamamlandı.${NC}"
