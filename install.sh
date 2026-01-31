#!/usr/bin/env bash
set -e

REPO="AkashKamatCreates/cbreath"
VERSION="1.0.0-1"
DEB="cbreath_${VERSION}_amd64.deb"
URL="https://github.com/${REPO}/releases/download/v1.0.0/${DEB}"

echo
echo "🔹 Installing cbreath (CLI breathing guide)…"
echo

# Download .deb
echo "📥 Downloading package…"
curl -fL "$URL" -o "/tmp/$DEB"

# Install the package
echo "📦 Installing package (may require sudo)…"
sudo dpkg -i "/tmp/$DEB" || sudo apt -f install -y

echo
echo "✅ cbreath installed successfully!"
echo
echo "▶ Run:"
echo "    cbreath"
echo
echo "🔹 Controls: space = pause/resume, q = quit"
echo
