#ifndef _NAS_PROTOCOL_H_
#define _NAS_PROTOCOL_H_

#define CMD_NAS_ISCSI                   0x00061000
#define CMD_NAS_FC                      0x00062000
#define CMD_NAS_AUTH                    0x00063000
#define CMD_NAS_SERVICE                 0x00064000
#define CMD_NAS_SHARE                   0x00065000

#define CMD_NAS_ISCSI_TARGET_LOOKUP     0x00061010
#define CMD_NAS_ISCSI_TARGET_ADD        0x00061020
#define CMD_NAS_ISCSI_TARGET_DEL        0x00061030
#define CMD_NAS_ISCSI_MAP               0x00061050
#define CMD_NAS_ISCSI_UNMAP             0x00061060
#define CMD_NAS_ISCSI_SHOWMAP           0x00061070
#define CMD_NAS_ISCSI_SHOWUNMAP         0x00061080
#define CMD_NAS_ISCSI_CHAP_ADD          0x00061090
#define CMD_NAS_ISCSI_CHAP_DEL          0x000610A0
#define CMD_NAS_ISCSI_CHAP_GET          0x000610B0
#define CMD_NAS_ISCSI_GETALLOW          0x000610D0
#define CMD_NAS_ISCSI_SETALLOW          0x000610E0

#define CMD_NAS_FC_LOOKUP               0x00062010
#define CMD_NAS_FC_MAP                  0x00062020
#define CMD_NAS_FC_UNMAP                0x00062030

#define CMD_NAS_AUTH_GET                0x00063010
#define CMD_NAS_AUTH_SET                0x00063020

#define CMD_NAS_AUTH_GROUP_GET          0x00063110
#define CMD_NAS_AUTH_GROUP_ADD          0x00063120
#define CMD_NAS_AUTH_GROUP_DEL          0x00063130
#define CMD_NAS_AUTH_GROUP_SET          0x00063140

#define CMD_NAS_AUTH_USER_GET           0x00063210
#define CMD_NAS_AUTH_USER_ADD           0x00063220
#define CMD_NAS_AUTH_USER_DEL           0x00063230
#define CMD_NAS_AUTH_USER_SET           0x00063240

#define CMD_NAS_QUOTA_GROUP_GET         0x00063310
#define CMD_NAS_QUOTA_GROUP_SET         0x00063320
#define CMD_NAS_QUOTA_USER_GET          0x00063330
#define CMD_NAS_QUOTA_USER_SET          0x00063340

#define CMD_NAS_SHARE_LIST              0x00065010
#define CMD_NAS_SHARE_ADD               0x00065020
#define CMD_NAS_SHARE_DEL               0x00065030
#define CMD_NAS_SHARE_GET               0x00065040
#define CMD_NAS_SHARE_SET               0x00065050

#define CMD_NAS_SERVICE_LIST            0x00064010
#define CMD_NAS_SERVICE_CTRL            0x00064020

#endif //_NAS_PROTOCOL_H_
