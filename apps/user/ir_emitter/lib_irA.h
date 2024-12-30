#ifndef __IRLIB_H__
#define __IRLIB_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "cpu.h"

#define PWM_IR_OPEN		1
#define PWM_IR_CLOSE	0


#define AIR_INFO_CODE_NUMBER 79
//空调控制动作列表
#define ACTION_OPEN 		1 	//打开空调
#define ACTION_CLOSE 		2 	//关闭空调
#define ACTION_MODE_AUTO	3 	//自动模式
#define ACTION_MODE_COOL	4 	//制冷模式
#define ACTION_MODE_HOT		5 	//制热模式
#define ACTION_MODE_CHUSHI	6 	//除湿模式
#define ACTION_MODE_TONGF	7 	//通风模式


#define ACTION_TEMP_16		16 	//16度
#define ACTION_TEMP_17		17	//17度
#define ACTION_TEMP_18		18 	//18度
#define ACTION_TEMP_19		19 	//19度
#define ACTION_TEMP_20		20 	//20度
#define ACTION_TEMP_21		21 	//21度
#define ACTION_TEMP_22		22 	//22度
#define ACTION_TEMP_23		23 	//23度
#define ACTION_TEMP_24		24 	//24度
#define ACTION_TEMP_25		25 	//25度
#define ACTION_TEMP_26		26 	//26度
#define ACTION_TEMP_27		27 	//27度
#define ACTION_TEMP_28		28 	//28度
#define ACTION_TEMP_29		29 	//29度
#define ACTION_TEMP_30		30 	//30度


#define ACTION_WIND_AUTO	23  //自动风量
#define ACTION_WIND_MIN		24	//最小风量
#define ACTION_WIND_MID		25	//中等风量
#define	ACTION_WIND_MAX		26	//最大风量
#define ACTION_WIND_SX		27	//上下扫风
#define	ACTION_WIND_JZ		28	//居中扫风
#define ACTION_WIND_STOP	29	//停止扫风
#define ACTION_WIND_UD		30	//左右扫风
#define ACTION_WIND_UD_STOP		31	//左右扫风


#define BRAND_DEFAULT       0

#define BRAND_GREE			1
#define BRAND_MIDEA			2
#define BRAND_XIAOTIANER    3
#define BRAND_HAIXIN	    4
#define BRAND_KELON	      5
#define BRAND_TCL			6
#define BRAND_HAIER			7
#define BRAND_YUETU 	    8
#define BRAND_RILI	        9
#define BRAND_AUX			10
#define BRAND_ZHIGAO		11
#define BRAND_SONGXIA	   12
#define BRAND_DAJIN	        13
#define BRAND_XIAOMI	    14
#define BRAND_SANSUNG		15
#define BRAND_CHANGHONG	    16

#define BRAND_SANLING		17
#define BRAND_HUALING		18
#define BRAND_KANGJIA		19
#define BRAND_SANLINGDIANJI 20
#define BRAND_GELANSHI      21
#define BRAND_YANGZI		22
#define BRAND_CHUANGWEI	    23
#define BRAND_DONGZHI	    24
#define BRAND_SANYANG	    25
#define BRAND_LG	        26
#define BRAND_YORK			27
#define BRAND_YINGHUA		28
#define BRAND_NEITIAN		29
#define BRAND_XINKE	        30
#define BRAND_HUABAO		31
#define BRAND_CHANGLING	    32
#define BRAND_LESHENG		33
#define BRAND_MAIKEWEIER	34
#define BRAND_FEILIPU		35

#define BRAND_XINFEI	        36
#define BRAND_SHAP		37
#define BRAND_HUIERPU	    38
#define BRAND_CHUNLAN	39

// #define BRAND_SANLING	        40		//BZ
#define BRAND_LEJIN		40//41				//BZ
#define BRAND_AOKEMA	    41//42				//BZ
#define BRAND_FUSHITONG	42//43				//BZ


