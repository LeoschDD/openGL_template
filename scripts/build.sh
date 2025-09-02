set -euo pipefail

CONFIG="${1:-Debug}"

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
cd "$ROOT_DIR"

rm -rf external build
mkdir -p external build

cmake -S . -B build -DCMAKE_BUILD_TYPE="${CONFIG}" \
  -Dglew-cmake_BUILD_SHARED=OFF -Dglew-cmake_BUILD_STATIC=ON -DONLY_LIBS=ON

cmake --build build --parallel "$(nproc)"