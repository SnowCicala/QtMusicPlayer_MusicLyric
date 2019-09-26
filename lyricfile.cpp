#include "lyricfile.h"
#include "internal.h"
#include <QDebug>

// 最大歌词时间
#define MAX_LYRIC_TIME    (static_cast<long>(static_cast<unsigned long>(static_cast<long>(-1)) >> 1))
// 历史记录参数用的类型转换
#define CAST(TYPE, value)    reinterpret_cast<TYPE *>(value)
// 克隆历史记录时用的参数复制方法
#define COPY(TYPE, INDEX)    *CAST(TYPE, newHistory->params[INDEX]) = *CAST(TYPE, history->params[INDEX])

void pushHistory(cszt::_History *history, QList<cszt::_History *>& histories, const int& max, int& historyPointer)
{
   if (max <= 0)
   {
      delete history;
      return;
   }
   // 清除已撤销项
   while (histories.size() > historyPointer)
   {
      delete histories[historyPointer];
      histories.removeAt(historyPointer);
   }
   // 如果历史记录已达上限，则清除最早的记录
   if (histories.size() == max)
   {
      delete histories[0];
      histories.removeAt(0);
   }
   // 压入历史记录
   histories.append(history);
   ++historyPointer;
}


/**
 * @brief
 * 复制用于撤销重做的历史记录
 * @param history
 * 原历史记录
 * @return
 * 新的历史记录
 */
cszt::_History *cloneHistory(cszt::_History *history)
{
   cszt::_History *newHistory = new cszt::_History;

   newHistory->opType = history->opType;
   newHistory->text   = history->text;
   switch (history->opType)
   {
   case OPTYPE_CHANGE_START_TIME:
      newHistory->params    = new void *[3];
      newHistory->params[0] = new int;
      newHistory->params[1] = new long;
      newHistory->params[2] = new long;
      COPY(int, 0);
      COPY(long, 1);
      COPY(long, 1);
      break;

   case OPTYPE_CHANGE_MAIN_TEXT:
      newHistory->params    = new void *[5];
      newHistory->params[0] = new int;
      newHistory->params[1] = new QString;
      newHistory->params[2] = new QString;
      newHistory->params[3] = new QList<int>;
      newHistory->params[4] = new QList<long>;
      COPY(int, 0);
      COPY(QString, 1);
      COPY(QString, 2);
      COPY(QList<int>, 3);
      COPY(QList<long>, 4);
      break;

   case OPTYPE_CHANGE_OTHER_TEXT:
      newHistory->params    = new void *[8];
      newHistory->params[0] = new int;
      newHistory->params[1] = new int;
      newHistory->params[2] = new QString;
      newHistory->params[3] = new QString;
      newHistory->params[4] = new QString;
      newHistory->params[5] = new QString;
      newHistory->params[6] = new QList<int>;
      newHistory->params[7] = new QList<long>;
      COPY(int, 0);
      COPY(int, 1);
      COPY(QString, 2);
      COPY(QString, 3);
      COPY(QString, 4);
      COPY(QString, 5);
      COPY(QList<int>, 6);
      COPY(QList<long>, 7);
      break;

   case OPTYPE_ADD_OTHER_TEXT:
      newHistory->params    = new void *[3];
      newHistory->params[0] = new int;
      newHistory->params[1] = new QString;
      newHistory->params[2] = new QString;
      COPY(int, 0);
      COPY(QString, 1);
      COPY(QString, 2);
      break;

   case OPTYPE_REMOVE_OTHER_TEXT:
      newHistory->params    = new void *[5];
      newHistory->params[0] = new int;
      newHistory->params[1] = new QString;
      newHistory->params[2] = new QString;
      newHistory->params[3] = new QList<int>;
      newHistory->params[4] = new QList<long>;
      COPY(int, 0);
      COPY(QString, 1);
      COPY(QString, 2);
      COPY(QList<int>, 3);
      COPY(QList<long>, 4);
      break;

   case OPTYPE_CLONE:
      newHistory->params    = new void *[1];
      newHistory->params[0] = new int;
      COPY(int, 0);
      break;

   case OPTYPE_CLEAR_MAIN_ALL_KEY_POINT:
      newHistory->params    = new void *[3];
      newHistory->params[0] = new int;
      newHistory->params[1] = new QList<int>;
      newHistory->params[2] = new QList<long>;
      COPY(int, 0);
      COPY(QList<int>, 1);
      COPY(QList<long>, 2);
      break;

   case OPTYPE_CLEAR_OTHER_ALL_KEY_POINT:
      newHistory->params    = new void *[4];
      newHistory->params[0] = new int;
      newHistory->params[1] = new int;
      newHistory->params[2] = new QList<int>;
      newHistory->params[3] = new QList<long>;
      COPY(int, 0);
      COPY(int, 1);
      COPY(QList<int>, 2);
      COPY(QList<long>, 3);
      break;

   case OPTYPE_REMOVE_LYRIC:
      newHistory->params    = new void *[2];
      newHistory->params[0] = new int;
      newHistory->params[1] = new cszt::_LyricStatements;
      COPY(int, 0);
      COPY(cszt::_LyricStatements, 1);
      break;

   case OPTYPE_CLEAR_ALL_TIME:
      newHistory->params    = new void *[6];
      newHistory->params[0] = new int;
      newHistory->params[1] = new long;
      newHistory->params[2] = new QList<int>;
      newHistory->params[3] = new QList<long>;
      newHistory->params[4] = new QList<QList<int> >;
      newHistory->params[5] = new QList<QList<long> >;
      COPY(int, 0);
      COPY(long, 1);
      COPY(QList<int>, 2);
      COPY(QList<long>, 3);
      COPY(QList<QList<int> >, 4);
      COPY(QList<QList<long> >, 5);
      break;

   case OPTYPE_CLEAR_ALL:
      newHistory->params    = new void *[5];
      newHistory->params[0] = new QList<long>;
      newHistory->params[1] = new QList<QList<int> >;
      newHistory->params[2] = new QList<QList<long> >;
      newHistory->params[3] = new QList<QList<QList<int> > >;
      newHistory->params[4] = new QList<QList<QList<long> > >;
      COPY(QList<long>, 0);
      COPY(QList<QList<int> >, 1);
      COPY(QList<QList<long> >, 2);
      COPY(QList<QList<QList<int> > >, 3);
      COPY(QList<QList<QList<long> > >, 4);
      break;

   case OPTYPE_INSERT:
      newHistory->params    = new void *[2];
      newHistory->params[0] = new int;
      newHistory->params[1] = new QString;
      COPY(int, 0);
      COPY(QString, 1);
      break;

   case OPTYPE_REPLACE:
      newHistory->params    = new void *[3];
      newHistory->params[0] = new int;
      newHistory->params[1] = new cszt::_LyricStatements;
      newHistory->params[2] = new cszt::_LyricStatements;
      COPY(int, 0);
      COPY(cszt::_LyricStatements, 1);
      COPY(cszt::_LyricStatements, 2);
      break;

   default:
      abort();
   }
   return newHistory;
}


/**
 * @brief 将匹配到的时间标签转化为时间
 * @param capturedTexts 匹配到的时间标签
 * @param start 时间的开始位置
 * @return
 */
inline long asTime(const QStringList& capturedTexts, const int& start)
{
   return capturedTexts [start].toLong() * 60 * 1000 +
          capturedTexts [start + 1].toInt() * 1000 +
          capturedTexts [start + 2].toInt();
}


/**
 * @brief 将时间转化为时间标签
 * @param time 时间
 * @return 时间标签
 */
inline QString asTimeLabel(long time)
{
   return QString("[%1:%2.%3]")
             .arg(time / 60 / 1000)
             .arg(time / 1000 % 60)
             .arg(time % 1000);
}


