rm -rf opk
mkdir -p opk
cp build/enigmistica opk
cp opendingux/default.gcw0.desktop opk
cp opendingux/icon.png opk
cp ../projects/msvc2017/Crosswords/chess.png opk
cp ../projects/msvc2017/Crosswords/font.png opk
mksquashfs opk enigmistica.opk -all-root -noappend -no-exports -no-xattrs -no-progress > /dev/null
# rm -rf opk
