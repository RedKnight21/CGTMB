#ifndef SSS_STRING_UTILS_H
#define SSS_STRING_UTILS_H

#include <QObject>
#include <QWidget>

QString sss_pad_beginning(const int total, const QString str1, const char c1);
QString sss_pad_end(const int total, const QString str1, const char c1);
QString sss_single_space_separator(const QString Str1);

#endif // SSS_STRING_UTILS_H
