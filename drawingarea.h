#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <optional>
#include <memory>

#include <QObject>
#include <QWidget>
#include <QPen>

#include "modification.h"
#include "shapes.h"

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    DrawingArea(QWidget *parent, std::shared_ptr<History> history);
    ~DrawingArea();

    void set_shape_type(ShapeTypes type) { current_shape_type = type; };
    void set_shape_color(QColor color) { current_color = color; };
    void set_shape_thickness(int i) { current_thickness = i; };

    QColor get_shape_color() { return current_color; };

    void add_shape(std::shared_ptr<DrawableShape> shape)
    {
        shapes.push_back(shape);
    }

    void remove_shape(std::shared_ptr<DrawableShape> shape)
    {
        shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
    }
protected:
    virtual void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:
    std::shared_ptr<History> m_history;

    int last_found_control_point_index = 0;

    bool m_dragging = false;
    std::optional<size_t> m_hovered_shape_index;
    std::optional<size_t> m_hovered_control_index;

    enum ShapeTypes current_shape_type;
    QColor current_color;
    int current_thickness = 1;
    std::optional<QPoint> start_pos;
    std::optional<QPoint> end_pos;

    std::optional<std::shared_ptr<DrawableShape>> live_shape;

    std::vector<std::shared_ptr<DrawableShape>> shapes;
};

#endif // DRAWINGAREA_H