/**
 * @brief 获取单句歌词的原始文本，既可以是主要歌词（需在前面加开始时间），也可以是其它歌词（需在前面加标签）
 * @param text 歌词内容
 * @param keyPoints 关键点位置
 * @param keyTimes 关键点时间
 * @return 该句歌词的原始文本
 */
QString getSingleStatementRawText(const QString& text, const QList<int>& keyPoints, const QList<long>& keyTimes)
{
   QString str("");

   int lastBreakPoint = 0;
   int keyPointSize   = keyPoints.size();

   for (int i = 0; i < keyPointSize; i++)
   {
      str           += text.mid(lastBreakPoint, keyPoints[i] - lastBreakPoint);
      str           += asTimeLabel(keyTimes[i]);
      lastBreakPoint = keyPoints[i];
   }
   str += text.mid(keyPoints[keyPointSize - 1]);
   str += '\n';
   return str;
}


/**
 * @brief 获取单句歌词的完整原始文本
 * @param statement 歌词
 * @return 完整原始文本
 */
QString getStatementRawText(cszt::_LyricStatements *statement)
{
   QString str("");

   // 输出主歌词
   str += asTimeLabel(statement->startTime);
   str += getSingleStatementRawText(statement->mainText, statement->mainKeyPoints, statement->mainKeyTimes);

   // 输出各项次要歌词
   int otherTextSize = statement->otherTexts.size();
   for (int i = 0; i < otherTextSize; i++)
   {
      QString singleRaw = getSingleStatementRawText(statement->otherTexts[i], statement->otherTextPoints[i], statement->otherKeyTimes[i]);
      // 当且仅当标签不为空，且不以'['开头时，输出标签
      if (!statement->otherTextLabels[i].isEmpty() || singleRaw.startsWith('['))
      {
         str += "<" + statement->otherTextLabels[i] + ">";
      }
      str += singleRaw;
   }

   return str;
}


/**
 * @brief 获取歌词播放进度
 * @param outStartIndex 返回歌词片段开始位置
 * @param outEndIndex 返回歌词片段结束位置
 * @param outProgress 返回歌词片段进度
 * @param startTime 歌词开始时间
 * @param endTime 歌词结束时间
 * @param currentTime 当前时间
 * @param points 关键点
 * @param times 关键点时间
 * @param textSize 歌词长度
 */
void getKeyPointProgress(int& outStartIndex, int& outEndIndex, float& outProgress, const long& startTime, const long& endTime, const long& currentTime, const QList<int>& points, const QList<long>& times, const int& textSize)
{
   long progressStartTime = startTime;
   long progressEndTime   = endTime;

   outStartIndex = 0;
   outEndIndex   = textSize;

   int count = points.size();

   for (int i = 0; i < count; i++)
   {
      long breakTime = times[i];
      if (currentTime >= breakTime)
      {
         progressStartTime = breakTime;
         outStartIndex     = points[i];
      }
      else
      {
         progressEndTime = breakTime;
         outEndIndex     = points[i];
         break;
      }
   }

   outProgress = (currentTime - progressStartTime) * 1.0f / (progressEndTime - progressStartTime);
}


cszt::LyricFile::LyricFile() = default;

cszt::LyricFile::LyricFile(const LyricFile& other)
{
   for (_LyricStatements *lyricStatementsPtr : other._statements)
   {
      _LyricStatements *clonePtr = new _LyricStatements;

      *clonePtr = *lyricStatementsPtr;
      _statements << clonePtr;
   }
   _flags = other._flags;
   for (_History *history : other._histories)
   {
      _History *newHistory = cloneHistory(history);
      _histories << newHistory;
   }
   _enableHistory    = other._enableHistory;
   _maxHistoriesSize = other._maxHistoriesSize;
}


cszt::LyricFile::~LyricFile()
{
   for (_LyricStatements *lyricStatementsPtr : _statements)
   {
      delete lyricStatementsPtr;
   }
   for (_History *history:_histories)
   {
      delete history;
   }
   _statements.clear();
   _histories.clear();
}


cszt::_History::~_History()
{
   switch (opType)
   {
   case OPTYPE_CHANGE_START_TIME:
      delete CAST(int, params[0]);
      delete CAST(long, params[1]);
      delete CAST(long, params[2]);
      break;

   case OPTYPE_CHANGE_MAIN_TEXT:
      delete CAST(int, params[0]);
      delete CAST(QString, params[1]);
      delete CAST(QString, params[2]);
      delete CAST(QList<int>, params[3]);
      delete CAST(QList<long>, params[4]);
      break;

   case OPTYPE_CHANGE_OTHER_TEXT:
      delete CAST(int, params[0]);
      delete CAST(int, params[1]);
      delete CAST(QString, params[2]);
      delete CAST(QString, params[3]);
      delete CAST(QString, params[4]);
      delete CAST(QString, params[5]);
      delete CAST(QList<int>, params[6]);
      delete CAST(QList<long>, params[7]);
      break;

   case OPTYPE_ADD_OTHER_TEXT:
      delete CAST(int, params[0]);
      delete CAST(QString, params[1]);
      delete CAST(QString, params[2]);
      break;

   case OPTYPE_REMOVE_OTHER_TEXT:
      delete CAST(int, params[0]);
      delete CAST(QString, params[1]);
      delete CAST(QString, params[2]);
      delete CAST(QList<int>, params[3]);
      delete CAST(QList<long>, params[4]);
      break;

   case OPTYPE_CLONE:
      delete CAST(int, params[0]);
      break;

   case OPTYPE_CLEAR_MAIN_ALL_KEY_POINT:
      delete CAST(int, params[0]);
      delete CAST(QList<int>, params[1]);
      delete CAST(QList<long>, params[2]);
      break;

   case OPTYPE_CLEAR_OTHER_ALL_KEY_POINT:
      delete CAST(int, params[0]);
      delete CAST(int, params[1]);
      delete CAST(QList<int>, params[2]);
      delete CAST(QList<long>, params[3]);
      break;

   case OPTYPE_REMOVE_LYRIC:
      delete CAST(int, params[0]);
      delete CAST(_LyricStatements, params[1]);
      break;

   case OPTYPE_CLEAR_ALL_TIME:
      delete CAST(int, params[0]);
      delete CAST(long, params[1]);
      delete CAST(QList<int>, params[2]);
      delete CAST(QList<long>, params[3]);
      delete CAST(QList<QList<int> >, params[4]);
      delete CAST(QList<QList<long> >, params[5]);
      break;

   case OPTYPE_CLEAR_ALL:
      delete CAST(QList<long>, params[0]);
      delete CAST(QList<QList<int> >, params[1]);
      delete CAST(QList<QList<long> >, params[2]);
      delete CAST(QList<QList<QList<int> > >, params[3]);
      delete CAST(QList<QList<QList<long> > >, params[4]);
      break;

   case OPTYPE_INSERT:
      delete CAST(int, params[0]);
      delete CAST(QString, params[1]);
      break;

   case OPTYPE_REPLACE:
      delete CAST(int, params[0]);
      delete CAST(_LyricStatements, params[1]);
      delete CAST(_LyricStatements, params[2]);
      break;

   default:
      abort();
   }
   delete[] params;
}


