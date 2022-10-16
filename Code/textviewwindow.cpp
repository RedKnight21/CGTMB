#include "textviewwindow.h"

// ----------------------------------------------------------------------------
TextViewWindow::TextViewWindow(QWidget *parent)
{
    saveFlag  = false;
    readFlag  = false;
    clearFlag = false;
    closeFlag = false;
    buildWindow();
}
// ----------------------------------------------------------------------------
TextViewWindow::TextViewWindow(QWidget *parent, bool rFlag, bool sFlag,
                               bool clrFlag, bool clsFlag)
{
    saveFlag  = sFlag;
    readFlag  = rFlag;
    clearFlag = clrFlag;
    closeFlag = clsFlag;
    buildWindow();
}
// ----------------------------------------------------------------------------
void TextViewWindow::buildWindow()
{
#ifdef USING_WINDOWS
    fontSize       = 8;
#else
    fontSize       = 12;
#endif

    thisTextString = "";
    thisFileName   = "";

    // Bottom widgets & layout
    if (closeFlag )
        closeButton = new QPushButton("CLOSE");
    if (saveFlag )
        saveButton  = new QPushButton("SAVE");
    if (clearFlag )
    {
        clearButton  = new QPushButton("CLEAR");
        clearButton->setToolTip("Clear the text window.");
    }

    fontSpinBox = new QSpinBox();
    fontSpinBox->setMinimumWidth(50);
    fontSpinBox->setRange(7,24);
    fontSpinBox->setValue(fontSize);
    fontSpinBox->setToolTip("Change the size of the font for the text window.");
    fontSpinBoxLabel = new QLabel("Font Size");

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(fontSpinBoxLabel);
    bottomLayout->addWidget(fontSpinBox);
    bottomLayout->addStretch();
    if (saveFlag ) bottomLayout->addWidget(saveButton);
    if (clearFlag) bottomLayout->addWidget(clearButton);
    if (closeFlag) bottomLayout->addWidget(closeButton);

    // Top widgets & layout
    mainTextEdit = new QTextEdit;
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(mainTextEdit);
    mainTextEdit->setReadOnly(readFlag);

    //  Master layout for this window
    QGridLayout *layout = new QGridLayout;
    layout->addLayout(topLayout,0,0);
    layout->addLayout(bottomLayout,1,0);
    setLayout(layout);

     // Connect signals & slots
    connect(fontSpinBox, SIGNAL(valueChanged(int)), this, SLOT(doFontChange(int)));
    if (closeFlag )
        connect(closeButton, SIGNAL(released()), this, SLOT(doClose()));
    if (saveFlag )
        connect(saveButton,  SIGNAL(released()), this, SLOT(doSave()));
    if (clearFlag )
        connect(clearButton, SIGNAL(released()), this, SLOT(doClear()));
}
// ------------------------------------------------------------------------------------------------
TextViewWindow::~TextViewWindow()
{
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::displayFileSummaryInformation(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("CGTMB"), tr("Can not read file %1: \n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
    thisFileName = fileName;
    QFont courierFont12("Courier", fontSize);
    mainTextEdit->setFont(courierFont12);
    mainTextEdit->setTabStopWidth(40);

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainTextEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::displayTextString(const QString &textString)
{
    thisTextString = textString;

    QFont courierFont12("Courier", fontSize);
    mainTextEdit->setFont(courierFont12);
    mainTextEdit->setTabStopWidth(40);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mainTextEdit->append(textString);
    QApplication::restoreOverrideCursor();
}

// ------------------------------------------------------------------------------------------------
void TextViewWindow::displayTextString(const QString &textString, const bool clear)
{
    thisTextString = textString;

    QFont courierFont12("Courier", fontSize);
    mainTextEdit->setFont(courierFont12);
    mainTextEdit->setTabStopWidth(40);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (clear)
        mainTextEdit->setPlainText(thisTextString);
    else
        mainTextEdit->append(textString);
    QApplication::restoreOverrideCursor();
}


// ------------------------------------------------------------------------------------------------
void TextViewWindow::refresh()
{
    doClear();
    if (thisTextString == "")
    {
        displayFileSummaryInformation(thisFileName);
    }
    else
    {
        displayTextString(thisTextString);
    }
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::doFontChange(int tmpSize)
{
    fontSize = tmpSize;
    refresh();
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::doClose()
{
    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::doClear()
{
    mainTextEdit->clear();
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::doSave()
{
    saveFile();
}
// ------------------------------------------------------------------------------------------------
void TextViewWindow::saveFile()
{

}
