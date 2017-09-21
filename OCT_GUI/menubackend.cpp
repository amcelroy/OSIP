#include "menubackend.h"

MenuBackend::MenuBackend(QObject *parent)
    : QObject(parent)
{

}

void MenuBackend::menuSelected(QString item){
    m_selectedItem = item;
}

