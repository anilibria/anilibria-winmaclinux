#ifndef CHANGESMODEL_H
#define CHANGESMODEL_H

#include <QObject>
#include <QHash>

class ChangesModel
{

public:
    ChangesModel();

private:
    QList<int>* m_NewReleases;
    QList<int>* m_NewOnlineSeries;
    QList<int>* m_NewTorrents;
    QList<int>* m_NewTorrentSeries;

public:
    QList<int>* newReleases();

    QList<int>* newOnlineSeries();

    QList<int>* newTorrents();

    QList<int>* newTorrentSeries();

    void fromJson(QString json);

    QString toJson();

};

#endif // CHANGESMODEL_H
