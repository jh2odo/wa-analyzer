/*
The MIT License (MIT)

Copyright (c) 2012 Jose Aguado (jh2odo)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/
#include "logger.h"
#include <QDebug>
#include <QMap>

Logger::Logger(QObject *parent) :
    QObject(parent)
{

}


Logger::Logger(QString pathWALogs)
{

    this->pathWALogs = pathWALogs;
    WHATSAPP_DEFAULT_VERSION = "2.8.22";
    MAX_LINES_PROCESS = 50000;

    totalLinesProcessed = 0;
    totalLinesFound = 0;

    PATH_LOGS_DATA = "logs/";
    PATH_MD5 = PATH_LOGS_DATA + "wa/";
    PATH_DAT = PATH_LOGS_DATA + "dat/";

    if(!QDir(PATH_LOGS_DATA).exists()){
        QDir().mkdir(PATH_LOGS_DATA);
    }

    if(!QDir(PATH_MD5).exists()){
        QDir().mkdir(PATH_MD5);
    }

    if(!QDir(PATH_DAT).exists()){
        QDir().mkdir(PATH_DAT);
    }

}

QString Logger::calculateHashMd5(QString data){
    QByteArray dataByte;
    dataByte.append(data);
    QByteArray result = QCryptographicHash::hash(dataByte,QCryptographicHash::Md5);
    QString md5 = QString(result.toHex());
    //qDebug() << "Md5 Hash: " << QString(result.toHex());
    return md5;
}

QMap<QString,int> Logger::detectVersion(QStringList lines)
{
    QMap<QString,int> versions;
    for (int i = lines.size() - 1; i >= 0; i--) {
        if(lines.at(i).contains(": WhatsApp 2.")){
            if(int vStart = lines.at(i).indexOf("WhatsApp")){
                if(int vEnd = lines.at(i).indexOf("(", vStart + 8)){
                     if(vEnd != -1){
                         QString version = lines.at(i).mid((vStart + 8),(vEnd - (vStart + 8))).trimmed();
                         if(!versions.contains(version)){
                             versions.insert(version,i);
                         }
                     }
                }
            }
        }
        if(lines.at(i).contains("chatview.py(625) : chat/recv-message:")){
            if(!versions.contains("2.8.22")){
                 versions.insert("2.8.22",i);
            }

        }
    }
    if(versions.isEmpty()){
        versions.insert(WHATSAPP_DEFAULT_VERSION,(lines.size() - 1));
    }
    return versions;
}

void Logger::setPathWALogs(QString pathWALogs){
    this->pathWALogs = pathWALogs;
}


QFileInfoList Logger::getLogsDataByRange(QDate dateStart, QDate dateEnd){

    QFileInfoList dirs = getListDirs(PATH_DAT);

    QFileInfoList logs = QFileInfoList();
    for (int i = 0; i < dirs.size(); ++i){
       QFileInfo fileInfo = dirs.at(i);
       if(fileInfo.suffix().contains("dat")){
           QDate dateFile = getDateFromLogName(fileInfo);
           if(!dateFile.isNull()){
                if((dateStart <= dateFile) && (dateEnd >= dateFile)){
                     logs.append(fileInfo);
                }
           }
       }
    }
    return logs;
}

QFileInfoList Logger::getLogsDataByDays(QList<QDate> days)
{
    QFileInfoList dirs = getListDirs(PATH_DAT);

    QFileInfoList logs = QFileInfoList();
    for (int i = 0; i < dirs.size(); ++i){
       QFileInfo fileInfo = dirs.at(i);
       if(fileInfo.suffix().contains("dat")){
           QDate dateFile = getDateFromLogName(fileInfo);
           if(!dateFile.isNull()){
                foreach (QDate day, days) {
                    if(dateFile.daysTo(day) == 0){
                         logs.append(fileInfo);
                         break;
                    }
                }
           }
       }
    }
    return logs;
}

QDate Logger::getDateFromLogName(QFileInfo fileInfo){
    QString name = fileInfo.baseName();
    QStringList dateFileText = name.split("-");
    if(dateFileText.size() == 3){
         return QDate(dateFileText.at(0).toInt(),dateFileText.at(1).toInt(),dateFileText.at(2).toInt());
    }
    return QDate();
}

QFileInfoList Logger::getListDirs(QString path){
    if(QDir(path).exists()){
        QDir dir = QDir(path);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Time);
        return dir.entryInfoList();
    }
    return QFileInfoList();
}

QFileInfoList Logger::getWALogsByDays(QList<QDate> days){

    QFileInfoList dirs = getListDirs(pathWALogs);

    QFileInfoList logs = QFileInfoList();
    for (int i = 0; i < dirs.size(); ++i){
       QFileInfo fileInfo = dirs.at(i);
       if(fileInfo.suffix().contains("log") || fileInfo.suffix().contains("gz")){
           QDate dateFile = getDateFromLogName(fileInfo);
           if(!dateFile.isNull()){
                foreach (QDate day, days) {
                    if(dateFile.daysTo(day) == 0){
                         logs.append(fileInfo);
                         break;
                    }
                }
           }
       }
    }
    return logs;
}

QFileInfoList Logger::getWALogsByRange(QDate dateStart, QDate dateEnd){

    QFileInfoList dirs = getListDirs(pathWALogs);

    QFileInfoList logs = QFileInfoList();
    for (int i = 0; i < dirs.size(); ++i){
       QFileInfo fileInfo = dirs.at(i);
       if(fileInfo.suffix().contains("log") || fileInfo.suffix().contains("gz")){
           QDate dateFile = getDateFromLogName(fileInfo);
           if(!dateFile.isNull()){
                if((dateStart <= dateFile) && (dateEnd >= dateFile)){
                     logs.append(fileInfo);
                }
           }
       }
    }
    return logs;
}

void Logger::processWALogs(QDate dateStart, QDate dateEnd){
    QFileInfoList listLogs = getWALogsByRange(dateStart,dateEnd);

    processWALogs(listLogs);
}

void Logger::setProcessed(int value)
{
    if (value != processed) {
        processed = value;
        emit valueProcessChanged(value);
     }
}


void Logger::processWALogs(QList<QDate> days){
    QFileInfoList listLogs = getWALogsByDays(days);

    processWALogs(listLogs);
}

int Logger::getTotalWALogsNotProcessed(QDate dateStart, QDate dateEnd){
    QFileInfoList listLogs = getWALogsByRange(dateStart,dateEnd);
    return getWALogsNotProcessed(listLogs).size();
}

bool Logger::removeLogs()
{
    //removeDir(PATH_MD5);
    //removeDir(PATH_DAT);
    int result = removeDir(PATH_LOGS_DATA);

    // Recreamos estructra
    if(!QDir(PATH_LOGS_DATA).exists()){
        QDir().mkdir(PATH_LOGS_DATA);
    }

    if(!QDir(PATH_MD5).exists()){
        QDir().mkdir(PATH_MD5);
    }

    if(!QDir(PATH_LOGS_DATA + "dat").exists()){
        QDir().mkdir(PATH_DAT);
    }

    return result;
}

QFileInfoList Logger::getWALogsNotProcessed(QFileInfoList listLogs){
    QFileInfoList list;
    foreach (const QFileInfo logFile, listLogs){
        QString fileNameMd5 = PATH_MD5 + logFile.fileName()+".md5";
        if(QFile::exists(fileNameMd5)){
            QFile fileMd5(fileNameMd5);
            fileMd5.open(QIODevice::ReadOnly);
            QTextStream in(&fileMd5);
            QString md5Saved = in.readAll();
            QString md5 = calculateHashMd5(logFile.lastModified().toString("dd.MM.yyyy.hh.mm.ss"));
            //qDebug() << logFile.fileName() << ":" << md5 << ":" << md5Saved;
            if(!md5.contains(md5Saved)){
                list.append(logFile);
            }
            fileMd5.close();
        }else{
            list.append(logFile);
        }
    }
    return list;
}

void Logger::setWALogProcessed(QFileInfo logFile){
    QFile fileMd5(PATH_MD5 + logFile.fileName()+".md5");
    fileMd5.open(QIODevice::WriteOnly);
    QTextStream out(&fileMd5);
    QString md5 = calculateHashMd5(logFile.lastModified().toString("dd.MM.yyyy.hh.mm.ss"));
    out << md5;
    //qDebug() << logFile.fileName() << ":" << md5;
    fileMd5.close();
}


QList<Log> Logger::groupLogsByDays(QList<Log> logs){

    qDebug()<< "Total logs: " << QString::number(logs.size());

    QList<Log> logsTmp = QList<Log>();

    foreach (Log log, logs){

        //qDebug() << "Log: " << log.getFilePath().toAscii();
        bool insert = false;
        for (int i = 0; i < logsTmp.size(); i++) {
            QDate dataLog = log.getDate();
            Log tmpLog = logsTmp.at(i);
            QDate dataLogTmp = tmpLog.getDate();
            if(dataLog.daysTo(dataLogTmp) == 0){
                foreach ( Line line, log.getLines()){
                    if(!tmpLog.existLine(line)){
                        tmpLog.addLine(line);
                    }
                }
                logsTmp.replace(i,tmpLog);
                insert = true;
                break;
            }
        }

        if(!insert){
             logsTmp.append(log);
        }
    }

    qDebug()<< "Total logs group: " << QString::number(logsTmp.size());

    // Ordenamos las lineas de los logs
    for (int i = 0; i < logsTmp.size(); i++) {
        logsTmp[i].bubbleSortDescendingLines();
    }

    return logsTmp;
}

Log Logger::processWALog(QFileInfo logFile){

    QString data;
    QFile inputFile(logFile.filePath());
    inputFile.open(QIODevice::ReadOnly);
    if(logFile.filePath().contains("gz")){
        data = decompressGz(inputFile.readAll());
    }else{
        QTextStream in(&inputFile);
        data = in.readAll();
    }
    inputFile.close();


   QDate dateFile = getDateFromLogName(logFile);

   Log log = Log(dateFile,logFile.filePath());

   QStringList lines = data.split('\n');

   qDebug() << "Log: " << log.getFilePath();
   //qDebug() << "Detected version: " << detectVersion(lines);

   lastLine = "";
   for (int i = lines.size() - 1; i >= 0; i--) {
       if(i > 0){
           int last = i - 1;
           lastLine = lines.at(last);
       }else{
           lastLine = "";
       }
       Line line = processWALine(lines.at(i));
       if(!line.isNull()){
          if(!log.existLine(line)){
              log.addLine(line);
          }
       }

   }

   totalLinesProcessed = totalLinesProcessed + lines.size();
   totalLinesFound = totalLinesFound + log.getLines().size();

   return log;

}

void Logger::processWALogs(QFileInfoList listLogs){

    setProcessed(0);
    qDebug() << "--------------";
     foreach (const QFileInfo logFile, listLogs){
           qDebug() << logFile.filePath();
     }
    qDebug() << "--------------";

    setProcessed(5);
    listLogs = getWALogsNotProcessed(listLogs);


    qDebug() << "--------------";
     foreach (const QFileInfo logFile, listLogs){
           qDebug() << logFile.filePath();
     }
    qDebug() << "--------------";

    QList<Log> logs = QList<Log>();

    int total = 0;
    foreach (const QFileInfo logFile, listLogs){
        Log log = processWALog(logFile);
        logs.append(log);
        setWALogProcessed(logFile);
        total++;
        setProcessed((total*90)/listLogs.size());
    }

    qDebug() << "Con precesamiento";
    qDebug() << "Total Lines Processed: " << totalLinesProcessed;
    qDebug() << "Total Lines Found: " << totalLinesFound;
    qDebug() << "Total: " << QString::number(totalLinesProcessed + totalLinesFound);



     qDebug() << "Total Log Processed: " << QString::number(logs.size());
     if(logs.size() > 0){
        setProcessed(95);
         logs = groupLogsByDays(logs);

         QList<QDate> listDates;
         foreach (Log log, logs){
             listDates.append(log.getDate());
         }

         QFileInfoList list = getLogsDataByDays(listDates);
         foreach (QFileInfo logFile, list){
             Log logSerialized = getLogDataSerialized(logFile);
             logs.append(logSerialized);
         }

         logs = groupLogsByDays(logs); // Reagrupamos

         // Guardamos el serializado
         foreach (Log log, logs){
            setLogDataSerialized(log);
        }
     }

     setProcessed(100);
}

QByteArray Logger::decompressGz(QByteArray data)
{
    if (data.size() <= 4) {
        qWarning("gUncompress: Input data is truncated");
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

QList<Log> Logger::getLogsByDays(QList<QDate> days)
{
    QList<Log> logs;
    QFileInfoList list = getLogsDataByDays(days);
    foreach (QFileInfo logFile, list){
        Log logSerialized = getLogDataSerialized(logFile);
        logs.append(logSerialized);
    }

    Log tmpLog;
    for (int j = 1; j <= logs.size(); j++)
    {
         for (int i = 0; i < (logs.size() -1); i++)
         {
             Log log1 = logs.at(i);
             Log log2 = logs.at(i+1);
             if (log1.getDate() < log2.getDate()) //Si Cambiamos el operador < por > entonces ordenaremos de menor a mayor
             {
               tmpLog = logs.at(i);
               logs.replace(i,logs.at(i+1));
               logs.replace(i+1,tmpLog);
             }
         }
    }

    return logs;
}

QList<Log> Logger::getLogsLastByDays(int days)
{
    QDate last = QDate::currentDate();
    days = (days * -1);
    last = last.addDays(days);
    //qDebug() << last.toString("yyyy.MM.dd");
    return getLogsByRange(last,QDate::currentDate());
}

QList<Log> Logger::getLogsByRange(QDate dateStart, QDate dateEnd)
{
    QList<Log> logs;
    QFileInfoList list = getLogsDataByRange(dateStart,dateEnd);
    foreach (QFileInfo logFile, list){
        Log logSerialized = getLogDataSerialized(logFile);
        logs.append(logSerialized);
    }

    Log tmpLog;
    for (int j = 1; j <= logs.size(); j++)
    {
         for (int i = 0; i < (logs.size() -1); i++)
         {
             Log log1 = logs.at(i);
             Log log2 = logs.at(i+1);
             if (log1.getDate() < log2.getDate()) //Si Cambiamos el operador < por > entonces ordenaremos de menor a mayor
             {
               tmpLog = logs.at(i);
               logs.replace(i,logs.at(i+1));
               logs.replace(i+1,tmpLog);
             }
         }
    }

    return logs;
}


QList<Log> Logger::reverseLogs(QList<Log> logs){
    QList<Log> tmp = QList<Log>();
    while(!logs.isEmpty()) {
         tmp.append(logs.last());
         logs.pop_back();
    }
    logs = tmp;
    return logs;
}


Log Logger::getLogDataSerialized(QFileInfo logFile){
    QFile fileLogSerializedIn(logFile.filePath());
    fileLogSerializedIn.open(QIODevice::ReadOnly);
    QDataStream inSerialized(&fileLogSerializedIn);   // we will serialize the data into the file
    Log logSerialized;
    // Read and check the header
    quint32 magic;
    inSerialized >> magic;
    if (magic != 0xA0B0C0D0)
        qDebug() << "After Serialized: Error bad file format";
    inSerialized >> logSerialized;   // get serialize a log
    fileLogSerializedIn.close();
    return logSerialized;
}

void Logger::setLogDataSerialized(Log log){
    qDebug() << "Before Serialize: " << QString::number(log.getLines().count());
    log.setFilePath(PATH_DAT + log.getDate().toString("yyyy-MM-dd")+".dat");
    QFile fileLogSerialized(log.getFilePath());
    fileLogSerialized.open(QIODevice::WriteOnly);
    QDataStream outSerialized(&fileLogSerialized);   // we will serialize the data into the file
    // Write a header with a "magic number" and a version
    outSerialized << (quint32)0xA0B0C0D0;
    outSerialized.setVersion(QDataStream::Qt_4_7);
    outSerialized << log;   // serialize a log
    fileLogSerialized.close();
}

QTime Logger::getTimeWALine(QString line){

    QStringList tmp = line.split(" : ");

    if(tmp.size() == 3){
        QStringList date = tmp.at(0).split(".");
        if(date.size() == 2){
            date = date.at(0).split(" ");
            if(date.size() == 2){
                date = date.at(1).split(":");
                if(date.size() == 3){
                    int h = date.at(0).toInt();
                    int m = date.at(1).toInt();
                    int s = date.at(2).toInt();
                    return QTime(h,m,s);
                }
            }
        }
    }

    return QTime();
}

bool Logger::removeDir(QString dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (QDir(dirName).exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
                qDebug() << "delete: " <<  result << " " << info.absoluteFilePath();
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
        qDebug() << "delete: " <<  result << " " << dirName;
    }

    return result;
}


Line Logger::processWALine(QString line){

    //qDebug() << "processWALine" << line;

    // SEND: All version

    if( line.contains("/send-text-message:") ||
        line.contains("/send-message:") ||
        line.contains("/send-image:") ||
        line.contains("/send-video:") ||
        line.contains("/send-audio:") ||
        line.contains("/send-contact:") ||
        line.contains("/send-location:")){

        QTime time = getTimeWALine(line);

        if(!time.isNull()){

            QStringList tmp = line.split(" : ");
            QString text = tmp.at(2).trimmed();

            QString type = "send-message";

            // contact,type,group,length,id
            QMap<QString,QString> listText;
            listText.insert("contact","undefined");
            listText.insert("source","");
            listText.insert("type","");
            listText.insert("length","");
            listText.insert("id","");

            // Text
            if(text.contains("send-message:")){
               // qDebug() << "Line: " << line;
               // qDebug() << "Last: " << lastLine;
                if(lastLine.contains("ChatStoreDb/InsertMsgL/id:")){
                    listText.insert("type","Text");
                }else{
                    return Line(); // No valido
                }
            }else if(text.contains("send-text-message")){
                listText.insert("type","Text");
            }else if(text.contains("send-image")){
                listText.insert("type","Image");
            }else if(text.contains("send-video")){
                listText.insert("type","Video");
            }else if(text.contains("send-audio")){
                listText.insert("type","Audio");
            }else if(text.contains("send-contact")){
                listText.insert("type","Contact");
            }else if(text.contains("send-location")){
                listText.insert("type","Location");
            }else{
                return Line(); // No valido
            }

            // Length Text only 2.8.22
            if(text.contains("send-text-message")){
                if(int lengthStart = text.indexOf("chars=")){
                     if(lengthStart != -1){
                            listText.insert("length",text.mid(lengthStart+6, text.length() - (lengthStart + 6)));
                     }
                }
            }

            int toStart = text.indexOf("to=");
            bool isGroup = false;
            int numberGroup = text.indexOf("-",toStart);
            if((numberGroup != -1) && (numberGroup <= (toStart + 20))){ // Solo puede ser el primer -
               isGroup = true;
               listText.insert("source","group");
               listText.insert("group","undefined");
            }else{
               isGroup = false;
               listText.insert("source","private");
            }

            if(!isGroup){
                // Private
                if(text.contains("-contact") || text.contains("-location")){
                     if(toStart != -1){
                         listText.insert("contact",text.mid((toStart+3), (text.length() - (toStart + 3))));
                     }
                }else{
                    if(int numberEnd = text.indexOf(",",toStart+3)){
                        if(numberEnd != -1){
                             listText.insert("contact",text.mid((toStart+3), (numberEnd - (toStart+3))));
                        }
                        if(listText.value("contact").contains("undefined")){
                         //   qDebug() << "aaaaaaaaaaaaaaaaaaa: " << text << ":::" << line;
                        }
                    }
                }
            }else{
                // Group
                 if(text.contains("-contact") || text.contains("-location")){
                     if(toStart != -1){
                         QString groupText = text.mid((toStart + 3),(text.length() -(toStart+3)));
                         QStringList groupTextList = groupText.split("-");
                         if(groupTextList.size() == 2){
                             listText.insert("contact",groupTextList.at(0));
                             listText.insert("group",groupTextList.at(0));

                             listText.insert("id",groupTextList.at(1));
                         }
                     }
                 }else{
                     if(int groupEnd = text.indexOf(",",toStart+3)){
                         QString groupText = text.mid((toStart + 3),(groupEnd -(toStart+3)));
                         QStringList groupTextList = groupText.split("-");
                         if(groupTextList.size() == 2){
                             listText.insert("contact",groupTextList.at(0));
                             listText.insert("group",groupTextList.at(0));

                             listText.insert("id",groupTextList.at(1));
                         }
                     }
                 }
            }

            Line lin = Line(time,type,listText);
            return lin;

        }else{
            return Line(); // No valido
        }
    // Fin

    // RECV: All Version

    // Private: from=34627358016
    // Group: from=34627358016-1359045108
    // Group: from=34627358016-1359041893/34677863637

    // 2.9.3
    // [, sizes=]
    // - x/0/0 Text or Contact ¿?
    // - 0/0/x Audio ¿?
    // - x/x/0 Contact ¿?
    // - 0/x/y Image(y < 6) or Video(y >= 6) ¿?
    // - 0/x/0 Location ¿?
    // - New Type Generic: Media

    // 2.8.22
    // [, len=]
    // [, type=] - Text, Image, Video, Audio, Contact, Location, unknown

    }else if(line.contains("chat/recv-message:")){

   //     qDebug() << "v2.9.3 :" << line;

        QTime time = getTimeWALine(line);

        if(!time.isNull()){

            QStringList tmp = line.split(" : ");
            QString text = tmp.at(2).trimmed();

            QString type = "recv-message";

            // contact,type,group,length,id
            QMap<QString,QString> listText;
            listText.insert("contact","undefined");
            listText.insert("source","");
            listText.insert("type","unknown");
            listText.insert("length","");
            listText.insert("id","");

            if(text.contains(", len=")){
                if(int start = text.indexOf(", type=")){
                    if(int end = text.indexOf(",", start + 7)){
                         if(end != -1){
                              listText.insert("type",text.mid((start + 7),(end - (start + 7))));
                         }
                    }
                }
                if(int start = text.indexOf(", len=")){
                    if(int end = text.indexOf(",", start + 6)){
                         if(end != -1){
                              listText.insert("length",text.mid((start + 6),(end - (start + 6))));
                         }
                    }
                }
            }else if(text.contains(", sizes=")){
                if(int start = text.indexOf(", sizes=")){
                     QString sizes = text.mid(start+8, text.length() - (start + 8));
                     QStringList sizesTmp = sizes.split("/");
                     if(sizesTmp.size() == 3){
                         if(!sizesTmp.at(0).contains("0") && sizesTmp.at(1).contains("0") && sizesTmp.at(2).contains("0")){
                             listText.insert("type","Text");
                             listText.insert("length",sizesTmp.at(0));
                         }else if(sizesTmp.at(0).contains("0") && sizesTmp.at(1).contains("0") && !sizesTmp.at(2).contains("0")){
                             listText.insert("type","Audio");
                         }else if(!sizesTmp.at(0).contains("0") && !sizesTmp.at(1).contains("0") && sizesTmp.at(2).contains("0")){
                             listText.insert("type","Contact");
                         }else if(sizesTmp.at(0).contains("0") && !sizesTmp.at(1).contains("0") && sizesTmp.at(2).contains("0")){
                             listText.insert("type","Location");
                         }else if(sizesTmp.at(0).contains("0") && !sizesTmp.at(1).contains("0") && !sizesTmp.at(2).contains("0")){
                             int length = sizesTmp.at(2).length();
                             if(length < 6){
                                listText.insert("type","Image");
                             }else{
                                listText.insert("type","Video");
                             }
                         }
                     }
                }
            }else{
                return Line(); // No valido
            }

            if(int start = text.indexOf(", id=")){
                if(int end = text.indexOf(",", start + 5)){
                     if(end != -1){
                          listText.insert("id",text.mid((start + 5),(end - (start + 5))));
                     }
                }
            }

            if(int start = text.indexOf(": from=")){
                if(int end = text.indexOf(",", start + 7)){
                     if(end != -1){
                         QString from = text.mid((start + 7),(end - (start + 7)));

                         bool isGroup = false;
                         int numberGroup = from.indexOf("-");
                         if((numberGroup != -1) && (numberGroup <= 15)){ // Solo puede ser el primer -
                            isGroup = true;
                            listText.insert("source","group");
                         }else{
                            isGroup = false;
                            listText.insert("source","private");
                         }

                         if(!isGroup){
                             // Private
                             listText.insert("contact",from);
                         }else{
                             // Group

                             QStringList groupTextList = from.split("-");

                             listText.insert("group",groupTextList.at(0));

                             if(groupTextList.at(1).indexOf("/") != -1){
                                groupTextList = groupTextList.at(1).split("/");
                                listText.insert("contact",groupTextList.at(1)); // Contact
                             }else{
                                listText.insert("contact","unknown");
                             }
                         }


                     }
                }
            }

            Line lin = Line(time,type,listText);
            return lin;

        }else{
            return Line(); // No valido
        }
    // Fin


    }else if(line.contains("chatview.py(625)")){

    //   qDebug() << "v2.8.22 :" << line;

       QTime time = getTimeWALine(line);

       if(!time.isNull()){

           QStringList tmp = line.split(" : ");
           QString text = tmp.at(2).trimmed();

           QString type = "recv-message";

           // contact,type,group,length,id
           QMap<QString,QString> listText;
           listText.insert("contact","");
           listText.insert("source","");
           listText.insert("type","");
           listText.insert("length","");
           listText.insert("id","");

           if(int typeStart = text.indexOf("type=",Qt::CaseInsensitive)){
               if(int typeEnd = text.indexOf(",",typeStart+5)){
                   if(typeEnd != -1){
                       listText.insert("type",text.mid((typeStart+5),(typeEnd - (typeStart + 5))));
                   }
               }
           }

           if(int lengthStart = text.indexOf("len=")){
                listText.insert("length",text.mid(lengthStart+4, text.length() - (lengthStart + 4)));
           }

           if(int idStart = text.indexOf("id=")){
               if(int idEnd = text.indexOf(",", idStart + 3)){
                    if(idEnd != -1){
                         listText.insert("id",text.mid((idStart + 3),(idEnd - (idStart + 3))));
                    }
               }
           }

           int fromStart = text.indexOf("from=");
           bool isGroup = false;
           if(text.indexOf("/",fromStart) != -1){
              isGroup = true;
              listText.insert("source","group");
           }else{
              isGroup = false;
              listText.insert("source","private");
           }

           if(!isGroup){
               // Private
               if(int numberEnd = text.indexOf(",",fromStart+5)){
                   if(numberEnd != -1){
                        listText.insert("contact",text.mid((fromStart + 5), (numberEnd - (fromStart + 5))));
                   }
               }
           }else{
               // Group
               if(int groupEnd = text.indexOf(",",fromStart+5)){
                   QString groupText = text.mid((fromStart + 5),(groupEnd -(fromStart+5)));
                   QStringList groupTextList = groupText.split("/");
                   QStringList groupTextListGroup = groupTextList.at(0).split("-");

                   listText.insert("contact",groupTextList.at(1));
                   listText.insert("group",groupTextListGroup.at(0));
               }
           }

           Line lin = Line(time,type,listText);
           return lin;

       }else{
           return Line(); // No valido
       }
    // Fin
    }else{
        return Line(); // No  valido
    }
}


