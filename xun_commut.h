#ifndef __LOG_COMMUT_H__
#define	__LOG_COMMUT_H__

//#include "MMIDataType.h"
//#include "cJSON.h"

// [dengxu start] add for code managment by features
//#include "xun_features.h"
// [dengxu end]

#define    MAX_IP_LEN           256
#define    MAX_DOMAIN_NAME_LEN  256
#define    MAX_ACCOUNT_LEN      128
#define    MAX_PASSWD_LEN       32
#define    MAX_DEVICE_LEN       128
//#define    SEND_BUFFER_LEN      512
#define    RECV_BUFFER_LEN      36000
#define    VM_MAX_SOCK_ADDR_LEN 28
#define    MAX_EID_LEN          40
#define    MAX_GID_LEN          40
#define    MAX_PHONE_GROUP_MEMBER_COUNT  10
#define    TCP_KEEPALIVE_TIME   30000
#define    TIME_STAMP_LEN       17
#define    E2C_TIME_STAMP_LEN   20
#define CAN_START_FOTA_BATTERY_LEVEL_MIN (65)

// RC definition
enum RESULT_CODE
{
	RC_SUCCESS = 1,
	RC_FAIL    = -1,
	RC_DONE    = 2,
	ERRCODE_SESSION_ILLEGAL = -14,
	ERRCODE_USER_NOT_EXIST = -103,
	ERRCODE_USER_ALREADY_EXIST = -104
};

enum CID_TYPE
{
	CID_REGISTER  =  10231,
	CID_REGISTER_RSP  =  10232,
	CID_DEVICE_LOGIN = 10211,
	CID_DEVICE_LOGIN_RSP = 10212,
	CID_DEVICE_LOGOUT = 10221,
	CID_DEVICE_LOGOUT_RSP = 10222,
	CID_DEVICE_SET = 10251,
	CID_DEVICE_SET_RSP = 10252,
	CID_MF_ADD_FRIEND = 10281,
	CID_MF_ADD_FRIEND_RSP = 10282,
	CID_MF_DEL_FRIEND = 10291,
	CID_MF_DEL_FRIEND_RSP = 10292,

	CID_DEVICE_GETGID = 20081,
	CID_DEVICE_GETGID_RSP = 20082,
	CID_CREAT_GROUP_FACE_TO_FACE = 20121,
	CID_CREAT_GROUP_FACE_TO_FACE_RSP = 20122,
	CID_E2E_UP = 30011,
	CID_E2E_DOWN = 30012,
	CID_E2G_UP = 30031,
	CID_E2G_DOWN = 30032,

	CID_C2E_LIST_MESSAGE_UP = 40161,
	CID_C2E_LIST_MESSAGE_DOWN = 40162,

    CID_C2E_PULL_OFFLINE_MESSAGES_UP = 40181,
    CID_C2E_PULL_OFFLINE_MESSAGES_DOWN = 40182,

	CID_E2C_UP = 40111,
	CID_E2C_DOWN = 40112,
	CID_DOWNLOAD_FILE = 40151,
	CID_DOWNLOAD_FILE_RSP = 40152,
	CID_UPLOAD_TRACE = 50021,
	CID_UPLOAD_TRACE_RSP = 50022,
	CID_DOWNLOAD_TRACE = 50031,      
	CID_UPLOAD_TRACE_EX = 50111,
	CID_UPLOAD_TRACE_TRACKING = 50121,
#ifdef __LC_EFNECE_FEATURE_SUPPORT__	  
      CID_EFENCE_DATA = 51041,
      CID_EFENCE_DATA_RSP = 51042,
#endif	  
	CID_GET_TRACKING_MODE = 53011,
	CID_GET_TRACKING_MODE_RSP = 53012,
#if defined(__XUN_LOC_POLICY_MGR__)
	CID_CHECK_POLICY_STATUS = 54011,
	CID_CHECK_POLICY_STATUS_RSP = 54012,
#endif
	CID_MAPGET = 60021,
	CID_MAPGET_RSP = 60022,
	CID_MAPSET = 60031,
	CID_MAPSET_RSP = 60032,
	/*Add by zhouhansheng for 2015-9-23*/
	CID_MAPMGET = 60051,
	CID_MAPMGET_RSP = 60052,
	CID_MAPSET_MSET = 60061,
	CID_MAPSET_MSET_RSP = 60062,
	CID_SIMINFO_CHANGED = 60151,
	CID_SIMINFO_CHANGED_RSP = 60152,
	CID_BIND_RAND_NUM = 60161,
	CID_BIND_RAND_NUM_RSP = 60162,
	CID_SESSION_PING = 70011,
	CID_SESSION_PING_RSP = 70012,
	CID_GET_TIME = 70021,
	CID_GET_TIME_RSP = 70022,
	CID_REG_WITH_DEFAULT_GROUP = 70031,
	CID_REG_WITH_DEFAULT_GROUP_RSP = 70032,