bool cszt::LyricFile::readFromFile(QFile& file)
{
   // 清空对象
   for (_LyricStatements *lyricStatementsPtr : _statements)
   {
      delete lyricStatementsPtr;
   }
   _statements.clear();
   _flags.clear();
   // 打开文件
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      return false;
   }

   // 逐行读取
   QRegExp          labelRegexp("\\[\\s*(\\w+)\\s*:\\s*([^\\]]+)\\s*\\](\n|\r)*");               // 寻找元信息标签所用的正则表达式
   QRegExp          timeRegexp("\\[\\s*(\\d+)\\s*:\\s*(\\d+)\\s*\\.\\s*(\\d+)\\s*\\]([^\\[]*)"); // 寻找时间标签所用的正则表达式
   QString          lineSource;                                                                  // 每行的原始内容（去掉左右空格）
   int              timeLabelIndex;                                                              // 找到的时间标签的位置
   _LyricStatements *lastStatement = nullptr;                                                    // 一句歌词
   QStringList      capturedTexts;                                                               // 正则表达式匹配到的内容

   while (!file.atEnd())
   {
      lineSource = QString(file.readLine()).trimmed();
      // 忽略空字符串
      if (lineSource.isEmpty())
      {
         continue;
      }
      // 元信息
      if (labelRegexp.exactMatch(lineSource))
      {
         capturedTexts = labelRegexp.capturedTexts();

         QString key   = capturedTexts.at(1).trimmed();
         QString value = capturedTexts.at(2).trimmed();

         _flags[key] = value;
         continue;
      }
      // 主要歌词内容
      timeLabelIndex = timeRegexp.indexIn(lineSource);
      if (timeLabelIndex == 0)
      {
         lastStatement            = new _LyricStatements;
         capturedTexts            = timeRegexp.capturedTexts();
         lastStatement->startTime = asTime(capturedTexts, 1);
         lastStatement->mainText  = capturedTexts [4];
         // 转化其它关键点信息
         while ((timeLabelIndex = timeRegexp.indexIn(lineSource, timeLabelIndex + 1)) != -1)
         {
            capturedTexts = timeRegexp.capturedTexts();
            lastStatement->mainKeyTimes << asTime(capturedTexts, 1);
            lastStatement->mainKeyPoints << lastStatement->mainText.size();
            lastStatement->mainText += capturedTexts [4];
         }
         _statements << lastStatement;
         continue;
      }
      // 次要歌词内容
      if (lastStatement == nullptr)
      {
         // 当且仅当有主要歌词，才能有次要歌词
         continue;
      }

      // 基本信息
      QString     text;
      QString     label;
      QList<int>  keyPoints;
      QList<long> keyTimes;

      // 如果没有时间标签，那么表示没有关键点信息
      if (timeLabelIndex == -1)
      {
         text = lineSource;
         // 检查是否含有标签
         int labelIndex = -1;
         if ((text[0] == '<') && ((labelIndex = text.indexOf('>')) != -1))
         {
            label = text.mid(1, labelIndex - 1);
            text  = text.mid(labelIndex + 1);
         }
      }
      else
      {
         // 先把时间标签之前的内容加上
         text = lineSource.mid(0, timeLabelIndex);
         // 检查是否含有标签
         int labelIndex = -1;
         if ((text[0] == '<') && ((labelIndex = text.indexOf('>')) != -1))
         {
            label = text.mid(1, labelIndex - 1);
            text  = text.mid(labelIndex + 1);
         }
         // 循环遍历关键点
         do
         {
            capturedTexts = timeRegexp.capturedTexts();
            keyTimes << asTime(capturedTexts, 1);
            keyPoints << text.size();
            text += capturedTexts[4];
         } while ((timeLabelIndex = timeRegexp.indexIn(lineSource, timeLabelIndex + 1)) != -1);
      }

      lastStatement->otherTexts << text;
      lastStatement->otherTextLabels << label;
      lastStatement->otherKeyTimes << keyTimes;
      lastStatement->otherTextPoints << keyPoints;
   }
   file.close();
   return true;
}


bool cszt::LyricFile::readAsTextFile(QFile& file)
{
   // 清空对象
   for (_LyricStatements *lyricStatementsPtr : _statements)
   {
      delete lyricStatementsPtr;
   }
   _statements.clear();
   _flags.clear();
   // 打开文件
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      return false;
   }

   // 逐行读取
   QString          lineSource;
   _LyricStatements *statement;
   while (!file.atEnd())
   {
      lineSource = file.readLine().trimmed();
      // 去掉空行
      if (lineSource.isEmpty())
      {
         continue;
      }
      statement            = new _LyricStatements;
      statement->mainText  = lineSource;
      statement->startTime = MAX_LYRIC_TIME;
   }
   return true;
}


bool cszt::LyricFile::writeToFile(QFile& file) const
{
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
   {
      return false;
   }
   // 输出标签
   QMapIterator<QString, QString> flags(_flags);
   while (flags.hasNext())
   {
      flags.next();
      file.write(("[" + flags.key() + ":" + flags.value() + "]\n").toUtf8());
   }
   // 输出歌词
   for (_LyricStatements *statements:_statements)
   {
      file.write(getStatementRawText(statements).toUtf8());
   }
   file.close();
   return true;
}


int cszt::LyricFile::getMaxOtherSize() const
{
   int max = 0;

   for (_LyricStatements *statements:_statements)
   {
      if (statements->otherTexts.size() > max)
      {
         max = statements->otherTexts.size();
      }
   }
   return max;
}


bool cszt::LyricFile::lyricByTime(struct Lyric& lyric, const long& time) const
{
   _LyricStatements *playingStatement = nullptr;
   long             startTime         = 0;
   long             endTime           = 0;

   for (_LyricStatements *statements: _statements)
   {
      if (time > statements->startTime)
      {
         startTime        = statements->startTime;
         endTime          = startTime + 5000;
         playingStatement = statements;
      }
      else
      {
         endTime = statements->startTime;
         break;
      }
   }

   // 没有找到歌词的情况下
   if (playingStatement == nullptr)
   {
      if (_statements.isEmpty())
      {
         lyric.mainTextBreakProgress = time / 5000.0f;
      }
      else
      {
         lyric.mainTextBreakProgress = time * 1.0f / _statements.first()->startTime;
      }
      return false;
   }

   lyric.mainText   = playingStatement->mainText;
   lyric.texts      = playingStatement->otherTexts;
   lyric.textLabels = playingStatement->otherTextLabels;
   lyric.textBreakStarts.clear();
   lyric.textBreakEnds.clear();
   lyric.textBreakProgresses.clear();

   // 寻找歌词关键点
   getKeyPointProgress(lyric.mainTextBreakStart, lyric.mainTextBreakEnd, lyric.mainTextBreakProgress, startTime, endTime, time, playingStatement->mainKeyPoints, playingStatement->mainKeyTimes, playingStatement->mainText.size());
   int otherTextSize = playingStatement->otherTextLabels.size();
   for (int i = 0; i < otherTextSize; i++)
   {
      int   start, end;
      float progress;
      getKeyPointProgress(start, end, progress, startTime, endTime, time, playingStatement->otherTextPoints[i], playingStatement->otherKeyTimes[i], playingStatement->otherTexts[i].size());
      lyric.textBreakStarts << start;
      lyric.textBreakEnds << end;
      lyric.textBreakProgresses << progress;
   }
   return true;
}


int cszt::LyricFile::lyricIndexByTime(const long& time) const
{
   int index = -1;

   for (_LyricStatements *statement:_statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      ++index;
   }
   return index;
}


void cszt::LyricFile::allLyric(QStringList& stringList) const
{
   stringList.clear();
   for (_LyricStatements *statements:_statements)
   {
      stringList << statements->mainText;
   }
}


int cszt::LyricFile::lyricCount() const
{
   return _statements.count();
}


void cszt::LyricFile::lyricByIndex(struct Lyric& lyric, const int& index) const
{
   _LyricStatements *statements = _statements[index];

   lyric.mainText   = statements->mainText;
   lyric.texts      = statements->otherTexts;
   lyric.textLabels = statements->otherTextLabels;
}


long cszt::LyricFile::lyricTimeByIndex(const int& index) const
{
   return _statements[index]->startTime;
}


const QMap<QString, QString>& cszt::LyricFile::flags() const
{
   return _flags;
}


QString cszt::LyricFile::flag(const QString& key) const
{
   return _flags[key];
}


void cszt::LyricFile::putFlag(const QString& key, const QString& value)
{
   _flags[key] = value;
}


void cszt::LyricFile::removeFlag(const QString& key)
{
   _flags.remove(key);
}


