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
  \file modis2scidb/types.hpp

  \brief General data types used in MODIS to SciDB.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

#ifndef __MODIS2SCIDB_TYPES_HPP__
#define __MODIS2SCIDB_TYPES_HPP__

// Modis2SciDB
#include "config.hpp"
#include "defines.hpp"

// STL
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace modis2scidb
{
  //! A class for keeping a decomposed MODIS file name.
  /*!
    We support definitions like the ones showned in the following sites:
    <ul>
      <li>http://modis-atmos.gsfc.nasa.gov/MOD06_L2/filename.html</li>
      <li>http://modis-atmos.gsfc.nasa.gov/products_filename.html</li>
      <li>https://lpdaac.usgs.gov/products/modis_overview</li>
    </ul>

    MOD = data collected from the Terra (AM overpass)
    MYD = data collected from the Aqua (PM overpass)

    Example: MOD06_L2.AYYYYDDD.h08v05.VVV.YYYYDDDHHMMSS.hdf

    MOD06_L2      => Earth Science Data Type Name => Product Short Name
    AYYYYDDD      => Date Year and Ju\lian Date (Julian Date of Acquisition)
    h08v05        => Tile Identifier (horizontalXXverticalYY)
    VVV           => Collection Version
    YYYYDDDHHMMSS => Production Date & Time (Julian Date of Production)
   */
  struct modis_file_descriptor
  {
    std::string data_type_name;       //!< Earth Science Data Type Name (product short name).
    std::string acquisition_date;     //!< Date year and julian date (julian date of acquisition).
    std::string tile;                 //!< Tile identifier.
    std::string collection_version;   //!< Collection version.
    std::string production_date;      //!< Production Date & Time (julian date of production).
  };

//  //! The name and alias used for a spectral data band.
//  struct band_info
//  {
//    std::string alias;  //! Band alias.
//    uint16_t number;    //! Band number.
//  };

} // end namespace modis2scidb

#endif  // __MODIS2SCIDB_TYPES_HPP__