	CID_E2C4_DEVICE_MSG = 70061,

	CID_STAT_DATA_MSG = 70071,
	CID_E2C4_DEVICE_VOICE_MSG = 70081,
	CID_E2C4_DEVICE_STRUCT_DATA_MSG = 70091,
	//call log sync cid.mayanjun. XUN_CALL_LOG_GET
	CID_SYNC_CALL_LOG_DATA_MSG = 70121,
	//XUN_CONTACT,add by mayanjun.20160407
	CID_SYNC_CONTACTS_DATA_MSG = 70141,
	CID_SYNC_CONTACT_RSP_MSG = 70142,
	CID_DM_UPLOAD_ITEM_INFO_MSG = 70161,
	CID_DM_UPLOAD_ITEM_INFO_MSG_CB = 70162,
	CID_DM_GET_ITEM_INFO_MSG = 70171,
	CID_DM_GET_ITEM_INFO_MSG_CB = 70172,


	//sos powerkey add
	CID_DEVICE_GETGIDINF = 20071,
	CID_DEVICE_GETGIDINF_RSP = 20072,
							   //sos powerkey add end

	//CID_GET_TIME = 80031,
	CID_GET_WEATHER_INFO = 80041,
};

enum SUB_ACTION_TYPE
{
	SUB_ACTION_GET_LOCATION = 100,               //发送e2e到手表获取地址信息
	SUB_ACTION_SET_WATCH_SETTING = 101,  //发送e2e到手表 设置手表
	SUB_ACTION_VOICE = 102,                   //发送e2e到手表 发送语音
	SUB_ACTION_FORCE_RECORD = 103,           //发送e2e到手表 强制录音
	SUB_ACTION_FORCE_CLOSE = 104,               //发送e2e到手表 强制关机
	SUB_ACTION_VOICE_70091 = 105,              //发送e2e到手表语音70091
	SUB_ACTION_GET_TRACKING_LOCATION = 106,
	SUB_ACTION_REAL_TIME_LOCATION = 108,//实时定位
	SUB_ACTION_CONNECTION_MODE = 149,
	SUB_ACTION_SET_LIGHT_BRIGHTNESS = 151, //发送e2e到手表设置指示灯亮度
	SUB_ACTION_SET_NOTICE_VOLUME = 152, //发送e2e到手表设置提示音量
	SUB_ACTION_SET_RECORD_VOLUME = 153, //发送e2e到手表设置语言音量
	SUB_ACTION_SET_SHUTDOWN_DEVICE = 154, //发送e2e到手表关机     0:关机
	SUB_ACTION_SET_AIRPLANE_MODE = 155, //发送e2e到手表设置飞行模式  1:打开   0:关闭
	SUB_ACTION_SET_ACROSS_CITY_REMIDD = 156, //发送e2e到手表设置跨城市提醒
	SUB_ACTION_SET_EMERGENCY_CALL = 157, //发送e2e到手表设置手表紧急呼叫
	SUB_ACTION_SET_WATCH_BUZZER = 158, // 找手机，手机发出警告音
	SUB_ACTION_SET_NOTIFY_STYLE = 159,   // 发送e2e 到手表设置提示方式
	SUB_ACTION_REPORT_BATTERY = 160,         //发送e2e到手机上报电量
	SUB_ACTION_SOS = 161,                     //发送e2e到手机SOS

	//add alert e2e
	SUB_ACTION_NOTICE_ACROSS_CITY_REMIDD = 162,
	SUB_ACTION_NOTICE_SAFEAREA = 163,
	SUB_ACTION_NOTICE_LOWPOWER = 164,
	SUB_ACTION_NOTICE_BATTERY_LEVEL = 165,
	//add by pujiabin for fwatch status notification
	SUB_ACTION_NOTICE_WATCH_STATUS = 166,
	//end by pujiabin

	SUB_ACTION_GET_SILENCE_TIME_RANGE = 170, //发送e2e到手表获取静音时间区间
	SUB_ACTION_SET_SILENCE_TIME_RANGE = 171, //发送e2e到手表设置静音时间区间
	SUB_ACTION_MODIFY_SILENCE_TIME_RANGE = 172, //发送e2e到手表修改静音时间区间
	SUB_ACTION_DELETE_SILENCE_TIME_RANGE = 173,

