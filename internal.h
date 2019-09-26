#pragma once

#include "lyricfile.h"

/**
 * @brief
 * 单句歌词原始结构信息
 */
struct cszt::_LyricStatements
{
   /// 开始时间
   long                startTime;

   /// 主要歌词内容
   QString             mainText;
   /// 主要歌词关键点位置
   QList<int>          mainKeyPoints;
   /// 主要歌词关键点时间
   QList<long>         mainKeyTimes;

   /// 其它歌词内容
   QStringList         otherTexts;
   /// 其它歌词标签
   QStringList         otherTextLabels;
   /// 其它歌词关键点内容
   QList<QList<int> >  otherTextPoints;
   /// 其它歌词关键点时间
   QList<QList<long> > otherKeyTimes;
};

/**
 * @brief
 * 用于记录操作历史记录
 */
struct cszt::_History
{
   /// 操作类型
   uint    opType;
   /// 简要介绍该操作
   QString text;
   /// 为复原该操作所需的参数
   void    **params;
   /// 析构函数
   ~_History();
};


/// 撤销操作类型
/// 增加、修改撤销操作需要修改以下方法的代码：
/// 1. 对应操作的函数
/// 2. undo、redo函数
/// 3. cloneHistory 函数
/// 4. 析构函数


/**
 * 操作类型：改变歌词开始时间
 * 参数：3个
 * 0 - 修改的位置下标 int
 * 1 - 原时间 long
 * 2 - 新时间 long
 */
#define OPTYPE_CHANGE_START_TIME    1

/**
 * 操作类型：改变主要歌词
 * 参数：5个
 * 0 - 修改的位置下标 int
 * 1 - 原歌词 QString
 * 2 - 新歌词 QString
 * 3 - 原歌词对应的中断点 QList<int>
 * 4 - 原歌词对应的中断点时间 QList<long>
 */
#define OPTYPE_CHANGE_MAIN_TEXT     2

/**
 * 操作类型：改变次要歌词
 * 参数：8个
 * 0 - 修改位置下标 int
 * 1 - 修改的哪一个歌词 int
 * 2 - 原歌词 QString
 * 3 - 原歌词标签 QString
 * 4 - 新歌词 QString
 * 5 - 新歌词标签 QString
 * 6 - 原歌词对应的中断点 QList<int>
 * 7 - 原歌词对应的中断点时间 QList<long>
 */
#define OPTYPE_CHANGE_OTHER_TEXT    3

/**
 * 操作类型：添加次要歌词
 * 参数：3个
 * 0 - 修改位置下标 int
 * 1 - 次要歌词内容 QString
 * 2 - 次要歌词标签 QString
 */
#define OPTYPE_ADD_OTHER_TEXT       4

/**
 * 操作类型：移除次要歌词
 * 参数：5个
 * 0 - 修改位置下标 int
 * 1 - 次要歌词内容 QString
 * 2 - 次要歌词标签 QString
 * 3 - 次要歌词关键点位置 QList<int>
 * 4 - 次要歌词关键点时间 QList<long>
 */
#define OPTYPE_REMOVE_OTHER_TEXT    5

/**
 * 操作类型：克隆歌词
 * 参数：1个
 * 0 - 被克隆位置下标 int
 */
#define OPTYPE_CLONE                6


/**
 * 操作类型：清除主要歌词全部关键点
 * 参数：3个
 * 0 - 清除的位置下标 int
 * 1 - 歌词关键点位置 QList<int>
 * 2 - 歌词关键点时间 QList<long>
 */
#define OPTYPE_CLEAR_MAIN_ALL_KEY_POINT     7

/**
 * 操作类型：清除指定次要歌词全部关键点
 * 参数：4个
 * 0 - 清除的位置下标 int
 * 1 - 次要歌词下标 int
 * 2 - 歌词关键点位置 QList<int>
 * 3 - 歌词关键点时间 QList<long>
 */
#define OPTYPE_CLEAR_OTHER_ALL_KEY_POINT    8

/**
 * 操作类型：移除指定歌词
 * 参数：2个
 * 0 - 移除歌词的位置 int
 * 1 - 移除歌词的内容 _LyricStatement
 */
#define OPTYPE_REMOVE_LYRIC                 9

/**
 * 操作类型：清除全部歌词信息
 * 参数：6个
 * 0 - 清除的位置下标 int
 * 1 - 歌词开始时间 long
 * 2 - 主要歌词关键点位置 QList<int>
 * 3 - 主要歌词关键点时间 QList<long>
 * 4 - 次要歌词关键点位置 QList<QList<int>>
 * 5 - 次要歌词关键点时间 QList<QList<long>>
 */
#define OPTYPE_CLEAR_ALL_TIME               10

/**
 * 操作类型：清除全部歌词的全部时间信息
 * 参数：5个
 * 0 - 歌词开始时间 QList<long>
 * 1 - 主要歌词关键点位置 QList<QList<int>>
 * 2 - 主要歌词关键点时间 QList<QList<long>>
 * 3 - 次要歌词关键点位置 QList<QList<QList<int>>>
 * 4 - 次要歌词关键点时间 QList<QList<QList<long>>>
 */
#define OPTYPE_CLEAR_ALL                    11

/**
 * 操作类型：插入歌词
 * 参数：2个
 * 0 - 插入位置 int
 * 1 - 主要歌词内容
 */
#define OPTYPE_INSERT                       12

/**
 * 操作类型：替换歌词
 * 参数：3个
 * 0 - 替换位置 int
 * 1 - 原歌词内容 _LyricStatement
 * 2 - 新歌词内容 _LyricStatement
 */
#define OPTYPE_REPLACE                      13
