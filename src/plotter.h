#pragma once

#include <Arduino.h>

// Emits named numeric signals in the format understood by Arduino Serial Plotter:
// We can 'subscribe" to values and then use the arduino ide to use the serial plotter to see values!
class Plotter {
  public:
    static constexpr size_t MAX_SERIES = 12;

    explicit Plotter(Stream& output = Serial, uint32_t periodMs = 20)
        : output_(output), periodMs_(periodMs), seriesCount_(0), lastOutputMs_(0) {}

    bool subscribe(const char* name, const float& value) {
        if (seriesCount_ >= MAX_SERIES || name == nullptr) {
            return false;
        }

        series_[seriesCount_++] = {name, &value};
        return true;
    }

    // This update function is what we will call perodically, (~20ms) 
    void update() {
        const uint32_t now = millis();
        if (seriesCount_ == 0 || (now - lastOutputMs_) < periodMs_) {
            return;
        }

        lastOutputMs_ = now;
        for (size_t i = 0; i < seriesCount_; ++i) {
            output_.print(series_[i].name);
            output_.print(':');
            output_.print(*series_[i].value, 4);
            if (i + 1 < seriesCount_) {
                output_.print('\t');
            }
        }
        output_.println();
    }

    void setPeriodMs(uint32_t periodMs) { periodMs_ = periodMs; }

  private:
    struct Series {
        const char* name;
        const float* value;
    };

    Stream& output_;
    uint32_t periodMs_;
    size_t seriesCount_;
    uint32_t lastOutputMs_;
    Series series_[MAX_SERIES];
};