	//alarmclock add
	SUB_ACTION_MODIFY_ALARMCLOCK_SETTING = 175,	//发送e2e到手表设置闹钟
	SUB_ACTION_MODIFY_ALARMCLOCK_SETTING_EX = 176,
	//alarmclock add
	SUB_ACTION_SET_FLIGHT_MODE_INFO = 177,   // 设置飞航模式时间
	SUB_ACTION_OTA_UPDATE_RESULT = 178,
	// report sim card info change
	SUB_ACTION_REPORT_SIMINFO = 180,

	//watch bind xlk add
	SUB_ACTION_WATCH_BIND = 200,
	SUB_ACTION_VALUE_NAME_GROUP_CHANGE_NOTICE = 202,
	//XUN_CONTACT,add by mayanjun.20160407
	SUB_ACTION_CONTACT_INFO = 205,

    	SUB_ACTION_DL_ITEMS_INFO = 206,
	SUB_ACTION_WATCH_CANCEL_BIND = 220,
	//watch bind xlk add end
	//OTA E2E msg
	SUB_ACTION_WATCH_OTA_MSG = 300,

	SUB_ACTION_TEST_LOCATION = 301,               //location test
	//OTA E2E msg end
	SUB_ACTION_SMS__VOICE = 302,                   //发送e2e到手表 接受语音
	SUB_ACTION_BACKGROUND_CALL = 303,		// background call permit.mayanjun

	SUB_ACTION_FETCH_TODAY_BATLOG_MSG = 304,

	SUB_ACTION_SET_WIHTE_LIST = 500,                   //接受发送白名单
	/*Modify by zhouhansheng  for 2015-11-4*/
	SUB_ACTION_SERVICE_PUSH_MSG = 501,                   //服务器push消息
	SUB_ACTION_REQ_SPORT_INFO = 502,                   //请求运动数据
	SUB_ACTION_REQUEST_CURR_VERSION = 504,


	 SUB_ACTION_REQUEST_WIFI_CONNECT = 505,
	 SUB_ACTION_REQUEST_WIFI_SCAN = 506,
	 SUB_ACTION_REQUEST_WIFI_STATUS = 507,
	 SUB_ACTION_PROGRESS_WIFI_SAVED_PROF = 508,
	 SUB_ACTION_REQUEST_WIFI_DISCONNECT = 509,


#ifdef __LC_EFNECE_FEATURE_SUPPORT__
	SUB_ACTION_EFENCE_DATA = 600,
#endif	     
	SUB_ACTION_REQ_BALANCE_INQUIRY = 601,                   //余额查询

#if defined(__XUN_LOC_POLICY_MGR__)  // for test
	// just for policy test-- start
	SUB_ACTION_LOC_POLICY_CFG = 701,
	SUB_ACTION_LOC_POLICY_READBACK = 702,
	// just for policy test--end
#endif
	SUB_ACTION_SAY_HELLO = 6666,
	SUB_ACTION_TEST_PING_TIME = 9527,
	SUB_ACTION_SMS_TCP_CONNECT = 820,
	SUB_ACTION_STEP_INFO_REQ = 830,
};


// E2E message type
enum E2E_MESSAGE_TYPE
{
	E2E_MSG_TO_SINGLE = 1,
	E2E_MSG_TO_GROUP = 2
};
// [dengxu start] define BIND status

//[dengxu end]
//OTA E2E msg
typedef enum
{
	OTA_MD5_VERIFICATION_ERR = -10,
	OTA_FAILED_BTMAC = -3,
	OTA_FAILED_VERSION = -2,
	OTA_FAILED_BATTERY = -1,
	OTA_SUCCESS = 0,
	OTA_TO_TRIGGER = 1, //DP file has been ready.
} OTA_STATUS_TYPE;
//OTA E2E msg end



typedef enum
{
	VOICE_DATA = 1,
	IMAGE_DATA = 2,


} SUB_DATA_TYPE;
/**  Message key definition begin ****/

//e2c和c2e消息内容中的相关参数
#define PREFIX_GP_E2C_MESSAGE  "GP/"
#define PREFIX_EP_E2C_MESSAGE  "EP/"
#define E2C_SPLIT_MEG  "/MSG/"
#define E2C_SPLIT_NOTICE  "/GPMSG/"
#define E2C_SPLIT_ALERT  "/WARNING/"
#define E2C_TIMESTAMP_WILD "#TIME#"

