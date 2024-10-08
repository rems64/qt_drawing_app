#include "drawingarea.h"

#include <QEvent>
#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(QWidget *parent, std::shared_ptr<History> history) : QWidget(parent), m_history(history), start_pos(), end_pos(), current_color(QColor::fromRgb(255, 255, 255)), shapes{}, current_shape_type(ShapeTypes::Line) {
    setMinimumSize(512, 512);
    setMouseTracking(true);
}

DrawingArea::~DrawingArea() {}

void DrawingArea::paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);

    QPainter painter(this);
    // Registered shapes
    for (auto shape = shapes.begin(); shape != shapes.end(); shape++) {
        (*shape)->draw(painter);
    }

    // Live shape
    if (start_pos.has_value() && end_pos.has_value()) {
        (*live_shape)->draw(painter);
    }
}


void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (m_hovered_shape_index.has_value() && m_hovered_control_index.has_value()) {
        // A handle is hovered
        m_dragging = true;
    } else {
        // We draw
        start_pos = event->pos();
        switch (current_shape_type) {
        case ShapeTypes::Line:
            live_shape = std::make_unique<DrawableLine>(start_pos.value(), start_pos.value());
            break;
        case ShapeTypes::Rectangle:
            live_shape = std::make_unique<DrawableRectangle>(start_pos.value(), start_pos.value());
            break;
        default:
            break;
        }
        if (live_shape.has_value()) {
            live_shape.value()->set_color(current_color);
            live_shape.value()->set_thickness(current_thickness);
        }
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();

    if (m_dragging && m_hovered_shape_index.has_value() && m_hovered_control_index.has_value()) {
        shapes[m_hovered_shape_index.value()]->set_point(m_hovered_control_index.value(), point);
        update();
    } else {
        bool drawing = start_pos.has_value();
        bool found = false;
        size_t control_index = 0;
        size_t shape_index = 0;
        int index = 0;
        for (auto shape = shapes.begin(); shape != shapes.end(); shape++) {
            QRect bb = (*shape)->get_bounding_box();
            std::vector<ControlPoint> &control_points = (*shape)->get_control_points();
            if (!found && !drawing && bb.contains(point)) {
                size_t i = 0;
                for (auto control=control_points.begin(); control != control_points.end(); control++, i++) {
                    if (found) {
                        control->set_hovered(false);
                        continue;
                    }
                    int r = (point-control->point()).manhattanLength();
                    found = r<(2*CONTROL_POINT_RADIUS);
                    index = found ? index : index+1;
                    control->set_hovered(found);
                    // We can set it freely, once the right value
                    // is reached it won't move again
                    control_index = i;
                }
            } else {
                if (!found)
                    index += control_points.size();
                std::vector<ControlPoint> &control_points = (*shape)->get_control_points();
                for (auto control=control_points.begin(); control != control_points.end(); control++) {
                    control->set_hovered(false);
                }
            }
            if (!found)
                shape_index++;
        }

        if (found) {
            m_hovered_shape_index = shape_index;
            m_hovered_control_index = control_index;
        } else {
            m_hovered_shape_index = {};
            m_hovered_control_index = {};
        }

        if (last_found_control_point_index != index)
            update();
        last_found_control_point_index = index;

        if (drawing) {
            end_pos = event->pos();
            if (live_shape.has_value()) {
                live_shape->get()->set_point(1, event->pos());
            }
            update();
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (start_pos.has_value() && end_pos.has_value()) {
        m_history->push_and_apply(std::make_shared<MAddShape>(this, live_shape.value()));
        // shapes.emplace_back(std::move(live_shape.value()));
    }
    m_dragging = false;
    start_pos.reset();
    end_pos.reset();
    update();
}
