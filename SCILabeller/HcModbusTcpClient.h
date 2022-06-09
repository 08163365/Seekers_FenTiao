#pragma once
#define  TEMP_STR_LEN 24
enum HUICHUAN_PLC_DATA
{
	HC_PLC_INPUT_ADDR = 20800, //MW Input
	HC_PLC_OUTPUT_ADDR = 19200, //MX Output

	HC_PLC_MACHINE_STATU = 748, //MD 设备状态读取 复位1，待机2，急停3，停止4，自动运行5，维护6,7硬件启动
	HC_PLC_USER_TYPE = 52,//101,//WORD //权限	

	HC_PLC_MACHIINE_START_ENABLE = 22129, //MX 设备是否允许启动
	//设置 MX
	HC_PLC_MANUAL_START = 22110, //+0启动 %MX2762.14
	HC_PLC_MANUAL_RESET = 22111, //+1复位
	HC_PLC_MANUAL_STOP = 22112, //+2停止 MX2764
	HC_PLC_MANUAL_ESTOP = 22113, //+3急停
	HC_PLC_MANUAL_MAINTENANCE = 22114, //+4维护状态
	//HC_PLC_PLCVERSION = 2203, //MD

	/*轴相关PLC地址(数据) MD*/
	//放卷纠偏
	HC_PLC_ONLOAD_TARGETPOS = 200, //目标位置
	HC_PLC_ONLOAD_TARGETSPEED = 201, //目标速度
	HC_PLC_ONLOAD_ACCSPEED = 202, //加速度
	HC_PLC_ONLOAD_DECSPEED = 203, //减速度
	HC_PLC_ONLOAD_HOMEMODEL = 204, //回零模式
	HC_PLC_ONLOAD_HOMEOFFSET = 205,//零点偏移
	HC_PLC_ONLOAD_HOMESPEED = 206, //回零速度
	HC_PLC_ONLOAD_HOMEDECSPEED = 207, //回零低速
	HC_PLC_ONLOAD_HOMEACCSPEED = 208, //回零加速

	//毛刷速度(速度比率)
	HC_PLC_BRUSH1_RATIO = 211, 
	HC_PLC_BRUSH2_RATIO = 221,

	//过程整体纠偏(二级纠偏整体)
	HC_PLC_SECONDEN_TARGETPOS = 230, //目标位置
	HC_PLC_SECONDEN_TARGETSPEED = 231, //目标速度
	HC_PLC_SECONDEN_ACCSPEED = 232, //加速度
	HC_PLC_SECONDEN_DECSPEED = 233, //减速度
	HC_PLC_SECONDEN_HOMEMODEL = 234, //回零模式
	HC_PLC_SECONDEN_HOMEOFFSET = 235,//零点偏移
	HC_PLC_SECONDEN_HOMESPEED = 236, //回零速度
	HC_PLC_SECONDEN_HOMEDECSPEED = 237, //回零低速
	HC_PLC_SECONDEN_HOMEACCSPEED = 238, //回零加速

	//过程纠偏(二级纠偏)
	HC_PLC_SECOND_TARGETPOS = 240, //目标位置
	HC_PLC_SECOND_TARGETSPEED = 241, //目标速度
	HC_PLC_SECOND_ACCSPEED = 242, //加速度
	HC_PLC_SECOND_DECSPEED = 243, //减速度
	HC_PLC_SECOND_HOMEMODEL = 244, //回零模式
	HC_PLC_SECOND_HOMEOFFSET = 245,//零点偏移
	HC_PLC_SECOND_HOMESPEED = 246, //回零速度
	HC_PLC_SECOND_HOMEDECSPEED = 247, //回零低速
	HC_PLC_SECOND_HOMEACCSPEED = 248, //回零加速

	//加强筋上调宽度
	HC_PLC_STRENG_UP_ADJUST_TARGETPOS = 250, //目标位置
	HC_PLC_STRENG_UP_ADJUST_TARGETSPEED = 251, //目标速度
	HC_PLC_STRENG_UP_ADJUST_ACCSPEED = 252, //加速度
	HC_PLC_STRENG_UP_ADJUST_DECSPEED = 253, //减速度
	HC_PLC_STRENG_UP_ADJUST_HOMEMODEL = 254, //回零模式
	HC_PLC_STRENG_UP_ADJUST_HOMEOFFSET = 255,//零点偏移
	HC_PLC_STRENG_UP_ADJUST_HOMESPEED = 256, //回零速度
	HC_PLC_STRENG_UP_ADJUST_HOMEDECSPEED = 257, //回零低速
	HC_PLC_STRENG_UP_ADJUST_HOMEACCSPEED = 258, //回零加速

	//加强筋下调宽度
	HC_PLC_STRENG_DN_ADJUST_TARGETPOS = 260, //目标位置
	HC_PLC_STRENG_DN_ADJUST_TARGETSPEED = 261, //目标速度
	HC_PLC_STRENG_DN_ADJUST_ACCSPEED = 262, //加速度
	HC_PLC_STRENG_DN_ADJUST_DECSPEED = 263, //减速度
	HC_PLC_STRENG_DN_ADJUST_HOMEMODEL = 264, //回零模式
	HC_PLC_STRENG_DN_ADJUST_HOMEOFFSET = 265,//零点偏移
	HC_PLC_STRENG_DN_ADJUST_HOMESPEED = 266, //回零速度
	HC_PLC_STRENG_DN_ADJUST_HOMEDECSPEED = 267, //回零低速
	HC_PLC_STRENG_DN_ADJUST_HOMEACCSPEED = 268, //回零加速

	//左放卷轴
	HC_PLC_ONLOADLEFT_TARGETPOS = 270, //目标位置
	HC_PLC_ONLOADLEFT_TARGETSPEED = 271, //目标速度
	HC_PLC_ONLOADLEFT_ACCSPEED = 272, //加速度
	HC_PLC_ONLOADLEFT_DECSPEED = 273, //减速度

	//右放卷轴
	HC_PLC_ONLOADRIGHT_TARGETPOS = 280, //目标位置
	HC_PLC_ONLOADRIGHT_TARGETSPEED = 281, //目标速度
	HC_PLC_ONLOADRIGHT_ACCSPEED = 282, //加速度
	HC_PLC_ONLOADRIGHT_DECSPEED = 283, //减速度

	//主牵引轴
	HC_PLC_MAINAXIS_TARGETPOS = 290, //目标位置
	HC_PLC_MAINAXIS_SPEED = 291, //速度
	HC_PLC_MAINAXIS_ACCSPEED = 292, //加速度
	HC_PLC_MAINAXIS_DECSPEED = 293, //减速度

	//左推料轴
	HC_PLC_SENDONLOADLEFT_TARGETPOS = 300, //目标位置
	HC_PLC_SENDONLOADLEFT_TARGETSPEED = 301, //目标速度
	HC_PLC_SENDONLOADLEFT_ACCSPEED = 302, //加速度
	HC_PLC_SENDONLOADLEFT_DECSPEED = 303, //减速度
	HC_PLC_SENDONLOADLEFT_HOMEOFFSET = 305, //回零偏移
	HC_PLC_SENDONLOADLEFT_HOMESPEED = 306, //回零速度

	//右推料轴
	HC_PLC_SENDONLOADRIGHT_TARGETPOS = 310, //目标位置
	HC_PLC_SENDONLOADRIGHT_TARGETSPEED = 311, //目标速度
	HC_PLC_SENDONLOADRIGHT_ACCSPEED = 312, //加速度
	HC_PLC_SENDONLOADRIGHT_DECSPEED = 313, //减速度
	HC_PLC_SENDONLOADRIGHT_HOMEOFFSET = 312, //回零偏移
	HC_PLC_SENDONLOADRIGHT_HOMESPEED = 313, //回零速度

	//压平装置
	HC_PLC_PRESSLEVEL_TARGETPOS = 320, //目标位置
	HC_PLC_PRESSLEVEL_TARGETSPEED = 321, //目标速度
	HC_PLC_PRESSLEVEL_ACCSPEED = 322, //加速度
	HC_PLC_PRESSLEVEL_DECSPEED = 323, //减速度
	HC_PLC_PRESSLEVEL_HOMEMODEL = 324, //回零模式
	HC_PLC_PRESSLEVEL_HOMEOFFSET = 325,//零点偏移
	HC_PLC_PRESSLEVEL_HOMESPEED = 326, //回零速度
	HC_PLC_PRESSLEVEL_HOMEDECSPEED = 327, //回零低速
	HC_PLC_PRESSLEVEL_HOMEACCSPEED = 328, //回零加速

