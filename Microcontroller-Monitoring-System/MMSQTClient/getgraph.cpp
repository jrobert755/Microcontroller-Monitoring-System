#include "getgraph.h"
#include "ui_getgraph.h"

GetGraph::GetGraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetGraph)
{
    ui->setupUi(this);
}

GetGraph::~GetGraph()
{
    delete ui;
}
