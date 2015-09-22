/* -*- c++ -*- */

#define AO40_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ao40_swig_doc.i"

%{
#include "ao40/encoder.h"
#include "ao40/decoder.h"
%}

%include "ao40/encoder.h"
GR_SWIG_BLOCK_MAGIC2(ao40, encoder);
%include "ao40/decoder.h"
GR_SWIG_BLOCK_MAGIC2(ao40, decoder);
