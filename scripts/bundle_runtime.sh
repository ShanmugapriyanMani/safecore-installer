#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
QTDIR="${QTDIR:-/home/sandy/Qt/6.10.1/gcc_64}"
DEST="${ROOT_DIR}/runtime"

if [[ ! -d "$QTDIR" ]]; then
  echo "Qt directory not found: $QTDIR" >&2
  exit 1
fi

rm -rf "$DEST"
mkdir -p "$DEST/lib" "$DEST/plugins" "$DEST/qml"

cp -a "$QTDIR/lib/"libQt6*.so* "$DEST/lib/"
cp -a "$QTDIR/lib/"libicu*.so* "$DEST/lib/" || true

cp -a "$QTDIR/plugins/." "$DEST/plugins/"
cp -a "$QTDIR/qml/." "$DEST/qml/"

echo "Runtime bundle created at: $DEST"
