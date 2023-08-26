#ifndef myLog_H
#define myLog_H

#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMetaEnum>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

class myLog : public QDialog {
    Q_OBJECT

public:
    enum Icon {
        NoIcon = 0,
        Information = 1,
        Question = 2,
        Success = 3,
        Warning = 4,
        Critical = 5
    };
    Q_ENUM(Icon)  //使用Q_ENUM注册枚举

    enum StandardButton {
        //尽量保持与 QDialogButtonBox::StandardButton 一致，在创建按钮时可能会用到 & 运算
        NoButton = 0x00000000,
        Ok = 0x00000400,
        Save = 0x00000800,
        SaveAll = 0x00001000,
        Open = 0x00002000,
        Yes = 0x00004000,
        YesToAll = 0x00008000,
        No = 0x00010000,
        NoToAll = 0x00020000,
        Abort = 0x00040000,
        Retry = 0x00080000,
        Ignore = 0x00100000,
        Close = 0x00200000,
        Cancel = 0x00400000,
        Discard = 0x00800000,
        Help = 0x01000000,
        Apply = 0x02000000,
        Reset = 0x04000000,
        RestoreDefaults = 0x08000000,

        FirstButton = Ok,
        LastButton = RestoreDefaults
    };
    Q_ENUM(StandardButton)
    Q_DECLARE_FLAGS(StandardButtons, StandardButton)
    Q_FLAG(StandardButtons)

    enum ButtonRole {
        //保持与 QDialogButtonBox::ButtonRole 一致
        InvalidRole = -1,
        AcceptRole,
        RejectRole,
        DestructiveRole,
        ActionRole,
        HelpRole,
        YesRole,
        NoRole,
        ResetRole,
        ApplyRole,

        NRoles
    };

    explicit myLog(QWidget* parent = nullptr);
    myLog(Icon icon, const QString& title, const QString& text, StandardButtons buttons,
                 QWidget* parent = nullptr);  //构造函数有默认值的要放后面
    ~myLog();

    void setTitle(const QString& title);

    Icon icon() const;
    void setIcon(Icon icon);

    QPixmap iconPixmap() const;
    void setIconPixmap(const QPixmap& pixmap);

    QString text() const;
    void setText(const QString& text);

    StandardButtons standardButtons() const;
    void setStandardButtons(StandardButtons buttons);
    StandardButton standardButton(QAbstractButton* button) const;
    QAbstractButton* button(StandardButton which) const;

    ButtonRole buttonRole(QAbstractButton* button) const;

    QAbstractButton* clickedButton() const;

    static StandardButton information(QWidget* parent, const QString& text, QString title = "Message", StandardButtons buttons = Ok);
    static StandardButton question(QWidget* parent, const QString& text, QString title = "Message", StandardButtons buttons = StandardButtons(Yes | No));
    static StandardButton success(QWidget* parent, const QString& text, QString title = "Message", StandardButtons buttons = Ok);
    static StandardButton warning(QWidget* parent, const QString& text, QString title = "Message", StandardButtons buttons = Ok);
    static StandardButton critical(QWidget* parent, const QString& text, QString title = "Message", StandardButtons buttons = Ok);

    static StandardButton showMessageBox(QWidget* parent, Icon icon, const QString& text, QString title, StandardButtons buttons);
    static void setMessageBoxGeometry(QWidget* parent, myLog& msgBox);

private slots:
    void slotPushButtonClicked(QAbstractButton* button);

private:
    void init();
    void setupLayout();
    QPixmap standardIcon(Icon icon);
    void setClickedButton(QAbstractButton* button);
    void finalize(QAbstractButton* button);
    int dialogCodeForButton(QAbstractButton* button) const;
    void setPushButtonProperty(QList<QAbstractButton*> buttonList);

private:
    QLabel* m_pIconLabel;
    myLog::Icon m_icon;
    QLabel* m_pTextLabel;
    QLabel* m_pLineLabel;
    QDialogButtonBox* m_pButtonBox;
    QAbstractButton* m_pClickedButton;
};



//在全局任意地方使用"|"操作符计算自定义的枚举量，需要使用Q_DECLARE_OPERATORS_FOR_FLAGS宏
Q_DECLARE_OPERATORS_FOR_FLAGS(myLog::StandardButtons)

#endif  // myLog_H
