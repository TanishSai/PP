#ifndef MYASSIGNMENTS_H
#define MYASSIGNMENTS_H
#include <QFile>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

//TODO - I see this code is moved to proxypics class.Please
//remove this files if it is not required for feature usage.
class MyAssignments
{
public:
    MyAssignments();
    //TODO - Please move the member variables to private access specifier.
    QXmlStreamReader xml;

    void writeXMLFile(QIODevice *file);
};

#endif // MYASSIGNMENTS_H
