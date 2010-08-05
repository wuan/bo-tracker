#include "Samples.h"

namespace blitzortung {
  namespace data {

    Samples::Samples(const sample::Base::Creator& creator) :
      creator_(creator)
    {
      init();
    }

    void Samples::init() {
      entries_ = sample::Base::VP(new sample::Base::V());
    }


    void Samples::add(std::auto_ptr<sample::Base>){
    }

    void Samples::add(Samples&) throw(exception::Base) {
    }

    const gr::date& Samples::getDate() const {
      return date_;
    }

    void Samples::setDate(const gr::date& date) {
      date_ = date;
    }

    const sample::Base& Samples::front() const {
      return entries_->front();
    }

    sample::Base& Samples::front() {
      return entries_->front();
    }

    const sample::Base& Samples::back() const {
      return entries_->back();
    }

    sample::Base& Samples::back() {
      return entries_->back();
    }

    sample::Base::CVI Samples::begin() const {
      return entries_->begin();
    }

    sample::Base::VI Samples::begin() {
      return entries_->begin();
    }

    sample::Base::CVI Samples::end() const {
      return entries_->end();
    }

    sample::Base::VI Samples::end() {
      return entries_->end();
    }


  }
}
