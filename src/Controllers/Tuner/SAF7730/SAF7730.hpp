#ifndef FMDX_TUNER_SAF7730_H
#define FMDX_TUNER_SAF7730_H

#include "../TunerDriver.hpp"
#include "../../../Utils/Timer.hpp"
#include "I2cDirana.hpp"
#include "TEF6730.hpp"
#include "Filter.hpp"
#include "Dirana.hpp"

class SAF7730 : public TunerDriver
{
public:
    void setup() override;
    bool start() override;
    void shutdown() override;
    void hello() override;
    void loop() override;

    void resetQuality() override;
    void resetRds() override;

    bool setMode(Mode value) override;
    bool setFrequency(uint32_t value, TuneFlags flags) override;
    bool setBandwidth(uint32_t value) override;
    bool setDeemphasis(uint32_t value) override;
    bool setAgc(uint32_t value) override;
    bool setAlignment(uint32_t value) override;
    bool setVolume(uint8_t value) override;
    bool setOutputMode(OutputMode value) override;
    bool setCustom(const char *name, const char *value) override;

    bool getQuality() override;
    int16_t getQualityRssi(QualityMode mode) override;
    int16_t getQualityCci(QualityMode mode) override;
    int16_t getQualityAci(QualityMode mode) override;
    int16_t getQualityModulation(QualityMode mode) override;
    int16_t getQualityOffset(QualityMode mode) override;
    int16_t getQualityBandwidth(QualityMode mode) override;
    bool getQualityStereo(QualityMode mode) override;
    
    const char* getName() override;

private:
    void readRds();

    I2cDirana i2c{DIRANA_DSP_I2C, DIRANA_IF_I2C};
    TEF6730 tef6730{i2c};
    Filter filter{i2c};
    bool qualityReady = false;
    bool forceFastDetector = false;
    int16_t lastRssi = 0;
    int16_t lastAciRaw = -1;

    Timer timerQualityAvailable;
    Timer timerQualityReset;
};

#endif
