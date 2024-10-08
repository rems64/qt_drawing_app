#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

#include "modification.h"

class DrawingArea;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void quit();
    void set_color();
    void set_thickness(int thickness);
    void undo();
    void redo();

    virtual void closeEvent(QCloseEvent *event) override;

    void update_selector_color();
private:
    std::shared_ptr<History> m_history;

    Ui::MainWindow *ui;
    DrawingArea *drawing_area;
    QPushButton *color_selector;
    QPushButton *undo_btn;
};
#endif // MAINWINDOW_H
