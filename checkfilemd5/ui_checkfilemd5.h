/********************************************************************************
** Form generated from reading UI file 'checkfilemd5.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKFILEMD5_H
#define UI_CHECKFILEMD5_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckFileMd5
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEditA;
    QPushButton *pushButtonSelectA;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEditB;
    QPushButton *pushButtonSelectB;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QLabel *labelResult;
    QPushButton *pushButtonCheckMd5;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *CheckFileMd5)
    {
        if (CheckFileMd5->objectName().isEmpty())
            CheckFileMd5->setObjectName(QString::fromUtf8("CheckFileMd5"));
        CheckFileMd5->resize(473, 204);
        verticalLayout = new QVBoxLayout(CheckFileMd5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEditA = new QLineEdit(CheckFileMd5);
        lineEditA->setObjectName(QString::fromUtf8("lineEditA"));

        horizontalLayout->addWidget(lineEditA);

        pushButtonSelectA = new QPushButton(CheckFileMd5);
        pushButtonSelectA->setObjectName(QString::fromUtf8("pushButtonSelectA"));

        horizontalLayout->addWidget(pushButtonSelectA);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEditB = new QLineEdit(CheckFileMd5);
        lineEditB->setObjectName(QString::fromUtf8("lineEditB"));

        horizontalLayout_2->addWidget(lineEditB);

        pushButtonSelectB = new QPushButton(CheckFileMd5);
        pushButtonSelectB->setObjectName(QString::fromUtf8("pushButtonSelectB"));

        horizontalLayout_2->addWidget(pushButtonSelectB);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        labelResult = new QLabel(CheckFileMd5);
        labelResult->setObjectName(QString::fromUtf8("labelResult"));

        horizontalLayout_3->addWidget(labelResult);

        pushButtonCheckMd5 = new QPushButton(CheckFileMd5);
        pushButtonCheckMd5->setObjectName(QString::fromUtf8("pushButtonCheckMd5"));

        horizontalLayout_3->addWidget(pushButtonCheckMd5);


        verticalLayout->addLayout(horizontalLayout_3);

        textBrowser = new QTextBrowser(CheckFileMd5);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);


        retranslateUi(CheckFileMd5);

        QMetaObject::connectSlotsByName(CheckFileMd5);
    } // setupUi

    void retranslateUi(QWidget *CheckFileMd5)
    {
        CheckFileMd5->setWindowTitle(QCoreApplication::translate("CheckFileMd5", "CheckFileMd5", nullptr));
        pushButtonSelectA->setText(QCoreApplication::translate("CheckFileMd5", "\351\200\211\346\213\251\346\226\207\344\273\266A", nullptr));
        pushButtonSelectB->setText(QCoreApplication::translate("CheckFileMd5", "\351\200\211\346\213\251\346\226\207\344\273\266B", nullptr));
        labelResult->setText(QString());
        pushButtonCheckMd5->setText(QCoreApplication::translate("CheckFileMd5", "\346\243\200\346\237\245Md5", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CheckFileMd5: public Ui_CheckFileMd5 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKFILEMD5_H
