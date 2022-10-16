#include <QtGui>
#include "sss_pushbutton.h"


SSS_PushButton::SSS_PushButton(QWidget *parent)
              : QPushButton(parent), v1(0), v2(0), b(0)
{
        connect(this, SIGNAL(pressed()), this,  SLOT(onPressed()));
        connect(this, SIGNAL(released()), this, SLOT(onReleased()));
}

int SSS_PushButton::val1() const
{
        return v1;
}

int SSS_PushButton::val2() const
{
        return v2;
}

void SSS_PushButton::setVal1(int vv)
{
        v1 = vv;
}

void SSS_PushButton::setVal2(int vv)
{
        v2 = vv;
}

void SSS_PushButton::setButton(SSS_PushButton *bb)
{
        b = bb;
}

void SSS_PushButton::onPressed()
{
        emit pressed(v1, v2, b);
}

void SSS_PushButton::onReleased()
{
    emit released(v1, v2);
}

