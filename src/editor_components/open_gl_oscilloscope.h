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

#pragma once
#ifndef OPEN_GL_OSCILLOSCOPE_H
#define OPEN_GL_OSCILLOSCOPE_H

#include "JuceHeader.h"

#include "memory.h"
#include "open_gl_component.h"

class OpenGLOscilloscope : public OpenGLComponent {
  public:
    OpenGLOscilloscope();
    virtual ~OpenGLOscilloscope();

    void setOutputMemory(const float* memory) { output_memory_ = memory; }

    void init(juce::OpenGLContext& open_gl_context) override;
    void render(juce::OpenGLContext& open_gl_context, bool animate = true) override;
    void destroy(juce::OpenGLContext& open_gl_context) override;
    void paintBackground(juce::Graphics& g) override;

  private:
    void drawLines(juce::OpenGLContext& open_gl_context);

    juce::ScopedPointer<juce::OpenGLShaderProgram> shader_;
    juce::ScopedPointer<juce::OpenGLShaderProgram::Attribute> position_;

    const float* output_memory_;
    float* line_data_;
    int* line_indices_;
    GLuint line_buffer_;
    GLuint line_indices_buffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OpenGLOscilloscope)
};

#endif // OPEN_GL_OSCILLOSCOPE_H
