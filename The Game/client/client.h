#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_client.h"

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = nullptr);
    ~client();

private slots:
    void HelloWorld();
private:
    Ui::clientClass ui;
};

