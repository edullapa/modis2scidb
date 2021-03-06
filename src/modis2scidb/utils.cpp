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
  \file modis2scidb/utils.cpp

  \brief Utility functions for dealing with MODIS data.

  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>

  \date 2014-2014

  \copyright GNU General Public License version 3
 */

// Modis2SciDB
#include "utils.hpp"

// Boost
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

// GDAL
#include <gdal_priv.h>

std::vector<uint16_t>
modis2scidb::parse_bands(const std::string& str)
{
  std::vector<uint16_t> result;

  std::vector<std::string> bands_str;

  boost::split(bands_str, str, boost::is_any_of(","));

  for(std::size_t i = 0; i != bands_str.size(); ++i)
  {
    result.push_back(boost::lexical_cast<uint16_t>(bands_str[i]));
  }

  return result;
}

//std::vector<modis2scidb::band_info>
//modis2scidb::parse_bands(const std::string& str)
//{
//  std::vector<modis2scidb::band_info> result;
//
//  std::vector<std::string> bands_str;
//
//  boost::split(bands_str, str, boost::is_any_of(";"));
//
//  for(std::size_t i = 0; i != bands_str.size(); ++i)
//  {
//    std::vector<std::string> band_str;
//
//    boost::split(band_str, bands_str[i], boost::is_any_of(","));
//
//    if(band_str.size() != 2)
//      throw parser_error() << error_description("invalid band information syntax!");
//
//    band_info bi;
//    bi.number = boost::lexical_cast<uint16_t>(band_str.front());
//    bi.alias = band_str.back();
//    
//    result.push_back(bi);
//  }
//
//  return result;
//}

std::pair<unsigned int, unsigned int>
modis2scidb::parse_tile_dimension_range(const std::string& range)
{
  std::pair<unsigned int, unsigned int> tile_range;
  std::size_t pos = range.find("-");

  if(pos != std::string::npos)
  {
    tile_range.first = boost::lexical_cast<unsigned int>(range.substr(0, pos));
    tile_range.second = boost::lexical_cast<unsigned int>(range.substr(pos + 1));
  }
  else
  {
    tile_range.second = tile_range.first = boost::lexical_cast<unsigned int>(range);
  }

  return tile_range;
}

modis2scidb::modis_file_descriptor
modis2scidb::parse_modis_file_name(const std::string& file_name)
{
  if(file_name.empty())
    throw parser_error() << error_description("can not parse and empty MODIS file name!");

  boost::char_separator<char> sep(".");

  boost::tokenizer<boost::char_separator<char> > tokens(file_name, sep);

  boost::tokenizer<boost::char_separator<char> >::iterator itoken = tokens.begin();

  if(itoken == tokens.end())
    throw parser_error() << error_description("invalid MODIS file format!");

  modis_file_descriptor mfile;

  mfile.data_type_name = *itoken;

  ++itoken;

  if(itoken == tokens.end())
    throw parser_error() << error_description("invalid MODIS file format: missing date of acquisition!");

  mfile.acquisition_date = *itoken;

  ++itoken;

  if(itoken == tokens.end())
    throw parser_error() << error_description("invalid MODIS file format: missing tile identifier!");

  mfile.tile = *itoken;

  ++itoken;

  if(itoken == tokens.end())
    throw parser_error() << error_description("invalid MODIS file format: missing collection version!");

  mfile.collection_version = *itoken;

  ++itoken;

  if(itoken == tokens.end())
    throw parser_error() << error_description("invalid MODIS file format: missing production date!");

  mfile.production_date = *itoken;

  return mfile;
}

boost::gregorian::date
modis2scidb::parse_modis_acquisition_date(const std::string& adate)
{
  if(adate.length() < 8)
    throw parser_error() << error_description("invalid acquisition date format!");

  if(adate[0] != 'A')
    throw parser_error() << error_description("invalid acquisition date format: not beginning with A!");

  std::string sjyear = adate.substr(1, 4);

  std::string sjday = adate.substr(5, 3);

  boost::gregorian::greg_year jyear(boost::lexical_cast<boost::gregorian::greg_year::value_type>(sjyear));

  long ljday = boost::lexical_cast<long>(sjday);

  boost::gregorian::date d(jyear, boost::gregorian::Jan, 1);

  boost::gregorian::date_duration dur(ljday - 1);

  return d + dur;
}

std::vector<std::string>
modis2scidb::extract_subdatasets_pattern_names(const std::string& file_name)
{
  std::vector<std::string> result_patterns;

  GDALDatasetH dataset = GDALOpen(file_name.c_str(), GA_ReadOnly);

  if(dataset == 0)
    throw gdal_error() << error_description("could not open hdf file!");

  char** subdatasets = GDALGetMetadata(dataset, "SUBDATASETS");

  if(subdatasets != 0)
  {
    for(char** i = subdatasets; *i != 0; i += 2)
    {
      std::string name = *i;

      std::size_t pos = name.find("=");

      if(pos == std::string::npos)
      {
        GDALClose(dataset);

        throw gdal_error() << error_description("could not find subdataset name metadata in the hdf file!");
      }

      name = name.substr(pos + 1);

      std::string pname = make_subdataset_pattern(file_name, name);

      result_patterns.push_back(pname);
    }
  }

  GDALClose(dataset);

  return result_patterns;
}

std::string
modis2scidb::make_subdataset_pattern(const std::string& file_name,
                                     const std::string& subdataset_name)
{
  std::size_t pos = subdataset_name.find(file_name);

  if(pos == std::string::npos)
    throw gdal_error() << error_description("could not determine a prefix pattern for a subdataset name to be used in GDAL!");

  std::string prefix = subdataset_name.substr(0, pos);

  std::string suffix = subdataset_name.substr(pos + file_name.length());

  std::string pname(prefix + "%1%" + suffix);

  return pname;
}

std::size_t
modis2scidb::num_bytes(GDALDataType dt)
{
  switch(dt)
  {
    case GDT_Byte:
      return sizeof(char);
      
    case GDT_UInt16:
      return sizeof(uint16_t);
      
    case GDT_Int16:
      return sizeof(int16_t);
      
    case GDT_UInt32:
      return sizeof(uint32_t);
      
    case GDT_Int32:
      return sizeof(int32_t);
      
    case GDT_Float32:
      return sizeof(float);
      
    case GDT_Float64:
      return sizeof(double);
      
    default:
      throw conversion_error() << error_description("can not convert this GDAL type to MODIS datatype range!");
  }
}


