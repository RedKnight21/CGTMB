#ifndef SSS_GUIUTILITY_H
#define SSS_GUIUTILITY_H

#include <QtCore>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>

void adjustQLabel(QLabel *label, int fsize, bool fbold);
void adjustQGroupBox(QGroupBox *widget, int fsize, bool fbold);
void adjustQRadioButton(QRadioButton *widget, int fsize, bool fbold);
void adjustQSpinBox(QSpinBox *widget, int fsize, bool fbold);
void adjustQTextEdit(QTextEdit *widget, int fsize, bool fbold);
void adjustQLineEdit(QLineEdit *widget, int fsize, bool fbold);
void adjustQComboBox(QComboBox *widget, int fsize, bool fbold);
void adjustQCheckBox(QCheckBox *widget, int fsize, bool fbold);
void adjustQDoubleSpinBox(QDoubleSpinBox *widget, int fsize, bool fbold);
void adjustQPushButton(QPushButton *widget, int fsize, bool fbold);


//class SSS_GuiUtility
//{
//public:
//    SSS_GuiUtility();
//};

#endif // SSS_GUIUTILITY_H
