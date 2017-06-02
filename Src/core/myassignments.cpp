#include "myassignments.h"

MyAssignments::MyAssignments()
{
    QFile myAssignmentsFile("myAssignments.xml");
    if (!myAssignmentsFile.open(QFile::ReadWrite | QFile::Text)) {
         qDebug()<<__FUNCTION__<<__LINE__<<"QXmlStream myAssignments.xml Cannot write file"<<myAssignmentsFile.errorString();
     }
    QString element;
    xml.setDevice(&myAssignmentsFile);
    while (!xml.atEnd()) {
           xml.readNext();
           element = xml.readElementText();
           // do processing
           if (xml.readNextStartElement()) {
                   qDebug()<<"Next Start Element"<<xml.readElementText();

           }
               else
                   xml.raiseError(QObject::tr("The file is not an XML version 1.0 file."));

               if (xml.name() == "myRequest")
                   qDebug()<<"My Request";
           }
    if (xml.hasError()) {
         //TODO error handling
       qDebug()<<__FUNCTION__<<__LINE__<<xml.errorString();
    }
}

void MyAssignments::writeXMLFile(QIODevice * file)
{
    QXmlStreamWriter xml(file);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xml>");
    xml.writeStartElement("myRequest");
    xml.writeAttribute("version", "1.0");
    xml.writeEndDocument();
    qDebug()<<__FUNCTION__<<__LINE__<<"My Requests File written";
}
