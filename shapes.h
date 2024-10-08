#ifndef SHAPES_H
#define SHAPES_H

#include <QPainter>

#include <iostream>

#define CONTROL_POINT_RADIUS 6

enum ShapeTypes {
    Line,
    Rectangle,
    Circle
};

class ControlPoint
{
public:
    ControlPoint(QPoint point);

    QPoint point() {
        return m_point;
    }

    void set_point(QPoint point) {
        m_point = point;
    }

    void set_hovered(bool hovered) {
        m_hovered = hovered;
    }

    bool is_hovered() const {
        return m_hovered;
    }

private:
    QPoint m_point;
    bool m_hovered = false;
};

class DrawableShape
{
public:
    DrawableShape() {};
    virtual ~DrawableShape() {};

    virtual void draw(QPainter &painter);
    virtual void post_draw(QPainter &painter);
    virtual void set_point(size_t index, QPoint point) {};

    virtual void update_bb() {};

    QRect get_bounding_box() const {
        return m_bounding_box;
    }

    std::vector<ControlPoint> &get_control_points() {
        return m_control_points;
    }

    void set_color(QColor color);
    void set_thickness(int thickness);

    std::vector<ControlPoint> m_control_points;

protected:
    QRect m_bounding_box;
    QColor color;
    QPen pen;
};

class DrawableTwoPoints : public DrawableShape
{
public:
    DrawableTwoPoints(QPoint begin, QPoint end) : DrawableShape(), begin(begin), end(end) {
        m_control_points.push_back(ControlPoint(begin));
        m_control_points.push_back(ControlPoint(end));

        DrawableTwoPoints::update_bb();
    };

    void set_point(size_t index, QPoint point) override;

    void update_bb() override {
        DrawableShape::update_bb();

        m_bounding_box = QRect::span(begin, end);
        m_bounding_box.adjust(-CONTROL_POINT_RADIUS, -CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS);
    };

protected:
    QPoint begin;
    QPoint end;
};

class DrawableLine : public DrawableTwoPoints
{
public:
    DrawableLine(QPoint begin, QPoint end) : DrawableTwoPoints(begin, end) {};

    void draw(QPainter &painter) override;
};

class DrawableRectangle : public DrawableTwoPoints
{
public:
    DrawableRectangle(QPoint begin, QPoint end) : DrawableTwoPoints(begin, end), m_fill_color{} {};

    void draw(QPainter &painter) override;

private:
    std::optional<QColor> m_fill_color;
};

#endif // SHAPES_H
