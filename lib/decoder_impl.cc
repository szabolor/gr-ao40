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
#include "decoder_impl.h"

extern "C" {
  #include "decode/dec_ref.h"
}


namespace gr {
  namespace ao40 {

    decoder::sptr decoder::make()
    {
      return gnuradio::get_initial_sptr (new decoder_impl());
    }

    decoder_impl::decoder_impl()
      : gr::block(
          "decoder",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(0, 0, 0)
        ),
      out_port(pmt::mp("data_out")),
      in_port(pmt::mp("encoded_in"))
    {
      message_port_register_out(out_port);
      message_port_register_in(in_port);
      set_msg_handler(in_port, boost::bind(&decoder_impl::decode, this, _1));
    }

    decoder_impl::~decoder_impl()
    {
    }

    void decoder_impl::decode(pmt::pmt_t pdu)
    {
      pmt::pmt_t meta = pmt::car(pdu);
      pmt::pmt_t vector = pmt::cdr(pdu);

      size_t INPUT_SIZE = 5200;

      // Input data must have header `("ao40", "encoded")` and length of 650 byte
      if (pmt::is_dict(meta) && 
          dict_has_key(meta, pmt::intern("ao40")) && 
          pmt::eqv(pmt::dict_ref(meta, pmt::intern("ao40"), pmt::PMT_NIL), pmt::intern("encoded")) &&
          pmt::length(vector) == INPUT_SIZE
      ) {        
        const uint8_t *encoded_ptr;
        uint8_t encoded[5200];
        uint8_t data[256];
        int8_t  error[2];
        pmt::pmt_t msg;

        encoded_ptr = pmt::u8vector_elements(vector, INPUT_SIZE);

        // TODO: learn C++; How can `uint8_t* -> uint8_t *[256]` conversation be done here?
        for (int i=0; i<INPUT_SIZE; ++i)
          encoded[i] = encoded_ptr[i];

        decode_data(&encoded, &data, &error);



        // Resemble PDU of decoded data
        meta = pmt::dict_delete(meta, pmt::intern("ao40"));
        meta = pmt::dict_add(meta, pmt::intern("ao40"), pmt::intern("data"));
        vector = pmt::init_u8vector(256, data);
        msg = pmt::cons(meta, vector);
        message_port_pub(out_port, msg);

        // Resemble PDU of the RS error value of decoded data
        meta = pmt::dict_delete(meta, pmt::intern("ao40"));
        meta = pmt::dict_add(meta, pmt::intern("ao40"), pmt::intern("error"));
        vector = pmt::init_s8vector(2, error);
        msg = pmt::cons(meta, vector);
        message_port_pub(out_port, msg);
      }
    }
  } /* namespace ao40 */
} /* namespace gr */

