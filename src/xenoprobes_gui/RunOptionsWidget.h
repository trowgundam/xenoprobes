/**
 * @file RunOptionsWidget.h
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#ifndef RUNOPTIONSWIDGET_H
#define RUNOPTIONSWIDGET_H

#include <QCheckBox>

#include <QJsonValue>
#include <QWidget>

#include "DoubleSliderWithValWidget.h"
#include "RunOptions.h"
#include "SliderWithValWidget.h"

class RunOptionsWidget : public QWidget {
  Q_OBJECT
public:

  [[nodiscard]] static QJsonValue optionsToJson(const RunOptions &options);
  static RunOptions optionsFromJson(const QJsonValue &json);

  explicit RunOptionsWidget(QWidget *parent = nullptr);
  [[nodiscard]] RunOptions options() const {
    return {
        .storageWeight = storageWeight(),
        .revenueWeight = revenueWeight(),
        .productionWeight = productionWeight(),
        .oreMultiplier = oreMultiplier(),
        .requireOres = requireOres(),
        .iterations = iterations(),
        .population = population(),
        .offsprings = offsprings(),
        .mutation = mutation(),
        .threads = threads(),
    };
  }
  void setOptions(const RunOptions &options) {
    setStorageWeight(options.storageWeight);
    setRevenueWeight(options.revenueWeight);
    setProductionWeight(options.productionWeight);
    setOreMultiplier(options.oreMultiplier);
    setRequireOres(options.requireOres);
    setIterations(options.iterations);
    setPopulation(options.population);
    setOffsprings(options.offsprings);
    setMutation(options.mutation);
    setThreads(options.threads);
  }
  [[nodiscard]] int storageWeight() const { return storageWeight_->value(); }
  void setStorageWeight(int storageWeight) {
    storageWeight_->setValue(storageWeight);
  }
  [[nodiscard]] int revenueWeight() const { return revenueWeight_->value(); }
  void setRevenueWeight(int revenueWeight) {
    revenueWeight_->setValue(revenueWeight);
  }
  [[nodiscard]] int productionWeight() const {
    return productionWeight_->value();
  }
  void setProductionWeight(int productionWeight) {
    productionWeight_->setValue(productionWeight);
  }
  [[nodiscard]] double oreMultiplier() const {
    return oreMultiplier_->value();
  }
  void setOreMultiplier(double oreMultiplier) {
    oreMultiplier_->setValue(oreMultiplier);
  }
  [[nodiscard]] bool requireOres() const {
    return requireOres_->checkState() == Qt::Checked;
  }
  void setRequireOres(bool requireOres) {
    requireOres_->setCheckState(requireOres ? Qt::Checked : Qt::Unchecked);
  }
  [[nodiscard]] int iterations() const { return iterations_->value(); }
  void setIterations(int iterations) { iterations_->setValue(iterations); }
  [[nodiscard]] int population() const { return population_->value(); }
  void setPopulation(int population) { population_->setValue(population); }
  [[nodiscard]] int offsprings() const { return offsprings_->value(); }
  void setOffsprings(int offsprings) { offsprings_->setValue(offsprings); }
  [[nodiscard]] int mutation() const { return mutation_->value(); }
  void setMutation(int mutation) { mutation_->setValue(mutation); }
  [[nodiscard]] int threads() const { return threads_->value(); }
  void setThreads(int threads) { threads_->setValue(threads); }

Q_SIGNALS:
  void settingsChanged();

public Q_SLOTS:
  void applyDefaultValues();

private:
  SliderWithValWidget *storageWeight_;
  SliderWithValWidget *revenueWeight_;
  SliderWithValWidget *productionWeight_;
  SliderWithValWidget *iterations_;
  SliderWithValWidget *population_;
  SliderWithValWidget *offsprings_;
  SliderWithValWidget *mutation_;
  SliderWithValWidget *threads_;
  DoubleSliderWithValWidget *oreMultiplier_;
  QCheckBox *requireOres_;
};

#endif // RUNOPTIONSWIDGET_H
