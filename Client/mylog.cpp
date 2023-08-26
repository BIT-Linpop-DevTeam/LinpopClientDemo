#include "mylog.h"

#define MESSAGEWIDTH 300  //界面的大小
#define MESSAGEHEIGHT 225

#define TEXTFONTFAMILY "微软雅黑"  //text的字体样式和大小
#define TEXTFONTSIZE 12

#define BUTTONFONTFAMILY "微软雅黑"  //按钮的字体样式和大小
#define BUTTONFONTSIZE 10

#define BUTTONWIDTH 100  //按钮的大小
#define BUTTONHEIGHT 30

myLog::myLog(QWidget* parent) :
    QDialog(parent) {
    init();
}

myLog::myLog(Icon icon, const QString& title, const QString& text, StandardButtons buttons, QWidget* parent) :
    QDialog(parent) {
    init();
    setTitle(title);
    setIcon(icon);
    setText(text);
    if (buttons != NoButton)
        setStandardButtons(buttons);
}

myLog::~myLog() {}

void myLog::setTitle(const QString& title) {
    setWindowTitle(title);
}

myLog::Icon myLog::icon() const {
    return m_icon;
}

void myLog::setIcon(myLog::Icon icon) {
    setIconPixmap(standardIcon(icon));
    m_icon = icon;
}

QPixmap myLog::iconPixmap() const {
    return *m_pIconLabel->pixmap();
}

void myLog::setIconPixmap(const QPixmap& pixmap) {
    //    m_pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pIconLabel->setPixmap(pixmap);
    m_icon = NoIcon;
}

QString myLog::text() const {
    return m_pTextLabel->text();
}

void myLog::setText(const QString& text) {
    m_pTextLabel->setFont(QFont(TEXTFONTFAMILY, TEXTFONTSIZE));
    m_pTextLabel->setText(text);
}

myLog::StandardButtons myLog::standardButtons() const {
    QDialogButtonBox::StandardButtons standardButtons = m_pButtonBox->standardButtons();
    return myLog::StandardButtons(int(standardButtons));  //不同类型的枚举转换
}

void myLog::setStandardButtons(StandardButtons buttons) {
    QDialogButtonBox::StandardButtons standardButtons = QDialogButtonBox::StandardButtons(int(buttons));
    m_pButtonBox->setStandardButtons(standardButtons);
    //    m_pButtonBox->setStandardButtons(QDialogButtonBox::StandardButtons(int(buttons)));  //上面两句归为一句
    setPushButtonProperty(m_pButtonBox->buttons());
}

myLog::StandardButton myLog::standardButton(QAbstractButton* button) const {
    QDialogButtonBox::StandardButton standardButton = m_pButtonBox->standardButton(button);
    return (myLog::StandardButton)standardButton;  //转化为当前类的StandardButton类型
}

QAbstractButton* myLog::button(myLog::StandardButton which) const {
    QDialogButtonBox::StandardButton standardButton = QDialogButtonBox::StandardButton(which);
    return m_pButtonBox->button(standardButton);
}

myLog::ButtonRole myLog::buttonRole(QAbstractButton* button) const {
    QDialogButtonBox::ButtonRole buttonRole = m_pButtonBox->buttonRole(button);
    return myLog::ButtonRole(buttonRole);
}

QAbstractButton* myLog::clickedButton() const {
    return m_pClickedButton;
}

myLog::StandardButton myLog::information(QWidget* parent, const QString& text, QString title, StandardButtons buttons) {
    return showMessageBox(parent, Information, text, title, buttons);
}

myLog::StandardButton myLog::question(QWidget* parent, const QString& text, QString title, StandardButtons buttons) {
    return showMessageBox(parent, Question, text, title, buttons);
}

myLog::StandardButton myLog::success(QWidget* parent, const QString& text, QString title, StandardButtons buttons) {
    return showMessageBox(parent, Success, text, title, buttons);
}

myLog::StandardButton myLog::warning(QWidget* parent, const QString& text, QString title, StandardButtons buttons) {
    return showMessageBox(parent, Warning, text, title, buttons);
}

myLog::StandardButton myLog::critical(QWidget* parent, const QString& text, QString title, StandardButtons buttons) {
    return showMessageBox(parent, Critical, text, title, buttons);
}

myLog::StandardButton myLog::showMessageBox(QWidget* parent, Icon icon, const QString& text, QString title, StandardButtons buttons) {
    myLog msgBox(icon, title, text, buttons, parent);
    //静态函数只能调用静态函数，setMessageBoxGeometry()必须声明为静态函数
    setMessageBoxGeometry(parent, msgBox);
    if (msgBox.exec() == -1)
        return myLog::Cancel;
    return msgBox.standardButton(msgBox.clickedButton());
}

void myLog::setMessageBoxGeometry(QWidget* parent, myLog& msgBox) {
    QRect rect = parent->geometry();
    int x = rect.x() + (rect.width() - msgBox.geometry().width()) / 2;
    int y = rect.y() + (rect.height() - msgBox.geometry().height()) / 2;
    msgBox.setGeometry(x, y, msgBox.geometry().width(), msgBox.geometry().height());
    msgBox.move(x, y);
}

