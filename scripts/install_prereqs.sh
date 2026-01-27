#!/usr/bin/env bash
set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

TARGET_UID=${PKEXEC_UID:-0}
TARGET_USER=$(id -un "$TARGET_UID" 2>/dev/null || echo root)

echo -e "\n${GREEN}[1/2] Checking Docker...${NC}"
if ! command -v docker >/dev/null 2>&1; then
  echo "Installing Docker..."
  curl -fsSL https://get.docker.com | sh
  usermod -aG docker "$TARGET_USER" || true
  echo -e "${YELLOW}Docker installed. You may need to logout/login for group changes.${NC}"
else
  echo "Docker already installed: $(docker --version)"
fi

echo -e "\n${GREEN}[2/2] Checking NVIDIA Container Toolkit...${NC}"
if ! dpkg -l | grep -q nvidia-container-toolkit; then
  echo "Installing NVIDIA Container Toolkit..."
  curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | gpg --batch --yes --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg 2>/dev/null || true
  curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    tee /etc/apt/sources.list.d/nvidia-container-toolkit.list > /dev/null
  apt-get update
  apt-get install -y nvidia-container-toolkit
  systemctl restart docker
  echo "NVIDIA Container Toolkit installed."
else
  echo "NVIDIA Container Toolkit already installed."
fi

echo -e "\n${YELLOW}Applying docker group changes...${NC}"
newgrp docker || true
echo -e "${YELLOW}If docker permission is still denied, logout/login or run 'newgrp docker' in your terminal and relaunch the app.${NC}"
