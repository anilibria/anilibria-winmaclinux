#ifndef CHANGESMODEL_H
#define CHANGESMODEL_H

#include <QObject>
#include <QHash>

class ChangesModel
{

public:
    ChangesModel() noexcept;

private:
    QList<int>* m_NewReleases;
    QList<int>* m_NewOnlineSeries;
    QList<int>* m_NewTorrents;
    QList<int>* m_NewTorrentSeries;

public:
    QList<int>* newReleases() const {return m_NewReleases;}

    QList<int>* newOnlineSeries() const {return m_NewOnlineSeries;}

    QList<int>* newTorrents() const {return m_NewTorrents;}

    QList<int>* newTorrentSeries() const {return m_NewTorrentSeries;}

    void fromJson(const QString &json);

    QString toJson() noexcept;

};

#endif // CHANGESMODEL_H