#define E2C_PL_KEY_TYPE  "Type"         //content类型
#define E2C_PL_KEY_EID  "Eid"           //e2c消息生产者的eid
#define E2C_PL_KEY_DURATION  "Duration" //语音消息持续时间
#define E2C_PL_KEY_CONTENT  "Content"   //content
#define E2C_PL_KEY_LEVEL   "Level"      // 电量
#define E2C_PL_KEY_TIME_STAMP   "Timestamp"      // 时间戳
#define E2C_PL_KEY_TYPE_VOICE  "voice"  //语音消息
#define E2C_PL_KEY_TYPE_TEXT  "text"    //文字消息

#define E2C_PL_KEY_NOTICE_TYPE  "Type"    //通知类消息类型,统一即可，有需要再区分
#define E2C_PL_KEY_WARNNING_TYPE  "Type"    //告警类消息类型

#define E2C_PL_KEY_NOTICE_TYPE_JOIN_GROUP  "NoticeJoin"  //加入群组通知
#define E2C_PL_KEY_NOTICE_TYPE_MERGE_GROUP  "NoticeMerge"  //加入群组通知    
#define E2C_PL_KEY_NOTICE_TYPE_LEAVE_GROUP  "NoticeLeave"    //离开群组通知
#define E2C_PL_KEY_NOTICE_TYPE_EDIT_USER_HEAD  "NoticeEditHead"  //设置用户头像
#define E2C_PL_KEY_NOTICE_TYPE_EDIT_BABY_INFO  "NoticeEditWatchInfo"    //设置baby信息    

#define E2C_PL_KEY_WARNNING_TYPE_SOS  "WarningTypeSOS"    //告警类消息类型SOS
#define E2C_PL_KEY_WARNNING_TYPE_CHANGE_CITY  "Type"    //告警类消息类型切换城市
#define E2C_PL_KEY_WARNNING_TYPE_SAFEAREA  "WarningTypeSafeArea"    //告警类消息类型 出入安全区域
#define E2C_PL_KEY_WARNNING_TYPE_POWER  "WarningTypePower"    //告警类消息类型 低电量

//VerifyCode
#define E2C_PL_KEY_BIND_RAND_NUM  "VerifyCode"


//cloud bridge message公共的key值
#define KEY_NAME_EID  "EID"
#define KEY_NAME_CID  "CID"
#define KEY_NAME_GID  "GID"
#define KEY_NAME_SN   "SN"
#define KEY_NAME_SID  "SID"
#define KEY_NAME_PL    "PL"
#define KEY_NAME_RC   "RC"
#define KEY_NAME_CP   "CP"
#define KEY_NAME_RN   "RN"
#define KEY_NAME_TEID     "TEID"
#define KEY_NAME_TGID     "TGID"
#define KEY_NAME_PERSISTENT  "Persistent"

#define KEY_NAME_PROTOCAL_VER  "Version"


#define KEY_NAME_TYPE   "Type"
#define KEY_NAME_NAME   "Name"
#define KEY_NAME_PASSWORD  "Password"
#define KEY_NAME_NUMBER  "number"
#define KEY_NAME_PHONE_NUM "phone_num"
#define KEY_NAME_CALL_TYPE   "type"
#define KEY_NAME_SUB_NUMBER   "sub_nmuber"
#define KEY_NAME_PHONE_INCOMING_RING   "ring"
#define KEY_NAME_ATTRI   "attri"
#if 1 //def XUN_CALL_LOG_GET
#define KEY_NAME_CLOG_ID	"log_id"
#define KEY_NAME_CLOG_NAME	"name"
#define KEY_NAME_CLOG_NUMBER	"number"
#define KEY_NAME_CLOG_TD_ARRAY	"td_array"
#define KEY_NAME_CLOG_TD_ITEM		"td_item"
#define KEY_NAME_CLOG_DURATION	"duration"
#define KEY_NAME_CLOG_TS	"timestamp"
#define KEY_NAME_CLOG_TOTAL	"total"
#endif
#define TYPE_WATCH        (200)

