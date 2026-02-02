#!/usr/bin/env bash
# Don't use set -e to allow graceful error handling

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

TARGET_UID=${PKEXEC_UID:-0}
TARGET_USER=$(id -un "$TARGET_UID" 2>/dev/null || echo root)

DOCKER_INSTALLED=false

# Temporarily disable problematic NVIDIA local repos (GPG key issues)
for repo in /etc/apt/sources.list.d/*nvidia-driver-local*.list; do
  [ -f "$repo" ] && mv "$repo" "$repo.disabled" 2>/dev/null && echo "Temporarily disabled: $repo"
done

echo -e "\n${GREEN}[1/2] Checking Docker...${NC}"
if ! docker --version >/dev/null 2>&1; then
  echo "Installing Docker..."
  curl -fsSL https://get.docker.com | sh
  DOCKER_INSTALLED=true
  echo "Docker installed."
else
  echo "Docker already installed: $(docker --version)"
fi

# Add user to docker group
echo "Adding $TARGET_USER to docker group..."
usermod -aG docker "$TARGET_USER" || true

echo -e "\n${GREEN}[2/2] Checking NVIDIA Container Toolkit...${NC}"
if ! dpkg -l | grep -q nvidia-container-toolkit; then
  echo "Installing NVIDIA Container Toolkit..."
  curl -fsSL https://nvidia.github.io/libnvidia-container/gpgkey | gpg --batch --yes --dearmor -o /usr/share/keyrings/nvidia-container-toolkit-keyring.gpg 2>/dev/null || true
  curl -s -L https://nvidia.github.io/libnvidia-container/stable/deb/nvidia-container-toolkit.list | \
    sed 's#deb https://#deb [signed-by=/usr/share/keyrings/nvidia-container-toolkit-keyring.gpg] https://#g' | \
    tee /etc/apt/sources.list.d/nvidia-container-toolkit.list > /dev/null
  apt-get update
  apt-get install -y nvidia-container-toolkit
  echo "NVIDIA Container Toolkit installed."
fi

# Configure NVIDIA runtime for Docker (always run to ensure it's configured)
echo "Configuring NVIDIA runtime for Docker..."
nvidia-ctk runtime configure --runtime=docker

# Re-enable disabled repos after all apt operations are done
for repo in /etc/apt/sources.list.d/*.disabled; do
  [ -f "$repo" ] && mv "$repo" "${repo%.disabled}" 2>/dev/null
done

# Always restart Docker to apply group and runtime changes
echo -e "\n${YELLOW}Restarting Docker service...${NC}"
systemctl restart docker
echo "Docker service restarted."

echo -e "\n${GREEN}Installation complete!${NC}"
echo -e "${YELLOW}Note: You may need to log out and back in for docker group changes to take effect.${NC}"
