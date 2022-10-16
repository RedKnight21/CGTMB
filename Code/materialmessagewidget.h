#ifndef MATERIALMESSAGEWIDGET_H
#define MATERIALMESSAGEWIDGET_H

#include <QObject>
#include <QWidget>
#include "textviewwindow.h"

class MaterialMessageWidget : public TextViewWindow
{
public:
    MaterialMessageWidget(QWidget *parent, bool readFlag, bool saveFlag, bool clrFlag, bool clsFlag);
    ~MaterialMessageWidget();

    void  writeSettings();
    void  readSettings();

private:
    void  saveFile();
    void  clearText();
    QSize sizeHint() const;

};

#endif // MATERIALMESSAGEWIDGET_H
