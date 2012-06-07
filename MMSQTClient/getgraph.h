#ifndef GETGRAPH_H
#define GETGRAPH_H

#include <QDialog>

namespace Ui {
class GetGraph;
}

class GetGraph : public QDialog
{
    Q_OBJECT
    
public:
    explicit GetGraph(QWidget *parent = 0);
    ~GetGraph();
    
private:
    Ui::GetGraph *ui;
};

#endif // GETGRAPH_H
