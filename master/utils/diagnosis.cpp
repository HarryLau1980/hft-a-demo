#include "diagnosis.h"
#include <QDebug>
#include <QDir>
#include <QString>

#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <stdio.h>

class FileLogger
{
public:
    FileLogger(const QString& file)
        : mFile(file)
        , mTmpBuff(NULL){
        mTmpBuff = new char[1024];
        open();
    }
    ~FileLogger() {
        if(mTmpBuff) {
            delete[] mTmpBuff;
        }
    }

private:
    QFile mFile;
    QMutex mMtx;
    char*  mTmpBuff;

public:
    bool open() {
        if(mFile.isOpen()) {
            return false;
        }
        return mFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
    }
    void close() {
        if(mFile.isOpen()) {
            mFile.close();
        }
    }

    void write(QtMsgType ty, const QString& msg) {
        QDateTime now = QDateTime::currentDateTime();
        QString   s_now = now.toString("[yyyy-MM-dd hh:mm:ss] ");
        int clen = 0;
        char prefix[16];
        if(ty == QtWarningMsg) {
            strcpy(prefix, "<Warning  >");
        } else if(ty == QtCriticalMsg) {
            strcpy(prefix, "<!Critical>");
        } else if( ty == QtFatalMsg) {
            strcpy(prefix, "<**!Fatal >");
        } else {
            prefix[0] = '\0';
        }
        fprintf(stderr, "%s\n", msg.toStdString().c_str());

        QMutexLocker alk(&mMtx);
        clen = sprintf(mTmpBuff, "%s %s %s\n", s_now.toStdString().c_str(), prefix, msg.toStdString().c_str());

        mFile.write(mTmpBuff, clen);
        mFile.flush();
    }
};

static FileLogger* gLogger = NULL;

void DbgMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    gLogger->write(type, msg);
    if(type == QtFatalMsg) {
        abort();
    }
}

void RedirectLog2Files()
{
    if(gLogger)
        return;

    QDir dir;
    dir.mkpath(DEF_LOGFOLDER);
    QString fileName = QString("%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmm"));
    gLogger = new FileLogger(QString("%1/%2").arg(DEF_LOGFOLDER).arg(fileName));

    qInstallMessageHandler(DbgMessageOutput);
}
