#include "student.h"
#include <QDebug>

Student::Student(QObject *parent) : QObject(parent)
{
    qDebug() << "Student:" << this;
}

Student::~Student()
{
    qDebug() << "~Student:" << this;
}

Student::Student(const Student &other)
{
    qDebug() << "Copy Student:" << this;
    this->name_ = other.name_;
    this->id_ = other.id_;
}

Student &Student::operator=(const Student &other)
{
    this->name_ = other.name_;
    this->id_ = other.id_;
    return *this;
}
