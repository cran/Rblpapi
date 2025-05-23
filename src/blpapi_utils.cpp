
//  blpapi_utils -- helper functions for the BLP API
//
//  Copyright (C) 2013         Whit Armstrong
//  Copyright (C) 2017 - 2025  Whit Armstrong and Dirk Eddelbuettel
//
//  This file is part of Rblpapi
//
//  Rblpapi is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  Rblpapi is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Rblpapi.  If not, see <http://www.gnu.org/licenses/>.

#if defined(HaveBlp)

#include <stdexcept>
#include <string>
#include <sstream>
//#include <iostream>
#include <algorithm>
#define BOOST_NO_AUTO_PTR
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/local_time/local_time_types.hpp>
#include <blpapi_session.h>
#include <blpapi_request.h>
#include <blpapi_datetime.h>
#include <Rcpp.h>
#include <blpapi_utils.h>

using std::vector;
using std::string;
using BloombergLP::blpapi::Session;
using BloombergLP::blpapi::Service;
using BloombergLP::blpapi::Request;
using BloombergLP::blpapi::Identity;
using BloombergLP::blpapi::Datetime;
using BloombergLP::blpapi::DatetimeParts;
using BloombergLP::blpapi::Element;
using BloombergLP::blpapi::Event;
using BloombergLP::blpapi::Message;
using BloombergLP::blpapi::MessageIterator;
using BloombergLP::blpapi::Name;

void* checkExternalPointer(SEXP xp_, const char* valid_tag) {
  if(xp_ == R_NilValue) {
    Rcpp::stop("External pointer is NULL.");
  }
  if(TYPEOF(xp_) != EXTPTRSXP) {
    Rcpp::stop("Not an external pointer.");
  }

  if(R_ExternalPtrTag(xp_)==R_NilValue) {
    Rcpp::stop("External pointer tag is NULL.");
  }
  const char* xp_tag = CHAR(PRINTNAME(R_ExternalPtrTag(xp_)));
  if(!xp_tag) {
    Rcpp::stop("External pointer tag is blank.");
  }
  if(std::strcmp(xp_tag,valid_tag) != 0) {
    Rcpp::stop("External pointer tag does not match.");
  }
  if(R_ExternalPtrAddr(xp_)==NULL) {
    Rcpp::stop("External pointer address is null.");
  }
  return R_ExternalPtrAddr(xp_);
}

const int bbgDateToRDate(const Datetime& bbg_date) {
  if(bbg_date.hasParts(DatetimeParts::TIME)) {
    Rcpp::stop("Attempt to convert a Datetime with time parts set to an R Date.");
  }
  const boost::gregorian::date r_epoch(1970,1,1);
  boost::gregorian::date bbg_boost_date(bbg_date.year(),bbg_date.month(),bbg_date.day());
  boost::gregorian::date_period dp(r_epoch,bbg_boost_date);
  return static_cast<int>(dp.length().days());
}

const int bbgDateToRDate(const double yyyymmdd_date) {
  if(yyyymmdd_date < 0) {
    Rcpp::stop("Attempt to convert a negative double value to an R Date.");
  }
  if(trunc(yyyymmdd_date)!=yyyymmdd_date) {
    Rcpp::stop("Attempt to convert a double value with time parts set to an R Date.");
  }

  const boost::gregorian::date r_epoch(1970,1,1);
  const int year = static_cast<int>(yyyymmdd_date/1.0e4);
  const int month = static_cast<int>(yyyymmdd_date/1.0e2) % 100;
  const int day = static_cast<int>(yyyymmdd_date) % 100;
  boost::gregorian::date bbg_boost_date(year,month,day);
  boost::gregorian::date_period dp(r_epoch,bbg_boost_date);
  return static_cast<int>(dp.length().days());
}

const double bbgDateToPOSIX(const Datetime& bbg_date) {
  boost::gregorian::date bbg_boost_date(bbg_date.year(),bbg_date.month(),bbg_date.day());
  struct tm tm_time(to_tm(bbg_boost_date));
  return static_cast<double>(mktime(&tm_time));
}

const double bbgDatetimeToPOSIX(const Datetime& dt) {
  boost::gregorian::date bbg_boost_date(dt.year(),dt.month(),dt.day());
  boost::posix_time::time_duration td =
    boost::posix_time::hours(dt.hours()) +
    boost::posix_time::minutes(dt.minutes()) +
    boost::posix_time::seconds(dt.seconds()) +
    boost::posix_time::milliseconds(dt.milliseconds());

  boost::posix_time::ptime bbg_ptime(bbg_boost_date,td);
  struct tm tm_time(to_tm(bbg_ptime));
  return static_cast<double>(mktime(&tm_time));
}