void myLog::slotPushButtonClicked(QAbstractButton* button) {
    setClickedButton(button);
    finalize(button);
    close();
}

void myLog::init() {
    resize(QSize(MESSAGEWIDTH, MESSAGEHEIGHT));
    setWindowIcon(QIcon("C:/Users/lyl/Desktop/3.png"));
    setWindowTitle("Message");
    //对于继承的QDialog，其Flags必须包含Qt::Dialog，不能自定义CustomizeWindowHint
    //否则在构造中指定了parent之后会出现：标题栏消失以及窗口不在正中间
    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);  //设置右上角按钮
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);  //去掉标题栏右上角的问号
    //setAttribute(Qt::WA_DeleteOnClose);                                   //关闭窗口时将窗口释放掉即释放内存

    m_pIconLabel = new QLabel(this);
    m_pIconLabel->setObjectName(QLatin1String("iconLabel"));
    m_pIconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  //图标不可拉伸和缩小，固定大小
    m_pTextLabel = new QLabel(this);
    m_pTextLabel->setObjectName(QLatin1String("textLabel"));
    m_pTextLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);  //label中的内容可用鼠标选择文本复制，链接激活
    m_pTextLabel->setOpenExternalLinks(true);                           //label中的内容若为链接，可直接点击打开
    m_pLineLabel = new QLabel(this);
    m_pLineLabel->setFrameStyle(QFrame::HLine | QFrame::Sunken);  //Sunken:凹陷，Raised：凸起
    m_pButtonBox = new QDialogButtonBox(this);                    //默认按钮为水平布局Qt::Horizontal
    m_pButtonBox->setObjectName(QLatin1String("buttonBox"));
    connect(m_pButtonBox, &QDialogButtonBox::clicked, this, &myLog::slotPushButtonClicked);

    setupLayout();

    setModal(true);

    setStyleSheet("QDialog{background-color:rgb(255 , 255 , 255);padding:0px;}"
                  "QPushButton{background-color:#E0E0E0;border: 0px solid #A6A6A6;border-radius:5px;}"
                  "QPushButton:hover{color:white;background-color:#4188FF;border-radius:5px;border: 0px}"
                   //                 "QPushButton{background-color:#E0E0E0;border: 0px solid #C2C2C2;border-radius:5px;}"
                  //                  "QPushButton:hover{background-color:#C2C2C2;border-radius:5px;border: 0px}"
                  "QPushButton:pressed{padding-left:0px;padding-top:3px;}");
}

void myLog::setupLayout() {
    QHBoxLayout* HLay = new QHBoxLayout;
    HLay->addWidget(m_pIconLabel, 1, Qt::AlignVCenter | Qt::AlignRight);
    HLay->addWidget(m_pTextLabel, 5, Qt::AlignCenter);
    QHBoxLayout* HLay1 = new QHBoxLayout;
    HLay1->addWidget(m_pButtonBox, Qt::AlignRight);
        //QMargins margin;
        //margin.setRight(20);
        //HLay1->setContentsMargins(margin);  //调节按钮不要太靠右
    QVBoxLayout* VLay = new QVBoxLayout;
    VLay->addLayout(HLay, 10);
    VLay->addWidget(m_pLineLabel, 1);
    VLay->addLayout(HLay1, 4);
    VLay->setSpacing(0);

    setLayout(VLay);
}

QPixmap myLog::standardIcon(Icon icon) {
    QPixmap pixmap;
    switch (icon) {
        case myLog::Information:
            pixmap.load(":/new/prefix1/Image/Information.png");
            break;
        case myLog::Question:
            pixmap.load(":/new/prefix1/Image/Question.png");
            break;
        case myLog::Success:
            pixmap.load(":/new/prefix1/Image/Success.png");
            break;
        case myLog::Warning:
            pixmap.load(":/new/prefix1/Image/Warning.png");
            break;
        case myLog::Critical:
            //pixmap.load("C:/Users/lyl/Desktop/1.png");
            break;
        default:
            break;
    }
    if (!pixmap.isNull())
        return pixmap;
    return QPixmap();
}

void myLog::setClickedButton(QAbstractButton* button) {
    m_pClickedButton = button;
}

void myLog::finalize(QAbstractButton* button) {
    int dialogCode = dialogCodeForButton(button);
    if (dialogCode == QDialog::Accepted) {
        emit accept();
    } else if (dialogCode == QDialog::Rejected) {
        emit reject();
    }
}

int myLog::dialogCodeForButton(QAbstractButton* button) const {
    switch (buttonRole(button)) {
        case myLog::AcceptRole:
        case myLog::YesRole:
            return QDialog::Accepted;
        case myLog::RejectRole:
        case myLog::NoRole:
            return QDialog::Rejected;
        default:
            return -1;
    }
}

void myLog::setPushButtonProperty(QList<QAbstractButton*> buttonList) {
    for (int i = 0; i < buttonList.size(); i++) {
        QPushButton* pushButton = qobject_cast<QPushButton*>(buttonList.at(i));
        pushButton->setMinimumSize(BUTTONWIDTH, BUTTONHEIGHT);
        pushButton->setFont(QFont(BUTTONFONTFAMILY, BUTTONFONTSIZE));
    }
}