//location 相关
#define KEY_NAME_ACCESSTYPE  "accesstype"
#define KEY_NAME_SMAC  "smac"
#define KEY_NAME_SERVERIP  "serverip"
#define KEY_NAME_CMDA  "cdma"
#define KEY_NAME_IMSI  "imsi"
#define KEY_NAME_IMEI  "imei"
#define KEY_NAME_GPS  "gps"
#define KEY_NAME_NETWORK  "network"
#define KEY_NAME_TEL  "tel"
#define KEY_NAME_BTS  "bts"
#define KEY_NAME_NEARBTS  "nearbts"
#define KEY_NAME_MMAC  "mmac"
#define KEY_NAME_MAC  "macs"
#define KEY_NAME_TIMESTAMP  "timestamp"
#define KEY_NAME_DROP "drop"
/*Modify by zhouhansheng  for 2015-11-13*/
#define KEY_NAME_STEPS "Steps"
#define KEY_NAME_HEALTH_INFO "health_info"
#define KEY_NAME_CUR_STEPS "cur_steps"
//setting 相关
#define KEY_NAME_SETTING_LEVEL  "Level"
#define KEY_NAME_SETTING_KEY  "Key"
#define KEY_NAME_SETTING_BAT_LEVEL "battery_level"
#define KEY_NAME_SETTING_VOL_LEVEL "volume_level"
#define KEY_NAME_SETTING_ALARM_VOL_LEVEL "alarm_volume_level"
#define KEY_NAME_SETTING_RING_VOL_LEVEL "ring_volume_level"
#define KEY_NAME_SETTING_NOTIFY_STYLE "volumevibrate"
#define KEY_NAME_SETTING_OUT_CALL_ENABLED  "tel_admit"
#define KEY_NAME_SETTING_INCOMINGCALL_ATUO_ANSWER  "auto_answer"

#define KEY_NAME_SETTING_PHONEBOOK_LIST "phone_book_list"

/*Modify by zhouhansheng  for 2015-11-4*/
#define KEY_NAME_SETTING_WHITE_LIST "phone_white_list"
#define KEY_NAME_SETTING_UPDATE_TIME "update_time"
#define KEY_NAME_SETTING_LAST_SETTINGS_TIME "old_update_time"
//#define KEY_NAME_ALARM_ONOFF  "onoff"
/*Modify by zhouhansheng  for 2015-11-4*/
#define KEY_NAME_SETTING_KEYS  "Keys"
#define SYNC_DATA_CONNECTION_MODE  "operation_mode_value"
#define SYNC_DATA_SLEEP_MODE  "SleepList"
//alarmclock add
#define KEY_NAME_SETTING_ALARMCLOCK "AlarmClockList"
#define KEY_NAME_SETTING_ALARMCLOCK_RET "alarm_mark"
#define KEY_NAME_SETTING_ALARMCLOCK_HOUR "hour"
#define KEY_NAME_SETTING_ALARMCLOCK_MIN	"min"
#define KEY_NAME_SETTING_ALARMCLOCK_DAYS	"days"
#define KEY_NAME_SETTING_ALARMCLOCK_ONOFF	"onoff"
#define KEY_NAME_SETTING_ALARMCLOCK_TIMERID	"timeid"
#define KEY_NAME_SETTING_ALARMCLOCK_NAME	"name"
#define KEY_NAME_SETTING_ALARMCLOCK_BELL	"bell"
//alarmclock add end

#define KEY_NAME_SETTING_LED_LIGHT "Led_light"
#define KEY_NAME_SETTING_BUZZ_ALART "Buzz_alart"
#define KEY_NAME_SETTING_STEP_RECORD "setps_setting"
#ifdef OFFLINE_STEP_GOALS_NOTIFICATION
#define KEY_NAME_SETTING_STEP_TARGET "steps_target_level"
#endif
#define KEY_NAME_PL_KEY  "Key"
#define KEY_NAME_STATUS  "status"
#define KEY_NAME_CALLER_ID  "callerid"
#define KEY_NAME_CELL_SIGNAL_LEVEL  "signal_level"
#define KEY_NAME_WLAN_STATUS  "wlan_status"

#ifdef XUN_CONTACT_ANY_CALL
#define KEY_NAME_SETTING_ANY_CALL "white_list_on"
#endif
//OTA E2E MSG
#define KEY_NAME_WATCH_VERSION "VersionCur"
#define KEY_NAME_WATCH_TARGET_VERSION "VersionTarget"
#define KEY_NAME_WATCH_BLT_MAC "BtMac"
#define KEY_NAME_PHONE_BT_MAC "PhoneBtMac"

//OTA E2E MSG end

// Key name for auto GPRS FOTA.
#define KEY_NAME_WATCH_AUTO_UPGRADE "watch_auto_upgrade"

//watch status start 1- power on/2- power off/3- flight mode
#define KEY_NAME_WATCH_STATUS "watch_status"
//watch status end
#define KEY_FLIGHT_MODE_LIST  "FlightMode"

#define TIMEID  "timeid"
#define STARTHOUR  "starthour"
#define STARTMIN  "startmin"
#define ENDHOUR  "endhour"
#define ENDMIN  "endmin"
#define DAYS  "days"
#define ONOFF  "onoff"
#define KEY_SILENCE_LIST  "SilenceList"
//advanced_operation_support,mayanjun.20160310
#define ADVANCEOP	"advanceop"
#define ADVANCEOP_PHONE	"advanceop_phone"
//end
#define KEY_CONNECTION_MODE  "mode_value"

