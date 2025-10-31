#include "client.h"
#include <cpr/cpr.h>
#include <QMessageBox>
client::client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.helloButton, &QPushButton::clicked, this, &client::HelloWorld);
}
void client::HelloWorld()
{
    cpr::Response r = cpr::Get(cpr::Url{ "http://127.0.0.1:18080/hello" });
    QMessageBox::information(this, "Raspuns server", QString::fromStdString(r.text));
}
client::~client()
{}
