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


#ifndef INCLUDED_AO40_DECODER_H
#define INCLUDED_AO40_DECODER_H

#include <ao40/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ao40 {

    /*!
     * \brief <+description of block+>
     * \ingroup ao40
     *
     */
    class AO40_API decoder : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<decoder> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ao40::decoder.
       *
       * To avoid accidental use of raw pointers, ao40::decoder's
       * constructor is in a private implementation
       * class. ao40::decoder::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ao40
} // namespace gr

#endif /* INCLUDED_AO40_DECODER_H */

