#ifndef MENUBACKEND_H
#define MENUBACKEND_H

#include <QObject>
#include <QString>

class MenuBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString selectedItem WRITE menuSelected READ getSelectedItem NOTIFY menuClicked)

public:
    explicit MenuBackend(QObject *parent = nullptr);

    QString getSelectedItem() { return m_selectedItem; }

signals:
    void menuClicked();

public slots:
    void menuSelected(QString item);

private:
    QString m_selectedItem;
};

#endif // MENUBACKEND_H