//子操作类型 ，e2e ， c2e ,等message需要 承载不同的子操作，
#define KEY_NAME_SEID  "SEID"
#define KEY_NAME_OFFLINE  "Offline"
#define KEY_NAME_CONTENT "Content"
#define KEY_NAME_KEY  "KEY"
#define KEY_NAME_NEXT_KEY  "NextKey"
#define KEY_NAME_SIZE  "Size"
#define KEY_NAME_IS_TRUNCATED  "IsTruncated"
#define KEY_NAME_LIST  "List"

#define KEY_NAME_SUB_ACTION  "sub_action"
#if defined(__XUN_LOC_POLICY_MGR__)
// key name for loc policy

#define KEY_NAME_SETTING_WLAN_PROF "ssid_list"


#define KEY_NAME_SETTING_WLAN "wlan_setting"
#define KEY_NAME_SETTING_WLAN_AUTO_CONNECT "auto_connect_wifi"
#define KEY_NAME_SETTING_BAT_MAP "Battery_map"
#define KEY_NAME_POLICY_LIST "policylist"
#define KEY_NAME_POLICY_VERSION "version"
#define KEY_NAME_POLICY_ID "policy_id"
#define KEY_NAME_POLICY_NORMAL_INTERVAL "normal_interval"
#define KEY_NAME_POLICY_SLEEP_INTERVAL "sleep_interval"
#define KEY_NAME_POLICY_FLIGHT_OUT_INTERVAL "flightout_interval"
#define KEY_NAME_POLICY_IGNORE_CELL_THRSHOLD "ignore_cell_thrshold"
#define KEY_NAME_POLICY_CELL_NO_CHANGE_STEPS "cell_no_chg_steps"
#define KEY_NAME_POLICY_CELL_PART_CHANGE_STEPS "cell_part_chg_steps"
#define KEY_NAME_POLICY_NORMAL_MOTION_STEPS "normal_motion_steps"
#define KEY_NAME_POLICY_NORMAL_TO_SLEEP_DURATION "normal_to_sleep_dur"
#define KEY_NAME_POLICY_SLEEP_MOTION_STEPS "sleep_motion_steps"
#define KEY_NAME_POLICY_SLEEP_TO_FLIGHT_STEPS "sleep_to_flight_steps"
#define KEY_NAME_POLICY_SLEEP_DEEPSLEEP_EXIT_STEPS "deepsleep_exit_steps"
#define KEY_NAME_POLICY_SLEEP_TO_DEEPSLEEP_DURATION "to_deepsleep_dur"
#define KEY_NAME_POLICY_SLEEP_TO_FLIGHT_DURATION "to_flight_dur"
#define KEY_NAME_POLICY_FLIGHT_EXIT_STEPS "flight_exit_steps"
#define KEY_NAME_POLICY_POSITIONING_CONTROL "pos_ctrl"
#define KEY_NAME_POLICY_REPORT_FREQ "report_freq"
#ifdef __XUN_LED_ARRAY_SCR_SUPPORT__	//LED_BRIGHT_CAN_SET_
#define KEY_NAME_SETTING_LED_LEVEL "led_level"
#endif


// key name for specified loc mode
#define KEY_NAME_LOC_SPEC_ONOFF   "value"
#define KEY_NAME_LOC_SPEC_FREQ    "freq"
#define KEY_NAME_LOC_SPEC_MODE    "mode"
#define KEY_NAME_LOC_SPEC_ENDTIME "endTime"
// Key name for Make Friends
#define KEY_NAME_MF_BT_DEV_LIST 		 "btIds"
#define KEY_NAME_MF_USER_ID 		     "user_eid"
#define KEY_NAME_MF_OP_TYPE				 "optype"
#define KEY_NAME_MF_CONTACTS_TYPE		 "contactsType"
#define KEY_NAME_MF_USER_GID    		 "user_gid"
#define KEY_NAME_MF_FRIEND_EID    		 "FriendEid"
#endif
#define ACCESS_KEY "74CECB85AE17BB85C56FFA91FE33F6E0"


#define PROTOCOL_VER  "00030000"

/**  Message key definition end****/


//#define ADDR "120.131.1.93"

#define TCP_PORT 7070
#define HTTP_PORT 8080
//#define APN VM_TCP_APN_CMNET
#define BUF_MAX_SIZE 2048
extern int tcp_handle;
extern int http_handle;
extern int FlgTcpWatchStat;
extern int FlgTcpConnect;
extern int FlgTcpCanWrit;