// In case data already comes in localtime (cf TZDF<GO>)
// we need to adjust back to UTC
const double bbgDatetimeToUTC(const BloombergLP::blpapi::Datetime& dt) {
  boost::gregorian::date bbg_boost_date(dt.year(),dt.month(),dt.day());
  boost::posix_time::time_duration td =
    boost::posix_time::hours(dt.hours()) +
    boost::posix_time::minutes(dt.minutes()) +
    boost::posix_time::seconds(dt.seconds()) +
    boost::posix_time::milliseconds(dt.milliseconds());
  boost::posix_time::ptime bbg_ptime(bbg_boost_date,td);

  // cf http://stackoverflow.com/a/4462309/143305
  boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
  boost::posix_time::time_duration::sec_type x = (bbg_ptime - epoch).total_seconds();
  return x;
}

void addPosixClass(SEXP x) {
    // create and add dates class to dates object
    // cf Rcpp's inst/include/Rcpp/date_datetime/newDatetimeVector.h
    Rcpp::Shield<SEXP> datetimeclass(Rf_allocVector(STRSXP,2));
    SET_STRING_ELT(datetimeclass, 0, Rf_mkChar("POSIXct"));
    SET_STRING_ELT(datetimeclass, 1, Rf_mkChar("POSIXt"));
    Rf_setAttrib(x, R_ClassSymbol, datetimeclass);
}

void appendOptionsToRequest(Request& request, SEXP options_) {
  if(options_== R_NilValue) { return; }
  Rcpp::CharacterVector options(options_);

  if(!options.hasAttribute("names")) {
    Rcpp::stop("Request options must be named.");
  }

  if(options.attr("names") == R_NilValue) {
    Rcpp::stop("Request optionnames must not be null.");
  }

  Rcpp::CharacterVector options_names(options.attr("names"));

  if(options.length() && options_names.length()==0) {
    Rcpp::stop("Request options must be non empty and named.");
  }

  for(R_len_t i = 0; i < options.length(); i++) {
    request.set(Name{ static_cast<std::string>(options_names[i]).c_str() }, static_cast<std::string>(options[i]).c_str());
  }
}

void appendOverridesToRequest(Request& request, SEXP overrides_) {
  if(overrides_ == R_NilValue) { return; }
  Rcpp::CharacterVector overrides(overrides_);

  if(!overrides.hasAttribute("names") || overrides.attr("names") == R_NilValue) {
    Rcpp::stop("Request overrides must be named.");
  }

  Rcpp::CharacterVector overrides_names(overrides.attr("names"));

  if(overrides.length() && overrides_names.length()==0) {
    Rcpp::stop("Request overrides must be non empty and named.");
  }

  Element request_overrides = request.getElement(Name{"overrides"});
  for(R_len_t i = 0; i < overrides.length(); i++) {
    Element this_override = request_overrides.appendElement();
    this_override.setElement(Name{"fieldId"}, static_cast<std::string>(overrides_names[i]).c_str());
    this_override.setElement(Name{"value"}, static_cast<std::string>(overrides[i]).c_str());
  }
}

void createStandardRequest(Request& request,
                           const std::vector<std::string>& securities,
                           const std::vector<std::string>& fields,
                           SEXP options_,
                           SEXP overrides_) {

  for(size_t i = 0; i < securities.size(); i++) {
    request.getElement(Name{"securities"}).appendValue(securities[i].c_str());
  }

  for(size_t i = 0; i < fields.size(); i++) {
    request.getElement(Name{"fields"}).appendValue(fields[i].c_str());
  }

  if(options_ != R_NilValue) { appendOptionsToRequest(request,options_); }
  if(overrides_ != R_NilValue) { appendOverridesToRequest(request,overrides_); }
}

void sendRequestWithIdentity(Session* session, Request& request, SEXP identity_) {
  Identity* ip;
  if(identity_ != R_NilValue) {
    ip = reinterpret_cast<Identity*>(checkExternalPointer(identity_,"blpapi::Identity*"));
    session->sendRequest(request,*ip);
  } else {
    session->sendRequest(request);
  }
}

