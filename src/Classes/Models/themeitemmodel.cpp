#include "themeitemmodel.h"

ThemeItemModel::ThemeItemModel()
{

}

void ThemeItemModel::readFromJson(const QJsonObject &object)
{
    if (object.contains("name")) setName(object["name"].toString());
    if (object.contains("author")) setAuthor(object["author"].toString());
    if (object.contains("screenshoot")) setScreenshoot(object["screenshoot"].toString());
    if (object.contains("source")) setSource(object["source"].toString());
}
