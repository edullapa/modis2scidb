/*
  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

  This file is part of modis2scidb.

  modis2scidb is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  modis2scidb is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with modis2scidb. See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

/*!
  \file modis2scidb/exception.hpp

  \brief Base exception class for modis2scidb.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

#ifndef __MODIS2SCIDB_EXCEPTION_HPP__
#define __MODIS2SCIDB_EXCEPTION_HPP__

// STL
#include <stdexcept>
#include <string>

// Boost
#include <boost/exception/all.hpp>

namespace modis2scidb
{
  //! Base exception class for MODIS to SciDB.
  struct exception: virtual std::exception, virtual boost::exception { };

  //! An exception occurried during a parse action.
  struct parser_error: virtual exception { };

  //! An exception indicating an invalid tile range.
  struct tile_range_error: virtual exception { };

  //! An exception indicating an invalid argument value.
  struct invalid_arg_value: virtual exception { };

  //! An exception indicating an invalid directory.
  struct invalid_dir_error: virtual exception { };

  //! An exception indicating a error due to file handling.
  struct file_error: virtual exception { };

  //! An exception indicatin an error reported by GDAL.
  struct gdal_error: virtual exception { };
  
  struct conversion_error: virtual exception { };

  //! The base type for error report messages.
  typedef boost::error_info<struct tag_error_description, std::string> error_description;
}

#endif // __MODIS2SCIDB_EXCEPTION_HPP__