	//展平1
	HC_PLC_FLATTEN1_TARGETPOS = 330, //目标位置
	HC_PLC_FLATTEN1_TARGETSPEED = 331, //目标速度
	HC_PLC_FLATTEN1_ACCSPEED = 332, //加速度
	HC_PLC_FLATTEN1_DECSPEED = 333, //减速度
	HC_PLC_FLATTEN1_HOMEMODEL = 334, //回零模式
	HC_PLC_FLATTEN1_HOMEOFFSET = 335,//零点偏移
	HC_PLC_FLATTEN1_HOMESPEED = 336, //回零速度
	HC_PLC_FLATTEN1_HOMEDECSPEED = 337, //回零低速
	HC_PLC_FLATTEN1_HOMEACCSPEED = 338, //回零加速

	//展平2
	HC_PLC_FLATTEN2_TARGETPOS = 340, //目标位置
	HC_PLC_FLATTEN2_TARGETSPEED = 341, //目标速度
	HC_PLC_FLATTEN2_ACCSPEED = 342, //加速度
	HC_PLC_FLATTEN2_DECSPEED = 343, //减速度
	HC_PLC_FLATTEN2_HOMEMODEL = 344, //回零模式
	HC_PLC_FLATTEN2_HOMEOFFSET = 345,//零点偏移
	HC_PLC_FLATTEN2_HOMESPEED = 346, //回零速度
	HC_PLC_FLATTEN2_HOMEDECSPEED = 347, //回零低速
	HC_PLC_FLATTEN2_HOMEACCSPEED = 348, //回零加速

	//收卷1
	HC_PLC_OFFLOAD1_TARGETPOS = 350, //目标位置
	HC_PLC_OFFLOAD1_TARGETSPEED = 351, //目标速度
	HC_PLC_OFFLOAD1_ACCSPEED = 352, //加速度
	HC_PLC_OFFLOAD1_DECSPEED = 353, //减速度
	HC_PLC_OFFLOAD1_HOMEMODEL = 354, //回零模式
	HC_PLC_OFFLOAD1_HOMEOFFSET = 355,//零点偏移
	HC_PLC_OFFLOAD1_HOMESPEED = 356, //回零速度
	HC_PLC_OFFLOAD1_HOMEDECSPEED = 357, //回零低速
	HC_PLC_OFFLOAD1_HOMEACCSPEED = 358, //回零加速

	//收卷2
	HC_PLC_OFFLOAD2_TARGETPOS = 360, //目标位置
	HC_PLC_OFFLOAD2_TARGETSPEED = 361, //目标速度
	HC_PLC_OFFLOAD2_ACCSPEED = 362, //加速度
	HC_PLC_OFFLOAD2_DECSPEED = 363, //减速度
	HC_PLC_OFFLOAD2_HOMEMODEL = 364, //回零模式
	HC_PLC_OFFLOAD2_HOMEOFFSET = 365,//零点偏移
	HC_PLC_OFFLOAD2_HOMESPEED = 366, //回零速度
	HC_PLC_OFFLOAD2_HOMEDECSPEED = 367, //回零低速
	HC_PLC_OFFLOAD2_HOMEACCSPEED = 368, //回零加速

	//收卷推料
	HC_PLC_OFFLOADSEND_TARGETPOS = 370, //目标位置
	HC_PLC_OFFLOADSEND_TARGETSPEED = 371, //目标速度
	HC_PLC_OFFLOADSEND_ACCSPEED = 372, //加速度
	HC_PLC_OFFLOADSEND_DECSPEED = 373, //减速度
	HC_PLC_OFFLOADSEND_HOMEMODEL = 374, //回零模式
	HC_PLC_OFFLOADSEND_HOMEOFFSET = 375,//零点偏移
	HC_PLC_OFFLOADSEND_HOMESPEED = 376, //回零速度
	HC_PLC_OFFLOADSEND_HOMEDECSPEED = 377, //回零低速
	HC_PLC_OFFLOADSEND_HOMEACCSPEED = 378, //回零加速

	//上加强筋
	HC_PLC_UPSTRENG_TARGETPOS = 380, //目标位置
	HC_PLC_UPSTRENG_TARGETSPEED = 381, //目标速度
	HC_PLC_UPSTRENG_ACCSPEED = 382, //加速度
	HC_PLC_UPSTRENG_DECSPEED = 383, //减速度
	HC_PLC_UPSTRENG_HOMEMODEL = 384, //回零模式
	HC_PLC_UPSTRENGD_HOMEOFFSET = 385,//零点偏移
	HC_PLC_UPSTRENG_HOMESPEED = 386, //回零速度
	HC_PLC_UPSTRENG_HOMEDECSPEED = 387, //回零低速
	HC_PLC_UPSTRENG_HOMEACCSPEED = 388, //回零加速

	//下加强筋
	HC_PLC_DNSTRENG_TARGETPOS = 390, //目标位置
	HC_PLC_DNSTRENG_TARGETSPEED = 391, //目标速度
	HC_PLC_DNSTRENG_ACCSPEED = 392, //加速度
	HC_PLC_DNSTRENG_DECSPEED = 393, //减速度
	HC_PLC_DNSTRENG_HOMEMODEL = 394, //回零模式
	HC_PLC_DNSTRENG_HOMEOFFSET = 395,//零点偏移
	HC_PLC_DNSTRENG_HOMESPEED = 396, //回零速度
	HC_PLC_DNSTRENG_HOMEDECSPEED = 397, //回零低速
	HC_PLC_DNSTRENG_HOMEACCSPEED = 398, //回零加速

	//激光1
	HC_PLC_LASER1_TARGETPOS = 400, //目标位置
	HC_PLC_LASER1_TARGETSPEED = 401, //目标速度
	HC_PLC_LASER1_ACCSPEED = 402, //加速度
	HC_PLC_LASER1_DECSPEED = 403, //减速度
	HC_PLC_LASER1_HOMEMODEL = 404, //回零模式
	HC_PLC_LASER1_HOMEOFFSET = 405,//零点偏移
	HC_PLC_LASER1_HOMESPEED = 406, //回零速度
	HC_PLC_LASER1_HOMEDECSPEED = 407, //回零低速
	HC_PLC_LASER1_HOMEACCSPEED = 408, //回零加速

	//激光2
	HC_PLC_LASER2_TARGETPOS = 410, //目标位置
	HC_PLC_LASER2_TARGETSPEED = 411, //目标速度
	HC_PLC_LASER2_ACCSPEED = 412, //加速度
	HC_PLC_LASER2_DECSPEED = 413, //减速度
	HC_PLC_LASER2_HOMEMODEL = 414, //回零模式
	HC_PLC_LASER2_HOMEOFFSET = 415,//零点偏移
	HC_PLC_LASER2_HOMESPEED = 416, //回零速度
	HC_PLC_LASER2_HOMEDECSPEED = 417, //回零低速
	HC_PLC_LASER2_HOMEACCSPEED = 418, //回零加速

	//左电缸
	HC_PLC_ELECTCYLL_TARGETPOS = 420, //目标位置
	HC_PLC_ELECTCYLL_TARGETSPEED = 421, //目标速度
	HC_PLC_ELECTCYLL_ACCSPEED = 422, //加速度
	HC_PLC_ELECTCYLL_DECSPEED = 423, //减速度
	HC_PLC_ELECTCYLL_HOMEMODEL = 424, //回零模式
	HC_PLC_ELECTCYLL_HOMEOFFSET = 425,//零点偏移
	HC_PLC_ELECTCYLL_HOMESPEED = 426, //回零速度
	HC_PLC_ELECTCYLL_HOMEDECSPEED = 427, //回零低速
	HC_PLC_ELECTCYLL_HOMEACCSPEED = 428, //回零加速

	//左电缸
	HC_PLC_ELECTCYLR_TARGETPOS = 430, //目标位置
	HC_PLC_ELECTCYLR_TARGETSPEED = 431, //目标速度
	HC_PLC_ELECTCYLR_ACCSPEED = 432, //加速度
	HC_PLC_ELECTCYLR_DECSPEED = 433, //减速度
	HC_PLC_ELECTCYLR_HOMEMODEL = 434, //回零模式
	HC_PLC_ELECTCYLR_HOMEOFFSET = 435,//零点偏移
	HC_PLC_ELECTCYLR_HOMESPEED = 436, //回零速度
	HC_PLC_ELECTCYLR_HOMEDECSPEED = 437, //回零低速
	HC_PLC_ELECTCYLR_HOMEACCSPEED = 438, //回零加速

