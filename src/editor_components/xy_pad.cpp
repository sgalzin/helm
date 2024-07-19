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

#include "xy_pad.h"

#include "colors.h"
#include "utils.h"

#define GRID_CELL_WIDTH 8
#define GRID_CELL_HEIGHT 16

XYPad::XYPad() {
  mouse_down_ = false;
  x_slider_ = nullptr;
  y_slider_ = nullptr;
  active_ = true;
  setOpaque(true);
}

XYPad::~XYPad() { }

void XYPad::paintBackground(juce::Graphics& g) {
  g.fillAll(juce::Colour(0xff424242));

  g.setColour(juce::Colour(0xff4a4a4a));
  for (int x = 0; x < getWidth(); x += GRID_CELL_WIDTH)
    g.drawLine(x, 0, x, getHeight());
  for (int y = 0; y < getHeight(); y += GRID_CELL_WIDTH)
    g.drawLine(0, y, getWidth(), y);
}

void XYPad::paint(juce::Graphics& g) {
  static const juce::DropShadow shadow(juce::Colour(0xbb000000), 5, juce::Point<int>(0, 0));

  g.drawImage(background_,
              0, 0, getWidth(), getHeight(),
              0, 0, background_.getWidth(), background_.getHeight());

  float x = x_slider_->getValue() * getWidth();
  float y = (1.0f - y_slider_->getValue()) * getHeight();

  juce::Path target;
  float handle_radius = 0.05f * getWidth();
  target.addEllipse(x - handle_radius, y - handle_radius,
                    2.0f * handle_radius, 2.0f * handle_radius);

  shadow.drawForPath(g, target);

  g.setColour(Colors::graph_fill);
  g.fillPath(target);

  if (active_)
    g.setColour(Colors::audio);
  else
    g.setColour(Colors::graph_disable);

  juce::PathStrokeType stroke(0.01f * getWidth(), juce::PathStrokeType::beveled, juce::PathStrokeType::rounded);
  g.strokePath(target, stroke);
  float dot_radius = 0.01f * getWidth();

  g.fillEllipse(x - dot_radius, y - dot_radius, 2.0f * dot_radius, 2.0f * dot_radius);

  if (mouse_down_) {
    g.setColour(juce::Colour(0x11ffffff));
    float hover_radius = 0.2 * getWidth();
    g.fillEllipse(x - hover_radius, y - hover_radius, 2.0f * hover_radius, 2.0f * hover_radius);
  }
}

void XYPad::resized() {
  const juce::Displays::Display& display = juce::Desktop::getInstance().getDisplays().getMainDisplay();
  float scale = display.scale;
  background_ = juce::Image(juce::Image::RGB, scale * getWidth(), scale * getHeight(), true);
  juce::Graphics g(background_);
  g.addTransform(juce::AffineTransform::scale(scale, scale));
  paintBackground(g);
}

void XYPad::mouseDown(const juce::MouseEvent& e) {
  setSlidersFromPosition(e.getPosition());
  mouse_down_ = true;
  repaint();
}

void XYPad::mouseDrag(const juce::MouseEvent& e) {
  setSlidersFromPosition(e.getPosition());
}

void XYPad::mouseUp(const juce::MouseEvent& e) {
  mouse_down_ = false;
  repaint();
}

void XYPad::setSlidersFromPosition(juce::Point<int> position) {
  if (x_slider_) {
    double percent = mopo::utils::clamp((1.0 * position.x) / getWidth(), 0.0, 1.0);
    x_slider_->setValue(percent);
  }
  if (y_slider_) {
    double percent = mopo::utils::clamp(1.0 - (1.0 * position.y) / getHeight(), 0.0, 1.0);
    y_slider_->setValue(percent);
  }
}

void XYPad::guiChanged(SynthSlider* moved_slider) {
  repaint();
}

void XYPad::setXSlider(SynthSlider* slider) {
  x_slider_ = slider;
  x_slider_->addSliderListener(this);
  repaint();
}

void XYPad::setYSlider(SynthSlider* slider) {
  y_slider_ = slider;
  y_slider_->addSliderListener(this);
  repaint();
}

void XYPad::setActive(bool active) {
  active_ = active;
  repaint();
}
