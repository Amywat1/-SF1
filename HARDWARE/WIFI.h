#ifndef _WIFI_H
#define _WIFI_H

/*所有命令（不允许修改值）*/
typedef enum {
	/*设备给APP发的指令*/
	CMD_KX_APP_HANDS					= 0xCCC0,			//握手指令
	CMD_KX_APP_SMART_LINK				= 0xCCC2,			//Smart Link指令
	CMD_KX_APP_FIRMWARE_UPGRADE			= 0xCCC5,			//固件升级指令
	CMD_KX_APP_GET_STATUS				= 0xCCC7,			//获取wifi状态
	CMD_KX_APP_RESTART_WIFI				= 0xCCC9,			//重启模块
	CMD_KX_APP_REMOVE_LINK				= 0x6001,			//清除设备绑定
	CMD_KX_APP_REPORT_STATUS			= 0X5000,			//设备主动上报状态
	CMD_KX_APP_KEY_DATA					= 0X6003,			//设备上报按键行为状态

	/*APP给设备发的指令*/
	CMD_APP_KX_STOP_WORK				= 0X00B1,			//APP下取消工作
	CMD_APP_KX_QUERY_STATUS				= 0X00B2,			//APP查询设备状态
	CMD_APP_KX_START_MENU				= 0X00B5,			//App启动菜单
	CMD_APP_KX_LAMP_ENABLE				= 0X1000,			//APP开关炉灯
	CMD_APP_KX_ROT_ENABLE				= 0X1001,			//APP开关转叉
	CMD_APP_KX_MODIFY_PARAMETER			= 0X1002,			//App参数修改
	CMD_APP_KX_SKIP_PRE_HEAT			= 0X1004,			//APP预热跳过
	CMD_APP_KX_PRE_HEAT_KEEP			= 0X1005,			//APP预热保持
	CMD_APP_KX_SNAPSHOT					= 0X820B,			//APP通知设备准备抓图
	CMD_APP_KX_ERR_DATA					= 0X820A,			//设备启动智能烘焙

	/*wiif模组给设备发的指令*/
	CMD_WIFI_KX_CAMMER_STATUS			= 0X2000,			//WIFI模组下发摄像头状态信息

}COMMAND_TypeDef;

/*wifi的状态类型（不允许修改值）*/
typedef enum {
	TRY_CONNET_ROUTER			= 0x00,						//未联网状态正在尝试连接路由器（指示灯灭）
	CAN_NOT_CONNET_SERVER,									//无法连接服务器（指示灯同 02 状态）
	CONNET_ROUTER_OK,										//已连接路由器（指示灯慢闪烁 0.5s 亮 0.5s 灭）
	CONNET_SERVER_OK,										//已连接服务器（指示灯常亮）
	SMART_LINK_OK_WAIT,										//已进入 SmartLink 等待 App 配置(指示灯急闪0.1s 亮 0.1s 灭)
	RESERVE_05,												//预留
	RESERVE_06,												//预留
	SMART_LINK_OK,											//SmartLink 配置成功（指示灯常亮同上 02状态）
	SOFT_AP_STATUS,											//SoftAP 状态(同 04 状态：指示灯急闪 0.1s 亮 0.1s灭)

}WIFI_STATUS_TypeDef;

/*允许读取的WIfi信息*/
typedef enum {
	WIFI_STATUS							= 0x70,				//wifi模组状态
	
}WIFI_INF_TypeDef;

void InitWifiUartIo(void);
void InitWifiVariable(void);
void InitWifiUart(void);
void WifiDealFunction(void);

void SendCommand(COMMAND_TypeDef cmd);
unsigned char ReadWifiInf(WIFI_INF_TypeDef inf);

#endif