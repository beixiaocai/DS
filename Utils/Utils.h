#ifndef UTILS_H
#define UTILS_H
#include <chrono>
#include <QDir>
#include <QString>
#include <QFileInfoList>

class Utils
{
public:
    static int64_t getCurTimestamp()// 获取毫秒级时间戳（13位）
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).
            count();

    }
    static int getCurTimestamp_second()// 获取秒级时间戳（10位）
    {
        int64_t curTimestamp = getCurTimestamp();

        return int(curTimestamp/1000);
    }

    static bool mkDirs(QString dirPath){
        QDir dir(dirPath);
        if(dir.exists()){
          return true;
        }else{
           bool result = dir.mkpath(dirPath);//创建多级目录
           return result;
        }
    }

    static void clearLogDir(const QString &logDir){

        QDir dir(logDir);
        if(!dir.exists()){
            return ;
        }
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Size | QDir::Reversed);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i){
            QFileInfo fileInfo = list.at(i);
            QString filename =fileInfo.fileName();
            if(filename.size() > 10){
                QDateTime time = QDateTime::fromString(filename.mid(0,10),"yyyy-MM-dd");
                if(time.toSecsSinceEpoch() > 0){
                    //此前遗留的日志文件，删除
                    fileInfo.dir().remove(fileInfo.absoluteFilePath());
                }
            }

        }
    }

    static bool clearDir(const QString& path)
    {
        if (path.isEmpty()){
                return false;
        }
        QDir dir(path);
        if(!dir.exists()){
            return true;
        }
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
        QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
        foreach (QFileInfo file, fileList){ //遍历文件信息
            if (file.isFile()){
//                file.dir().remove(file.fileName());
                file.dir().removeRecursively();
            }else{ // 递归删除
                clearDir(file.absoluteFilePath());
            }
        }
        return dir.rmdir(dir.absolutePath());
    }

};

#endif // UTILS_H
