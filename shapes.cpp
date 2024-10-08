#include "shapes.h"

ControlPoint::ControlPoint(QPoint point) : m_point(point)
{
}

void DrawableShape::set_color(QColor color) {
    color = color;
    pen.setColor(color);
}

void DrawableShape::set_thickness(int thickness) {
    pen.setWidth(thickness);
}

void DrawableTwoPoints::set_point(size_t index, QPoint point) {
    switch (index) {
    case 0:
        begin = point;
        m_control_points[0].set_point(point);
        update_bb();
        break;
    case 1:
        end = point;
        m_control_points[1].set_point(point);
        m_bounding_box = QRect::span(begin, end);
        m_bounding_box.adjust(-CONTROL_POINT_RADIUS, -CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS);
        break;
    default:
        break;
    }
}

void DrawableShape::draw(QPainter &painter) {
    painter.setPen(pen);
    // painter.drawRect(m_bounding_box.x(), m_bounding_box.y(), m_bounding_box.width(), m_bounding_box.height());
}

void DrawableShape::post_draw(QPainter &painter) {
    QPen pen;
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    // std::vector<QPoint> points;
    // points.reserve(m_control_points.size());
    for (auto control_point=m_control_points.begin(); control_point != m_control_points.end(); control_point++) {
        // points.push_back(control_point->point());
        // int r = control_point->is_hovered() ? 2*CONTROL_POINT_RADIUS : CONTROL_POINT_RADIUS;
        if (control_point->is_hovered()) {
            painter.drawEllipse(control_point->point(), CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS);
        }
    }
    // painter.drawPoints(points.data(), points.size());
}

void DrawableLine::draw(QPainter &painter) {
    DrawableShape::draw(painter);

    painter.drawLine(begin.x(), begin.y(), end.x(), end.y());

    DrawableShape::post_draw(painter);
}

void DrawableRectangle::draw(QPainter &painter) {
    DrawableShape::draw(painter);

    if (m_fill_color.has_value()) {
        painter.setBrush(QBrush());
    } else {
        painter.setBrush(QBrush());
    }
    painter.drawRect(begin.x(), begin.y(), end.x() - begin.x(), end.y() - begin.y());

    DrawableShape::post_draw(painter);
}
