#ifndef COMMON_H
#define COMMON_H

#include <QDateTime>

#define DEF_DBFILE "/home/harry/localdb/fiona.db"
#define DATETIME_FMT "yyyy-MM-dd hh:mm:ss"

#define CN_AMOPEN_TIME  QTime(9, 15, 0)
#define CN_AMCLOSE_TIME QTime(11, 30, 0)
#define CN_PMOPEN_TIME  QTime(13, 00, 0)
#define CN_PMCLOSE_TIME QTime(15, 14, 00)

#define STR2DATETIME(str) (QDateTime::fromString((str), DATETIME_FMT))
#define DATETIME2STR(dt) (dt.toString(DATETIME_FMT))

#endif // COMMON_H