	/*轴相关PLC地址(动作) MD/MX*/
	//左放卷
	HC_PLC_ONLOADLEFT_FBPOS = 500, //反馈位置 MD
	HC_PLC_ONLOADLEFT_FBSPEED = 501, // 反馈速度 MD
	HC_PLC_ONLOADLEFT_HOME = 16064, //原点位 MX2008.0
	HC_PLC_ONLOADLEFT_LIMITP = 16064 + 1, //正限位 MX2008.1
	HC_PLC_ONLOADLEFT_LIMITM = 16064 + 2, //负限位 MX2008.2
	HC_PLC_ONLOADLEFT_ENABLELAMP = 16064 + 3, //使能指示灯 MX2008.3
	HC_PLC_ONLOADLEFT_READY = 16064 + 4, //准备好 MX2008.4
	HC_PLC_ONLOADLEFT_INPLACE = 16064 + 5, //到位 MX2008.5
	HC_PLC_ONLOADLEFT_ALARM = 16064 + 6, //报警 MX2008.6
	HC_PLC_ONLOADLEFT_STOP = 16064 + 7, //停止 MX2008.7
	HC_PLC_ONLOADLEFT_ALARMCLEAR = 16064 + 8, //报警清除 MX2008.8
	HC_PLC_ONLOADLEFT_ENABLE = 16064 + 9, //使能 MX2008.9
	HC_PLC_ONLOADLEFT_MOVE = 16064 + 10, //移动 MX2008.10
	HC_PLC_ONLOADLEFT_MOVEHOME = 16064 + 11, //归零 MX2008.11
	HC_PLC_ONLOADLEFT_ADVANCE = 16064 + 12, //前进 MX2008.12
	HC_PLC_ONLOADLEFT_BACK = 16064 + 13, //后退 MX2008.13
	HC_PLC_ONLOADLEFT_CTRL = 502,

	//右放卷
	HC_PLC_ONLOADRIGHT_FBPOS = 503, //反馈位置 MD
	HC_PLC_ONLOADRIGHT_FBSPEED = 504, // 反馈速度 MD
	HC_PLC_ONLOADRIGHT_HOME = 16160, //原点位
	HC_PLC_ONLOADRIGHT_LIMITP = 16160 + 1, //正限位 
	HC_PLC_ONLOADRIGHT_LIMITM = 16160 + 2, //负限位 
	HC_PLC_ONLOADRIGHT_ENABLELAMP = 16160 + 3, //使能指示灯 
	HC_PLC_ONLOADRIGHT_READY = 16160 + 4, //准备好 
	HC_PLC_ONLOADRIGHT_INPLACE = 16160 + 5, //到位
	HC_PLC_ONLOADRIGHT_ALARM = 16160 + 6, //报警 
	HC_PLC_ONLOADRIGHT_STOP = 16160 + 7, //停止
	HC_PLC_ONLOADRIGHT_ALARMCLEAR = 16160 + 8, //报警清除 
	HC_PLC_ONLOADRIGHT_ENABLE = 16160 + 9, //使能 
	HC_PLC_ONLOADRIGHT_MOVE = 16160 + 10, //移动
	HC_PLC_ONLOADRIGHT_MOVEHOME = 16160 + 11, //归零
	HC_PLC_ONLOADRIGHT_ADVANCE = 16160 + 12, //前进
	HC_PLC_ONLOADRIGHT_BACK = 16160 + 13, //后退
	HC_PLC_ONLOADRIGHT_CTRL = 505,

	//主牵引
	HC_PLC_MAINAXIS_FBPOS = 506, //反馈位置 MD
	HC_PLC_MAINAXIS_FBSPEED = 507, // 反馈速度 MD
	HC_PLC_MAINAXIS_HOME = 16256, //原点位
	HC_PLC_MAINAXIS_LIMITP = 16256 + 1, //正限位 
	HC_PLC_MAINAXIS_LIMITM = 16256 + 2, //负限位 
	HC_PLC_MAINAXIS_ENABLELAMP = 16256 + 3, //使能指示灯 
	HC_PLC_MAINAXIS_READY = 16256 + 4, //准备好 
	HC_PLC_MAINAXIS_INPLACE = 16256 + 5, //到位
	HC_PLC_MAINAXIS_ALARM = 16256 + 6, //报警 
	HC_PLC_MAINAXIS_STOP = 16256 + 7, //停止
	HC_PLC_MAINAXIS_ALARMCLEAR = 16256 + 8, //报警清除 
	HC_PLC_MAINAXIS_ENABLE = 16256 + 9, //使能 
	HC_PLC_MAINAXIS_MOVE = 16256 + 10, //移动
	HC_PLC_MAINAXIS_MOVEHOME = 16256 + 11, //归零
	HC_PLC_MAINAXIS_ADVANCE = 16256 + 12, //前进
	HC_PLC_MAINAXIS_BACK = 16256 + 13, //后退
	HC_PLC_MAINAXIS_CTRL = 508,

	//左推料
	HC_PLC_SENDONLOADLEFT_FBPOS = 509, //反馈位置 MD
	HC_PLC_SENDONLOADLEFT_FBSPEED = 510, // 反馈速度 MD
	HC_PLC_SENDONLOADLEFT_HOME = 16352, //原点位
	HC_PLC_SENDONLOADLEFT_LIMITP = 16352 + 1, //正限位 
	HC_PLC_SENDONLOADLEFT_LIMITM = 16352 + 2, //负限位 
	HC_PLC_SENDONLOADLEFT_ENABLELAMP = 16352 + 3, //使能指示灯 
	HC_PLC_SENDONLOADLEFT_READY = 16352 + 4, //准备好 
	HC_PLC_SENDONLOADLEFT_INPLACE = 16352 + 5, //到位
	HC_PLC_SENDONLOADLEFT_ALARM = 16352 + 6, //报警 
	HC_PLC_SENDONLOADLEFT_STOP = 16352 + 7, //停止
	HC_PLC_SENDONLOADLEFT_ALARMCLEAR = 16352 + 8, //报警清除 
	HC_PLC_SENDONLOADLEFT_ENABLE = 16352 + 9, //使能 
	HC_PLC_SENDONLOADLEFT_MOVE = 16352 + 10, //移动
	HC_PLC_SENDONLOADLEFT_MOVEHOME = 16352 + 11, //归零
	HC_PLC_SENDONLOADLEFT_ADVANCE = 16352 + 12, //前进
	HC_PLC_SENDONLOADLEFT_BACK = 16352 + 13, //后退
	HC_PLC_SENDONLOADLEFT_CTRL = 511,

	//右推料
	HC_PLC_SENDONLOADRIGHT_FBPOS = 514, //反馈位置 MD
	HC_PLC_SENDONLOADRIGHT_FBSPEED = 515, // 反馈速度 MD
	HC_PLC_SENDONLOADRIGHT_HOME = 16512, //原点位
	HC_PLC_SENDONLOADRIGHT_LIMITP = 16512 + 1, //正限位 
	HC_PLC_SENDONLOADRIGHT_LIMITM = 16512 + 2, //负限位 
	HC_PLC_SENDONLOADRIGHT_ENABLELAMP = 16512 + 3, //使能指示灯 
	HC_PLC_SENDONLOADRIGHT_READY = 16512 + 4, //准备好 
	HC_PLC_SENDONLOADRIGHT_INPLACE = 16512 + 5, //到位
	HC_PLC_SENDONLOADRIGHT_ALARM = 16512 + 6, //报警 
	HC_PLC_SENDONLOADRIGHT_STOP = 16512 + 7, //停止
	HC_PLC_SENDONLOADRIGHT_ALARMCLEAR = 16512 + 8, //报警清除 
	HC_PLC_SENDONLOADRIGHT_ENABLE = 16512 + 9, //使能 
	HC_PLC_SENDONLOADRIGHT_MOVE = 16512 + 10, //移动
	HC_PLC_SENDONLOADRIGHT_MOVEHOME = 16512 + 11, //归零
	HC_PLC_SENDONLOADRIGHT_ADVANCE = 16512 + 12, //前进
	HC_PLC_SENDONLOADRIGHT_BACK = 16512 + 13, //后退
	HC_PLC_SENDONLOADRIGHT_CTRL = 516,

	//压平
	HC_PLC_PRESSLEVEL_FBPOS = 517, //反馈位置 MD
	HC_PLC_PRESSLEVEL_FBSPEED = 518, // 反馈速度 MD
	HC_PLC_PRESSLEVEL_HOME = 16608, //原点位
	HC_PLC_PRESSLEVEL_LIMITP = 16608 + 1, //正限位 
	HC_PLC_PRESSLEVEL_LIMITM = 16608 + 2, //负限位 
	HC_PLC_PRESSLEVEL_ENABLELAMP = 16608 + 3, //使能指示灯 
	HC_PLC_PRESSLEVEL_READY = 16608 + 4, //准备好 
	HC_PLC_PRESSLEVEL_INPLACE = 16608 + 5, //到位
	HC_PLC_PRESSLEVEL_ALARM = 16608 + 6, //报警 
	HC_PLC_PRESSLEVEL_STOP = 16608 + 7, //停止
	HC_PLC_PRESSLEVEL_ALARMCLEAR = 16608 + 8, //报警清除 
	HC_PLC_PRESSLEVEL_ENABLE = 16608 + 9, //使能 
	HC_PLC_PRESSLEVEL_MOVE = 16608 + 10, //移动
	HC_PLC_PRESSLEVEL_MOVEHOME = 16608 + 11, //归零
	HC_PLC_PRESSLEVEL_ADVANCE = 16608 + 12, //前进
	HC_PLC_PRESSLEVEL_BACK = 16608 + 13, //后退
	HC_PLC_PRESSLEVEL_CTRL = 519,

