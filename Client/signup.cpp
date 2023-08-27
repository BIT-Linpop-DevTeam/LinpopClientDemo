#include "signup.h"
#include "ui_signup.h"
#include <mylog.h>
#include <client.h>
#include <QMouseEvent>
#include<mylog.h>
#include "login.h"
SignUp::SignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    m_areaMovable = geometry();
    m_bPressed = false;
}

void SignUp::mousePressEvent(QMouseEvent *e)
{
  //鼠标左键
  if(e->button() == Qt::LeftButton)
  {
  m_ptPress = e->pos();
//  qDebug() << pos() << e->pos() << m_ptPress;
  m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void SignUp::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
//  qDebug() << pos() << e->pos() << m_ptPress;
  move(pos() + e->pos() - m_ptPress);
  }
}

void SignUp::mouseReleaseEvent(QMouseEvent *)
{
  m_bPressed = false;
}

//设置鼠标按下的区域
void SignUp::setAreaMovable(const QRect rt)
{
  if(m_areaMovable != rt)
  {
  m_areaMovable = rt;
  }
}

void SignUp::on_toolButton_2_clicked()
{
    showMinimized();
}

void SignUp::on_toolButton_clicked()
{
    close();
}

void SignUp::on_pushButton_clicked()
{
    qDebug() << "in slots of register: onclicked";
    QString Username = ui->e1->text();
    QString UserPassword = ui->e2->text();
    bool flag=1;
    for(auto x:Username){
        if(x=='/'||x=='\\'||x=='|'||x=='\''||x=='"'){
            flag=0;
            break;
        }
    }
    if(!flag){
        myLog aa(myLog::Critical, "提示", "包含非法字符/,\\,|,\",'等,请重新输入", myLog::Ok);
        aa.show();
        aa.exec();
    }
    else{
        RequestSignUpMessage requestSignUpMessage(Username, UserPassword);
        QByteArray dataSrc = Message::FromRequestSignUpMessage(requestSignUpMessage);
        emit signalRequestSignUpMessageToLogin(dataSrc);
//        myLog aa(myLog::Critical, "欸嘿", "创建成功", myLog::Ok);
//        aa.show();
//        aa.exec();
    }
}

void SignUp::onSignUpCheckMessageFromLogin(const QByteArray &msg) {
    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);
    if(Message::getType(dataStream) != Message::SIGNUP_CHECK_MESSAGE)	{
        qDebug() << "in slot: onSignUpCheckMessageFromClient. expect signup check, but not.";
    }

    SignUpCheckMessage signUpCheckMessage = Message::toSignUpCheckMessage(dataStream);
    if(signUpCheckMessage.state == Message::SUCCESS) {
        myLog aa(myLog::Critical, "欸嘿", "创建成功", myLog::Ok);
        aa.show();
        aa.exec();
        this->close();
    } else {
        myLog aa(myLog::Critical, "啊哦", "注册失败", myLog::Ok);
        aa.show();
        aa.exec();
    }
}

void SignUp::initSignUp(const Login &login) {
    connect(&login, &Login::signalSignUpCheckMessageToRegister, this, &SignUp::onSignUpCheckMessageFromLogin);
    connect(this, &SignUp::signalRequestSignUpMessageToLogin, &login, &Login::onRequestSignUpMessageFromRegister);
}
