#ifndef TEXTVIEWWINDOW_H
#define TEXTVIEWWINDOW_H

#include <QObject>
#include <QtWidgets>

class TextViewWindow : public QWidget
{
    Q_OBJECT

    public:
       TextViewWindow(QWidget *parent = 0);
       TextViewWindow(QWidget *parent = 0, bool readFlag = false, bool sFlag = false,
                      bool clrFlag = false, bool clsFlag = false);

       ~TextViewWindow();
       void    displayFileSummaryInformation(const QString &fileName);
       void    displayTextString(const QString &textString);
       void    displayTextString(const QString &textString, const bool clear);
       void    refresh();

       virtual void writeSettings() = 0;

       QTextEdit       *mainTextEdit;

    public slots:
         void    doClear();

    protected:

    private slots:
         void    doClose();
         void    doSave();
         void    doFontChange(int);

    private:
         int             fontSize;
         bool            saveFlag;
         bool            readFlag;
         bool            clearFlag;
         bool            closeFlag;
         QPushButton     *saveButton;
         QPushButton     *closeButton;
         QPushButton     *clearButton;
         QSpinBox        *fontSpinBox;
         QLabel          *fontSpinBoxLabel;
         QString         thisFileName;
         QString         thisTextString;

         void buildWindow();
         virtual void saveFile() = 0;
         virtual void readSettings() = 0;
         virtual void clearText() = 0;
};

#endif // TEXTVIEWWINDOW_H
