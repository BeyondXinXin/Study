#include "DoubleSlider.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionSlider>

#include <cmath>
#include <qdrawutil.h>

static bool DoublesAreEqual(double left, double right)
{
    static constexpr double qtDoublePrecision { 1e-12 };
    return std::abs(left - right) <= qtDoublePrecision * std::max({ 1.0, std::abs(left), std::abs(right) });
}

DoubleSlider::DoubleSlider(double min, double max, QWidget * parent)
  : QSlider(parent)
  , min_(min)
  , max_(max)
  , current_min_(min)
  , current_max_(max)
  , last_emitted_min_(min)
  , last_emitted_max_(max)

{
    setOrientation(Qt::Horizontal);
    setMouseTracking(true);
    setMinimum(0);
    setMaximum(MAX_PERCENT);
    SetHandleRect();
}

double DoubleSlider::GetCurrentMin() const
{
    return current_min_;
}

double DoubleSlider::GetCurrentMax() const
{
    return current_max_;
}

double DoubleSlider::GetMin() const
{
    return min_;
}

double DoubleSlider::GetMax() const
{
    return max_;
}

void DoubleSlider::SetCurrentMin(double currentMinToSet)
{
    if (DoublesAreEqual(current_min_, currentMinToSet))
        return;
    current_min_ = GetNormalizedValue(currentMinToSet);
    if (current_min_ > current_max_)
        SetCurrentMax(current_min_);
    update();
}

void DoubleSlider::SetCurrentMax(double currentMaxToSet)
{
    if (DoublesAreEqual(current_max_, currentMaxToSet)) {
        return;
    }

    current_max_ = GetNormalizedValue(currentMaxToSet);
    if (current_max_ < current_min_)
        SetCurrentMin(current_max_);

    update();
}

double DoubleSlider::GetNormalizedValue(double value)
{
    if (value <= max_ && value >= min_)
        return value;

    return value > max_ ? max_ : min_;
}

int DoubleSlider::GetMousePosition(QMouseEvent * event) const
{
    QStyleOptionSlider defaultStyle;
    initStyleOption(&defaultStyle);
    const QRect inner_rect(defaultStyle.rect);
    const int slider_bar_width { inner_rect.width() - inner_rect.x() };
    // 手柄绘制矩形根据手柄上的位置移动。
    const double shift_ratio = static_cast<double>(event->x()) / slider_bar_width - 0.5;
    const int shift_in_pixels = static_cast<int>(std::round(handle_rect_.width() * shift_ratio));
    const int pos = QStyle::sliderValueFromPosition(0, MAX_PERCENT, event->x() + shift_in_pixels, slider_bar_width);
    return pos;
}

void DoubleSlider::mousePressEvent(QMouseEvent *)
{
    moving_ = Handle::NONE;
}

void DoubleSlider::mouseReleaseEvent(QMouseEvent * event)
{
    moving_ = Handle::NONE;
    mouseMoveEvent(event);
    update();
}

void DoubleSlider::changeEvent(QEvent * event)
{
    QSlider::changeEvent(event);

    if (event->type() == QEvent::StyleChange)
        SetHandleRect();
}

bool DoubleSlider::MouseIsOnHandle(int mouse_pos_x, int handle_pos) const
{
    QStyleOptionSlider defaultStyle;
    initStyleOption(&defaultStyle);
    const QRect inner_rect(defaultStyle.rect);
    const int slider_bar_width { inner_rect.width() - inner_rect.x() };
    int handle_pos_x = QStyle::sliderPositionFromValue(0, MAX_PERCENT, handle_pos, slider_bar_width);
    // 手柄绘制矩形根据手柄条上的位置移动：-左边缘把手矩形位于最小值和把手宽度之间。-右边缘把手矩形位于最大把手宽度和最大。-中间把手矩形位于中间
    const double shift_ratio { static_cast<double>(handle_pos) / MAX_PERCENT };
    return mouse_pos_x <= handle_pos_x + handle_rect_.width() * (1 - shift_ratio) && mouse_pos_x >= handle_pos_x - handle_rect_.width() * shift_ratio;
}

