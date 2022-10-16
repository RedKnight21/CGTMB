#include <QtGui>
#include "sss_linedit.h"


SSS_LineEdit::SSS_LineEdit(QWidget *parent) : QLineEdit(parent), v1(0)
{
        connect(this, SIGNAL(textChanged(QString)), this,  SLOT(onTextChanged(QString)));
}

int SSS_LineEdit::val1() const
{
        return v1;
}

void SSS_LineEdit::setVal1(int vv)
{
        v1 = vv;
}

void SSS_LineEdit::setLineEdit(SSS_LineEdit *bb)
{
        b = bb;
}

void SSS_LineEdit::onTextChanged(QString s)
{
        emit textChanged(s, v1);
}


