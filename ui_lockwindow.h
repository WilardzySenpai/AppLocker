/********************************************************************************
** Form generated from reading UI file 'lockwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCKWINDOW_H
#define UI_LOCKWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LockWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QFrame *headerFrame;
    QVBoxLayout *headerLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QFrame *mainFrame;
    QVBoxLayout *mainLayout;
    QVBoxLayout *appSelectionLayout;
    QLabel *appLabel;
    QComboBox *appComboBox;
    QVBoxLayout *appPasswordLayout;
    QLabel *appPasswordLabel;
    QLineEdit *appPasswordEdit;
    QVBoxLayout *masterPasswordLayout;
    QLabel *masterPasswordLabel;
    QLineEdit *masterPasswordEdit;
    QSpacerItem *verticalSpacer1;
    QHBoxLayout *actionButtonsLayout;
    QPushButton *lockButton;
    QPushButton *unlockButton;
    QSpacerItem *verticalSpacer2;
    QPushButton *changeMasterButton;
    QPushButton *updateButton;

    void setupUi(QMainWindow *LockWindow)
    {
        if (LockWindow->objectName().isEmpty())
            LockWindow->setObjectName("LockWindow");
        LockWindow->resize(450, 550);
        LockWindow->setMinimumSize(QSize(450, 550));
        LockWindow->setMaximumSize(QSize(450, 550));
        LockWindow->setStyleSheet(QString::fromUtf8("\n"
"    QMainWindow {\n"
"     background-color: #f5f5f5;\n"
"    }\n"
"    QLabel {\n"
"     color: #333;\n"
"     font-size: 11pt;\n"
"    }\n"
"    QLabel#titleLabel {\n"
"     font-size: 18pt;\n"
"     font-weight: bold;\n"
"     color: #2c3e50;\n"
"    }\n"
"    QLabel#subtitleLabel {\n"
"     font-size: 10pt;\n"
"     color: #7f8c8d;\n"
"    }\n"
"    QComboBox, QLineEdit {\n"
"     padding: 10px;\n"
"     border: 2px solid #ddd;\n"
"     border-radius: 8px;\n"
"     background-color: white;\n"
"     font-size: 10pt;\n"
"    }\n"
"    QComboBox:hover, QLineEdit:hover {\n"
"     border-color: #3498db;\n"
"    }\n"
"    QComboBox:focus, QLineEdit:focus {\n"
"     border-color: #3498db;\n"
"     background-color: #f8fbff;\n"
"    }\n"
"    QPushButton {\n"
"     padding: 12px 24px;\n"
"     border: none;\n"
"     border-radius: 8px;\n"
"     font-size: 11pt;\n"
"     font-weight: bold;\n"
"    }\n"
"    QPushButton#lockButton {\n"
"     background-color: #27ae60;\n"
"     color: white;\n"
"    }\n"
"    QP"
                        "ushButton#lockButton:hover {\n"
"     background-color: #229954;\n"
"    }\n"
"    QPushButton#unlockButton {\n"
"     background-color: #e74c3c;\n"
"     color: white;\n"
"    }\n"
"    QPushButton#unlockButton:hover {\n"
"     background-color: #c0392b;\n"
"    }\n"
"    QPushButton#changeMasterButton {\n"
"     background-color: #3498db;\n"
"     color: white;\n"
"    }\n"
"    QPushButton#changeMasterButton:hover {\n"
"     background-color: #2980b9;\n"
"    }\n"
"    QPushButton#updateButton {\n"
"     background-color: #95a5a6;\n"
"     color: white;\n"
"    }\n"
"    QPushButton#updateButton:hover {\n"
"     background-color: #7f8c8d;\n"
"    }\n"
"    QFrame#headerFrame {\n"
"     background-color: white;\n"
"     border-radius: 12px;\n"
"    }\n"
"    QFrame#mainFrame {\n"
"     background-color: white;\n"
"     border-radius: 12px;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(LockWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(30, 30, 30, 30);
        headerFrame = new QFrame(centralwidget);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setFrameShape(QFrame::Shape::StyledPanel);
        headerLayout = new QVBoxLayout(headerFrame);
        headerLayout->setSpacing(8);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(20, 20, 20, 20);
        titleLabel = new QLabel(headerFrame);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        headerLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(headerFrame);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        headerLayout->addWidget(subtitleLabel);

        mainFrame = new QFrame(headerFrame);
        mainFrame->setObjectName("mainFrame");
        mainFrame->setFrameShape(QFrame::Shape::StyledPanel);
        mainLayout = new QVBoxLayout(mainFrame);
        mainLayout->setSpacing(20);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(25, 25, 25, 25);
        appSelectionLayout = new QVBoxLayout();
        appSelectionLayout->setSpacing(8);
        appSelectionLayout->setObjectName("appSelectionLayout");
        appLabel = new QLabel(mainFrame);
        appLabel->setObjectName("appLabel");

        appSelectionLayout->addWidget(appLabel);

        appComboBox = new QComboBox(mainFrame);
        appComboBox->addItem(QString());
        appComboBox->addItem(QString());
        appComboBox->addItem(QString());
        appComboBox->addItem(QString());
        appComboBox->addItem(QString());
        appComboBox->setObjectName("appComboBox");

        appSelectionLayout->addWidget(appComboBox);


        mainLayout->addLayout(appSelectionLayout);

        appPasswordLayout = new QVBoxLayout();
        appPasswordLayout->setSpacing(8);
        appPasswordLayout->setObjectName("appPasswordLayout");
        appPasswordLabel = new QLabel(mainFrame);
        appPasswordLabel->setObjectName("appPasswordLabel");

        appPasswordLayout->addWidget(appPasswordLabel);

        appPasswordEdit = new QLineEdit(mainFrame);
        appPasswordEdit->setObjectName("appPasswordEdit");
        appPasswordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        appPasswordLayout->addWidget(appPasswordEdit);


        mainLayout->addLayout(appPasswordLayout);

        masterPasswordLayout = new QVBoxLayout();
        masterPasswordLayout->setSpacing(8);
        masterPasswordLayout->setObjectName("masterPasswordLayout");
        masterPasswordLabel = new QLabel(mainFrame);
        masterPasswordLabel->setObjectName("masterPasswordLabel");

        masterPasswordLayout->addWidget(masterPasswordLabel);

        masterPasswordEdit = new QLineEdit(mainFrame);
        masterPasswordEdit->setObjectName("masterPasswordEdit");
        masterPasswordEdit->setEchoMode(QLineEdit::EchoMode::Password);

        masterPasswordLayout->addWidget(masterPasswordEdit);


        mainLayout->addLayout(masterPasswordLayout);

        verticalSpacer1 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        mainLayout->addItem(verticalSpacer1);

        actionButtonsLayout = new QHBoxLayout();
        actionButtonsLayout->setSpacing(15);
        actionButtonsLayout->setObjectName("actionButtonsLayout");
        lockButton = new QPushButton(mainFrame);
        lockButton->setObjectName("lockButton");

        actionButtonsLayout->addWidget(lockButton);

        unlockButton = new QPushButton(mainFrame);
        unlockButton->setObjectName("unlockButton");

        actionButtonsLayout->addWidget(unlockButton);


        mainLayout->addLayout(actionButtonsLayout);

        verticalSpacer2 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        mainLayout->addItem(verticalSpacer2);

        changeMasterButton = new QPushButton(mainFrame);
        changeMasterButton->setObjectName("changeMasterButton");

        mainLayout->addWidget(changeMasterButton);

        updateButton = new QPushButton(mainFrame);
        updateButton->setObjectName("updateButton");

        mainLayout->addWidget(updateButton);


        headerLayout->addWidget(mainFrame);


        verticalLayout->addWidget(headerFrame);

        LockWindow->setCentralWidget(centralwidget);

        retranslateUi(LockWindow);

        QMetaObject::connectSlotsByName(LockWindow);
    } // setupUi

    void retranslateUi(QMainWindow *LockWindow)
    {
        LockWindow->setWindowTitle(QCoreApplication::translate("LockWindow", "App Locker", nullptr));
        titleLabel->setText(QCoreApplication::translate("LockWindow", "\360\237\224\222 App Locker", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("LockWindow", "Secure your applications with password protection", nullptr));
        appLabel->setText(QCoreApplication::translate("LockWindow", "Select Application:", nullptr));
        appComboBox->setItemText(0, QCoreApplication::translate("LockWindow", "Android Studio", nullptr));
        appComboBox->setItemText(1, QCoreApplication::translate("LockWindow", "Chrome", nullptr));
        appComboBox->setItemText(2, QCoreApplication::translate("LockWindow", "WhatsApp", nullptr));
        appComboBox->setItemText(3, QCoreApplication::translate("LockWindow", "Instagram", nullptr));
        appComboBox->setItemText(4, QCoreApplication::translate("LockWindow", "Settings", nullptr));

        appPasswordLabel->setText(QCoreApplication::translate("LockWindow", "App Password:", nullptr));
        appPasswordEdit->setPlaceholderText(QCoreApplication::translate("LockWindow", "Enter password for this app", nullptr));
        masterPasswordLabel->setText(QCoreApplication::translate("LockWindow", "Master Password:", nullptr));
        masterPasswordEdit->setPlaceholderText(QCoreApplication::translate("LockWindow", "Enter your master password", nullptr));
        lockButton->setText(QCoreApplication::translate("LockWindow", "\360\237\224\222 Lock", nullptr));
        unlockButton->setText(QCoreApplication::translate("LockWindow", "\360\237\224\223 Unlock", nullptr));
        changeMasterButton->setText(QCoreApplication::translate("LockWindow", "\360\237\224\221 Change Master Password", nullptr));
        updateButton->setText(QCoreApplication::translate("LockWindow", "\360\237\224\204 Check for Updates", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LockWindow: public Ui_LockWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCKWINDOW_H
