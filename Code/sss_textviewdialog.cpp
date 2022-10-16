#include "sss_textviewdialog.h"

SSS_TextViewDialog::SSS_TextViewDialog(QWidget *parent, unsigned char which) : QDialog(parent)
{
#ifdef USING_WINDOWS
    fontSize       = 9;
#else
    fontSize       = 14;
#endif

    agree          = false;
    thisTextString = "";
    thisFileName   = "";
    whichStyle     = which;

    // Bottom widgets & layout
    if (which == 0)
    {
        okButton  = new QPushButton("OK");
    }
    else
    {
        noButton  = new QPushButton("Don't Agree");
        yesButton = new QPushButton("Agree");
    }

    fontSpinBox = new QSpinBox();
    fontSpinBox->setRange(7,24);
    fontSpinBox->setValue(fontSize);
    fontSpinBoxLabel = new QLabel("Font Size");

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout->addWidget(fontSpinBoxLabel);
    bottomLayout->addWidget(fontSpinBox);
    bottomLayout->addStretch();
    if (which == 0)
    {
        bottomLayout->addWidget(okButton);
    }
    else
    {
        bottomLayout->addWidget(noButton);
        bottomLayout->addWidget(yesButton);
    }

    // Top widgets & layout
    mainTextEdit = new QTextEdit;
    mainTextEdit->setReadOnly(true);
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(mainTextEdit);

    //  Master layout for this window
    QGridLayout *layout = new QGridLayout;
    layout->addLayout(topLayout,    0, 0);
    layout->addLayout(bottomLayout, 1, 0);
    setLayout(layout);

    // Connect signals & slots
    if (which == 0)
    {
        connect(okButton,    SIGNAL(released()),        this, SLOT(handleOK()));
    }
    else
    {
        connect(noButton,    SIGNAL(released()),        this, SLOT(noAgree()));
        connect(yesButton,   SIGNAL(released()),        this, SLOT(yesAgree()));
    }
    connect(fontSpinBox, SIGNAL(valueChanged(int)), this, SLOT(doFontChange(int)));
}
// ------------------------------------------------------------------------------------------------
SSS_TextViewDialog::~SSS_TextViewDialog()
{
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::displayTextString(const QString &textString)
{
    thisTextString = textString;

    QFont courierFont("Courier", fontSize);
    mainTextEdit->setFont(courierFont);
    mainTextEdit->setTabStopWidth(40);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainTextEdit->setPlainText(thisTextString);
    QApplication::restoreOverrideCursor();
}
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::doFontChange(int tmpSize)
{
    fontSize = tmpSize;
    mainTextEdit->clear();
    displayTextString(thisTextString);
}
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::yesAgree()
{
    agree = true;
    close();
}
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::noAgree()
{
    agree = false;
    close();
}
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::closeEvent(QCloseEvent *event)
 {
    textWindowAnswerSignal(agree);
 }
// ------------------------------------------------------------------------------------------------
void SSS_TextViewDialog::handleOK()
{
    agree = true;
    close();
}
// ------------------------------------------------------------------------------------------------
QSize SSS_TextViewDialog::sizeHint() const
{
#ifdef USING_WINDOWS
    return QSize(610, 450);
#else
    return QSize(500, 400);
#endif
}
