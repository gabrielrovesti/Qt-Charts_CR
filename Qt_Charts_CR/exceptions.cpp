#include "exceptions.h"

exceptions::exceptions(const QString& msg): message(msg){}

exceptions::operator QString() {
    return QString(message);
}

logical_exceptions::logical_exceptions(const QString& msg): exceptions(msg){
    if(msg != ""){
        QMessageBox criticalBox;
        criticalBox.critical(0,"Errore","Errore nell'elaborazione dei dati");
        criticalBox.setFixedSize(500,200);
    }
}

file_exceptions::file_exceptions(const QString& msg): exceptions(msg){
    QMessageBox criticalBox;
    if(msg == "XML"){
        criticalBox.critical(0,"Errore","Errore nell'elaborazione XML");
        criticalBox.setFixedSize(500,200);
    }
    if(msg == "JSON"){
        criticalBox.critical(0,"Errore","Errore nell'elaborazione JSON");
        criticalBox.setFixedSize(500,200);
    }
}

graphical_exceptions::graphical_exceptions(const QString& msg): exceptions(msg){
    QMessageBox criticalBox;
    if(msg == "Window"){
        criticalBox.critical(0,"Errore","Errore nella finestra");
        criticalBox.setFixedSize(500,200);
    }
    if(msg == "Mapper"){
        criticalBox.critical(0,"Errore","Errore nella creazione del mapper");
        criticalBox.setFixedSize(500,200);
    }
    if(msg == "Table"){
        criticalBox.critical(0,"Errore","Errore nella creazione della tabella");
        criticalBox.setFixedSize(500,200);
    }
}
