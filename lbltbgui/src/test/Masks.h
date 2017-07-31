#ifndef __Masks__hh
#define __Masks__hh

#ifndef __CINT__

#include <iomanip>
#include <sstream>

#include <Rtypes.h> // TESTBIT

//#define TESTBIT(w,n) (w & (0x1 << n))
#define IS_FWREG(w) (TESTBIT(w, 28) == 0 && TESTBIT(w, 24) == 0)
#define IS_HEADW(w) (TESTBIT(w, 28) == 0 && TESTBIT(w, 24) == 1)
#define IS_PAYLW(w) (TESTBIT(w, 28) == 1 && TESTBIT(w, 24) == 1)
#define IS_TRAIL(w) (TESTBIT(w, 28) == 1 && TESTBIT(w, 24) == 0)

#define BYTE(w, n) ( (w & (0xff << (n * 8))) >> (n * 8) )

#define BIT_MASK(a, b) (((unsigned) -1 >> (31 - (b))) & ~((1U << (a)) - 1))
#define BIT_INT(w, i, k) ( (unsigned)((w & BIT_MASK(i,k)) >> i ) )

#define HAS_MASK(w,m) ( (w & m) == m )
#define IS_MASK(w,m) ( w == m )

#define IS_ADC_HEAD(w) 			( IS_HEADW(w) && ( HAS_MASK(w, 0xadc00000) || HAS_MASK(w, 0xadc10000) ) )
#define IS_ADC0_HEAD(w) 		( IS_HEADW(w) && HAS_MASK(w, 0xadc00000) )
#define IS_ADC1_HEAD(w) 		( IS_HEADW(w) && HAS_MASK(w, 0xadc10000) )

#define IS_SPI_HEAD(w) 			( IS_HEADW(w) && HAS_MASK(w, 0x01230000 ) )
#define IS_I2C_HEAD(w) 			( IS_HEADW(w) && HAS_MASK(w, 0xabcd0000 ) )

#define IS_COP_SEU_HEAD(w)              ( IS_HEADW(w) && IS_MASK(w, 0xcf000000 ) ) //Configuration Cross CHeck                                 
#define IS_COP_DUT_HEAD(w)              ( IS_HEADW(w) && IS_MASK(w, 0xcf000001 ) ) //Bit test I^2C/SPI components on DUT                       
#define IS_COP_ADCS_HEAD(w)             ( IS_HEADW(w) && IS_MASK(w, 0xaf000002 ) ) //AMplifiers DC Scan                                        
#define IS_COP_DCM_HEAD(w)              ( IS_HEADW(w) && IS_MASK(w, 0xcd000003 ) ) //Digital Potentiometer DC Measurement                      
#define IS_COP_ACM_HEAD(w)              ( IS_HEADW(w) && IS_MASK(w, 0xcd000013 ) ) //Digital Potentiometer AC Measurement                      
#define IS_COP_DACDCS_HEAD(w)           ( IS_HEADW(w) && IS_MASK(w, 0x0d000004 ) ) //DAC DC Scan                                               
#define IS_COP_CDCS_HEAD(w)             ( IS_HEADW(w) && IS_MASK(w, 0xc1000005 ) ) //Comparator DC Scan                                        
#define IS_COP_LUCT_HEAD(w)             ( IS_HEADW(w) && IS_MASK(w, 0xab000006 ) ) //Latch-Up Circuit Test                                     
#define IS_COP_RTDS_HEAD(w)             ( IS_HEADW(w) && IS_MASK(w, 0x6d000007 ) ) //Ressistance to Digital Scan                               
#define IS_COP_RCC_HEAD(w)              ( IS_HEADW(w) && IS_MASK(w, 0xef000008 ) ) //Reset Circuit Check                                       
#define IS_COP_OMADCRO_HEAD(w)          ( IS_HEADW(w) && IS_MASK(w, 0xc3000009 ) ) //Online Monitoring ADC readout 

#define IS_ADC_TRAIL(w)			( IS_TRAIL(w) && IS_MASK(w, 0xfeedbeef ) )
#define IS_CO_TRAIL(w) 			( IS_TRAIL(w) && IS_MASK(w, 0xfadecafe ) )

#define TSTAMP(w) (BIT_INT(w,0,15))

#define ADCMP603_Q_OUT(w)			( TESTBIT (w, 0) )
#define ADCMP603_Q_BAR_OUT(w)		( TESTBIT (w, 1) )
#define ADCMP603_LE_HYS_OUT(w)		( TESTBIT (w, 2) )

namespace Masks
{
	inline bool is_known_header(const unsigned int &w)
	{
		return 	IS_ADC_HEAD(w) 			| 
				IS_SPI_HEAD(w) 			| 
				IS_I2C_HEAD(w) 			| 
				IS_COP_SEU_HEAD(w) 		| 
				IS_COP_DUT_HEAD(w) 		| 
				IS_COP_ADCS_HEAD(w) 	| 
				IS_COP_DCM_HEAD(w) 		| 
				IS_COP_ACM_HEAD(w) 		| 
				IS_COP_DACDCS_HEAD(w) 	| 
				IS_COP_CDCS_HEAD(w) 	| 
				IS_COP_LUCT_HEAD(w) 	| 
				IS_COP_RTDS_HEAD(w) 	| 
				IS_COP_RCC_HEAD(w) 		| 
				IS_COP_OMADCRO_HEAD(w);
	}

	inline bool is_known_trailer(const unsigned int &w)
	{
		return IS_ADC_TRAIL(w) | IS_CO_TRAIL(w);
	}

	inline std::string debug_type_word(const unsigned int &w)
	{
		std::string ret = "";
		if (IS_FWREG(w))
		{
			ret += "fw. reg. ";
			if (ADCMP603_Q_OUT(w))
				ret += "Q:1 ";
			else
				ret += "Q:0 ";			
			if (ADCMP603_Q_BAR_OUT(w))
				ret += "Q_BAR:1 ";
			else
				ret += "Q_BAR:0 ";			
			if (ADCMP603_LE_HYS_OUT(w))
				ret += "LE_HYS:1 ";
			else
				ret += "LE_HYS:0 ";
		}
		if (IS_HEADW(w))
		{
			ret += "header   ";
			if (is_known_header(w))
				ret += "known  ";
			else
				ret += "UNKOWN ";
			if (IS_ADC_HEAD(w))
			{
				std::stringstream ss;
				ss << std::hex << "t-stamp: 0x" << std::setfill('0') << std::setw(4) << TSTAMP(w) 
				<< " (" << std::dec << TSTAMP(w) << ") ";
				ret += ss.str();
			}
		}
		if (IS_PAYLW(w))
			ret += "payload  ";
		if (IS_TRAIL(w))
		{
			ret += "trailer  ";
			if (is_known_trailer(w))
				ret += "known  ";
			else
				ret += "UNKOWN ";
			if (IS_ADC_TRAIL(w))
			{
				ret += "ADC ";
			}
			if (IS_CO_TRAIL(w))
			{
				ret += "comp. op. ";
			}
		}
		return ret;
	}
}

#endif // __CINT__
#endif // __Masks__hh