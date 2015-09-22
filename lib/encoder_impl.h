/* -*- c++ -*- */
/* 
 * Copyright 2015 szabolor.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_AO40_ENCODER_IMPL_H
#define INCLUDED_AO40_ENCODER_IMPL_H

#include <ao40/encoder.h>

namespace gr {
  namespace ao40 {

    class encoder_impl : public encoder
    {
     private:
      pmt::pmt_t out_port;
      pmt::pmt_t in_port;
      uint8_t low_bit;
      uint8_t high_bit;
      bool bit_output;

     public:
      encoder_impl(bool bit_output, uint8_t low_bit, uint8_t high_bit);
      ~encoder_impl();
      void encode(pmt::pmt_t pdu);
    };

  } // namespace ao40
} // namespace gr

#endif /* INCLUDED_AO40_ENCODER_IMPL_H */

