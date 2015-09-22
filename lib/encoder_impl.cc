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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "encoder_impl.h"

extern "C" {
  #include "encode/enc_ref.h"
}

namespace gr {
  namespace ao40 {

    encoder::sptr encoder::make(bool bit_output, uint8_t low_bit, uint8_t high_bit)
    {
      return gnuradio::get_initial_sptr (new encoder_impl(bit_output, low_bit, high_bit));
    }

    // TODO: output byte format configurable: bit/byte or byte/byte
    encoder_impl::encoder_impl(bool bit_output, uint8_t low_bit, uint8_t high_bit)
      : gr::block(
          "encoder",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(0, 0, 0)
        ),
      out_port(pmt::mp("encoded_out")),
      in_port(pmt::mp("data_in")),
      bit_output(bit_output),
      low_bit(low_bit),
      high_bit(high_bit)
    {
      message_port_register_out(out_port);
      message_port_register_in(in_port);
      set_msg_handler(in_port, boost::bind(&encoder_impl::encode, this, _1));
    }

    encoder_impl::~encoder_impl()
    {
    }

    void encoder_impl::encode(pmt::pmt_t pdu)
    {
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t vector = pmt::cdr(pdu);

      size_t INPUT_SIZE = 256;

      // Input data must have header `("ao40", "data")` and length of 256 byte
      if (pmt::is_dict(meta) && 
          dict_has_key(meta, pmt::intern("ao40")) && 
          pmt::eqv(pmt::dict_ref(meta, pmt::intern("ao40"), pmt::PMT_NIL), pmt::intern("data")) &&
          pmt::length(vector) == INPUT_SIZE
      ) {
        // Mark PDU as encoded data
        meta = pmt::dict_delete(meta, pmt::intern("ao40"));
        meta = pmt::dict_add(meta, pmt::intern("ao40"), pmt::intern("encoded"));
        
        const uint8_t *data_ptr;
        uint8_t data[256];
        uint8_t encoded[650];

        data_ptr = pmt::u8vector_elements(vector, INPUT_SIZE);

        // TODO: learn C++; How can `uint8_t* -> uint8_t *[256]` conversation be done here?
        for (int i=0; i<INPUT_SIZE; ++i)
          data[i] = data_ptr[i];

        // Do the encoding!
        encode_data(&data, &encoded);

        // Pack the return value
        if (bit_output) {
          // Bit MSB mode
          uint8_t bit_encoded[5200];

          for (int i = 0; i < 5200; ++i)
            bit_encoded[i] = ( (encoded[i >> 3] & (1 << (7 - (i & 7)))) != 0 ) ? high_bit : low_bit;

          vector = pmt::init_u8vector(5200, bit_encoded);
        } else {
          // Byte mode
          vector = pmt::init_u8vector(650, encoded);
        }

        
        // Resemble PDU with its metadata and content
        pmt::pmt_t msg = pmt::cons(meta, vector);
        message_port_pub(out_port, msg);
      }
    }

  } /* namespace ao40 */
} /* namespace gr */