	//展平1
	HC_PLC_FLATTEN1_FBPOS = 520, //反馈位置 MD
	HC_PLC_FLATTEN1_FBSPEED = 521, // 反馈速度 MD
	HC_PLC_FLATTEN1_HOME = 16704, //原点位 
	HC_PLC_FLATTEN1_LIMITP = 16704 + 1, //正限位 
	HC_PLC_FLATTEN1_LIMITM = 16704 + 2, //负限位 
	HC_PLC_FLATTEN1_ENABLELAMP = 16704 + 3, //使能指示灯 
	HC_PLC_FLATTEN1_READY = 16704 + 4, //准备好 
	HC_PLC_FLATTEN1_INPLACE = 16704 + 5, //到位
	HC_PLC_FLATTEN1_ALARM = 16704 + 6, //报警 
	HC_PLC_FLATTEN1_STOP = 16704 + 7, //停止
	HC_PLC_FLATTEN1_ALARMCLEAR = 16704 + 8, //报警清除 
	HC_PLC_FLATTEN1_ENABLE = 16704 + 9, //使能 
	HC_PLC_FLATTEN1_MOVE = 16704 + 10, //移动
	HC_PLC_FLATTEN1_MOVEHOME = 16704 + 11, //归零
	HC_PLC_FLATTEN1_ADVANCE = 16704 + 12, //前进
	HC_PLC_FLATTEN1_BACK = 16704 + 13, //后退
	HC_PLC_FLATTEN1_CTRL = 522,

	//展平2
	HC_PLC_FLATTEN2_FBPOS = 523, //反馈位置 MD
	HC_PLC_FLATTEN2_FBSPEED = 524, // 反馈速度 MD
	HC_PLC_FLATTEN2_HOME = 16800, //原点位 
	HC_PLC_FLATTEN2_LIMITP = 16800 + 1, //正限位 
	HC_PLC_FLATTEN2_LIMITM = 16800 + 2, //负限位 
	HC_PLC_FLATTEN2_ENABLELAMP = 16800 + 3, //使能指示灯 
	HC_PLC_FLATTEN2_READY = 16800 + 4, //准备好 
	HC_PLC_FLATTEN2_INPLACE = 16800 + 5, //到位
	HC_PLC_FLATTEN2_ALARM = 16800 + 6, //报警 
	HC_PLC_FLATTEN2_STOP = 16800 + 7, //停止
	HC_PLC_FLATTEN2_ALARMCLEAR = 16800 + 8, //报警清除 
	HC_PLC_FLATTEN2_ENABLE = 16800 + 9, //使能 
	HC_PLC_FLATTEN2_MOVE = 16800 + 10, //移动
	HC_PLC_FLATTEN2_MOVEHOME = 16800 + 11, //归零
	HC_PLC_FLATTEN2_ADVANCE = 16800 + 12, //前进
	HC_PLC_FLATTEN2_BACK = 16800 + 13, //后退
	HC_PLC_FLATTEN2_CTRL = 525,

	//收卷1
	HC_PLC_OFFLOAD1_FBPOS = 526, //反馈位置 MD
	HC_PLC_OFFLOAD1_FBSPEED = 527, // 反馈速度 MD
	HC_PLC_OFFLOAD1_HOME = 16896, //原点位 
	HC_PLC_OFFLOAD1_LIMITP = 16896 + 1, //正限位 
	HC_PLC_OFFLOAD1_LIMITM = 16896 + 2, //负限位 
	HC_PLC_OFFLOAD1_ENABLELAMP = 16896 + 3, //使能指示灯 
	HC_PLC_OFFLOAD1_READY = 16896 + 4, //准备好 
	HC_PLC_OFFLOAD1_INPLACE = 16896 + 5, //到位
	HC_PLC_OFFLOAD1_ALARM = 16896 + 6, //报警 
	HC_PLC_OFFLOAD1_STOP = 16896 + 7, //停止
	HC_PLC_OFFLOAD1_ALARMCLEAR = 16896 + 8, //报警清除 
	HC_PLC_OFFLOAD1_ENABLE = 16896 + 9, //使能 
	HC_PLC_OFFLOAD1_MOVE = 16896 + 10, //移动
	HC_PLC_OFFLOAD1_MOVEHOME = 16896 + 11, //归零
	HC_PLC_OFFLOAD1_ADVANCE = 16896 + 12, //前进
	HC_PLC_OFFLOAD1_BACK = 16896 + 13, //后退
	HC_PLC_OFFLOAD1_CTRL = 528,

	//收卷2
	HC_PLC_OFFLOAD2_FBPOS = 529, //反馈位置 MD
	HC_PLC_OFFLOAD2_FBSPEED = 530, // 反馈速度 MD
	HC_PLC_OFFLOAD2_HOME = 16992, //原点位 
	HC_PLC_OFFLOAD2_LIMITP = 16992 + 1, //正限位 
	HC_PLC_OFFLOAD2_LIMITM = 16992 + 2, //负限位 
	HC_PLC_OFFLOAD2_ENABLELAMP = 16992 + 3, //使能指示灯 
	HC_PLC_OFFLOAD2_READY = 16992 + 4, //准备好 
	HC_PLC_OFFLOAD2_INPLACE = 16992 + 5, //到位
	HC_PLC_OFFLOAD2_ALARM = 16992 + 6, //报警 
	HC_PLC_OFFLOAD2_STOP = 16992 + 7, //停止
	HC_PLC_OFFLOAD2_ALARMCLEAR = 16992 + 8, //报警清除 
	HC_PLC_OFFLOAD2_ENABLE = 16992 + 9, //使能 
	HC_PLC_OFFLOAD2_MOVE = 16992 + 10, //移动
	HC_PLC_OFFLOAD2_MOVEHOME = 16992 + 11, //归零
	HC_PLC_OFFLOAD2_ADVANCE = 16992 + 12, //前进
	HC_PLC_OFFLOAD2_BACK = 16992 + 13, //后退
	HC_PLC_OFFLOAD2_CTRL = 531,
	

	//收卷推料
	HC_PLC_OFFLOADSEND_FBPOS = 532, //反馈位置 MD
	HC_PLC_OFFLOADSEND_FBSPEED = 533, // 反馈速度 MD
	HC_PLC_OFFLOADSEND_HOME = 17088, //原点位
	HC_PLC_OFFLOADSEND_LIMITP = 17088 + 1, //正限位 
	HC_PLC_OFFLOADSEND_LIMITM = 17088 + 2, //负限位 
	HC_PLC_OFFLOADSEND_ENABLELAMP = 17088 + 3, //使能指示灯 
	HC_PLC_OFFLOADSEND_READY = 17088 + 4, //准备好 
	HC_PLC_OFFLOADSEND_INPLACE = 17088 + 5, //到位
	HC_PLC_OFFLOADSEND_ALARM = 17088 + 6, //报警 
	HC_PLC_OFFLOADSEND_STOP = 17088 + 7, //停止
	HC_PLC_OFFLOADSEND_ALARMCLEAR = 17088 + 8, //报警清除 
	HC_PLC_OFFLOADSEND_ENABLE = 17088 + 9, //使能 
	HC_PLC_OFFLOADSEND_MOVE = 17088 + 10, //移动
	HC_PLC_OFFLOADSEND_MOVEHOME = 17088 + 11, //归零
	HC_PLC_OFFLOADSEND_ADVANCE = 17088 + 12, //前进
	HC_PLC_OFFLOADSEND_BACK = 17088 + 13, //后退
	JC_PLC_OFFLOADSEND_CTRL = 534,

	//上加强筋
	HC_PLC_UPSTRENG_FBPOS = 535, //反馈位置 MD
	HC_PLC_UPSTRENG_FBSPEED = 536, // 反馈速度 MD
	HC_PLC_UPSTRENG_HOME = 17184, //原点位 
	HC_PLC_UPSTRENG_LIMITP = 17184 + 1, //正限位 
	HC_PLC_UPSTRENG_LIMITM = 17184 + 2, //负限位 
	HC_PLC_UPSTRENG_ENABLELAMP = 17184 + 3, //使能指示灯 
	HC_PLC_UPSTRENG_READY = 17184 + 4, //准备好 
	HC_PLC_UPSTRENG_INPLACE = 17184 + 5, //到位
	HC_PLC_UPSTRENG_ALARM = 17184 + 6, //报警 
	HC_PLC_UPSTRENG_STOP = 17184 + 7, //停止
	HC_PLC_UPSTRENG_ALARMCLEAR = 17184 + 8, //报警清除 
	HC_PLC_UPSTRENG_ENABLE = 17184 + 9, //使能 
	HC_PLC_UPSTRENG_MOVE = 17184 + 10, //移动
	HC_PLC_UPSTRENG_MOVEHOME = 17184 + 11, //归零
	HC_PLC_UPSTRENG_ADVANCE = 17184 + 12, //前进
	HC_PLC_UPSTRENG_BACK = 17184 + 13, //后退
	HC_PLC_UPSTRENG_CTRL = 537,