/*
格力空调
美的空调
小天鹅空调
海信空调
科龙空调
TCL空调
海尔空调
月兔空调
日立空调
奥克斯空调
志高空调
松下空调
大金空调
小米空调
三星空调
长虹空调
三菱重工空调
华凌空调
康佳空调
三菱电机空调
格兰仕空调
扬子空调
创维空调
东芝空调
三洋空调
LG 空调
约克空调
樱花空调
内田空调
新科空调
华宝空调
长岭空调
乐声空调
麦克维尔空调
飞利浦空调
新飞空调
夏普
惠而浦
春兰

乐金空调 是 LG空调	????	改为LG 空调		//BZ		无
澳柯玛			//BZ
富士通			//BZ

*/


typedef enum {
  AC_MODE_INVALID = -1,
  AC_MODE_AUTO,
  AC_MODE_COOL,
  AC_MODE_DRY,
  AC_MODE_FAN,
  AC_MODE_HEAT,
  AC_MODE_MAX
}AC_MODE_TYPE;

typedef enum {
  AC_WIND_INVALID = -1,
  AC_WIND_AUTO,
  AC_WIND_HIGH,
  AC_WIND_MID,
  AC_WIND_LOW,
  AC_WIND_MAX
}AC_WIND_SPEED;

typedef enum {
  AC_TMP_INVALID = 15,
  AC_TMP_16,
  AC_TMP_17,
  AC_TMP_18,
  AC_TMP_19,
  AC_TMP_20,
  AC_TMP_21,
  AC_TMP_22,
  AC_TMP_23,
  AC_TMP_24,
  AC_TMP_25,
  AC_TMP_26,
  AC_TMP_27,
  AC_TMP_28,
  AC_TMP_29,
  AC_TMP_30,
  AC_TMP_MAX
}AC_TMP_NUMBER;




enum AC_KEYNUM{
	ACKEY_ECO=0,	//节能		0
	ACKEY_MODE,	//模式		1
	ACKEY_FASTCOLL,	//快冷		2
	ACKEY_FAN,	//风速		3
	ACKEY_FASTHEAT,	//快热		4
	ACKEY_WIND_UD,	//上下风	5
	ACKEY_FORCE,	//强力		6
	ACKEY_SLEEP,	//睡眠		7
	ACKEY_WIND_LR,	//左右风	8
	ACKEY_ELEC_HEAT,//电辅热	9
	ACKEY_INC,	//温度+		10
	ACKEY_DEC,	//温度-		11
	ACKEY_HEALTH,	//健康
	ACKEY_QUIET,	//静音
	ACKEY_POWER,	//电源
	ACKEY_TRUBO,	//TRUBO
	ACKEY_AIR,	//换气
	ACKEY_SELF_DRY,	//机内干燥
	ACKEY_LIGHT,	//灯光
	ACKEY_TIMER,	//定时
};

enum ACMODE{		//模式
	ACMODE_AUTO=0,	//自动
	ACMODE_COOL,	//制冷
	ACMODE_HUMIDITY,//除湿
	ACMODE_WIND,	//送风
	ACMODE_HEAT,	//制热
};



enum AC_FAN{		//风速
	ACFAN_AUTO=0,	//自动
	ACFAN_HIGH,	//高风
	ACFAN_MID,	//中风
	ACFAN_LOW	//低风
};


#define AC_SWING_UD		0x00		//上下扫风BIT
#define AC_SWING_LR    		0x01		//左右扫风BIT
#define AC_SWING_ON		1		//扫风开
#define AC_SWING_OFF		0		//扫风关

enum AC_UDSF{
	AC_UDSF_OFF=0,	//关闭
	AC_UDSF_AUTO,	//自动上下扫风
	AC_UDSF_1,	//手动上下1档～5档
	AC_UDSF_2,
	AC_UDSF_3,
	AC_UDSF_4,
	AC_UDSF_5,
};

enum AC_LRSF{
	AC_LRSF_OFF=0,	//关闭
	AC_LRSF_AUTO,	//自动左右扫风
	AC_LRSF_1,	//手动上下1档～5档
	AC_LRSF_2,
	AC_LRSF_3,
	AC_LRSF_4,
	AC_LRSF_5,
};


