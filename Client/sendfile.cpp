#include "sendfile.h"
#include "ui_sendfile.h"
#include <QFileDialog>
#include <QDesktopServices>

SendFile::SendFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendFile)
{
    ui->setupUi(this);
//    ui->filename->setFrame(QFrame::NoFrame);
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setWindowFlags(Qt::Window);
    ui->filename->setStyleSheet(QString("font: 25 9pt Microsoft YaHei;"));
    ui->filesize->setStyleSheet(QString("font: 25 8pt Microsoft YaHei;"));
    ui->filename->setReadOnly(true);
    connect(ui->openFileButton, &QPushButton::clicked, this, &SendFile::on_openFileButton_clicked);
    connect(ui->openFolderButton, &QPushButton::clicked, this, &SendFile::on_openFolderButton_clicked);


}

SendFile::~SendFile()
{
    delete ui;
}

void SendFile::initsender(QString filePath, QFileInfo info, qint64 size)
{
    filepath = filePath;
    ui->filename->setText(info.fileName());
    int siz = static_cast<int>(size) / 1024;
    QString str =  tr("%1kb").arg(siz);
    ui->filesize->setText(str);
    QString suffix = info.completeSuffix();
    if(suffix == "doc")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/doc.png);"));
        }
        else if(suffix == "exe")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/exe.png)"));
        }
        else if(suffix == "jpg" || suffix == "png")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/jpg.png)"));
        }
        else if(suffix == "mp3")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/mp3.png)"));
        }
        else if(suffix == "mp4")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/mp4.png)"));
        }
        else if(suffix == "pdf")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/pdf.png)"));
        }
        else if(suffix == "ppt" || suffix == "pptx")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/ppt.png)"));
        }
        else if(suffix == "rar")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/rar.png)"));
        }
        else if(suffix == "txt")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/txt.png)"));
        }
        else if(suffix == "xls" || suffix == "xlsx" )
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/xlsx.png)"));
        }
        else if(suffix == "zip")
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/zip.png)"));
        }
        else
        {
            ui->fileicon->setStyleSheet(QString("border-image: url(:/src/GUI/fileicon/file.png)"));
        }
}

void SendFile::on_openFileButton_clicked()
{
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!filepath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
    }
}

void SendFile::on_openFolderButton_clicked()
{
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    if (!filepath.isEmpty()) {
        QFileInfo fileInfo(filepath);
        QString folderPath = fileInfo.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
    }
}