	//下加强筋
	HC_PLC_DNSTRENG_FBPOS = 538, //反馈位置 MD
	HC_PLC_DNSTRENG_FBSPEED = 539, // 反馈速度 MD
	HC_PLC_DNSTRENG_HOME = 17280, //原点位 
	HC_PLC_DNSTRENG_LIMITP = 17280 + 1, //正限位 
	HC_PLC_DNSTRENG_LIMITM = 17280 + 2, //负限位 
	HC_PLC_DNSTRENG_ENABLELAMP = 17280 + 3, //使能指示灯 
	HC_PLC_DNSTRENG_READY = 17280 + 4, //准备好 
	HC_PLC_DNSTRENG_INPLACE = 17280 + 5, //到位
	HC_PLC_DNSTRENG_ALARM = 17280 + 6, //报警 
	HC_PLC_DNSTRENG_STOP = 17280 + 7, //停止
	HC_PLC_DNSTRENG_ALARMCLEAR = 17280 + 8, //报警清除 
	HC_PLC_DNSTRENG_ENABLE = 17280 + 9, //使能 
	HC_PLC_DNSTRENG_MOVE = 17280 + 10, //移动
	HC_PLC_DNSTRENG_MOVEHOME = 17280 + 11, //归零
	HC_PLC_DNSTRENG_ADVANCE = 17280 + 12, //前进
	HC_PLC_DNSTRENG_BACK = 17280 + 13, //后退
	HC_PLC_DESTRENG_CTRL = 540,

	//激光1
	HC_PLC_LASER1_FBPOS = 541, //反馈位置 MD
	HC_PLC_LASER1_FBSPEED = 542, // 反馈速度 MD
	HC_PLC_LASER1_HOME = 17376, //原点位 
	HC_PLC_LASER1_LIMITP = 17376 + 1, //正限位 
	HC_PLC_LASER1_LIMITM = 17376 + 2, //负限位 
	HC_PLC_LASER1_ENABLELAMP = 17376 + 3, //使能指示灯 
	HC_PLC_LASER1_READY = 17376 + 4, //准备好 
	HC_PLC_LASER1_INPLACE = 17376 + 5, //到位
	HC_PLC_LASER1_ALARM = 17376 + 6, //报警 
	HC_PLC_LASER1_STOP = 17376 + 7, //停止
	HC_PLC_LASER1_ALARMCLEAR = 17376 + 8, //报警清除 
	HC_PLC_LASER1_ENABLE = 17376 + 9, //使能 
	HC_PLC_LASER1_MOVE = 17376 + 10, //移动
	HC_PLC_LASER1_MOVEHOME = 17376 + 11, //归零
	HC_PLC_LASER1_ADVANCE = 17376 + 12, //前进
	HC_PLC_LASER1_BACK = 17376 + 13, //后退
	HC_PLC_LASER1_CTRL = 543,

	//激光2
	HC_PLC_LASER2_FBPOS = 544, //反馈位置 MD
	HC_PLC_LASER2_FBSPEED = 545, // 反馈速度 MD
	HC_PLC_LASER2_HOME = 17472, //原点位 
	HC_PLC_LASER2_LIMITP = 17472 + 1, //正限位 
	HC_PLC_LASER2_LIMITM = 17472 + 2, //负限位 
	HC_PLC_LASER2_ENABLELAMP = 17472 + 3, //使能指示灯 
	HC_PLC_LASER2_READY = 17472 + 4, //准备好 
	HC_PLC_LASER2_INPLACE = 17472 + 5, //到位
	HC_PLC_LASER2_ALARM = 17472 + 6, //报警 
	HC_PLC_LASER2_STOP = 17472 + 7, //停止
	HC_PLC_LASER2_ALARMCLEAR = 17472 + 8, //报警清除 
	HC_PLC_LASER2_ENABLE = 17472 + 9, //使能 
	HC_PLC_LASER2_MOVE = 17472 + 10, //移动
	HC_PLC_LASER2_MOVEHOME = 17472 + 11, //归零
	HC_PLC_LASER2_ADVANCE = 17472 + 12, //前进
	HC_PLC_LASER2_BACK = 17472 + 13, //后退
	HC_PLC_LASER2_CTRL = 546,

	//放卷纠偏
	HC_PLC_ONLOAD_FBPOS = 547, //反馈位置 MD
	HC_PLC_ONLOAD_FBSPEED = 548, // 反馈速度 MD
	HC_PLC_ONLOAD_HOME = 17568, //原点位 
	HC_PLC_ONLOAD_LIMITP = 17568 + 1, //正限位 
	HC_PLC_ONLOAD_LIMITM = 17568 + 2, //负限位 
	HC_PLC_ONLOAD_ENABLELAMP = 17568 + 3, //使能指示灯 
	HC_PLC_ONLOAD_READY = 17568 + 4, //准备好 
	HC_PLC_ONLOAD_INPLACE = 17568 + 5, //到位
	HC_PLC_ONLOAD_ALARM = 17568 + 6, //报警 
	HC_PLC_ONLOAD_STOP = 17568 + 7, //停止
	HC_PLC_ONLOAD_ALARMCLEAR = 17568 + 8, //报警清除 
	HC_PLC_ONLOAD_ENABLE = 17568 + 9, //使能 
	HC_PLC_ONLOAD_MOVE = 17568 + 10, //移动
	HC_PLC_ONLOAD_MOVEHOME = 17568 + 11, //归零
	HC_PLC_ONLOAD_ADVANCE = 17568 + 12, //前进
	HC_PLC_ONLOAD_BACK = 17568 + 13, //后退
	HC_PLC_ONLOAD_CTRL = 549,

	//毛刷1反馈速度
	HC_PLC_BRUSH1_FBSPEED = 551, //MD
	HC_PLC_BRUSH1_STOP = 17671, //停止 
	HC_PLC_BRUSH1_ENABLE = 17673, //MX 使能
	//HC_PLC_BRUSH1_CTRL = 17673,

	//毛刷2反馈速度
	HC_PLC_BRUSH2_FBSPEED = 554, //MD
	HC_PLC_BRUSH2_STOP = 17767, //停止
	HC_PLC_BRUSH2_ENABLE = 17769, //MX 使能
	//HC_PLC_BRUSH2_CTRL = 17769,

	//过程整体纠偏(二级纠偏整体)
	HC_PLC_SECONDEN_FBPOS = 556, //反馈位置 MD
	HC_PLC_SECONDEN_FBSPEED = 557, // 反馈速度 MD
	HC_PLC_SECONDEN_HOME = 17856, //原点位 
	HC_PLC_SECONDEN_LIMITP = 17856 + 1, //正限位 
	HC_PLC_SECONDEN_LIMITM = 17856 + 2, //负限位 
	HC_PLC_SECONDEN_ENABLELAMP = 17856 + 3, //使能指示灯 
	HC_PLC_SECONDEN_READY = 17856 + 4, //准备好 
	HC_PLC_SECONDEN_INPLACE = 17856 + 5, //到位
	HC_PLC_SECONDEN_ALARM = 17856 + 6, //报警 
	HC_PLC_SECONDEN_STOP = 17856 + 7, //停止
	HC_PLC_SECONDEN_ALARMCLEAR = 17856 + 8, //报警清除 
	HC_PLC_SECONDEN_ENABLE = 17856 + 9, //使能 
	HC_PLC_SECONDEN_MOVE = 17856 + 10, //移动
	HC_PLC_SECONDEN_MOVEHOME = 17856 + 11, //归零
	HC_PLC_SECONDEN_ADVANCE = 17856 + 12, //前进
	HC_PLC_SECONDEN_BACK = 17856 + 13, //后退
	HC_PLC_SECONDEN_CTRL = 558,

	//过程纠偏(二级纠偏)
	HC_PLC_SECOND_FBPOS = 559, //反馈位置 MD
	HC_PLC_SECOND_FBSPEED = 560, // 反馈速度 MD
	HC_PLC_SECOND_HOME = 17952, //原点位 
	HC_PLC_SECOND_LIMITP = 17952 + 1, //正限位 
	HC_PLC_SECOND_LIMITM = 17952 + 2, //负限位 
	HC_PLC_SECOND_ENABLELAMP = 17952 + 3, //使能指示灯 
	HC_PLC_SECOND_READY = 17952 + 4, //准备好 
	HC_PLC_SECOND_INPLACE = 17952 + 5, //到位
	HC_PLC_SECOND_ALARM = 17952 + 6, //报警 
	HC_PLC_SECOND_STOP = 17952 + 7, //停止
	HC_PLC_SECOND_ALARMCLEAR = 17952 + 8, //报警清除 
	HC_PLC_SECOND_ENABLE = 17952 + 9, //使能 
	HC_PLC_SECOND_MOVE = 17952 + 10, //移动
	HC_PLC_SECOND_MOVEHOME = 17952 + 11, //归零
	HC_PLC_SECOND_ADVANCE = 17952 + 12, //前进
	HC_PLC_SECOND_BACK = 17952 + 13, //后退
	HC_PLC_SECOND_CTRL = 561,