void cszt::LyricFile::setMainLyricTextByIndex(const int& index, const QString& mainText)
{
   _LyricStatements *statements = _statements[index];

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                        = OPTYPE_CHANGE_MAIN_TEXT;
      history->text                          = "修改主要歌词";
      history->params                        = new void *[5];
      history->params[0]                     = new int;
      history->params[1]                     = new QString;
      history->params[2]                     = new QString;
      history->params[3]                     = new QList<int>;
      history->params[4]                     = new QList<long>;
      *CAST(int, history->params[0])         = index;
      *CAST(QString, history->params[1])     = statements->mainText;
      *CAST(QString, history->params[2])     = mainText;
      *CAST(QList<int>, history->params[3])  = statements->mainKeyPoints;
      *CAST(QList<long>, history->params[4]) = statements->mainKeyTimes;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statements->mainText = mainText;
   statements->mainKeyPoints.clear();
   statements->mainKeyTimes.clear();
}


bool cszt::LyricFile::setOtherLyricTextByIndex(const int& index, const QString& otherText, const QString& label, const int& otherIndex)
{
   _LyricStatements *statements = _statements[index];

   if (statements->otherTexts.size() <= otherIndex)
   {
      return false;
   }
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                        = OPTYPE_CHANGE_OTHER_TEXT;
      history->text                          = "修改次要歌词";
      history->params                        = new void *[8];
      history->params[0]                     = new int;
      history->params[1]                     = new int;
      history->params[2]                     = new QString;
      history->params[3]                     = new QString;
      history->params[4]                     = new QString;
      history->params[5]                     = new QString;
      history->params[6]                     = new QList<int>;
      history->params[7]                     = new QList<long>;
      *CAST(int, history->params[0])         = index;
      *CAST(int, history->params[1])         = otherIndex;
      *CAST(QString, history->params[2])     = statements->otherTexts[otherIndex];
      *CAST(QString, history->params[3])     = statements->otherTextLabels[otherIndex];
      *CAST(QString, history->params[4])     = otherText;
      *CAST(QString, history->params[5])     = label;
      *CAST(QList<int>, history->params[6])  = statements->otherTextPoints[otherIndex];
      *CAST(QList<long>, history->params[7]) = statements->otherKeyTimes[otherIndex];
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statements->otherTexts[otherIndex]      = otherText;
   statements->otherTextLabels[otherIndex] = label;
   statements->otherTextPoints[otherIndex].clear();
   statements->otherKeyTimes[otherIndex].clear();
   return true;
}


bool cszt::LyricFile::setMainLyricTextByTime(const long& time, const QString& mainText)
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement : _statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }
   if (statements)
   {
      if (_enableHistory)
      {
         _History *history = new _History;
         history->opType                        = OPTYPE_CHANGE_MAIN_TEXT;
         history->text                          = "修改主要歌词";
         history->params                        = new void *[5];
         history->params[0]                     = new int;
         history->params[1]                     = new QString;
         history->params[2]                     = new QString;
         history->params[3]                     = new QList<int>;
         history->params[4]                     = new QList<long>;
         *CAST(int, history->params[0])         = _statements.indexOf(statements);
         *CAST(QString, history->params[1])     = statements->mainText;
         *CAST(QString, history->params[2])     = mainText;
         *CAST(QList<int>, history->params[3])  = statements->mainKeyPoints;
         *CAST(QList<long>, history->params[4]) = statements->mainKeyTimes;
         pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
      }
      statements->mainText = mainText;
      statements->mainKeyPoints.clear();
      statements->mainKeyTimes.clear();
      return true;
   }
   return false;
}


bool cszt::LyricFile::setOtherLyricTextByTime(const long& time, const QString& otherText, const QString& label, const int& index)
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement : _statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }
   if (statements)
   {
      if (statements->otherTexts.size() <= index)
      {
         return false;
      }
      if (_enableHistory)
      {
         _History *history = new _History;
         history->opType                        = OPTYPE_CHANGE_OTHER_TEXT;
         history->text                          = "修改次要歌词";
         history->params                        = new void *[8];
         history->params[0]                     = new int;
         history->params[1]                     = new int;
         history->params[2]                     = new QString;
         history->params[3]                     = new QString;
         history->params[4]                     = new QString;
         history->params[5]                     = new QString;
         history->params[6]                     = new QList<int>;
         history->params[7]                     = new QList<long>;
         *CAST(int, history->params[0])         = _statements.indexOf(statements);
         *CAST(int, history->params[1])         = index;
         *CAST(QString, history->params[2])     = statements->otherTexts[index];
         *CAST(QString, history->params[3])     = statements->otherTextLabels[index];
         *CAST(QString, history->params[4])     = otherText;
         *CAST(QString, history->params[5])     = label;
         *CAST(QList<int>, history->params[6])  = statements->otherTextPoints[index];
         *CAST(QList<long>, history->params[7]) = statements->otherKeyTimes[index];
         pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
      }
      statements->otherTexts[index]      = otherText;
      statements->otherTextLabels[index] = label;
      statements->otherTextPoints[index].clear();
      statements->otherKeyTimes[index].clear();
      return true;
   }
   return false;
}


bool cszt::LyricFile::addOtherLyricByTime(const long& time, const QString& text, const QString& label)
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement:_statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }

   if (statements)
   {
      if (_enableHistory)
      {
         _History *history = new _History;
         history->opType                    = OPTYPE_ADD_OTHER_TEXT;
         history->text                      = "添加次要歌词音轨";
         history->params                    = new void *[3];
         history->params[0]                 = new int;
         history->params[1]                 = new QString;
         history->params[2]                 = new QString;
         *CAST(int, history->params[0])     = _statements.indexOf(statements);
         *CAST(QString, history->params[1]) = text;
         *CAST(QString, history->params[2]) = label;
         pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
      }
      statements->otherTexts << text;
      statements->otherTextLabels << label;
      statements->otherTextPoints << QList<int>();
      statements->otherKeyTimes << QList<long>();
      return true;
   }
   return false;
}


void cszt::LyricFile::addOtherLyricByIndex(const int& index, const QString& text, const QString& label)
{
   _LyricStatements *statements = _statements[index];

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                    = OPTYPE_ADD_OTHER_TEXT;
      history->text                      = "添加次要歌词音轨";
      history->params                    = new void *[3];
      history->params[0]                 = new int;
      history->params[1]                 = new QString;
      history->params[2]                 = new QString;
      *CAST(int, history->params[0])     = _statements.indexOf(statements);
      *CAST(QString, history->params[1]) = text;
      *CAST(QString, history->params[2]) = label;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statements->otherTexts << text;
   statements->otherTextLabels << label;
   statements->otherTextPoints << QList<int>();
   statements->otherKeyTimes << QList<long>();
}


bool cszt::LyricFile::removeOtherLyricByIndex(const int& index)
{
   _LyricStatements *statements = _statements[index];

   if (statements->otherTexts.size() == 0)
   {
      return false;
   }
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                        = OPTYPE_REMOVE_OTHER_TEXT;
      history->text                          = "缩减音轨";
      history->params                        = new void *[5];
      history->params[0]                     = new int;
      history->params[1]                     = new QString;
      history->params[2]                     = new QString;
      history->params[3]                     = new QList<int>;
      history->params[4]                     = new QList<long>;
      *CAST(int, history->params[0])         = index;
      *CAST(QString, history->params[1])     = statements->otherTexts.last();
      *CAST(QString, history->params[2])     = statements->otherTextLabels.last();
      *CAST(QList<int>, history->params[3])  = statements->otherTextPoints.last();
      *CAST(QList<long>, history->params[4]) = statements->otherKeyTimes.last();
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statements->otherTexts.removeLast();
   statements->otherTextLabels.removeLast();
   statements->otherTextPoints.removeLast();
   statements->otherKeyTimes.removeLast();
   return true;
}


