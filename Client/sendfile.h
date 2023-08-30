#ifndef SENDFILE_H
#define SENDFILE_H

#include <QWidget>
#include <QFileInfo>

namespace Ui {
class SendFile;
}

class SendFile : public QWidget
{
    Q_OBJECT

public:
    explicit SendFile(QWidget *parent = nullptr);
    ~SendFile();
    void initsender(QString filePath, QFileInfo info, qint64 size);

private:
    Ui::SendFile *ui;
    QString filename;
    qint64 filesize;
    QString filepath;

private slots:
    void on_openFileButton_clicked();
    void on_openFolderButton_clicked();

};

#endif // SENDFILE_H
