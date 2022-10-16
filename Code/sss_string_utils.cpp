#include "sss_string_utils.h"

QString sss_pad_beginning(const int total, const QString str1, const char c1)
{
    QString str2 = str1;
    int len1 = str2.length();
    if (len1 < total)
    {
        len1 = total - len1;
        for(int i=0; i<len1; ++i) str2.insert(0, c1);
    }

    return str2;
}

// ----------------------------------------------------------------------------
QString sss_pad_end(const int total, const QString str1, const char c1)
{
    QString str2 = str1;
    int len1 = str2.length();
    if (len1 < total)
    {
        len1 = total - len1;
        for(int i=0; i<len1; ++i) str2 += c1;
    }

    return str2;
}
// ----------------------------------------------------------------------------
QString sss_single_space_separator(const QString Str1)
{
    QString s1 = Str1;
    s1.leftJustified(1, ' ');
    s1.trimmed();
    int len = s1.length();
    QString s2;

    int j = 0;
    while(s1[j] == ' ')
    {
        ++j;
    }

    s2[0] = s1[j];
    int k = 0;

    for(int i=j+1; i<len; ++i)
    {
        if (s1[i] != ' ')
        {
            s2 += s1[i];
            ++k;
        }
        else if(s1[i] == ' '  &&  s2[k] != ' ')
        {
            s2 += s1[i];
            ++k;
        }
    }
    return s2;
}
