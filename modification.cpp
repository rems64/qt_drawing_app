#include "modification.h"

#include "drawingarea.h"
#include "mainwindow.h"

MAddShape::MAddShape(DrawingArea *drawing_area, std::shared_ptr<DrawableShape> shape) : m_shape(shape), m_drawing_area(drawing_area)
{
}

void MAddShape::undo(History *history)
{
    Modification::undo(history);
    m_drawing_area->remove_shape(m_shape);
}

void MAddShape::apply()
{
    m_drawing_area->add_shape(m_shape);
}

MChangeColor::MChangeColor(MainWindow *main_window, DrawingArea *drawing_area, QColor color) : m_drawing_area(drawing_area), m_new_color(color), m_main_window(main_window)
{
    m_old_color = m_drawing_area->get_shape_color();
}

void MChangeColor::apply()
{
    Modification::apply();
    m_drawing_area->set_shape_color(m_new_color);
}

void MChangeColor::undo(History *history)
{
    Modification::undo(history);
    m_drawing_area->set_shape_color(m_old_color);
    m_main_window->update_selector_color();
}


History::History() : m_history{}
{

}


void History::push_and_apply(std::shared_ptr<Modification> modification)
{
    m_history.push_back(modification);
    modification->apply();
}

void History::undo() {
    if (m_history.size() > 0) {
        std::shared_ptr<Modification> modification = m_history[m_history.size()-1];
        modification->undo(this);
        m_back_history.push_back(modification);
        m_history.pop_back();
    }
}

void History::redo() {
    if (m_back_history.size() > 0) {
        std::shared_ptr<Modification> modification = m_back_history[m_back_history.size()-1];
        modification->apply();
        m_history.push_back(modification);
        m_back_history.pop_back();
    }
}
