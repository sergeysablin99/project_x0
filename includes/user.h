#ifndef USER_H
#define USER_H

#include <QString>

enum position{
  ADMIN,
  USER,
  BOSS
};

class User{
private:
  QString name;
  QString surname;
  QString email;
  QString password;
  position post;

public:
  User(const QString &input_email, const QString &input_password, const position input_post = USER);
  User(const QString &input_name, const QString &input_surname,
       const QString &input_email, const QString &input_password,
       const position input_post = position::USER);

  void setName(const QString &newName);
  void setSurname(const QString &newSurname);
  void setEmail(const QString &newEmail);
  void setPassword(const QString &newPassword);
  void setPost(const position newPost);

  QString getName();
  QString getSurname();
  QString getEmail();
  position getPost();
};

#endif // USER_H
