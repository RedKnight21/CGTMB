#ifndef SSS_LINEEDIT_H
#define SSS_LINEEDIT_H

#include <QtWidgets>

class SSS_LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SSS_LineEdit(QWidget *parent = 0);

    int val1() const;

public slots:
    void setVal1(int vv);
    
    void setLineEdit(SSS_LineEdit *b);

private slots:
    void onTextChanged(QString);

signals:
    void textChanged(QString s1, int val1);
    
private:
    int v1;
    SSS_LineEdit *b;

};
#endif // SSS_LINEEDIT_H
