/**
  ******************************************************************************
  * @brief   Repeat macro.
  *          Taken from: 
https://stackoverflow.com/questions/8551418/c-preprocessor-macro-for-returning-a-string-repeated-a-certain-number-of-times
  ******************************************************************************
  */ 

#define REP0(X)
#define REP1(X) X
#define REP2(X) REP1(X) REP1(X)
#define REP3(X) REP2(X) REP1(X)
#define REP4(X) REP3(X) REP1(X)
#define REP5(X) REP4(X) REP1(X)
#define REP6(X) REP5(X) REP1(X)
#define REP7(X) REP6(X) REP1(X)
#define REP8(X) REP7(X) REP1(X)
#define REP9(X) REP8(X) REP1(X)

#define REP00(X)
#define REP10(X)  REP9(X)  REP1(X)
#define REP20(X) REP10(X) REP10(X)
#define REP30(X) REP20(X) REP10(X)
#define REP40(X) REP30(X) REP10(X)
#define REP50(X) REP40(X) REP10(X)
#define REP60(X) REP50(X) REP10(X)
#define REP70(X) REP60(X) REP10(X)
#define REP80(X) REP70(X) REP10(X)
#define REP90(X) REP80(X) REP10(X)

#define REP(TENS,ONES,X) \
	REP##TENS##0(X) \
	REP##ONES(X)
