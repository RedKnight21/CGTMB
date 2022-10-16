#include <QtGui>
#include "sss_combobox.h"

// ----------------------------------------------------------------------------

SSS_ComboBox::SSS_ComboBox(QWidget *parent): QComboBox(parent), v1(0), v2(0), cb(0)
{
        connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(valuesChanged(int)));
}

int SSS_ComboBox::val1() const
{
        return v1;
}

int SSS_ComboBox::val2() const
{
        return v2;
}

void SSS_ComboBox::setVal1(int vv)
{
        v1 = vv;
}

void SSS_ComboBox::setVal2(int vv)
{
        v2 = vv;
}

void SSS_ComboBox::setComboBox(SSS_ComboBox *bb)
{
        cb = bb;
}

void SSS_ComboBox::valuesChanged(int value)
{
        emit currentIndexChanged(value, v1, v2);
}

// ----------------------------------------------------------------------------

