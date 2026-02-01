#!/usr/bin/env bash
# Don't use set -e to allow graceful error handling

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

TARGET_UID=${PKEXEC_UID:-0}
TARGET_USER=$(id -un "$TARGET_UID" 2>/dev/null || echo root)

DOCKER_INSTALLED=false
INSTALL_FAILED=false

# Function to wait for apt lock
wait_for_apt() {
  local max_wait=60
  local waited=0
  while fuser /var/lib/dpkg/lock-frontend >/dev/null 2>&1 || \
        fuser /var/lib/apt/lists/lock >/dev/null 2>&1 || \
        fuser /var/lib/dpkg/lock >/dev/null 2>&1; do
    if [ $waited -eq 0 ]; then
      echo "Waiting for other package managers to finish..."
    fi
    sleep 2
    waited=$((waited + 2))
    if [ $waited -ge $max_wait ]; then
      echo -e "${RED}Timeout waiting for package manager lock${NC}"
      return 1
    fi
  done
  return 0
}

echo -e "\n${GREEN}[1/2] Checking Docker...${NC}"
if ! docker --version >/dev/null 2>&1; then
  echo "Installing Docker..."
  echo "Downloading Docker install script..."
  
  if ! curl -fsSL https://get.docker.com -o /tmp/get-docker.sh; then
    echo -e "${RED}Failed to download Docker install script${NC}"
    INSTALL_FAILED=true
  else
    echo "Running Docker install script..."
    wait_for_apt
    if ! sh /tmp/get-docker.sh; then
      echo -e "${RED}Docker installation failed${NC}"
      INSTALL_FAILED=true
    else
      usermod -aG docker "$TARGET_USER" || true
      DOCKER_INSTALLED=true
      echo -e "${GREEN}Docker installed successfully.${NC}"
    fi
    rm -f /tmp/get-docker.sh
  fi
else
  echo "Docker already installed: $(docker --version)"
fi

echo -e "\n${GREEN}[2/2] Checking NVIDIA Container Toolkit...${NC}"
if ! dpkg -l 2>/dev/null | grep -q nvidia-container-toolkit; then
  echo "Installing NVIDIA Container Toolkit..."
  
  # Add NVIDIA GPG key
  echo "Adding NVIDIA repository key..."
  curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | \
    gpg --batch --yes --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg 2>/dev/null || true
  
  # Add NVIDIA repository
  echo "Adding NVIDIA repository..."
  curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    tee /etc/apt/sources.list.d/nvidia-container-toolkit.list > /dev/null
  
  echo "Updating package lists..."
  wait_for_apt
  if ! apt-get update -qq; then
    echo -e "${YELLOW}Warning: apt-get update had issues, continuing anyway...${NC}"
  fi
  
  echo "Installing nvidia-container-toolkit package..."
  wait_for_apt
  if ! apt-get install -y nvidia-container-toolkit; then
    echo -e "${RED}Failed to install NVIDIA Container Toolkit${NC}"
    INSTALL_FAILED=true
  else
    echo -e "${GREEN}NVIDIA Container Toolkit installed successfully.${NC}"
  fi
else
  echo "NVIDIA Container Toolkit already installed."
fi

if [ "$DOCKER_INSTALLED" = true ] || ! dpkg -l 2>/dev/null | grep -q nvidia-container-toolkit-base; then
  echo -e "\n${YELLOW}Restarting Docker service...${NC}"
  if systemctl restart docker; then
    echo "Docker service restarted."
  else
    echo -e "${YELLOW}Warning: Failed to restart Docker service${NC}"
  fi
fi

if [ "$INSTALL_FAILED" = true ]; then
  echo -e "\n${RED}Installation completed with errors.${NC}"
  exit 1
fi

echo -e "\n${GREEN}Installation completed successfully!${NC}"
exit 0
