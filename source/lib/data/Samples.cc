#include "Samples.h"

namespace blitzortung {
  namespace data {

    Samples::Samples(const Sample::Creator& creator) :
      creator_(creator)
    {
      init();
    }

    void Samples::init() {
      samples_ = Sample::VP(new Sample::V());
    }


    void Samples::add(std::auto_ptr<sample::Base>&) {
    }

    void Samples::add(sample::Base::V::auto_type sample) {
      samples_->push_back(sample.release());
    }

    void Samples::add(Samples&) throw(exception::Base) {
    }

    const gr::date& Samples::getDate() const {
      return date_;
    }

    void Samples::setDate(const gr::date& date) {
      date_ = date;
    }

    int Samples::size() const {
      return samples_->size();
    }

    void Samples::sort() {
      samples_->sort();
    }

    void Samples::clear() {
      samples_->clear();
    }

    Samples::Sample::VI Samples::erase(Sample::VI start, Sample::VI end) {
      return samples_->erase(start, end);
    }

    const sample::Base& Samples::front() const {
      return samples_->front();
    }

    sample::Base& Samples::front() {
      return samples_->front();
    }

    const sample::Base& Samples::back() const {
      return samples_->back();
    }

    sample::Base& Samples::back() {
      return samples_->back();
    }

    Samples::Sample::CVI Samples::begin() const {
      return samples_->begin();
    }

    Samples::Sample::VI Samples::begin() {
      return samples_->begin();
    }

    Samples::Sample::CVI Samples::end() const {
      return samples_->end();
    }

    Samples::Sample::VI Samples::end() {
      return samples_->end();
    }

  }
}
