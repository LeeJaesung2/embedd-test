#!/bin/bash
# 파이썬 2.7.18 다운로드 URL
URL="https://www.python.org/ftp/python/2.7.18/Python-2.7.18.tgz"

# 다운로드 파일 이름
FILENAME="Python-2.7.18.tgz"

# 다운로드 받을 디렉토리
INSTALL_DIR="~/temp"

mkdir ~/temp
# 파이썬 다운로드 및 압축 해제
cd $INSTALL_DIR
curl -O $URL
tar -zxvf $FILENAME

# 필수 라이브러리 설치
apt-get update
apt-get install build-essential checkinstall
apt-get install libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev -y

# 파이썬 빌드 및 설치
cd Python-2.7.18
./configure --enable-optimizations
make altinstall

# 파이썬 2.7.18을 기본 파이썬으로 설정
update-alternatives --install /usr/bin/python python /usr/local/bin/python2.7 2
update-alternatives --config python

# 설치된 버전 확인
python --version

mv /usr/local/include/python2.7 /usr/include
rm -r ~/temp
