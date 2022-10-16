#ifndef SSS_CHECKBOX_H
#define SSS_CHECKBOX_H

#include <QtWidgets>

class SSS_CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    SSS_CheckBox(QWidget *parent = 0);
    ~SSS_CheckBox();

    int val1() const;
    int val2() const;

public slots:
    void setVal1(int vv);
    void setVal2(int vv);
    
    void setCheck(SSS_CheckBox *b);

private slots:
    void onPressed(int state);
    void onPressed2(int state);

signals:
    void pressed(int val1, int val2, SSS_CheckBox *b);
    void pressed(int state, int val1, int val2, SSS_CheckBox *b);

private:
    int v1, v2;
    SSS_CheckBox *b;
};
#endif // SSS_CHECKBOX_H
