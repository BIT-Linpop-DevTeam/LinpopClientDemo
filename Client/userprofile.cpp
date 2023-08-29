#include "userprofile.h"
#include "ui_userprofile.h"
#include <QMouseEvent>
#include<QDebug>

#include<QLabel>
#include<QFileDialog>

UserProfile::UserProfile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserProfile)
{
    ui->setupUi(this);
    //ui->frame->setStyleSheet("");

    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    m_areaMovable = geometry();
    m_bPressed = false;
    ui->stack->setCurrentIndex(0);
    QStringList imagePathList;
    imagePathList.clear();
        ui->listWidget->setViewMode(QListWidget::IconMode);//显示模式
        ui->listWidget->setIconSize(QSize(100, 100));//设置图片大小
        ui->listWidget->setSpacing(10);//间距
        ui->listWidget->setResizeMode(QListView::Adjust);//适应布局调整
        ui->listWidget->setMovement(QListView::Static);//不能移动

    QString Filepath;
    Filepath =QDir::currentPath (); qDebug () << Filepath;
    int index = Filepath.lastIndexOf("/");
        Filepath = Filepath.left(index);
        qDebug()<<"path=="<<Filepath;
        Filepath+="/Client/src/GUI/head";
        qDebug()<<"path=="<<Filepath;
        QString dirPath_ = Filepath;
        qDebug()<<Filepath;
        QDir dir(Filepath);
        QStringList filters;
        filters<<"*.png"<<"*.jpg";  // 设置哪些格式图片的可以显示
        dir.setNameFilters(filters);
        // 设置显示的item
        for(uint idx=0;idx<dir.count();idx++)
        {
            QString imagePath = QString("%1\\%2").arg(Filepath).arg(dir[idx]);
            imagePathList.push_back(imagePath);
            QListWidgetItem *pItem = new QListWidgetItem;
            pItem->setSizeHint(QSize(60,60));
            pItem->setText(imagePath);
            pItem->setIcon(QIcon(QPixmap(imagePathList[idx]).scaled(QSize(60,60))));
            ui->listWidget->addItem(pItem);
        }
        qDebug()<<"all imagepath: "<<imagePathList;
    ui->pushButtonChangeName->setStyleSheet("font: '微软雅黑';font-size: 20pt;color:rgba(144,14,177,100);"
                                            "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
    ui->pushButtonChangeAvatar->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                            "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
    ui->pushButtonChangeSignature->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                            "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
    //!!!
    //QObject::connect(&listWidget,&QListWidget::itemClicked,&print,&QMyLabel::rsetText);
    connect(ui->pushButtonChangeName,&QPushButton::clicked,this,&UserProfile::switchPage);
    connect(ui->pushButtonChangeAvatar,&QPushButton::clicked,this,&UserProfile::switchPage);
    connect(ui->pushButtonChangeSignature,&QPushButton::clicked,this,&UserProfile::switchPage);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &UserProfile::on_listWidget_itemClicked);

    connect(ui->confirmUsername, &QPushButton::clicked, this, &UserProfile::onConfirmUsernameClicked);
    connect(ui->confirmSign, &QPushButton::clicked, this, &UserProfile::onConfirmSignatureClicked);
    connect(ui->confirmAvatar, &QPushButton::clicked, this, &UserProfile::onConfirmAvatarClicked);
}

UserProfile::~UserProfile()
{
    delete ui;
}

void UserProfile::switchPage(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());//得到按下的按钮的指针
    if(button==ui->pushButtonChangeName){
        ui->stack->setCurrentIndex(0);//根据按下的button按索引显示相应的页面
        ui->pushButtonChangeName->setStyleSheet("font: '微软雅黑';font-size: 20pt;color:rgba(144,14,177,100);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeAvatar->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeSignature->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        }


    else if(button==ui->pushButtonChangeAvatar){
        ui->pushButtonChangeName->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeAvatar->setStyleSheet("font: '微软雅黑';font-size: 20pt;color:rgba(144,14,177,100);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeSignature->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");

        ui->stack->setCurrentIndex(1);
    }
    else{
        ui->stack->setCurrentIndex(2);
        ui->pushButtonChangeName->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeAvatar->setStyleSheet("font: '微软雅黑';font-size: 13pt;color:rgb(0,0,0);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
        ui->pushButtonChangeSignature->setStyleSheet("font: '微软雅黑';font-size: 20pt;color:rgba(144,14,177,100);"
                                                "QPushButton#pushButtonChangeName::hover{font: '微软雅黑';font-size: 50pt;color:rgb(0,0,0);}");
    }

}

void UserProfile::on_toolButton_2_clicked()
{
    minimumSize();
}

void UserProfile::on_toolButton_clicked()
{
    close();
}

void UserProfile::mousePressEvent(QMouseEvent *e)
{
  //鼠标左键
  if(e->button() == Qt::LeftButton)
  {
  m_ptPress = e->pos();
//  qDebug() << pos() << e->pos() << m_ptPress;
  m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void UserProfile::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
//  qDebug() << pos() << e->pos() << m_ptPress;
  move(pos() + e->pos() - m_ptPress);
  }
}

void UserProfile::mouseReleaseEvent(QMouseEvent *)
{
  m_bPressed = false;
}

//设置鼠标按下的区域
void UserProfile::setAreaMovable(const QRect rt)
{
  if(m_areaMovable != rt)
  {
  m_areaMovable = rt;
  }
}

void UserProfile::on_cancelButton_2_clicked()
{
    close();
}

void UserProfile::on_cancelButton_3_clicked()
{
    close();
}

void UserProfile::on_cancelButton_1_clicked()
{
    close();
}

QString ss="C:/Users/lyl/Desktop/qt/client/new/LinpopClientDemo/Client/src/GUI/head\\0.jpg";
void UserProfile::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ss=item->text();//单击操作
    //qDebug()<<ss;
}


int getIconId(){
    int flag=0;
    QString id;
    for(auto x:ss){

        if(x=='.')break;
        if(flag==1){
            id+=x;
        }
        if(x=='\\')flag++;
    }
    flag=0;
    flag=id.toInt();
    return flag;

}

void UserProfile::onConfirmUsernameClicked()
{
    QString username = ui->usernameEdit->text();
    if(username.size() <= 0)	return;
    emit signalConfirmUsernameToClient(username);
    close();
}

void UserProfile::onConfirmSignatureClicked()
{
    QString signature = ui->signatureEdit->text();
    if(signature.size() <= 0) 	return;
    emit signalConfirmSignatureToClient(signature);
    close();
}

void UserProfile::onConfirmAvatarClicked()
{
    qint32 avatarId = getIconId();
    emit signalConfirmAvatarToClient(avatarId);
    close();
}

