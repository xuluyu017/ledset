@echo off
setlocal enableDelayedExpansion
cd %~dp0
echo f|xcopy /y music_led_app\music_led_board_cfg.h	 music_led_board_cfg.h
.\byle_prbuild_h.exe -E -dM .\byle_cfg.h   > abc.h
 del music_led_board_cfg.h
.\Byle_file_op.exe "/BYLE_KWS_ASR|BYLE_CMD_KWS|BYLE_TTS_APP|Byle_TTS_Lib_Type|KWS_TYPE|kwsB_lib|KWS_AEC_EN|KWS_TONE_EN|fix_tone_file|FIX_KWS_LIB_NAME|Byle_Kws_Lib_Type|REDEFINE_DOWNLOAD|isd_config_redefine|Byle_Authorization_file|KwsA_SDK_API_EN|Fix_Lib_language|Fix_Lib_size/" .\abc.h > .\byle_cfg_temp.h
 del abc.h

 echo byle sdk start
 
 IF EXIST "byle_kws_app\libuni_kws" (
 echo f|xcopy /y byle_kws_app\libuni_kws\libuni_kws.a	     byle_kws_app\byle_kws_KWSA_SDK_lib.a
 echo f|xcopy /y byle_kws_app\libuni_kws\libkws_utils.h	 byle_kws_app\byle_kwsA_SDK_lib.h
 )
 



	 
 for /f "tokens=*" %%i in ('.\Byle_file_op.exe "/Byle_Authorization_file/ {print $3}" .\byle_cfg_temp.h ') do (
      set key_type=%%i
	  set key_type=!key_type:~-4!
	   echo !key_type!
 )
	

for /f "tokens=*" %%i in ('.\Byle_file_op.exe "/Byle_Kws_Lib_Type/ {print $3}" .\byle_cfg_temp.h ') do (
	set find_Lib_name=%%i
	echo !find_Lib_name!
	echo f|xcopy /y byle_kws_app\byle_kws_!find_Lib_name!.a	 byle_kws_app\byle_kws.a
	
 )

 for /f "tokens=*" %%i in ('.\Byle_file_op.exe "/KWS_TYPE/ {print $3}" .\byle_cfg_temp.h ') do (
      set kws_type=%%i
	  echo !kws_type!
	echo f|xcopy /y  byle_kws_app\byle_kws_api_!kws_type!.a    byle_kws_app\byle_kws_api.a
 )

 echo byle sdk ok
 del byle_cfg_temp.h


	