/**
 * @brief 打断单句歌词
 * @param index
 * 拆分位置
 * @param originalText
 * 原始歌词，同时保存前半句歌词
 * @param originalPoints
 * 原始关键点位置，同时保存前半句关键点位置
 * @param originalTimes
 * 原始关键点时间，同时保存前半句关键点时间
 * @param newText
 * 保存后半句歌词
 * @param newPoints
 * 保存后半句关键点
 * @param newTimes
 * 保存后半句时间
 */
void breakStatement(const int& index, QString& originalText, QList<int>& originalPoints, QList<long>& originalTimes, QString& newText, QList<int>& newPoints, QList<long>& newTimes)
{
   // 清空内容
   newPoints.clear();
   newTimes.clear();
   // 歌词内容
   newText      = originalText.mid(index);
   originalText = originalText.mid(0, index);
   // 关键点开始复制位置
   int startCopyIndex = -1;
   int size           = originalPoints.size();
   if (size == 0)
   {
      // 关键点为空，直接退出
      return;
   }
   for (int i = 0; i < size; i++)
   {
      if (originalPoints[i] >= index)
      {
         startCopyIndex = i;
         break;
      }
   }
   if (startCopyIndex == -1)
   {
      // 不需要复制，直接退出
      return;
   }
   for (int i = 0; i < size - startCopyIndex; i++)
   {
      newPoints << originalPoints[startCopyIndex];
      newTimes << originalTimes[startCopyIndex];
      originalPoints.removeAt(startCopyIndex);
      originalTimes.removeAt(startCopyIndex);
   }
}


bool cszt::LyricFile::breakLyricText(long time, int mainIndex, QList<int> indexs)
{
   _LyricStatements *statements = nullptr;
   int              index       = 0;

   for (_LyricStatements *statement:_statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      index++;
      statements = statement;
   }

   if (statements)
   {
      _LyricStatements *newStatements = new _LyricStatements;
      // 打断主要歌词
      breakStatement(mainIndex, statements->mainText, statements->mainKeyPoints, statements->mainKeyTimes, newStatements->mainText, newStatements->mainKeyPoints, newStatements->mainKeyTimes);
      // 打断次要歌词
      int size = statements->otherTexts.size();
      for (int i = 0; i < size; i++)
      {
         QString     text;
         QList<int>  keyPoints;
         QList<long> keyTimes;
         breakStatement(indexs[i], statements->otherTexts[i], statements->otherTextPoints[i], statements->otherKeyTimes[i], text, keyPoints, keyTimes);
         newStatements->otherTexts << text;
         newStatements->otherTextPoints << keyPoints;
         newStatements->otherKeyTimes << keyTimes;
      }
      // 复制次要歌词标签
      newStatements->otherTextLabels = statements->otherTextLabels;
      // 插入指定位置
      _statements.insert(index, newStatements);
      return true;
   }
   return false;
}


/**
 * @brief
 * 在指定位置插入歌词
 * @param index
 * 插入位置
 * @param mainText
 * 主要歌词内容
 * @param statements
 * 歌词列表
 */
void insertLyric(const int& index, const QString& mainText, QList<cszt::_LyricStatements *>& statements)
{
   long startTime = MAX_LYRIC_TIME;
   int  size      = statements.size();

   if (index < size)
   {
      startTime = statements[index]->startTime;
   }
   cszt::_LyricStatements *statement = new cszt::_LyricStatements;
   statement->startTime = startTime;
   statement->mainText  = mainText;
   statements.insert(index, statement);
}


void cszt::LyricFile::addBefore(int index, QString mainText)
{
   insertLyric(index, mainText, _statements);

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                    = OPTYPE_INSERT;
      history->text                      = "前面插入歌词";
      history->params                    = new void *[2];
      history->params[0]                 = new int;
      history->params[1]                 = new QString;
      *CAST(int, history->params[0])     = index;
      *CAST(QString, history->params[1]) = mainText;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
}


void cszt::LyricFile::addAfter(int index, QString mainText)
{
   insertLyric(index + 1, mainText, _statements);

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                    = OPTYPE_INSERT;
      history->text                      = "后面插入歌词";
      history->params                    = new void *[2];
      history->params[0]                 = new int;
      history->params[1]                 = new QString;
      *CAST(int, history->params[0])     = index + 1;
      *CAST(QString, history->params[1]) = mainText;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
}


void cszt::LyricFile::finishSentence(long time)
{
   for (_LyricStatements *statement:_statements)
   {
      if (statement->startTime > time)
      {
         if (_enableHistory)
         {
            _History *history = new _History;
            history->opType                 = OPTYPE_CHANGE_START_TIME;
            history->text                   = "结束歌词时间";
            history->params                 = new void *[3];
            history->params[0]              = new int;
            history->params[1]              = new long;
            history->params[2]              = new long;
            *CAST(int, history->params[0])  = _statements.indexOf(statement);
            *CAST(long, history->params[1]) = statement->startTime;
            *CAST(long, history->params[2]) = time;
            pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
         }
         statement->startTime = time;
         return;
      }
   }
   _LyricStatements *statement = _statements.last();
   statement->mainKeyTimes << time;
   statement->mainKeyPoints << statement->mainText.size();
   int size = statement->otherTexts.size();
   for (int i = 0; i < size; i++)
   {
      statement->otherKeyTimes[i] << time;
      statement->otherTextPoints[i] << statement->otherTextPoints[i].size();
   }
}


void cszt::LyricFile::addMainBreakTime(long time, int index)
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement : _statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }

   if (statements)
   {
      int i;
      int size = statements->mainKeyPoints.size();
      for (i = 0; i < size; i++)
      {
         if (statements->mainKeyPoints[i] > time)
         {
            break;
         }
      }
      statements->mainKeyPoints.insert(i, index);
      statements->mainKeyTimes.insert(i, time);
   }
}


void cszt::LyricFile::addBreakTime(long time, int index, int lyricIndex)
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement : _statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }

   if (statements)
   {
      int i;
      int size = statements->otherTextPoints[lyricIndex].size();
      for (i = 0; i < size; i++)
      {
         if (statements->otherKeyTimes[lyricIndex][i] > time)
         {
            break;
         }
      }
      statements->otherTextPoints[lyricIndex].insert(i, index);
      statements->otherKeyTimes[lyricIndex].insert(i, time);
   }
}


QString cszt::LyricFile::printStatementByTime(const long& time) const
{
   _LyricStatements *statements = nullptr;

   for (_LyricStatements *statement : _statements)
   {
      if (statement->startTime > time)
      {
         break;
      }
      statements = statement;
   }
   if (statements)
   {
      return getStatementRawText(statements);
   }
   return QString::Null();
}


QString cszt::LyricFile::printStatementByIndex(const int& index) const
{
   return getStatementRawText(_statements[index]);
}


cszt::LyricFile& cszt::LyricFile::operator=(const LyricFile& other)
{
   _flags = other._flags;
   for (_LyricStatements *statements:_statements)
   {
      delete statements;
   }
   _statements.clear();
   for (_History *history:_histories)
   {
      delete history;
   }
   _histories.clear();
   for (_LyricStatements *statements:other._statements)
   {
      _LyricStatements *statement = new _LyricStatements;
      *statement = *statements;
      _statements << statement;
   }
   for (_History *history : other._histories)
   {
      _History *newHistory = cloneHistory(history);
      _histories << newHistory;
   }
   _enableHistory    = other._enableHistory;
   _maxHistoriesSize = other._maxHistoriesSize;
   _historyPointer   = other._historyPointer;
   return *this;
}


void cszt::LyricFile::clear()
{
   _flags.clear();
   for (_LyricStatements *statement:_statements)
   {
      delete statement;
   }
   for (_History *history:_histories)
   {
      delete history;
   }
   _statements.clear();
   _histories.clear();
}


