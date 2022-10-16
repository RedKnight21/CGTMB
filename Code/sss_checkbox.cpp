#include <QtGui>
#include "sss_checkbox.h"

SSS_CheckBox::SSS_CheckBox(QWidget *parent) : QCheckBox(parent), v1(0), v2(0), b(0)
{
        connect(this, SIGNAL(stateChanged(int)), this, SLOT(onPressed(int)));
        connect(this, SIGNAL(stateChanged(int)), this, SLOT(onPressed2(int)));
}

SSS_CheckBox::~SSS_CheckBox()
{
    //qDebug() << "MyTableCheck";
}

int SSS_CheckBox::val1() const
{
        return v1;
}

int SSS_CheckBox::val2() const
{
        return v2;
}

void SSS_CheckBox::setVal1(int vv)
{
        v1 = vv;
}

void SSS_CheckBox::setVal2(int vv)
{
        v2 = vv;
}

void SSS_CheckBox::setCheck(SSS_CheckBox *bb)
{
        b = bb;
}

void SSS_CheckBox::onPressed(int state)
{
        emit pressed(state, v1, b);
}

void SSS_CheckBox::onPressed2(int state)
{
        emit pressed(state, v1, v2, b);
}

