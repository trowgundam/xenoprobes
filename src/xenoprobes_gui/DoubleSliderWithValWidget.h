/**
 * @file DoubleSliderWithValWidget.h
 *
 * @author Minneyar
 * @date 3/31/2025
 * @copyright GNU GPLv3
 */

#ifndef DOUBLESLIDERWITHVALWIDGET_H
#define DOUBLESLIDERWITHVALWIDGET_H

#include <QSlider>
#include <QDoubleSpinBox>
#include <QWidget>

/**
 * A QSlider with a matching QDoubleSpinBox.
 * QSlider only supports int values and there is no equivalent floating point version, so this scales the value stored
 * in the QDoubleSpinBox up by 1000 so it can be stored in an int with three decimals of precision.
 */
class DoubleSliderWithValWidget : public QWidget {
  Q_OBJECT
public:
  explicit DoubleSliderWithValWidget(QWidget *parent = nullptr);

  [[nodiscard]] QSlider *slider() { return slider_; }
  [[nodiscard]] QDoubleSpinBox *spinBox() { return spinBox_; }
  void setMinimum(double minimum);
  void setMaximum(double maximum);
  void setValue(double value);
  [[nodiscard]] double value() const { return spinBox_->value(); }

  Q_SIGNALS:
    void valueChanged(double value);

private:
  QSlider *slider_;
  QDoubleSpinBox *spinBox_;

  void handleQSliderChange(int value);
  void handleQDoubleSpinBoxChange(double value);

  static constexpr double SCALING_FACTOR = 1000.0;
};

#endif // DOUBLESLIDERWITHVALWIDGET_H