void cszt::LyricFile::clearAllTime()
{
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType    = OPTYPE_CLEAR_ALL;
      history->text      = "清除全部歌词时间信息";
      history->params    = new void *[5];
      history->params[0] = new QList<long>;
      history->params[1] = new QList<QList<int> >;
      history->params[2] = new QList<QList<long> >;
      history->params[3] = new QList<QList<QList<int> > >;
      history->params[4] = new QList<QList<QList<long> > >;
      for (_LyricStatements *statements:_statements)
      {
         *CAST(QList<long>, history->params[0]) << statements->startTime;
         *CAST(QList<QList<int> >, history->params[1]) << statements->mainKeyPoints;
         *CAST(QList<QList<long> >, history->params[2]) << statements->mainKeyTimes;
         *CAST(QList<QList<QList<int> > >, history->params[3]) << statements->otherTextPoints;
         *CAST(QList<QList<QList<long> > >, history->params[4]) << statements->otherKeyTimes;
      }
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   for (_LyricStatements *statements:_statements)
   {
      statements->startTime = MAX_LYRIC_TIME;
      statements->mainKeyPoints.clear();
      statements->mainKeyTimes.clear();
      int size = statements->otherTextPoints.size();
      for (int i = 0; i < size; i++)
      {
         statements->otherTextPoints[i].clear();
         statements->otherKeyTimes[i].clear();
      }
   }
}


void cszt::LyricFile::cloneSentence(const int& index)
{
   _LyricStatements *statement    = _statements[index];
   _LyricStatements *newStatement = new _LyricStatements;

   *newStatement = *statement;
   _statements.insert(index + 1, newStatement);
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                = OPTYPE_CLONE;
      history->text                  = "克隆歌词";
      history->params                = new void *[1];
      history->params[0]             = new int;
      *CAST(int, history->params[0]) = index;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
}


void cszt::LyricFile::clearMainAllKeyPointByIndex(const int& index)
{
   _LyricStatements *statement = _statements[index];

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                        = OPTYPE_CLEAR_MAIN_ALL_KEY_POINT;
      history->text                          = "清除主要歌词全部关键点";
      history->params                        = new void *[3];
      history->params[0]                     = new int;
      history->params[1]                     = new QList<int>;
      history->params[2]                     = new QList<long>;
      *CAST(int, history->params[0])         = index;
      *CAST(QList<int>, history->params[1])  = statement->mainKeyPoints;
      *CAST(QList<long>, history->params[2]) = statement->mainKeyTimes;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }

   statement->mainKeyPoints.clear();
   statement->mainKeyTimes.clear();
}


void cszt::LyricFile::clearStartTimeByIndex(const int& index)
{
   _LyricStatements *statement = _statements[index];
   long             to         = 0;
   int              size       = _statements.size();

   if (index + 1 == size)
   {
      to = MAX_LYRIC_TIME;
   }
   else
   {
      to = _statements[index + 1]->startTime;
   }
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                 = OPTYPE_CHANGE_START_TIME;
      history->text                   = "清除开始时间";
      history->params                 = new void *[3];
      history->params[0]              = new int;
      history->params[1]              = new long;
      history->params[2]              = new long;
      *CAST(int, history->params[0])  = index;
      *CAST(long, history->params[1]) = statement->startTime;
      *CAST(long, history->params[2]) = to;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statement->startTime = to;
}


bool cszt::LyricFile::clearOtherAllKeyPointByIndex(const int& index, const int& otherIndex)
{
   _LyricStatements *statements = _statements[index];

   if (statements->otherTexts.size() <= otherIndex)
   {
      return false;
   }

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                        = OPTYPE_CLEAR_OTHER_ALL_KEY_POINT;
      history->text                          = "清除次要歌词全部关键点";
      history->params                        = new void *[4];
      history->params[0]                     = new int;
      history->params[1]                     = new int;
      history->params[2]                     = new QList<int>;
      history->params[3]                     = new QList<long>;
      *CAST(int, history->params[0])         = index;
      *CAST(int, history->params[1])         = otherIndex;
      *CAST(QList<int>, history->params[2])  = statements->otherTextPoints[otherIndex];
      *CAST(QList<long>, history->params[3]) = statements->otherKeyTimes[otherIndex];
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   statements->otherTextPoints[otherIndex].clear();
   statements->otherKeyTimes[otherIndex].clear();
   return true;
}


void cszt::LyricFile::removeLyricAt(const int& index)
{
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                = OPTYPE_REMOVE_LYRIC;
      history->text                  = "移除歌词";
      history->params                = new void *[2];
      history->params[0]             = new int;
      history->params[1]             = new _LyricStatements;
      *CAST(int, history->params[0]) = index;
      *CAST(_LyricStatements, history->params[1]) = *_statements[index];
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   delete _statements[index];
   _statements.removeAt(index);
}


void cszt::LyricFile::clearAllTimeByIndex(const int& index)
{
   _LyricStatements *statement = _statements[index];

   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                                = OPTYPE_CLEAR_ALL_TIME;
      history->text                                  = "清除歌词全部时间";
      history->params                                = new void *[6];
      history->params[0]                             = new int;
      history->params[1]                             = new long;
      history->params[2]                             = new QList<int>;
      history->params[3]                             = new QList<long>;
      history->params[4]                             = new QList<QList<int> >;
      history->params[5]                             = new QList<QList<long> >;
      *CAST(int, history->params[0])                 = index;
      *CAST(long, history->params[1])                = statement->startTime;
      *CAST(QList<int>, history->params[2])          = statement->mainKeyPoints;
      *CAST(QList<long>, history->params[3])         = statement->mainKeyTimes;
      *CAST(QList<QList<int> >, history->params[4])  = statement->otherTextPoints;
      *CAST(QList<QList<long> >, history->params[5]) = statement->otherKeyTimes;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }

   if (index + 1 == _statements.size())
   {
      statement->startTime = MAX_LYRIC_TIME;
   }
   else
   {
      statement->startTime = _statements[index + 1]->startTime;
   }

   statement->mainKeyPoints.clear();
   statement->mainKeyTimes.clear();
   statement->otherTextPoints.clear();
   statement->otherKeyTimes.clear();
}


bool cszt::LyricFile::replaceStatement(const int& index, const QString& raw)
{
   QRegExp          timeRegexp("\\[\\s*(\\d+)\\s*:\\s*(\\d+)\\s*\\.\\s*(\\d+)\\s*\\]([^\\[]*)");
   _LyricStatements *statements = new _LyricStatements;
   QStringList      list        = raw.split("\n", QString::SplitBehavior::SkipEmptyParts);
   QStringList      capturedTexts;
   int              timeLabelIndex;
   int              size = list.size();

   if (size == 0)
   {
      return false;
   }
   // 主要歌词内容
   timeLabelIndex = timeRegexp.indexIn(list[0]);
   if (timeLabelIndex != 0)
   {
      return false;
   }
   capturedTexts         = timeRegexp.capturedTexts();
   statements->startTime = asTime(capturedTexts, 1);
   statements->mainText  = capturedTexts[4];
   while ((timeLabelIndex = timeRegexp.indexIn(list[0], timeLabelIndex + 1)) != -1)
   {
      capturedTexts = timeRegexp.capturedTexts();
      statements->mainKeyTimes << asTime(capturedTexts, 1);
      statements->mainKeyPoints << statements->mainText.size();
      statements->mainText += capturedTexts[4];
   }
   // 次要歌词内容
   for (int i = 1; i < size; i++)
   {
      QString     text;
      QString     label;
      QList<int>  keyPoints;
      QList<long> keyTimes;
      timeLabelIndex = timeRegexp.indexIn(list[i]);
      // 如果没有时间标签，那么表明没有关键点信息
      if (timeLabelIndex == -1)
      {
         text = list[i];
         // 检查是否含有标签
         int labelIndex = -1;
         if ((text[0] == '<') && ((labelIndex = text.indexOf('>')) != -1))
         {
            label = text.mid(1, labelIndex - 1);
            text  = text.mid(labelIndex + 1);
         }
      }
      else
      {
         // 先把时间标签之前的内容加上
         text = list[i].mid(0, timeLabelIndex);
         // 检查是否含有标签
         int labelIndex = -1;
         if ((text[0] == '<') && ((labelIndex = text.indexOf('>')) != -1))
         {
            label = text.mid(1, labelIndex - 1);
            text  = text.mid(labelIndex + 1);
         }
         // 循环遍历关键点
         do
         {
            capturedTexts = timeRegexp.capturedTexts();
            keyTimes << asTime(capturedTexts, 1);
            keyPoints << text.size();
            text += capturedTexts[4];
         } while ((timeLabelIndex = timeRegexp.indexIn(list[i], timeLabelIndex + 1)) != -1);
      }
      statements->otherTexts << text;
      statements->otherTextLabels << label;
      statements->otherKeyTimes << keyTimes;
      statements->otherTextPoints << keyPoints;
   }

   // 替换指定歌词
   _LyricStatements *oldStatement = _statements[index];
   if (_enableHistory)
   {
      _History *history = new _History;
      history->opType                = OPTYPE_REPLACE;
      history->text                  = "编辑原始内容";
      history->params                = new void *[3];
      history->params[0]             = new int;
      history->params[1]             = new _LyricStatements;
      history->params[2]             = new _LyricStatements;
      *CAST(int, history->params[0]) = index;
      *CAST(_LyricStatements, history->params[1]) = *oldStatement;
      *CAST(_LyricStatements, history->params[2]) = *statements;
      pushHistory(history, _histories, _maxHistoriesSize, _historyPointer);
   }
   delete oldStatement;
   _statements[index] = statements;
   return true;
}


