#include <array>
#include <cinttypes>

#include "volta/volta.h"

// AudioSamples is basically a stack allocated Circular Array for collecting
// signed 16-bit samples from audio samples from a Microphone via interrupt
// service routines
template <size_t Duration = 200UL,   // 200 milli-seconds
          size_t SampleRate = 16UL,  // 16kHz
          size_t Channels = 1UL      // Mono
          >
class AudioSamples {
 public:
  using sample_type = int16_t;
  static constexpr auto size_ = SampleRate * Duration * Channels;

  // I use constexpr functions to make it comprehensibile. Though,
  // this will be zero-cost when using optimization
  constexpr auto sample_rate() { return SampleRate; }
  constexpr auto channels() { return Channels; }
  constexpr auto duration() { return Duration; }
  constexpr auto size() { return size_; }

  // array is left uninitialized
  constexpr AudioSamples() : pos_{0} {}

  // NOTE: begin and end pointers never change, irregardless of optimizations as
  // it is already allocated on the stack
  constexpr sample_type* begin() { return audio_samples_.begin(); }
  constexpr sample_type* end() { return audio_samples_.end(); }

  // NOTE: It is ok to overrun
  void volatile_push(sample_type sample) {
    // NOTE: we never do any direct read nor write

    // fetch address of position counter, doesn't change
    auto start_pos = volta::volatile_read(&pos_);
    // put data to the position
    // write in a volatile manner
    volta::volatile_write(begin() + start_pos, sample);
    // increment position counter and reset to start if need be
    // write in a volatile manner
    volta::volatile_write(&pos_, (start_pos + 1UL) % size());
  }

  // NOTE: It is ok to overrun
  // normal, non-volatile push
  void push(sample_type sample) {
    // normal sample push
    audio_samples_[pos_] = sample;
    // normal position increment
    pos_ = (pos_ + 1UL) % size();
  }

  // normal position
  size_t& pos() { return pos_; }

  // read position in a volatile manner
  size_t volatile_pos() { return volta::volatile_read(&pos_); }

 private:
  // array used to store audio samples
  std::array<sample_type, size_> audio_samples_;
  // indicates position of data insertion
  size_t pos_;
};

// holds audio samples
AudioSamples samples = {};

void AudioSampleISR_Auto(int16_t sample) {
  // NOTE: here, we never do any direct read nor write in ISRs
  samples.volatile_push(sample);
}

void AudioSampleISR_Manual(int16_t sample) {
  // NOTE: here, we never do any direct read nor write in ISRs

  // NOTE: It is ok to overrun

  // fetch address of position counter, doesn't change
  auto pos_ptr = &samples.pos();
  // read in a volatle manner
  auto pos = volta::volatile_read(pos_ptr);
  // put data to the position
  // write in a volatile manner
  volta::volatile_write(samples.begin() + pos, sample);
  // increment position counter and reset to start if need be
  // write in a volatile manner
  volta::volatile_write(pos_ptr, (pos + 1) % samples.size());
}

void SetupISRs() {
  // beep beep boop
}

void ProcessMonoFrame(int16_t frame) {
  // beep beep boop
  (void)frame;
}

int main() {
  SetupISRs();
  while (true) {
    // process data stream if need be, use volatile_read and volatile_write
    // perform desired op, though, note that the data can be overrun
    for (auto iter = samples.begin(); iter < samples.end(); iter++) {
      auto mono_frame = volta::volatile_read(iter);
      ProcessMonoFrame(mono_frame);
    }
  }
}