void populateDfRow(SEXP ans, R_len_t row_index, const Element& e, RblpapiT rblpapitype) {
  // the vectors are already initialized to NAs
  // so no need to set as NA here
  if(e.isNull()) { return; }

  switch(rblpapitype) {
  case RblpapiT::Logical:
    LOGICAL(ans)[row_index] = e.getValueAsBool(); break;
  case RblpapiT::Integer:
    INTEGER(ans)[row_index] = e.getValueAsInt32(); break;
  case RblpapiT::Integer64:
    REAL(ans)[row_index] = e.getValueAsFloat64(); break;
  case RblpapiT::Double:
    REAL(ans)[row_index] = e.getValueAsFloat64(); break;
  case RblpapiT::Float:
    REAL(ans)[row_index] = e.getValueAsFloat64(); break;
  case RblpapiT::Date:
    // handle the case of BBG passing down dates as double in YYYYMMDD format
    REAL(ans)[row_index] = e.datatype()==BLPAPI_DATATYPE_FLOAT32 || e.datatype()==BLPAPI_DATATYPE_FLOAT64 ?
      bbgDateToRDate(e.getValueAsFloat64()) :
      bbgDateToRDate(e.getValueAsDatetime());
    break;
  case RblpapiT::Datetime:
    REAL(ans)[row_index] = bbgDateToPOSIX(e.getValueAsDatetime()); break;
  case RblpapiT::String:
    SET_STRING_ELT(ans,row_index,Rf_mkCharCE(e.getValueAsString(), CE_UTF8)); break;
  default: // try to convert it as a string
    SET_STRING_ELT(ans,row_index,Rf_mkCharCE(e.getValueAsString(), CE_UTF8)); break;
  }
}

Rcpp::NumericVector createPOSIXtVector(const std::vector<double> & ticks,
                                       const std::string tz) {
    Rcpp::NumericVector pt(ticks.begin(), ticks.end());
    pt.attr("class") = Rcpp::CharacterVector::create("POSIXct", "POSIXt");
    pt.attr("tzone") = tz;
    return pt;
}

std::string vectorToCSVString(const std::vector<std::string>& vec) {
  if(vec.empty()) {
    return std::string();
  } else {
    std::ostringstream oss;
    std::copy(vec.begin(), vec.end()-1,std::ostream_iterator<std::string>(oss, ","));
    oss << vec.back();
    return oss.str();
  }
}

// map to RblpapiT using datatype and ftype
// both are needed b/c datatype does not distinguish between date and datetime
RblpapiT fieldInfoToRblpapiT(const std::string& datatype, const std::string& ftype) {
  auto iter = stringToDatatypeT.find(datatype);
  if(iter == stringToDatatypeT.end()) {
    std::ostringstream err;
    err << "datatype not found: " << datatype;
    // No throw, try to be graceful
  }

  switch(iter->second) {
  case DatatypeT::Bool:
    return RblpapiT::Logical;
    break;
  case DatatypeT::String:
    return RblpapiT::String;
    break;
  case DatatypeT::Int32:
    return RblpapiT::Integer;
    break;
  case DatatypeT::Int64:
    return RblpapiT::Integer64;
    break;
  case DatatypeT::Double:
    return RblpapiT::Double;
    break;
  case DatatypeT::Float:
    return RblpapiT::Float;
    break;
  case DatatypeT::Datetime:
    if(ftype=="Date") {
      return RblpapiT::Date;
    } else {
      // ftype in ("Time","DateOrTime")
      return RblpapiT::String;
    }
    break;
  default:
    // just try to return it as a string
    return RblpapiT::String;
    break;
  }
  // never gets here
  return RblpapiT::String;
}

SEXP allocateDataFrameColumn(RblpapiT rblpapitype, const size_t n) {
    SEXP ans;
    switch(rblpapitype) {
    case RblpapiT::Logical:
        ans = Rcpp::LogicalVector(n, NA_LOGICAL);
        break;
    case RblpapiT::Integer:
        ans = Rcpp::IntegerVector(n, NA_INTEGER);
        break;
    case RblpapiT::Integer64:
    case RblpapiT::Double:
    case RblpapiT::Float:
        ans = Rcpp::NumericVector(n, NA_REAL);
        break;
    case RblpapiT::Date:
        ans = Rcpp::NumericVector(n, NA_REAL);
        ans = Rcpp::DateVector(ans);
        break;
    case RblpapiT::Datetime:
        // FIXME: string for datetime ?
        // ans = PROTECT(Rf_allocVector(STRSXP,n));
        ans = Rcpp::StringVector(n);
        break;
    case RblpapiT::String:
        ans = Rcpp::StringVector(n);
        break;
    default: // try to convert it as a string
        ans = Rcpp::StringVector(n);
        break;
    }
    return ans;
}

