/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUTHORIZATIONVIEWMODEL_H
#define AUTHORIZATIONVIEWMODEL_H

#include <QObject>
#include "../Services/anilibriaapiservice.h"

class AuthorizationViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString twoFactorCode READ twoFactorCode WRITE setTwoFactorCode NOTIFY twoFactorCodeChanged)
    Q_PROPERTY(QString registerLink READ registerLink NOTIFY registerLinkChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

private:
    QString m_login { "" };
    QString m_password { "" };
    QString m_twoFactorCode { "" };
    QString m_registerLink { "<a href='https://www.anilibria.tv/pages/login.php'>Регистрация через сайт</a>" };
    QString m_errorMessage { "" };
    AnilibriaApiService* m_apiService;

public:
    explicit AuthorizationViewModel(QObject *parent = nullptr);

    QString login() { return m_login; }
    void setLogin(const QString& login);

    QString password() { return m_password; }
    void setPassword(const QString& password);

    QString twoFactorCode() { return m_twoFactorCode; }
    void setTwoFactorCode(const QString& twoFactorCode);

    QString registerLink() { return m_registerLink; }

    QString errorMessage() { return m_errorMessage; }

    Q_INVOKABLE void resetModel();
    Q_INVOKABLE void signin();

private:
    void setErrorMessage(const QString& errorMessage);

signals:
    void loginChanged();
    void passwordChanged();
    void twoFactorCodeChanged();
    void registerLinkChanged();
    void errorMessageChanged();
    void successAuthentificated(const QString& token);

private slots:
    void handleSignin(QString token, QString data);

};

#endif // AUTHORIZATIONVIEWMODEL_H
