#pragma once

#include "musiclyric_global.h"
#include <QFile>
#include <QMap>

#define NO_UNDO

namespace cszt {
/**
 * @brief
 * 表示正在播放的歌词进度相关信息。
 *
 * 注意：所有涉及下标越界问题均不会检查
 */
struct MUSICLYRICSHARED_EXPORT Lyric
{
   /// 主要歌词内容
   QString      mainText;
   /// 主要歌词正在播放的片段开始处
   int          mainTextBreakStart;
   /// 主要歌词正在播放的片段结束处
   int          mainTextBreakEnd;
   /// 主要歌词片段播放进度（范围：0~1）
   float        mainTextBreakProgress;

   /// 其它歌词内容
   QStringList  texts;
   /// 这些歌词的标签
   QStringList  textLabels;
   /// 其它歌词正在播放的片段开始处
   QList<int>   textBreakStarts;
   /// 其它歌词正在播放的片段结束处
   QList<int>   textBreakEnds;
   /// 其它歌词片段播放进度（范围：0~1）
   QList<float> textBreakProgresses;
};

/**
 * @brief
 * 单句歌词原始结构信息
 */
struct _LyricStatements;

/**
 * @brief
 * 用于记录操作历史记录
 */
struct _History;

/**
 * @brief
 * 歌词文件，提供歌词的解析、使用、修改和保存功能
 */
class MUSICLYRICSHARED_EXPORT LyricFile
{
public:
   LyricFile();
   LyricFile(const LyricFile& other);
   ~LyricFile();

   /**
    * @brief
    * 从文件中读取歌词信息
    * 该操作不可撤销
    * @param file
    * 要读取的文件
    * 文件将会以只读模式打开，并在读取完毕后关闭
    * @return
    * 如果成功读取，将返回 true，否则返回 false
    */
   bool readFromFile(QFile& file);

   /**
    * @brief
    * 从文件中读取歌词信息
    * 该操作不可撤销
    * 文件将会被当做纯文本文件对待
    * 每行都是一句新的歌词
    * @param file
    * 要读取的文件
    * 文件将会以只读模式打开，并在读取完毕后关闭
    * @return
    * 如果读取成功，将返回 true，否则返回 false
    */
   bool readAsTextFile(QFile& file);

   /**
    * @brief
    * 将歌词信息写出到文件
    * @param file
    * 要写出的文件
    * 文件将会以只写模式打开，并在写出后关闭
    * @return
    * 如果成功写出，将返回 true，否则返回 false
    */
   bool writeToFile(QFile& file) const;

   /**
    * @brief
    * 获取每句歌词中其它歌词最多的歌词的其它歌词数
    * @return
    */
   int getMaxOtherSize() const;

   /**
    * @brief
    * 根据时间获取当前歌词信息
    * @param lyric
    * 歌词结构体，歌词信息将写入该结构体中
    * @param time
    * 时间，以毫秒为单位
    * @return
    * 如果成功获取，将返回 true，否则返回 false。
    * 获取失败可能是因为当前播放位置在歌词起始位置之前，
    * 此时可根据 Lyric 结构体中 mainTextBreakProgress
    * 获取前奏进度。
    */
   bool lyricByTime(struct Lyric& lyric, const long& time) const;

   /**
    * @brief
    * 获取指定时间对应的歌词位置
    * @param time
    * 时间
    * @return
    * 位置。如果在首句歌词之前，将返回 -1
    */
   int lyricIndexByTime(const long& time) const;

   /**
    * @brief
    * 获取所有歌词（仅主要歌词）
    * @param stringList
    * 内容将写入该列表，写入前会清除数据
    */
   void allLyric(QStringList& stringList) const;

   /**
    * @brief
    * 获取歌词数量
    * @return
    * 歌词数量
    */
   int lyricCount() const;

   /**
    * @brief
    * 根据位置获取歌词信息
    * @param lyric
    * 歌词结构体，歌词信息将写入该结构体中。结构体中只有 mainText、
    * texts 和 textLabels 字段有意义
    * @param index
    * 位置下标，应从 0 开始
    */
   void lyricByIndex(struct Lyric& lyric, const int& index) const;

   /**
    * @brief
    * 根据下标获得歌词时间
    * @param index
    * 下标
    * @return
    * 该句歌词的开始时间
    */
   long lyricTimeByIndex(const int& index) const;

   /**
    * @brief
    * 获取全部标签
    * @return
    * 全部标签
    */
   const QMap<QString, QString>& flags() const;

   /**
    * @brief
    * 获取指定标签的值
    * @param key
    * 标签键
    * @return
    * 对应标签的值，如果不存在对应的标签，则返回 QString::Null
    */
   QString flag(const QString& key) const;

   /**
    * @brief
    * 设置（添加或改变）标签
    * @param key
    * 标签键
    * @ param value
    * 标签值
    */
   NO_UNDO
   void putFlag(const QString& key, const QString& value);