extern char WatchSID[40];
extern char WatchEID[MAX_EID_LEN];
extern char WatchFamiGID[40];

#define FILE_NAME_LEN (100)

typedef struct
{
	char server_domain[MAX_DOMAIN_NAME_LEN];
	char domain_name[MAX_DOMAIN_NAME_LEN];
	char account_id[MAX_ACCOUNT_LEN];
	char password[MAX_PASSWD_LEN];
	char device_id[MAX_DEVICE_LEN];
	char device_name[MAX_ACCOUNT_LEN];
} bc_config_info;

#if 0

typedef struct
{
	U16 http_port;
	U16 rtsp_port;
} mapping_port;



/* Socket address struct */
typedef struct
{
	U16	addr_len;                   /* address length */
	U16	port;                   /* port number */
	VMUINT8	addr[VM_MAX_SOCK_ADDR_LEN]; /* address */
} vm_sockaddr_struct;
#endif


// 手表注册
typedef struct conWatch_Register
{
	char Name[40];
	char SerialNo[40]; //sn
	char AccessKey[40];
	char NickName[40];
	char DateOfBirth[20];
	char SimNo[40];   //iccid
	char Imei[20];
	char Imsi[20];
	char BtMac[20];
	char WifiMac[20];
	char VersionCur[40];
	char VersionOrg[40];
	int Sex;
} conWatch_Register;



typedef struct conWatchInfo
{
	int CID;
	conWatch_Register PL;
} conWatchInfo;



typedef struct conWatch_Register_res
{
	int RC;
	int CID;
} conWatch_Register_res;



// 手表登录
typedef struct conWatch_Login
{
	char Name[40];
	char Password[40];
	int Type;
} conWatch_Login;



typedef struct conWatch_Login_Info
{
	int SN;
	int CID;
	conWatch_Login PL;
} conWatch_Login_Info;



typedef struct conWatch_Login_PL
{
	char EID[MAX_EID_LEN];
} conWatch_Login_PL;



typedef struct conWatch_Login_res
{
	int SN;
	int CID;
	char SID[40];
	int RC;
	conWatch_Login_PL PL;
} conWatch_Login_res;



// 手表登出
typedef struct conWatch_Logout
{
	int CID;
	char SID[40];
} conWatch_Logout;



typedef struct conWatch_Logout_res
{
	int RC;
	int CID;
} conWatch_Logout_res;


typedef struct conWatch_GetGroupID_PL
{
	char EID[MAX_EID_LEN];
} conWatch_GetGroupID_PL;



typedef struct conWatch_GetGroupID_Info
{
	int CID;
	char SID[40];
	conWatch_GetGroupID_PL PL;
} conWatch_GetGroupID_Info;



typedef struct conWatch_GetMainGroupID_ResPL
{
	char GID[40];
} conWatch_GetMainGroupID_ResPL;



typedef struct conWatch_GetMainGroupID_res
{
	int RC;
	int CID;
	conWatch_GetMainGroupID_ResPL PL;
} conWatch_GetMainGroupID_res;


//
typedef struct conWatch_CreatGroupFaceToFacePL
{
	char LocalBtMAC[40];  //ble bt
	char RemoteBtMAC[40];
} conWatch_CreatGroupFaceToFacePL;

typedef struct conWatch_CreatGroupFaceToFace
{
	int CID;
	char SID[40];
	conWatch_CreatGroupFaceToFacePL PL;
} conWatch_CreatGroupFaceToFace;

typedef struct conWatch_CreatGroupFaceToFace_resPL
{
	char GID[40];
} conWatch_CreatGroupFaceToFace_resPL;

typedef struct conWatch_CreatGroupFaceToFace_res
{
	int RC;
	int CID;
	conWatch_CreatGroupFaceToFace_resPL PL;
} conWatch_CreatGroupFaceToFace_res;



// UploadTraceData 消息
typedef struct conWatch_UploadTraceDataPL
{
	char *timestamp;
	int accesstype;
	char *smac;
	char *serverip;
	int cdma;
	char *imsi;
	char *gps;
	char *network;
	char *tel;
	char *bts;
	char *nearbts;
	char *mmac;
	char *macs;
	char *imei;
} conWatch_UploadTraceDataPL;


#if 0
typedef struct conWatch_UploadTraceData
{
	int CID;
	char SID[40];
	char GID[40];
	conWatch_UploadTraceDataPL PL;
} conWatch_UploadTraceData;
#endif


typedef struct conWatch_UploadTraceData_res
{
	int RC;
	int CID;
} conWatch_UploadTraceData_res;


typedef struct conWatch_UploadVoiceData_res
{
	int RC;
	int CID;
	char PL[128];
} conWatch_UploadVoiceData_res;



