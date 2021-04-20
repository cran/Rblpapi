## ---- eval=FALSE--------------------------------------------------------------
#  library(Rblpapi)

## ---- eval=FALSE--------------------------------------------------------------
#  blpConnect()

## ---- eval=FALSE--------------------------------------------------------------
#  options("blpAutoConnect" = TRUE)
#  options("blpHost" = "x.x.x.x")
#  options("blpPort" = 8194)
#  options("blpAppName" = "yyy")

## ---- eval=FALSE--------------------------------------------------------------
#  blpAuthenticate()

## ---- eval=FALSE--------------------------------------------------------------
#  options("blpAutoAuthenticate" = TRUE)
#  options("blpUUID" = "xxx")
#  options("blpLoginHostname" = "yyy")
#  options("blpLoginIP" = "z.z.z.z")
#  

## ---- eval=FALSE--------------------------------------------------------------
#  blpConnect( ... , appName = "appName")
#  blpAuthenticate()

## ---- eval=FALSE--------------------------------------------------------------
#  blpConnect( ... )
#  blpAuthenticate( uuid="UUID", ip.address="x.x.x.x")

## ---- eval=FALSE--------------------------------------------------------------
#  bdp(c("ESA Index", "SPY US Equity"), c("PX_LAST", "VOLUME"))

## ---- eval=FALSE--------------------------------------------------------------
#  bds("GOOG US Equity", "TOP_20_HOLDERS_PUBLIC_FILINGS")

## ---- eval=FALSE--------------------------------------------------------------
#  bdh("SPY US Equity", c("PX_LAST", "VOLUME"), start.date=Sys.Date()-31)

## ---- eval=FALSE--------------------------------------------------------------
#  getBars("ES1 Index")

## ---- eval=FALSE--------------------------------------------------------------
#  getTicks("ES1 Index")

## ---- eval=FALSE--------------------------------------------------------------
#  res <- fieldSearch("vwap")

## ---- eval=FALSE--------------------------------------------------------------
#  beqs("Global Oil Companies YTD Return","GLOBAL")