   /**
    * @brief
    * 移除指定标签
    * @param key
    * 标签键
    */
   NO_UNDO
   void removeFlag(const QString& key);

   /**
    * @brief
    * 根据位置设置主要歌词内容
    * 该操作将清除该句歌词的中间关键信息
    * @param index
    * 位置下标，从 0 开始
    * @param mainText
    * 主要歌词内容
    */
   void setMainLyricTextByIndex(const int& index, const QString& mainText);

   /**
    * @brief
    * 根据位置设置次要歌词内容
    * @param index
    * 位置下标，从 0 开始
    * @param otherText
    * 次要歌词内容
    * @param label
    * 次要歌词标签
    * @param otherIndex
    * 次要歌词下标，从 0 开始
    * @return
    * 是否成功设置。设置失败是因为没有对应下标的次要歌词
    */
   bool setOtherLyricTextByIndex(const int& index, const QString& otherText, const QString& label, const int& otherIndex);

   /**
    * @brief
    * 根据播放进度设置主要歌词内容
    * 该操作将清除该句主要歌词的中间关键点信息
    * @param time
    * 时间
    * @param mainText
    * 主要歌词内容
    * @return
    * 是否成功设置。设置失败是因为指定时间歌词不存在。
    * 例如，首句歌词开始之前的时间设置会导致失败。
    */
   bool setMainLyricTextByTime(const long& time, const QString& mainText);

   /**
    * @brief
    * 根据播放进度设置次要歌词内容
    * 该操作将清除该句对应次要歌词的中间关键信息
    * @param time
    * 时间
    * @param otherText
    * 次要歌词内容
    * @param label
    * 次要歌词标签
    * @param index
    * 次要歌词下标，从0开始
    * @return
    * 是否成功设置。设置失败是因为指定时间歌词不存在，
    * 或对应时间的歌词没有对应下标的次要歌词
    */
   bool setOtherLyricTextByTime(const long& time, const QString& otherText, const QString& label, const int& index);

   /**
    * @brief
    * 根据播放进度添加次要歌词
    * 新的内容将被添加在该句所有次要歌词之后
    * @param time
    * 时间
    * @param text
    * 次要歌词内容
    * @param label
    * 次要歌词标签
    * @return
    * 是否成功设置。设置失败是因为指定时间歌词不存在
    */
   bool addOtherLyricByTime(const long& time, const QString& text, const QString& label);

   /**
    * @brief
    * 根据位置添加次要歌词
    * 新的内容将被添加在该句所有次要歌词之后
    * @param index
    * 位置下标
    * @param text
    * 次要歌词内容
    * @param label
    * 次要歌词标签
    */
   void addOtherLyricByIndex(const int& index, const QString& text, const QString& label);

   /**
    * @brief
    * 根据位置移除次要歌词
    * @param index
    * 位置下标
    * @return
    * 是否成功移除，移除失败是因为指定位置已经没有次要歌词
    */
   bool removeOtherLyricByIndex(const int& index);

   /**
    * @brief
    * 打断歌词，将歌词拆成两句
    * 歌词拆成两句后将保留原来的时间信息
    * @param time
    * 时间，将依据时间寻找拆分的歌词，以及设置歌词的时间
    * @param mainIndex
    * 主要歌词拆分位置
    * @param indexs
    * 其余歌词拆分位置
    * @return
    * 是否成功设置。设置失败是因为指定时间歌词不存在。
    */
   NO_UNDO
   bool breakLyricText(long time, int mainIndex, QList<int> indexs);

   /**
    * @brief
    * 在指定歌词前添加新的歌词
    * @param index
    * 位置下标，从 0 开始
    * @param mainText
    * 主要歌词内容
    */
   void addBefore(int index, QString mainText);

   /**
    * @brief
    * 在指定歌词后添加新的歌词
    * @param index
    * 位置下标，从 0 开始
    * @param mainText
    * 主要歌词内容
    */
   void addAfter(int index, QString mainText);

   /**
    * @brief
    * 标记该句歌词已经结束，应当开始下一句歌词
    * 该操作将保留原来的时间信息
    * @param time
    * 时间
    */
   void finishSentence(long time);

   /**
    * @brief
    * 克隆歌词
    * 歌词将会复制一份放在其后
    * 内容完全一致
    * @param index
    * 位置下标
    */
   void cloneSentence(const int& index);

   /**
    * @brief
    * 添加主要歌词关键点
    * @param time
    * 时间
    * @param index
    * 关键点的位置下标
    */
   NO_UNDO
   void addMainBreakTime(long time, int index);

   /**
    * @brief
    * 添加其他歌词关键点
    * @param time
    * 时间
    * @param index
    * 关键点的位置下标
    * @param lyricIndex
    * 第几个歌词
    */
   NO_UNDO
   void addBreakTime(long time, int index, int lyricIndex);

