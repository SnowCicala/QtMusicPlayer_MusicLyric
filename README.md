# QtMusicPlayer_MusicLyric
歌词解析器

![](https://img.shields.io/badge/Build-passing-brightgreen) ![](https://img.shields.io/badge/Qt-5.13.0-brightgreen) ![](https://img.shields.io/badge/License-LGPL-brightgreen) ![](https://img.shields.io/badge/License-anti_996-brightgreen)

## 功能

1. 支持常用歌词文件（*.lrc）解析

2. 支持对歌词进行操作

3. 支持更加复杂的歌词格式（[Wiki](https://github.com/chushi0/QtMusicPlayer_MusicLyric/wiki/Lyric-Struct)）

## 使用

1. 从[release](https://github.com/chushi0/QtMusicPlayer_MusicLyric/releases)页面下载MusicLyric.zip，并解压缩到合适的文件夹
2. 将include文件夹下的内容复制到项目路径
3. 将项目以合适的方式链接到dll文件夹下的文件
4. 使用`#include "lyricfile/lyricfile.h"`引入库