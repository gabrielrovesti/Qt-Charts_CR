#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QtCore>
#include <QtWidgets>

class exceptions: public QException{
private:
    QString message;
public:
    void raise() const override { throw *this; }
    exceptions *clone() const override { return new exceptions(*this); }
    exceptions(const QString& msg = "");
    exceptions()=default;
    operator QString();
};

class file_exceptions: public exceptions{
public:
    file_exceptions(const QString& msg = "");
};

class logical_exceptions: public exceptions{
public:
    logical_exceptions(const QString& msg = "");
};

class graphical_exceptions: public exceptions{
public:
    graphical_exceptions(const QString& msg = "");
};

#endif // EXCEPTIONS_H