	//上加强筋调宽
	HC_PLC_STRENG_UP_ADJUST_FBPOS = 562, //反馈位置 MD
	HC_PLC_STRENG_UP_ADJUST_FBSPEED = 563, // 反馈速度 MD
	HC_PLC_STRENG_UP_ADJUST_HOME = 18048, //原点位 
	HC_PLC_STRENG_UP_ADJUST_LIMITP = 18048 + 1, //正限位 
	HC_PLC_STRENG_UP_ADJUST_LIMITM = 18048 + 2, //负限位 
	HC_PLC_STRENG_UP_ADJUST_ENABLELAMP = 18048 + 3, //使能指示灯 
	HC_PLC_STRENG_UP_ADJUST_READY = 18048 + 4, //准备好 
	HC_PLC_STRENG_UP_ADJUST_INPLACE = 18048 + 5, //到位
	HC_PLC_STRENG_UP_ADJUST_ALARM = 18048 + 6, //报警 
	HC_PLC_STRENG_UP_ADJUST_STOP = 18048 + 7, //停止
	HC_PLC_STRENG_UP_ADJUST_ALARMCLEAR = 18048 + 8, //报警清除 
	HC_PLC_STRENG_UP_ADJUST_ENABLE = 18048 + 9, //使能 
	HC_PLC_STRENG_UP_ADJUST_MOVE = 18048 + 10, //移动
	HC_PLC_STRENG_UP_ADJUST_MOVEHOME = 18048 + 11, //归零
	HC_PLC_STRENG_UP_ADJUST_ADVANCE = 18048 + 12, //前进
	HC_PLC_STRENG_UP_ADJUST_BACK = 18048 + 13, //后退
	HC_PLC_STRENG_UP_ADJUST_CTRL = 564,

	//下加强筋调宽
	HC_PLC_STRENG_DN_ADJUST_FBPOS = 565, //反馈位置 MD
	HC_PLC_STRENG_DN_ADJUST_FBSPEED = 566, // 反馈速度 MD
	HC_PLC_STRENG_DN_ADJUST_HOME = 18144, //原点位 
	HC_PLC_STRENG_DN_ADJUST_LIMITP = 18144 + 1, //正限位 
	HC_PLC_STRENG_DN_ADJUST_LIMITM = 18144 + 2, //负限位 
	HC_PLC_STRENG_DN_ADJUST_ENABLELAMP = 18144 + 3, //使能指示灯 
	HC_PLC_STRENG_DN_ADJUST_READY = 18144 + 4, //准备好 
	HC_PLC_STRENG_DN_ADJUST_INPLACE = 18144 + 5, //到位
	HC_PLC_STRENG_DN_ADJUST_ALARM = 18144 + 6, //报警 
	HC_PLC_STRENG_DN_ADJUST_STOP = 18144 + 7, //停止
	HC_PLC_STRENG_DN_ADJUST_ALARMCLEAR = 18144 + 8, //报警清除 
	HC_PLC_STRENG_DN_ADJUST_ENABLE = 18144 + 9, //使能 
	HC_PLC_STRENG_DN_ADJUST_MOVE = 18144 + 10, //移动
	HC_PLC_STRENG_DN_ADJUST_MOVEHOME = 18144 + 11, //归零
	HC_PLC_STRENG_DN_ADJUST_ADVANCE = 18144 + 12, //前进
	HC_PLC_STRENG_DN_ADJUST_BACK = 18144 + 13, //后退
	HC_PLC_STRENG_DN_ADJUST_CTRL = 567,

	//左电缸electricCly
	HC_PLC_ELECTCYLL_FBPOS = 568, //反馈位置 MD
	HC_PLC_ELECTCYLL_FBSPEED = 569, // 反馈速度 MD
	HC_PLC_ELECTCYLL_HOME = 18240, //原点位 
	HC_PLC_ELECTCYLL_LIMITP = 18240 + 1, //正限位 
	HC_PLC_ELECTCYLL_LIMITM = 18240 + 2, //负限位 
	HC_PLC_ELECTCYLL_ENABLELAMP = 18240 + 3, //使能指示灯 
	HC_PLC_ELECTCYLL_READY = 18240 + 4, //准备好 
	HC_PLC_ELECTCYLL_INPLACE = 18240 + 5, //到位
	HC_PLC_ELECTCYLL_ALARM = 18240 + 6, //报警 
	HC_PLC_ELECTCYLL_STOP = 18240 + 7, //停止
	HC_PLC_ELECTCYLL_ALARMCLEAR = 18240 + 8, //报警清除 
	HC_PLC_ELECTCYLL_ENABLE = 18240 + 9, //使能 
	HC_PLC_ELECTCYLL_MOVE = 18240 + 10, //移动
	HC_PLC_ELECTCYLL_MOVEHOME = 18240 + 11, //归零
	HC_PLC_ELECTCYLL_ADVANCE = 18240 + 12, //前进
	HC_PLC_ELECTCYLL_BACK = 18240 + 13, //后退
	HC_PLC_ELECTCYLL_CTRL = 570, //Test

	//右电缸electricCyl
	HC_PLC_ELECTCYLR_FBPOS = 571, //反馈位置 MD
	HC_PLC_ELECTCYLR_FBSPEED = 572, // 反馈速度 MD
	HC_PLC_ELECTCYLR_HOME = 18336, //原点位 
	HC_PLC_ELECTCYLR_LIMITP = 18336 + 1, //正限位 
	HC_PLC_ELECTCYLR_LIMITM = 18336 + 2, //负限位 
	HC_PLC_ELECTCYLR_ENABLELAMP = 18336 + 3, //使能指示灯 
	HC_PLC_ELECTCYLR_READY = 18336 + 4, //准备好 
	HC_PLC_ELECTCYLR_INPLACE = 18336 + 5, //到位
	HC_PLC_ELECTCYLR_ALARM = 18336 + 6, //报警 
	HC_PLC_ELECTCYLR_STOP = 18336 + 7, //停止
	HC_PLC_ELECTCYLR_ALARMCLEAR = 18336 + 8, //报警清除 
	HC_PLC_ELECTCYLR_ENABLE = 18336 + 9, //使能 
	HC_PLC_ELECTCYLR_MOVE = 18336 + 10, //移动
	HC_PLC_ELECTCYLR_MOVEHOME = 18336 + 11, //归零
	HC_PLC_ELECTCYLR_ADVANCE = 18336 + 12, //前进
	HC_PLC_ELECTCYLR_BACK = 18336 + 13, //后退
	HC_PLC_ELECTCYLR_CTRL = 573, //Test

	/*IO输入输出点在IOConfig中配置*/

