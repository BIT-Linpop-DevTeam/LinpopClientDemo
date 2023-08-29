#include "userprofile.h"
#include "ui_userprofile.h"
#include <QMouseEvent>
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
    //setFont(":/src/ui.ttf");
    ui->stack->setCurrentIndex(0);
    ui->listWidget->addItem("C语言中文网");
    ui->listWidget->addItem("http://c.biancheng.net");
    //!!!
    //QObject::connect(&listWidget,&QListWidget::itemClicked,&print,&QMyLabel::rsetText);
    connect(ui->pushButtonChangeName,&QPushButton::clicked,this,&UserProfile::switchPage);
    connect(ui->pushButtonChangeAvatar,&QPushButton::clicked,this,&UserProfile::switchPage);
    connect(ui->pushButtonChangeSignature,&QPushButton::clicked,this,&UserProfile::switchPage);

    connect(ui->confirmUsername, &QPushButton::clicked, this, &UserProfile::onConfirmUsernameClicked);
    connect(ui->confirmSign, &QPushButton::clicked, this, &UserProfile::onConfirmSignatureClicked);
}

UserProfile::~UserProfile()
{
    delete ui;
}

void UserProfile::switchPage(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());//得到按下的按钮的指针
    if(button==ui->pushButtonChangeName)
        ui->stack->setCurrentIndex(0);//根据按下的button按索引显示相应的页面
    else if(button==ui->pushButtonChangeAvatar)
        ui->stack->setCurrentIndex(1);
    else{
        ui->stack->setCurrentIndex(2);
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

void UserProfile::onConfirmUsernameClicked()
{
    QString username = ui->usernameEdit->text();
    if(username.size() <= 0)	return;
    emit signalConfirmUsernameToClient(username);
}

void UserProfile::onConfirmSignatureClicked()
{
    QString signature = ui->signatureEdit->text();
    if(signature.size() <= 0) 	return;
    emit signalConfirmSignatureToClient(signature);
}

//void UserProfile::onConfirm
