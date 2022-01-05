#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);
    ~Student();

    Student(const Student &other);
    Student &operator=(const Student &other);

public:
    //编号
    QString     id_;
    //名称
    QString     name_;
};

Q_DECLARE_METATYPE(Student)

#endif // STUDENT_H
