#ifndef _APP_IR_CONFIG_H_
#define _APP_IR_CONFIG_H_

  typedef struct _IR_EMITTER_INFO
  {
  u8  brand;
  u8   pair_flag; // 文件扇区号
  } IR_EMITTER_INFO;

typedef enum {
  IR_IDLE = 0,
  IR_AC_SEND,
  IR_AC_MACH,
  IR_AC_SEARCH,
  IR_AC_BRAND,
  IR_LEARN,
  IR_LEARN_SEND,

}IR_STATUS;

#define  MAX_IR_CARRIOR   208 // 20个按键需要8K VM 区


typedef struct 
{
	u8  ir_carrior_len;//载波有效长度
	u16  ir_carriors[MAX_IR_CARRIOR];
}learn_ir_array;

#define  FAN_OPEN       0
#define  FAN_CLOSE      1
#define  FAN_SWING     2
#define  FAN_SPEED      3
#define  FAN_SPEED_UP  4
#define  FAN_SPEED_DM  5
#define  FAN_SPEED_MODE  6
#define  FAN_TIMER         7
#define  MAX_FAN_KEY  8

#define  TV_OPEN       0
#define  TV_CLOSE      1
#define  TV_CH_UP     2
#define  TV_CH_DM     3
#define  TV_VOL_UP     4
#define  TV_VOL_DOWM     5
#define  TV_MUTE     6
#define  TV_SOURCE    7
#define  MAX_TV_KEY  8


#define  LAMP_OPEN       0
#define  LAMP_CLOSE      1
#define  LAMP_COLOR_TEMP_UP    2
#define  LAMP_COLOR_TEMP_DM   3
#define  LAMP_LIGHT_UP     4
#define  LAMP_LIGHT_DM     5
#define  LAMP_NIGHT_OPEN     6
#define  LAMP_NIGHT_CLOSE    7
#define  MAX_LAMP_KEY  8

#ifndef IR_RECEIVE_IO
#define  MAX_AC_LEARN_IR_NUM   1
#else
#define  MAX_AC_LEARN_IR_NUM   5
#endif
typedef struct _IR_LEARN_INFO
{
u8	ac_num;
u16  ac_brand;
u8  ac_search;

u16	ac_table[MAX_AC_LEARN_IR_NUM];

} IR_LEARN_INFO;


/*=================================
area		区域
device		代码号
keynum  	按键
mode   		模式
fan    		风速
swing   	风向
udsf  		上下扫风
lrsf  		左右扫风
temp    	设定温度
auto_temp   	自动模式温度
timer   	定时
flag		标志
===================================*/

typedef	struct input_param_type{
	uint8_t 	area;
	uint16_t	device;
	uint8_t		keynum;
	uint8_t		mode;
	uint8_t		fan;
	uint8_t		swing;
	uint8_t		udsf;
	uint8_t		lrsf;
	uint8_t		temp;
	uint8_t		auto_temp;
	uint8_t		timer;
	uint16_t	flag;
	uint16_t	device_brk;
	uint8_t		pp_num;
	uint8_t         brand;
	

}AcInputParamType;

#ifdef IR_POWER_PORT

#ifndef IR_POWER_ON_LEVEL
#define  IR_POWER_ON_LEVEL  1
#endif

#ifndef IR_POWER_OFF_LEVEL 
#define  IR_POWER_OFF_LEVEL  0
#endif

#define IR_POWER_ON()	 { gpio_set_die(IR_POWER_PORT,0);gpio_set_direction(IR_POWER_PORT,0);gpio_write(IR_POWER_PORT,IR_POWER_ON_LEVEL);}
 #define IR_POWER_OFF()	{ gpio_set_die(IR_POWER_PORT,0);gpio_set_direction(IR_POWER_PORT,0); gpio_write(IR_POWER_PORT,IR_POWER_OFF_LEVEL);}
#endif

#define IR_SEND_ARRAY_MAX	1  //每个空调品牌最多发4组
#define IR_CARRIOR_MAX		731//500//731 //空调红外码最大载波个数

//一条空调红外码载波
typedef struct ir_array
{
	u16  ir_carrior_len;//载波有效长度
	u16 ir_carriors[IR_CARRIOR_MAX];
}s_ir_array;

//要发送的多组红外码信息
typedef struct ir_send_arrays
{
	unsigned char  ir_array_num;//ir_arrays有效长度
	s_ir_array ir_arrays[IR_SEND_ARRAY_MAX];
}s_ir_send_arrays;


int ir_emitter_handle_action( int  cmd_index);

void clear_ir_learn_data(void);
void ir_emitter_init(void);

 void ir_send_data_handle(s_ir_send_arrays *g_ir_arrays);
 void ir_receive_data_handle(s_ir_send_arrays *g_ir_arrays);
 void ac_ir_send_handle(void);
 void rsp_ac_status(void);
 void  ac_off_timeout(void);
 void  ac_on_timeout(void);
 void ir_emitter(int action);
 bool is_ir_busy(void);
void wati_ir_send_end(void);
int  iremitterMsgProccess(int Msg);
void send_test_data(void);
#endif
