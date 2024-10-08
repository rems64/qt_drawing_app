#ifndef MODIFICATION_H
#define MODIFICATION_H

#include <vector>
#include <iostream>
#include <memory.h>

#include "shapes.h"

class DrawingArea;
class History;
class MainWindow;

class Modification
{
public:
    Modification() {};

    virtual void apply() {};
    virtual void undo(History *history) {};
};

class MAddShape : public Modification
{
public:
    MAddShape(DrawingArea *drawing_area, std::shared_ptr<DrawableShape> shape);

    void undo(History *history) override;
    void apply() override;

private:
    std::shared_ptr<DrawableShape> m_shape{};
    DrawingArea *m_drawing_area;
};

class MChangeColor : public Modification
{
public:
    MChangeColor(MainWindow *main_window, DrawingArea *drawing_area, QColor new_color);

    void undo(History *history) override;
    void apply() override;

private:
    MainWindow *m_main_window;
    QColor m_old_color;
    QColor m_new_color;
    DrawingArea *m_drawing_area;
};


class History
{
public:
    History();

    void undo();
    void redo();

    void push_and_apply(std::shared_ptr<Modification> modification);

private:
    std::vector<std::shared_ptr<Modification>> m_history;
    std::vector<std::shared_ptr<Modification>> m_back_history;
};

#endif // MODIFICATION_H
