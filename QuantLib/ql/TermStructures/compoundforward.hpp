
/*
 Copyright (C) 2002, 2003 Decillion Pty(Ltd).

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file compoundforward.hpp
    \brief compounded forward term structure
*/

#ifndef quantlib_compoundforward_curve_h
#define quantlib_compoundforward_curve_h
 
#include <ql/termstructure.hpp>
#include <ql/Math/loglinearinterpolation.hpp>

namespace QuantLib {

    class CompoundForward : public ForwardRateStructure,
                            public Observer {
      public:
        // constructor
        CompoundForward(const Date & todaysDate,
                        const Date & referenceDate,
                        const std::vector<Date> &dates,
                        const std::vector<Rate> &forwards,
                        const Calendar & calendar,
                        const BusinessDayConvention conv,
                        const Integer compounding,
                        const DayCounter & dayCounter);
        Date todaysDate() const { return todaysDate_; }
        Date referenceDate() const { return referenceDate_; };
        Calendar calendar() const { return calendar_; };
        BusinessDayConvention businessDayConvention() const {
            return conv_; };
        DayCounter dayCounter() const { return dayCounter_; };
        Integer compounding() const { return compounding_; };
        Date maxDate() const;
        Time maxTime() const;
        const std::vector<Time>& times() const;
        const std::vector<Date>& dates() const;
        const std::vector<Rate>& forwards() const;
        boost::shared_ptr<TermStructure> discountCurve() const;
        //! \name Observer interface
        //@{
        void update();
        //@}
      protected:
        // methods
        void calibrateNodes() const;
        boost::shared_ptr<TermStructure> bootstrap() const;
        Rate zeroYieldImpl(Time) const;
        DiscountFactor discountImpl(Time) const;
        Size referenceNode(Time) const;
        Rate forwardImpl(Time) const;
        Rate compoundForwardImpl(Time, Integer) const;
      private:
        // data members
        Date todaysDate_;
        Date referenceDate_;
        DayCounter dayCounter_;
        Calendar calendar_;
        BusinessDayConvention conv_;
        Integer compounding_;
        mutable bool needsBootstrap_;
        mutable std::vector<Date> dates_;
        mutable std::vector<Rate> forwards_;
        mutable std::vector<Time> times_;
        mutable Interpolation fwdinterp_;
        mutable boost::shared_ptr<TermStructure> discountCurve_;
    };

    // inline definitions

    inline Date CompoundForward::maxDate() const {
        return dates_.back();
    }

    inline Time CompoundForward::maxTime() const {
        return times_.back();
    }

    inline const std::vector<Time>& CompoundForward::times() const {
        return times_;
    }

    inline const std::vector<Date>& CompoundForward::dates() const {
        return dates_;
    }

    inline const std::vector<Rate>& CompoundForward::forwards() const {
        return forwards_;
    }

    inline void CompoundForward::update() {
        needsBootstrap_ = true;
        notifyObservers();
    }

}


#endif
