#include "sss_guiutility.h"

// ------------------------------------------------------------------------------------------------
void adjustQLabel(QLabel *label, int fsize, bool fbold)
{
    QFont font = label->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    label->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQGroupBox(QGroupBox *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQPushButton(QPushButton *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQRadioButton(QRadioButton *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQSpinBox(QSpinBox *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQDoubleSpinBox(QDoubleSpinBox *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQTextEdit(QTextEdit *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQLineEdit(QLineEdit *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQComboBox(QComboBox *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
// ------------------------------------------------------------------------------------------------
void adjustQCheckBox(QCheckBox *widget, int fsize, bool fbold)
{
    QFont font = widget->font();
    font.setPointSize(font.pointSize() + fsize);
    font.setBold(fbold);
    widget->setFont(font);
}