	/*功能选择/指示状态*///MD
	HC_PLC_RUNMODEL = 22080,//手自动切换 ,0手动，1自动
	HC_PLC_RUNDIRFORWARD = 22080 + 1,//正向运行
	HC_PLC_RUNDIRBACK = 22080 + 2, //反向运行
	//HC_PLC_ONLOADDIRSEL = 22080 + 3, //放卷方向选择(顺逆时钟)
	HC_PLC_ONLOADSTATU = 22080 + 4, //放卷纠偏工作状态指示
	HC_PLC_ONLOAD_LORR_STATU = 20805,//22080 + 5, //左右放卷工作状态指示 //
	HC_PLC_OFFLOAD_A_STATU = 22119, //A收卷工作状态指示 //
	HC_PLC_OFFLOAD_B_STATU = 22120, //A收卷工作状态指示 //
	HC_PLC_ONLOAD_DIRFORWARD = 22087,//106608 + 7, //放卷正转
	HC_PLC_ONLOAD_DIRBACK = 22080 + 8, //放卷反转
	HC_PLC_OFFLOAD_DIRFORWARD = 22080 + 9, //收卷正转
	HC_PLC_OFFLOAD_DIRBACK = 22080 + 10, //收卷反转 
	HC_PLC_BRUSH_TEST = 22080 + 11, //毛刷手动测试
	HC_PLC_LOCATION_TEST = 22080 + 12, //手动定位测试
	//HC_PLC_MAINCLY = 22080 + 13, //主牵引气缸
	HC_PLC_MAINCLY_PRESS = 19251,//主牵引气缸下压
	HC_PLC_MAINCLY_RELESAE = 19252,//主牵引气缸释放
	HC_PLC_EXTERNAL_SAFETYDOOR = 22080 + 14, //外部安全门
	HC_PLC_LASER_SAFETYDOOR = 22080 + 15, //激光安全门
	HC_PLC_LASER1_LETHE_EN = 22096, //激光1漏切使能
	HC_PLC_LASER2_LETHE_EN = 22096 + 1, //激光2漏切使能
	HC_PLC_PROTJUMP_EN = 22096 + 2, //接口自动跳过
	HC_PLC_PROTCHECK_EN = 22096 + 3, //接口检测
	HC_PLC_AUTOCORRECT_POINT = 22096 + 4, //自动纠偏定位
	HC_PLC_RESETAXIS_POINT = 22096 + 5, //复位时电机定位
	HC_PLC_ONLINE_EN = 22096 + 6, //起始边使能
	HC_PLC_LASER1_EN = 22096 + 7, //激光1使能
	HC_PLC_LASER2_EN = 22096 + 8, //激光2使能
	HC_PLC_COMPUTERCARD = 22108, //脱机卡
	HC_PLC_MAINAXIS_DIRFORWARD = 16268,//22105, //主牵引正转
	HC_PLC_MAINAXIS_DIRBACK = 16269,//22106, //主牵引反转
	HC_PLC_CONNECT_LOAD_DOWN = 19213, //MX 接料下压
	HC_PLC_LASER1_UPDATA_END = 22109, //激光上传完成
	HC_PLC_LASER2_UPDATA_END = 22110, //激光上传完成
	HC_PLC_LEFT_ONLOADDIR_SEL = 22115, //左放卷方向(0逆时针，1顺时针)
	HC_PLC_RIGHT_ONLOADDIR_SEL = 22116, //右放卷方向(0逆时针，1顺时针)
	HC_PLC_ONLOAD_TAPESPLICING_TRIGGER = 22117, //放卷接带触发
	HC_PLC_OFFLOAD_TAPESPLICING_TRIGGER = 22118, //收卷接带触发
	HC_PLC_COUNTER_TEST = 22122, //计数器测试
	//计数器状态 MX 2764 MW 1382
	HC_PLC_MAIN_COUNTER_STATUS = 22123,
	HC_PLC_LASER1_COUNTER_STATUS = 22124,
	HC_PLC_LASER2_COUNTER_STATUS = 22125,
	HC_PLC_MEASURELENGTH_COUNTER_STATUS = 22126,
	HC_RISINGEDGE = 22127,//上升沿触发下降沿触发(0下降沿)
	HC_PLC_HARDWARE_RESET_TRIGGER = 22128, //硬件复位触发
	HC_PLC_AUTOSTOP_ENABLE = 22128 + 1, //自动停机使能
	HC_PLC_AUTO_JUMP_JOINTING = 22128 + 2, //接口跳过处理中
	HC_PLC_STRENG_ENABLE = 22128 + 3, //加强筋使能

	/*数据设置或显示数据 MD*/
	HC_PLC_MAINAXISPOS = 700, //牵引位置 
	HC_PLC_WORKPOS = 701, //工作位置
	HC_PLC_WORKSPEED = 702, //工作速度
	//HC_PLC_BRUSH_SPEED = 703, //毛刷速度
	HC_PLC_LASER1_STEP = 704, //激光1步数
	HC_PLC_LASER2_STEP = 705, //激光2步数
	HC_PLC_LASER1_CURYIELD = 706, //激光1当班产量
	HC_PLC_LASER2_CURYIELD = 707, //激光2当班产量
	HC_PLC_LASER_CURSUMYIELD = 708, //当班总产量
	HC_PLC_LASER1_SUMYIELD = 709, //激光1总产量
	HC_PLC_LASER2_SUMYIELD = 710, //激光2总产量
	HC_PLC_LASER_SUMYIELD = 711, //激光总产量
	HC_PLC_ONLOAD_ARMPOS = 712, //放卷力臂位
	HC_PLC_OFFLOAD_ARMPOS = 713, //收卷力臂位
	HC_PLC_ONLOAD_TENSION = 714, //放卷张力
	HC_PLC_OFFLOAD_TENSION = 715, //收卷张力
	HC_PLC_ONLOAD_DIAMETER = 716, //放卷卷径
	HC_PLC_OFFLOAD_DIAMETER = 717, //收卷卷径
	HC_PLC_MAIN_COUNTER = 718, //主计数器
	HC_PLC_LASER1_COUNTER = 719, //激光1计数器
	HC_PLC_LASER2_COUNTER = 720, //激光2计数器
	HC_PLC_TOLASER1_DIS = 721, //到激光1的距离
	HC_PLC_TOLASER2_DIS = 722, //到激光2的距离
	HC_PLC_START_AIR = 723, //起始风量
	HC_PLC_RUN_AIR = 724, //运行风量
	HC_PLC_PROT_TO_LASER2_DIS = 725, //接口道激光2的距离
	HC_PLC_JUMP_LASER2_DIS = 726, //接口跳过激光2的距离
	HC_PLC_STRAT_ONLOAD_SPEED = 727, //初始导料速度
	HC_PLC_START_ONLAOD_DIS = 728, //初始导料距离
	HC_PLC_ONLOAD_TENSION_RATIO = 729, //放卷张力比率
	HC_PLC_OFFLOAD_TENSION_RATIIO = 730, //收卷张力比率
	HC_PLC_PULLTAPER = 731, //张力锥度
	HC_PLC_PULLTAPER_ADD = 732, //锥度补偿
	HC_PLC_ONLOAD_TENSION_RATIO_P = 733, //放卷张力比例值
	HC_PLC_ONLOAD_TENSION_RATIO_I = 734, //放卷张力微分值
	HC_PLC_ONLOAD_TENSION_UPVAL = 735, //放卷张力上限值
	HC_PLC_ONLOAD_TENSION_DOWNVAL = 736, //放卷张力下限值
	HC_PLC_ONLOAD_TENSION_POSIE = 737, //放卷张力平衡值
	HC_PLC_ONLOAD_TENSION_RESET = 738, //放卷张力复位值
	HC_PLC_OFFLOAD_TENSION_RATIO_P = 739, //收卷张力比例值
	HC_PLC_OFFLOAD_TENSION_RATIO_I = 740, //收卷张力微分值
	HC_PLC_OFFLOAD_TENSION_UPVAL = 741, //收卷张力上限值
	HC_PLC_OFFLOAD_TENSION_DOWNVAL = 742, //收卷张力下限值
	HC_PLC_OFFLOAD_TENSION_POSIE = 743, //收卷张力平衡值
	HC_PLC_OFFLOAD_TENSION_RESET = 744, //收卷张力复位值
	HC_PLC_TO_MIDDLE_DIS = 745, //到中心线的距离
	HC_PLC_COATFILMWIDTH = 746, //涂布宽度
	HC_PLC_INCISEWIDTH = 747, //模切宽度
	HC_PLC_ONLOADDIMRATIO_LEFT = 749, //卷径调试
	HC_PLC_ONLOADDIMRATIO_RIGHT = 750,
	HC_PLC_OFFLOADDIMRATIO_LEFT = 751,
	HC_PLC_OFFLOADDIMRATIO_RIGHT = 752,
	HC_PLC_BRUSH1_SPEED_RATIO = 753, //毛刷速度
	HC_PLC_BRUSH2_SPEED_RATIO = 754, //毛刷速度
	HC_PLC_STRENG_SPEED_RATIO = 755, //加强筋速度比率
	HC_PLC_WORKSPEED_FEEDBACK = 756, //工作速度反馈
	HC_PLC_LASER1_COMP = 757, //激光比较值
	HC_PLC_LASER2_COMP = 758,
	HC_PLC_FILMDIMCMP = 759, //极片长度 
	HC_PLC_LASER1_CUTOVER_OFFSET = 760, //激光1切入偏移
	HC_PLC_LASER1_CUTOUT_OFFSET = 761, //激光1切出偏移
	HC_PLC_LASER2_CUTOVER_OFFSET = 762, //激光2切入偏移
	HC_PLC_LASER2_CUTOUT_OFFSET = 763, //激光2切出偏移
	HC_PLC_FILMDIMCMP_COUNTER = 764, //极片测长计数器
	 //产品优率Qualified
	HC_PLC_QUALIFIED = 765,
	HC_PLC_DISQUALIFIED = 766,
	HC_PLC_ENCODERATIO = 767,//编码器比率
	HC_PLC_SUMLENGTH_CPS = 768,//总长度补偿
	HC_PLC_STOPMINDIMSET = 769, //停机最小卷径
	HC_PLC_FILM_INTERVAL = 770, // 极片测长误差区间
	HC_PLC_STRENG_SPEED_RATIO_BACK = 771,
	HC_PLC_SUM_OUTPUT    = 772,

	/*极耳间距数组*/
	HC_PLC_FILM_STEPLONGTH = 1500, //极耳间距数组 //MD 
	HC_PLC_FILM_STEP_COUNT = 1100, //极耳个数 MD 
	HC_PLC_FILM_GROUP_TO_GROUP = 1102,

	HC_PLC_MEASURETEST_ADDR = 801,//WORD //风量调节

