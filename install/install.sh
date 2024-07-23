# MAKE SURE YOU RUN THE SCRIPT WITH SUDO!
# INSTALLATION PATH (LINUX)
PREFIX="/usr"
BIN_DIR="$PREFIX/bin/quarzum"
LIB_DIR="$PREFIX/lib/quarzum"
# CREATE DIRECTORIES
echo "Installing Quarzum..."
mkdir -p "$BIN_DIR"
mkdir -p "$LIB_DIR"

cp /home/lecus/code/quarzum/out/build/preset/quarzum "$BIN_DIR"
cp --recursive /home/lecus/code/quarzum/lib/std "$LIB_DIR"