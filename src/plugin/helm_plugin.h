/* Copyright 2013-2017 Matt Tytel
 *
 * helm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * helm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with helm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HELM_PLUGIN_H
#define HELM_PLUGIN_H

#include "JuceHeader.h"

#include "synth_base.h"
#include "value_bridge.h"

class ValueBridge;

class HelmPlugin : public SynthBase, public juce::AudioProcessor, public ValueBridge::Listener {
  public:
    HelmPlugin();
    virtual ~HelmPlugin();

    // SynthBase
    SynthGuiInterface* getGuiInterface() override;
    void beginChangeGesture(const std::string& name) override;
    void endChangeGesture(const std::string& name) override;
    void setValueNotifyHost(const std::string& name, mopo::mopo_float value) override;
    const juce::CriticalSection& getCriticalSection() override;

    // AudioProcessor
    void prepareToPlay(double sample_rate, int buffer_size) override;
    void releaseResources() override;
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    const juce::String getInputChannelName(int channel_index) const override;
    const juce::String getOutputChannelName(int channel_index) const override;
    bool isInputChannelStereoPair(int index) const override;
    bool isOutputChannelStereoPair(int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& new_name) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int size_in_bytes) override;

    // ValueBridge::Listener
    void parameterChanged(std::string name, mopo::mopo_float value) override;

    void loadPatches();

  private:
    juce::uint32 set_state_time_;

    int current_program_;
    juce::Array<juce::File> all_patches_;
    juce::AudioPlayHead::CurrentPositionInfo position_info_;

    std::map<std::string, ValueBridge*> bridge_lookup_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelmPlugin)
};

#endif // HELM_PLUGIN_H
