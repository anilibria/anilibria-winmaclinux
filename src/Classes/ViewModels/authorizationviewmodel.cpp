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

#include <QUrl>
#include <QJsonDocument>
#include "authorizationviewmodel.h"

AuthorizationViewModel::AuthorizationViewModel(QObject *parent) : QObject(parent),
    m_apiService(new AnilibriaApiService(this))
{
    connect(m_apiService, &AnilibriaApiService::signinReceived,this,&AuthorizationViewModel::handleSignin);
}

void AuthorizationViewModel::setLogin(const QString &login)
{
    if (m_login == login) return;

    m_login = login;
    emit loginChanged();
}

void AuthorizationViewModel::setPassword(const QString &password)
{
    if (m_password == password) return;

    m_password = password;
    emit passwordChanged();
}

void AuthorizationViewModel::setTwoFactorCode(const QString &twoFactorCode)
{
    if (m_twoFactorCode == twoFactorCode) return;

    m_twoFactorCode = twoFactorCode;
    emit twoFactorCodeChanged();
}

void AuthorizationViewModel::resetModel()
{
    setLogin("");
    setPassword("");
    setTwoFactorCode("");
    setErrorMessage("");
}

void AuthorizationViewModel::signin()
{
    if (m_login.isEmpty()) {
        setErrorMessage("Поле <b>Логин</b> не заполнено!");
        return;
    }
    if (m_password.isEmpty()) {
        setErrorMessage("Поле <b>Пароль</b> не заполнено!");
        return;
    }

    m_apiService->signin(QUrl::toPercentEncoding(m_login), QUrl::toPercentEncoding(m_password), QUrl::toPercentEncoding(m_twoFactorCode));
}

void AuthorizationViewModel::setErrorMessage(const QString &errorMessage)
{
    if (m_errorMessage == errorMessage) return;

    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}

void AuthorizationViewModel::handleSignin(QString token, QString data)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8(), &jsonError);
    auto object = jsonDocument.object();
    auto errorReply = object.value("err").toString();
    auto errorMessage = object.value("mes").toString();

    if (errorReply == "ok") {
        resetModel();
        emit successAuthentificated(token);
        return;
    }

    setErrorMessage(errorMessage);
}
