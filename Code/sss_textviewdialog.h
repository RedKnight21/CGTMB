#ifndef SSS_TEXTVIEWDIALOG_H
#define SSS_TEXTVIEWDIALOG_H

//#include "myqwidget.h"
#include <QtWidgets>
#include <QDialog>
#include <QObject>
#include <QWidget>

class SSS_TextViewDialog : public QDialog
{
     Q_OBJECT

    public:
        SSS_TextViewDialog(QWidget *parent = 0, unsigned char which = 0);
        ~SSS_TextViewDialog();
        void    displayTextString(const QString &textString);

    public slots:

    private slots:
        void    doFontChange(int);
        void    handleOK();
        void    noAgree();
        void    yesAgree();
        void    closeEvent(QCloseEvent *event);

    protected:

    signals:
        void textWindowAnswerSignal(bool);

    private:
        int             fontSize;
        QPushButton     *noButton;
        QPushButton     *yesButton;
        QPushButton     *okButton;
        QSpinBox        *fontSpinBox;
        QLabel          *fontSpinBoxLabel;
        QString         thisFileName;
        QString         thisTextString;
        QTextEdit       *mainTextEdit;
        bool            agree;
        unsigned char   whichStyle;
        QSize           sizeHint() const;
};

#endif // SSS_TEXTVIEWDIALOG_H