   /**
    * @brief
    * 根据时间打印当句歌词的原始信息
    * @param time
    * 时间
    * @return
    * 该句歌词将写入文件的原始信息
    */
   QString printStatementByTime(const long& time) const;

   /**
    * @brief
    * 打印指定位置歌词的原始信息
    * @param index
    * 位置下标
    * @return
    * 该句歌词将写入文件的原始信息
    */
   QString printStatementByIndex(const int& index) const;

   /**
    * @brief
    * 清除全部歌词信息和历史记录
    * 该操作不可撤销
    */
   void clear();

   /**
    * @brief
    * 清除全部歌词的全部时间信息
    */
   void clearAllTime();

   /**
    * @brief
    * 根据位置清除歌词开始时间。
    * 对于最后一句而言，将时间调整为最大值；
    * 对于其它歌词而言，将时间调整为与下一句歌词相同
    * @param index
    * 位置坐标
    */
   void clearStartTimeByIndex(const int& index);

   /**
    * @brief
    * 根据位置清除主要歌词全部关键点
    * @param index
    * 位置下标
    */
   void clearMainAllKeyPointByIndex(const int& index);

   /**
    * @brief
    * 根据位置清除指定次要歌词全部关键点
    * @param index
    * 位置下标
    * @param otherIndex
    * 次要歌词下标
    * @return
    * 如果成功清除，则返回 true，否则返回 false。
    * 设置失败可能是因为指定位置没有指定的次要歌词
    */
   bool clearOtherAllKeyPointByIndex(const int& index, const int& otherIndex);

   /**
    * @brief
    * 移除指定歌词
    * @param index
    * 位置下标
    */
   void removeLyricAt(const int& index);

   /**
    * @brief
    * 根据位置清除全部歌词时间信息
    * 包括开始时间和歌词内的关键点
    * 清除开始时间是指将开始时间设置为与之后的歌词开始时间相同
    * 当之后没有歌词时，将时间设置为最大歌词时间
    * @param index
    * 位置下标
    */
   void clearAllTimeByIndex(const int& index);

   /**
    * @brief
    * 替换指定歌词内容
    * @param index
    * 位置下标
    * @param raw
    * 原始文本
    * @return
    * 如果成功解析原始文本并替换指定歌词，返回 true，否则返回 false
    */
   bool replaceStatement(const int& index, const QString& raw);

   /**
    * @brief
    * 启动历史记录功能
    * 启动历史记录功能后可以使用撤销重做方法
    */
   void enableHistory();

   /**
    * @brief
    * 设置最大历史记录大小
    * @param size
    * 大小
    */
   void setMaxHistoriesSize(const int& size);

   /**
    * @brief
    * 获取最大历史记录大小
    * @return
    * 大小
    */
   int maxHistoriesSize() const;

   /**
    * @brief
    * 撤销上一次操作
    * 需要开启历史记录功能
    * @return
    * 如果撤销成功，返回变化的下标。全局改动的撤销返回 0。
    * 如果撤销失败，返回 -1。
    */
   int undo();

   /**
    * @brief
    * 重做上次撤销的操作
    * 需要开启历史记录功能
    * @return
    * 如果重做成功，返回变化的下标。全局改动的撤销返回 0。
    * 如果重做失败，返回 -1。
    */
   int redo();

   /**
    * @brief
    * 判断当前是否可以执行撤销操作
    * @return
    * 如果存在可撤销的上一步操作，则返回 true，否则返回 false
    */
   bool canUndo() const;

   /**
    * @brief
    * 判断当前是否可以执行重做操作
    * @return
    * 如果存在可重做的下一步操作，则返回 true，否则返回 false
    */
   bool canRedo() const;

   /**
    * @brief
    * 获取撤销操作的简单介绍
    * @return
    * 上次执行的可以撤销的操作的简单介绍
    */
   QString undoText() const;

   /**
    * @brief
    * 获取重做操作的简单介绍
    * @return
    * 上次撤销的可以重做的操作的简单介绍
    */
   QString redoText() const;

   /**
    * @see #readFromFile(QFile& file);
    */
   inline bool operator<<(QFile& file);

   /**
    * @see #writeToFile(QFile& file) const;
    */
   inline bool operator>>(QFile& file) const;

   /**
    * @see #flag(const QString& key) const;
    */
   inline QString operator[](const QString& key) const;

   LyricFile& operator=(const LyricFile& other);

private:
   QList<_LyricStatements *> _statements;
   QMap<QString, QString> _flags;
   QList<_History *> _histories;
   bool _enableHistory   = false;
   int _maxHistoriesSize = 10;
   int _historyPointer   = 0;
};
}


inline bool cszt::LyricFile::operator<<(QFile& file)
{
   return readFromFile(file);
}


inline bool cszt::LyricFile::operator>>(QFile& file) const
{
   return writeToFile(file);
}


inline QString cszt::LyricFile::operator[](const QString& key) const
{
   return flag(key);
}
