/**
 * @file DoubleSliderWithValWidget.cpp
 *
 * @author Minneyar
 * @date 03/31/2025
 * @copyright GNU GPLv3
 */

#include "DoubleSliderWithValWidget.h"

#include <QHBoxLayout>

DoubleSliderWithValWidget::DoubleSliderWithValWidget(QWidget *parent)
: QWidget(parent)
, slider_(new QSlider(Qt::Horizontal, this))
, spinBox_(new QDoubleSpinBox(this))
{
  auto *layout = new QHBoxLayout(this);
  layout->addWidget(slider_);
  layout->addWidget(spinBox_);
  connect(slider_, &QSlider::valueChanged, this,
          &DoubleSliderWithValWidget::handleQSliderChange);
  connect(spinBox_, &QDoubleSpinBox::valueChanged, this,
          &DoubleSliderWithValWidget::handleQDoubleSpinBoxChange);
}

void DoubleSliderWithValWidget::handleQSliderChange(int value) {
  auto scaledValue = static_cast<double>(value) / SCALING_FACTOR;
  if (std::fabs(scaledValue - spinBox_->value()) > 0.001) {
    spinBox_->setValue(scaledValue);
  }
  Q_EMIT(valueChanged(scaledValue));
}

void DoubleSliderWithValWidget::handleQDoubleSpinBoxChange(double value) {
  auto scaledValue = static_cast<int>(value * SCALING_FACTOR);
  if (scaledValue != slider_->value()) {
    slider_->setValue(scaledValue);
  }
  Q_EMIT(valueChanged(value));
}
void DoubleSliderWithValWidget::setMinimum(double minimum) {
  slider_->setMinimum(static_cast<int>(minimum * SCALING_FACTOR));
  spinBox_->setMinimum(minimum);
}

void DoubleSliderWithValWidget::setMaximum(double maximum) {
  slider_->setMaximum(static_cast<int>(maximum * SCALING_FACTOR));
  spinBox_->setMaximum(maximum);
}

void DoubleSliderWithValWidget::setValue(double value) {
  slider_->setValue(static_cast<int>(value * SCALING_FACTOR));
  spinBox_->setValue(value);
}
