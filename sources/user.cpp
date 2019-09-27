#include "includes/user.h"

User::User(const QString &input_email,
           const QString &input_password,
           const position input_post) : _email(input_email),
  _password(input_password), _post(input_post){

}

User::User(const QString &input_name, const QString &input_surname,
           const QString &input_email, const QString &input_password,
           const position input_post) : _name(input_name),
  _surname(input_surname), _email(input_email),
  _password(input_password), _post(input_post){

}

void User::setName(const QString &newName){
  this->_name = newName;
}

void User::setPost(const position newPost){
  this->_post = newPost;
}

void User::setSurname(const QString &newSurname){
  this->_surname = newSurname;
}

void User::setEmail(const QString &newEmail){
  this->_email = newEmail;
}

void User::setPassword(const QString &newPassword){
  this->_password = newPassword;
}

QString User::getName(){
  return this->_name;
}
QString User::getSurname(){
  return this->_surname;
}

QString User::getEmail(){
  return this->_email;
}
position User::getPost (){
  return this->_post;
}
