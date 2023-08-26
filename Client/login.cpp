#include "login.h"
#include "ui_login.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <mylog.h>
#include <client.h>
#include <QMouseEvent>
using namespace std;

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
    setAttribute(Qt::WA_TranslucentBackground);
    m_areaMovable = geometry();
    m_bPressed = false;
}

Login::~Login()
{
    delete ui;
}


void Login::on_toolButton_2_clicked()
{
    showMinimized();
}

void Login::on_toolButton_clicked()
{
    close();
}
bool check(QString a,QString b){
    return a==b;
}

void Login::onLoginCheckFromClient(const QByteArray &msg) {
    QByteArray dataSrc = msg;
    QDataStream dataStream(&dataSrc, QIODevice::ReadOnly);
    Message::Type type = Message::getType(dataStream);

    if(type != Message::LOGIN_CHECK_MESSAGE) {
        qDebug() << "when login, expect loginCheckMessage but not";
        return;
    }

    struct LoginCheckMessage loginCheckMessage = Message::toLoginCheckMessage(dataStream);
    if(loginCheckMessage.state == Message::FAIL) {
        myLog aa(myLog::Critical, "提示", loginCheckMessage.hint, myLog::Ok);
        aa.show();
        aa.exec();

        //todo
        QString userId = ui->e1->text();
        emit userLogin(userId, loginCheckMessage.username);
    } else {
        myLog aa(myLog::Critical, "提示", "登录成功", myLog::Ok);
        aa.show();
        aa.exec();
    }

}

void Login::on_pushButton_clicked()
{
    bool flag=1;
    QString str = ui->e1->text();
    QString str1 = ui->e2->text();
    qDebug()<<str<<str1<<endl;
    for(auto x:str){
        if(x<'0'||x>'9'){
            flag=0;
            break;
        }
    }
    if(str.size()>20)flag=0;
    if(str.size()*str1.size()==0){
        myLog aa(myLog::Critical, "提示", "账号或密码不能为空，请重试", myLog::Ok);
        aa.show();
        aa.exec();
    }
    else if(!flag){
        myLog aa(myLog::Critical, "提示", "账号为非法输入，请重试", myLog::Ok);
        aa.show();
        aa.exec();
    }
    else{
//        if(check(str,str1)){
//            myLog aa(myLog::Critical, "提示", "登录成功", myLog::Ok);
//            aa.show();
//            aa.exec();
//            //
//            emit userLogin(str,str1);
//        }
//        else{
//            myLog aa(myLog::Critical, "提示", "当前账号不存在或密码错误，请重试", myLog::Ok);
//            aa.show();
//            aa.exec();
//        }

        struct RequestLoginMessage requestLoginMesage(str, str1);
        QByteArray msg = Message::FromRequestLoginMessage(requestLoginMesage);
        emit signalRequestLoginToCommunicator(msg);
    }
}


void Login::on_lineEdit_editingFinished(){
    //int a=1;
}
void Login::on_pushButton_2_clicked()
{
    myLog aa(myLog::Critical, "提示", "注册账号", myLog::Ok);
    aa.show();
    aa.exec();
}

void Login::on_pushButton_3_clicked()
{
    myLog aa(myLog::Critical, "提示", "忘记密码", myLog::Ok);
    aa.show();
    aa.exec();
}
void Login:: loginWindowClose(){
    close();
}

void Login::mousePressEvent(QMouseEvent *e)
{
  //鼠标左键
  if(e->button() == Qt::LeftButton)
  {
  m_ptPress = e->pos();
  qDebug() << pos() << e->pos() << m_ptPress;
  m_bPressed = m_areaMovable.contains(m_ptPress);
  }
}

void Login::mouseMoveEvent(QMouseEvent *e)
{
  if(m_bPressed)
  {
  qDebug() << pos() << e->pos() << m_ptPress;
  move(pos() + e->pos() - m_ptPress);
  }
}

void Login::mouseReleaseEvent(QMouseEvent *)
{
  m_bPressed = false;
}

//设置鼠标按下的区域
void Login::setAreaMovable(const QRect rt)
{
  if(m_areaMovable != rt)
  {
  m_areaMovable = rt;
  }
}

void Login::initLogin(Client &client) {
    QObject::connect(&client, &Client::signalLoginCheckToLogin, this, &Login::onLoginCheckFromClient);
    QObject::connect(this, &Login::userLogin, &client, &Client::rcvLogin);
    QObject::connect(&client, &Client::closeLoginWindow, this, &Login::loginWindowClose);
    this->show();
//    emit userLogin("id1", "nihao");
}
