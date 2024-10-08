#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtWidgets/QToolBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QCloseEvent>
#include <QActionGroup>
#include <QComboBox>
#include <QColorDialog>
#include <QSpinBox>
#include <QShortcut>

#include "drawingarea.h"

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
    quit();
}

void MainWindow::quit() {
    QApplication::quit();

    auto ret = QMessageBox::question(this, "Nooo don't leave", "Do you really want to quit?");
    switch (ret) {
    case QMessageBox::Yes:
        QApplication::quit();
        break;
    case QMessageBox::No:
    default:
        break;
    }
}

void MainWindow::set_color() {
    QColor new_color = QColorDialog::getColor(drawing_area->get_shape_color(), this, tr("Choose a color"));
    m_history->push_and_apply(std::make_shared<MChangeColor>(this, drawing_area, new_color));
    // drawing_area->set_shape_color(new_color);

    update_selector_color();
}

void MainWindow::set_thickness(int i) {
    drawing_area->set_shape_thickness(i);
}

void MainWindow::undo()
{
    m_history->undo();

    update();
}

void MainWindow::redo()
{
    m_history->redo();

    update();
}

void MainWindow::update_selector_color() {
    QPalette pal = color_selector->palette();
    pal.setColor(QPalette::Button, drawing_area->get_shape_color());

    color_selector->setAutoFillBackground(true);
    color_selector->setPalette(pal);
    color_selector->update();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_history(std::make_shared<History>())
{
    QMenuBar *mbar = this->menuBar();
    // this->addToolBar(new QToolBar(this));
    QMenu *fmenu = mbar->addMenu(tr("&File"));

    QAction *action_open = new QAction(QIcon(":images/open.png"), tr("&Open..."), this);
    QAction *action_save = new QAction(QIcon(":images/save.png"), tr("&Save..."), this);
    QAction *action_quit = new QAction(QIcon(":images/quit.png"), tr("&Quit..."), this);
    QAction *action_undo = new QAction(QIcon(":images/quit.png"), tr("&Undo..."), this);
    QAction *action_redo = new QAction(QIcon(":images/quit.png"), tr("&Redo..."), this);

    // Why tr ?
    action_open->setShortcut(Qt::CTRL | Qt::Key_O);
    action_save->setShortcut(Qt::CTRL | Qt::Key_S);
    action_quit->setShortcut(Qt::CTRL | Qt::Key_Q);
    action_undo->setShortcut(Qt::CTRL | Qt::Key_Z);
    action_redo->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_Z);

    action_open->setToolTip(tr("Open a file"));
    action_save->setToolTip(tr("Save current file"));
    action_quit->setToolTip(tr("Quit app"));
    action_undo->setToolTip(tr("Undo last action"));
    action_redo->setToolTip(tr("Redo last undone action"));

    fmenu->addAction(action_open);
    fmenu->addAction(action_save);
    fmenu->addAction(action_quit);
    fmenu->addAction(action_undo);
    fmenu->addAction(action_redo);

    connect(action_quit, &QAction::triggered, this, [&]() {this->quit();});
    connect(action_undo, &QAction::triggered, this, [&]() {this->undo();});
    connect(action_redo, &QAction::triggered, this, [&]() {this->redo();});

    drawing_area = new DrawingArea(this, m_history);

    QToolBar *toolbar = this->addToolBar(tr("Tools"));
    toolbar->setMovable(false);

    QComboBox *shape_combo = new QComboBox(this);
    QSpinBox *thickness = new QSpinBox(this);
    thickness->setMinimum(1);
    thickness->setMaximum(1000);
    thickness->setStepType(QSpinBox::StepType::AdaptiveDecimalStepType);

    color_selector = new QPushButton(this);
    undo_btn = new QPushButton(this);

    QAction *action_add_line = new QAction(QIcon(":images/open.png"), tr("Add line"), this);

    shape_combo->addItem(tr("Line"));
    shape_combo->addItem(tr("Rectangle"));

    drawing_area->set_shape_color(QColor::fromRgb(100, 255, 255));

    update_selector_color();

    connect(shape_combo, &QComboBox::currentIndexChanged, this, [&](size_t index){
        switch (index) {
        case 0:
            drawing_area->set_shape_type(ShapeTypes::Line);
            break;
        case 1:
            drawing_area->set_shape_type(ShapeTypes::Rectangle);
            break;
        default:
            break;
        }
    });

    QAction *action_set_color = new QAction(QIcon(":images/open.png"), tr("Set color"), this);

    color_selector->addAction(action_set_color);
    undo_btn->addAction(action_undo);

    connect(color_selector, &QPushButton::clicked, this, [&]() {this->set_color();});
    connect(thickness, &QSpinBox::valueChanged, this, [&](int thickness) {this->set_thickness(thickness);});

    connect(undo_btn, &QPushButton::clicked, this, [&]() {this->undo();});

    toolbar->addWidget(shape_combo);
    toolbar->addWidget(color_selector);
    toolbar->addWidget(thickness);
    toolbar->addSeparator();
    toolbar->addWidget(undo_btn);

    this->setCentralWidget(drawing_area);
    this->statusBar();
}

MainWindow::~MainWindow()
{
    delete drawing_area;
    delete ui;
    delete undo_btn;
}