// DeviceGet
typedef struct conWatch_DeviceGet
{
	int CID;
	char SID[40];
} conWatch_DeviceGet;



typedef struct conWatch_DeviceGet_resPL
{
	char *EID;
	char *Name;
	char *SerinalNo;
	char *NickName;
	int Sex;
	char *DateOfBirth;
	char *SimNo;
	char *Imei;
	char *Imsi;
	char *BtMac;
	char *WifiMac;
	char *VersionCur;
	char *VersionOrg;
	char *CreateTime;
	char *ExpireTime;
} conWatch_DeviceGet_resPL;



typedef struct conWatch_DeviceGet_res
{
	int RC;
	int CID;
	conWatch_DeviceGet_resPL PL;
} conWatch_DeviceGet_res;



// gettime
typedef struct conWatch_GetTime
{
	int CID;
	char SID[40];
} conWatch_GetTime;



typedef struct conWatch_GetTime_resPL
{
	char GMT[40];
} conWatch_GetTime_resPL;



typedef struct conWatch_GetTime_res
{
	int RC;
	int CID;
	conWatch_GetTime_resPL PL;
} conWatch_GetTime_res;

typedef struct sendToCloudData
{
	char len[2];
	char *data;
} sendToCloudData;

typedef struct STRU_FLG_Watch_res
{
	int reg;
	int login;
	int logout;
	int e2e;
	int uptrace;
	int downvoice;
	int gettime;
} STRU_FLG_Watch_res;

#if 0
typedef struct
{
	U16  len;
	BOOL is_encode;
	char *buf;
} conWatch_base64_data_t;
#endif

typedef struct
{
	char type_key[32];
	conWatch_UploadTraceDataPL loc;
#if 0
	char timestamp[32];
	char smac[32];
	char serverip[32];
	char imsi[32];
	char gps[32];
	char network[32];
	char tel[32];
	char bts[32];
	char nearbts[32];
	char mmac[32];
	char macs[32];
	int cdma;
	int accesstype;
#endif
} sos_location;

#if 0
typedef struct
{
	U8 EID[MAX_EID_LEN];
} group_member;

typedef struct
{
	int GID;
	int size;
	group_member *members;
} conWatch_Group_Info;

#endif
typedef struct
{
	int    count;
	char   *eid_list[MAX_PHONE_GROUP_MEMBER_COUNT];
} group_members_t;

#if 0
typedef struct cache_data_t
{
	int     id;
	cJSON  *data;
	struct cache_data_t *next;
} cache_data_t;
#endif

typedef struct
{
	int  sn;
	char  eid[MAX_EID_LEN];
} oppo_info_t;




//lixin add begin
#define SETTING_PATH			"z:\\@setting"
#define OLD_SETTING_PATH		"d:\\setting"
#define SILENCE_RULE_PREFIX     "SILR"
#define WATCH_REG_DATA           "reg.dat"
//[dengxu start] vibration setting
#define NOTIFY_STATUS_CONFIG_FILE       "notify.cfg"
//[dengxu end]

#define XUN_LED_LIGHT_SET_FILE "led_light_set.txt"
#define XUN_BUZZ_ALART_SET_FILE "buzz_alart_set.txt"
#define XUN_STEP_RECORD_SET_FILE "step_record_set.txt"
#ifdef OFFLINE_STEP_GOALS_NOTIFICATION
#define XUN_STEP_TARGET_SET_FILE "step_target_set"
#endif
#define XUN_ALARM_VOL_SET_FILE "alarm_vol_set.txt"
#define XUN_SYS_VOL_SET_FILE "sys_vol_set.txt"
#define XUN_OUT_CALL_ENABLED_SET_FILE "out_call_enabled_set.txt"
#define XUN_INCOMINGCALL_AUTO_ANSWER_SET_FILE "auto_answer.txt"

typedef struct conWatch_Param
{
	char EID[MAX_EID_LEN];
	char NickName[40];
	char SerialNo[40];
	char Imei[20];
	char Imsi[20];
	char BtMac[20];
	char BleMac[20];
	char WifiMac[20];
	char VersionCur[40];
	char VersionOrg[40];
	int battery;
	int volume;
	int notify_type;
} conWatch_Param;

typedef struct conWatch_DevSet_Info
{
	int CID;
	char SID[40];
	conWatch_Param PL;
} conWatch_DevSet_Info;

typedef struct
{
	int  sn;
	char  eid[MAX_EID_LEN];
	int  gps_start_tik;
	int  location_type;
	int  regular;
	int  time_interval;
} location_test_struct;




#endif