void cszt::LyricFile::enableHistory()
{
   _enableHistory = true;
}


void cszt::LyricFile::setMaxHistoriesSize(const int& size)
{
   _maxHistoriesSize = size;
}


int cszt::LyricFile::maxHistoriesSize() const
{
   return _maxHistoriesSize;
}


int cszt::LyricFile::undo()
{
   if (!_enableHistory)
   {
      return -1;
   }
   int index = _historyPointer - 1;
   if (index < 0)
   {
      return -1;
   }
   _historyPointer = index;
   _History *history = _histories[index];
   switch (history->opType)
   {
   case OPTYPE_CHANGE_START_TIME:
      {
         int  changeIndex = *CAST(int, history->params[0]);
         long time        = *CAST(long, history->params[1]);
         _statements[changeIndex]->startTime = time;
         return changeIndex;
      }

   case OPTYPE_CHANGE_MAIN_TEXT:
      {
         int         changeIndex    = *CAST(int, history->params[0]);
         QString     *text          = CAST(QString, history->params[1]);
         QList<int>  *keyPoints     = CAST(QList<int>, history->params[3]);
         QList<long> *keyPointTimes = CAST(QList<long>, history->params[4]);
         _statements[changeIndex]->mainText      = *text;
         _statements[changeIndex]->mainKeyPoints = *keyPoints;
         _statements[changeIndex]->mainKeyTimes  = *keyPointTimes;
         return changeIndex;
      }

   case OPTYPE_CHANGE_OTHER_TEXT:
      {
         int         changeIndex = *CAST(int, history->params[0]);
         int         otherIndex  = *CAST(int, history->params[1]);
         QString     *text       = CAST(QString, history->params[2]);
         QString     *label      = CAST(QString, history->params[3]);
         QList<int>  *keyPoints  = CAST(QList<int>, history->params[6]);
         QList<long> *keyTimes   = CAST(QList<long>, history->params[7]);
         _statements[changeIndex]->otherTexts[otherIndex]      = *text;
         _statements[changeIndex]->otherTextLabels[otherIndex] = *label;
         _statements[changeIndex]->otherTextPoints[otherIndex] = *keyPoints;
         _statements[changeIndex]->otherKeyTimes[otherIndex]   = *keyTimes;
         return changeIndex;
      }

   case OPTYPE_ADD_OTHER_TEXT:
      {
         int changeIndex = *CAST(int, history->params[0]);
         _statements[changeIndex]->otherTexts.removeLast();
         _statements[changeIndex]->otherTextLabels.removeLast();
         _statements[changeIndex]->otherTextPoints.removeLast();
         _statements[changeIndex]->otherKeyTimes.removeLast();
         return changeIndex;
      }

   case OPTYPE_REMOVE_OTHER_TEXT:
      {
         int         changeIndex = *CAST(int, history->params[0]);
         QString     *text       = CAST(QString, history->params[1]);
         QString     *label      = CAST(QString, history->params[2]);
         QList<int>  *keyPoints  = CAST(QList<int>, history->params[3]);
         QList<long> *keyTimes   = CAST(QList<long>, history->params[4]);
         _statements[changeIndex]->otherTexts << *text;
         _statements[changeIndex]->otherTextLabels << *label;
         _statements[changeIndex]->otherTextPoints << *keyPoints;
         _statements[changeIndex]->otherKeyTimes << *keyTimes;
         return changeIndex;
      }

   case OPTYPE_CLONE:
      {
         int              changeIndex = *CAST(int, history->params[0]);
         _LyricStatements *statements = _statements[changeIndex + 1];
         delete statements;
         _statements.removeAt(changeIndex + 1);
         return changeIndex;
      }

   case OPTYPE_CLEAR_MAIN_ALL_KEY_POINT:
      {
         int         changeIndex = *CAST(int, history->params[0]);
         QList<int>  *keyPoints  = CAST(QList<int>, history->params[1]);
         QList<long> *keyTimes   = CAST(QList<long>, history->params[2]);
         _statements[changeIndex]->mainKeyPoints = *keyPoints;
         _statements[changeIndex]->mainKeyTimes  = *keyTimes;
         return changeIndex;
      }

   case OPTYPE_CLEAR_OTHER_ALL_KEY_POINT:
      {
         int         changeIndex = *CAST(int, history->params[0]);
         int         otherIndex  = *CAST(int, history->params[1]);
         QList<int>  *keyPoints  = CAST(QList<int>, history->params[2]);
         QList<long> *keyTimes   = CAST(QList<long>, history->params[3]);
         _statements[changeIndex]->otherTextPoints[otherIndex] = *keyPoints;
         _statements[changeIndex]->otherKeyTimes[otherIndex]   = *keyTimes;
         return changeIndex;
      }

   case OPTYPE_REMOVE_LYRIC:
      {
         int              changeIndex    = *CAST(int, history->params[0]);
         _LyricStatements *statements    = CAST(_LyricStatements, history->params[1]);
         _LyricStatements *newStatements = new _LyricStatements;
         *newStatements = *statements;
         _statements.insert(changeIndex, newStatements);
         return changeIndex;
      }

   case OPTYPE_CLEAR_ALL_TIME:
      {
         int                 changeIndex     = *CAST(int, history->params[0]);
         long                startTime       = *CAST(long, history->params[1]);
         QList<int>          *mainKeyPoints  = CAST(QList<int>, history->params[2]);
         QList<long>         *mainKeyTimes   = CAST(QList<long>, history->params[3]);
         QList<QList<int> >  *otherKeyPoints = CAST(QList<QList<int> >, history->params[4]);
         QList<QList<long> > *otherKeyTimes  = CAST(QList<QList<long> >, history->params[5]);
         _statements[changeIndex]->startTime       = startTime;
         _statements[changeIndex]->mainKeyPoints   = *mainKeyPoints;
         _statements[changeIndex]->mainKeyTimes    = *mainKeyTimes;
         _statements[changeIndex]->otherTextPoints = *otherKeyPoints;
         _statements[changeIndex]->otherKeyTimes   = *otherKeyTimes;
         return changeIndex;
      }

   case OPTYPE_CLEAR_ALL:
      {
         QList<long>                 startTimes     = *CAST(QList<long>, history->params[0]);
         QList<QList<int> >          mainKeyPoints  = *CAST(QList<QList<int> >, history->params[1]);
         QList<QList<long> >         mainKeyTimes   = *CAST(QList<QList<long> >, history->params[2]);
         QList<QList<QList<int> > >  otherKeyPoints = *CAST(QList<QList<QList<int> > >, history->params[3]);
         QList<QList<QList<long> > > otherKeyTimes  = *CAST(QList<QList<QList<long> > >, history->params[4]);
         int size = _statements.size();
         for (int i = 0; i < size; i++)
         {
            _statements[i]->startTime       = startTimes[i];
            _statements[i]->mainKeyPoints   = mainKeyPoints[i];
            _statements[i]->mainKeyTimes    = mainKeyTimes[i];
            _statements[i]->otherTextPoints = otherKeyPoints[i];
            _statements[i]->otherKeyTimes   = otherKeyTimes[i];
         }
         return 0;
      }

   case OPTYPE_INSERT:
      {
         int changeIndex = *CAST(int, history->params[0]);
         delete _statements[changeIndex];
         _statements.removeAt(changeIndex);
         int size = _statements.size();
         if (size == changeIndex)
         {
            --changeIndex;
         }
         return changeIndex;
      }

   case OPTYPE_REPLACE:
      {
         int              changeIndex = *CAST(int, history->params[0]);
         _LyricStatements *statements = CAST(_LyricStatements, history->params[1]);
         _LyricStatements *statement  = new _LyricStatements;
         *statement = *statements;
         delete _statements[changeIndex];
         _statements[changeIndex] = statement;
         return changeIndex;
      }
   }
   abort();
}


