#!/bin/bash

# batuhan DEMİREL
# 2420171009
# https://www.btkakademi.gov.tr/portal/certificate/validate?certificateId=WJ1SkPPXak
# https://www.btkakademi.gov.tr/portal/certificate/validate?certificateId=zXztnNNZo7
# https://credsverse.com/credentials/3b113b45-6e27-465b-8272-9e53da03229e

LOG_FILE="report.log"
echo "=== Script Başlatıldı: $(date -Iseconds) ===" > "$LOG_FILE"
echo "----------------------------------------" >> "$LOG_FILE"

OS_TYPE=$(uname)
echo "Sistem bilgileri toplanıyor..."

if [ "$OS_TYPE" == "Darwin" ]; then
    echo "[İşletim Sistemi: macOS]" >> "$LOG_FILE"
    system_profiler SPHardwareDataType | grep -E "Processor Name|Total Number of Cores|Memory" >> "$LOG_FILE"
    system_profiler SPHardwareDataType | grep -E "Model Identifier|Hardware UUID" >> "$LOG_FILE"
    system_profiler SPStorageDataType | grep -A 5 "Device Name" >> "$LOG_FILE"
    ifconfig | grep -A 1 "ether" >> "$LOG_FILE"
else
    echo "[İşletim Sistemi: Windows]" >> "$LOG_FILE"
    wmic cpu get Name, NumberOfCores /value 2>/dev/null | grep = >> "$LOG_FILE"
    wmic ComputerSystem get TotalPhysicalMemory /value 2>/dev/null | grep = >> "$LOG_FILE"
    wmic baseboard get Product,Manufacturer /value 2>/dev/null | grep = >> "$LOG_FILE"
    wmic csproduct get UUID /value 2>/dev/null | grep = >> "$LOG_FILE"
    wmic diskdrive get Model,SerialNumber /value 2>/dev/null | grep = >> "$LOG_FILE"
    getmac /v /fo list 2>/dev/null | grep -E "Fiziksel|Physical|Yol" >> "$LOG_FILE"
fi

echo "----------------------------------------" >> "$LOG_FILE"
echo "=== Bilgi Toplama Tamamlandı: $(date -Iseconds) ===" >> "$LOG_FILE"

echo ""
read -sp "Lütfen şifreleme parolasını (MYO+202) giriniz: " PAROLA
echo ""

echo "Dosya AES256 ile şifreleniyor..."
echo "$PAROLA" | gpg --batch --yes --passphrase-fd 0 \
  --symmetric \
  --cipher-algo AES256 \
  --output report.log.gpg \
  "$LOG_FILE"

if [ $? -eq 0 ]; then
    rm -f "$LOG_FILE"
    echo "[OK] 'report.log.gpg' başarıyla oluşturuldu ve orijinal dosya silindi."
else
    echo "[HATA] Şifreleme başarısız oldu!"
    exit 1
fi

echo ""
echo "=== GitHub İmza Ayarları Başlatılıyor ==="
read -p "GitHub E-posta adresinizi giriniz: " REPO_EMAIL
read -p "GitHub Adınızı ve Soyadınızı giriniz: " REPO_USER
read -p "GitHub Repository URL adresinizi giriniz: " REPO_URL

git config --global user.name "$REPO_USER"
git config --global user.email "$REPO_EMAIL"

if [ ! -f ~/.ssh/id_ed25519 ]; then
    echo "SSH imzalama anahtarı oluşturuluyor..."
    ssh-keygen -t ed25519 -C "$REPO_EMAIL" -N "" -f ~/.ssh/id_ed25519
fi

git config --global gpg.format ssh
git config --global user.signingkey ~/.ssh/id_ed25519.pub
git config --global commit.gpgsign true

echo "------------------------------------------------------------"
echo "KRİTİK ADIM: Aşağıdaki SSH imzalama anahtarını kopyalayın:"
echo "------------------------------------------------------------"
cat ~/.ssh/id_ed25519.pub
echo "------------------------------------------------------------"
echo "1. GitHub hesabınıza girin -> Settings -> SSH and GPG keys bölümüne gidin."
echo "2. 'New SSH Key' butonuna tıklayın."
echo "3. Key Type kısmını kesinlikle 'Signing Key' seçin!"
echo "4. Yukarıdaki anahtarı yapıştırın ve kaydedin."
echo "------------------------------------------------------------"
read -p "GitHub'a anahtarı eklediyseniz devam etmek için [ENTER] tuşuna basın..."

echo "Git deposu başlatılıyor ve dosyalar commit ediliyor..."

if [ ! -d .git ]; then
    git init
    git remote add origin "$REPO_URL"
fi

git add main.sh report.log.gpg
git commit -S -m "Ödev teslimi: Sistem raporu (şifrelenmiş) ve script dosyası"
git branch -M main
git push -u origin main

echo ""
echo "İşlem tamamlandı! GitHub commit geçmişinizde yeşil 'Verified' rozetini kontrol edebilirsiniz."