void DoubleSlider::mouseMoveEvent(QMouseEvent * event)
{
    if ((event->buttons() & Qt::LeftButton) == 0U) {
        moving_ = Handle::NONE;
        return;
    }

    const int new_position = GetMousePosition(event);
    const int min_x = GetHandlePosition(Handle::LEFT);
    const int max_x = GetHandlePosition(Handle::RIGHT);
    const bool on_min_handle = MouseIsOnHandle(event->x(), min_x);
    const bool on_max_handle = MouseIsOnHandle(event->x(), max_x);
    const bool on_middle_handle = (new_position > min_x) && (new_position < max_x);
    const double range { max_ - min_ };
    const double new_current = static_cast<double>(new_position) / MAX_PERCENT * range + min_;

    if (moving_ == Handle::LEFT || (moving_ == Handle::NONE && on_min_handle)) {
        SetCurrentMin(new_current);
        moving_ = Handle::LEFT;
    } else if (moving_ == Handle::RIGHT || (moving_ == Handle::NONE && on_max_handle)) {
        SetCurrentMax(new_current);
        moving_ = Handle::RIGHT;
    } else if (moving_ == Handle::NONE && on_middle_handle) {
        moving_ = Handle::Middle;
        current_middle_ = GetNormalizedValue(new_current);
        last_emitted_middle_ = current_middle_;
    } else if (moving_ == Handle::Middle) {
        current_middle_ = new_current;
        const double current_middle_change = current_middle_ - last_emitted_middle_;
        SetCurrentMax(current_max_ + current_middle_change);
        SetCurrentMin(current_min_ + current_middle_change);
        last_emitted_middle_ = current_middle_;
    }

    if (!DoublesAreEqual(last_emitted_min_, current_min_)) {
        last_emitted_min_ = current_min_;
        Q_EMIT SgnCurrentMinChanged(current_min_);
        const double ww = current_max_ - current_min_;
        Q_EMIT SgnCurrentValueChanged(current_min_ + ww * 0.5, ww);
    }
    if (!DoublesAreEqual(last_emitted_max_, current_max_)) {
        last_emitted_max_ = current_max_;
        Q_EMIT SgnCurrentMaxChanged(current_max_);
        const double ww = current_max_ - current_min_;
        Q_EMIT SgnCurrentValueChanged(current_max_ - ww * 0.5, ww);
    }
    this->update();
}

int DoubleSlider::GetHandlePosition(Handle handle) const
{
    const double range { max_ - min_ };
    const double current_value = (handle == Handle::LEFT ? current_min_ : current_max_) - min_;
    return static_cast<int>((std::round(current_value / range * MAX_PERCENT)));
}

void DoubleSlider::DrawSliderBarText(QPainter & painter) const
{
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 8));
    const QString str_min_value = QString("%1").arg(min_);
    const QString str_max_value = QString("%1").arg(max_);
    const double ww = current_max_ - current_min_;
    const double WL = current_min_ + ww * .5;
    const QString str_win_value = QString("WL/WW: %1/%2")
                                    .arg(QString::number(WL, 'f', 0))
                                    .arg(QString::number(ww, 'f', 0));
    const QFontMetrics metrics = painter.fontMetrics();
    const int mintext_width = metrics.width(str_min_value);
    const int maxtext_width = metrics.width(str_max_value);
    const int text_height = metrics.height();
    const int win_width = metrics.width(str_win_value);
    painter.drawText(QRectF(2, 0, mintext_width, text_height), str_min_value);
    painter.drawText(QRectF(width() - maxtext_width - 2, 0, maxtext_width, text_height), str_max_value);
    painter.drawText(QRectF(width() / 2 - win_width / 2, 0, win_width, text_height), str_win_value);
}

