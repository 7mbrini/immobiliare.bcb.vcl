//---------------------------------------------------------------------------
#ifndef errormsgsH
#define errormsgsH

#define ERR_DBNOASSETS			1100
#define ERR_DBEMPTYASSETS		1110
#define ERR_DBNOCUSTOMERS		1120
#define ERR_DBRECORDNOTFOUND	1130
#define ERR_DBNOCITIESFILE		1140
#define ERR_DBNODATA			1150
#define ERR_DBSAVEASSETS		1160
#define ERR_DBSAVECUSTOMERS		1170
#define ERR_DBEMPTYCUSTOMERS	1180

#define ERR_OPENASSETS			2100
#define ERR_OPENCUSTOMERS		2110


enum enErrMsgType { emtError, emtWarning, emtNotice };

void ShowErrorMsg(int nErrCode, enErrMsgType = emtError);

//---------------------------------------------------------------------------
#endif