//设定温度  范围  16～30

enum AC_AUTO_TEMP{
	AC_AUTO_TEMP_DEC2=0,		//自动温度  -2
	AC_AUTO_TEMP_DEC1,		//自动温度  -1
	AC_AUTO_TEMP_0,			//自动温度  0
	AC_AUTO_TEMP_ADD1,		//自动温度  +1
	AC_AUTO_TEMP_ADD2		//自动温度  +2
};



//定时 1～8 小时

//================FLAG BIT
#define AC_FLAG_POWER_ON	0x0001		//开关机
#define AC_FLAG_SLEEP_ON	0x0002		//睡眠
#define AC_FLAG_MUTE_ON		0x0004		//静音
#define AC_FLAG_FORCE		0x0008		//强力
#define AC_FLAG_DRY		0x0010		//干燥
#define AC_FLAG_HEALTH		0x0020		//健康
#define AC_FLAG_ECO		0x0040		//节能
#define AC_FLAG_ELECHEAT	0x0080		//电辅热
#define AC_FLAG_AIR		0x0100		//通风
#define AC_FLAG_LIGHT		0x0200		//灯光
#define AC_FLAG_TRUBO		0x0400		//TRUBO
#define AC_FLAG_SLEEP		0x0800		//睡眠

//======================redefine
#define AC_FLAG_SLEEP_SWITCH	AC_FLAG_SLEEP_ON
#define AC_FALG_SLEEP_TIMER		AC_FLAG_SLEEP

#define AC_FLAG_ON		1
#define AC_FALG_OFF		0
//REDEFINE
#define AC_FLAG_OFF		AC_FALG_OFF



/*===========================================
hVer 	//大版本号
mVer 	//主版本号
sVer 	//次版本号
===========================================*/

typedef struct irlib_version{
	uint8_t hVer;
	uint8_t mVer;
	uint8_t sVer;
}IrLibVersion;



#define IR_A_SEND_ARRAY_MAX	1  //每个空调品牌最多发4组
#define IR_A_CARRIOR_MAX		731//500//731 //空调红外码最大载波个数

//一条空调红外码载波
typedef struct ir_a_array
{
	u16  ir_carrior_len;//载波有效长度
	u16 ir_carriors[IR_CARRIOR_MAX];
}s_ir_a_array;

//要发送的多组红外码信息
typedef struct ir_a_send_arrays
{
	unsigned char  ir_array_num;//ir_arrays有效长度
	s_ir_a_array ir_arrays[IR_SEND_ARRAY_MAX];
}s_ir_a_send_arrays;

/*==================================================
函数: getAirConditionerDat: 获取空调发码数据
参数:
	input:  输入参数  空调状态
	cmp_size 输出参数 红外波形个数
	ware: 输出参数 红外波形数据(精度10us)
返回值:
	0: ok
	1：fail
==================================================*/

extern uint8_t getAirConditionerDat(const AcInputParamType* input,uint16_t *cmp_size, uint16_t* ware);

/*==================================================
函数: matchAirConditionerCode: 匹配空调数据
参数:
	wave_buff	输入参数 捕获的红外波形数据(精度40us)
	wave_len:	输入参数 捕获的红外波形个数
	out_code:   输出参数 匹配到的代码
返回值:
	0: ok
	1：fail
==================================================*/

extern uint8_t matchAirConditionerCode(uint16_t *wave_buff,uint16_t wave_len,uint16_t *out_code);


/*==================================================
函数: getIrLibVersion: 获取irlib版本号  （上电必须调用）
参数:
	soft:         输出参数 程序版本号
	lib:          输出参数 红外库版本号
	compile_time: 输出参数 编译时间
返回值:
	0: ok
	1：fail
==================================================*/
extern uint8_t getIrLibVersion(IrLibVersion *soft,IrLibVersion *lib,char **compile_time);




#ifdef __cplusplus
}
#endif

#endif