void DoubleSlider::DrawSliderBar(QPainter & painter) const
{
    QStyleOptionSlider barStyle;
    initStyleOption(&barStyle);
    const QRect inner_rect(barStyle.rect);
    const int bar_height = static_cast<int>(std::round(inner_rect.height() * BAR_HEIGHT_RATIO));
    const int bar_begin = static_cast<int>(inner_rect.height() * 0.5 - bar_height * 0.5);
    const QRect plain = QRect(inner_rect.x() + handle_rect_.width() / 4, bar_begin,
                              inner_rect.width() - handle_rect_.width() / 2, bar_height);
    qDrawPlainRect(&painter, plain, palette().color(QPalette::Dark));
}

int DoubleSlider::GetHandleMiddlePosX(int handle_pos, int handle_width, int span) const
{
    const int handle_pos_x = QStyle::sliderPositionFromValue(0, MAX_PERCENT, handle_pos, span);
    const double handle_shift_ratio { handle_pos / static_cast<double>(MAX_PERCENT) };
    const int handle_middle_x = handle_pos_x + static_cast<int>(std::round((1 - handle_shift_ratio) * handle_width / 2.));
    return handle_middle_x;
}

void DoubleSlider::SetHandleRect()
{
    QStyleOptionSlider default_style;
    initStyleOption(&default_style);
    handle_rect_ = style()->subControlRect(QStyle::CC_Slider, &default_style, QStyle::SC_SliderHandle, this);
}

void DoubleSlider::DrawSliderBarBetweenHandles(QPainter & painter) const
{
    QStyleOptionSlider default_style;
    initStyleOption(&default_style);
    const QRect inner_rect(default_style.rect);
    const int bar_height = static_cast<int>(std::round(inner_rect.height() * BAR_HEIGHT_RATIO));
    const int bar_begin = static_cast<int>(inner_rect.height() * 0.5 - bar_height * 0.5);
    const int left_handle_position = GetHandlePosition(Handle::LEFT);
    const int right_handle_position = GetHandlePosition(Handle::RIGHT);
    const int left_handle_middle_x = GetHandleMiddlePosX(left_handle_position, handle_rect_.width(), inner_rect.width() - inner_rect.x());
    const int right_handle_middle_x = GetHandleMiddlePosX(right_handle_position, handle_rect_.width(), inner_rect.width() - inner_rect.x());
    const QRect bar_rect = QRect(inner_rect.x() + left_handle_middle_x, bar_begin, inner_rect.x() + right_handle_middle_x - left_handle_middle_x, bar_height);
    const QBrush brush = palette().brush(QPalette::Dark);
    qDrawShadePanel(&painter, bar_rect, palette(), true, 1, &brush);
}

QStyleOptionSlider DoubleSlider::GetHandleStyle(bool moving, int position) const
{
    QStyleOptionSlider handleStyle;
    initStyleOption(&handleStyle);
    handleStyle.sliderPosition = position;
    if (!moving) {
        handleStyle.state = QStyle::State_Raised;
    }
    handleStyle.subControls = QStyle::SC_SliderHandle;
    return handleStyle;
}

void DoubleSlider::DrawHandles(QPainter & painter) const
{
    const QStyleOptionSlider left_style = GetHandleStyle(moving_ == Handle::LEFT, GetHandlePosition(Handle::LEFT));
    style()->drawComplexControl(QStyle::CC_Slider, &left_style, &painter, this);
    const QStyleOptionSlider right_style = GetHandleStyle(moving_ == Handle::RIGHT, GetHandlePosition(Handle::RIGHT));
    style()->drawComplexControl(QStyle::CC_Slider, &right_style, &painter, this);
}

void DoubleSlider::paintEvent(QPaintEvent * event)
{
    if (!DoublesAreEqual(min_, max_)) {
        QPainter painter(this);
        painter.setClipRegion(event->region());
        DrawSliderBar(painter);
        DrawSliderBarBetweenHandles(painter);
        DrawHandles(painter);
        DrawSliderBarText(painter);
    }
}
