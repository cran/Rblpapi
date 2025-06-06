\name{NEWS}
\title{News for Package \pkg{Rblpapi}}
\newcommand{\ghrepo}{\href{https://github.com/#1}{\pkg{#1}}}
\newcommand{\ghpr}{\href{https://github.com/Rblp/Rblpapi/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/Rblp/Rblpapi/issues/#1}{##1}}

\section{Changes in Rblpapi version 0.3.16 (2025-03-31)}{
  \itemize{
    \item A quota error message is now improved (Rodolphe Duge in \ghpr{400})
    \item Convert remaining \code{throw} into \code{Rcpp::stop} (Dirk in
    \ghpr{402} fixing \ghit{401})
    \item Add optional \code{appIdentityKey} argument to \code{blpConnect}
    (Kai Lin in \ghpr{404})
    \item Rework build as function of Blp library availability (Dirk and
    John in \ghpr{406}, \ghpr{409}, \ghpr{410} fixing \ghit{407}, \ghit{408})
  }
}

\section{Changes in Rblpapi version 0.3.15 (2024-09-18)}{
  \itemize{
    \item A warning is now issued if more than 1000 results are returned
    (John in \ghpr{377} addressing \ghit{375})
    \item A few typos in the rblpapi-intro vignette were corrected (Michael
    Streatfield in \ghpr{378})
    \item The continuous integration setup was updated (Dirk in \ghpr{388})
    \item Deprecation warnings over \code{char*} where C++ class \code{Name}
    is now preferred have been addressed (Dirk in \ghpr{391})
    \item Several package files have been updated (Dirk in \ghpr{392})
    \item The \code{request} formation has been corrected, and an example was
    added (Dirk and John in \ghpr{394} and \ghpr{396})
    \item The Bloomberg API has been upgraded to release 3.24.6.1 (Dirk in
    \ghpr{397})
  }
}

\section{Changes in Rblpapi version 0.3.14 (2022-10-05)}{
  \itemize{
    \item Build configuration was generalized to consider local copies
    of library and headers (Dirk in \ghpr{362})
    \item A ticker symbol was corrected (Dirk in \ghpr{368} addressing
    an issue \ghit{366} and \ghit{367})
    \item Support for B-PIPE was added (Robert Harlow in \ghpr{369}
    closing \ghit{342})
    \item The package no longer requires staged installation thanks to
    an assist from Tomas Kalibera (Dirk in \ghpr{373})
    \item The retired package \pkg{fts} is no longer suggested (Dirk in
    \ghpr{374} closing \ghpr{372})
  }
}

\section{Changes in Rblpapi version 0.3.13 (2022-01-09)}{
  \itemize{
    \item Add a test for \code{bds} (Michael Kerber in \ghpr{352})
    \item Add \code{simplify} argument (and option) to \code{bdh} and
    \code{bds} (Dirk in \ghpr{354} closing \ghit{353}, \ghit{351})
    \item Improve documentation for \code{bsearch} (John in \ghpr{357}
    closing \ghit{356})
  }
}

\section{Changes in Rblpapi version 0.3.12 (2021-12-07)}{
  \itemize{
    \item \code{bdh()} supports new option \code{returnAs} (Michael
    Kerber and Dirk in \ghpr{335} fixing \ghit{206})
    \item Remove extra backtick in vignette (Yihui Xie in \ghpr{343})
    \item Fix a segfault from bulk access with \code{bds} (Kai Lin in
    \ghpr{347} fixing \ghit{253})
    \item Support \code{REQUEST_STATUS} in \code{bdh} (Kai Lin and John
    in \ghpr{349} fixing \ghit{348})
    \item Vignette now uses \pkg{simplermarkdown} (Dirk in \ghpr{350})
  }
}

\section{Changes in Rblpapi version 0.3.11 (2021-04-20)}{
  \itemize{
    \item Support blpAutoAuthenticate and B-PIPE access, refactor and
    generalise authentication (James Bell in \ghpr{285})
    \item Deprecate \code{excludeterm} (John in \ghpr{306})
    \item Correct example in README.md (Maxime Legrand in \ghpr{314})
    \item Correct \code{bds} man page (and code) (Michael Kerber, and
    John, in \ghpr{320})
    \item Add GitHub Actions continuous integration (Dirk in \ghpr{323})
    \item Remove bashisms detected by R CMD check (Dirk \ghpr{324})
    \item Switch vignette to minidown (Dirk in \ghpr{331})
    \item Switch unit tests framework to tinytest (Dirk in \ghpr{332})
  }
}

\section{Changes in Rblpapi version 0.3.10 (2019-04-02)}{
  \itemize{
    \item The \code{start.date} format for \code{bdh} now allows character
    values with relative date expressions (John in \ghpr{267} fixing
    \ghit{266}).
    \item Relative date values are allowed in return (John in \ghpr{269}
    fixing \ghit{268}).
    \item The \code{Rcpp::DateVector} class is used instead of a local
    date vector class (John in \ghpr{273}).
    \item The file \code{src/init.c} is now redundant and removed as
    updated registration is in \code{src/RcppExports.cpp} (Davis Vaughan
    in \ghpr{275} fixing \ghit{274}).
    \item Returned strings are now marked as UTF-8 encoded (Xianying Tan in
      \ghpr{278})
    \item Date vectors are now initialized with NA (Xianying Tan in
      \ghpr{279} fixing an inssue introduced in \ghpr{273})
    \item Authentication for BPIPE is now supported (Alfred Kanzler in
      \ghpr{288} with assistance help from Dirk and John)
  }
}

\section{Changes in Rblpapi version 0.3.9 (2019-02-20)}{
  \itemize{
    \item Add 'StagedInstall: no' to DESCRIPTION to accomodate R 3.6.0.
  }
}

\section{Changes in Rblpapi version 0.3.8 (2018-01-20)}{
  \itemize{
    \item This release should have been 0.3.7 but was accidentally
    tagged as 0.3.8 in the DESCRIPTION file so we skipped 0.3.7.
    \item The 140 day limit for intra-day data histories is now
    mentioned in the \code{getTicks} help (Dirk in \ghpr{226} addressing
    \ghit{215} and \ghit{225}).
    \item The Travis CI script was updated to use \code{run.sh} (Dirk in
    \ghpr{226}).
    \item The \code{install_name_tool} invocation under macOS was
    corrected (@spennihana in \ghit{232})
    \item The \code{blpAuthenticate} help page has additional examples
    (@randomee in \ghpr{252}).
    \item The \code{blpAuthenticate} code was updated and improved (Whit
    in \ghpr{258} addressing \ghpr{257})
  }
}

\section{Changes in Rblpapi version 0.3.6 (2017-04-20)}{
  \itemize{
    \item \code{bdh} can now store in \code{double} preventing overflow
    (Whit and John in \ghpr{205} closing \ghit{163})
    \item \code{bdp} documentation has another ovveride example
    \item A new function \code{lookupSecurity} can search for securities,
    optionally filtered by yellow key (Kevin Jin and Dirk in \ghpr{216}
    and \ghpr{217} closing \ghit{215})
    \item Added file \code{init.c} with calls to \code{R_registerRoutines()}
    and \code{R_useDynamicSymbols()}; also use \code{.registration=TRUE}
    in \code{useDynLib} in \code{NAMESPACE} (Dirk in \ghpr{220})
    \item \code{getBars} and \code{getTicks} can now return
    \code{data.table} objects (Dirk in \ghpr{221})
    \item \code{bds} has improved internal protect logic via
    \code{Rcpp::Shield} (Dirk in \ghpr{222})
  }
}

\section{Changes in Rblpapi version 0.3.5 (2016-10-25)}{
  \itemize{
    \item Add new function \code{getPortfolio} to retrieve portfolio data
    via \code{bds} (John in \ghpr{176})
    \item Extend \code{getTicks()} to (optionally) return non-numeric
    data as part of \code{data.frame} or \code{data.table} (Dirk in
    \ghpr{200})
    \item Similarly extend \code{getMultipleTicks} (Dirk in \ghpr{202})
    \item Correct statement on timestamp for \code{getBars} (Closes
    issue \ghit{192})
    \item Minor edits to a few files in order to either please
    \code{R(-devel) CMD check --as-cran}, or update documentation
    \item Adding registration of routines are R 3.4.0 now desires.
    \item Optional export as \code{data.table} is now more standardized.
  }
}

\section{Changes in Rblpapi version 0.3.4 (2016-05-08)}{
  \itemize{
    \item On startup, the API versions of both the headers and the
    runtime are displayed (PR \ghpr{161} and \ghpr{165}).
    \item Documentation about extended futures roll notation was added
    to the \code{bdh} manul page.
    \item Additional examples for overrides where added to \code{bdh}
    (PR \ghpr{158}).
    \item Internal code changes make retrieval of data in
    \sQuote{unusual} variable types more robust (PRs \ghpr{157} and \ghpr{153})
    \item General improvements and fixes to documentation (PR \ghpr{156})
    \item The \code{bdp} function now also supports an option
    \code{verbose} (PR \ghpr{149}).
    \item The internal header \code{Rblpapi_types.h} was renamed from a
    lower-cased variant to conform with Rcpp Attributes best
    practices (PR \ghpr{145}).
  }
}

\section{Changes in Rblpapi version 0.3.3 (2016-03-14)}{
  \itemize{
    \item \code{configure} adds a missing \code{method="libcurl"}
    options (PR \ghpr{109} by Martin Bel).
    \item New function \code{bsrch()} adds basic search functionality
    (PR \ghpr{111} by Morgan Williams fixing ticket \ghit{82}).
    \item The licensing status of the Rblpapi source package was
    clarified (PR \ghpr{119}).
    \item The \code{bds()} help page now shows an example using an
    \code{overrides} argument (PR \ghpr{121}).
    \item A new function \code{fieldInfo} was added, and field
    information is used in \code{bdh} and \code{bdp} (PRs \ghpr{122},
    \ghpr{123}, \ghpr{125}, and \ghpr{127}).
    \item The \code{bdp} function now checks the order (PR \ghpr{126}).
    \item Unit testing was added (PRs \ghpr{129} and \ghpr{131}).
    \item Data retrieval is now smarter about the types returned from
    Bloomberg (PR \ghpr{132} and \ghpr{133}; and \ghpr{141} and \ghpr{142}).
    \item The \code{bdh} and \code{bds} functions now support an option
    \code{verbose} (PRs \ghpr{138}).
  }
}

\section{Changes in Rblpapi version 0.3.2 (2015-12-06)}{
  \itemize{
    \item A new \code{subscribe()} function supports live data
    subscription for a set of tickers and fields (\ghpr{88}).
    \item In the \code{beqs()} example the correct date format is used
    (PR \ghpr{85} by Rademeyer).
    \item Both \code{getTicks()} and \code{getBars()} now check the
    start and end times for proper \code{Datetime} class.
    \item The \code{getBars()} function now also return the
    \sQuote{value traded} (request of \ghit{89}).
    \item An error in the documentation for \code{bdh} was corrected (PR
    \ghpr{93} by Rademeyer closing \ghit{85}).
    \item The \code{beqs()} function is now more robust with respect to
    empty return columns (PR \ghpr{100} by Rademeyer fixing ticket
    \ghit{99}).
    \item The \code{getBars()} function now uses a single (named) vector
    \code{options} to pass optional values to the underlying function
    (PR \ghpr{105} updating PR \ghpr{48} and fixing \ghit{47}).
    \item When R is built with libcurl support, it used to download the
    build-time library and headers; otherwise \code{curl} is used. This
    avoids an issue on OS X where \code{curl} is insufficient.
  }
}

\section{Changes in Rblpapi version 0.3.1 (2015-10-19)}{
  \itemize{
    \item Added \code{beqs()} to run Bloomberg Equity Screen Search,
    based on initial PR \ghpr{79} by Rademeyer Vermaak, reworked in PRs
    \ghpr{83} and \ghpr{84} by Dirk; closes tickets \ghit{63} and \ghit{80}.
    \item Bloomberg header and library files are now cached locally
    instead of being re-downloaded for every build (PR \ghpr{78}
    by Dirk addressing issue \ghpr{65}).
    \item For multiple ticks, time-stamps are unique-yfied before
    merging (PR \ghpr{77} by Dirk addressing issue \ghit{76}).
    \item A compiler warning under new g++ versions is now suppressed
    (PR \ghpr{69} by John, fixing \ghit{68}).
  }
}

\section{Changes in Rblpapi version 0.3.0 (2015-08-13)}{
  \itemize{
    \item First CRAN Release
    \item Source and binary builds supported on Linux, Windows and OS X
    \item Build procedures rewritten so that required headers and
    libraries are downloaded during installation from corresponding GitHub
    repo \ghrepo{Rblp/blp} which contains custom tarballs derived from the
    corresponding upstream releases at
    \url{https://www.bloomberg.com/professional/support/api-library/ }.
  }
}
