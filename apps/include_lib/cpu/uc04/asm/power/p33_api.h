#ifndef __P33_API_H__
#define __P33_API_H__

void adc_pmu_ch_enable();
void adc_pmu_ch_disable();
void adc_pmu_ch_select(u16 ch);


//
//
//					vol
//
//
//
/****************************************************************/
enum {
    DVDD_VOL_SEL_084V = 0,
    DVDD_VOL_SEL_087V,
    DVDD_VOL_SEL_090V,
    DVDD_VOL_SEL_093V,
    DVDD_VOL_SEL_096V,
    DVDD_VOL_SEL_099V,
    DVDD_VOL_SEL_102V,
    DVDD_VOL_SEL_105V,
    DVDD_VOL_SEL_108V,
    DVDD_VOL_SEL_111V,
    DVDD_VOL_SEL_114V,
    DVDD_VOL_SEL_117V,
    DVDD_VOL_SEL_120V,
    DVDD_VOL_SEL_123V,
    DVDD_VOL_SEL_126V,
    DVDD_VOL_SEL_129V,
};


enum {
    DCVDD_VOL_SEL_100V = 0,
    DCVDD_VOL_SEL_105V,
    DCVDD_VOL_SEL_110V,
    DCVDD_VOL_SEL_115V,
    DCVDD_VOL_SEL_120V,
    DCVDD_VOL_SEL_125V,
    DCVDD_VOL_SEL_130V,
    DCVDD_VOL_SEL_135V,
    DCVDD_VOL_SEL_140V,
    DCVDD_VOL_SEL_145V,
    DCVDD_VOL_SEL_150V,
    DCVDD_VOL_SEL_155V,
    DCVDD_VOL_SEL_160V,
};


enum {
    VDDIOM_VOL_21V = 0,
    VDDIOM_VOL_22V,
    VDDIOM_VOL_23V,
    VDDIOM_VOL_24V,
    VDDIOM_VOL_25V,
    VDDIOM_VOL_26V,
    VDDIOM_VOL_27V,
    VDDIOM_VOL_28V,
    VDDIOM_VOL_29V,
    VDDIOM_VOL_30V,
    VDDIOM_VOL_31V,
    VDDIOM_VOL_32V,
    VDDIOM_VOL_33V,
    VDDIOM_VOL_34V,
    VDDIOM_VOL_35V,
    VDDIOM_VOL_36V,
};
enum {
    VDDIOW_VOL_21V = 0,
    VDDIOW_VOL_22V,
    VDDIOW_VOL_23V,
    VDDIOW_VOL_24V,
    VDDIOW_VOL_25V,
    VDDIOW_VOL_26V,
    VDDIOW_VOL_27V,
    VDDIOW_VOL_28V,
    VDDIOW_VOL_29V,
    VDDIOW_VOL_30V,
    VDDIOW_VOL_31V,
    VDDIOW_VOL_32V,
    VDDIOW_VOL_33V,
    VDDIOW_VOL_34V,
    VDDIOW_VOL_35V,
    VDDIOW_VOL_36V,
};

enum {
    WVDD_VOL_050V = 0,
    WVDD_VOL_055V,
    WVDD_VOL_060V,
    WVDD_VOL_065V,
    WVDD_VOL_070V,
    WVDD_VOL_075V,
    WVDD_VOL_080V,
    WVDD_VOL_085V,
    WVDD_VOL_090V,
    WVDD_VOL_095V,
    WVDD_VOL_100V,
    WVDD_VOL_105V,
    WVDD_VOL_110V,
    WVDD_VOL_115V,
    WVDD_VOL_120V,
    WVDD_VOL_125V,
};

void dvdd_vol_sel(u8 vol);
u8 get_dvdd_vol_sel();
void dcvdd_vol_sel(u8 vol);
u8 get_dcvdd_vol_sel();
void vddiom_vol_sel(u8 vol);
u8 get_vddiom_vol_sel();
void vddiow_vol_sel(u8 vol);
u8 get_vddiow_vol_sel();

//
//
//					lvd
//
//
//
/****************************************************************/
typedef enum {
    LVD_RESET_MODE,		//复位模式
    LVD_EXCEPTION_MODE, //异常模式，进入异常中断
    LVD_WAKEUP_MODE,    //唤醒模式，进入唤醒中断，callback参数为回调函数
} LVD_MODE;

typedef enum {
    VLVD_SEL_18V = 0,
    VLVD_SEL_19V,
    VLVD_SEL_20V,
    VLVD_SEL_21V,
    VLVD_SEL_22V,
    VLVD_SEL_23V,
    VLVD_SEL_24V,
    VLVD_SEL_25V,
} LVD_VOL;

#define VLVD_LOWEST_VOL 1800

void lvd_en(u8 en);
void lvd_config(LVD_VOL vol, u8 expin_en, LVD_MODE mode, void (*callback));
u32 get_lvd_en(void);
u32 get_lvd_level(void);
u32 get_lvd_vol(void);


//
//
//				 p33_io
//
//
//
/****************************************************************/
bool is_pinr_en();
u8 get_pinr_port();
bool is_mclr_en();



#endif
