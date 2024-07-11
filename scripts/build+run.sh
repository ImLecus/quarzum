/usr/bin/cmake --build ./out/build/preset --parallel 14
#mv ./out/build/preset/quarzum ./install/
#sh ../install/install.sh
echo "Build finished. Press any key to continue..."
read
sh ./scripts/run.sh