FieldInfo getFieldType(Session *session, Service& fieldInfoService, const std::string& field) {

  Request request = fieldInfoService.createRequest("FieldInfoRequest");
  request.append(Name{"id"}, field.c_str());
  request.set(Name{"returnFieldDocumentation"}, false);
  session->sendRequest(request);

  FieldInfo ans;
  while (true) {
    Event event = session->nextEvent();
    if (event.eventType() != Event::RESPONSE &&
        event.eventType() != Event::PARTIAL_RESPONSE) {
      continue;
    }

    MessageIterator msgIter(event);
    while (msgIter.next()) {
      Message msg = msgIter.message();
      //msg.asElement().print(std::cout);
      Element fields = msg.getElement(Name{"fieldData"});
      if(fields.numValues() > 1) {
        Rcpp::stop("getFieldType: too many fields returned.");
      }
      Element field = fields.getValueAsElement(0);
      if (!field.hasElement(Name{"id"})) {
        Rcpp::stop("Did not find 'id' in repsonse.");
      }
      if (field.hasElement(Name{"fieldError"})) {
        std::ostringstream err;
        err << "Bad field: " << field.getElementAsString(Name{"id"}) << std::endl;
        Rcpp::stop(err.str());
      }
      if (!field.hasElement(Name{"fieldInfo"})) {
        Rcpp::stop("Did not find fieldInfo in repsonse.");
      }
      Element fieldInfo = field.getElement(Name{"fieldInfo"});
      if (!fieldInfo.hasElement(Name{"mnemonic"}) ||
          !fieldInfo.hasElement(Name{"datatype"}) ||
          !fieldInfo.hasElement(Name{"ftype"})) {
        Rcpp::stop("fieldInfo missing info mnemonic/datatype/ftype.");
      }
      ans.id = field.getElementAsString(Name{"id"});
      ans.mnemonic = fieldInfo.getElementAsString(Name{"mnemonic"});
      ans.datatype = fieldInfo.getElementAsString(Name{"datatype"});
      ans.ftype = fieldInfo.getElementAsString(Name{"ftype"});
    }
    if (event.eventType() == Event::RESPONSE) {
      break;
    }
  }
  return ans;
}


std::vector<FieldInfo> getFieldTypes(Session *session,
                                     const std::vector<std::string> &fields) {
  const std::string APIFLDS_SVC("//blp/apiflds");
  if (!session->openService(APIFLDS_SVC.c_str())) {
    Rcpp::stop(std::string("Failed to open " + APIFLDS_SVC));
  }
  Service fieldInfoService = session->getService(APIFLDS_SVC.c_str());
  std::vector<FieldInfo> ans;
  for(auto field : fields) {
    ans.push_back(getFieldType(session, fieldInfoService, field));
  }
  return ans;
}

Rcpp::List allocateDataFrame(const vector<string>& rownames, const vector<string>& colnames, vector<RblpapiT>& coltypes) {

  if(colnames.size() != coltypes.size()) {
    Rcpp::stop("colnames size inconsistent with column types size.");
  }

  Rcpp::List ans(colnames.size());
  ans.attr("class") = "data.frame";
  ans.attr("names") = colnames;
  ans.attr("row.names") = rownames;
  for(size_t i = 0; i < colnames.size(); ++i) {
    ans[i] = allocateDataFrameColumn(coltypes[i], rownames.size());
  }
  return ans;
}

Rcpp::List allocateDataFrame(size_t nrows, const vector<string>& colnames, const vector<RblpapiT>& coltypes) {

  if(colnames.size() != coltypes.size()) {
    Rcpp::stop("colnames size inconsistent with column types size.");
  }

  Rcpp::List ans(colnames.size());
  ans.attr("class") = "data.frame";
  ans.attr("names") = colnames;
  Rcpp::IntegerVector rnms(nrows); std::iota(rnms.begin(), rnms.end(), 1);
  ans.attr("row.names") = rnms;
  for(size_t i = 0; i < colnames.size(); ++i) {
    ans[i] = allocateDataFrameColumn(coltypes[i], nrows);
  }
  return ans;
}

#endif