	HC_PLC_PLCVERSION_F = 1103, //PLC Version MD
	HC_PLC_PLCVERSION_M1 = 1104,
	HC_PLC_PLCVERSION_M2 = 1105,
	HC_PLC_PLCVERSION_L = 1106,
	HC_PLC_FILM_WIDTH = 1107, //MD 极耳宽度

	HC_PLC_CURPACKAGENAME = 1110, //当前产品名称

	//报警
	HC_PLC_ERROR_STATE_CODE1 = 695,//DWORD 
	HC_PLC_ERROR_SATAE_CODE2 = 696,
	HC_PLC_ERROR_STATE_CODE3 = 697,
};

const CString strMachineErrorCode1[32] =
{
	_T("放卷压带汽缸报警"),//MX2780
	_T("放卷升降汽缸报警"), //.1
	_T("放卷横移汽缸报警"), //.2
	_T("放卷左切刀汽缸报警"), //.3
	_T("放卷右切刀汽缸报警"), //.4
	_T("激光一吸尘移动汽缸报警"), //.5
	_T("激光二吸尘移动汽缸报警"),  //.6
	_T("展平压紧汽缸报警"), //.7
	_T("测长压紧汽缸报警"), //.8
    _T("加强筋压紧汽缸报警"), //.9
	_T("主牵引压紧汽缸报警"), //.10
	_T("毛刷除尘汽缸报警"), //.11
	_T("收卷自动压带汽缸报警"), //.12
	_T("收卷自动接带切刀汽缸报警"), //.13
	_T("收卷自动接带汽缸报警"), //.14
	_T("收卷拖带汽缸报警"), //.15
	_T("收卷推板汽缸报警"), //2782
	_T("放卷接带左下真空报警"), //.1
	_T("放卷接带右下真空报警"),//.2
	_T("激光1吸尘真空报警"), //.3
	_T("激光1真空辊真空报警"), //.4
	_T("激光1切割前真空报警"),  //.5
	_T("激光2吸尘真空报警"), //.6
	_T("激光2真空辊真空报警"), //.7
	_T("激光2切割前真空报警"), //.8
	_T("CCD压紧气缸报警"), //.9
	_T("放卷平台气缸位置异常"), //.10
	_T("放卷纠偏极限报警"), //.11
	_T("二次过程纠偏极限报警"), //.12
	_T("备用报警11"), //.13
	_T("备用报警12"), //.14
	_T("备用报警13"), //.15
};

const CString strMachineErrorCode2[32] =
{
	_T("左放卷伺服报警"),// 2784
	_T("右放卷伺服报警"),//.1
	_T("主牵引伺服报警"), //.2
	_T("左推料伺服报警"),
	_T("右推料伺服报警"),
	_T("压平轴伺服报警"),
	_T("展平1轴伺服报警"),
	_T("展平2轴伺服报警"),
	_T("收卷1轴伺服报警"),
	_T("收卷2轴伺服报警"),
	_T("收卷推料轴伺服报警"),
	_T("上加强筋轴伺服报警"),
	_T("下加强筋轴伺服报警"),
	_T("激光1轴伺服报警"),
	_T("激光2轴伺服报警"),
	_T("备用报警14"),
	_T("放卷纠偏电机报警"),
	_T("激光1毛刷电机报警"),
	_T("激光2毛刷电机报警"),
	_T("过程整体纠偏电机报警"),
	_T("过程传感器纠偏电机报警"),
	_T("加强筋上调宽电机报警"),
	_T("加强筋下调宽电机报警"),
	_T("左电缸步进电机报警"),
	_T("右电缸步进电机报警"),
	_T("备用报警15"),
	_T("备用报警16"),
	_T("备用报警17"),
	_T("备用报警18"),
	_T("备用报警19"),
	_T("备用报警20"),
	_T("备用报警21"),
};

const CString strMachineErrorCode3[21] =
{
	_T("激光内门打开报警"),
	_T("激光前大门打开报警"),
	_T("激光后大门打开报警"),
	_T("左电柜门打开报警"),
	_T("右电柜门打开报警"),
	_T("左电柜下门打开报警"),
	_T("右电柜下门打开报警"),
	_T("放卷侧门打开报警"),
	_T("放卷前门打开报警"),
	_T("显示器门打开报警"),
	_T("收卷前门打开报警"),
	_T("收卷侧门打开报警"),
	_T("收卷后门打开报警"),
	_T("接带接口检测报警"),
	_T("激光1漏切检测报警"),
	_T("激光2漏切检测报警"),
	_T("放卷摆杆异常报警"),
	_T("收卷摆杆异常报警"),
	_T("放卷双面胶检测报警"),
	_T("CCD掉线报警"),
	_T("CCD_NG报警"),
};

//const CString strMachineWarningCode[32] =
//{
//	_T("放卷未恢复到中点位"), // 139400 //2786
//	_T("收卷未恢复到中点位"), //.1
//	_T("停止过久启动前需要先复位"), //.2
//	_T("牵引速度为0"), //.3
//	_T("有报警未复位"), //.4
//	_T("激光轴未到工作位"),  //.5
//	_T("激光1未准备好"), //.6
//	_T("激光2未准备好"), //.7
//	_T("激光轴2未归零"), //.8
//    _T("产品文件未加载"), //.9
//	_T("接带气缸未升起"), //.10
//	_T("气源压力过低"), //.11
//	_T("激光安全门未关闭"), //.12
//	_T("外部安全门未关闭"), //.13
//	_T("接口感应处理中"), //.14
//	_T("吸尘压力异常"), //.15
//	_T("上追棍电机未归零"), //2788
//	_T("下追棍电机未归零"), //.1
//	_T("激光轴1未归零"), //.2
//	_T("纠偏2传感器距离电机未归零"), //.3
//	_T("纠偏2传感器定位电机未归零"), //.4
//	_T("纠偏3传感器距离电机未归零"), //.5
//	_T("纠偏3传感器定位电机未归零"), //.6
//	_T("纠偏1传感器定位电机未归零"), //.7
//	_T(""),
//	_T(""),
//	_T(""),
//	_T(""),
//	_T(""),
//	_T(""),
//	_T(""),
//	_T(""),
//};

typedef int (*MBTCPCONNECT)(int nport, int IpPort, DWORD IpAddress);
typedef int (*MBTCPCLOSE)(int nport);
typedef int (*MBTCPSETTIMEOUT)(int nport,int nTime);
typedef int (*MBTCPSETDELAY)(int nport,int nTime);
typedef int (*MBTCPFCN01)(int nport, int node, int address, int Count,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN02)(int nport, int node, int address, int Count,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN03)(int nport, int node, int address, int Count,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN04)(int nport, int node, int address, int Count,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN05)(int nport, int node, int address, int Count,int value,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN06)(int nport, int node, int address, int Count,int value,int* RxdBuffer,int* RxdLength);

typedef int (*MBTCPFCN15)(int nport, int node, int address, int Count,WORD* TxdBuffer,int* RxdBuffer,int* RxdLength);
typedef int (*MBTCPFCN16)(int nport, int node, int address, int Count,WORD* TxdBuffer,int* RxdBuffer,int* RxdLength);

class HcModbusTcpClient
{
public:
	HcModbusTcpClient(void);
	~HcModbusTcpClient(void);

	BOOL Init();

	MBTCPCONNECT m_fnConnect;
	MBTCPCLOSE m_fnClose;
	MBTCPSETTIMEOUT m_fnSetTimeOut;
	MBTCPSETDELAY m_fnSetDelay;
	MBTCPFCN01 m_fnMbt01;
	MBTCPFCN02 m_fnMbt02;
	MBTCPFCN03 m_fnMbt03;
	MBTCPFCN04 m_fnMbt04;
	MBTCPFCN05 m_fnMbt05;
	MBTCPFCN06 m_fnMbt06;

	MBTCPFCN15 m_fnMbt15;
	MBTCPFCN16 m_fnMbt16;
	HINSTANCE hInst;
	BOOL Connect();
	BOOL GetMutiWordData(int nAddress,int* ReadData,int nCount=1);
	DWORD GetDWordData(int nAddress);
	WORD GetWordData(int nAddress);
	BOOL SetDWordData(int nAddress,int nValue);
	BOOL SetWordData(int nAddress,int nValue);
	BOOL ReadBit(int nAddress);
	BOOL ReadWBit(int nAddress);
	int ReadInChannel(int nAddressStart,int nCount = 8);
	BOOL WriteBit(int nAddress,BOOL bOn=TRUE);
	BOOL WriteWBit(int nAddress,BOOL bOn=TRUE);

	BOOL ServerConnectIsEnable(CString strIp,int nPort,int nTimeOut = 5);
public:
	int m_nPort;
	BOOL m_bConnected;
	BOOL m_bFirstFailed;
	CString m_strIpAddress;
	DWORD m_dIpAddress;
	DWORD m_dwLastConnectTime;
	CCriticalSection m_readLock;
	BOOL m_bExiting;
};


