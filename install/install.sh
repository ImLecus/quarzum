# MAKE SURE YOU RUN THE SCRIPT WITH SUDO!
# INSTALLATION PATH (LINUX)
PREFIX="/usr"
BIN_DIR="$PREFIX/bin/quarzum"
LIB_DIR="$PREFIX/lib/quarzum"
# CREATE DIRECTORIES
echo "Installing Quarzum..."
mkdir -p "$BIN_DIR"
mkdir -p "$LIB_DIR"

cp ../out/build/preset/quarzum "$BIN_DIR"
cp --recursive ../lib/std "$LIB_DIR"