int cszt::LyricFile::redo()
{
   if (!_enableHistory)
   {
      return -1;
   }
   int index = _historyPointer + 1;
   if (_historyPointer == _histories.size())
   {
      return -1;
   }
   _History *history = _histories[_historyPointer];
   _historyPointer = index;
   switch (history->opType)
   {
   case OPTYPE_CHANGE_START_TIME:
      {
         int  changeIndex = *CAST(int, history->params[0]);
         long time        = *CAST(long, history->params[2]);
         _statements[changeIndex]->startTime = time;
         return changeIndex;
      }

   case OPTYPE_CHANGE_MAIN_TEXT:
      {
         int     changeIndex = *CAST(int, history->params[0]);
         QString *text       = CAST(QString, history->params[2]);
         _statements[changeIndex]->mainText = *text;
         _statements[changeIndex]->mainKeyPoints.clear();
         _statements[changeIndex]->mainKeyTimes.clear();
         return changeIndex;
      }

   case OPTYPE_CHANGE_OTHER_TEXT:
      {
         int     changeIndex = *CAST(int, history->params[0]);
         int     otherIndex  = *CAST(int, history->params[1]);
         QString *text       = CAST(QString, history->params[4]);
         QString *label      = CAST(QString, history->params[5]);
         _statements[changeIndex]->otherTexts[otherIndex]      = *text;
         _statements[changeIndex]->otherTextLabels[otherIndex] = *label;
         _statements[changeIndex]->otherTextPoints[otherIndex].clear();
         _statements[changeIndex]->otherKeyTimes[otherIndex].clear();
         return changeIndex;
      }

   case OPTYPE_ADD_OTHER_TEXT:
      {
         int     changeIndex = *CAST(int, history->params[0]);
         QString *text       = CAST(QString, history->params[1]);
         QString *label      = CAST(QString, history->params[2]);
         _statements[changeIndex]->otherTexts << *text;
         _statements[changeIndex]->otherTextLabels << *label;
         _statements[changeIndex]->otherTextPoints << QList<int>();
         _statements[changeIndex]->otherKeyTimes << QList<long>();
         return changeIndex;
      }

   case OPTYPE_REMOVE_OTHER_TEXT:
      {
         int changeIndex = *CAST(int, history->params[0]);
         _statements[changeIndex]->otherTexts.removeLast();
         _statements[changeIndex]->otherTextLabels.removeLast();
         _statements[changeIndex]->otherTextPoints.removeLast();
         _statements[changeIndex]->otherKeyTimes.removeLast();
         return changeIndex;
      }

   case OPTYPE_CLONE:
      {
         int              changeIndex    = *CAST(int, history->params[0]);
         _LyricStatements *statements    = _statements[changeIndex];
         _LyricStatements *newStatements = new _LyricStatements;
         *newStatements = *statements;
         _statements.insert(changeIndex + 1, newStatements);
         return changeIndex;
      }

   case OPTYPE_CLEAR_MAIN_ALL_KEY_POINT:
      {
         int changeIndex = *CAST(int, history->params[0]);
         _statements[changeIndex]->mainKeyPoints.clear();
         _statements[changeIndex]->mainKeyTimes.clear();
         return changeIndex;
      }

   case OPTYPE_CLEAR_OTHER_ALL_KEY_POINT:
      {
         int changeIndex = *CAST(int, history->params[0]);
         int otherIndex  = *CAST(int, history->params[1]);
         _statements[changeIndex]->otherTextPoints[otherIndex].clear();
         _statements[changeIndex]->otherKeyTimes[otherIndex].clear();
         return changeIndex;
      }

   case OPTYPE_REMOVE_LYRIC:
      {
         int changeIndex = *CAST(int, history->params[0]);
         delete _statements[changeIndex];
         _statements.removeAt(changeIndex);
         if (changeIndex >= _statements.size())
         {
            return changeIndex - 1;
         }
         else
         {
            return changeIndex;
         }
      }

   case OPTYPE_CLEAR_ALL_TIME:
      {
         int changeIndex = *CAST(int, history->params[0]);
         if (changeIndex + 1 == _statements.size())
         {
            _statements[changeIndex]->startTime = MAX_LYRIC_TIME;
         }
         else
         {
            _statements[changeIndex]->startTime = _statements[changeIndex + 1]->startTime;
         }
         _statements[changeIndex]->mainKeyPoints.clear();
         _statements[changeIndex]->mainKeyTimes.clear();
         _statements[changeIndex]->otherTextPoints.clear();
         _statements[changeIndex]->otherKeyTimes.clear();
         return changeIndex;
      }

   case OPTYPE_CLEAR_ALL:
      {
         int size = _statements.size();
         for (int i = 0; i < size; i++)
         {
            _statements[i]->startTime = MAX_LYRIC_TIME;
            _statements[i]->mainKeyPoints.clear();
            _statements[i]->mainKeyTimes.clear();
            int textSize = _statements[i]->otherTextPoints.size();
            for (int j = 0; j < textSize; j++)
            {
               _statements[i]->otherTextPoints[j].clear();
               _statements[i]->otherKeyTimes[j].clear();
            }
         }
         return 0;
      }

   case OPTYPE_INSERT:
      {
         int     changeIndex = *CAST(int, history->params[0]);
         QString *text       = CAST(QString, history->params[1]);
         insertLyric(changeIndex, *text, _statements);
         return changeIndex;
      }

   case OPTYPE_REPLACE:
      {
         int              changeIndex = *CAST(int, history->params[0]);
         _LyricStatements *statements = CAST(_LyricStatements, history->params[2]);
         _LyricStatements *statement  = new _LyricStatements;
         *statement = *statements;
         delete _statements[changeIndex];
         _statements[changeIndex] = statement;
         return changeIndex;
      }
   }
   abort();
}


bool cszt::LyricFile::canUndo() const
{
   return _enableHistory && _historyPointer > 0;
}


bool cszt::LyricFile::canRedo() const
{
   return _enableHistory && (_historyPointer < _histories.size());
}


QString cszt::LyricFile::undoText() const
{
   if (_enableHistory && (_historyPointer > 0))
   {
      return _histories[_historyPointer - 1]->text;
   }
   return QString::Null();
}


QString cszt::LyricFile::redoText() const
{
   if (_enableHistory && (_historyPointer < _histories.size()))
   {
      return _histories[_historyPointer]->text;
   }
   return QString::Null();
}
