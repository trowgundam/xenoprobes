/**
 * @file RunOptionsWidget.cpp
 *
 * @author Dan Keenan
 * @date 11/29/24
 * @copyright GNU GPLv3
 */

#include "RunOptionsWidget.h"

#include <QJsonObject>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>

RunOptionsWidget::RunOptionsWidget(QWidget *parent)
    : QWidget(parent), storageWeight_(new SliderWithValWidget(this)),
      revenueWeight_(new SliderWithValWidget(this)),
      productionWeight_(new SliderWithValWidget(this)),
      iterations_(new SliderWithValWidget(this)),
      population_(new SliderWithValWidget(this)),
      offsprings_(new SliderWithValWidget(this)),
      mutation_(new SliderWithValWidget(this)),
      threads_(new SliderWithValWidget(this)),
      oreMultiplier_(new DoubleSliderWithValWidget(this)),
      requireOres_(new QCheckBox(this))
{
  auto *layout = new QVBoxLayout(this);
  layout->addWidget(new QLabel(tr("Storage Weight:"), this));
  layout->addWidget(storageWeight_);
  storageWeight_->setMinimum(0);
  storageWeight_->setMaximum(1000);
  connect(storageWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Revenue Weight:"), this));
  layout->addWidget(revenueWeight_);
  revenueWeight_->setMinimum(0);
  revenueWeight_->setMaximum(1000);
  connect(revenueWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Production Weight:"), this));
  layout->addWidget(productionWeight_);
  productionWeight_->setMinimum(0);
  productionWeight_->setMaximum(1000);
  connect(productionWeight_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Ore Multiplier:"), this));
  layout->addWidget(oreMultiplier_);
  oreMultiplier_->setMinimum(0.0);
  oreMultiplier_->setMaximum(10);
  connect(oreMultiplier_, &DoubleSliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Require Ores:"), this));
  layout->addWidget(requireOres_);

  connect(requireOres_,
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    &QCheckBox::checkStateChanged,
#else
    &QCheckBox::stateChanged,
#endif
    this,
    &RunOptionsWidget::settingsChanged);

  layout->addWidget(new QLabel(tr("Iterations:"), this));
  layout->addWidget(iterations_);
  iterations_->setMinimum(1);
  iterations_->setMaximum(5000);
  connect(iterations_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Population:"), this));
  layout->addWidget(population_);
  population_->setMinimum(100);
  population_->setMaximum(500);
  connect(population_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Offspring:"), this));
  layout->addWidget(offsprings_);
  offsprings_->setMinimum(10);
  offsprings_->setMaximum(200);
  connect(offsprings_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("Mutation:"), this));
  layout->addWidget(mutation_);
  mutation_->setMinimum(1);
  mutation_->setMaximum(100);
  mutation_->spinBox()->setSuffix(tr("%"));
  connect(mutation_, &SliderWithValWidget::valueChanged, this,
          &RunOptionsWidget::settingsChanged);
  layout->addWidget(new QLabel(tr("CPU Threads:"), this));
  layout->addWidget(threads_);
  threads_->setMinimum(1);
  threads_->setMaximum(QThread::idealThreadCount() * 2);
  connect(threads_, &SliderWithValWidget::valueChanged, this,
      &RunOptionsWidget::settingsChanged);
  applyDefaultValues();

  auto applyDefaultsBtn = new QPushButton(tr("Restore Defaults"), this);
  layout->addWidget(applyDefaultsBtn);
  connect(applyDefaultsBtn, &QPushButton::clicked, this,
          &RunOptionsWidget::applyDefaultValues);

  // Match spinbox widths for aesthetics.
  const std::vector<QAbstractSpinBox*> allWidgets = {
    storageWeight_->spinBox(), revenueWeight_->spinBox(), productionWeight_->spinBox(), iterations_->spinBox(),
    population_->spinBox(), offsprings_->spinBox(), mutation_->spinBox(), threads_->spinBox(), oreMultiplier_->spinBox()
  };
  const auto maxWidth =
      std::ranges::max(allWidgets,
                       [](QAbstractSpinBox *lhs, QAbstractSpinBox *rhs) {
                         return lhs->width() <
                                rhs->width();
                       })
          ->width();
  for (auto *widget : allWidgets) {
    widget->setMinimumWidth(maxWidth);
  }
}

QJsonValue RunOptionsWidget::optionsToJson(const RunOptions &options) {
  QJsonObject json;
  json["storageWeight"] = options.storageWeight;
  json["revenueWeight"] = options.revenueWeight;
  json["productionWeight"] = options.productionWeight;
  json["oreMultiplier"] = options.oreMultiplier;
  json["requireOres"] = options.requireOres;
  json["iterations"] = options.iterations;
  json["population"] = options.population;
  json["offsprings"] = options.offsprings;
  json["mutation"] = options.mutation;
  json["threads"] = options.threads;

  return json;
}

RunOptions RunOptionsWidget::optionsFromJson(const QJsonValue &json) {
  if (!json.isObject()) {
    throw std::runtime_error("Bad options format.");
  }
  const auto &jsonObj = json.toObject();
  RunOptions options;

  if (!jsonObj.contains("storageWeight") ||
      !jsonObj["storageWeight"].isDouble()) {
    throw std::runtime_error("Bad storageWeight");
  }
  options.storageWeight = json["storageWeight"].toInt();

  if (!jsonObj.contains("revenueWeight") ||
      !jsonObj["revenueWeight"].isDouble()) {
    throw std::runtime_error("Bad revenueWeight");
  }
  options.revenueWeight = json["revenueWeight"].toInt();

  if (!jsonObj.contains("productionWeight") ||
      !jsonObj["productionWeight"].isDouble()) {
    throw std::runtime_error("Bad productionWeight");
  }
  options.productionWeight = json["productionWeight"].toInt();

  if (!jsonObj.contains("oreMultiplier") ||
    !jsonObj["oreMultiplier"].isDouble()) {
    throw std::runtime_error("Bad oreMultiplier");
  }
  options.oreMultiplier = json["oreMultiplier"].toDouble();

  if (!jsonObj.contains("requireOres") ||
    !jsonObj["requireOres"].isBool()) {
    throw std::runtime_error("Bad requireOres");
  }
  options.requireOres = json["requireOres"].toBool();

  if (!jsonObj.contains("iterations") || !jsonObj["iterations"].isDouble()) {
    throw std::runtime_error("Bad iterations");
  }
  options.iterations = json["iterations"].toInt();

  if (!jsonObj.contains("population") || !jsonObj["population"].isDouble()) {
    throw std::runtime_error("Bad population");
  }
  options.population = json["population"].toInt();

  if (!jsonObj.contains("offsprings") || !jsonObj["offsprings"].isDouble()) {
    throw std::runtime_error("Bad offsprings");
  }
  options.offsprings = json["offsprings"].toInt();

  if (!jsonObj.contains("mutation") || !jsonObj["mutation"].isDouble()) {
    throw std::runtime_error("Bad mutation");
  }
  options.mutation = json["mutation"].toInt();

  if  (!jsonObj.contains("threads") || !jsonObj["threads"].isDouble()) {
    throw std::runtime_error("Bad threads");
  }
  options.threads = json["threads"].toInt();

  return options;
}

void RunOptionsWidget::applyDefaultValues() {
  const RunOptions defaults;
  setStorageWeight(defaults.storageWeight);
  setRevenueWeight(defaults.revenueWeight);
  setProductionWeight(defaults.productionWeight);
  setOreMultiplier(defaults.oreMultiplier);
  setRequireOres(defaults.requireOres);
  setIterations(defaults.iterations);
  setPopulation(defaults.population);
  setOffsprings(defaults.offsprings);
  setMutation(defaults.mutation);
  setThreads(defaults.threads);
}
