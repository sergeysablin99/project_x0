#include "includes/user.h"

User::User(const QString &input_email,
           const QString &input_password,
           const position input_post) : email(input_email),
  password(input_password), post(input_post){

}

User::User(const QString &input_name, const QString &input_surname,
           const QString &input_email, const QString &input_password,
           const position input_post) : name(input_name),
  surname(input_surname), email(input_email),
  password(input_password), post(input_post){

}

void User::setName(const QString &newName){
  this->name = newName;
}

void User::setPost(const position newPost){
  this->post = newPost;
}

void User::setSurname(const QString &newSurname){
  this->surname = newSurname;
}

void User::setEmail(const QString &newEmail){
  this->email = newEmail;
}

void User::setPassword(const QString &newPassword){
  this->password = newPassword;
}

QString User::getName(){
  return this->name;
}
QString User::getSurname(){
  return this->surname;
}

QString User::getEmail(){
  return this->email;
}
position User::getPost (){
  return this->post;
}
